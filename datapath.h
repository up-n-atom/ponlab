// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2025, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_H
#define DATAPATH_H
#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/skbuff.h>	/*skb */
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/platform_device.h>
#include <linux/atomic.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <uapi/linux/if.h> /* for IFNAMSIZ */

#if IS_ENABLED(CONFIG_QOS_MGR)
#include <net/qos_mgr/qos_mgr_tc_hook.h>
#endif
#include <linux/percpu.h>
#include <linux/version.h>

#if IS_ENABLED(CONFIG_SOC_LGM) || IS_ENABLED(CONFIG_SOC_PRX)
	/* 5.15 */
	#include <net/mxl_cbm_api.h>
#elif IS_ENABLED(CONFIG_PRX300_CQM) || IS_ENABLED(CONFIG_GRX500_CBM)
	/* 4.9 */
	#include <net/lantiq_cbm_api.h>
#else
	/* 4.19? */
	#include <net/intel_cbm_api.h>
#endif

#define dp_vlan_dev_priv vlan_dev_priv
#include <linux/pp_qos_api.h>
#if IS_ENABLED(CONFIG_INTEL_CBM_SKB) || \
	IS_ENABLED(CONFIG_MXL_CBM_SKB) || \
	LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0)
	#define DP_SKB_HACK
#endif
#include <net/datapath_api_qos.h>
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
#include <net/switchdev.h>
#endif
#include "datapath_swdev.h"
#include <net/datapath_inst.h>
#include <net/datapath_api_umt.h>
#ifdef DP_SWITCHDEV_NO_HACKING
/* Note: this header file just to let DPM compilation pass with new linux kernel
 *       without any switchdev related kernel hacking yet
 */
#include <net/datapath_switchdev_no_hack.h>
#endif
#include "datapath_gdb_wrapper.h"

/* Max value for the limit of MAC addresses per bridge port. */
#define GSWIP_LEARN_LIMIT_PORT_MAX	254

/* Max value for the CQM IGP ports 256 has put for timebeing*/
#define DP_MAX_CQM_IGP			256

#define DP_DEBUGFS_PATH "/sys/kernel/debug/dp"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define call_rcu_bh call_rcu
#endif /* LINUX_VERSION_CODE */
#define DP_NOT_USE_NETDEV_REGISTER 1
#define DP_OPS_HACK 1 /* be default to enable it for PPA to hack ndo_xxx in some ops */

/* DPM build bug on for compile time assert on condition*/
#define DPM_BUILD_BUG_ON(cond, msg) typedef u8 msg[(cond) ? -1:0]
/* Instead of BUG_ON(), we defined our own, to use under dis-optimization*/
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
#define DPM_BUG_ON(cond, ...) do {\
	if (cond) { \
		int *null_ptr = NULL; \
		pr_err("\nDPM: %s %d trigger manual panic here\n\n", __func__, \
			__LINE__); \
		dump_stack(); \
		*null_ptr = 1; \
	} \
} while (0)
#else
#define DPM_BUG_ON(...)
#endif

#define TOE_TC_DEF_VALUE -1 /*default tc value for toe*/

struct dp_gswip {
	struct timer_list	timer;
	unsigned long		ageout;
	u32			version;
	u32			features;
	/* These bits are different on NetCP NU Switch ALE */
	u32			port_mask_bits;
	u32			port_num_bits;
	u32			vlan_field_bits;
	unsigned long		*p0_untag_vid_mask;
};

/* Note: below macro should equal or bigger than HW real capability
 *       since it is used for array size
 */
#define MAX_SUBIFS 256 /* Max subif per DPID */
#define MAX_DP_PORTS 16
#define PMAC_CPU_ID 0
#define DP_MAX_BP_NUM 128
/* BP 0/1 are internal reserved for PMAC 1/2 */
#define MAX_BP_AVAIL_NUM (DP_MAX_BP_NUM -2)

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
#define DP_MAX_PPV4_PORT 128
#define DP_MAX_QUEUE_NUM 256
#else
#define DP_MAX_PPV4_PORT 256
#define DP_MAX_QUEUE_NUM 512
#endif
#define DP_MAX_CQM_DEQ DP_MAX_PPV4_PORT
#define DP_MAX_NODES 2048 /* Maximum PPV4 nodes */

#define MAX_Q_PER_PORT          65 /* Maximum queue per port */

/* CPU path CTP usage */
#if IS_ENABLED(CONFIG_PRX300_CQM)
/* Number of CTP for Normal CPU path. */
#define CPU_CTP_NUM CQM_MAX_CPU

/* Number of PP_NF Special connnections.
 * Note: each special connection reserve 1 CTP only
 */
#define MAX_PP_NF_CNT    0

/* Number of CTP for special connection excluding PP_NF */
#define CTP_OTHER_SPL_CONN_NUM 0
#else
/* Number of CTP for Normal CPU path: 2 CTP/subif per CPU */
#define CPU_CTP_NUM (CQM_MAX_CPU * 2)

/* Number of PP_NF Special connnections.
 * Note: each special connection reserve 1 CTP only
 */
#define MAX_PP_NF_CNT    6

/* Number of CTP for other special connection excluding PP_NF.
 * Note: make sure DP_SPL_PP_NF is the last valid entry in enum DP_SPL_TYPE
 */
#define CTP_OTHER_SPL_CONN_NUM (DP_SPL_PP_NF -1)

#endif
/* Number of CTP for all CPU path, including normal cpu + special connection */
#define ALL_CPU_CTP_NUM (CPU_CTP_NUM + CTP_OTHER_SPL_CONN_NUM + MAX_PP_NF_CNT)
/* DP_EMULATE_MAX_NODES will be passed via ccflag */
#ifdef DP_EMULATE_MAX_NODES
#undef DP_MAX_NODES
#define DP_MAX_NODES DP_EMULATE_MAX_NODES
#endif
#define DP_PLATFORM_DE_INIT BIT(0)

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DISABLE_OPTIMIZE)
#define DP_NO_OPTIMIZE_PUSH \
	_Pragma("GCC push_options") \
	_Pragma("GCC optimize (\"O0\")")
#define DP_NO_OPTIMIZE_POP _Pragma("GCC pop_options")
#else
#define DP_NO_OPTIMIZE_PUSH
#define DP_NO_OPTIMIZE_POP
#endif

#define MAX_TC_ID 15 /* maximum traffic class ID */
#define GSWIP_VER_REG_OFFSET 0x013

#define SWDEV_PARENT_ID(inst, fid) ((u32)(inst) << 24 | (fid))
#define UP_STATS(atomic) atomic_add(1, &(atomic))

#define STATS_GET(atomic) atomic_read(&(atomic))
#define STATS_SET(atomic, val) atomic_set(&(atomic), val)
#define DP_CB(i, x) dp_port_prop[i].info.x

#define dp_set_val(reg, val, mask, offset) do {\
	(reg) &= ~(mask);\
	(reg) |= (((val) << (offset)) & (mask));\
} while (0)

#define dp_get_val(val, mask, offset) (((val) & (mask)) >> (offset))

#define DP_DEBUG_ASSERT(expr, fmt, arg...)  do { if (expr) \
	pr_err(fmt, ##arg); \
} while (0)

#define SET_BP_MAP(x, ix) (x[(ix) / 16] |= 1 << ((ix) % 16))
#define GET_BP_MAP(x, ix) ((x[(ix) / 16] >> ((ix) % 16)) & 1)
#define UNSET_BP_MAP(x, ix) (x[(ix) / 16] &= ~(1 << ((ix) % 16)))

extern u64 dp_dbg_flag;
extern u64 dp_dbgfs_flag;
extern u32 dp_dbg_err;
extern int dp_dbg_mode;
extern u8 g_toe_disable;
extern char *log_buf;
extern int log_buf_len;

void dp_trace_pr(unsigned long ip, const char *fmt, ...);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
/* DP_DUMP no __func__ inserted */
#define DP_DUMP(fmt, arg...) do { \
	if (!dp_dbg_mode) \
		pr_info(fmt, ##arg); \
	else \
		dp_trace_pr(_THIS_IP_, fmt, ##arg); \
} while (0)

/* DP_INFO will insert "DPM: __func__" */
#define DP_INFO(fmt, arg...) DP_DUMP("DPM: %s: " fmt, __func__, ##arg)

/* _DP_DEBUG will not insert "DPM: __func__:" with specified print catagory */
#define _DP_DEBUG(flags, fmt, arg...)  do { \
	if (unlikely(dp_dbg_flag & (flags))) { \
		DP_DUMP(fmt, ##arg); \
	} \
} while (0)

/* DP_DEBUG will insert "DPM: __func__:" with specified print catagory */
#define DP_DEBUG(flags, fmt, args...) \
	_DP_DEBUG(flags, "DPM: %s: " fmt, __func__, ##args)

#else /* CONFIG_DPM_DATAPATH_DBG */
#define DP_INFO(fmt, arg...)
#define DP_DUMP(fmt, arg...)
#define DP_DEBUG(flags, fmt, arg...)
#define _DP_DEBUG(flags, fmt, arg...)
#endif /* end of CONFIG_DPM_DATAPATH_DBG */

extern u32 reinsert_deq_port[DP_MAX_INST];
extern bool dp_mod_exiting;
extern int dflt_cpu_vap[DP_MAX_INST];
extern int dflt_q_cpu[DP_MAX_INST];
extern int dflt_q_toe[DP_MAX_INST];
extern int dflt_q_re_insert[DP_MAX_INST];
extern int dflt_q_drop[DP_MAX_INST];
extern int dflt_bp[DP_MAX_INST];

static inline bool is_invalid_port(int port_id)
{
	if ((port_id < 0) || (port_id >= MAX_DP_PORTS)) {
		pr_err("DPM: port_id(%d) out of 0~%d\n", port_id, MAX_DP_PORTS -1);
		return true;
	}
	return false;
}

static inline bool is_invalid_inst(int inst)
{
	if ((inst < 0) || (inst >= DP_MAX_INST)) {
		pr_err("DPM: inst(%d) out of 0~%d\n", inst, DP_MAX_INST-1);
		return true;
	}
	return false;
}

#define DP_MAX_HW_CAP 4

#ifdef DP_SPIN_LOCK
#define DP_LOCK_T spinlock_t
#define DP_LOCK_INIT(lock) spin_lock_init(lock)
#define DP_DEFINE_LOCK(lock) DEFINE_SPINLOCK(lock)
#define DP_LIB_LOCK    spin_lock_bh
#define DP_LIB_UNLOCK  spin_unlock_bh
#else
#define DP_LOCK_T struct mutex
#define DP_LOCK_INIT(lock) mutex_init(lock)
#define DP_DEFINE_LOCK(lock) DEFINE_MUTEX(lock)
#define DP_LIB_LOCK    mutex_lock
#define DP_LIB_UNLOCK  mutex_unlock
#endif

extern DP_LOCK_T dp_lock;

#define PARSER_FLAG_SIZE   40
#define PARSER_OFFSET_SIZE 8

#define PMAC_HDR_SIZE (sizeof(struct pmac_rx_hdr))
#define PKT_PASER_FLAG_OFFSET   0
#define PKT_PASER_OFFSET_OFFSET (PARSER_FLAG_SIZE)
#define PKT_PMAC_OFFSET         ((PARSER_FLAG_SIZE) + (PARSER_OFFSET_SIZE))
#define PKT_DATA_OFFSET         ((PKT_PMAC_OFFSET) + (PMAC_HDR_SIZE))

#define CHECK_BIT(var, pos) (((var) & (1 << (pos))) ? 1 : 0)

#define PASAR_OFFSETS_NUM 40	/*40 bytes offset */
#define PASAR_FLAGS_NUM 8	/*8 bytes */

#define PMAC_RECID_GET_EXT_PNT(recid) (recid & 0x3)

/* maximum number of dma core: dma0/1/2*/
#define DP_MAX_DMA_CORE 3
/* maximum number of dma port per core: so far only support 1 */
#define DP_MAX_DMA_PORT 1
/* maximum number of dma channel per core */
#define DP_MAX_DMA_CH 18

/* maximum dma controller*/
#define DP_DMAMAX 7
#define DP_DEQ(p, q) (dp_deq_port_tbl[p][q])

enum dp_xmit_errors {
	DP_XMIT_ERR_DEFAULT = 0,
	DP_XMIT_ERR_NOT_INIT,
	DP_XMIT_ERR_IN_IRQ,
	DP_XMIT_ERR_NULL_SUBIF,
	DP_XMIT_ERR_PORT_TOO_BIG,
	DP_XMIT_ERR_NULL_SKB,
	DP_XMIT_ERR_NULL_IF,
	DP_XMIT_ERR_REALLOC_SKB,
	DP_XMIT_ERR_EP_ZERO,
	DP_XMIT_ERR_GSO_NOHEADROOM,
	DP_XMIT_ERR_CSM_NO_SUPPORT,
	DP_XMIT_PTP_ERR,
};

enum PARSER_FLAGS {
	PASER_FLAGS_NO = 0,
	PASER_FLAGS_END,
	PASER_FLAGS_CAPWAP,
	PASER_FLAGS_GRE,
	PASER_FLAGS_LEN,
	PASER_FLAGS_GREK,
	PASER_FLAGS_NN1,
	PASER_FLAGS_NN2,

	PASER_FLAGS_ITAG,
	PASER_FLAGS_1VLAN,
	PASER_FLAGS_2VLAN,
	PASER_FLAGS_3VLAN,
	PASER_FLAGS_4VLAN,
	PASER_FLAGS_SNAP,
	PASER_FLAGS_PPPOES,
	PASER_FLAGS_1IPV4,

	PASER_FLAGS_1IPV6,
	PASER_FLAGS_2IPV4,
	PASER_FLAGS_2IPV6,
	PASER_FLAGS_ROUTEXP,
	PASER_FLAGS_TCP,
	PASER_FLAGS_1UDP,
	PASER_FLAGS_IGMP,
	PASER_FLAGS_IPV4OPT,

	PASER_FLAGS_IPV6EXT,
	PASER_FLAGS_TCPACK,
	PASER_FLAGS_IPFRAG,
	PASER_FLAGS_EAPOL,
	PASER_FLAGS_2IPV6EXT,
	PASER_FLAGS_2UDP,
	PASER_FLAGS_L2TPNEXP,
	PASER_FLAGS_LROEXP,

	PASER_FLAGS_L2TP,
	PASER_FLAGS_GRE_VLAN1,
	PASER_FLAGS_GRE_VLAN2,
	PASER_FLAGS_GRE_PPPOE,
	PASER_FLAGS_BYTE4_BIT4,
	PASER_FLAGS_BYTE4_BIT5,
	PASER_FLAGS_BYTE4_BIT6,
	PASER_FLAGS_BYTE4_BIT7,

	PASER_FLAGS_BYTE5_BIT0,
	PASER_FLAGS_BYTE5_BIT1,
	PASER_FLAGS_BYTE5_BIT2,
	PASER_FLAGS_BYTE5_BIT3,
	PASER_FLAGS_BYTE5_BIT4,
	PASER_FLAGS_BYTE5_BIT5,
	PASER_FLAGS_BYTE5_BIT6,
	PASER_FLAGS_BYTE5_BIT7,

	PASER_FLAGS_BYTE6_BIT0,
	PASER_FLAGS_BYTE6_BIT1,
	PASER_FLAGS_BYTE6_BIT2,
	PASER_FLAGS_BYTE6_BIT3,
	PASER_FLAGS_BYTE6_BIT4,
	PASER_FLAGS_BYTE6_BIT5,
	PASER_FLAGS_BYTE6_BIT6,
	PASER_FLAGS_BYTE6_BIT7,

	PASER_FLAGS_BYTE7_BIT0,
	PASER_FLAGS_BYTE7_BIT1,
	PASER_FLAGS_BYTE7_BIT2,
	PASER_FLAGS_BYTE7_BIT3,
	PASER_FLAGS_BYTE7_BIT4,
	PASER_FLAGS_BYTE7_BIT5,
	PASER_FLAGS_BYTE7_BIT6,
	PASER_FLAGS_BYTE7_BIT7,

	/*Must be put at the end of the enum */
	PASER_FLAGS_MAX
};

enum dp_message_errors {
	DP_ERR_SUBIF_NOT_FOUND = -7,
	DP_ERR_INIT_FAIL = -6,
	DP_ERR_INVALID_PORT_ID = -5,
	DP_ERR_MEM = -4,
	DP_ERR_NULL_DATA = -3,
	DP_ERR_INVALID_SUBIF = -2,
	DP_ERR_DEFAULT = -1,
};

/*! Registration status flag */
enum PORT_FLAG {
	PORT_FREE = 0,		/*! The port is free */
	PORT_ALLOCATED,		/*! the port is already allocated to others,
				 * but not registered or no need to register.\n
				 * eg, LRO/CAPWA, only need to allocate,
				 * but no need to register
				 */
	PORT_DEV_REGISTERED,	/*! dev Registered already. */
	PORT_SUBIF_REGISTERED,	/*! subif Registered already. */

	PORT_FLAG_NO_VALID	/*! Not valid flag */
};

#define DP_DBG_ENUM_OR_STRING(name, value, short_name) {name = value}

enum DP_DBG_FLAG {
	DP_DBG_FLAG_DBG = BIT_ULL(0),
	DP_DBG_FLAG_DUMP_RX_DATA = BIT_ULL(1),
	DP_DBG_FLAG_DUMP_RX_DESCRIPTOR = BIT_ULL(2),
	DP_DBG_FLAG_DUMP_RX_PASER = BIT_ULL(3),
	DP_DBG_FLAG_DUMP_RX_PMAC = BIT_ULL(4),
	DP_DBG_FLAG_DUMP_RX = (BIT_ULL(1) | BIT_ULL(2) | BIT_ULL(3) | BIT_ULL(4)),
	DP_DBG_FLAG_DUMP_TX_DATA = BIT_ULL(5),
	DP_DBG_FLAG_DUMP_TX_DESCRIPTOR = BIT_ULL(6),
	DP_DBG_FLAG_DUMP_TX_PMAC = BIT_ULL(7),
	DP_DBG_FLAG_DUMP_TX_SUM = BIT_ULL(8),
	DP_DBG_FLAG_DUMP_TX = (BIT_ULL(5) | BIT_ULL(6) | BIT_ULL(7) | BIT_ULL(8)),
	DP_DBG_FLAG_COC = BIT_ULL(9),
	DP_DBG_FLAG_MIB = BIT_ULL(10),
	DP_DBG_FLAG_MIB_ALGO = BIT_ULL(11),
	DP_DBG_FLAG_CBM_BUF = BIT_ULL(12),
	DP_DBG_FLAG_PAE = BIT_ULL(13),
	DP_DBG_FLAG_INST = BIT_ULL(14),
	DP_DBG_FLAG_SWDEV = BIT_ULL(15),
	DP_DBG_FLAG_NOTIFY = BIT_ULL(16),
	DP_DBG_FLAG_LOGIC = BIT_ULL(17),
	DP_DBG_FLAG_GSWIP_API = BIT_ULL(18),
	DP_DBG_FLAG_QOS = BIT_ULL(19),
	DP_DBG_FLAG_QOS_DETAIL = BIT_ULL(20),
	DP_DBG_FLAG_LOOKUP = BIT_ULL(21),
	DP_DBG_FLAG_REG = BIT_ULL(22),
	DP_DBG_FLAG_BR_VLAN = BIT_ULL(23),
	DP_DBG_FLAG_PCE = BIT_ULL(24),
	DP_DBG_FLAG_OPS = BIT_ULL(25),
	DP_DBG_FLAG_QMAP = BIT_ULL(26),
	DP_DBG_FLAG_SPL = BIT_ULL(27),
	DP_DBG_FLAG_SWDEV_DETAIL = BIT_ULL(28),
	DP_DBG_FLAG_INIT_STAT = BIT_ULL(29),
	DP_DBG_FLAG_GDB = BIT_ULL(30),
	DP_DBG_FLAG_HOOK = BIT_ULL(31),

	/*Note, once add a new entry here in the enum,
	 *need to add new item in below macro DP_DBG_FLAG_LIST
	 */
	DP_DBG_FLAG_MAX = BIT_ULL(63)
};

enum DP_DBGFS_FLAG {
	DP_DBGFS_FLAG_DPID   = BIT_ULL(0),
	DP_DBGFS_FLAG_PORT   = BIT_ULL(1),
	DP_DBGFS_FLAG_DEV    = BIT_ULL(2),
	DP_DBGFS_FLAG_DEVOPS = BIT_ULL(3),
	DP_DBGFS_FLAG_BR     = BIT_ULL(4),
	DP_DBGFS_FLAG_DEQ    = BIT_ULL(5),
	DP_DBGFS_FLAG_ENQ    = BIT_ULL(6),
	DP_DBGFS_FLAG_QOS    = BIT_ULL(7),
	DP_DBGFS_FLAG_HOOKS  = BIT_ULL(8),
	DP_DBGFS_FLAG_DBGFS  = BIT_ULL(0) | BIT_ULL(1) |
			       BIT_ULL(2) | BIT_ULL(3) |
			       BIT_ULL(4) | BIT_ULL(5) |
			       BIT_ULL(6) | BIT_ULL(7) |
			       BIT_ULL(8),
	DP_DBGFS_FLAG_MAX    = BIT_ULL(63)
};

/*Note: per bit one variable */
#define DP_DBG_FLAG_LIST {\
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DBG, "dbg"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_DATA, "rx_data"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_DESCRIPTOR, "rx_desc"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_PASER, "rx_parse"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_PMAC, "rx_pmac"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX, "rx"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_DATA, "tx_data"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_DESCRIPTOR, "tx_desc"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_PMAC, "tx_pmac"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_SUM, "tx_sum"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX, "tx"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_COC, "coc"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_MIB, "mib"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_MIB_ALGO, "mib_algo"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_CBM_BUF, "cbm_buf"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_PAE, "pae"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_INST, "inst"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_SWDEV, "swdev"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_NOTIFY, "notify"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_LOGIC, "logic"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_GSWIP_API, "gswip"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_QOS, "qos"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_QOS_DETAIL, "qos2"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_LOOKUP, "lookup"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_REG, "register"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_BR_VLAN, "br_vlan"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_PCE, "pce"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_OPS, "ops"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_QMAP, "qmap"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_SPL, "spl"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_SWDEV_DETAIL, "swdev2"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_INIT_STAT, "init"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_GDB, "gdb"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_HOOK, "hook"), \
	/*must be last one */\
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_MAX, "")\
}

#define DP_DBGFS_FLAG_LIST {\
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_DPID, "dbgfs_dpid"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_PORT, "dbgfs_port"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_DEV, "dbgfs_dev"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_DEVOPS, "dbgfs_devops"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_BR, "dbgfs_br"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_DEQ, "dbgfs_deq"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_ENQ, "dbgfs_enq"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_QOS, "dbgfs_qos"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_HOOKS, "dbgfs_hooks"), \
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_DBGFS, "dbgfs"), \
	/*must be last one */\
	DP_DBGFS_ENUM_OR_STRING(DP_DBGFS_FLAG_MAX, "")\
}

enum QOS_FLAG {
	NODE_LINK_ADD = 0, /*add a link node */
	NODE_LINK_GET,     /*get a link node */
	NODE_LINK_EN_GET,  /*Get link status: enable/disable */
	NODE_LINK_EN_SET,  /*Set link status: enable/disable */
	NODE_UNLINK,       /*unlink a node: in fact, it is just flush now*/
	LINK_ADD,          /*add a link with multiple link nodes */
	LINK_GET,          /*get a link may with multiple link nodes */
	LINK_PRIO_SET,     /*set arbitrate/priority */
	LINK_PRIO_GET,     /*get arbitrate/priority */
	QUEUE_CFG_SET,     /*set queue configuration */
	QUEUE_CFG_GET,     /*get queue configuration */
	SHAPER_SET,        /*set shaper/bandwidth*/
	SHAPER_GET,        /*get shaper/bandwidth*/
	NODE_ALLOC,        /*allocate a node */
	NODE_FREE,         /*free a node */
	NODE_CHILDREN_FREE,  /*free all children under one specified parent:
			      *   scheduler/port
			      */
	DEQ_PORT_RES_GET,  /*get all full links under one specified port */
	COUNTER_MODE_SET,  /*set counter mode: may only for TMU now so far*/
	COUNTER_MODE_GET,  /*get counter mode: may only for TMU now so far*/
	QUEUE_MAP_GET,     /*get lookup entries based on the specified qid*/
	QUEUE_MAP_SET,     /*set lookup entries to the specified qid*/
	NODE_CHILDREN_GET, /*get direct children list of node*/
	QOS_LEVEL_GET,     /* get Max Scheduler level for Node */
	QOS_Q_LOGIC,       /* get logical queue ID based on physical queue ID */
	QOS_GLOBAL_CFG_GET, /* get global qos config info */
	QOS_PORT_CFG_SET, /* set qos port config info */
	QOS_BLOCK_FLUSH_PORT, /* Block and Flush all QiD's in the port */
	QOS_BLOCK_FLUSH_QUEUE, /* Block and Flush particular QiD */
	CODEL_GET,	/* get system level codel configuration */
	CODEL_SET,	/* set system level codel configuration */
};

enum DP_MAP_Q_TYPE {
	DP_MAP_Q_SUBIF = 0, /*!< to this subif's queue */
	DP_MAP_Q_CPU,  /*!< to CPU 1st default queue */
	DP_MAP_Q_REINSERT, /*!< For reinsertion default queue */
	DP_MAP_Q_TOE, /*!< For TOE default queue */
	DP_MAP_Q_SPL_CONN_IGP, /*!< For SPL_CONN igp's egp queue map */
	DP_MAP_Q_DROP, /*!< For reset queue mapping during de_regsiter_subif */
	DP_MAP_Q_DC_RXOUT_QOS, /*!< For DC RXOUT Ring QOS queue mapping */
};

enum CQM_PORT_INFO_FLAG {
	CQM_PORT_INFO_DC_RX_Q = 1, /* shared RXOUT QOS queue */
};

#define Q_MAP_F_DONT_CARE -1
struct dp_q_map_entry {
	u32 lookup_f; /*!< CQM lookup set flag */
	u8 enc;  /*!< 0, 1 or Q_MAP_F_DONT_CARE */
	u8 mpe2; /*!< 0, 1 or Q_MAP_F_DONT_CARE */
	u8 mpe1; /*!< 0, 1 or Q_MAP_F_DONT_CARE */
	u8 egflag; /*!< 0, 1 or Q_MAP_F_DONT_CARE */
	u8 cls1; /*!< class range from: 0 ~ 15 only  */
	u8 cls2;  /*!< class range to:    0 ~ 15 only */
	enum DP_MAP_Q_TYPE q_type;
};

struct q_map_info {
	char *name; /* name for this set of rules */
	int num; /* number of q_map rules */
	u32 alloc_flag; /* its alloc_flag of this dp_port */
	struct dp_q_map_entry *entry; /* cqm lookup entry rules */
};

struct dev_mib {
	atomic_t rx_fn_rxif_pkt; /*! received packet counter */
	atomic_t rx_fn_txif_pkt; /*! transmitted packet counter */
	atomic_t rx_fn_dropped; /*! transmitted packet counter */
	atomic_t tx_cbm_pkt; /*! transmitted packet counter */
	atomic_t tx_clone_pkt; /*! duplicate unicast packet for cloned flag */
	atomic_t tx_hdr_room_pkt; /*! duplicate pkt for no enough headerroom*/
	atomic_t tx_tso_pkt;	/*! transmitted packet counter */
	atomic_t tx_pkt_dropped;	/*! dropped packet counter */
};

struct mib_global_stats {
	u64 rx_rxif_pkts;
	u64 rx_txif_pkts;
	u64 rx_rxif_clone;
	u64 rx_drop;
	u64 tx_pkts;
	u64 tx_drop;
};

DECLARE_PER_CPU_SHARED_ALIGNED(struct mib_global_stats, mib_g_stats);

#define MIB_G_STATS_INC(member) do { \
			per_cpu(mib_g_stats, get_cpu()).member++; \
			put_cpu(); \
		} while(0)

#define MIB_G_STATS_RESET(member, cpu) do { \
			per_cpu(mib_g_stats, cpu).member = 0; \
		} while(0)

#define MIB_G_STATS_GET(member, cpu) \
			per_cpu(mib_g_stats, cpu).member

/* Note: for pmapper device, from dpm design point of view, we can not support
 *       VLAN device created via vconfig
 *       Otherwise logical_dev is difficult to handle in dpm
 */
struct logic_dev {
	struct list_head list;
	struct net_device *dev;
	u16 bp; /*bridge port */
	u16 ep;
	u16 ctp;
	u32 subif_flag; /* save the flag used during dp_register_subif */
	s16 fid; /* need support it later */
};

struct dp_igp {
	u32 igp_id; /* CQM enqueue port based ID */
	u32 igp_dma_ch_to_gswip; /* DMA TX channel base to GSWIP */
	u32 num_out_cqm_deq_port; /* num of CQM dequeue port to GSWIP */
};

struct dp_egp {
	int egp_id; /* EGP port ID */
	enum DP_EGP_TYPE type; /* EGP port: DP_EGP_TO_DEV, DP_EGP_TO_GSWIP */
};

struct qos_cqm_info {
#if (defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN)) || defined(__BIG_ENDIAN)
	u16 num;  /* the nunber of continuous DEq port/ring */
	u16 base;  /* CQM Deq Port(LGM/PRX)/Ring(TPZ) base */
#else
	u16 base;  /* CQM Deq Port(LGM/PRX)/Ring(TPZ) base */
	u16 num; /* the nunber of continuous DEq port/ring */
#endif
};

struct qos_setting_match {
	char *catagory;
	u32 flag;
	int idx_catagory;
};

struct dp_qos_setting {
	bool valid;
	const char *node_name;
	const char *category;
	u32 flag;
	union {
		u32 qos_id;
		struct qos_cqm_info cqm_deq;
	};
	u32 wred_en;
	u32 codel_en;
	u32 qlen;

	/* for debug only */
	u16 category_idx;
	u16 sub_category_idx;
};

#define DP_DFLT_DOMAIN_MEMBER 0xFFFFFFFF

/*! Sub interface detail information */
struct dp_subif_info {
	s32 flags;
	u32 subif;
	struct net_device *netif; /*! pointer to  net_device */
	char device_name[IFNAMSIZ]; /*! devide name, like wlan0, */
	struct dev_mib mib; /*! mib */
	struct dp_reinsert_count reins_cnt;
	struct net_device *ctp_dev; /*CTP dev for PON pmapper case*/
	u16 fid; /* switch bridge id */
	struct list_head logic_dev; /*unexplicit logical dev*/
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	void *swdev_priv; /*to store ext vlan info*/
#endif
	u8 deq_port_idx; /* To store deq port relative index from register_subif */
	u8 data_toe_disable;
	u8 f_spl_gpid; /*use spl gpid, 0: for is_stream_port() true, else 1*/
	union {
		struct {
			u16 bp;     /* bridge port */
			u16 gpid; /*!< [out] gpid which is mapped from
				   * dpid +subif normally one GPID per subif for
				   *   non PON device.
				   *   For PON case, one GPID per bridge port
				   */
			u32 data_flag_ops; /* To store original flag from caller
					    * during dp_register_subif
					    * under data->flag_ops
					    */
			u8 num_qid; /*!< number of queue id*/
			union {
				u16 qid;    /* physical queue id Still keep it
					     * to be back-compatible for legacy
					     * platform and legacy integration
					     */
				/* physical queue id */
				u16 qid_list[DP_MAX_DEQ_PER_SUBIF];
			};
			u32 subif_groupid;
		};
		struct dp_subif_common subif_common;
	};
	u8 domain_id:5;	/*!< we support 32 domain per bridge only
			 * valid only DP_SUBIF_BR_DOMAIN is set in flag_ops
			 */
	u32 domain_members; /*!< one bit for one domain_id:
			     * bit 0 for domain_id 0, bit 1 for domain 1 and so on.
			     * If one bit is set to 1, the traffic received
			     * from this dev can be forwarded to this domain
			     * as specified in domain_members
			     * valid only if DP_SUBIF_BR_DOMAIN is set in flag_ops
			     */
	u32 ctp_base;	/* ctp base for this subif's dp_port */
	u32 ctp;	/* ctp for this subif's dp_port */
	s16 sched_id; /* can be physical scheduler id or logical node id */
	s16 q_node[DP_MAX_DEQ_PER_SUBIF]; /* logical Q node Id if applicable */
	s16 qos_deq_port[DP_MAX_DEQ_PER_SUBIF]; /* qos port id */
	s16 cqm_deq_port[DP_MAX_DEQ_PER_SUBIF]; /* CQM physical dequeue port ID
						 * (absolute)
						 */
	s16 cqm_port_idx; /* CQM relative dequeue port index, like tconf id */
	u32 subif_flag; /* To store original flag from caller during
			 * dp_register_subif
			 */
	u16 mac_learn_dis; /* To store mac learning capability of subif from
			    * caller during dp_register_subif
			    */
	atomic_t rx_flag; /* To enable/disable DP rx */
	u16 dfl_sess[DP_DFL_SESS_NUM];
				  /*<! default CPU egress
				   * session ID Valid
				   * only if its >= 0
				   * one sesson per
				   * class[up to 4 bits]
				   */
	u16 max_pkt_size;
	u16 cqm_mtu_size;
	u16 headroom_size;
	u16 tailroom_size;
	u16 spl_gpid_headroom_size;
	u16 spl_gpid_tailroom_size;
	u16 min_pkt_len;
	int min_pkt_len_cfg;
	u16 tx_policy_base;    /* TX policy base */
	u8 tx_policy_num;      /* TX policy number */
	u8 tx_policy_map;      /* TX policy map */
	u16 rx_policy_base; /* RX policy base */
	u8 rx_policy_num;   /* RX policy number */
	u8 rx_policy_map;   /* RX policy map */
	u8 pkt_only_en;
	u8 seg_en;
	u16 swdev_en;
	bool vlan_aware_en; /* Bport vlan based mac learning enable/disable,
						 * Valid only for LGM
						 */
	bool cpu_port_en;
	u8 prel2_len:2;	 /* 0 = disabled, 1 = 16 bytes, 2 = 32 bytes, 3 = 48 bytes */
	dp_rx_fn_t rx_fn;	/*!< Rx function callback */
	dp_get_netif_subifid_fn_t get_subifid_fn; /*! get subif ID callback */
	enum DP_DATA_PORT_TYPE type;
	enum DP_SPL_TYPE spl_conn_type; /* only for special path,
					 * otherwise set to DP_SPL_INVAL
					 */
	int igp_id;
	int tx_pkt_credit;
	u8 num_igp;
	u8 num_egp;
	struct dp_spl_cfg *spl_cfg;
	const struct pmac_port_info *port_info; /* points to port_info */
	struct q_map_info *q_map; /* If NULL, then map to drop queue */
	int toe_tc;
	u16 last_hosif_qid[DP_MAX_CPU][DP_NUM_HIF_PRIO];
	struct dp_dev *dp_dev; /* link to dp_dev */
};

struct vlan_info {
	u16 out_proto;
	u16 out_vid;
	u16 in_proto;
	u16 in_vid;
	int cnt;
};

enum DP_TEMP_DMA_PMAC {
	TEMPL_NORMAL = 0,
	TEMPL_CHECKSUM,
	TEMPL_PTP,
	TEMPL_INSERT,
	TEMPL_OTHERS,
	TEMPL_CHECKSUM_PTP,
	MAX_TEMPLATE
};

enum DP_PRIV_F {
	DP_PRIV_PER_CTP_QUEUE = BIT(0), /*Manage Queue per CTP/subif */
};

struct pmac_port_info {
	enum PORT_FLAG status;	/*! port status */
	struct dp_cb cb;	/*! Callback Pointer to DIRECTPATH_CB */
	struct module *owner;
	struct net_device *dev;
	int inst; /* instance id */
	struct inst_property *inst_prop; /* instance property pointer */
	u32 dev_port;
	u32 num_subif; /* number of subif registered under this dp_port */
	u16 subif_max;
	union {
		struct {
			int port_id;
			int alloc_flags; /* the flags saved when calling
					  * dp_port_alloc
					  */
			u8  cqe_lu_mode; /* cqe lookup mode */
			u32 gsw_mode; /* gswip mode for subif */
			s16 gpid_spl;  /* special GPID:
					* alloc it at dp_alloc_port
					* config it at dp_register_dev
					* for policy setting
					*/

		};
		struct dp_subif_port_common subif_port_cmn;
	};
	atomic_t tx_err_drop;
	atomic_t rx_err_drop;
	struct gsw_itf *itf_info;  /*point to switch interface configuration */
	int ctp_max; /*maximum ctp */
	u32 n_wib_credit_pkt; /*WIB credit packet caller set for the port*/
	u32 vap_offset; /*shift bits to get vap value */
	u32 vap_mask; /*get final vap after bit shift */
	u8 oob_subif_offset;
	u8 oob_subif_size;
	u8 oob_class_size;
	u32 flag_other; /*save flag from cbm_dp_port_alloc */
	u32 deq_port_base; /*CQE Dequeue Port */
	u32 deq_port_num;  /*for PON IP: 64 ports, CPU 4/8, most are 1 only*/
	u16 deq_ports[64]; /* CQM dequeue port may not continuous, like
			    * like wave700 case.
			    */
	u32 dma_chan; /*associated dma tx CH,-1 means no DMA CH*/
	u32 tx_pkt_credit;  /*PP port tx bytes credit */
	u32 tx_b_credit;  /*PP port tx bytes credit */
	void *txpush_addr_qos;  /*QoS push addr after shift or mask from
				 * PP QOS point of view
				 */
	void *txpush_addr;  /* QOS push address without any shift/mask */
	u32 tx_ring_size; /*PP ring size */
	u32 tx_ring_offset;  /*PP: next tx_ring_addr=
			      *   current tx_ring_addr + tx_ring_offset
			      */
	u16 gpid_base; /* gpid base
			* For CPU/DPDK:
			*   alloc it in dp_platform_set
			*   config it in dp_platform_set
			* For peripheral device
			*   alloc it at dp_alloc_port via gpid_port_assign
			*   config it at dp_register_subif
			*/
	u16 gpid_num; /* reserved nubmer of continuous of gpid */
	u16 tx_policy_base; /* TX policy base */
	u8 tx_policy_num;   /* TX policy number */
	u16 rx_policy_base; /* RX policy base */
	u8 rx_policy_num;   /* RX policy number */
	u16 spl_tx_policy_base; /* TX policy base   for spl GPID */
	u8 spl_tx_policy_num;   /* TX policy number for spl GPID */
	u8 spl_tx_policy_map;   /* TX policy map    for spl GPID */
	u16 spl_rx_policy_base; /* RX policy base   for spl GPID */
	u8 spl_rx_policy_num;   /* RX policy number for spl GPID */
	u8 spl_rx_policy_map;   /* RX policy map    for spl GPID */
	u32 num_dma_chan; /*For G.INT it's 8 or 16, for other 1*/
	u32 lct_idx; /* LCT subif register flag */
	struct dp_lct_rx_cnt *lct_rx_cnt; /* For lct counter corrections,
					   * only allocated in case of lct subif
					   */
	u32 dma_chan_tbl_idx; /*! Base entry index of dp_dma_chan_tbl */
	u32 res_qid_base; /* Base entry for the device's reserved Q */
	u32 num_resv_q; /* Num of reserved Q per device */
	u32 f_ptp: 1; /* PTP1588 support enablement */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	u32 swdev_en; /* switchdev enable/disable flag for port */
#endif
	u16 loop_dis; /*!< If set do not add this dp port's bp
		       * into other bp's member port map
		       * if source = dest dp port
		       */
	u16 bp_hairpin_cap; /*!< If set, then add BP's in this dev
			     * into its own bridgeport member list
			     * For PRX: Only WLAN/Wifi device need
			     * hair pin capability
			     */
	/*only valid for 1st dp instanace which need dp_xmit/dp_rx*/
	/*[0] for non-checksum case,
	 *[1] for checksum offload
	 *[2] two cases:
	 * a: only traffic directly to MPE DL FW
	 * b: DSL bonding FCS case
	 */
	struct pmac_tx_hdr pmac_template[MAX_TEMPLATE];
	u32 desc_dw_templ[4][MAX_TEMPLATE];
	u32 desc_dw_mask[4][MAX_TEMPLATE];
	u32 num_tx_ring; /* Num of Tx ring */
	u32 num_rx_ring; /* Num of Rx ring */
	struct dp_rx_ring rx_ring[DP_RX_RING_NUM]; /*!< DC rx ring info	*/
	struct dp_tx_ring tx_ring[DP_TX_RING_NUM]; /*!< DC tx ring info	*/
	u8 num_umt_port; /* Num of UMT port */
	u16 umt_dc_map_idx[DP_MAX_UMT];
	struct dp_umt_port *umt[DP_MAX_UMT]; /*!< UMT Param */
	enum DP_DATA_PORT_TYPE type;
	u32 blk_size;	/*!< PCE Block Size */
	int qid_base; /*!< current it is only for EPON case */
#ifdef CONFIG_RFS_ACCEL
	struct cpu_rmap  *rx_cpu_rmap; /*!<CPU Affinity Reverse Map for CQM IRQs */
#endif /* CONFIG_RFS_ACCEL */
	u32 data_flag_ops; /* flag_ops from caller */
	struct dp_qos_setting *dts_qos;

	/* These members must be end. */
	u32 tail;
	struct dp_subif_info *subif_info;
	spinlock_t mib_cnt_lock; /* lock for updates from mib_counters module*/
};

struct ctp_dev {
	struct list_head list;
	struct net_device *dev; /* CTP device pointer */
	u16 bp; /* bridge port */
	u16 ctp; /* CTP port */
};

#define BRIDGE_MEMBER_PORT_BIT_MAP  16
/*bridge port with pmapper supported dev structure */
struct bp_pmapper {
	int f_alloc; /* allot flag: 0 -not allocated, 1-allocated */
	int flag;/* pmapper flag: 0-non-pmapper, 1-pmapper case */
	struct net_device *dev; /*bridge port device pointer */
	struct list_head ctp_dev; /* CTP dev list */
	int pcp[DP_PMAP_PCP_NUM];  /*PCP table */
	int dscp[DP_PMAP_DSCP_NUM]; /*DSCP table*/
	int def_ctp; /*Untag & nonip*/
	int mode; /*mode*/
	int ref_cnt; /* reference counter of ctp_dev under this device:
		      * zero means currently it is non-pmapper device
		      */
	int ctp_flow_index; /* to store CTP first flow entry index
			     * for vlan aware feature PCE rule
			     */
	u8 domain_id : 5;  /* maximum 32 domain */
	/* current bridge port members */
	u16 bp_member_map[BRIDGE_MEMBER_PORT_BIT_MAP];
	u32 domain_member;  /* domain_members */
};

/*queue struct */
struct q_info {
	int flag;  /*0-FREE, 1-Used*/
	int need_free; /*if this queue is allocated by dp_register_subif,
			*   it needs free during de-register.
			*Otherwise, no free
			*/
	int q_node_id;
	int ref_cnt; /*subif_counter*/
	int cqm_dequeue_port; /*CQM dequeue port */
};

/*scheduler struct */
struct dp_sched_info {
	int flag;  /*0-FREE, 1-Used*/
	int ref_cnt; /*subif_counter*/
	int cqm_dequeue_port; /*CQM dequeue port */
};

struct dma_chan_info {
	atomic_t ref_cnt; /* number of subif is using this DMA channel */
};

struct dp_umt_info {
	bool umt_valid;	/*!< umt info 1: valid 0: Invalid */
	u8 ref_cnt_umt; /*!< ref counter of UMT which was increased/decreased
			 * during dp_register_dev
			 */
	struct dp_umt_port umt; /*!< UMT Param */
};

/* This is mainly used for storing dummy read flag for RxIn ring*/
struct cqm_igp_info {
	u8 igp;
	/* Below all are arrays because if IGP is shared resource, need to store
	 * all ports, like wave700
	 */
	u8 dp_port[MAX_DP_PORTS]; /* If igp is shared across dp ports, each idx denotes a dp port,
				   * if dp_port[i] >= 0 then i'th dp_port has
				   * this IGP and dp_port[i] is ref_count for that.
				   */
	u8 dc_port[MAX_DP_PORTS][DP_DC_NUM];
	u8 umt_port[DP_MAX_PPV4_PORT];
	struct {
		u8 dc_dummy_read:1; /*!< 1: cqm dc dummy read already done; 0: Not done */
		u8 ref_cnt:7; /*!< ref cnt; currently incrementing when umt_ref_cnt inc */
	};
};

struct cqm_port_info {
	u32 f_first_qid : 1; /* first_qid not valid if f_first_qid zero */
	u32 f_dma_ch : 1; /* 1 means dma_chan valid, 0 means not valid */
	u32 ref_cnt; /* reference counter: the number of subif attached to it
		      * which was increased/decreased during dp_register_subif
		      * or shared by different DC RXOUT rings during
		      * dp_register_dev.
		      * For each cqm port, only one mode will be used, each by
		      * CTP or Rxout ring. Never will be both.
		      * Note: these two mode has to share same variable ref_cnt
		      * since alloc_q related API heavily depends on ref_cnt
		      * during dp_register_dev and dp_deregister_dev
		      */
	u32 tx_pkt_credit;  /* PP port tx bytes credit */
	void *txpush_addr; /* QOS push addr after shift or mask from
			    * PP QOS point of view
			    */
	void *txpush_addr_qos; /* QOS push address without any shift/mask */
	u32 tx_ring_size; /* PP port ring size */
	union {
		int first_qid; /* in order to auto sharing queue,
				* 1st queue allocated by dp_register_subif_ext
				* for that cqm_dequeue_port will be stored here.
				* later it will be shared by other subif via
				* dp_register_subif_ext
				*/
		int qid[DP_MAX_DEQ_PER_SUBIF];
	};
	int q_node; /* first_qid's logical node id*/
	u8 dp_port[MAX_DP_PORTS]; /* If egp is shared across dp ports, each idx denotes a dp port,
				   * if dp_port[i] = 1 then i'th dp_port has
				   * this EGP.
				   */
	u32 dma_chan; /* dma_chan */
	/*! Offset of dp_dma_chan_tbl. Zero means not NA
	 *  So far we only set prpoper dma_ch_offset for non-aca device, ie,
	 *  only for those device which need call cbm_dp_enable to
	 *  enable/disable its DMA channel
	 *  For ACA, CPU, re-insertion and SPL, dma_ch_offset should be zero
	 */
	u32 dma_ch_offset;
	u32 cpu_type;
	enum CQM_PORT_INFO_FLAG flag; /* other special flags */
	struct dp_umt_info umt_info[DP_MAX_UMT];
	struct dp_qos_setting *dts_qos;
};

struct parser_info {
	u8 v;
	s8 size;
};

struct subif_platform_data {
	struct net_device *dev;
	struct dp_subif_data *subif_data;  /*from dp_register_subif_ex */
#define TRIGGER_CQE_DP_ENABLE  1
	int act; /*Set by HAL subif_platform_set and used by DP lib */
};

struct vlan_info1 {
	/* Changed this TPID field to GSWIP API enum type.
	 * We do not have flexible for any TPID, only following are supported:
	 * 1. ignore (don't care)
	 * 2. 0x8100
	 * 3. Value configured int VTE Type register
	 */
	GSW_ExtendedVlanFilterTpid_t tpid;  /* TPID like 0x8100, 0x8800 */
	u16 vid ;  /*VLAN ID*/
	/*note: user priority/CFI both don't care */
	/* DSCP to Priority value mapping is possible */
};

struct vlan1 {
	int bp;  /*assigned bp for this single VLAN dev */
	struct vlan_info1 outer_vlan; /*out vlan info */
	/* Add Ethernet type with GSWIP API enum type.
	 * Following types are supported:
	 * 1. ignore	(don't care)
	 * 2. IPoE	(0x0800)
	 * 3. PPPoE	(0x8863 or 0x8864)
	 * 4. ARP	(0x0806)
	 * 5. IPv6 IPoE	(0x86DD)
	 */
	GSW_ExtendedVlanFilterEthertype_t ether_type;
};

struct vlan2 {
	int bp;  /*assigned bp for this double VLAN dev */
	struct vlan_info1 outer_vlan;  /*out vlan info */
	struct vlan_info1 inner_vlan;   /*in vlan info */
	/* Add Ethernet type with GSWIP API enum type.
	 * Following types are supported:
	 * 1. ignore	(don't care)
	 * 2. IPoE	(0x0800)
	 * 3. PPPoE	(0x8863 or 0x8864)
	 * 4. ARP	(0x0806)
	 * 5. IPv6 IPoE	(0x86DD)
	 */
	GSW_ExtendedVlanFilterEthertype_t ether_type;
};

struct ext_vlan_info {
	int subif_grp, logic_port; /* base subif group and logical port.
				    * In DP it is subif
				    */
	int bp; /*default bp for this ctp */
	int n_vlan1, n_vlan2; /*size of vlan1/2_list*/
	int n_vlan1_drop, n_vlan2_drop; /*size of vlan1/2_drop_list */
	struct vlan1 *vlan1_list; /*allow single vlan dev info list auto
				   * bp is for egress VLAN setting
				   */
	struct vlan2 *vlan2_list; /* allow double vlan dev info list auto
				   * bp is for egress VLAN setting
				   */
	struct vlan1 *vlan1_drop_list; /* drop single vlan list - manual
					*  bp no use
					*/
	struct vlan2 *vlan2_drop_list; /* drop double vlan list - manual
					* bp no use
					*/
	/* Need add other input / output information for deletion. ?? */
	/* private data stored by function set_gswip_ext_vlan */
	void *priv;
};

struct dp_tc_vlan_info {
	int dev_type; /* bit 0 - 1: apply VLAN to bp
		       *         0: apply VLAN to subix (subif group)
		       * bit 1 - 0: apply VLAN to non-multicast session on CTP
		       *         1: don't apply VLAN to non-multicast session
		       * bit 2 - 0: apply VLAN to multicast session on CTP
		       *         1: don't apply VLAN to multicast session
		       */
	int subix;  /*similar like GSWIP subif group*/
	int bp;  /*bridge port id */
	int dp_port; /*logical port */
	int inst;  /*DP instance */
};

/*port 0 is reserved*/
extern int dp_inst_num;
extern int dp_print_len;
extern struct inst_property dp_port_prop[DP_MAX_INST];
extern struct pmac_port_info *dp_port_info[DP_MAX_INST];
extern struct q_info dp_q_tbl[DP_MAX_INST][DP_MAX_QUEUE_NUM];
extern struct dp_sched_info dp_sched_tbl[DP_MAX_INST][DP_MAX_NODES];
extern struct cqm_port_info dp_deq_port_tbl[DP_MAX_INST][DP_MAX_PPV4_PORT];
extern struct cqm_igp_info dp_enq_port_tbl[DP_MAX_INST][DP_MAX_CQM_IGP];
extern struct bp_pmapper dp_bp_tbl[DP_MAX_INST][DP_MAX_BP_NUM];
extern struct dma_chan_info *dp_dma_chan_tbl[DP_MAX_INST];
extern struct cbm_ops *dp_cbm_ops[DP_MAX_INST];

#define CBM_OPS(inst, fn, ...)	\
({\
	((dp_cbm_ops[inst]) && (dp_cbm_ops[inst]->fn)) ? dp_cbm_ops[inst]->fn(__VA_ARGS__) : -1;\
})

void dp_die(const char *func_name, int curr_v, int ref_v);

static inline struct inst_property *get_dp_port_prop(int inst)
{
	if ((inst < 0) || (inst >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_port_prop[0];
	}
	return &dp_port_prop[inst];
}

static inline struct pmac_port_info *get_dp_port_info(int inst, int index)
{
	if ((inst < 0) || (inst  >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_port_info[0][0];
	}
	if ((index < 0) ||
	    (index  >= dp_port_prop[inst].info.cap.max_num_dp_ports)) {
		pr_err("DPM: %s wrong dp_port=%d\n", __func__, index);
		dp_die(__func__, index,
		       dp_port_prop[inst].info.cap.max_num_dp_ports);
		return &dp_port_info[0][0];
	}
	return &dp_port_info[inst][index];
}

static inline struct cqm_port_info *get_dp_deqport_info(int inst, int idx)
{
	if ((inst < 0) || (inst  >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_deq_port_tbl[0][0];
	}
	if ((idx < 0) || (idx  >= DP_MAX_PPV4_PORT)) {
		pr_err("DPM: %s wrong deq_port=%d\n", __func__, idx);
		dp_die(__func__, idx, DP_MAX_PPV4_PORT);
		return &dp_deq_port_tbl[0][0];
	}
	return &dp_deq_port_tbl[inst][idx];
}

static inline struct cqm_igp_info *get_dp_enqport_info(int inst, int idx)
{
	if ((inst < 0) || (inst  >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_enq_port_tbl[0][0];
	}
	if ((idx < 0) || (idx  >= DP_MAX_CQM_IGP)) {
		pr_err("DPM: %s wrong enq_port=%d\n", __func__, idx);
		dp_die(__func__, idx, DP_MAX_CQM_IGP);
		return &dp_enq_port_tbl[0][0];
	}
	return &dp_enq_port_tbl[inst][idx];
}

static inline struct bp_pmapper *get_dp_bp_info(int inst, int idx)
{
	if ((inst < 0) || (inst  >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_bp_tbl[0][0];
	}
	if ((idx < 0) ||
	    (idx  >= dp_port_prop[inst].info.cap.max_num_bridge_port)) {
		pr_err("DPM: %s wrong bridge_port=%d\n", __func__, idx);
		dp_die(__func__, idx,
		       dp_port_prop[inst].info.cap.max_num_bridge_port);
		return &dp_bp_tbl[0][0];
	}
	return &dp_bp_tbl[inst][idx];
}

static inline struct q_info *get_dp_q_info(int inst, int idx)
{
	if ((inst < 0) || (inst  >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_q_tbl[0][0];
	}
	if ((idx < 0) ||
	    (idx  >= dp_port_prop[inst].info.cap.max_num_queues)) {
		pr_err("DPM: %s wrong queue_id=%d\n", __func__, idx);
		dp_die(__func__, idx,
		      dp_port_prop[inst].info.cap.max_num_queues);
		return &dp_q_tbl[0][0];
	}

	return &dp_q_tbl[inst][idx];
}

static inline struct inst_info *get_dp_prop_info(int inst)
{
	if ((inst < 0) || (inst  >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return &dp_port_prop[0].info;
	}
	return &dp_port_prop[inst].info;
}

static inline struct dp_subif_info *get_dp_port_subif(
	const struct pmac_port_info *port, u16 vap)
{
	/* Note: here we canot do accurate vap sanity check
	 *       We need call this API to initialize dp_port_info[][].subif_info
	 *       internal list related memory. At that time, subif_info content
	 *       not set yet
	 */
	if (vap >= dp_port_prop[port->inst].info.cap.max_num_subif) {
		pr_err("DPM: %s wrong vap=%u subif_max=%d dp_port=%d\n",
		       __func__, vap,
		       dp_port_prop[port->inst].info.cap.max_num_subif,
		       port->port_id);
		dp_die(__func__, vap, port->subif_max);
	}
	return &port->subif_info[vap];
}

static inline struct dev_mib *get_dp_port_subif_mib(struct dp_subif_info *sif)
{
	return &sif->mib;
}

/* TODO: Need to improve this API later */
static inline bool is_soc_lgm(int inst)
{
	struct inst_info *info;

	if ((inst < 0) || (inst >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return false;
	}
	info = get_dp_prop_info(inst);

	if (info->type == GSWIP32_TYPE)
		return true;
	return false;
}

/* TODO: Need to improve this API later */
static inline bool is_soc_prx(int inst)
{
	struct inst_info *info;

	if ((inst < 0) || (inst >= DP_MAX_INST)) {
		pr_err("DPM: %s wrong inst=%d\n", __func__, inst);
		dp_die(__func__, inst, DP_MAX_INST);
		return false;
	}
	info = get_dp_prop_info(inst);
	if (info->type == GSWIP31_TYPE)
		return true;
	return false;
}

/*Just find the first valid dp_port from the given array*/
static inline int dp_deq_find_a_dpport(u8 *dp_arr)
{
	int i;
	for (i = 0; i < MAX_DP_PORTS; i++)
		if(dp_arr[i])
			return i;
	return -1;
}

void dp_loop_eth_dev_exit(void);
char *dp_qos_flag_to_str(enum QOS_FLAG qf);
char *dp_arbi_to_str(enum dp_arbitate ar);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
struct dentry *dp_proc_install(void);
#else
static inline struct dentry *dp_proc_install(void) { return NULL;}
#endif

extern char *dp_dbg_flag_str[];
extern u64 dp_dbg_flag_list[];
extern char *dp_dbgfs_flag_str[];
extern u64 dp_dbgfs_flag_list[];
extern u32 dp_port_flag[];
extern char *dp_port_type_str[];
extern char *dp_port_status_str[];
extern struct parser_info pinfo[];

enum TEST_MODE {
	DP_RX_MODE_NORMAL = 0,
	DP_RX_MODE_LAN_WAN_BRIDGE,
	DPR_RX_MODE_MAX
};

extern struct platform_device *g_dp_dev;
extern u32 rx_desc_mask[4];
extern u32 tx_desc_mask[4];
extern const bool dp_dsable_optimize;

ssize_t proc_print_mode_write(struct file *file, const char *buf,
			      size_t count, loff_t *ppos);
void proc_print_mode_read(struct seq_file *s);
int parser_size_via_index(u8 index);
struct pmac_port_info *get_port_info_via_dev(struct net_device *dev);
void dp_clear_mib(dp_subif_t *subif, uint32_t flag);
extern u32 dp_drop_all_tcp_err;
extern u32 dp_pkt_size_check;
void print_parser_status(struct seq_file *s);
void proc_mib_timer_read(struct seq_file *s);
int mpe_fh_netfiler_install(void);
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
int dp_cpufreq_notify_init(int inst);
int dp_cpufreq_notify_exit(void);
#endif
int proc_qos_init(void *param);
int proc_qos_dump(struct seq_file *s, int pos);
int proc_sched_hal_dump(struct seq_file *s, int pos);
int proc_sched_child_hal_dump(struct seq_file *s, int pos);
ssize_t proc_qos_write(struct file *file, const char *buf,
		       size_t count, loff_t *ppos);
void dump_parser_flag(char *buf);

//int dp_reset_sys_mib(u32 flag);
void dp_clear_all_mib_inside(uint32_t flag);

extern int ip_offset_hw_adjust;
int register_netdev_notifier(u32 flag);
int unregister_netdev_notifier(u32 flag);
char *get_netdev_evt_name(int event);
//int supported_logic_dev(int inst, struct net_device *dev, char *subif_name);
struct net_device *get_base_dev(struct net_device *dev, int level);
int add_logic_dev(int inst, int port_id, struct net_device *dev,
		  dp_subif_t *subif_id, struct dp_subif_data *data, u32 flags);
int del_logic_dev(int inst, struct list_head *head, struct net_device *dev,
		  u32 flags);
int get_vlan_via_dev(struct net_device *dev, struct vlan_prop *vlan_prop);
void dp_parser_info_refresh(u32 cpu, u32 mpe1, u32 mpe2, u32 mpe3, u32 verify);
int dp_inst_init(u32 flag);
void dp_inst_free(void);
int request_dp(u32 flag);
int dp_init_module(void);
void dp_cleanup_module(void);
int dp_probe(struct platform_device *pdev);
#define NS_INT16SZ	 2
#define NS_INADDRSZ	 4
#define NS_IN6ADDRSZ	16

int low_10dec(u64 x);
int high_10dec(u64 x);
int dp_atoi(unsigned char *str);
u64 dp_atoull(unsigned char *str);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
int get_offset_clear_chksum(struct sk_buff *skb, u32 *ip_offset,
			    u32 *tcp_h_offset,
			    u32 *tcp_type);
#else
int get_offset_clear_chksum(struct sk_buff *skb, u32 *ip_offset,
			    u32 *tcp_h_offset, u16 *l3_csum_off,
			    u16 *l4_csum_off, u32 *tcp_type);
#endif
int dp_basic_proc(void);

struct pmac_port_info *get_port_info_via_dp_port(int inst, int dp_port);

void set_dp_dbg_flag(uint64_t flags);
uint64_t get_dp_dbg_flag(void);
void dp_dump_raw_data(const void *buf, int len, char *prefix_str);
char *dp_skb_csum_str(struct sk_buff *skb);
extern struct dentry *dp_proc_node;
int get_dp_dbg_flag_str_size(void);
int get_dp_dbgfs_flag_str_size(void);
int get_dp_port_status_str_size(void);

int dp_request_inst(struct dp_inst_info *info, u32 flag);
int register_dp_cap(u32 flag);
int bp_pmapper_get(int inst, struct net_device *dev);
extern int dp_init_ok;
extern int dp_cpu_init_ok;
void set_chksum(struct pmac_tx_hdr *pmac, u32 tcp_type,
		u32 ip_offset, int ip_off_hw_adjust, u32 tcp_h_offset);

#if IS_ENABLED(CONFIG_QOS_MGR)
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
extern int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev, u32 handle,
					__be16 protocol,
					struct tc_to_netdev *tc);
#else
extern int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev,
					enum tc_setup_type type,
					void *type_data);
#endif
#endif

#define DP_SUBIF_LIST_HASH_SHIFT 8
#define DP_SUBIF_LIST_HASH_BIT_LENGTH 10
#define DP_SUBIF_LIST_HASH_SIZE ((1 << DP_SUBIF_LIST_HASH_BIT_LENGTH) - 1)

int dp_get_drv_mib(dp_subif_t *subif, dp_drv_mib_t *mib, uint32_t flag);
extern struct hlist_head dp_subif_list[DP_SUBIF_LIST_HASH_SIZE];
int32_t dp_sync_subifid(struct net_device *dev, char *subif_name,
			dp_subif_t *subif_id, struct dp_subif_data *data,
			u32 flags);
int32_t dp_sync_subifid_priv(struct net_device *dev, char *subif_name,
			     dp_subif_t *subif_id, struct dp_subif_data *data,
			     u32 flags, dp_get_netif_subifid_fn_t subifid_fn,
			     int f_notif, bool no_notify);
int32_t	dp_update_subif(struct net_device *netif, void *data, dp_subif_t *subif,
			char *subif_name, u32 flags,
			dp_get_netif_subifid_fn_t subifid_fn);
int32_t	dp_del_subif(struct net_device *netif, void *data, dp_subif_t *subif,
		     char *subif_name, u32 flags);
struct dp_subif_cache *dp_subif_lookup_safe(
	struct hlist_head *head,
	const struct net_device *dev,
	void *data);
int dp_subif_list_init(void);
void dp_subif_list_free(void);
int parser_enabled(int ep, struct dma_rx_desc_1 *desc_1);
int dp_lan_wan_bridging(int port_id, struct sk_buff *skb);
int get_dma_chan_idx(int inst, u32 dma_chan);
int dp_get_dma_ch_num(int inst, int ep, int num_deq_port);
u32 alloc_dma_chan_tbl(int inst);
void free_dma_chan_tbl(int inst);
u32 alloc_dp_port_subif_info(int inst);
void free_dp_port_subif_info(int inst);
u32 dp_subif_hash(struct net_device *dev);
int dp_cbm_deq_port_enable(struct module *owner, int inst, int port_id,
			   int deq_port_idx, int num_deq_port, int flags,
			   u32 dma_ch_off);
int32_t dp_get_subifid_for_update(int inst, struct net_device *netif,
				  dp_subif_t *subif, uint32_t flags);
int do_tx_hwtstamp(int inst, int dpid, struct sk_buff *skb);
struct hlist_head *get_dp_g_bridge_id_entry_hash_table_info(int index);
struct dp_evt_notif_info {
	struct notifier_block nb;
	struct dp_event evt_info;
	struct list_head list;
};

struct dp_evt_notif_data {
	enum DP_DATA_PORT_TYPE type;
	enum DP_EVENT_OWNER owner;
	struct net_device *dev;
	struct module *mod;
	u32 dev_port;
	s32 subif;
	int dpid;
	int inst;
	int alloc_flag;
	union {
		struct dp_dev_data *dev_data; /*!< valid only
					       * for DP_EVENT_REGISTER_DEV
					       * event
					       */
	} data;
};

extern struct blocking_notifier_head dp_evt_notif_list;
int register_dp_event_notifier(struct dp_event *info);
int unregister_dp_event_notifier(struct dp_event *info);
void dp_set_tmp_inst(int);

#define dp_dump_debugfs(function) do {\
	int pos = 0; \
	do { \
		dp_set_tmp_inst(0); \
		pos = function(NULL, pos); \
	} while (pos >= 0); \
} while (0)

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS) && IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
#define dp_dump_debugfs_all(port, subif_grp) do {\
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_DPID) { \
		pr_info("\n------------> Dumping dp/dpid <-------------\n"); \
		dp_dump_debugfs(proc_dpid_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_PORT) { \
		pr_info("\n------------> Dumping dp/port portid: %d, subif_grp: %d <-------------\n", port, subif_grp); \
		dp_set_tmp_inst(0); \
		proc_port_dump_one(NULL, port); \
		pr_info("    Curr Subif:%d info:\n", subif_grp); \
		proc_subif_dump_one(NULL, port, subif_grp); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_DEQ) { \
		pr_info("\n------------> Dumping dp/deq <-------------\n"); \
		dp_dump_debugfs(proc_registration_deq_port_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_QOS) { \
		pr_info("\n------------> Dumping dp/qos <-------------\n"); \
		dp_dump_debugfs(proc_qos_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_ENQ) { \
		pr_info("\n------------> Dumping dp/enq <-------------\n"); \
		dp_dump_debugfs(proc_registration_enq_port_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_DEV) { \
		pr_info("\n------------> Dumping dp/dev <-------------\n"); \
		dp_dump_debugfs(proc_dev_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_DEVOPS) { \
		pr_info("\n------------> Dumping dp/dev_ops <-------------\n"); \
		dp_dump_debugfs(proc_dev_ops_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_BR) { \
		pr_info("\n------------> Dumping dp/br <-------------\n"); \
		dp_dump_debugfs(proc_br_dump); \
	} \
	if (dp_dbgfs_flag & DP_DBGFS_FLAG_HOOKS) { \
		pr_info("\n------------> Dumping dp/hooks <-------------\n"); \
		proc_dp_active_tx_hook_dump(NULL); \
		proc_dp_active_rx_hook_dump(NULL); \
	} \
} while (0)

#define DP_DUMP_DEBUGFS_QOS_ALL(...) do {\
	dp_dump_debugfs(proc_qos_dump); \
	dp_dump_debugfs(proc_sched_hal_dump); \
	dp_dump_debugfs(proc_sched_child_hal_dump); \
} while (0)
#else
#define dp_dump_debugfs_all(...)
#define DP_DUMP_DEBUGFS_QOS_ALL(...)
#endif

static inline bool is_directpath(struct pmac_port_info *port)
{
	return (port->alloc_flags & DP_F_DIRECT);
}

static inline bool is_dsl(struct pmac_port_info *port)
{
	return (port->alloc_flags & DP_F_FAST_DSL);
}

void dp_print_err_info(int res);
int dp_notifier_invoke(int inst,
		       struct net_device *dev, u32 port_id, u32 subif_id,
		       void *evt_data, enum DP_EVENT_TYPE type);
int proc_dp_event_list_dump(struct seq_file *s, int pos);

int dp_dealloc_cqm_port(struct module *owner, u32 dev_port,
			struct pmac_port_info *port,
			struct cbm_dp_alloc_data *data, u32 flags);

int dp_alloc_cqm_port(struct module *owner, struct net_device *dev,
		      u32 dev_port, s32 port_id,
		      struct cbm_dp_alloc_data *cbm_data, u32 flags);

int dp_enable_cqm_port(struct module *owner, struct pmac_port_info *port,
		       struct cbm_dp_en_data *data, u32 flags);

int dp_cqm_port_alloc_complete(struct module *owner,
			       struct pmac_port_info *port, s32 dp_port,
			       struct cbm_dp_alloc_complete_data *data,
			       u32 flags);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
int dp_cqm_gpid_lpid_map(int inst, struct cbm_gpid_lpid *map);
#endif

int dp_init_pce(void);
int proc_dpid_dump(struct seq_file *s, int pos);
int proc_port_dump_one(struct seq_file *s, int pos);
char *dp_get_sym_name_by_addr(void *symaddr, char *symname,
		const char *fail_str);
int proc_subif_dump_one(struct seq_file *s, int pos, int subif);
int proc_registration_deq_port_dump(struct seq_file *s, int pos);
int proc_registration_enq_port_dump(struct seq_file *s, int pos);
int proc_pce_dump(struct seq_file *s, int pos);
ssize_t proc_pce_write(struct file *file, const char *buf, size_t count,
		       loff_t *ppos);
void proc_dp_active_tx_hook_dump(struct seq_file *);
void proc_dp_active_rx_hook_dump(struct seq_file *);
int proc_br_dump(struct seq_file *s, int pos);
int proc_dev_ops_dump(struct seq_file *s, int pos);
int proc_dev_dump(struct seq_file *s, int pos);
int dp_ctp_dev_list_add(struct list_head *head, struct net_device *dev, int bp,
			int vap);
void dp_init_fn(u32 flags);
int dp_ctp_dev_list_del(struct list_head *head, struct net_device *dev);
int dp_update_shared_bp_to_subif(int inst, struct net_device *netif, int bp,
				 int portid);
void dump_cpu_data(int inst, struct cbm_cpu_port_data *p);
int proc_bp_pmapper_dump(struct seq_file *s, int pos);
int dp_free_deq_port(int inst, u8 ep, struct dp_dev_data *data,
			uint32_t flags);
int _dp_init_subif_q_map_rules(struct dp_subif_info *subif_info,
					    int spl_conn_igp);
int _dp_set_subif_q_lookup_tbl(struct dp_subif_info *subif_info,
			       int cls_idx,
			       /* spl_conn_igp_qid must be set if this
				* API call is for sp_conn queue mapping
				*/
			       int spl_conn_igp_qid,
			       /* reins_inst be must set if this API
				* call is for reinsertion queue mapping
				*/
			       int reins_inst);
int _dp_reset_subif_q_lookup_tbl(struct dp_subif_info *subif_info,
				 int cls_idx, int reins_inst);
int _dp_reset_q_lookup_tbl(int inst);
bool subif_bit8_workaround(struct dp_subif_info *subif_info);
u32 get_subif_q_map_reset_flag(int inst);
int dp_add_subif_spl_dev(int inst,
				   struct net_device *dev,
				   char *subif_name,
				   dp_subif_t *subif_id,
				   struct dp_subif_data *data,
				   u32 flags);
int dp_del_subif_spl_dev(int inst,
				   struct net_device *dev,
				   char *subif_name,
				   dp_subif_t *subif_id,
				   struct dp_subif_data *data,
				   u32 flags);

int dp_switchdev_register_notifiers(void);
void dp_switchdev_unregister_notifiers(void);
bool dp_valid_netif(const struct net_device *netif);
void dp_free_remaining_dev_list(void);
char *dp_strsep(char **stringp, const char *delim);
void dp_dump_addr(struct seq_file *s);
void dp_gdb_break(void);
int dp_set_cpu_mac(struct net_device *dev, bool reset);
extern int n_dp_bp;
bool dp_has_spare_bp(void);
int dp_datapath_dts_parse(void);
int proc_qos_cfg_dump(struct seq_file *s, int pos);
#if IS_ENABLED(CONFIG_OF)
void proc_qos_raw_dts_dump(struct seq_file *s);
#endif
void proc_qos_category_dump(struct seq_file *s);
int alloc_flag_str(int flag, char *buf, int buf_len);
ssize_t proc_dts_raw_write(struct file *file, const char *buf, size_t count,
			   loff_t *ppos);
void init_qos_setting(void);
struct dp_qos_setting* dp_get_qos_cfg(int inst, int dp_port, int alloc_flag, u32 qos_id);
struct dp_qos_setting* dp_get_inter_qos_cfg(int inst, u32 deq_port);

int dp_qos_get_q_global_parms(int inst, int dp_port, int alloc_flag, u32 qos_id,
			      struct dp_qos_q_parms *parms);


struct net_device *dp_create_netdev(const char *name);

/* These below apis used to seemless switch b/w seqfs and pr_info */
#define dp_sprintf(seq, fmt, ...) do {\
	if (seq) \
		seq_printf(seq, fmt, ##__VA_ARGS__); \
	else \
		pr_cont(fmt, ##__VA_ARGS__); \
} while (0)

static inline bool dp_seq_has_overflowed(struct seq_file *seq)
{
	bool ret = false;
	if (seq)
		ret = seq_has_overflowed(seq);
	return ret;
}

#define dp_sputc(seq, c) do {\
	if (seq) \
		seq_putc(seq, c); \
	else	\
		pr_cont("%c", c); \
} while (0)

#define dp_sputs(seq, str) do {\
	if (seq) \
		seq_puts(seq, str); \
	else \
		pr_cont("%s", str);	\
} while (0)

#endif /*DATAPATH_H */

