// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/dmaengine.h>
#include <linux/dma-map-ops.h>
#include <linux/errno.h>
#include <linux/genalloc.h>
#include <linux/interrupt.h>
#include <linux/iopoll.h>
#include <linux/io-64-nonatomic-lo-hi.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/if_pppox.h>

#include <net/datapath_api_gswip32.h>
#include <net/datapath_api_tx.h>
#include <net/mxl_cbm_api.h>
#include <net/toe_np_lro.h>
#include <net/ip.h>

#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>

#include <dt-bindings/soc/mxl,firewall.h>

#include "pktprs.h"

#define LRO_MAX_SEG_CNT        19
#define LRO_AGP_LIMIT          (LRO_MAX_SEG_CNT + 1)
#define LRO_DEFAULT_SPLIT_BOUNDARY	224

#define LRO_FID0_FID           GENMASK(11, 0)
#define LRO_FID0_MAX_AGP       GENMASK(16, 12)
#define LRO_FID0_TYPE          GENMASK(18, 17)
#define LRO_FID0_TIMEOUT_IDX   GENMASK(29, 27)
#define LRO_FID0_CPU_OWN       BIT(31)
#define LRO_FID0_SEND          BIT(30)

#define LRO_OC_AGG            BIT(0)
#define LRO_OC_STALL          BIT(2)
#define LRO_OC_EXP            GENMASK(6, 3)
#define LRO_OC_EA_SUSP        BIT(7)
#define LRO_OC_SEG_NR         GENMASK(14, 10)
#define LRO_OC_OWN_BY_CPU     BIT(31)

#define LRO_OC5_STALL_CNT     GENMASK(23, 16)
#define LRO_OC5_LRO_TAIL      GENMASK(31, 24)

#define LRO_SC_FID            GENMASK(27, 16)
#define LRO_SC_OV             BIT(31)

#define UD_DW1_H              GENMASK(15, 8)
#define UD_DW1_NH             GENMASK(31, 24)

#define UD_DW2_IFLAG          GENMASK(15, 8)
#define UD_DW2_FLOW           GENMASK(27, 16)
#define UD_DW2_LRO_EXP        BIT(28)

#define TSO_PORT_CNT          SZ_4
#define LRO_PORT_CNT          SZ_256
#define LRO_STALL_CNT         SZ_256
#define LRO_GROUP_CNT         BITS_PER_TYPE(u32)
#define LRO_PORT_PER_GROUP    (LRO_PORT_CNT / LRO_GROUP_CNT)
#define LRO_OC_DEPTH          SZ_2
#define LRO_EXP_DEPTH         SZ_2
#define LRO_NOMATCH_DEPTH     SZ_16

#define LRO_OC_SEG_LEN        GENMASK(15, 0)
#define LRO_OC_SEG_ADDRH      GENMASK(19, 16)
#define LRO_OC_SEG_POOL       GENMASK(23, 20)
#define LRO_OC_SEG_POLICY     GENMASK(31, 24)

#define TOE_INT_LRO_STALL     GENMASK(6, 5)
#define TOE_INT_LRO_EXP       BIT(4)
#define TOE_INT_TSO(n)        BIT(n)

#define TOE_GCTRL_RST_TSO_POS      22
#define TOE_GCTRL_RST_TSO(n)       BIT((n) + TOE_GCTRL_RST_TSO_POS)
#define TOE_GCTRL_TSO_OPTION3_POS  20
#define TOE_GCTRL_SWAP_WORD        BIT(19)
#define TOE_GCTRL_RST              BIT(16)
#define TOE_GCTRL_HDR_BUF_QUAD     BIT(7)
#define TOE_GCTRL_EN_TSO_POS       1
#define TOE_GCTRL_EN_TSO(n)        BIT((n) + TOE_GCTRL_EN_TSO_POS)
#define TOE_GCTRL_EN               BIT(0)

#define TSO_CMD5_CPU_OWN        BIT(31)
#define TSO_CMD5_TIRQ           BIT(30)
#define TSO_CMD5_ADDRH          GENMASK(23, 20)
#define TSO_CMD5_SPHY           BIT(19)
#define TSO_CMD5_DPHY           BIT(18)
#define TSO_CMD5_IE             BIT(17)
#define TSO_CMD5_PKTTYPE        GENMASK(16, 14)
#define TSO_CMD5_SEG_LEN        GENMASK(12, 0)

#define TSO_CMD0_ADDRH          GENMASK(31, 28)
#define TSO_CMD0_GATHER_POS     25
#define TSO_CMD0_LAST_POS       24
#define TSO_CMD0_SIOC           BIT(23)
#define TSO_CMD0_CHUNKSZ        GENMASK(22, 20)
#define TSO_CMD0_LEN            GENMASK(19, 0)

#define TSO_RES_SEG_NR         GENMASK(4, 0)
#define TSO_RES_LEN            GENMASK(22, 6)
#define TSO_RES_PKT_AH         GENMASK(26, 23)
#define TSO_RES_ERR_CODE       GENMASK(29, 27)
#define TSO_RES_ERR             BIT(30)
#define TSO_RES_DONE            BIT(31)

#define TIE_LRO_PORT_CHAIN      BIT(20)
#define TIE_LRO_RST_TO_ONRCV    BIT(19)
#define TIE_LRO_STALL_BIT       17
#define TIE_LRO_WEIGHT3         BIT(15)
#define TIE_LRO_WEIGHT2         BIT(14)
#define TIE_TSO_WEIGHT          BIT(13)
#define TIE_LRO_WEIGHT1         BIT(12)
#define TIE_TSO_POST_SEL_ENQ    BIT(3)
#define TIE_TSO_POST_SEL_SEG    BIT(2)
#define TIE_LRO_NONPOST_WRITE   BIT(1)
#define TIE_TSO_EXCL_PMAC       BIT(0)

#define LRO_TIMEOUT_TABLE_CNT   SZ_8

#define LRO_CTL_EXT_OC_EN       GENMASK(31, 30)
#define LRO_CTL_LAST_WR_NP_EN   BIT(29)
#define LRO_CTL_FO_XOC          BIT(13)
#define LRO_CTL_TS_XOC          BIT(12)
#define LRO_CTL_DSS_XOC         BIT(11)
#define LRO_CTL_SEQ_XOC         BIT(10)

#define TOE_TSO_CMD(port)      (0x20 * (port))
#define TOE_EXT_OC_DA          0x80
#define TOE_SPLIT_OC_DA        0x84
#define TOE_LRO_HEAD           0x94
#define TOE_LRO_THRESHOLD      0x95
#define TOE_TSO_RES(port)      (0x100 + 0x20 * (port))
#define TOE_TSO_GCTRL          0x200
#define TOE_TSO_HDR_BASE(port, bank) \
	(0x220 + 4 * ((port) + TSO_PORT_CNT * (bank)))
#define TOE_LRO_INT_MASK(n)    (0x300 + (n))
#define TOE_LRO_INT_EN(n)      (0x320 + (n))
#define TOE_LRO_INT_STS(n)     (0x340 + (n))
#define TOE_INT_MASK           0x360
#define TOE_INT_MASK_LRO_EXP   0x361
#define TOE_INT_EN             0x364
#define TOE_INT_EN_LRO_EXP     0x365
#define TOE_INT_STS            0x368
#define TOE_INT_STS_LRO_EXP    0x369
#define TOE_INT_STS_LRO_STALL  0x36A
#define TOE_INT_STS2EN(x)      ((x) + TOE_INT_EN - TOE_INT_STS)
#define TOE_INT_STS2MASK(x)    ((x) + TOE_INT_MASK - TOE_INT_STS)
#define TOE_TSO_DBG(n)         (0x36C + 4 * (n))
#define TOE_VERSION            GENMASK(31, 28)
#define TOE_TSO_INTL_INT_STS   0x380
#define TOE_TSO_INTL_INT_EN    0x384
#define TOE_TSO_INTL_INT_MASK  0x388
#define TOE_LRO_DBG            0x38C
#define TOE_MEM_BYPASS         0x390
#define TOE_SPARE_TIE0         0x394
#define TOE_LRO_CTRL           0x39C
#define TOE_LRO_EN             0x800
#define TOE_BUFF_OFFSET        0x820
#define TOE_LRO_EAK_EN         0x840
#define TOE_LRO_L1_EXP         0x900
#define TOE_LRO_NOMATCH        0x902
#define TOE_LRO_L2_EXP_BASE    0x910
#define TOE_LRO_FID(n)         (0x1000 + sizeof(struct lro_flow_id) * (n))
#define TOE_LRO_TIMEOUT(n)     (0x3000 + 0x20 * (n))
#define TOE_LRO_OC_FLAG(port, bank, n) \
	(0x18 * ((bank) + (port) * 2) + 4 * (n))

#define TSO_HDR_BUF_LEN         SZ_256
/* change SZ_4 to SZ_8 if TOE_GCTRL_HDR_BUF_QUAD is set */
#define TSO_HDR_BUF_SZ          (TSO_HDR_BUF_LEN * SZ_4)

static int max_gso_sz = GSO_MAX_SIZE;

module_param(max_gso_sz, int, S_IRUGO);
MODULE_PARM_DESC(max_gso_sz, "Max GSO size, max = 65536");

#define TSO_MAX_GATHER_BUF	(TSO_PORT_CNT * SZ_2)
#define TSO_GSO_MAX_SIZE	max_gso_sz
#define TSO_GATHER_BUF_SZ       (TSO_GSO_MAX_SIZE + TSO_HDR_BUF_LEN)
#define TSO_MAX_SLICE           SZ_2
#define TSO_MIN_GAE             SZ_2
#define TSO_MAX_GAE             SZ_4 /* concurrent gather engines */
#define TOE_CLK_MHZ             600
#define TSO_DMA_CH_PER_PORT     12
#define TSO_MAX_MSS             (SZ_8K - 1)
#define TSO_FSM_CNT             3
#define TSO_FSM_ENQ_LO_MASK     GENMASK(3, 0)
#define TSO_FSM_ENQ_HI_MASK     GENMASK(5, 4)
#define TSO_HWQ_DEPTH           16
#define TSO_CMDCNT_DEPTH        (TSO_HWQ_DEPTH * TSO_MAX_SLICE)
#define TSO_CMDCNT_IDX(x)       ((x) & (TSO_CMDCNT_DEPTH - 1))
#define TSO_CMDCNT_MAX_UNALIGN  2   /* linear data + first fragment */
#define TSO_CMDCNT_MAXREQ(nfrags) ((nfrags) + 1 + TSO_CMDCNT_MAX_UNALIGN)
#define TSO_DEFAULT_LOW_WM      12
#define LRO_DEFAULT_TIMEOUT     SZ_64 /* around 64 microseconds */

#define LRO_PORT_STATE_INUSE    0x1  /* Port is in use */
#define LRO_PORT_STATE_STOP     0x2  /* lro stop triggered on Port */
#define LRO_PORT_STATE_RETRY    0x4  /* lro stop triggered but Port busy */

#define INVALID_FID(x)          ((x) >= LRO_PORT_CNT || (x) < 0)
#define CMD_FIFO_SIZED_SKB_FRAGS (TSO_HWQ_DEPTH - (TSO_CMDCNT_MAX_UNALIGN \
				  + 1)) /* max nr_frags for CMD FIFO */
#define TOE_TC_DEF_VALUE	((u8)-1)

enum TSO_GATHER_BUF_TYPE {
	TSO_GB_SRAM,
	TSO_GB_DDR,
};

enum LRO_OC_FLAG_IDX {
	OC_FLAG,
	OC_ACK_SN,
	OC_DSS,
	OC_MPTCP_LEN,
	OC_MPTCP_WIN,
	OC_MPTCP_TS,
	OC_FLAG_CNT,
};

enum LRO_EXCEPTION_REASON {
	LRO_EXP_PPV4,
	LRO_EXP_ZLEN,
	LRO_EXP_TS,
	LRO_EXP_ELEN,
	LRO_EXP_TIMEOUT,
	LRO_EXP_IP,
	LRO_EXP_ID,
	LRO_EXP_FRAG,
	LRO_EXP_XDSS,
	LRO_EXP_XDSN,
	LRO_EXP_SEQ,
	LRO_EXP_SSN,
	LRO_EXP_DSN,
	LRO_EXP_FEH,
	LRO_EXP_TYPE,
	LRO_EXP_OK,
	LRO_EXP_MAX = LRO_EXP_OK,
};

enum LRO_RESULT_TYPE {
	LRO_RSLT_STALL = LRO_EXP_MAX,
	LRO_RSLT_SC_OV,
	LRO_RSLT_NOMATCH,
	LRO_RSLT_AGG,
};

enum TSO_PKT_TYPE {
	TSO_IPV4_UDP = 0,
	TSO_IPV4_TCP,
	TSO_IPV4_MPTCP,
	TSO_IPV6_OFFSET = 4,
	TSO_IPV6_UDP = TSO_IPV6_OFFSET,
	TSO_IPV6_TCP,
	TSO_IPV6_MPTCP,
	TSO_TYPE_MAX,
	TSO_TYPE_INVALID,
};

enum TOE_IOMEM_RESOURCE {
	TOE_IOMEM_REGS,
	TOE_IOMEM_OCFLAG,
	TOE_IOMEM_LRO_DMA,
	TOE_IOMEM_CNT,
};

enum LRO_CACHE_TYPE {
	OC_CACHE,
	EC_CACHE,
	SC_CACHE,
	SC_CNT_CACHE,
	LRO_CACHE_CNT,
};

enum TSO_GATHER {
	TSO_GATHER_OFF,
	TSO_GATHER_ON,
};

enum TSO_FRAG {
	TSO_FRAG_NON_LAST,
	TSO_FRAG_LAST,
};

enum TOE_FW_DOM {
	FW_DOM_TOE,
	FW_DOM_TOE_DMA,
	FW_DOM_MAX
};

enum TOE_REV {
	TOE_REV_A = 0xa,
	TOE_REV_B = 0xb,
	TOE_REV_C = 0xc,
};

enum EXT_OC_TYPE {
	EXT_OC_INT,
	EXT_OC_SPLT,
	EXT_OC_EXT,
	EXT_OC_LEG
};

struct lro_result_ctx {
	struct dp_dma_desc head;
	struct {
		u32 dw[SZ_2];
	} segs[LRO_MAX_SEG_CNT];
};

struct tso_gb {
	struct list_head list;
	dma_addr_t gather_buf;
	unsigned int id;
	unsigned int type;
	bool in_used;
} ____cacheline_aligned_in_smp;

struct lro_dma_descs {
	struct lro_result_ctx oc[LRO_PORT_CNT][LRO_OC_DEPTH];
	struct dp_dma_desc nomatch[LRO_NOMATCH_DEPTH];
	struct dp_dma_desc exception[LRO_PORT_CNT][LRO_EXP_DEPTH];
	u32 stall_flag[LRO_PORT_CNT];
	struct dp_dma_desc stall_desc[LRO_STALL_CNT];
} __attribute__((__packed__));

struct tso_cmd {
	union {
		u32 cmd[6];
		u64 qcmd[3];
	};
};

struct tso_res {
	u32 addr;
	u32 flag;
};

struct lro_flow_id {
	u32 reg[SZ_8];
};

#define TP_MAX_DEV_TXQ 16 /* 8 netdev * 2 txqs per TSO port */
struct tso_wq_dev {
	struct net_device *dev;
	unsigned int tx_qindex;
};

struct tso_port {
	struct tso_cmd *cmd ____cacheline_aligned_in_smp;
	struct dma_chan *pch[TSO_DMA_CH_PER_PORT];

	/* record # of CMD FIFO used for each skb */
	u8 cmd_cnt[TSO_CMDCNT_DEPTH];

	union {
		struct {
			/* skb queued */
			unsigned int queue_cnt;

			/* skb completed, including err_cnt */
			unsigned int cmpl_cnt;

			/* CMD fifo queued */
			u64 cmd_enq;

			/* CMD fifo dequeued, including failure ones */
			u64 cmd_deq;

			/* skb failed to process */
			unsigned int err_cnt;

			/* # of DP_TX_FN_BUSY returned due
			 * to port busy since last check
			 */
			unsigned int port_busy;

			/* # of DP_TX_FN_BUSY returned due
			 * to CMD FIFO exhausted since last check
			 */
			unsigned int cmd_busy;

			/* # of times a queue has been restarted returned due
			 * to cmd_busy
			 */
			unsigned int wake_queue;
			/* # of device tx queues wake pending */
			unsigned int wake_pending;
			/* # of no gather buf available when sending cmd */
			unsigned int no_gb;

			/* gather buf type stats */
			unsigned int sram_gb;
			unsigned int ddr_gb;

			/* skb with high frags */
			u64 hi_frags;
		};
		unsigned int tso_stats[16];
	} ____cacheline_aligned_in_smp;

	struct toe_dev *toe;

	/* lock for CMD FIFO */
	spinlock_t lock;

	/* device wait queue for which wake pending */
	struct {
		struct tso_wq_dev q[TP_MAX_DEV_TXQ];
		unsigned int head;
		unsigned int count;
		/* lock for the wait q */
		spinlock_t lock;
	} wait_q;

	struct tso_res *res ____cacheline_aligned_in_smp;
	void __iomem *sts;
	unsigned long flag;

	/* port id starting from 0 */
	int id;

	/* port in used */
	atomic_t used;
};

struct lro_store_common {
	struct lro_store_common *next;
	int result;
};

struct lro_sc_store {
	struct lro_sc_store *next;
	struct dp_dma_desc stall;
};

struct lro_group {
	struct toe_dev *toe;
	u8 *sts;
	/* LRO port offset */
	int offset;
	int irq;
};

struct lro_port {
	struct toe_dev *toe;

	/* link list of OC, EC store and SC cnt */
	struct lro_store_head {
		struct lro_store_common *head;
		struct lro_store_common **tail;
	} store;

	/* link list of SC store */
	struct lro_sc_head {
		struct lro_sc_store *head;
		struct lro_sc_store **tail;
		struct lro_sc_store *last;
	} stall;

	struct work_struct wk;

	struct completion wait_stall;

	/* protects store */
	spinlock_t store_lock;

	/* protects stall */
	struct mutex stall_mtx;

	unsigned int gso_type;
	unsigned int rx_cnt;
	unsigned int oc_cnt;
	unsigned int ec_cnt;
	unsigned int sc_cnt;
	unsigned int sc_acc_cnt;
	unsigned int ov_cnt;
	int id;
};

struct lro_oc_store {
	struct lro_store_common cmn;
	struct lro_result_ctx oc;
	int nr_frags;
};

struct lro_ec_store {
	struct lro_store_common cmn;
	struct dp_dma_desc ec;
};

struct lro_sc_cnt {
	struct lro_store_common cmn;
	int port;
	int nr_segs;
};

struct toe_dev {
	struct tso_port *tso[TSO_PORT_CNT];
	struct lro_port *lro[LRO_PORT_CNT];
	struct lro_group group[LRO_GROUP_CNT];
	struct kmem_cache *cache[LRO_CACHE_CNT];
	struct work_struct lro_sc_wk;
	struct delayed_work lro_exp_wk;

	u32 stall_flag[LRO_PORT_CNT];
	struct dp_dma_desc stall_desc[LRO_STALL_CNT];

	int tso_active_ports;
	int tso_low_wm;
	unsigned long tie;
	unsigned int stall_idx;
	int dpid;
	unsigned int nomatch_cnt;
	unsigned int ip_len_mismatch_cnt;
	bool tso_option3;
	bool tso_disable;
	u32 rev;

	struct lro_dma_descs *lro_ssb;
	enum EXT_OC_TYPE oc_mode;
	/* number of OCs in internal SRAM if OC_SPLT is used */
	u32 split_boundary;

	void __iomem *base[TOE_IOMEM_CNT];
	struct reset_control *reset;
	struct dentry *debugfs;
	struct clk *clk;
	struct device *dev;
	struct lro_ops ops;

	struct gen_pool *pool;
	size_t sram_sz;
	dma_addr_t sram_pa;
	void *sram_va;

	struct gen_pool *ddr_pool;
	size_t ddr_sz;
	unsigned long ddr_va;

	int sai[FW_DOM_MAX];

	void *pktprs_priv;
	struct delayed_work wk_cleanup;

	unsigned long lro_groups[LRO_GROUP_CNT];

	/*protects Gather Buffer*/
	spinlock_t gb_lock;
	/* protects TOE_LRO_INT_EN and TOE_LRO_EN */
	spinlock_t en_lock[LRO_PORT_CNT / BITS_PER_TYPE(u32)];
	int gb_num;
	struct list_head gb_list;
	struct tso_gb gb[TSO_MAX_GATHER_BUF];
};

struct lro_port_state {
	int index;   /* LRO port index 0..255 */
	unsigned int state_flags;
};

static struct lro_port_state lro_port_state_table[LRO_PORT_CNT];

static int lro_fid_to_port(struct toe_dev *toe, int fid);

static inline void __iomem *toe_reg(struct toe_dev *toe, int offset)
{
	return toe->base[TOE_IOMEM_REGS] + offset;
}

static inline void __iomem *toe_ocflag(struct toe_dev *toe, int offset)
{
	return toe->base[TOE_IOMEM_OCFLAG] + offset;
}

static inline void toe_update_bits(void __iomem *reg, u32 mask, u32 val)
{
	u32 tmp = readl(reg);

	tmp &= ~mask;
	tmp |= val & mask;
	writel(tmp, reg);
}

static inline phys_addr_t toe_get_phys_addr(struct device *dev, void *virt,
					    unsigned int len)
{
	return virt_to_phys(virt);
}

static inline unsigned int get_addr_high(dma_addr_t addr)
{
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
	return upper_32_bits(addr);
#else
	return 0;
#endif
}

static inline void lro_memcpy_fromio(void *buffer,
				     const volatile void __iomem *addr,
				     size_t size, bool invalidate)
{
	if (invalidate)
		clflush_cache_range(__io_virt(addr), size);

	/* memcpy_fromio() in x86 lib uses 32-bit (movsl) instruction.
	 * To get better performance we use generic way of memcpy(), which will
	 * eventually use 64-bit (movsq) instruction.
	 */
	memcpy(buffer, __io_virt(addr), size);
}

static inline u32 tso_make_cmd0(unsigned int len, enum TSO_FRAG last,
				enum TSO_GATHER gather,
				unsigned int src_addr_high, bool sioc)
{
	u32 val = FIELD_PREP(TSO_CMD0_ADDRH, src_addr_high) |
		gather << TSO_CMD0_GATHER_POS |
		last << TSO_CMD0_LAST_POS |
		FIELD_PREP(TSO_CMD0_CHUNKSZ, 2) |
		FIELD_PREP(TSO_CMD0_LEN, len);

	if (sioc)
		val |= TSO_CMD0_SIOC;

	return val;
}

static inline u32 tso_make_cmd5(unsigned int mss, unsigned int pkt_addr_high,
				enum TSO_PKT_TYPE type)
{
	return FIELD_PREP(TSO_CMD5_SEG_LEN, mss) | TSO_CMD5_TIRQ |
		TSO_CMD5_SPHY | TSO_CMD5_DPHY | TSO_CMD5_IE |
		FIELD_PREP(TSO_CMD5_ADDRH, pkt_addr_high) |
		FIELD_PREP(TSO_CMD5_PKTTYPE, type);
}

static inline int lock_tso_port(struct tso_port *tp)
{
	if (atomic_add_unless(&tp->used, 1, 1))
		return 1;

	return 0;
}

static inline void unlock_tso_port(struct tso_port *tp)
{
	atomic_dec(&tp->used);
}

static inline void tso_gb_stat(struct tso_port *tp, unsigned int type)
{
	if (type == TSO_GB_SRAM)
		tp->sram_gb++;
	else
		tp->ddr_gb++;
}

static inline struct tso_gb *tso_pop_gather_buf(struct tso_port *tp,
						struct sk_buff *skb)
{
	struct toe_dev *toe = tp->toe;
	struct tso_gb *gb = NULL;
	unsigned long flags;

	spin_lock_irqsave(&toe->gb_lock, flags);
	if (!list_empty(&toe->gb_list)) {
		gb = list_first_entry(&toe->gb_list, struct tso_gb, list);
		list_del(&gb->list);
		gb->in_used = true;
		skb->DW3 = gb->id;
	}
	spin_unlock_irqrestore(&toe->gb_lock, flags);

	return gb;
}

static inline void tso_push_gather_buf(struct tso_port *tp,
				       struct sk_buff *skb)
{
	struct toe_dev *toe = tp->toe;
	struct tso_gb *gb;

	spin_lock(&toe->gb_lock);
	if (skb->DW3 < toe->gb_num) {
		gb = &toe->gb[skb->DW3];
		gb->in_used = false;
		if (gb->type == TSO_GB_SRAM)
			list_add(&gb->list, &toe->gb_list);
		else
			list_add_tail(&gb->list, &toe->gb_list);
		tso_gb_stat(tp, gb->type);
	}
	spin_unlock(&toe->gb_lock);
}

static void tso_port_enq(struct tso_port *tp, struct tso_cmd *buf)
{
	struct tso_cmd *regs = tp->cmd;

	while (!(readl(&regs->cmd[5]) & TSO_CMD5_CPU_OWN))
		cpu_relax();

	tp->cmd_cnt[TSO_CMDCNT_IDX(tp->queue_cnt)]++;
	writeq_relaxed(buf->qcmd[0], &regs->qcmd[0]);
	if (buf->qcmd[1])
		writeq_relaxed(buf->qcmd[1], &regs->qcmd[1]);

	writeq(buf->qcmd[2], &regs->qcmd[2]);
}

static void tso_frag(struct device *dev, struct tso_port *tp, dma_addr_t phys,
		     unsigned int len, unsigned int mss, unsigned long priv,
		     enum TSO_PKT_TYPE type, enum TSO_FRAG last,
		     enum TSO_GATHER gather, u64 qw, bool sioc)
{
	struct tso_cmd buf;
	unsigned int high;

	if (IS_ENABLED(CONFIG_LGM_MM_HYBRID)) {
		if (WARN(phys < SZ_1M || phys >= SZ_256M,
			 "SKB buffer %pad is not within DDR range\n", &phys))
			return;
	}

	high = get_addr_high(phys);
	buf.cmd[0] = tso_make_cmd0(len, last, gather, high, sioc);
	buf.cmd[1] = phys;
	buf.qcmd[1] = qw;
	buf.cmd[4] = priv;
	buf.cmd[5] = tso_make_cmd5(mss, get_addr_high(priv), type);

	tso_port_enq(tp, &buf);
}

static unsigned int unaligned_length(dma_addr_t phys, unsigned int len)
{
	dma_addr_t aligned_addr = round_up(phys, SMP_CACHE_BYTES);
	unsigned int unaligned = aligned_addr - phys;

	return unaligned >= len ? 0 : unaligned;
}

static void tso_prog(struct device *dev, struct tso_port *tp, dma_addr_t phys,
		     unsigned int len, unsigned int mss, unsigned long priv,
		     enum TSO_PKT_TYPE type, enum TSO_FRAG last,
		     enum TSO_GATHER gather, u64 qw, bool sioc)
{
	unsigned int unaligned;

	unaligned = unaligned_length(phys, len);
	if (!unaligned) {
		tso_frag(dev, tp, phys, len, mss, priv, type, last, gather, qw,
			 sioc);
		return;
	}

	tso_frag(dev, tp, phys, unaligned, mss, priv, type, TSO_FRAG_NON_LAST,
		 TSO_GATHER_ON, qw, sioc);
	tso_frag(dev, tp, phys + unaligned, len - unaligned, mss, priv, type,
		 last, TSO_GATHER_ON, qw, sioc);
}

static void tso_xmit_passthrough(struct device *dev, struct tso_port *tp,
				 struct sk_buff *skb,
				 unsigned int mss, enum TSO_PKT_TYPE type,
				 bool sioc)
{
	phys_addr_t phys = toe_get_phys_addr(dev, skb->data, skb->len);
	unsigned long priv = virt_to_phys((void *)skb);
	u64 qw = *(u64 *)&skb->DW0;

	WARN(skb_shinfo(skb)->nr_frags, "%s can't deal with fragments\n",
	     __func__);
	skb->DW3 = -1;
	tso_frag(dev, tp, phys, skb->len, mss, priv, type,
		 TSO_FRAG_LAST, TSO_GATHER_OFF, qw, sioc);
}

static inline void tso_prog_frag(struct device *dev, struct tso_port *tp,
				 const skb_frag_t *frag, unsigned int mss,
				 unsigned long priv, enum TSO_PKT_TYPE type,
				 enum TSO_FRAG last, bool sioc)
{
	unsigned int len = skb_frag_size(frag);
	phys_addr_t phys;

	phys = toe_get_phys_addr(dev, skb_frag_address(frag), len);
	tso_prog(dev, tp, phys, len, mss, priv, type, last, TSO_GATHER_ON, 0,
		 sioc);
}

static int tso_xmit_normal(struct device *dev, struct tso_port *tp,
			   const struct sk_buff *skb, unsigned int mss,
			   enum TSO_PKT_TYPE type, bool sioc, struct tso_gb *gb)
{
	const struct skb_shared_info *shinfo = skb_shinfo(skb);
	unsigned int len = skb_headlen(skb);
	u64 qw = *(u64 *)&skb->DW0;
	const skb_frag_t *frag;
	unsigned long priv;
	int last_frag_idx;
	phys_addr_t phys;
	int i;

	priv = gb->gather_buf;
	WARN(!len, "skb headlen = 0\n");
	WARN(!skb_shinfo(skb)->nr_frags, "skb without fragments\n");
	/* TSO can only handle up to 8191 bytes */
	if (WARN(mss > TSO_MAX_MSS, "invalid mss %d\n", mss))
		mss = TSO_MAX_MSS;

	phys = toe_get_phys_addr(dev, skb->data, len);
	tso_prog(dev, tp, phys, len, mss, priv, type,
		 TSO_FRAG_NON_LAST, TSO_GATHER_ON, qw, sioc);

	frag = &shinfo->frags[0];
	last_frag_idx = shinfo->nr_frags - 1;
	priv = virt_to_phys((void *)skb);
	for (i = 0; i < last_frag_idx; i++) {
		tso_prog_frag(dev, tp, frag, mss, priv, type,
			      TSO_FRAG_NON_LAST, sioc);
		frag++;
	}
	/* last fragment */
	tso_prog_frag(dev, tp, frag, mss, priv, type, TSO_FRAG_LAST, sioc);
	return 0;
}

static enum TSO_PKT_TYPE tso_get_type(const struct pmac_tx_hdr *pmac,
				      struct skb_shared_info *si)
{
	WARN_ON(pmac->ip_offset < 7 || pmac->tcp_h_offset < 5);
	switch (pmac->tcp_type) {
	case TCP_OVER_IPV4:
		return TSO_IPV4_TCP;
	case TCP_OVER_IPV6:
		return TSO_IPV6_TCP;
	case UDP_OVER_IPV4:
		return si->gso_type & SKB_GSO_UDP_L4 ?
			TSO_IPV4_UDP : TSO_TYPE_INVALID;
	case UDP_OVER_IPV6:
		return si->gso_type & SKB_GSO_UDP_L4 ?
			TSO_IPV6_UDP : TSO_TYPE_INVALID;
	default:
		return TSO_TYPE_INVALID;
	}
}

/* note: called from tso_isr() that is in hardirq context */
static void tso_port_maybe_wake_tx(struct tso_port *tp)
{
	struct toe_dev *toe = tp->toe;
	struct tso_wq_dev *wd;
	struct netdev_queue *tx_q;
	int pending = READ_ONCE(tp->cmd_enq) - READ_ONCE(tp->cmd_deq);

	if (pending >= toe->tso_low_wm)
		return;

	/* wake up all the pending device txqs to give fair chance
	 * and also to avoid any starvation or delay
	 */
	spin_lock(&tp->wait_q.lock);
	while (tp->wait_q.count) {
		wd = &tp->wait_q.q[tp->wait_q.head];
		tx_q = netdev_get_tx_queue(wd->dev, wd->tx_qindex);
		if (netif_tx_queue_stopped(tx_q)) {
			netif_tx_wake_queue(tx_q);
			tp->wake_queue++;

			dev_dbg(toe->dev,
				"Wake Queue- dev: %s port : %d txq: %d\n",
				wd->dev->name, tp->id, wd->tx_qindex);
		}

		dev_put(wd->dev);
		memset(wd, 0, sizeof(struct tso_wq_dev));
		tp->wait_q.head = (tp->wait_q.head + 1) % TP_MAX_DEV_TXQ;
		tp->wait_q.count--;
	}
	tp->wake_pending = tp->wait_q.count;
	spin_unlock(&tp->wait_q.lock);
}

static bool tso_process_resp(struct tso_port *tp)
{
	struct sk_buff *skb;
	struct tso_res res;

	res.addr = readl(&tp->res->addr);
	if (unlikely(!res.addr))
		return false;

	writeb(tp->flag, tp->sts);
	writel(0, &tp->res->addr);
	res.flag = readl(&tp->res->flag);
	skb = phys_to_virt(DP_MAKE_PHYS(FIELD_GET(TSO_RES_PKT_AH, res.flag),
					res.addr));
	if (unlikely(res.flag & TSO_RES_ERR)) {
		dev_warn(tp->toe->dev, "TSO port%d skb:%p err:%lx\n", tp->id,
			 skb, FIELD_GET(TSO_RES_ERR_CODE, res.flag));
		dev_kfree_skb_irq(skb);
		tp->err_cnt++;
	} else {
		WARN(!(res.flag & TSO_RES_DONE), "invalid TSO result %X",
		     res.flag);
		dev_consume_skb_irq(skb);
	}

	WRITE_ONCE(tp->cmd_deq, tp->cmd_cnt[TSO_CMDCNT_IDX(tp->cmpl_cnt)]
		   + tp->cmd_deq);
	WRITE_ONCE(tp->cmpl_cnt, tp->cmpl_cnt + 1);
	tso_push_gather_buf(tp, skb);

	pm_runtime_mark_last_busy(tp->toe->dev);
	pm_runtime_put_autosuspend(tp->toe->dev);

	return true;
}

static irqreturn_t tso_isr(int irq, void *dev_id)
{
	struct tso_port *tp = (struct tso_port *)dev_id;

	while (tso_process_resp(tp))
		continue;

	tso_port_maybe_wake_tx(tp);

	return IRQ_HANDLED;
}

static phys_addr_t setup_tso_port_hdr_buf(struct toe_dev *toe, int port,
					  phys_addr_t base)
{
	if (IS_ENABLED(CONFIG_LGM_MM_HYBRID))
		WARN(base < SZ_1M || base >= SZ_256M, "buffer may not work\n");
	WARN(base & 0xff, "TSO header buffer is not 256 aligned\n");
	writel(base, toe_reg(toe, TOE_TSO_HDR_BASE(port, 0)));
	base += TSO_HDR_BUF_SZ;

	if (toe->tso_option3) {
		writel(base, toe_reg(toe, TOE_TSO_HDR_BASE(port, 1)));
		base += TSO_HDR_BUF_SZ;
	}
	return base;
}

static int tso_port_set_gather_buf(struct toe_dev *toe, int buf_sz,
				   phys_addr_t base, int start_idx, int type)
{
	int i = start_idx, sz = buf_sz;

	if (IS_ENABLED(CONFIG_LGM_MM_HYBRID))
		WARN(base < SZ_1M || base >= SZ_256M, "buffer may not work\n");
	WARN(base & 0xff, "TSO gather buffer is not 256 aligned\n");
	while (sz >= TSO_GATHER_BUF_SZ && i < TSO_MAX_GATHER_BUF) {
		toe->gb[i].gather_buf = base;
		toe->gb[i].type = type;
		toe->gb[i].id = i;
		list_add_tail(&toe->gb[i].list, &toe->gb_list);
		base += TSO_GATHER_BUF_SZ;
		sz -= TSO_GATHER_BUF_SZ;
		i++;
	}
	return i - start_idx;
}

static int toe_reset(struct toe_dev *toe)
{
	void __iomem *reg = toe_reg(toe, TOE_TSO_GCTRL);
	unsigned long v = TOE_GCTRL_EN | TOE_GCTRL_RST;
	int ret;

	writel(v, reg);
	ret = readl_poll_timeout(reg, v, !(v & TOE_GCTRL_RST), 100, 10000);
	if (ret)
		return ret;

	v = TOE_GCTRL_EN;
	writel(v, reg);
	return 0;
}

static void lro_port_work(struct work_struct *work);
static void lro_port_work_cleanup(struct work_struct *work);

static inline struct lro_group *lro_get_group(struct toe_dev *toe, int port)
{
	return &toe->group[port / LRO_PORT_PER_GROUP];
}

static void del_fid(struct toe_dev *toe, int fid)
{
	int group;
	int off;

	group = fid / LRO_PORT_PER_GROUP;
	off = fid % LRO_PORT_PER_GROUP;
	__clear_bit(off, &toe->lro_groups[group]);
}

static int find_free_lro_group(struct toe_dev *toe, int start, int end,
			       const struct cpumask *cpumask)
{
	u32 min_weight = LRO_PORT_PER_GROUP;
	struct cpumask *group_cpumask;
	unsigned long bm;
	int group = -1;
	u32 weight;
	int i;

	/* look for group with lowest usage */
	for (i = start; i < end; i++) {
		if (cpumask) {
			group_cpumask =	irq_get_effective_affinity_mask(toe->group[i].irq);
			if (!cpumask_intersects(group_cpumask, cpumask)) {
				/* skip groups with unmatched mask */
				dev_dbg(toe->dev, "Skip group %d mask %lx:%lx\n",
					group, *cpumask_bits(group_cpumask),
					*cpumask_bits(cpumask));
				continue;
			}
		}

		bm = toe->lro_groups[i];
		weight = hweight8(bm);
		if (weight < min_weight) {
			min_weight = weight;
			group = i;
		}
	}

	return group;
}

static int find_free_lro_port(struct toe_dev *toe,
			      const struct cpumask *cpumask)
{
	u32 ext_group = LRO_GROUP_CNT;
	int fid = -1;
	int group = -1;
	u32 offset;

retry:
	/* In split mode, we first look for ports in external OC, as they are
	 * faster than internal ones.
	 */
	if (toe->oc_mode == EXT_OC_SPLT) {
		ext_group = toe->split_boundary / LRO_PORT_PER_GROUP;
		group = find_free_lro_group(toe, ext_group, LRO_GROUP_CNT,
					    cpumask);
		if (group >= 0)
			goto port_found;
	}

	/* Look from remaining ports */
	group = find_free_lro_group(toe, 0, ext_group, cpumask);
	if (group >= 0)
		goto port_found;

	if (cpumask) {
		/* Unable to find port with requested cpumask. Retry without
		 * cpumask.
		 */
		cpumask = NULL;
		goto retry;
	}

	dev_err(toe->dev, "Unable to find free LRO port\n");
	return -ENOMEM;

port_found:
	offset = ffz(toe->lro_groups[group]);
	fid = group * LRO_PORT_PER_GROUP + offset;
	__set_bit(offset, &toe->lro_groups[group]);
	lro_port_state_table[fid].state_flags |= LRO_PORT_STATE_INUSE;
	return fid;
}

static void toe_lro_en(struct toe_dev *toe, int port, bool enable)
{
	u32 offset = port / BITS_PER_TYPE(u32);
	u32 val = BIT(port % BITS_PER_TYPE(u32));

	spin_lock_bh(&toe->en_lock[offset]);
	toe_update_bits(toe_reg(toe, TOE_LRO_INT_EN(0) + offset * sizeof(u32)),
			val, enable ? val : 0);
	toe_update_bits(toe_reg(toe, TOE_LRO_EN + offset * sizeof(u32)),
			val, enable ? val : 0);
	spin_unlock_bh(&toe->en_lock[offset]);
}

static int lro_start(struct toe_dev *toe, int fid, enum LRO_TYPE type)
{
	struct lro_port *lp;
	void __iomem *reg;
	unsigned int v;
	int port;

	WARN(in_irq(), "%s should not be called from ISR context\n", __func__);
	if (INVALID_FID(fid)) {
		dev_err(toe->dev, "invalid fid %d\n", fid);
		return -EINVAL;
	}

	port = lro_fid_to_port(toe, fid);

	if (toe->lro[port])
		return -EEXIST;

	dev_dbg(toe->dev, "start port = %d\n", port);

	reg = toe_reg(toe, TOE_LRO_FID(port));
	if (readl_poll_timeout_atomic(reg, v, v & LRO_FID0_CPU_OWN, 0, 10000))
		return -EBUSY;

	lp = devm_kzalloc(toe->dev, sizeof(*lp), GFP_ATOMIC);
	if (!lp)
		return -ENOMEM;

	switch (type) {
	case LRO_TYPE_TCP:
	case LRO_TYPE_MPTCP:
		lp->gso_type = SKB_GSO_TCPV4;
		break;
	case LRO_TYPE_TCPV6:
	case LRO_TYPE_MPTCPV6:
		lp->gso_type = SKB_GSO_TCPV6;
		break;
	case LRO_TYPE_UDP:
	case LRO_TYPE_UDPV6:
		lp->gso_type = SKB_GSO_UDP_L4;
		break;
	default:
		dev_err(toe->dev, "invalid LRO type:%d\n", type);
		devm_kfree(toe->dev, lp);
		return -EINVAL;
	}

	lp->toe = toe;
	lp->id = port;
	lp->store.tail = &lp->store.head;
	lp->stall.tail = &lp->stall.head;
	spin_lock_init(&lp->store_lock);
	mutex_init(&lp->stall_mtx);
	INIT_WORK(&lp->wk, lro_port_work);
	init_completion(&lp->wait_stall);
	pm_runtime_get_sync(toe->dev);

	toe->lro[port] = lp;
	toe_lro_en(toe, port, true);
	writel(FIELD_PREP(LRO_FID0_TIMEOUT_IDX, 0) |
	       FIELD_PREP(LRO_FID0_TYPE, type) |
	       FIELD_PREP(LRO_FID0_MAX_AGP, MAX_SKB_FRAGS + 1) |
	       FIELD_PREP(LRO_FID0_FID, fid), reg);
	return 0;
}

static void lro_set_threshold(struct toe_dev *toe, u16 tcp_payload_len)
{
	void __iomem *reg = toe_reg(toe, TOE_LRO_THRESHOLD);

	writel(tcp_payload_len, reg);
}

static void lro_stop(struct toe_dev *toe, int fid)
{
	struct lro_port *lp;
	int port;

	WARN(in_irq(), "%s should not be called from ISR context\n", __func__);
	if (INVALID_FID(fid)) {
		dev_err(toe->dev, "invalid fid %d\n", fid);
		return;
	}
	port = lro_fid_to_port(toe, fid);
	lp = toe->lro[port];
	if (!lp)
		return;

	lro_port_state_table[fid].state_flags |= LRO_PORT_STATE_STOP;
	schedule_delayed_work(&toe->wk_cleanup, msecs_to_jiffies(5000));
}

static void lro_set_timeout(struct toe_dev *toe, u32 microseconds)
{
	writel(TOE_CLK_MHZ * microseconds, toe_reg(toe, TOE_LRO_TIMEOUT(0)));
}

static unsigned long lro_get_cap(struct toe_dev *toe)
{
	unsigned long a0 = BIT(LRO_TYPE_TCP) | BIT(LRO_TYPE_MPTCP) |
			   BIT(LRO_TYPE_TCPV6) | BIT(LRO_TYPE_MPTCPV6);
	unsigned long b0 = BIT(LRO_TYPE_UDP) | BIT(LRO_TYPE_UDPV6) | a0;

	return b0;
}

static inline void lro_set_skb_csum_unnecessary(struct sk_buff *skb)
{
	/* Checksum validation is expected to be done in Hardware
	 * to avoid performance impact in Software.
	 */
	skb->ip_summed = CHECKSUM_UNNECESSARY;
}

static bool verify_ip_length(struct toe_dev *toe, struct sk_buff *skb,
			     const struct dp_dma_desc *desc)
{
	unsigned int l3_offset;
	unsigned int pre_l2;
	struct ipv6hdr *ip6h;
	struct iphdr *iph;
	int tot_len;
	int ip_len;

	l3_offset = *(skb->head + 0x15);
	pre_l2 = FIELD_GET(DW1_PRE_L2, desc->dw[1]);
	iph = (struct iphdr *)(skb->data + 16 * pre_l2 + l3_offset);
	tot_len = skb->len - 16 * pre_l2 - l3_offset;

	if (iph->version == 4) {
		ip_len = ntohs(iph->tot_len);
	} else if (iph->version == 6) {
		ip6h = (struct ipv6hdr *)iph;
		ip_len = sizeof(*ip6h) + ntohs(ip6h->payload_len);
	} else {
		dev_err(toe->dev, "Invalid IP packet\n");
		return false;
	}

	if (unlikely(tot_len > ip_len)) {
		dev_dbg(toe->dev, "IP len mismatch (%d != %d)\n",
			tot_len, ip_len);

		/* Known hardware bug (hardware prematurely stop updating
		 * length). In this case we correct the length by using
		 * accumulated length of segments.
		 */
		if (iph->version == 4)
			iph->tot_len = htons(tot_len);
		else if (iph->version == 6)
			ip6h->payload_len = htons(tot_len - sizeof(*ip6h));

		toe->ip_len_mismatch_cnt++;
		return true;
	}

	if (unlikely(tot_len < ip_len)) {
		/* Unexpected mismatch. In this case we drop the packet. */
		dev_warn(toe->dev, "IP len mismatch (%d != %d)\n",
			 tot_len, ip_len);
		toe->ip_len_mismatch_cnt++;
		return false;
	}

	return true;
}

static void lro_process_single(struct lro_port *lp,
			       const struct dp_dma_desc *desc, int reason)
{
	struct sk_buff *skb = cbm_build_skb_rx(desc);

	if (likely(skb)) {
		lro_set_skb_csum_unnecessary(skb);
		dp_rx(skb, 0);
	}
}

static void recalc_iphdr_csum(struct sk_buff *skb,
			      const struct dp_dma_desc *desc)
{
	unsigned int l3_offset;
	unsigned int pre_l2;
	struct iphdr *iph;

	l3_offset = *(skb->head + 0x15);
	pre_l2 = FIELD_GET(DW1_PRE_L2, desc->dw[1]);
	iph = (struct iphdr *)(skb->data + 16 * pre_l2 + l3_offset);

	if (iph->version == 4) {
		iph->check = 0;
		iph->check = ip_fast_csum((u8 *)iph, iph->ihl);
	}
}

static void add_frag(struct device *dev, struct sk_buff *skb,
		     struct lro_result_ctx *r, int i,
		     unsigned int truesize, unsigned int *mss)
{
	unsigned int addrh;
	struct page *page;
	unsigned int len;
	dma_addr_t pa;
	u64 ioc_pa;
#if IS_ENABLED(CONFIG_CQM_BUF_REF_DBG)
	int cnt;
#endif

	len = FIELD_GET(LRO_OC_SEG_LEN, r->segs[i].dw[0]);
	addrh = FIELD_GET(LRO_OC_SEG_ADDRH, r->segs[i].dw[0]);
	ioc_pa = DP_MAKE_PHYS(addrh, r->segs[i].dw[1]);
	pa = ioc_pa;
#if IS_ENABLED(CONFIG_CQM_BUF_REF_DBG)
	cnt = cqm_page_ref(pa);
	WARN(cnt != 1, "new CQM buffer %pad refcnt:%d\n", &pa, cnt);
#endif
	page = pfn_to_page(pa >> PAGE_SHIFT);
	skb_add_rx_frag(skb, i, page, pa - page_to_phys(page), len, truesize);
	*mss = max(len, *mss);
}

static void free_segment(const u32 dw[2])
{
	cqm_ioc_free(DP_MAKE_PHYS(FIELD_GET(LRO_OC_SEG_ADDRH, dw[0]), dw[1]));
}

static void lro_process_agg(struct lro_port *lp, struct lro_result_ctx *r,
			    int nr_frags)
{
	struct sk_buff *skb = cbm_build_skb_rx(&r->head);
	unsigned int truesize;
	unsigned int mss = 0;
	int i;
	struct ethhdr *eth = NULL;
	struct pppoe_hdr *ph = NULL;
	struct vlan_hdr *vh = NULL;
	unsigned int proto_overhead = ETH_HLEN;

	WARN(nr_frags > MAX_SKB_FRAGS, "wrong nr_frags %d\n", nr_frags);
	if (unlikely(!skb)) {
		for (i = 0; i < nr_frags; i++)
			free_segment(r->segs[i].dw);
		return;
	}

	truesize = skb->truesize;
	WARN(skb_shinfo(skb)->nr_frags || skb_shinfo(skb)->frag_list,
	     "allocated SKB is not initialized\n");
	for (i = 0; i < nr_frags; i++)
		add_frag(lp->toe->dev, skb, r, i, truesize, &mss);
	skb_shinfo(skb)->gso_size = mss;
	skb_shinfo(skb)->gso_type = lp->gso_type;

	if (nr_frags) {
		if (!verify_ip_length(lp->toe, skb, &r->head)) {
			consume_skb(skb);
			return;
		}
	}

	recalc_iphdr_csum(skb, &r->head);
	lro_set_skb_csum_unnecessary(skb);
	dev_dbg(lp->toe->dev, "skb:%p len:%u mss:%u nr_sg:%d\n",
		skb->data, skb->len, mss, skb_shinfo(skb)->nr_frags);
	eth = (struct ethhdr *)(skb->data);
	if (eth->h_proto == htons(ETH_P_PPP_SES)) {
		ph = (struct pppoe_hdr *)(skb->data + ETH_HLEN);
		proto_overhead += sizeof(struct pppoe_hdr);
	} else if (eth->h_proto == htons(ETH_P_8021Q)) {
		vh = (struct vlan_hdr *)(skb->data + ETH_HLEN);
		if (vh && vh->h_vlan_encapsulated_proto
				== htons(ETH_P_PPP_SES)) {
			ph = (struct pppoe_hdr *)(skb->data + ETH_HLEN +
					VLAN_HLEN);
			proto_overhead += sizeof(struct vlan_hdr);
			proto_overhead += sizeof(struct pppoe_hdr);
		}
	}
	if (ph)
		ph->length = cpu_to_be16(skb->len - proto_overhead);

	dp_rx(skb, 0);

}

static void lro_process_stall(struct toe_dev *toe, struct lro_port *lp)
{
	struct lro_sc_store *ls;

	if (unlikely(!lp->stall.last)) {
		while (true) {
			lp->stall.last = READ_ONCE(lp->stall.head);
			if (likely(lp->stall.last))
				break;
			wait_for_completion(&lp->wait_stall);
		}

		/* accessed in lro_stall workqueue */
		mutex_lock(&lp->stall_mtx);
		lp->stall.head = NULL;
		lp->stall.tail = &lp->stall.head;
		mutex_unlock(&lp->stall_mtx);
	}

	ls = lp->stall.last;
	lro_process_single(lp, &ls->stall, LRO_RSLT_STALL);
	lp->stall.last = ls->next;
	kmem_cache_free(toe->cache[SC_CACHE], ls);
}

static void lro_delayed_process(struct lro_port *lp,
				struct lro_store_common *s)
{
	struct toe_dev *toe = lp->toe;

	if (likely(s->result == LRO_RSLT_AGG)) {
		struct lro_oc_store *ls =
			container_of(s, struct lro_oc_store, cmn);

		lro_process_agg(lp, &ls->oc, ls->nr_frags);
		kmem_cache_free(toe->cache[OC_CACHE], s);
	} else if (s->result == LRO_RSLT_STALL) {
		struct lro_sc_cnt *ls =
			container_of(s, struct lro_sc_cnt, cmn);
		int nr_segs = ls->nr_segs;

		dev_dbg(toe->dev, "port %d stall:%d\n", ls->port, ls->nr_segs);
		do {
			lro_process_stall(toe, toe->lro[ls->port]);
		} while (--nr_segs);
		kmem_cache_free(toe->cache[SC_CNT_CACHE], s);
	} else {
		struct dp_dma_desc *desc =
			&container_of(s, struct lro_ec_store, cmn)->ec;

		lro_process_single(lp, desc, s->result);
		kmem_cache_free(toe->cache[EC_CACHE], s);
	}
}

static void lro_port_work(struct work_struct *work)
{
	struct lro_port *lp = container_of(work, struct lro_port, wk);
	struct lro_store_common *head;

	head = READ_ONCE(lp->store.head);
	if (!head)
		return;

	WRITE_ONCE(lp->store.head, NULL);

	/* accessed by lro_isr() -> lro_port_process() -> append_result() */
	spin_lock_irq(&lp->store_lock);
	lp->store.tail = &lp->store.head;
	spin_unlock_irq(&lp->store_lock);
	dev_dbg(lp->toe->dev, "port %d started, head: %p\n",
		lp->id, head);

	while (head) {
		struct lro_store_common *s = head;

		head = head->next;
		lro_delayed_process(lp, s);
	}
}

static void lro_flush_stall(struct lro_port *lp, struct lro_sc_store *head)
{
	struct lro_sc_store *ls;

	ls = head;
	while (ls) {
		lro_process_single(lp, &ls->stall, LRO_RSLT_STALL);
		kmem_cache_free(lp->toe->cache[SC_CACHE], ls);
		dev_dbg(lp->toe->dev, "cleanup stall port %d\n", lp->id);

		ls = ls->next;
	}
}

static int lro_real_stop_flow(struct toe_dev *toe, int fid)
{
	unsigned int retries = 1000;
	struct lro_port *lp;
	void __iomem *reg;
	int port;

	if (INVALID_FID(fid)) {
		dev_err(toe->dev, "invalid fid %d\n", fid);
		return -EINVAL;
	}

	port = lro_fid_to_port(toe, fid);
	lp = toe->lro[port];
	if (!lp)
		return -EEXIST;

	reg = toe_reg(toe, TOE_LRO_FID(port));
	if (!(lro_port_state_table[fid].state_flags & LRO_PORT_STATE_RETRY))
		writel(LRO_FID0_SEND, reg);

	do {
		if (readl(reg) & LRO_FID0_CPU_OWN)
			break;
		cpu_relax();
	} while (--retries);

	if (!retries) {
		dev_dbg(toe->dev, "retries 0 for port %d\n", fid);
		lro_port_state_table[fid].state_flags |= LRO_PORT_STATE_RETRY;
		return -EBUSY;
	}

	toe_lro_en(toe, port, false);

	flush_work(&lp->wk);
	lro_flush_stall(lp, lp->stall.last);
	lro_flush_stall(lp, lp->stall.head);

	del_fid(toe, fid);
	devm_kfree(toe->dev, lp);
	toe->lro[port] = NULL;
	lro_port_state_table[fid].state_flags = 0;
	pm_runtime_mark_last_busy(toe->dev);
	pm_runtime_put_autosuspend(toe->dev);

	dev_dbg(toe->dev, "LRO entry deleted for port = %d retries = %d\n",
		fid, retries);

	return 0;
}

static void lro_port_work_cleanup(struct work_struct *work)
{
	struct toe_dev *toe = container_of(work, struct toe_dev,
					   wk_cleanup.work);
	int i;

	for (i = 0; i < LRO_PORT_CNT; i++) {
		if (lro_port_state_table[i].state_flags & LRO_PORT_STATE_STOP)
			if (lro_real_stop_flow(toe, i) == -EBUSY)
				break;
	}

	if (i < LRO_PORT_CNT)
		schedule_delayed_work(&toe->wk_cleanup, msecs_to_jiffies(5000));
}

static struct lro_result_ctx *lro_oc(struct toe_dev *toe, int port, int depth)
{
	struct lro_dma_descs *lro;

	switch (toe->oc_mode) {
	case EXT_OC_EXT:
		lro = (struct lro_dma_descs *)toe->lro_ssb;
		return &lro->oc[port][depth];
	case EXT_OC_SPLT:
		if (port < toe->split_boundary) {
			lro = (struct lro_dma_descs *)toe->base[TOE_IOMEM_LRO_DMA];
			return &lro->oc[port][depth];
		} else {
			lro = (struct lro_dma_descs *)toe->lro_ssb;
			return &lro->oc[port][depth];
		}
	case EXT_OC_INT:
	case EXT_OC_LEG:
		lro = (struct lro_dma_descs *)toe->base[TOE_IOMEM_LRO_DMA];
		return &lro->oc[port][depth];
	default:
		return NULL;
	}
}

static struct dp_dma_desc *lro_exception(struct toe_dev *toe, int port,
					 int depth)
{
	struct lro_dma_descs *lro;

	switch (toe->oc_mode) {
	case EXT_OC_SPLT:
	case EXT_OC_EXT:
		lro = (struct lro_dma_descs *)toe->lro_ssb;
		return &lro->exception[port][depth];
	case EXT_OC_INT:
	case EXT_OC_LEG:
		lro = (struct lro_dma_descs *)toe->base[TOE_IOMEM_LRO_DMA];
		return &lro->exception[port][depth];
	default:
		return NULL;
	}
}

static struct dp_dma_desc *lro_nomatch(struct toe_dev *toe, int depth)
{
	struct lro_dma_descs *lro;

	switch (toe->oc_mode) {
	case EXT_OC_SPLT:
	case EXT_OC_EXT:
		lro = (struct lro_dma_descs *)toe->lro_ssb;
		return &lro->nomatch[depth];
	case EXT_OC_INT:
	case EXT_OC_LEG:
		lro = (struct lro_dma_descs *)toe->base[TOE_IOMEM_LRO_DMA];
		return &lro->nomatch[depth];
	default:
		return NULL;
	}
}

static struct dp_dma_desc *lro_stall_desc(struct toe_dev *toe, int depth)
{
	struct lro_dma_descs *lro;

	switch (toe->oc_mode) {
	case EXT_OC_SPLT:
	case EXT_OC_EXT:
		lro = (struct lro_dma_descs *)toe->lro_ssb;
		return &lro->stall_desc[depth];
	case EXT_OC_INT:
	case EXT_OC_LEG:
		lro = (struct lro_dma_descs *)toe->base[TOE_IOMEM_LRO_DMA];
		return &lro->stall_desc[depth];
	default:
		return NULL;
	}
}

static u32 *lro_stall_flag(struct toe_dev *toe, int port)
{
	struct lro_dma_descs *lro;

	switch (toe->oc_mode) {
	case EXT_OC_SPLT:
	case EXT_OC_EXT:
		lro = (struct lro_dma_descs *)toe->lro_ssb;
		return &lro->stall_flag[port];
	case EXT_OC_INT:
	case EXT_OC_LEG:
		lro = (struct lro_dma_descs *)toe->base[TOE_IOMEM_LRO_DMA];
		return &lro->stall_flag[port];
	default:
		return NULL;
	}
}

static void append_result(struct lro_store_common *ls, struct lro_port *lp,
			  int result)
{
	ls->result = result;
	ls->next = NULL;
	spin_lock(&lp->store_lock);
	*lp->store.tail = ls;
	lp->store.tail = &ls->next;
	spin_unlock(&lp->store_lock);
}

static void add_lro_oc_store(struct kmem_cache *cache, struct lro_port *lp,
			     int nr_frags, const struct lro_result_ctx *rslt)
{
	size_t len = sizeof(rslt->head) + sizeof(rslt->segs[0]) * nr_frags;
	struct lro_oc_store *ls = kmem_cache_alloc(cache, GFP_ATOMIC);
	bool invalidate = false;

	if (unlikely(!ls)) {
		dev_warn(lp->toe->dev, "no memory to store OC flags\n");
		return;
	}

	if (lp->toe->lro_ssb &&
	    lp->id >= lp->toe->split_boundary)
		invalidate = true;

	lro_memcpy_fromio(&ls->oc, rslt, len, invalidate);
	ls->nr_frags = nr_frags;
	append_result(&ls->cmn, lp, LRO_RSLT_AGG);
}

static void add_lro_ec_store(struct kmem_cache *cache, struct lro_port *lp,
			     int reason, const struct dp_dma_desc *rslt)
{
	struct lro_ec_store *ls = kmem_cache_alloc(cache, GFP_ATOMIC);
	bool invalidate = false;

	if (unlikely(!ls)) {
		dev_warn(lp->toe->dev, "no memory to store EC flags\n");
		return;
	}

	if (lp->toe->lro_ssb)
		invalidate = true;

	lro_memcpy_fromio(&ls->ec, rslt, sizeof(*rslt), invalidate);
	append_result(&ls->cmn, lp, reason);
}

static void add_lro_sc_cnt(struct kmem_cache *cache, struct lro_port *lp,
			   int nr_segs)
{
	struct lro_sc_cnt *ls = kmem_cache_alloc(cache, GFP_ATOMIC);

	if (unlikely(!ls)) {
		dev_warn(lp->toe->dev, "no memory to store SC flags\n");
		return;
	}
	ls->nr_segs = nr_segs;
	ls->port = lp->id;
	append_result(&ls->cmn, lp, LRO_RSLT_STALL);
}

static bool lro_port_ec_check(struct toe_dev *toe, struct lro_port *lp,
			      int port, int depth, int reason)
{
	const int PORTS_PER_REG = BITS_PER_BYTE / LRO_EXP_DEPTH;
	const struct dp_dma_desc *desc;
	int idx = port / PORTS_PER_REG;
	unsigned int exp;
	u8 __iomem *reg;
	u8 bm;

	reg = toe_reg(toe, TOE_LRO_L2_EXP_BASE + idx);
	bm = readb(reg);

	exp = BIT((port % PORTS_PER_REG) * LRO_EXP_DEPTH + depth);
	if (unlikely(!(bm & exp))) {
		dev_err(toe->dev, "lro exp port %d-%d reason:%d\n",
			port, depth, reason);
		return false;
	}

	dev_dbg(toe->dev, "lro exp port %d-%d\n", port, depth);
	desc = lro_exception(toe, port, depth);
	add_lro_ec_store(toe->cache[EC_CACHE], lp, reason, desc);
	writeb(exp, reg);
	return true;
}

static bool lro_port_process(struct toe_dev *toe, struct lro_port *lp, int port)
{
	int oc_depth = lp->rx_cnt % LRO_OC_DEPTH;
	void __iomem *reg;
	unsigned long oc;
	int nr_segs;
	int reason;

	reg = toe_ocflag(toe, TOE_LRO_OC_FLAG(port, oc_depth, OC_FLAG));
	oc = readl(reg);
	if (unlikely(!(oc & LRO_OC_OWN_BY_CPU)))
		return false;

	reason = FIELD_GET(LRO_OC_EXP, oc);
	nr_segs = FIELD_GET(LRO_OC_SEG_NR, oc);
	dev_dbg(toe->dev, "port:%d-%d nr_segs:%d reason:%d\n",
		port, oc_depth, nr_segs, reason);
	if (nr_segs) {
		add_lro_oc_store(toe->cache[OC_CACHE], lp, nr_segs - 1,
				 lro_oc(toe, port, oc_depth));
		lp->oc_cnt += nr_segs;
	}

	switch (reason) {
	case LRO_EXP_OK:
	case LRO_EXP_TIMEOUT:
	case LRO_EXP_DSN:
		break;
	default:
		/* OC depth == EC depth, so passing in OC depth directly */
		if (likely(lro_port_ec_check(toe, lp, port, oc_depth, reason)))
			lp->ec_cnt++;
		break;
	}

	/* External OC mode requires rechecking OC flag (after delay),
	 * due to hardware issue of late stall bit update.
	 */
	if (toe->lro_ssb) {
		udelay(2);
		oc = readl(reg);
	}

	if (toe->rev >= TOE_REV_B && (oc & LRO_OC_STALL)) {
		unsigned int offset;
		unsigned long oc5;
		int cnt;

		offset = TOE_LRO_OC_FLAG(port, oc_depth, OC_MPTCP_WIN);
		oc5 = readl(toe_ocflag(toe, offset));
		cnt = FIELD_GET(LRO_OC5_STALL_CNT, oc5);
		if (cnt)
			add_lro_sc_cnt(toe->cache[SC_CNT_CACHE], lp, cnt);
		lp->sc_acc_cnt += cnt;
	}
	writel(LRO_OC_OWN_BY_CPU, reg);
	lp->rx_cnt++;
	return true;
}

static irqreturn_t lro_isr(int irq, void *dev_id)
{
	struct lro_group *group = (struct lro_group *)dev_id;

	while (true) {
		unsigned long sts = readb(group->sts);
		int i;

		if (unlikely(!sts)) {
			pr_debug("dummy LRO%d interrupt\n", group->offset);
			break;
		}

		writeb(sts, group->sts);
		for_each_set_bit(i, &sts, LRO_PORT_PER_GROUP) {
			int port = i + group->offset;
			struct toe_dev *toe = group->toe;
			struct lro_port *lp = toe->lro[port];

			if (!lp)
				continue;

			while (lro_port_process(group->toe, lp, port))
				continue;
			schedule_work(&lp->wk);
		}
	}
	return IRQ_HANDLED;
}

static struct lro_port *get_stall_port(struct toe_dev *toe, int idx)
{
	int fid = FIELD_GET(LRO_SC_FID, toe->stall_flag[idx]);

	return toe->lro[lro_fid_to_port(toe, fid)];
}

static void lro_add_per_port_stall(struct toe_dev *toe, int idx)
{
	const struct dp_dma_desc *desc = &toe->stall_desc[idx];
	struct lro_sc_store *ls;
	struct lro_port *lp;
	bool cmpl;

	if (unlikely(toe->stall_flag[idx] & LRO_SC_OV)) {
		struct lro_port *lp = get_stall_port(toe, idx);

		lro_process_single(lp, desc, LRO_RSLT_SC_OV);
		lp->ov_cnt++;
		return;
	}
	ls = kmem_cache_alloc(toe->cache[SC_CACHE], GFP_KERNEL);
	if (!ls) {
		dev_warn(toe->dev, "out of memory to store stall context\n");
		return;
	}
	memcpy(&ls->stall, desc, sizeof(*desc));
	ls->next = NULL;

	lp = get_stall_port(toe, idx);
	mutex_lock(&lp->stall_mtx);
	cmpl = !READ_ONCE(lp->stall.head);
	*lp->stall.tail = ls;
	lp->stall.tail = &ls->next;
	mutex_unlock(&lp->stall_mtx);
	if (cmpl)
		complete(&lp->wait_stall);

	lp->sc_cnt++;
}

static void lro_send_per_port_stall(struct toe_dev *toe, int idx)
{
	struct lro_port *lp = get_stall_port(toe, idx);
	const struct dp_dma_desc *desc;

	desc = &toe->stall_desc[idx];

	if (!lp)
		return;

	if ((toe->stall_flag[idx] & LRO_SC_OV)) {
		lro_process_single(lp, desc, LRO_RSLT_SC_OV);
		lp->ov_cnt++;
	} else {
		lro_process_single(lp, desc, LRO_RSLT_STALL);
		lp->sc_cnt++;
	}
}

static void sc_io_cpy(struct toe_dev *toe, int dst, int src, int cnt)
{
	bool invalidate = false;

	if (toe->lro_ssb)
		invalidate = true;

	lro_memcpy_fromio(&toe->stall_flag[dst], lro_stall_flag(toe, src),
			  sizeof(toe->stall_flag[0]) * cnt, invalidate);
	lro_memcpy_fromio(&toe->stall_desc[dst], lro_stall_desc(toe, src),
			  sizeof(toe->stall_desc[0]) * cnt, invalidate);
}

static void lro_stall_mask_set(struct toe_dev *toe, bool en)
{
	__assign_bit(TIE_LRO_STALL_BIT, &toe->tie, en);
	writel(toe->tie, toe_reg(toe, TOE_SPARE_TIE0));
}

static int lro_sc_copy(struct toe_dev *toe, unsigned int head,
		       unsigned int tail)
{
	int cnt;

	if (tail < head) {
		int first_part = LRO_STALL_CNT - head;
		int second_part = tail;

		sc_io_cpy(toe, 0, head, first_part);
		sc_io_cpy(toe, first_part, 0, second_part);
		cnt = first_part + second_part;
	} else {
		cnt = tail - head;
		sc_io_cpy(toe, 0, head, cnt);
	}
	return cnt;
}

static void lro_stall_cmpl(struct toe_dev *toe, unsigned int tail)
{
	toe->stall_idx = tail;
	lro_stall_mask_set(toe, false);
}

static void lro_sc_work(struct work_struct *work)
{
	struct toe_dev *toe = container_of(work, struct toe_dev, lro_sc_wk);
	int offset = TOE_LRO_OC_FLAG(0, 0, OC_MPTCP_WIN);
	void __iomem *reg = toe_ocflag(toe, offset);
	unsigned int head = toe->stall_idx;
	unsigned int tail;

	while (true) {
		int cnt;
		int i;

		tail = FIELD_GET(LRO_OC5_LRO_TAIL, readl(reg));

		cnt = lro_sc_copy(toe, head, tail);
		if (!cnt)
			break;

		for (i = 0; i < cnt; i++) {
			if (toe->rev >= TOE_REV_B)
				lro_add_per_port_stall(toe, i);
			else
				lro_send_per_port_stall(toe, i);
		}
		writeb(tail, toe_reg(toe, TOE_LRO_HEAD));
		head = tail;
	}
	lro_stall_cmpl(toe, tail);
}

static irqreturn_t lro_stall_isr(int irq, void *dev_id)
{
	struct toe_dev *toe = (struct toe_dev *)dev_id;

	lro_stall_mask_set(toe, true);
	schedule_work(&toe->lro_sc_wk);
	return IRQ_HANDLED;
}

static irqreturn_t lro_err_isr(int irq, void *dev_id)
{
	WARN(true, "%s should never be triggered\n", __func__);
	return IRQ_NONE;
}

static void lro_exp_work(struct work_struct *work)
{
	struct toe_dev *toe;
	void __iomem *reg;
	unsigned long bm;

	toe = container_of(work, struct toe_dev, lro_exp_wk.work);
	reg = toe_reg(toe, TOE_LRO_NOMATCH);
	WARN(LRO_NOMATCH_DEPTH > SZ_16,
	     "please use readl() to read EC register\n");
	bm = readw(reg);
	if (bm) {
		const struct dp_dma_desc *desc;
		bool invalidate = false;
		int i;

		if (toe->lro_ssb)
			invalidate = true;

		/* nomatch will occur if CQM pool size if not power of 2 */
		dev_err(toe->dev, "nomatch packet(s) found, please check\n");
		for_each_set_bit(i, &bm, LRO_NOMATCH_DEPTH) {
			struct dp_dma_desc d;

			desc = lro_nomatch(toe, i);
			lro_memcpy_fromio(&d, desc, sizeof(*desc), invalidate);
			lro_process_single(toe->lro[0], &d, LRO_RSLT_NOMATCH);
			toe->nomatch_cnt++;
		}
		writew(bm, reg);
	}

	writeb(0, toe_reg(toe, TOE_INT_MASK_LRO_EXP));
}

static irqreturn_t lro_exp_isr(int irq, void *dev_id)
{
	struct toe_dev *toe = (struct toe_dev *)dev_id;
	void __iomem *reg = toe_reg(toe, TOE_INT_STS_LRO_EXP);

	if (unlikely(!(readb(reg) & TOE_INT_LRO_EXP)))
		return IRQ_HANDLED;

	writeb(TOE_INT_LRO_EXP, TOE_INT_STS2MASK(reg));
	writeb(TOE_INT_LRO_EXP, reg);
	schedule_delayed_work(&toe->lro_exp_wk, msecs_to_jiffies(5000));
	return IRQ_HANDLED;
}

static int lro_init(struct toe_dev *toe, u64 pool_cfg)
{
	void __iomem *lro_exp_en_reg = toe_reg(toe, TOE_INT_EN_LRO_EXP);
	static const struct {
		const char *name;
		size_t size;
	} lro_cache[LRO_CACHE_CNT] = {
		{"lro_oc", sizeof(struct lro_oc_store)},
		{"lro_ec", sizeof(struct lro_ec_store)},
		{"lro_sc", sizeof(struct lro_sc_store)},
		{"lro_sc_cnt", sizeof(struct lro_sc_cnt)},
	};
	u32 xoc;
	int i;

	for (i = 0; i < LRO_CACHE_CNT; i++) {
		toe->cache[i] = kmem_cache_create(lro_cache[i].name,
						  lro_cache[i].size,
						  0, SLAB_PANIC, NULL);
		if (!toe->cache[i])
			return -ENOMEM;
	}

	/* Disable all cross checks */
	xoc = LRO_CTL_SEQ_XOC | LRO_CTL_DSS_XOC | LRO_CTL_TS_XOC |
	      LRO_CTL_FO_XOC;
	toe_update_bits(toe_reg(toe, TOE_LRO_CTRL), xoc, xoc);

	lro_set_timeout(toe, LRO_DEFAULT_TIMEOUT);

	writeq(pool_cfg, toe_reg(toe, TOE_BUFF_OFFSET));

	/* enable LRO exp interrupt */
	writeb(readb(lro_exp_en_reg) | TOE_INT_LRO_EXP, lro_exp_en_reg);

	/* set default LRO threshould to 800 */
	lro_set_threshold(toe, 800);
	return 0;
}

static int lro_fid_to_port(struct toe_dev *toe, int fid)
{
	return fid;
}

static inline bool get_lro_bit(u8 __iomem *bm, int id)
{
	return readb(bm + id / 8) & BIT(id % 8);
}

static void lro_dump_regs(struct seq_file *seq, struct toe_dev *toe, int port)
{
	int depth;
	int i;

	seq_printf(seq, "port%d en:%d ea:%d intr mask:%d en:%d sts:%d\n", port,
		   get_lro_bit(toe_reg(toe, TOE_LRO_EN), port),
		   get_lro_bit(toe_reg(toe, TOE_LRO_EAK_EN), port),
		   get_lro_bit(toe_reg(toe, TOE_LRO_INT_MASK(0)), port),
		   get_lro_bit(toe_reg(toe, TOE_LRO_INT_EN(0)), port),
		   get_lro_bit(toe_reg(toe, TOE_LRO_INT_STS(0)), port));

	seq_puts(seq, "fid:");
	for (i = 0; i < 8; i++) {
		seq_printf(seq, "%x ",
			   readl(toe_reg(toe, TOE_LRO_FID(port) + i * 4)));
	}
	seq_putc(seq, '\n');

	for (depth = 0; depth < LRO_OC_DEPTH; depth++) {
		seq_printf(seq, "OC%d:", depth);
		for (i = 0; i < OC_FLAG_CNT; i++) {
			int oc_offset = TOE_LRO_OC_FLAG(port, depth, i);
			void __iomem *oc1 = toe_ocflag(toe, oc_offset);

			seq_printf(seq, "%x ", readl(oc1));
		}
		seq_putc(seq, '\n');
	}
}

static void *dbgfs_lro_start(struct seq_file *s, loff_t *pos)
{
	if (*pos >= LRO_PORT_CNT)
		return NULL;

	return pos;
}

static void *dbgfs_lro_next(struct seq_file *s, void *v, loff_t *pos)
{
	return (++*pos >= LRO_PORT_CNT) ? NULL : pos;
}

static void dbgfs_lro_stop(struct seq_file *s, void *v)
{
}

static int dbgfs_lro_show(struct seq_file *seq, void *v)
{
	struct toe_dev *toe = (struct toe_dev *)seq->private;
	int port = *(loff_t *)v;
	struct lro_port *lp;

	lp = toe->lro[port];
	if (!lp)
		return SEQ_SKIP;

	lro_dump_regs(seq, toe, port);
	seq_printf(seq, "rx:%u oc:%u ec:%u stall:%u/%u ov:%u\n\n",
		   lp->rx_cnt, lp->oc_cnt, lp->ec_cnt, lp->sc_cnt,
		   lp->sc_acc_cnt, lp->ov_cnt);
	return 0;
}

static const struct seq_operations dbgfs_lro_ops = {
	.start = dbgfs_lro_start,
	.next = dbgfs_lro_next,
	.stop = dbgfs_lro_stop,
	.show = dbgfs_lro_show,
};

static int dbgfs_lro_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &dbgfs_lro_ops);

	if (!ret) {
		struct seq_file *s = file->private_data;

		s->private = inode->i_private;
	}

	return ret;
}

static const struct file_operations dbgfs_lro_fops = {
	.owner = THIS_MODULE,
	.open = dbgfs_lro_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

static int dbgfs_lro_cfg_show(struct seq_file *seq, void *v)
{
	struct toe_dev *toe = (struct toe_dev *)seq->private;

	seq_printf(seq, "lro_ctrl: 0x%x\n", readl(toe_reg(toe, TOE_LRO_CTRL)));
	seq_printf(seq, "lro_dbg_info:%x\n", readl(toe_reg(toe, TOE_LRO_DBG)));
	if (toe->rev >= TOE_REV_C) {
		seq_printf(seq, "oc_da: 0x%x\n",
			   readl(toe_reg(toe, TOE_EXT_OC_DA)));
		seq_printf(seq, "split_oc_da: 0x%x\n",
			   readl(toe_reg(toe, TOE_SPLIT_OC_DA)));
	}

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dbgfs_lro_cfg);

static void tso_dump_stats(struct seq_file *seq, struct toe_dev *toe)
{
	struct tso_port *tp;
	int i;

	seq_printf(seq,
		   "+------+------------+------------+---------------+---------------+--------+----------+----------+--------+------------+------------+\n");
	seq_printf(seq,
		   "| port | skb queued | completed  |    cmd enq    |    cmd deq    |  err   | cmdbusy  | portbusy | no gb  |  sram gb   |   ddr gb   |\n");
	seq_printf(seq,
		   "+------+------------+------------+---------------+---------------+--------+----------+----------+--------+------------+------------+\n");
	for (i = 0; i < toe->tso_active_ports;  i++) {
		tp = toe->tso[i];
		seq_printf(seq,
			   "| %-4d | %-10d | %-10d | %-13lld | %-13lld | %-6d | %-8d | %-8d | %-6d | %-10d | %-10d |\n",
			   i, tp->queue_cnt, tp->cmpl_cnt, tp->cmd_enq,
			   tp->cmd_deq, tp->err_cnt, tp->cmd_busy,
			   tp->port_busy, tp->no_gb, tp->sram_gb,
			   tp->ddr_gb);
	}
	seq_printf(seq,
		   "+------+------------+------------+---------------+---------------+--------+----------+----------+--------+------------+------------+\n");
	seq_printf(seq,
		   "+------+-----------+------------+--------------+---------------+\n");
	seq_printf(seq,
		   "| port |  cmdbusy  | wake_queue | wake_pending | hi_frags      |\n");
	seq_printf(seq,
		   "+------+-----------+------------+--------------+---------------+\n");
	for (i = 0; i < toe->tso_active_ports;  i++) {
		tp = toe->tso[i];
		seq_printf(seq,
			   "| %-4d | %-9d | %-10d | %-12d | %-13lld |\n",
			   i, tp->cmd_busy, tp->wake_queue, tp->wake_pending,
			   tp->hi_frags);
	}
	seq_printf(seq,
		   "+------+-----------+------------+--------------+---------------+\n");
}

static void tso_dump_regs(struct seq_file *seq, struct toe_dev *toe, int port)
{
	struct tso_port *tp = toe->tso[port];
	u32 val;
	int i;

	seq_printf(seq, "P%d\n", port);
	seq_puts(seq, "CMD0-5:");
	for (i = 0;  i < 6; i++) {
		val = readl(&tp->cmd[i]);
		seq_printf(seq, "%*x ", 8, val);
	}
	seq_printf(seq, "RES 0:%x\n", readl(&tp->res->addr));

	seq_printf(seq, "HDR BASE:%x %x\n\n",
		   readl(toe_reg(toe, TOE_TSO_HDR_BASE(port, 0))),
		   readl(toe_reg(toe, TOE_TSO_HDR_BASE(port, 1))));
}

static void print_fsm_state(struct seq_file *seq, u32 dbg[5],
			    int port, int slice, int fsm)
{
	const u32 TSO_FSM[TSO_FSM_CNT][TSO_PORT_CNT][TSO_MAX_SLICE] = { {
		{GENMASK(4,   0), GENMASK(9,   5)},
		{GENMASK(14, 10), GENMASK(19, 15)},
		{GENMASK(24, 20), GENMASK(4,   0)},
		{GENMASK(9,   5), GENMASK(14, 10)},
	}, {
		{GENMASK(19, 15), GENMASK(24, 20)},
		{GENMASK(29, 25), GENMASK(4,   0)},
		{GENMASK(9,   5), GENMASK(14, 10)},
		{GENMASK(19, 15), GENMASK(24, 20)},
	}, {
		{GENMASK(30, 25), GENMASK(5,   0)},
		{GENMASK(11,  6), GENMASK(17, 12)},
		{GENMASK(23, 18), GENMASK(29, 24)},
		{GENMASK(5,   0), GENMASK(11,  6)},
	} };
	const u8 REG_OFF[TSO_FSM_CNT][TSO_PORT_CNT][2] = {
		{ {0, 0}, {0, 0}, {0, 1}, {1, 1} },
		{ {1, 1}, {1, 2}, {2, 2}, {2, 2} },
		{ {2, 3}, {3, 3}, {3, 3}, {4, 4} },
	};
	const char *const FSM_NAME[TSO_FSM_CNT] = {
		"GA", "S", "ENQ",
	};
	const char *const GE_FSM[] = {
		"RD_CMD", "CHK_CMD", "GEN_FIRST", "GEN_DES", "GEN_LAST",
		"CHK_OWN", "CHK_CSN", "WR_DES", "WAIT4DES_UPD", "WAIT4DMA",
		"WR_OC", "WAIT4CMD", "WAIT4SEG", "SRST_WAIT",
	};
	const char *const S_FSM[] = {
		"WR_GE_DATA_FOR_ENQ0", "WR_GE_DATA_FOR_ENQ1", "RD_HDR",
		"CHK_IP_HDR", "STORE_TCP_UDP_HDR", "UDP_CHECK_IPV6",
		"MPTCP_CHECK", "MPTCP_CHECK_OPT", "UPD_HDR_INFO", "WR_HDR",
		"HDR_SRAM", "WAIT_WR_HDR_END", "WR_ENQ_DES",
	};
	const char *const EN_LO_FSM[] = {
		"RD_SEG_DMAWORD", "CHK_OWN", "RD_CBM_DES", "CHK_CS",
		"RD_ENQ_DES_FIFO", "WR_DES", "WAIT_END_ENQ", "SOFT_RESET",
	};
	const char *const EN_HI_FSM[] = {
		" UPD_CBM_DES",
	};

	u32 mask = TSO_FSM[fsm][port][slice];
	u8 off = REG_OFF[fsm][port][slice];
	u8 hi, lo;
	u8 val;

	val = (dbg[off] & mask) >> __ffs64(mask);
	if (!val)
		return;

	seq_printf(seq, "P%dS%d %s %d ", port, slice, FSM_NAME[fsm], val);
	switch (fsm) {
	case 0:
		seq_puts(seq, GE_FSM[val - 1]);
		break;
	case 1:
		seq_puts(seq, S_FSM[val - 1]);
		break;
	default:
		lo = FIELD_GET(TSO_FSM_ENQ_LO_MASK, val);
		hi = FIELD_GET(TSO_FSM_ENQ_HI_MASK, val);
		if (lo)
			seq_puts(seq, EN_LO_FSM[lo - 1]);
		if (hi)
			seq_puts(seq, EN_HI_FSM[hi - 1]);
		break;
	};
	seq_putc(seq, '\n');
}

static int dbgfs_tso_show(struct seq_file *seq, void *v)
{
	struct toe_dev *toe = (struct toe_dev *)seq->private;
	int port, slice, fsm, i;
	u32 dbg[5];

	pm_runtime_get_sync(toe->dev);

	tso_dump_stats(seq, toe);

	for (port = 0; port < toe->tso_active_ports; port++)
		tso_dump_regs(seq, toe, port);

	seq_printf(seq,
		   "+------------------ Gather Buffers Info ------------------+\n\n");
	seq_printf(seq, "SRAM:%pad\t    Size:%zu\n",
		   &toe->sram_pa, toe->sram_sz);

	for (i = 0; i < toe->gb_num; i++)
		seq_printf(seq, "gather buf[%d]: %-12llx Type: %-4s \t Status: %s\n",
			   i, toe->gb[i].gather_buf,
			   toe->gb[i].type == TSO_GB_SRAM ? "SRAM" : "DDR",
			   toe->gb[i].in_used ? "In Use" : "Free");

	seq_printf(seq,
		   "+---------------------------------------------------------+\n\n");

	seq_printf(seq, "INT mask:%x en:%x sts:%x\n",
		   readl(toe_reg(toe, TOE_INT_MASK)),
		   readl(toe_reg(toe, TOE_INT_EN)),
		   readl(toe_reg(toe, TOE_INT_STS)));

	seq_printf(seq, "internal INT mask:%x en:%x sts:%x\n",
		   readl(toe_reg(toe, TOE_TSO_INTL_INT_STS)),
		   readl(toe_reg(toe, TOE_TSO_INTL_INT_EN)),
		   readl(toe_reg(toe, TOE_TSO_INTL_INT_MASK)));

	seq_printf(seq, "disabled:%d option3:%d\n",
		   toe->tso_disable, toe->tso_option3);

	for (i = 0;  i < 5; i++)
		dbg[i] = readl(toe_reg(toe, TOE_TSO_DBG(i)));
	for (port = 0; port < toe->tso_active_ports; port++) {
		for (slice = 0; slice < TSO_MAX_SLICE; slice++) {
			for (fsm = 0; fsm < TSO_FSM_CNT; fsm++)
				print_fsm_state(seq, dbg, port, slice, fsm);
		}
	}

	pm_runtime_mark_last_busy(toe->dev);
	pm_runtime_put_autosuspend(toe->dev);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dbgfs_tso);

static int dbgfs_tso_low_wm_show(struct seq_file *seq, void *v)
{
	struct toe_dev *toe = (struct toe_dev *)seq->private;

	seq_printf(seq, "TSO Low Watermark %d\n", toe->tso_low_wm);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dbgfs_tso_low_wm);

static int tso_clear_stats(void *data, u64 val)
{
	struct toe_dev *toe = (struct toe_dev *)data;
	struct tso_port *tp;
	int i;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (val != 0)
		return -EINVAL;

	/* make sure ports are lock and cmds are completed */
	for (i = 0; i < toe->tso_active_ports;  i++) {
		tp = toe->tso[i];
		while (!lock_tso_port(tp))
			cpu_relax();
		while (tp->queue_cnt != tp->cmpl_cnt)
			cpu_relax();
		memset(tp->tso_stats, 0, sizeof(tp->tso_stats));
		unlock_tso_port(tp);
	}

	return 0;
}

static int dbgfs_nomatch_show(struct seq_file *seq, void *v)
{
	struct toe_dev *toe = (struct toe_dev *)seq->private;
	unsigned long bm;
	int i;

	bm = readw(toe_reg(toe, TOE_LRO_NOMATCH));
	seq_printf(seq, "nomatch count:%d\n", toe->nomatch_cnt);
	for_each_set_bit(i, &bm, LRO_NOMATCH_DEPTH) {
		const struct dp_dma_desc *desc = lro_nomatch(toe, i);

		seq_printf(seq, "nomatch %d:%x %x %x %x\n", i,
			   readl(&desc->dw[0]),
			   readl(&desc->dw[1]),
			   readl(&desc->dw[2]),
			   readl(&desc->dw[3]));
	}
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dbgfs_nomatch);

static int dbgfs_stall_show(struct seq_file *seq, void *v)
{
	struct toe_dev *toe = (struct toe_dev *)seq->private;
	int offset = TOE_LRO_OC_FLAG(0, 0, OC_MPTCP_WIN);
	void __iomem *reg = toe_ocflag(toe, offset);
	int tail;

	tail = FIELD_GET(LRO_OC5_LRO_TAIL, readl(reg));
	seq_printf(seq, "sw tail:%d hw tail:%d\n", toe->stall_idx, tail);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dbgfs_stall);

static int threshold_get(void *data, u64 *val)
{
	*val = readl(toe_reg((struct toe_dev *)data, TOE_LRO_THRESHOLD));
	return 0;
}

static int threshold_set(void *data, u64 val)
{
	struct toe_dev *toe = (struct toe_dev *)data;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (val > U16_MAX)
		return -EINVAL;

	lro_set_threshold(toe, val);
	dev_info(toe->dev, "change threshold to %llu\n", val);
	return 0;
}

static int timeout_set(void *data, u64 val)
{
	struct toe_dev *toe = (struct toe_dev *)data;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (val > U32_MAX)
		return -EINVAL;

	lro_set_timeout(toe, val);
	dev_info(toe->dev, "change timeout to %llu\n", val);
	return 0;
}

static int tso_disable_set(void *data, u64 val)
{
	struct toe_dev *toe = (struct toe_dev *)data;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	toe->tso_disable = !!val;
	return 0;
}

static int lro_start_set(void *data, u64 val)
{
	struct toe_dev *toe = (struct toe_dev *)data;

	if (val > LRO_PORT_CNT)
		return -EINVAL;

	if (val == LRO_PORT_CNT) {
		int i;

		for (i = 0; i < LRO_PORT_CNT; i++)
			lro_stop(toe, i);
		return 0;
	}

	return lro_start(toe, val, LRO_TYPE_TCP);
}

static int set_tso_low_wm(void *data, u64 val)
{
	struct toe_dev *toe = (struct toe_dev *)data;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	toe->tso_low_wm = val;
	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(tso_clear_fops, NULL, tso_clear_stats, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(tso_disable_fops, NULL, tso_disable_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(threshold_fops, threshold_get, threshold_set,
			 "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(timeout_fops, NULL, timeout_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(lro_start_fops, NULL, lro_start_set, "%llu\n");
DEFINE_DEBUGFS_ATTRIBUTE(tso_low_wm_fops, NULL, set_tso_low_wm, "%llu\n");

static void toe_debugfs_init(struct toe_dev *toe)
{
	toe->debugfs = debugfs_create_dir("toe", NULL);
	if (IS_ERR_OR_NULL(toe->debugfs))
		return;

	debugfs_create_file("tso", 0400, toe->debugfs, toe, &dbgfs_tso_fops);
	debugfs_create_file("lro", 0400, toe->debugfs, toe, &dbgfs_lro_fops);
	debugfs_create_file("lro_cfg", 0400, toe->debugfs, toe,
			    &dbgfs_lro_cfg_fops);
	debugfs_create_file("lro_nomatch", 0400, toe->debugfs, toe,
			    &dbgfs_nomatch_fops);
	debugfs_create_file("lro_stall", 0400, toe->debugfs, toe,
			    &dbgfs_stall_fops);
	debugfs_create_file("tso_clr_stats", 0200, toe->debugfs, toe,
			    &tso_clear_fops);
	debugfs_create_file("tso_disable", 0200, toe->debugfs, toe,
			    &tso_disable_fops);
	debugfs_create_file("lro_threshold", 0600, toe->debugfs, toe,
			    &threshold_fops);
	debugfs_create_file("lro_timeout", 0200, toe->debugfs, toe,
			    &timeout_fops);
	debugfs_create_file("lro_start", 0200, toe->debugfs, toe,
			    &lro_start_fops);
	debugfs_create_file("set_tso_low_wm", 0200, toe->debugfs, toe,
			    &tso_low_wm_fops);
	debugfs_create_file("get_tso_low_wm", 0400, toe->debugfs, toe,
			    &dbgfs_tso_low_wm_fops);
	debugfs_create_u32("ip_len_mismatch", 0400, toe->debugfs,
			   &toe->ip_len_mismatch_cnt);
}

static int update_dp_spl_conn(enum DP_SPL_TYPE type, unsigned int flag, int id)
{
	struct dp_spl_cfg *conn;
	int spl_id;

	conn = kzalloc(sizeof(*conn), GFP_KERNEL);
	if (!conn)
		return 0;

	conn->type = type;
	conn->flag = flag;
	conn->spl_id = id;

	if (dp_spl_conn(0, conn))
		return 0;

	spl_id = conn->spl_id;
	kfree(conn);
	return spl_id;
}

static int toe_remove(struct platform_device *pdev)
{
	struct toe_dev *toe = platform_get_drvdata(pdev);
	void __iomem *gctrl_reg;
	int i;

#ifdef CONFIG_NETFILTER
	if (!IS_ENABLED(CONFIG_MXL_SKB_EXT))
		lro_pktprs_exit(&toe->pktprs_priv);
#endif

	dp_register_lro_ops(NULL);
	dp_register_tx(DP_TX_TSO, NULL, NULL);

	if (!toe)
		return -ENODEV;

	pm_runtime_get_sync(toe->dev);
	pm_runtime_put_noidle(toe->dev);
	pm_runtime_disable(toe->dev);

	if (toe->dpid)
		update_dp_spl_conn(DP_SPL_TOE, DP_F_DEREGISTER, toe->dpid);

	debugfs_remove_recursive(toe->debugfs);
	for (i = 0; i < LRO_PORT_CNT; i++)
		lro_stop(toe, i);
	cancel_work_sync(&toe->lro_sc_wk);
	cancel_delayed_work_sync(&toe->lro_exp_wk);
	cancel_delayed_work_sync(&toe->wk_cleanup);
	for (i = 0; i < LRO_CACHE_CNT; i++)
		kmem_cache_destroy(toe->cache[i]);
	gctrl_reg = toe_reg(toe, TOE_TSO_GCTRL);
	for (i = 0; i < toe->tso_active_ports; i++) {
		struct tso_port *tp = toe->tso[i];
		int j;

		for (j = 0; j < TSO_DMA_CH_PER_PORT; j++) {
			if (tp->pch[j])
				dmaengine_pause(tp->pch[j]);
		}
		writel(readl(gctrl_reg) & ~TOE_GCTRL_EN_TSO(i), gctrl_reg);
		for (j = 0; j < TSO_DMA_CH_PER_PORT; j++) {
			if (tp->pch[j])
				dma_release_channel(tp->pch[j]);
		}
	}
	clk_disable_unprepare(toe->clk);
	if (toe->pool && toe->sram_sz)
		gen_pool_free(toe->pool, (unsigned long)toe->sram_va,
			      toe->sram_sz);
	if (toe->ddr_pool && toe->ddr_sz)
		gen_pool_free(toe->ddr_pool, toe->ddr_va, toe->ddr_sz);

	return 0;
}

static int toe_req_irq(struct platform_device *pdev, const char *name,
		       irq_handler_t hdl, void *dev_id)
{
	int irq = platform_get_irq_byname(pdev, name);

	name = devm_kstrdup(&pdev->dev, name, GFP_KERNEL);
	return devm_request_irq(&pdev->dev, irq, hdl, 0, name, dev_id);
}

static inline unsigned int tso_port_cmd_hw_avail(struct tso_port *tp)
{
	return (TSO_HWQ_DEPTH - (tp->cmd_enq - READ_ONCE(tp->cmd_deq)));
}

static inline unsigned int tso_port_cmd_sw_avail(struct tso_port *tp)
{
	return (TSO_CMDCNT_DEPTH - (tp->queue_cnt - READ_ONCE(tp->cmpl_cnt)));
}

static inline bool tso_port_cmd_avail(struct tso_port *tp, int size)
{
	return (tso_port_cmd_sw_avail(tp) &&
		((size <= tso_port_cmd_hw_avail(tp)) ||
		 (size > TSO_HWQ_DEPTH)));
}

/* note: called from tso_xmit() that is in softirq context */
static int __tso_port_maybe_stop_tx(struct tso_port *tp, int size,
				    struct sk_buff *skb)
{
	unsigned long flags;
	struct tso_wq_dev *wd;
	unsigned int tail;

	smp_mb();
	spin_lock_irqsave(&tp->wait_q.lock, flags);
	if (unlikely(tso_port_cmd_avail(tp, size))) {
		spin_unlock_irqrestore(&tp->wait_q.lock, flags);
		return 0;
	}

	if (unlikely(tp->wait_q.count == TP_MAX_DEV_TXQ)) {
		spin_unlock_irqrestore(&tp->wait_q.lock, flags);
		WARN_ONCE(1, "Wait Queue full- dev: %s port : %d txq: %d\n",
			  skb->dev->name, tp->id, skb_get_queue_mapping(skb));
		return 0;
	}

	tail = (tp->wait_q.head + tp->wait_q.count) % TP_MAX_DEV_TXQ;
	wd = &tp->wait_q.q[tail];
	wd->dev = skb->dev;
	wd->tx_qindex = skb_get_queue_mapping(skb);
	dev_hold(wd->dev);

	netif_stop_subqueue(wd->dev, wd->tx_qindex);
	dev_dbg(tp->toe->dev, "Stop Queue- dev: %s port : %d txq: %d\n",
		skb->dev->name, tp->id, wd->tx_qindex);

	tp->wait_q.count++;
	tp->wake_pending = tp->wait_q.count;
	spin_unlock_irqrestore(&tp->wait_q.lock, flags);

	return -EBUSY;
}

static inline int tso_port_maybe_stop_tx(struct tso_port *tp, int size,
					 struct sk_buff *skb)
{
	if (likely(tso_port_cmd_avail(tp, size)))
		return 0;

	return __tso_port_maybe_stop_tx(tp, size, skb);
}

static bool is_tso_feature_enabled(struct toe_dev *toe, struct net_device *dev,
				   enum TSO_PKT_TYPE type)
{
	bool enabled = false;

	switch (type) {
	case TSO_IPV4_TCP:
		enabled = !!(dev->features & NETIF_F_TSO);
		break;
	case TSO_IPV6_TCP:
		enabled = !!(dev->features & NETIF_F_TSO6);
		break;
	case TSO_IPV4_UDP:
	case TSO_IPV6_UDP:
		enabled = !!(dev->features & NETIF_F_GSO_UDP_L4);
		break;
	default:
		break;
	}

	return enabled;
}

static inline u16 toe_skb_padded(struct sk_buff *skb, struct pmac_tx_hdr *pmac)
{
	struct iphdr *iph;
	int maclen = pmac->ip_offset << 1;

	iph = (struct iphdr *)(skb_mac_header(skb) + maclen);
	return (skb->len - (ntohs(iph->tot_len) + maclen));
}

static int tso_xmit(struct sk_buff *skb, struct dp_tx_common *cmn, void *p)
{
	struct pmac_tx_hdr *pmac = (struct pmac_tx_hdr *)cmn->pmac;
	struct toe_dev *toe = (struct toe_dev *)p;
	struct dma_rx_desc_1 *desc_1;
	struct skb_shared_info *si;
	enum TSO_PKT_TYPE type;
	struct tso_port *tp;
	struct tso_gb *gb;
	int port;

	if (unlikely(toe->tso_disable))
		return DP_TX_FN_CONTINUE;

	if (unlikely(!cmn->pmac_len))
		return DP_TX_FN_CONTINUE;

	if (unlikely(!pmac || !pmac->tcp_chksum))
		return DP_TX_FN_CONTINUE;

	if (!skb->sk)
		return DP_TX_FN_CONTINUE;

	if (skb->ip_summed == CHECKSUM_PARTIAL && !pmac->tcp_chksum)
		return DP_TX_FN_CONTINUE;

	if (IS_ENABLED(CONFIG_ARCH_DMA_ADDR_T_64BIT)) {
		/* workaround for LMTSYS-2344: result 0 could be 0 */
		if (unlikely(!lower_32_bits(virt_to_phys(skb)))) {
			struct sk_buff *oskb = skb;

			skb = skb_clone(oskb, GFP_ATOMIC);
			if (!skb)
				return DP_TX_FN_CONTINUE;
			dev_consume_skb_any(oskb);
		}
	}

	si = skb_shinfo(skb);
	type = tso_get_type(pmac, si);

	if (!is_tso_feature_enabled(toe, skb->dev, type))
		return DP_TX_FN_CONTINUE;

	/* LGM B step limitations:
	 * TOE calculates wrong IPH total length for Padded short packets.
	 * If TOE corrects the IPH then desc length becomes less and
	 * PON IP doesn't pad the short packets and drops.
	 */
	if (toe->rev == TOE_REV_B) {
		if (skb->len <= ETH_ZLEN &&
		    (type == TSO_IPV4_TCP || type == TSO_IPV4_UDP) &&
			toe_skb_padded(skb, pmac)) {
			dev_dbg(toe->dev, "Packet padded [%u]\n", toe_skb_padded(skb, pmac));
			return DP_TX_FN_CONTINUE;
		}
	}

	WARN(!skb->data, "SKB no data?\n");
	if (unlikely(WARN(skb_headroom(skb) < sizeof(struct pmac_tx_hdr),
			  "please increase headroom size\n")))
		return DP_TX_FN_CONTINUE;

	port = skb_get_queue_mapping(skb);
	if (unlikely(port >= toe->tso_active_ports))
		port %= toe->tso_active_ports;

	tp = toe->tso[port];
	if (unlikely(!lock_tso_port(tp))) {
		tp->port_busy++;
		return DP_TX_FN_BUSY;
	}

	if (tso_port_maybe_stop_tx(tp, TSO_CMDCNT_MAXREQ(si->nr_frags), skb)) {
		tp->cmd_busy++;
		unlock_tso_port(tp);
		return DP_TX_FN_BUSY;
	}

	if (likely(skb_is_nonlinear(skb))) {
		gb = tso_pop_gather_buf(tp, skb);
		if (unlikely(!gb)) {
			tp->no_gb++;
			unlock_tso_port(tp);
			return DP_TX_FN_BUSY;
		}
	}

	if (cmn->toe_tc != TOE_TC_DEF_VALUE) {
		desc_1 = (struct dma_rx_desc_1 *)&skb->DW1;
		desc_1->field.classid = cmn->toe_tc;
	}

	if (unlikely(si->nr_frags > CMD_FIFO_SIZED_SKB_FRAGS))
		tp->hi_frags++;

	if (likely(skb->data == skb_mac_header(skb))) {
		skb_push(skb, sizeof(*pmac));
		memcpy(skb->data, pmac, sizeof(*pmac));
	}

	tp->cmd_cnt[TSO_CMDCNT_IDX(tp->queue_cnt)] = 0;

	pm_runtime_get_sync(toe->dev);

	if (likely(skb_is_nonlinear(skb)))
		tso_xmit_normal(toe->dev, tp, skb, si->gso_size ?: skb->len,
				type, false, gb);
	else
		tso_xmit_passthrough(toe->dev, tp, skb, TSO_MAX_MSS, type,
				     false);

	tp->cmd_enq += tp->cmd_cnt[TSO_CMDCNT_IDX(tp->queue_cnt)];
	tp->queue_cnt++;
	unlock_tso_port(tp);
	return 0;
}

static unsigned int get_gso_max_size(struct toe_dev *toe)
{
	return TSO_GSO_MAX_SIZE;
}

static unsigned int get_nr_txq(struct toe_dev *toe)
{
	return toe ? toe->tso_active_ports : 0;
}

static int cfg_netdev_feature(struct toe_dev *toe, struct net_device *dev,
			      bool en)
{
	netdev_features_t toe_features = NETIF_F_SG |
					 NETIF_F_HW_CSUM | NETIF_F_RXCSUM |
					 NETIF_F_TSO | NETIF_F_TSO6 |
					 NETIF_F_LRO | NETIF_F_GSO_UDP_L4 |
					 NETIF_F_GRO_HW;

	if (en) {
		dev->features |= toe_features;
		dev->hw_features |= toe_features;
		dev->vlan_features |= toe_features;
	} else {
		dev->features &= ~toe_features;
		dev->hw_features &= ~toe_features;
		dev->vlan_features &= ~toe_features;
	}

	return 0;
}

static int dp_register(struct toe_dev *toe)
{
	struct lro_ops ops = {
		.toe = toe,
		.lro_start = lro_start,
		.lro_stop = lro_stop,
		.lro_set_threshold = lro_set_threshold,
		.lro_set_timeout = lro_set_timeout,
		.lro_get_cap = lro_get_cap,
		.get_gso_max_size = get_gso_max_size,
		.get_nr_txq = get_nr_txq,
		.find_free_lro_port = find_free_lro_port,
		.cfg_netdev_feature = cfg_netdev_feature,
	};
	int ret;

	memcpy(&toe->ops, &ops, sizeof(ops));

	ret = dp_register_tx(DP_TX_TSO, tso_xmit, toe);
	if (ret)
		return ret;
	dp_register_lro_ops(&toe->ops);

	toe->dpid = update_dp_spl_conn(DP_SPL_TOE, 0, 0);
	if (!toe->dpid) {
		dev_warn(toe->dev, "LRO registration failed\n");
		return -ENOMEM;
	}

	return 0;
}

static u64 get_pool_cfg(struct platform_device *pdev)
{
	struct toe_dev *toe = platform_get_drvdata(pdev);
	const int DEFAULT_POOL = SZ_512;
	const int BITS_PER_POOL = 4;
	const int BITS_OFFSET = 7;
	struct device_node *node;
	struct property *p;
	u64 pool_cfg = 0;
	const __be32 *c;
	int pool = 0;
	int pool_cnt;
	int sizex;
	u32 val;

	pool_cnt = BITS_PER_TYPE(u64) / BITS_PER_POOL;
	sizex = __ffs(DEFAULT_POOL) - BITS_OFFSET;
	node = of_parse_phandle(pdev->dev.of_node, "buff-size", 0);
	of_property_for_each_u32(node, "mxl,bm-buff-pool-size", p, c, val) {
		unsigned long long field = __ffs(val) - BITS_OFFSET;
		int mask = BIT(BITS_PER_POOL) - 1;

		if (!val || field > mask || hweight32(val) != 1)
			field = sizex;

		pool_cfg = ror64(pool_cfg | field, BITS_PER_POOL);
		if (++pool >= pool_cnt)
			break;
	}
	for (; pool < pool_cnt; pool++)
		pool_cfg = ror64(pool_cfg | sizex, BITS_PER_POOL);
	dev_dbg(toe->dev, "pool cfg:%llx\n", pool_cfg);
	return pool_cfg;
}

static int toe_config_firewall(struct toe_dev *toe, phys_addr_t phys, int sz)
{
	struct fw_nb_apply_deferred_data def_data;
	struct fw_nb_resolve_data res_data;
	int ret;

	res_data.dev = toe->dev;
	res_data.pseudo_addr = (((u64)NOC_FW_SPECIAL_REGION_DYN) << 32) |
				NOC_FW_DYN_DDR_GATHERBUF_START;
	res_data.resolve_addr = phys;
	ret = mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_RESOLVE_ADDR,
					     &res_data);
	if (notifier_to_errno(ret)) {
		dev_dbg(toe->dev, "Failed resolving firewall start addr\n");
		return notifier_to_errno(ret);
	}

	res_data.pseudo_addr = (((u64)NOC_FW_SPECIAL_REGION_DYN << 32)) |
				NOC_FW_DYN_DDR_GATHERBUF_END;
	res_data.resolve_addr = phys + sz - 1;
	ret = mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_RESOLVE_ADDR,
					     &res_data);
	if (notifier_to_errno(ret)) {
		dev_dbg(toe->dev, "Failed resolving firewall end addr\n");
		return notifier_to_errno(ret);
	}

	def_data.dev = toe->dev;
	ret = mxl_fw_notifier_blocking_chain(NOC_FW_EVENT_APPLY_DEFE_RULE,
					     &def_data);
	if (notifier_to_errno(ret)) {
		dev_dbg(toe->dev, "Failed applying firewall deferred rule\n");
		return notifier_to_errno(ret);
	}

	return 0;
}

static int lro_set_ssb(struct toe_dev *toe, phys_addr_t sram, int sram_sz)
{
	u32 split_offset = 0;
	int ssb_sz;
	int ret;

	if (toe->rev < TOE_REV_C)
		return -EINVAL;

	ret = of_property_read_u32(toe->dev->of_node, "mxl,oc-mode",
				   &toe->oc_mode);
	if (ret || toe->oc_mode > EXT_OC_LEG)
		toe->oc_mode = EXT_OC_SPLT;

	switch (toe->oc_mode) {
	case EXT_OC_SPLT:
		ret = of_property_read_u32(toe->dev->of_node,
					   "mxl,oc-split-boundary",
					   &toe->split_boundary);
		if (ret || toe->split_boundary > LRO_PORT_CNT)
			toe->split_boundary = LRO_DEFAULT_SPLIT_BOUNDARY;
		split_offset = toe->split_boundary * LRO_OC_DEPTH *
			       sizeof(struct lro_result_ctx);
		ssb_sz = sizeof(struct lro_dma_descs) - split_offset;

		/* in split mode, starting point of external SSB to be used by
		 * hw is OC_DA + SPLIT_DA. So in order to make use of the whole
		 * defined sram, we shift sram address by SPLIT_DA.
		 */
		sram -= split_offset;
		break;
	case EXT_OC_EXT:
		ssb_sz = sizeof(struct lro_dma_descs);
		break;
	case EXT_OC_INT:
	case EXT_OC_LEG:
	default:
		return 0;
	}

	if (sram_sz < ssb_sz) {
		dev_warn(toe->dev, "sram size %d is too small for LRO SSB\n",
			 sram_sz);
		return -EIO;
	}

	toe->lro_ssb = devm_memremap(toe->dev, sram,
				     split_offset + ssb_sz, MEMREMAP_WB);
	if (!toe->lro_ssb) {
		dev_err(toe->dev, "Failed to remap MEMAXI\n");
		return -EIO;
	}

	writel(sram, toe_reg(toe, TOE_EXT_OC_DA));
	if (toe->oc_mode == EXT_OC_SPLT)
		writel(split_offset, toe_reg(toe, TOE_SPLIT_OC_DA));
	toe_update_bits(toe_reg(toe, TOE_LRO_CTRL), LRO_CTL_EXT_OC_EN,
			FIELD_PREP(LRO_CTL_EXT_OC_EN, toe->oc_mode));

	/* use non-posted mode per design team recommendation */
	toe_update_bits(toe_reg(toe, TOE_LRO_CTRL), LRO_CTL_LAST_WR_NP_EN,
			FIELD_PREP(LRO_CTL_LAST_WR_NP_EN, 1));
	toe_update_bits(toe_reg(toe, TOE_SPARE_TIE0), TIE_LRO_NONPOST_WRITE,
			FIELD_PREP(TIE_LRO_NONPOST_WRITE, 0));
	return 0;
}

static int toe_alloc_buf(struct toe_dev *toe)
{
	int port, sram_sz, sram_gb_sz, ddr_sz;
	struct tso_port *tp;
	phys_addr_t sram, ddr;
	size_t sz;
	struct mxl_pool_alloc_data data;
	struct device_node *np;
	int i;
	int ret;

	toe->pool = of_gen_pool_get(toe->dev->of_node, "mxl,sram-pool", 0);
	if (!toe->pool) {
		dev_err(toe->dev, "mxl,sram-pool not available\n");
		return -ENOMEM;
	}

	sz = gen_pool_avail(toe->pool);
	if (!sz)
		return -ENOMEM;

	toe->ddr_pool = of_gen_pool_get(toe->dev->of_node, "mxl,ddr-pool", 0);
	if (!toe->ddr_pool) {
		dev_err(toe->dev, "mxl,ddr-pool not available\n");
		return -ENOMEM;
	}

	/* Sanity check for module param */
	if (max_gso_sz > GSO_MAX_SIZE)
		max_gso_sz = GSO_MAX_SIZE;

	toe->tso_active_ports = TSO_PORT_CNT;

	toe->sram_sz = gen_pool_size(toe->pool);
	if (toe->sram_sz > sz || !toe->sram_sz) {
		dev_err(toe->dev,
			"mxl,sram-size %zu is larger than SRAM :%zu or 0\n",
			toe->sram_sz, sz);
		return -ENOMEM;
	}
	dev_info(toe->dev, "TSO allocating SRAM Size = %zu\n", toe->sram_sz);
	toe->sram_va = gen_pool_dma_alloc(toe->pool, toe->sram_sz,
					  &toe->sram_pa);
	if (!toe->sram_va) {
		dev_err(toe->dev, "failed to allocate sram\n");
		return -ENOMEM;
	}

	sram = toe->sram_pa;
	for (port = 0; port < toe->tso_active_ports; port++) {
		sram = setup_tso_port_hdr_buf(toe, port, sram);

		tp = devm_kzalloc(toe->dev, sizeof(*tp), GFP_KERNEL);
		if (!tp)
			return -ENOMEM;
		toe->tso[port] = tp;
	}

	ret = of_property_read_u32(toe->dev->of_node, "mxl,sram-gb",
				   &toe->gb_num);
	if (ret || toe->gb_num > TSO_MAX_GATHER_BUF)
		toe->gb_num = TSO_MAX_GATHER_BUF;

	sram_sz = toe->sram_sz - (sram - toe->sram_pa);
	sram_gb_sz = toe->gb_num * TSO_GATHER_BUF_SZ;
	if (sram_sz < sram_gb_sz)
		sram_gb_sz = sram_sz;

	toe->gb_num = tso_port_set_gather_buf(toe, sram_gb_sz, sram, 0,
					      TSO_GB_SRAM);
	sram += sram_gb_sz;
	sram_sz -= sram_gb_sz;

	if (lro_set_ssb(toe, sram, sram_sz))
		toe->oc_mode = EXT_OC_INT;

	ddr_sz = (TSO_MAX_GATHER_BUF - toe->gb_num) * TSO_GATHER_BUF_SZ;
	ddr_sz = ALIGN(ddr_sz, SZ_4K);

	for (i = 0; i < FW_DOM_MAX; i++) {
		toe->sai[i] = -1;

		np = of_parse_phandle(toe->dev->of_node, "firewall-domains", i);
		if (np)
			of_property_read_s32(np, "sai", &toe->sai[i]);
	}

	data.dev = toe->dev;
	data.sai = toe->sai[FW_DOM_TOE_DMA];
	data.perm = FW_READ_WRITE;
	data.opt = MXL_FW_OPT_USE_NONCOHERENT;
	toe->ddr_va = mxl_soc_pool_alloc(toe->ddr_pool, ddr_sz, &data);
	if (!toe->ddr_va)
		return -ENOMEM;
	toe->ddr_sz = ddr_sz;

	ddr = ioc_addr_to_nioc_addr(toe_get_phys_addr(toe->dev,
						      (void *)toe->ddr_va,
						      ddr_sz));
	toe_config_firewall(toe, ddr, ddr_sz);
	toe->gb_num += tso_port_set_gather_buf(toe, ddr_sz, ddr, toe->gb_num,
					       TSO_GB_DDR);

	return 0;
}

#ifdef CONFIG_PM
static int toe_runtime_suspend(struct device *dev)
{
	struct toe_dev *toe = dev_get_drvdata(dev);

	clk_disable(toe->clk);
	return 0;
}

static int toe_runtime_resume(struct device *dev)
{
	struct toe_dev *toe = dev_get_drvdata(dev);

	return clk_enable(toe->clk);
}
#endif

static const struct dev_pm_ops toe_dev_pm_ops = {
	SET_RUNTIME_PM_OPS(toe_runtime_suspend, toe_runtime_resume, NULL)
};

static int toe_probe(struct platform_device *pdev)
{
	void __iomem *gctrl_reg;
	struct toe_dev *toe;
	int ret;
	int i;
	int port;

	toe = devm_kzalloc(&pdev->dev, sizeof(*toe), GFP_KERNEL);
	if (!toe)
		return -ENOMEM;

	toe->dev = &pdev->dev;
	platform_set_drvdata(pdev, toe);
	INIT_WORK(&toe->lro_sc_wk, lro_sc_work);
	INIT_DELAYED_WORK(&toe->lro_exp_wk, lro_exp_work);
	INIT_DELAYED_WORK(&toe->wk_cleanup, lro_port_work_cleanup);

	toe->tso_option3 = of_property_read_bool(toe->dev->of_node, "option3");
	toe->clk = devm_clk_get(toe->dev, "g_toe");
	if (IS_ERR(toe->clk)) {
		ret = PTR_ERR(toe->clk);
		goto _error;
	}

	ret = clk_prepare_enable(toe->clk);
	if (ret)
		goto _error;

	toe->reset = devm_reset_control_get(toe->dev, "toe");
	if (IS_ERR(toe->reset)) {
		ret = PTR_ERR(toe->reset);
		goto _error;
	}

	reset_control_deassert(toe->reset);

	/* TSO only supports 32bit header and gather buffer */
	if (dma_set_mask(toe->dev, DMA_BIT_MASK(32))) {
		dev_err(toe->dev, "no suitable DMA available\n");
		goto _error;
	}

	for (i = 0; i < TOE_IOMEM_CNT; i++) {
		struct resource *r =
			platform_get_resource(pdev, IORESOURCE_MEM, i);
		void __iomem *p = devm_ioremap_resource(toe->dev, r);

		if (IS_ERR(p)) {
			ret = PTR_ERR(p);
			goto _error;
		}
		toe->base[i] = p;
	}
	toe->tso_low_wm = TSO_DEFAULT_LOW_WM;

	toe->rev = FIELD_GET(TOE_VERSION, readl(toe_reg(toe, TOE_TSO_DBG(0))));
	if (toe->rev < TOE_REV_A || toe->rev > TOE_REV_C) {
		dev_err(toe->dev, "unsupported TOE version %x\n", toe->rev);
		clk_disable_unprepare(toe->clk);
		return -ENODEV;
	}

	ret = toe_reset(toe);
	if (ret)
		goto _error;

	gctrl_reg = toe_reg(toe, TOE_TSO_GCTRL);
	toe->tie = TIE_TSO_EXCL_PMAC | TIE_LRO_NONPOST_WRITE |
		TIE_TSO_POST_SEL_SEG | TIE_LRO_WEIGHT1 | TIE_LRO_WEIGHT2 |
		TIE_LRO_WEIGHT3 | TIE_LRO_RST_TO_ONRCV;
	writel(toe->tie, toe_reg(toe, TOE_SPARE_TIE0));

	INIT_LIST_HEAD(&toe->gb_list);
	spin_lock_init(&toe->gb_lock);

	ret = toe_alloc_buf(toe);
	if (ret)
		return ret;

	for (i = 0; i < toe->tso_active_ports; i++) {
		struct tso_port *tp = toe->tso[i];
		void __iomem *int_en_reg;
		char irq_name[8];
		int j;

		tp->toe = toe;
		tp->id = i;
		tp->res = toe_reg(toe, TOE_TSO_RES(i));
		tp->sts = toe_reg(toe, TOE_INT_STS) + i;
		tp->flag = BIT(0);

		int_en_reg = TOE_INT_STS2EN(tp->sts);
		tp->cmd = toe_reg(toe, TOE_TSO_CMD(i));
		atomic_set(&tp->used, 0);
		spin_lock_init(&tp->lock);
		spin_lock_init(&tp->wait_q.lock);
		snprintf(irq_name, sizeof(irq_name), "tso%d", i);
		ret = toe_req_irq(pdev, irq_name, tso_isr, tp);
		if (ret)
			goto _error;

		for (j = 0; j < TSO_DMA_CH_PER_PORT; j++) {
			struct dma_chan *ch;
			char name[8];

			snprintf(name, sizeof(name), "p%dc%d", i, j);
			ch = dma_request_slave_channel(toe->dev, name);
			if (!ch) {
				dev_err(toe->dev,
					"failed to request DMA channel %s\n",
					name);
				ret = -EPROBE_DEFER;
				goto _error;
			}

			tp->pch[j] = ch;
			dma_async_issue_pending(tp->pch[j]);
		}

		writel(readl(gctrl_reg) | TOE_GCTRL_EN_TSO(i), gctrl_reg);
		writeb(readb(int_en_reg) | tp->flag, int_en_reg);
	}

	ret = lro_init(toe, get_pool_cfg(pdev));
	if (ret)
		goto _error;

	for (i = 0; i < LRO_GROUP_CNT; i++) {
		char irq_name[8];

		toe->group[i].toe = toe;
		toe->group[i].offset = i * LRO_PORT_PER_GROUP;
		toe->group[i].sts = (u8 *)toe_reg(toe, TOE_LRO_INT_STS(i));
		snprintf(irq_name, sizeof(irq_name), "lro%d", i);
		toe->group[i].irq = platform_get_irq_byname(pdev, irq_name);
		ret = toe_req_irq(pdev, irq_name, lro_isr, &toe->group[i]);
		if (ret)
			goto _error;
	}

	for (i = 0; i < LRO_PORT_CNT / BITS_PER_TYPE(u32); i++)
		spin_lock_init(&toe->en_lock[i]);

	for (port = 0; port < LRO_PORT_CNT; port++) {
		memset(&lro_port_state_table[port], 0,
		       sizeof(struct lro_port_state));
		lro_port_state_table[port].index = port;
	}

	ret = toe_req_irq(pdev, "lro_sc", lro_stall_isr, toe);
	if (ret)
		goto _error;

	ret = toe_req_irq(pdev, "lro_err", lro_err_isr, toe);
	if (ret)
		goto _error;

	ret = toe_req_irq(pdev, "lro_exp", lro_exp_isr, toe);
	if (ret)
		goto _error;

	toe_debugfs_init(toe);
	ret = dp_register(toe);
	if (ret)
		goto _error;

#ifdef CONFIG_NETFILTER
	if (!IS_ENABLED(CONFIG_MXL_SKB_EXT))
		lro_pktprs_init(toe->dev, &toe->pktprs_priv);
#endif
	pm_runtime_get_noresume(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_irq_safe(&pdev->dev);
	pm_runtime_set_autosuspend_delay(&pdev->dev, 2000);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
	pm_runtime_mark_last_busy(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);

	dev_info(toe->dev, "TOE rev %X initialized.\n", toe->rev);
	return 0;

_error:
	dev_err(toe->dev, "failed to load TOE: %d\n", ret);
	toe_remove(pdev);
	return ret;
}

static const struct of_device_id id_match[] = {
	{.compatible = "mxl,lgm-toe"},
	{}
};
MODULE_DEVICE_TABLE(of, id_match);

static struct platform_driver toe_driver = {
	.driver = {
		.name = "toe",
		.of_match_table = of_match_ptr(id_match),
		.pm = &toe_dev_pm_ops,
	},
	.probe = toe_probe,
	.remove = toe_remove,
};
module_platform_driver(toe_driver);

MODULE_AUTHOR("Li Yin <yin1.li@intel.com>");
MODULE_LICENSE("GPL v2");
