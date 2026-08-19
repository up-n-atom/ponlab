/*******************************************************************************

  Intel SmartPHY DSL PCIe TC driver
  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/
#ifndef __TC_MAIN_H__
#define __TC_MAIN_H__

#include <net/dc_ep.h>
#include "dsl_tc.h"

#define EP_MAX_NUM	(DC_EP_MAX_PEER + 1)
#define TCPRIV_ALIGN	32
#define DMA_PACKET_SZ	2048
#define PMAC_SIZE		8
#ifdef CONFIG_LTQ_UMT_518_FW_SG
#define FIX_DMA_BYTE_ALIGNMENT 1
#endif


#define FEATURE_CONF_DESC_LENGTH 1

#define FEATURE_POWER_DOWN 1

#ifdef FIX_DMA_BYTE_ALIGNMENT
#define DMA_RXOUT_SRC_OWN 1
#define DMA_SG_TX_OWN 1
#define DMA_SG_RX_OWN 1
#define DMA_RXOUT_DST_OWN 1
#endif /* FIX_DMA_BYTE_ALIGNMENT */

#define FEATURE_PPE_DYNAMIC_FEQ 1

enum {
	MSG_RX		= BIT(0),
	MSG_TX		= BIT(1),
	MSG_EVENT	= BIT(2),
	MSG_RXDATA	= BIT(3),
	MSG_TXDATA	= BIT(4),
	MSG_INIT	= BIT(5),
	MSG_OAM_RX	= BIT(6),
	MSG_OAM_TX	= BIT(7),
	MSG_OAM_RXDATA	= BIT(8),
	MSG_OAM_TXDATA	= BIT(9),
	MSG_QOS		= BIT(10),
	MSG_SWITCH	= BIT(11),
	MSG_LOOPBACK	= BIT(12),
	MSG_MIB		= BIT(14),
};

#define tc_level(level, priv, type, fmt, args...)	\
do {							\
	if (priv->msg_enable & (type))			\
		pr_##level##_ratelimited(fmt, ##args);	\
} while (0)

#define tc_dbg(priv, type, fmt, args...)		\
	tc_level(debug, priv, type, fmt, ##args)
#define tc_err(priv, type, fmt, args...)		\
	tc_level(err, priv, type, fmt, ##args)
#define tc_info(priv, type, fmt, args...)		\
	tc_level(info, priv, type, fmt, ##args)

#define TC_DEF_DBG	(MSG_INIT | MSG_SWITCH | MSG_EVENT | MSG_LOOPBACK)
#define DUMP_HDR_SZ	32

enum tc_dir {
	US_DIR = 0,
	DS_DIR = 1,
};

enum tc_status {
	TC_RUN = 0,	/* TC loaded succesfully */
	TC_INIT,	/* in Initialization */
	NO_TC,		/* Init done, but NO TC loaded */
	TC_SWITCHING,	/* TC is swiching */
	TC_EXIT,	/* module is going to be unloadeds */
	TC_ERR,
};

enum dsl_tc_mode {
	TC_ATM_SL_MODE = 0, /* ATM Single line mode */
	TC_PTM_SL_MODE = 1, /* PTM Single line mode */
	TC_PTM_BND_MODE = 2, /* PTM Bonding mode */
	TC_NONE_MODE,
};

enum tc_pkt_type {
	ATM_SL_PKT = 0,
	PTM_SL_PKT,
	PTM_BOND_PKT,
	ATM_OAM_PKT,
};
#ifdef FEATURE_POWER_DOWN
struct conf_parameter {
	char *name;
	char *value;
	int valid;
};
enum power_conf_opts {
	SW_OFF_AFE = 0,
	SW_OFF_DSL_LINE_DRIVER,
	SW_OFF_VRX518_CLOCK,
	SW_OFF_VRX518_PLL,
};
enum conf_para_var {
	xDSL_Cfg_LdAfeShutdown = 0,
	xDSL_Cfg_PLL_SwitchOff,
	xDSL_Cfg_PLL_MAX,
};
#endif
struct soc_cfg {
	u32 txin_dbase;
	u32 txin_dnum;
	u32 txout_dbase;
	u32 txout_dnum;
	u32 rxin_dbase;
	u32 rxin_dnum;
	u32 rxout_dbase;
	u32 rxout_dnum;
	u32 desc_dw_sz;
};

struct tc_param {
	unsigned int p2p:1;
	unsigned int dfe_loopback:1;
	unsigned int bonding_en:1;
	unsigned int sharing_cdma_en:1;
	unsigned int cdma_desc_loc:2;
	unsigned int ps:1;
	unsigned int res0:25;

	unsigned int umt_period;
	unsigned int qsb_tstep;
	unsigned int qsb_tau;
	unsigned int qsb_srvm;
	unsigned int aal5r_max_pktsz;
	unsigned int aal5r_min_pktsz;
	unsigned int aal5s_max_pktsz;
	unsigned int oam_prio;
	#ifdef FIX_DMA_BYTE_ALIGNMENT
	unsigned int ena_dma_tc_sg;
	#endif
#ifdef FEATURE_CONF_DESC_LENGTH
	/* __US_FAST_PATH_DES_LIST_NUM:64
	 * __ACA_TX_IN_PD_LIST_NUM
	 * __ACA_TX_OUT_PD_LIST_NUM
	 * */
	u32 conf_us_fp_desq_len;
	/*
	 * Number of queue per QoS queue: QOS_DES_NUM / QOSQ_NUM
	 * */
	u32 conf_us_qos_queue_len;
	/* __US_OUTQ0_DES_LIST_NUM: 32
	 * __US_OUTQ1_DES_LIST_NUM: 32
	 * OUTQ_DESC_PER_Q
	 * */
	u32 conf_us_outq_len;
	/**/
	u32 conf_us_local_q0_desq_len;
#endif
};

struct cdma {
	u32 chans;
};

#define DSL_DMA_CTRL_ID DMA0
#define DSL_DMA_PORT_ID DMA0_MEMCOPY
#define FW_DMA_RX_CID DMA_CHANNEL_12
#define FW_DMA_TX_CID DMA_CHANNEL_13

#define FW_DMA0_DES_NUM 255
#define FW_DMA_PACKET_LEN 2048
#define FW_DMA_DES_SIZE 8
#define FW_DMA_4DW_DES_LEN 16

#ifdef FIX_DMA_BYTE_ALIGNMENT
struct soc_dma_ch {
	u32 rch_dbase;
	u32 tch_dbase;
	u32 rch_dnum;
	u32 tch_dnum;
	u32 rch_cid;
	u32 tch_cid;
	u32 onoff;
	char rch_name[32];
	char tch_name[32];
};

struct soc_tc_dma_conf
{
	void *shared_mem;
	void *dma_device;
	/* DMA1TX & DMA0 descriptors list */
	dma_addr_t rxout_src_dbase_phy;
	u32 rxout_src_dbase_vir;
	u32 rxout_src_dnum;
	dma_addr_t tch_dbase_phy;
	u32 tch_dbase_vir;
	dma_addr_t rch_dbase_phy;
	u32 rch_dbase_vir;
	dma_addr_t rxout_dst_dbase_phy;
	u32 rxout_dst_dbase_vir;
	u32 rxout_dst_dnum;
	/* data pointer allocation */
	u32 rxout_data_ptr_lst_vir;
	dma_addr_t rxout_data_ptr_lst_phy;
	struct mips_tc_rxout_dst_cache_ctxt *cache_rxout_ptr;
	u32 cache_rxout_ptr_vir;
	dma_addr_t cache_rxout_ptr_phy;
	/* UMT RXIN HD */
	u32 aca_umt_rxin_hd_base_phy;
	u32 aca_umt_rxin_hd_base_vir;
	u32 mips_tc_umt_rxin_hd_base_vir;
	u32 mips_tc_umt_rxin_hd_base_phy;
	struct soc_dma_ch dma_ch_conf;
};
#endif

struct tc_hw_ops {
	/*PTM/ATM/BONDING callback functions */
	void (*recv)(struct net_device *pdev, struct sk_buff *skb);
	int (*get_qid)(struct net_device *pdev, struct sk_buff *skb,
		void *vcc, uint32_t flags);
	void (*irq_on)(u32 irq_no);
	void (*irq_off)(u32 irq_no);
	int (*showtime_enter)(const unsigned char idx,
		struct port_cell_info *port_cell, void *xdata_addr);
	int (*showtime_exit)(const unsigned char idx);
	/*int (*tc_reset)(const unsigned char idx, u32 type);*/
	int (*erb_addr_get)(const unsigned char idx,
		unsigned int *data_addr, unsigned int *desc_addr);
	int (*framer_request_en)(const unsigned char idx);

	/*SoC callback functions */
	int (*send)(struct net_device *pdev, struct sk_buff *skb,
			int qid, enum tc_pkt_type type);
	void *(*alloc)(size_t size, enum tc_dir dir);
	void (*free)(dma_addr_t phyaddr, enum tc_dir dir);
	int (*dev_reg)(struct net_device *pdev, char *dev_name,
			int *subif_id, int flag);
	void (*dev_unreg)(struct net_device *pdev, char *dev_name,
			int subif_id, int flag);
	/*umt init/exit including the corresponding DMA init/exit */
	int (*umt_init)(u32 umt_id, u32 umt_period, u32 umt_dst, u32 v_umt_dst);
	void (*umt_exit)(u32 umt_id);
	void (*umt_start)(u32 umt_id);
	int (*soc_cfg_get)(struct device *dev,
		struct soc_cfg *cfg, u32 umt_id, int bonding);
	void (*disable_us)(int en);
	int (*get_mib)(struct net_device *pdev, struct rtnl_link_stats64 *stat);
#ifdef FIX_DMA_BYTE_ALIGNMENT
	int (*soc_fw_init)(struct device *dev);
	void (*soc_fw_deinit)(struct device *dev);
#endif
};

enum fw_id {
	FW_TX = 0,
	FW_BONDING,
	FW_RX,
	FW_ATM,
	FW_MAX
};

struct fw_info {
	__be32 fw_id;
	__be32 fw_size;
};

struct fw_ver_id {
	/* DWORD 0 */
	unsigned int family:4;
	unsigned int package:4;
	unsigned int major:8;
	unsigned int mid:8;
	unsigned int minor:8;

	/* DWORD 1 */
	unsigned int features;
} __packed;

struct fw_hdr {
	/* header information */
	struct fw_ver_id ptm_ver;
	struct fw_ver_id atm_ver;
	__be32 comp_id;
	__be32 hdr_sz;
	__be32 date;
	__be32 res[9];
	/* firmware information */
	__be32 fw_num;
	struct fw_info fw_info[FW_MAX];
};

struct fw_bin {
	const struct firmware *fw;
	struct fw_hdr fw_hdr;
	const u8 *fw_ptr[FW_MAX];
};

struct tc_priv {
	struct dc_ep_dev ep_dev[EP_MAX_NUM]; /* EP info from EP driver */
	int ep_num; /* EP num, if two, Bonding feature will be enabled */
	int showtime[EP_MAX_NUM]; /* showtime status */
	enum tc_status tc_stat;
	enum dsl_tc_mode tc_mode;
	u32 tc_idx;
	u32 msg_enable;
	struct fw_bin fw;
	struct proc_dir_entry *proc_dir;
	struct tc_hw_ops tc_ops;
	struct tc_param param;
	struct soc_cfg cfg;
	struct cdma dma;
	void *priv; /* point to ATM/PTM TC structure */
	u32 switch_exist;
	spinlock_t tc_lock;
#ifdef FIX_DMA_BYTE_ALIGNMENT
	struct soc_tc_dma_conf soc_tc_conf;
#endif
#ifdef FEATURE_PPE_DYNAMIC_FEQ
	u32 dsl_fw_freq_ind;
#endif
#ifdef FEATURE_POWER_DOWN
	u32 dsl_pwr_policy[EP_MAX_NUM];
#endif
};

/**
 * Common share struct for ATM/PTM/Bonding priv structure
 */
struct tc_comm {
	struct dc_ep_dev *ep; /* This should be always the first one */
	struct tc_priv *tc_priv;
	u32 ep_id;
};

/* p is a pointer to ATM/PTM/Bonding priv structure */
#define to_tcpriv(p)	(((struct tc_comm __force *)p)->tc_priv)
#define to_epdev(p)	(((struct tc_comm __force *)p)->ep)


extern void ptm_tc_unload(enum dsl_tc_mode);
extern void ptm_tc_load(struct tc_priv *, u32, enum dsl_tc_mode);
extern void ptm_exit(void);
extern void tc_request(u32, enum dsl_tc_mode);
extern void tc_unload(struct tc_priv *);
extern void tc_load(struct tc_priv *, u32, enum dsl_tc_mode);
extern void tc_get_drv_version(const char **, const char **);
#ifdef FEATURE_POWER_DOWN
extern int conf_init(struct tc_priv *priv);
extern int conf_get_int(enum conf_para_var, char *, int *);
extern int conf_get_str(enum conf_para_var, char *, char *);
extern void power_configure(struct tc_priv *,int ,int ,enum power_conf_opts);
#endif
extern int atm_tc_load(struct tc_priv *, u32 , enum dsl_tc_mode);
extern void atm_tc_unload(void);
extern void atm_exit(void);
#endif /* __TC_MAIN_H__ */

