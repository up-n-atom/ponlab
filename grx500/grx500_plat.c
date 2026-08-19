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
#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
#include <lantiq_dmax.h>
#else
#include <linux/dma/lantiq_dmax.h>
#endif
#include <linux/wait.h>
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/etherdevice.h>
#include <linux/workqueue.h>
#include <linux/ltq_hwmcpy.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
#include <net/lantiq_cbm.h>
#endif
#include <net/lantiq_cbm_api.h>
#include <net/datapath_api.h>


#include "inc/tc_main.h"
#include "inc/reg_addr.h"
#include "inc/platform.h"
#include "inc/dsl_tc.h"

#define DS_DEF_DESC_NUM		2048

/* PMAC configuration */
#define PMAC_SPPID		0xF
#define PMAC_SPPID_S		4
#define PMAC_PMAP_EN		0x1
#define PMAC_PMAP_EN_S		0x7
#define PMAC_FCS_INS		0x1
#define PMAC_FCS_INS_S		0x7

/* skb port configuration */
#define SKB_PORT_ID		0xF
#define SKB_PORT_ID_S		8
#define SKB_PORT_SUBID		0x7FFF
#define SKB_PORT_SUBID_S	0


struct plat_priv *g_plat_priv;
static struct module tc_mod;

void *ppa_callback_get(e_ltq_mei_cb_type type)
{
	struct plat_priv *priv;

	priv = g_plat_priv;
	if (WARN_ON(priv == NULL))
		return NULL;

	switch (type) {
	case LTQ_MEI_SHOWTIME_CHECK:
		return priv->dsl_ops.check_ptr;
	case LTQ_MEI_SHOWTIME_ENTER:
		return priv->dsl_ops.enter_ptr;
	case LTQ_MEI_SHOWTIME_EXIT:
		return priv->dsl_ops.exit_ptr;
	case LTQ_MEI_TC_REQUEST:
		return priv->dsl_ops.req_tc_ptr;
	case LTQ_MEI_TC_RESET:
		return priv->dsl_ops.tc_reset_ptr;
	case LTQ_MEI_ERB_ADDR_GET:
		return priv->dsl_ops.erb_addr_ptr;

	default:
		tc_err(priv->tc_priv, MSG_INIT,
			"get mei unknown function type %d\n", type);
		return NULL;
	}
}
EXPORT_SYMBOL(ppa_callback_get);

int ppa_callback_set(e_ltq_mei_cb_type type, void *func)
{
	struct plat_priv *priv;

	priv = g_plat_priv;
	if (WARN_ON(priv == NULL))
		return -ENODEV;

	switch (type) {
	/* save func address within global struct */
	case LTQ_MEI_SHOWTIME_CHECK:
		priv->dsl_ops.check_ptr = func;
		break;
	case LTQ_MEI_SHOWTIME_ENTER:
		priv->dsl_ops.enter_ptr = func;
		break;
	case LTQ_MEI_SHOWTIME_EXIT:
		priv->dsl_ops.exit_ptr = func;
		break;
	case LTQ_MEI_TC_REQUEST:
		priv->dsl_ops.req_tc_ptr = func;
		break;
	case LTQ_MEI_TC_RESET:
		priv->dsl_ops.tc_reset_ptr = func;
		break;
	case LTQ_MEI_ERB_ADDR_GET:
		priv->dsl_ops.erb_addr_ptr = func;
		break;

	default:
		tc_err(priv->tc_priv, MSG_INIT,
			"set mei unknown function type: %d\n", type);
		return -1;
	}

	return 0;
}
EXPORT_SYMBOL(ppa_callback_set);

size_t plat_priv_sz(void)
{
	return sizeof(struct plat_priv);
}

static inline void *tc_plat_priv(struct tc_priv *priv)
{
	return (char *)priv + ALIGN(sizeof(struct tc_priv), TCPRIV_ALIGN);
}

static inline struct tc_priv *plat_to_tcpriv(void)
{
	return g_plat_priv->tc_priv;
}

static int32_t plat_rx(struct net_device *rxdev, struct net_device *txdev,
	struct sk_buff *skb, int32_t len)
{
	int32_t err;
	struct tc_priv *tc_priv = plat_to_tcpriv();

	if (unlikely(!tc_priv)) {
		tc_dbg(tc_priv, MSG_RX,
			"Invalid TC private data\n");
		err = -EINVAL;
		goto err1;
	}
	/* remove PMAC header */
	if (tc_priv->tc_mode != TC_ATM_SL_MODE)
		skb_pull(skb, PMAC_SIZE);

	tc_priv->tc_ops.recv(rxdev, skb);
	return 0;

err1:
	dev_kfree_skb_any(skb);

	return err;
}

static int32_t plat_get_subifid(struct net_device *dev, struct sk_buff *skb,
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
	void *subif_data, uint8_t dst_mac[MAX_ETH_ALEN],
#else
	void *subif_data, uint8_t dst_mac[DP_MAX_ETH_ALEN],
#endif
	dp_subif_t *subif, uint32_t flags)
{
	int qid;
	struct tc_priv *priv = plat_to_tcpriv();

	qid = priv->tc_ops.get_qid(dev, skb, subif_data, flags);
	if (qid < 0)
		return qid;
	else
		subif->subif = qid;

	return 0;
}
static int plat_send(struct net_device *pdev, struct sk_buff *skb,
	int qid, enum tc_pkt_type type)
{
	struct plat_priv *priv = g_plat_priv;
	dp_subif_t dp_id;
	u32 flags;

	dp_id.port_id = priv->port_id;
	dp_id.subif = qid;
	flags = 0;

	set_mask_bit(skb->DW1, priv->port_id, SKB_PORT_ID, SKB_PORT_ID_S);
	set_mask_bit(skb->DW0, qid, SKB_PORT_SUBID, SKB_PORT_SUBID_S);

	if (type == PTM_BOND_PKT)
		flags = DP_TX_DSL_FCS;
	if (type == ATM_OAM_PKT)
		flags = DP_TX_OAM;

	return dp_xmit(pdev, &dp_id, skb, skb->len, flags);
}

/* return virtual address */
static void *plat_mem_alloc(size_t size, enum tc_dir dir)
{
	if (dir == US_DIR && size > CBM_STD_BUF_SIZE) {
		tc_err(plat_to_tcpriv(), MSG_INIT,
			"US alloc size(%d) bigger than CBM STD SIZE %d\n",
			size, CBM_STD_BUF_SIZE);
		return NULL;
	}

	if (dir == US_DIR) { /* ALLOC CBM buffer */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
		return cbm_buffer_alloc(0, 0);
#else
		return cbm_buffer_alloc(0, 0, size);
#endif
	} else {
		return kmalloc(size, GFP_KERNEL);
	}
}

static void plat_mem_free(u32 phy_addr, enum tc_dir dir)
{
	void *mem;

	if (dir == US_DIR)
		cbm_buffer_free(0, phy_addr, 1);
	else {
		mem = (void * __force)__va(phy_addr);
		kfree(mem);
	}
}

static void pmac_cfg_set(u32 pid, enum dsl_tc_mode tc_mode)
{
	struct dp_pmac_cfg pmac_cfg;

	memset(&pmac_cfg, 0, sizeof(pmac_cfg));

	if (tc_mode >= TC_NONE_MODE)
		return;

	pmac_cfg.eg_pmac_flags = EG_PMAC_F_PMAC |
			EG_PMAC_F_FCS | EG_PMAC_F_TCENA;

	pmac_cfg.eg_pmac.pmac = 0;
	pmac_cfg.eg_pmac.tc_enable = 1;
	pmac_cfg.eg_pmac.fcs = (tc_mode == TC_PTM_BND_MODE) ? 1 : 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
	pmac_cfg.ig_pmac_flags = IG_PMAC_F_PRESENT | IG_PMAC_F_SPID
				| IG_PMAC_F_PMACHDR3 | IG_PMAC_F_PMACHDR5;
#else
	pmac_cfg.ig_pmac_flags = IG_PMAC_F_PRESENT | IG_PMAC_F_SPID
				| IG_PMAC_F_PMACHDR3 | IG_PMAC_F_PMACHDR5
                                | IG_PMAC_F_PMAP | IG_PMAC_F_PMAPENA;
#endif

	if (tc_mode == TC_PTM_BND_MODE)
		pmac_cfg.ig_pmac.tx_dma_chan = g_plat_priv->dma[1].chan;
	else
		pmac_cfg.ig_pmac.tx_dma_chan = g_plat_priv->dma[0].chan;

	pmac_cfg.ig_pmac.pmac = 0;
	pmac_cfg.ig_pmac.def_pmac_src_port = 1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        pmac_cfg.ig_pmac.def_pmac_en_pmap=1;
        pmac_cfg.ig_pmac.def_pmac_pmap=1;
#endif
	pmac_cfg.ig_pmac.def_pmac_hdr[2] = SM(pid, PMAC_SPPID);
	if (tc_mode == TC_ATM_SL_MODE) /* Packet has NO FCS in ATM mode */
		pmac_cfg.ig_pmac.def_pmac_hdr[4] = SM(0, PMAC_FCS_INS);
	else
		pmac_cfg.ig_pmac.def_pmac_hdr[4] = SM(1, PMAC_FCS_INS);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
	dp_pmac_set(pid, &pmac_cfg);
#else
	dp_pmac_set(0, pid, &pmac_cfg);
#endif
}

static int plat_dp_init(struct plat_priv *priv, const char *drv_name)
{
	struct tc_priv *tcpriv;
	int dp_id;
	int err;
	int flags;

	tcpriv = priv->tc_priv;
	priv->owner = THIS_MODULE;
	if (!priv->owner) {
		memset(&tc_mod.name, 0, sizeof(tc_mod.name));
		snprintf(tc_mod.name, MODULE_NAME_LEN - 1, "%s", drv_name);
		priv->owner = &tc_mod;
	}

	flags = DP_F_FAST_DSL | DP_F_FAST_WLAN;
	if (tcpriv->ep_num > 1)
		flags |= DP_F_DSL_BONDING;
	dp_id  = dp_alloc_port(priv->owner, NULL, 0, 0, NULL, flags);
	if (dp_id == DP_FAILURE) {
		tc_err(tcpriv, MSG_INIT, "dp_alloc_port fail!\n");
		return -ENOMEM;
	}
	priv->port_id		= dp_id;
	priv->cb.stop_fn	= NULL;
	priv->cb.restart_fn	= NULL;
	priv->cb.rx_fn		= plat_rx;
	priv->cb.get_subifid_fn = plat_get_subifid;
	if (dp_register_dev(priv->owner, dp_id, &priv->cb, 0) != DP_SUCCESS) {
		tc_err(tcpriv, MSG_INIT,
			"dp_register_dev failed for port id %d", dp_id);
		err = -ENODEV;
		goto err1;
	}

	return 0;

err1:
	dp_alloc_port(priv->owner, NULL, 0, dp_id, NULL, DP_F_DEREGISTER);
	return err;
}

static void plat_dp_exit(struct plat_priv *priv)
{
	dp_register_dev(priv->owner, priv->port_id, NULL, DP_F_DEREGISTER);
	dp_alloc_port(priv->owner, NULL, 0,
		priv->port_id, NULL, DP_F_DEREGISTER);
}
#define CBM_NEW_FLUSH 1

static void plat_umt_start(u32 id)
{
	struct plat_dma *dma;
	struct plat_umt *umt;
#ifdef CBM_NEW_FLUSH
	uint32_t num_tmu_ports = 0;
	cbm_tmu_res_t *cbm_tmu_res = NULL;
	uint32_t tmu_port = 0;
	int32_t cbm_port_id = -1;
	int ret;

	ret = cbm_dp_port_resources_get(
		(uint32_t *)&g_plat_priv->port_id,
		&num_tmu_ports,
		&cbm_tmu_res, 0);
	if ((NULL != cbm_tmu_res) && (ret >= 0)) {
		tmu_port = cbm_tmu_res[0].tmu_port;
		cbm_port_id = cbm_tmu_res[0].cbm_deq_port;
#if 0 /* for debug only */
		pr_info("%s-%d: port_id[%d] num_tmu_ports[%d ]tmu_port[%d] cbm_port_id[%d]\n",
			__func__, __LINE__, g_plat_priv->port_id,
			num_tmu_ports, tmu_port, cbm_port_id);
		for (i = 0; i < num_tmu_ports; i++) {
			pr_info("%d: %d %d %d\n", i, cbm_tmu_res[i].tmu_port,
				cbm_tmu_res[i].cbm_deq_port,
				cbm_tmu_res[i].tmu_q);

		}
#endif
		kfree(cbm_tmu_res);
	}
#endif
	/*Reset CBM port */
	cbm_port_quick_reset(g_plat_priv->cbm_pid, CBM_PORT_F_DEQUEUE_PORT);
#ifdef CBM_NEW_FLUSH
	if (ret >= 0) {
		pr_info("Flushing the TMU queues - cbm_pid=%d, tmu_port=%d.\n",
				g_plat_priv->cbm_pid, tmu_port);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
		cbm_dp_q_enable(g_plat_priv->port_id,
			-1, tmu_port, -1 , 200000, 0,
			(CBM_Q_F_FLUSH | CBM_Q_F_FORCE_FLUSH));
#else
		cbm_dp_q_enable(0, g_plat_priv->port_id,
			-1, tmu_port, -1 , 200000, 0,
			(CBM_Q_F_FLUSH | CBM_Q_F_FORCE_FLUSH));
#endif
		/* Reset the Dequeue port current desc index and buffer return desc index */
		pr_info("Resetting CBM port - port_id=%d, cbm_port_id=%d.\n",
			g_plat_priv->port_id, cbm_port_id);
		cbm_port_quick_reset(cbm_port_id, CBM_PORT_F_DEQUEUE_PORT);
	}
#endif


	dma = &g_plat_priv->dma[id];
	ltq_dma_chan_on(dma->dma_chan);

	umt = &g_plat_priv->umt[id];
	ltq_umt_enable(umt->id, g_plat_priv->port_id, UMT_ENABLE);

}

static int plat_dma_init(u32 id)
{
	struct plat_dma *dma;
	struct plat_priv *priv;
	int ret;

	priv = g_plat_priv;
	dma = &priv->dma[id];
	dma->ds_dnum = DS_DEF_DESC_NUM;

	ret = ltq_request_dma(dma->dma_chan, priv->owner->name);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "request DMA fail!\n");
		return ret;
	}

	ret = ltq_dma_chan_desc_alloc(dma->dma_chan, dma->ds_dnum);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT,
			"Alloc dma desc (%d)fail!\n", dma->ds_dnum);
		goto err1;
	}
	ltq_dma_chan_off(dma->dma_chan);

	return 0;
err1:
	ltq_free_dma(dma->dma_chan);
	return ret;
}

/* init UMT but not started */
static int plat_umt_init(u32 id, u32 period, u32 dst_addr, u32 v_dst_addr)
{
	struct plat_priv *priv;
	struct plat_umt *umt;
	u32 dma_ctrlid;
	u32 dma_cid;
	u32 cbm_id;
	int ret;

	if (WARN_ON(id >= EP_MAX_NUM))
		return -EINVAL;

	priv = g_plat_priv;
	umt = &priv->umt[id];

	ret = cbm_get_wlan_umt_pid(priv->port_id, &cbm_id);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "Get CBM pid fail!\n");
		goto err1;
	}

	umt->cbm_id = cbm_id + id;
	ret = ltq_umt_request(priv->port_id, umt->cbm_id,
		&dma_ctrlid, &dma_cid, &umt->id);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "Requst UMT port fail!\n");
		goto err1;
	}
	/* FIXME: UMT driver should directly return dma_chan,
	 * wave500 dp change required as well
	 */
	tc_info(priv->tc_priv, MSG_INIT,
		"id: %d, CBM ID: %d, UMT ID: %d, DMA CTRLID: %d, DMA CID: %d\n",
		id, umt->cbm_id, umt->id, dma_ctrlid, dma_cid);
	priv->dma[id].dma_chan = _DMA_C(dma_ctrlid, DMA1TX_PORT, dma_cid);
	priv->dma[id].chan = dma_cid;
	umt->period = period;
	umt->dst = dst_addr;
	plat_dma_init(id);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
	ret = ltq_umt_set_mode(umt->id, priv->port_id, UMT_SELFCNT_MODE,
		UMT_MSG0_MSG1, umt->dst, umt->period, UMT_DISABLE);
#else
	while (1) {
		struct umt_set_mode umt_mode;
		umt_mode.umt_mode = UMT_SELFCNT_MODE;
		umt_mode.msg_mode = UMT_MSG0_MSG1;
		/* physical address for HW UMT */
		umt_mode.phy_dst = umt->dst;
		umt_mode.period = umt->period;
		umt_mode.enable = UMT_DISABLE;
		/* virtual address for SW UMT */
		umt_mode.umt_ep_dst = v_dst_addr;
		ret = ltq_umt_set_mode(umt->id, priv->port_id, &umt_mode);
		break;
	}

#endif
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "UMT mode set fail!\n");
		goto err2;
	}

	return 0;
err2:
	ltq_umt_release(umt->id, priv->port_id);
err1:
	return -ENODEV;
}

static void plat_dma_exit(u32 id)
{
	struct plat_dma *dma;

	dma = &g_plat_priv->dma[id];

	ltq_dma_chan_off(dma->dma_chan);
	ltq_dma_chan_reset(dma->dma_chan);
	ltq_dma_chan_desc_free(dma->dma_chan);
	ltq_free_dma(dma->dma_chan);
	memset(dma, 0, sizeof(*dma));
}

static void plat_umt_exit(u32 id)
{
	struct plat_umt *umt;
	struct plat_priv *priv;

	if (WARN_ON(id >= EP_MAX_NUM))
		return;

	priv = g_plat_priv;
	plat_dma_exit(id);
	umt = &priv->umt[id];
	udelay(umt->period * 2);
	ltq_umt_release(umt->id, priv->port_id);
	memset(umt, 0, sizeof(*umt));
}

static int plat_soc_cfg_get(struct device *dev,
	struct soc_cfg *cfg, u32 id, int bonding)
{
	struct cbm_dq_port_res cbm_res;
	struct plat_priv *priv;
	struct tc_priv *tcpriv;
	int ret;

	priv = g_plat_priv;
	tcpriv = plat_to_tcpriv();
	if (!priv || !tcpriv) {
		pr_err("Invalid resources %p %p\n", priv, tcpriv);
		return -ENODEV;
	}

	ret = cbm_dequeue_port_resources_get(priv->port_id, &cbm_res, 0);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "Get CBM DQ port res fail!\n");
		return -ENODEV;
	}
	if (cbm_res.num_deq_ports < id + 1) {
		tc_err(priv->tc_priv, MSG_INIT,
			"CBM port(%d) less than required(%d)\n",
			cbm_res.num_deq_ports, id + 1);
		kfree(cbm_res.deq_info);
		return -ENODEV;
	}
	pr_info("free base: 0x%x, free num: %d, port num: %d\n",
		(u32)cbm_res.cbm_buf_free_base,
		cbm_res.num_free_entries, cbm_res.num_deq_ports);
	pr_info("port_no: %d, deq dbase: 0x%x, dnum: %d, free base: 0x%x, free num: %d\n",
		cbm_res.deq_info[0].port_no,
		(u32)cbm_res.deq_info[0].cbm_dq_port_base,
		cbm_res.deq_info[0].num_desc,
		(u32)cbm_res.cbm_buf_free_base,
		cbm_res.num_free_entries);
	priv->cbm_pid = cbm_res.deq_info[0].port_no;
	cfg->txin_dbase = (u32 __force)cbm_res.deq_info[0].cbm_dq_port_base;
	cfg->txin_dbase = RPHYSADDR(cfg->txin_dbase);
	cfg->txin_dnum = cbm_res.deq_info[0].num_desc;
	cfg->txout_dbase = (u32 __force)cbm_res.cbm_buf_free_base;
	cfg->txout_dbase = RPHYSADDR(cfg->txout_dbase);
	cfg->txout_dnum = cbm_res.num_free_entries;
#ifdef FIX_DMA_BYTE_ALIGNMENT
	if (bonding && dev && (id == 1) && tcpriv->param.ena_dma_tc_sg) {
		dma_addr_t new_rxout_src_dbase_phy;
		dma_addr_t rxout_dst_phy;
		u32 rxout_dst_vir;
		struct dma_tx_desc *tx_desc;
		struct dma_data_ptr_lst *buffer_lst;
		int i;

		rxout_dst_phy = ltq_dma_chan_get_desc_phys_base(
			priv->dma[id].dma_chan);
		pr_info("%s: rxout_dst_phy - %x\n", __func__,rxout_dst_phy);
		rxout_dst_vir = ltq_dma_chan_get_desc_vir_base(
			priv->dma[id].dma_chan);
		pr_info("%s: rxout_dst_vir - %x\n", __func__,rxout_dst_vir);
		plat_ex_mips_tc_rxout_src(
			dev,
			&new_rxout_src_dbase_phy,
			rxout_dst_phy,
			rxout_dst_vir,
			priv->dma[id].ds_dnum,
			&buffer_lst
		);
		/* Init RXOUT DES descriptor */
		tx_desc = (struct dma_tx_desc *)rxout_dst_vir;
		for (i = 0;
				i < priv->dma[id].ds_dnum;
				i++, tx_desc++, buffer_lst++) {
			u32 vir;

            if (buffer_lst == NULL)
                return -EINVAL;
			memset(tx_desc, 0, sizeof(struct dma_tx_desc));
			buffer_lst->vir = (u32)dma_zalloc_coherent(dev,
				DMA_PACKET_SZ,
				&tx_desc->data_pointer,
				GFP_ATOMIC | GFP_DMA
			);
			buffer_lst->phy = tx_desc->data_pointer;
			buffer_lst->len = DMA_PACKET_SZ;
			tx_desc->status.field.own = !DMA_RXOUT_DST_OWN;
			tx_desc->status.field.data_len = DMA_PACKET_SZ;
		}
		cfg->rxout_dbase = new_rxout_src_dbase_phy;
		pr_info("%s: EPID[%d] UMT RXOUT SRC 0x%08x\n", __func__, id, cfg->rxout_dbase);
	} else {
		cfg->rxout_dbase
			= ltq_dma_chan_get_desc_phys_base(priv->dma[id].dma_chan);
	}
#else
	cfg->rxout_dbase
		= ltq_dma_chan_get_desc_phys_base(priv->dma[id].dma_chan);
#endif
	cfg->rxout_dnum = priv->dma[id].ds_dnum;
	cfg->desc_dw_sz = 4;

	tc_info(priv->tc_priv, MSG_INIT,
		"id: %d, txin(0x%x: %d), txout(0x%x: %d), rxin(0x%x: %d), rxout(0x%x: %d), dma ch: %d\n",
		id, cfg->txin_dbase, cfg->txin_dnum,
		cfg->txout_dbase, cfg->txout_dnum,
		cfg->rxin_dbase, cfg->rxout_dnum,
		cfg->rxout_dbase, cfg->rxout_dnum, priv->dma[id].chan);

	kfree(cbm_res.deq_info);
	return 0;
}

static int plat_open(struct net_device *pdev, char *dev_name,
		int *subif, int flag)
{
	struct plat_priv *priv;
	dp_subif_t subif_id;
	int ret;

	if (WARN_ON(pdev == NULL && dev_name == NULL))
		return -ENODEV;

	priv = g_plat_priv;
	subif_id.port_id = priv->port_id;
	subif_id.subif = -1;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
	ret = dp_register_subif(priv->owner, pdev, dev_name, &subif_id, 0);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "Register subif fail!\n");
		return ret;
	}
	*subif = subif_id.subif;
#else
	ret = dp_register_subif_ext(0, priv->owner, pdev, dev_name, &subif_id,
		NULL, 0);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT, "Register subif fail!\n");
		return ret;
	}
	*subif = subif_id.subif;
#endif
	return 0;
}

static void plat_close(struct net_device *pdev, char *dev_name,
		int subif, int flag)
{
	struct plat_priv *priv;
	dp_subif_t subif_id;
	int ret;

	if (WARN_ON(pdev == NULL && dev_name == NULL))
		return;

	//cbm_dp_q_enable(priv->port_id, -1,
	//		priv->cbm_pid, -1, 0, 0, CBM_Q_F_FLUSH);

	priv = g_plat_priv;
	subif_id.port_id = priv->port_id;
	subif_id.subif = subif;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
	ret = dp_register_subif(priv->owner, pdev, dev_name,
		&subif_id, DP_F_DEREGISTER);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT,
			"Unregister pid(%d) subif(%d) fail!\n",
			priv->port_id, subif);
	}
#else
	ret = dp_register_subif_ext(0, priv->owner, pdev, dev_name,
		&subif_id, NULL, DP_F_DEREGISTER);
	if (ret < 0) {
		tc_err(priv->tc_priv, MSG_INIT,
			"Unregister pid(%d) subif(%d) fail!\n",
			priv->port_id, subif);
	}
#endif
	return;
}

static void plat_disable_us(int en)
{
}

static int plat_get_mib(struct net_device *pdev,
			struct rtnl_link_stats64 *stat)
{
#if IS_ENABLED(CONFIG_LTQ_DATAPATH_MIB)
	dp_get_netif_stats(pdev, NULL, stat, 0);
	return 0;
#else
	return -ENOTSUPP;
#endif
}

static void plat_tc_ops_setup(struct tc_priv *priv)
{
	priv->tc_ops.send = plat_send;
	priv->tc_ops.alloc = plat_mem_alloc;
	priv->tc_ops.free = plat_mem_free;
	priv->tc_ops.dev_reg = plat_open;
	priv->tc_ops.dev_unreg = plat_close;
	priv->tc_ops.umt_init = plat_umt_init;
	priv->tc_ops.umt_exit = plat_umt_exit;
	priv->tc_ops.umt_start = plat_umt_start;
	priv->tc_ops.soc_cfg_get = plat_soc_cfg_get;
	priv->tc_ops.disable_us = plat_disable_us;
	priv->tc_ops.get_mib = plat_get_mib;
#ifdef FIX_DMA_BYTE_ALIGNMENT
	priv->tc_ops.soc_fw_init = plat_init_soc_fw;
	priv->tc_ops.soc_fw_deinit = plat_deinit_soc_fw;
#endif
}

static int showtime_enter(const unsigned char idx,
		struct port_cell_info *cell_info, void *data)
{
	struct tc_priv *priv;

	if (WARN_ON(idx >= EP_MAX_NUM || cell_info == NULL))
		return -EINVAL;

	priv = g_plat_priv->tc_priv;
	if (priv->showtime[idx] == 1)
		return 0;

	priv->showtime[idx] = 1;

	spin_lock_bh(&priv->tc_lock);
	if (priv->tc_stat == TC_RUN && priv->tc_ops.showtime_enter != NULL)
		priv->tc_ops.showtime_enter(idx, cell_info, data);
	spin_unlock_bh(&priv->tc_lock);

	return 0;
}

static int showtime_exit(const unsigned char idx)
{
	struct tc_priv *priv;

	if (WARN_ON(idx >= EP_MAX_NUM))
		return -EINVAL;

	priv = g_plat_priv->tc_priv;

	if (!priv->showtime[idx])
		return 0;

	priv->showtime[idx] = 0;

	spin_lock_bh(&priv->tc_lock);
	if (priv->tc_stat == TC_RUN && priv->tc_ops.showtime_exit != NULL)
		priv->tc_ops.showtime_exit(idx);
	spin_unlock_bh(&priv->tc_lock);

	return 0;
}

static int plat_erb_addr_get(const unsigned char idx,
	unsigned int *data_addr, unsigned int *desc_addr)
{
	struct tc_priv *priv;

	if (WARN_ON(idx >= EP_MAX_NUM || !data_addr || !desc_addr))
		return -EINVAL;

	priv = g_plat_priv->tc_priv;

	if (priv->tc_stat == TC_RUN && priv->tc_ops.erb_addr_get != NULL)
		return priv->tc_ops.erb_addr_get(idx, data_addr, desc_addr);
	else {
		/* MEI driver request both values has */
		/* to be set to zero in error condition */
		*data_addr = *desc_addr = 0;
		tc_err(priv, MSG_EVENT,
			"erb_addr_get not supported or timing not correct!, tc stat: %u\n",
			(u32)priv->tc_stat);
		return -1;
	}

	return -1;
}

static void plat_tc_req_workqueue(struct work_struct *work)
{
	struct tc_req *req_work;

	req_work = container_of(work, struct tc_req, work);
	tc_request(req_work->id, req_work->tc_mode);
	pmac_cfg_set(g_plat_priv->port_id, req_work->tc_mode);
}

#ifdef FEATURE_PPE_DYNAMIC_FEQ
static int is_tc_reload(struct tc_priv *tc_priv,
    enum dsl_tc_mode mode,
    const unsigned char id)
{
    struct dc_ep_dev *ep;
    int result = 0;
    u32 dsl_fw_freq_ind;
    tc_dbg(tc_priv, MSG_SWITCH, "id[%d]\n", id);
    if (tc_priv->tc_mode == TC_PTM_SL_MODE ||
    	tc_priv->tc_mode == TC_ATM_SL_MODE) {
		/* read the frequency indication from correct EP */
		ep = &tc_priv->ep_dev[id];
		tc_dbg(tc_priv, MSG_SWITCH, "Base[%x]\n", (u32)ep->membase);
		dsl_fw_freq_ind = readl(ep->membase + (DSL_FW_FREQ_INC));
		tc_dbg(tc_priv, MSG_SWITCH,
			"dsl_fw_freq_ind [%x]\n", dsl_fw_freq_ind);
		if (dsl_fw_freq_ind != tc_priv->dsl_fw_freq_ind)
			result = 1;
    }

    return result;
}
#endif
static int plat_tc_request(const unsigned char id,
		mei_tc_request_type tc_type, int is_bonding)
{
	enum dsl_tc_mode mode;
	struct tc_priv *priv;
	int i;
#ifdef FEATURE_PPE_DYNAMIC_FEQ
	int tc_reload;
#endif

	if (WARN_ON(id >= EP_MAX_NUM))
		return -EINVAL;

	priv = g_plat_priv->tc_priv;
	if (!priv) {
		pr_err("%s-%d: Invalid priv data\n",
			__func__, __LINE__);
		return -EINVAL;
	}

	switch (tc_type) {
	case MEI_TC_REQUEST_OFF:
		mode = TC_NONE_MODE;
		for (i = 0; i < priv->ep_num; i++) {
			if (priv->showtime[i] == 1)
				showtime_exit(i);
		}
		break;
	case MEI_TC_REQUEST_ATM:
		mode = TC_ATM_SL_MODE;
		break;
	case MEI_TC_REQUEST_PTM:
		if (is_bonding)
			mode = TC_PTM_BND_MODE;
		else
			mode = TC_PTM_SL_MODE;
		break;
	default:
		return -EINVAL;
	}

	tc_dbg(priv, MSG_EVENT,
		"%s: dsl id: %d, mode: %d, tc_mode: %d, tc_idx: %d\n",
		__func__, id, mode, priv->tc_mode, priv->tc_idx);
#ifdef FEATURE_PPE_DYNAMIC_FEQ
	tc_reload = is_tc_reload(priv, mode, id);
	if (mode == priv->tc_mode && !tc_reload) {
#else
	if (mode == priv->tc_mode) {
#endif
		if (((((mode == TC_PTM_SL_MODE) || (mode == TC_ATM_SL_MODE))
			&& (id == priv->tc_idx)) || (mode == TC_PTM_BND_MODE))
			&& (priv->tc_ops.framer_request_en != NULL)) {
			if (!priv->tc_ops.framer_request_en(id))
				return 0;
		}
	}

	spin_lock(&priv->tc_lock);
	if (priv->tc_stat != TC_RUN && priv->tc_stat != NO_TC) {
		tc_err(priv, MSG_SWITCH,
			"TC status(%d) not allowed to switch\n", priv->tc_stat);
		spin_unlock(&priv->tc_lock);
		return -1;
	}
	priv->tc_stat = TC_SWITCHING;
	spin_unlock(&priv->tc_lock);

	g_plat_priv->req_work.id = id;
	g_plat_priv->req_work.tc_mode = mode;
	queue_work(system_wq, &g_plat_priv->req_work.work);

	return 0;
}

void simu_tc_request(const u8 id, int type)
{
	switch (type) {
	case TC_NONE_MODE:
		plat_tc_request(id, MEI_TC_REQUEST_OFF, 0);
		break;
	case TC_ATM_SL_MODE:
		plat_tc_request(id, MEI_TC_REQUEST_ATM, 0);
		break;
	case TC_PTM_SL_MODE:
		plat_tc_request(id, MEI_TC_REQUEST_PTM, 0);
		break;
	case TC_PTM_BND_MODE:
		plat_tc_request(id, MEI_TC_REQUEST_PTM, 1);
		break;
	}
}

static inline void plat_dsl_ops_setup(void)
{
	ppa_callback_set(LTQ_MEI_SHOWTIME_ENTER, showtime_enter);
	ppa_callback_set(LTQ_MEI_SHOWTIME_EXIT, showtime_exit);
	ppa_callback_set(LTQ_MEI_TC_REQUEST, plat_tc_request);
	ppa_callback_set(LTQ_MEI_ERB_ADDR_GET, plat_erb_addr_get);
}

static inline void plat_dsl_ops_exit(void)
{
	ppa_callback_set(LTQ_MEI_SHOWTIME_ENTER, NULL);
	ppa_callback_set(LTQ_MEI_SHOWTIME_EXIT, NULL);
	ppa_callback_set(LTQ_MEI_TC_REQUEST, NULL);
	ppa_callback_set(LTQ_MEI_ERB_ADDR_GET, NULL);
}

int platform_init(struct tc_priv *tc_priv, const char *drv_name)
{
	struct plat_priv *priv;
	int ret;

	priv = (struct plat_priv *)tc_plat_priv(tc_priv);
	priv->tc_priv = tc_priv;
	g_plat_priv = priv;

	ret = plat_dp_init(priv, drv_name);
	if (ret < 0)
		goto err1;

	INIT_WORK(&priv->req_work.work, plat_tc_req_workqueue);
	plat_tc_ops_setup(tc_priv);
	plat_dsl_ops_setup();

	return 0;

err1:
	g_plat_priv = NULL;
	return ret;
}

void platform_dsl_exit(void)
{
	plat_dsl_ops_exit();
	cancel_work_sync(&g_plat_priv->req_work.work);
}

void platform_exit(void)
{
	plat_dp_exit(g_plat_priv);
	g_plat_priv = NULL;
}
