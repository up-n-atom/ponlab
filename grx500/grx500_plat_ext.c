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
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
#include <lantiq_dma.h>
#else
#include <linux/dma/lantiq_dma.h>
#endif
#include "inc/mips_tc_sg.h"

#include "inc/tc_main.h"
#include "inc/platform.h"
#include "inc/dsl_tc.h"


#ifdef FIX_DMA_BYTE_ALIGNMENT
#define FW_DMA_RX_ID 0
#define FW_DMA_TX_ID 0

/* DMA0 2DW descriptor from arch/mips/lantiq/grx500/ltq_dma.h */
#define DMA_OWN         1
#define CPU_OWN         0
#ifdef CONFIG_CPU_LITTLE_ENDIAN
struct rx_desc {
	union {
		struct {
			volatile unsigned int data_length:16;
			volatile unsigned int reserved16_22:7;
			volatile unsigned int byte_offset:2;
			volatile unsigned int reserve_25_27:3;
			volatile unsigned int eop:1;
			volatile unsigned int sop:1;
			volatile unsigned int C:1;
			volatile unsigned int OWN:1;
		} __packed field;
		volatile unsigned int word;
	} __packed status;
	volatile dma_addr_t data_pointer;
} __packed __aligned(8);

struct tx_desc {
	union {
		struct {
			volatile unsigned int data_length:16;
			volatile unsigned int reserved:7;
			volatile unsigned int byte_offset:5;
			volatile unsigned int eop:1;
			volatile unsigned int sop:1;
			volatile unsigned int C:1;
			volatile unsigned int OWN:1;
		} __packed field;
		volatile unsigned int word;
	} __packed status;
	volatile dma_addr_t data_pointer; /* Descriptor data pointer */
} __packed __aligned(8);
#else
struct rx_desc {
	union {
		struct {
			volatile unsigned int OWN:1;
			volatile unsigned int C:1;
			volatile unsigned int sop:1;
			volatile unsigned int eop:1;
			volatile unsigned int reserve_25_27:3;
			volatile unsigned int byte_offset:2;
			volatile unsigned int reserve16_22:7;
			volatile unsigned int data_length:16;
		} __packed field;
		volatile unsigned int word;
	} __packed status;
	volatile dma_addr_t data_pointer; /* Descriptor data pointer */
} __packed __aligned(8);

struct tx_desc {
	union {
		struct {
			volatile unsigned int OWN:1;
			volatile unsigned int C:1;
			volatile unsigned int sop:1;
			volatile unsigned int eop:1;
			volatile unsigned int byte_offset:5;
			volatile unsigned int reserved:7;
			volatile unsigned int data_length:16;
		} __packed field;
		volatile unsigned int word;
	} __packed status;
	volatile dma_addr_t data_pointer; /* Descriptor data pointer */
} __packed __aligned(8);

#endif /*CONFIG_CPU_LITTLE_ENDIAN  */



static struct mips_tc_sg_mem *shared_memory;
/* stop mips tc */
int mips_tc_stop(struct mips_tc_sg_mem *shared_memory)
{
	int retries = 5;
	if (!shared_memory) {
		pr_err("%s-%d: Invalid shared memory\n", __func__, __LINE__);
		return -1;
	}
	shared_memory->ctrl.mips_tc_control = MIPS_TC_C_STOP;
	while (shared_memory->mib.state != MIPS_TC_S_IDLE && retries) {
		msleep(400);
		retries--;
	}
	if (shared_memory->mib.state != MIPS_TC_S_IDLE) {
		pr_err("%s-%d: cannot stop MIPS TC\n", __func__, __LINE__);
		return -1;
	}
	return 0;
}
static void init_shared_memory(void)
{
	struct tc_priv *tc_priv;
	struct mips_tc_ctrl_dbg *ctrl;
	struct mips_tc_mem_conf *conf;
	struct mips_tc_info *mib;
	struct soc_dma_ch *pchan;
	struct dma_data_ptr_lst *dma1tx_data_ptr;
	struct mips_tc_rxout_dst_cache_ctxt * cache_rxout_ptr;
	int i;

	mips_tc_get_shared_mem(&shared_memory);
	/* get shared memory */
	if (!shared_memory) {
		pr_err("%s:%d: Invalid shared memory\n", __func__, __LINE__);
		return;
	}
	tc_priv = g_plat_priv->tc_priv;
	if (!tc_priv) {
		pr_err("%s:%d: Invalid TC Priv\n", __func__, __LINE__);
		return;
	}
	pchan = &tc_priv->soc_tc_conf.dma_ch_conf;
	tc_priv->soc_tc_conf.shared_mem = (void *)shared_memory;
	/* Memory descriptor list */
	conf = &shared_memory->conf;
	memset((void *)conf, 0, sizeof(struct mips_tc_mem_conf));
	/*   RXOUT SRC */
	conf->soc_rx_out_src_ctxt.des_base_addr	=
		tc_priv->soc_tc_conf.rxout_src_dbase_vir;
	conf->soc_rx_out_src_ctxt.des_num =
			tc_priv->soc_tc_conf.rxout_src_dnum;
	conf->soc_rx_out_src_ctxt.des_in_own_val = DMA_RXOUT_SRC_OWN;
	/*   SG TX */
	conf->soc_sg_dma_tx_ctxt.des_base_addr	=
			tc_priv->soc_tc_conf.tch_dbase_vir;
	conf->soc_sg_dma_tx_ctxt.des_num =
			pchan->tch_dnum;
	conf->soc_sg_dma_tx_ctxt.des_in_own_val = DMA_SG_TX_OWN;

	/*   SG TX */
	conf->soc_sg_dma_rx_ctxt.des_base_addr	=
			tc_priv->soc_tc_conf.rch_dbase_vir;
	conf->soc_sg_dma_rx_ctxt.des_num =
			pchan->rch_dnum;
	conf->soc_sg_dma_rx_ctxt.des_in_own_val = DMA_SG_RX_OWN;
	/*   RXOUT SRC */
	conf->soc_rx_out_dst_ctxt.des_base_addr	=
		tc_priv->soc_tc_conf.rxout_dst_dbase_vir;
	conf->soc_rx_out_dst_ctxt.des_num =
			tc_priv->soc_tc_conf.rxout_dst_dnum;
	conf->soc_rx_out_dst_ctxt.des_in_own_val = DMA_RXOUT_DST_OWN;
	conf->aca_hw_rxin_hd_acc_addr = tc_priv->soc_tc_conf.aca_umt_rxin_hd_base_vir;
	conf->tc_mode_bonding = 1;

	/* Configure DMA1TX data pointers cache context */
	dma1tx_data_ptr = (struct dma_data_ptr_lst *)
		tc_priv->soc_tc_conf.rxout_data_ptr_lst_vir;
	conf->cache_rxout_ptr = tc_priv->soc_tc_conf.cache_rxout_ptr;
	cache_rxout_ptr = conf->cache_rxout_ptr;	
	if (dma1tx_data_ptr && cache_rxout_ptr) {
		for (i = 0;
			i < conf->soc_rx_out_dst_ctxt.des_num;
			i++) {
			cache_rxout_ptr->data_ptr = (u32)dma1tx_data_ptr->phy;
			cache_rxout_ptr++;
			dma1tx_data_ptr++;
		}
	}

	/* Control & Debug configuration */
	ctrl = &shared_memory->ctrl;
	/* reset private data */
	if (ctrl->priv_data_vir && ctrl->priv_data_len)
		memset((void *)ctrl->priv_data_vir, 0, ctrl->priv_data_len);
	else
		pr_err("%s-%d: No private data for MIPS 0x%08x %d\n",
			__func__, __LINE__,
			ctrl->priv_data_vir, ctrl->priv_data_len);
	ctrl->dbg_flags = 1;
	ctrl->mips_tc_control = 1;

}

int plat_ex_mips_tc_rxout_src(struct device *dev,
	dma_addr_t *new_rxout_phy,
	dma_addr_t rxout_dst_phy,
	u32 rxout_dst_vir,
	int rxout_dnum,
	struct dma_data_ptr_lst **buffer_lst
	)
{
	struct tc_priv *tc_priv;
	tc_priv = g_plat_priv->tc_priv;
	if (!tc_priv) {
		pr_err("%s:%d: Invalid TC Priv\n", __func__, __LINE__);
		return -1;
	}
	if (!dev) {
		pr_err("%s:%d: Invalid device\n", __func__, __LINE__);
		return -1;
	}
	/* allocate rxout src descriptor list */
	tc_priv->soc_tc_conf.rxout_src_dnum = rxout_dnum;
	tc_priv->soc_tc_conf.rxout_src_dbase_vir =
		(u32)dma_zalloc_coherent(dev,
			FW_DMA_4DW_DES_LEN * rxout_dnum,
			&tc_priv->soc_tc_conf.rxout_src_dbase_phy,
			GFP_ATOMIC | GFP_DMA
		);
	*new_rxout_phy = tc_priv->soc_tc_conf.rxout_src_dbase_phy;
	/* set upDMA1TX to rxout dst descriptor info */
	tc_priv->soc_tc_conf.rxout_dst_dbase_phy = rxout_dst_phy;
	tc_priv->soc_tc_conf.rxout_dst_dbase_vir = rxout_dst_vir;
	tc_priv->soc_tc_conf.rxout_dst_dnum = rxout_dnum;
	tc_priv->soc_tc_conf.rxout_data_ptr_lst_vir =
		(u32)dma_zalloc_coherent(dev,
					sizeof(struct dma_data_ptr_lst) * rxout_dnum,
					&tc_priv->soc_tc_conf.rxout_data_ptr_lst_phy,
					GFP_ATOMIC
				);
	*buffer_lst = (struct dma_data_ptr_lst *)tc_priv->soc_tc_conf.rxout_data_ptr_lst_vir;
	tc_priv->soc_tc_conf.cache_rxout_ptr_vir =
			(u32)dma_zalloc_coherent(dev,
						sizeof(struct dma_data_ptr_lst) * rxout_dnum,
						&tc_priv->soc_tc_conf.cache_rxout_ptr_phy,
						GFP_ATOMIC
					);
	tc_priv->soc_tc_conf.cache_rxout_ptr =
		(struct mips_tc_rxout_dst_cache_ctxt *)
			__va(tc_priv->soc_tc_conf.cache_rxout_ptr_phy);
	return 0;
}
int plat_init_soc_fw(struct device *dev)
{
	int ret;
	struct tc_priv *tc_priv;
	struct soc_dma_ch *pchan;
	_dma_device_info *dma;
	struct rx_desc *desc;

	tc_priv = g_plat_priv->tc_priv;
	if (!tc_priv) {
		pr_err("%s:%d: Invalid TC Priv\n", __func__, __LINE__);
		return -1;
	}
	if (!dev) {
		pr_err("%s:%d: Invalid device\n", __func__, __LINE__);
		return -1;
	}

	pchan = &tc_priv->soc_tc_conf.dma_ch_conf;

	/* RXOUT DEST */
	tc_info(tc_priv, MSG_INIT, "RXOUT DST: Vir[%p]Phy[0x%08x]\n",
		tc_priv->soc_tc_conf.rxout_dst_dbase_vir,
		tc_priv->soc_tc_conf.rxout_dst_dbase_phy);
	/* SG RX */
	pchan->rch_dnum = FW_DMA0_DES_NUM;
	tc_priv->soc_tc_conf.rch_dbase_vir =
		dma_zalloc_coherent(dev,
			pchan->rch_dnum * FW_DMA_DES_SIZE,
			&tc_priv->soc_tc_conf.rch_dbase_phy,
			GFP_ATOMIC | GFP_DMA);
	tc_info(tc_priv, MSG_INIT,
		"DMA0 RX: Vir[%p]Phy[0x%08x]\n",
		tc_priv->soc_tc_conf.rch_dbase_vir,
		tc_priv->soc_tc_conf.rch_dbase_phy
	);
	/* SG TX */
	pchan->tch_dnum = FW_DMA0_DES_NUM;
	tc_priv->soc_tc_conf.tch_dbase_vir =
		dma_zalloc_coherent(dev,
			pchan->tch_dnum * FW_DMA_DES_SIZE,
			&tc_priv->soc_tc_conf.tch_dbase_phy,
			GFP_ATOMIC | GFP_DMA);
	tc_info(tc_priv, MSG_INIT,
		"DMA0 TX: Vir[%p]Phy[0x%08x]\n",
		tc_priv->soc_tc_conf.tch_dbase_vir,
		tc_priv->soc_tc_conf.tch_dbase_phy
	);
	pchan->rch_cid = FW_DMA_RX_ID;
	pchan->tch_cid = FW_DMA_TX_ID;
	pchan->rch_dbase = tc_priv->soc_tc_conf.rch_dbase_vir;
	pchan->tch_dbase = tc_priv->soc_tc_conf.tch_dbase_vir;
	pchan->onoff = 1;
	sprintf(pchan->rch_name, "MCTRL");
	sprintf(pchan->tch_name, "MCTRL");

	dma = dma_device_reserve(pchan->rch_name);
	if (!dma) {
		pr_err("%s-%d:Reserve DMA for [%s] failed\n",
			__func__, __LINE__, pchan->rch_name);
		ret = -1;
		goto __FW_TC_DMA_REG_FAIL;
	}
	tc_info(tc_priv, MSG_INIT,
		"Reserved: %p, rch_name:%s RXMAX-%d TXMAX-%d\n",
		dma, dma->device_name,
		dma->max_rx_chan_num,
		dma->max_tx_chan_num
	);
	tc_priv->soc_tc_conf.dma_device = (void *)dma;
	/* configure Legacy DMA0 RX */
	if (pchan->rch_cid < dma->max_rx_chan_num) {
		dma->rx_chan[pchan->rch_cid]->desc_base =
			tc_priv->soc_tc_conf.rch_dbase_vir;
		dma->rx_chan[pchan->rch_cid]->desc_phys =
			tc_priv->soc_tc_conf.rch_dbase_phy;
		dma->rx_chan[pchan->rch_cid]->desc_len  =
			pchan->rch_dnum;
		dma->rx_chan[pchan->rch_cid]->packet_size =
			DMA_PACKET_SZ; //without this, on will crash
		dma->rx_chan[pchan->rch_cid]->control =
			IFX_DMA_CH_ON;

		tc_info(tc_priv, MSG_INIT,
			"Set up RX %p-%p-%d\n",
			dma->rx_chan[pchan->rch_cid]->desc_base,
			dma->rx_chan[pchan->rch_cid]->desc_phys,
			dma->rx_chan[pchan->rch_cid]->desc_len
		);
	}
	/* configure Legacy DMA0 TX */
	if (pchan->tch_cid < dma->max_tx_chan_num) {
		dma->tx_chan[pchan->tch_cid]->desc_base =
			tc_priv->soc_tc_conf.tch_dbase_vir;
		dma->tx_chan[pchan->tch_cid]->desc_phys =
			tc_priv->soc_tc_conf.tch_dbase_phy;
		dma->tx_chan[pchan->tch_cid]->desc_len =
			pchan->tch_dnum;
		dma->tx_chan[pchan->tch_cid]->packet_size =
			DMA_PACKET_SZ;
		dma->tx_chan[pchan->tch_cid]->control =
				IFX_DMA_CH_ON;

		tc_info(tc_priv, MSG_INIT,
			"Set up TX %p-%p-%d\n",
			dma->tx_chan[pchan->tch_cid]->desc_base,
			dma->tx_chan[pchan->tch_cid]->desc_phys,
			dma->tx_chan[pchan->tch_cid]->desc_len
		);
	}
	if (dma_device_register(dma)) {
		pr_err("%s-%d:failed in \"dma_device_register\"",
			__func__, __LINE__);
		ret = -1;
		goto __FW_TC_DMA_REG_FAIL;
	} else {
		int i;
		desc = (struct rx_desc *)dma->rx_chan[pchan->rch_cid]->desc_base;
		for (i = 0; i < dma->rx_chan[pchan->rch_cid]->desc_len; i++, desc++) {
			/* printk("Free %d: 0x%08x-%p-0x%08x\n",
				i, desc->data_pointer,
				(u8 *)__va(desc->data_pointer),
				dma->rx_chan[pchan->rch_cid]->opt[i]); */
			if (desc->data_pointer != 0) {
				dma_unmap_single(dev,
					desc->data_pointer,
					desc->status.field.data_length,
					DMA_FROM_DEVICE
				);
				dma->buffer_free(
					(u8 *)__va(desc->data_pointer),
					desc->data_pointer);
				desc->data_pointer = 0;
				desc->status.field.OWN = CPU_OWN;
			}
		}

		desc = (struct rx_desc *)dma->rx_chan[pchan->rch_cid]->desc_base;
		desc->data_pointer = *((u32 *)tc_priv->soc_tc_conf.rxout_dst_dbase_vir + 3);
		dma->rx_chan[pchan->rch_cid]->dir = 1;  //  IFX_DMA_TX_CH
		dma->rx_chan[pchan->rch_cid]->open(dma->rx_chan[pchan->rch_cid]);
		dma->rx_chan[pchan->rch_cid]->dir = 0;  //  IFX_DMA_RX_CH
		dma->tx_chan[pchan->tch_cid]->reset(dma->tx_chan[pchan->tch_cid]);
		dma->tx_chan[pchan->tch_cid]->open(dma->tx_chan[pchan->tch_cid]);
		/* disable IRQ */
		dma->rx_chan[pchan->rch_cid]->disable_irq(dma->rx_chan[pchan->rch_cid]);
		dma->tx_chan[pchan->tch_cid]->disable_irq(dma->tx_chan[pchan->tch_cid]);

	}
	init_shared_memory();
	tc_info(tc_priv, MSG_INIT, "Register: %p, shared_mem:%p\n",
			dma, tc_priv->soc_tc_conf.shared_mem);

	return 0;
__FW_TC_DMA_REG_FAIL:
	if (dma)
		dma_device_release(dma);
	dma = NULL;


	return ret;
}

void plat_deinit_soc_fw(struct device *dev)
{
	struct tc_priv *tc_priv;
	struct soc_dma_ch *pchan;
	struct mips_tc_sg_mem *shared_mem;
	_dma_device_info *dma;
	struct rx_desc *desc;
	struct dma_data_ptr_lst *buffer_lst;
	int i;

	tc_priv = g_plat_priv->tc_priv;

	if (!tc_priv) {
		pr_err("%s:%d: Invalid TC Priv\n", __func__, __LINE__);
		return;
	}
	if (!dev) {
		pr_err("%s:%d: Invalid device\n", __func__, __LINE__);
		return;
	}
	pchan = &tc_priv->soc_tc_conf.dma_ch_conf;
	/* Stop the MIPS TC */
	shared_mem = (struct mips_tc_sg_mem *)tc_priv->soc_tc_conf.shared_mem;

	if (mips_tc_stop(shared_mem))
		return;
	dma = (_dma_device_info *)tc_priv->soc_tc_conf.dma_device;
	if (!dma) {
		pr_err("%s-%d: DMA device is INVALID\n", __func__, __LINE__);
		return;
	}
	/* Close DMA0 TX and RX channel */
	dma->rx_chan[pchan->rch_cid]->close(dma->rx_chan[pchan->rch_cid]);
	dma->tx_chan[pchan->tch_cid]->close(dma->tx_chan[pchan->tch_cid]);
	desc = (struct rx_desc *)dma->rx_chan[pchan->rch_cid]->desc_base;
	/* Don't let dma driver free RX buffer */
	for (i = 0; i < pchan->rch_dnum; i++, desc++) {
		desc->status.field.OWN = CPU_OWN;
		desc->status.field.C = 0;
	}
	/* Don't let dma driver free TX buffer */
	dma->tx_chan[pchan->tch_cid]->peri_to_peri = 1;
	dma_device_unregister(dma);
	dma_device_release(dma);
	/* free all memory */
	/* RXOUT SRC descriptor list */
	dma_free_coherent(dev, FW_DMA_4DW_DES_LEN * tc_priv->soc_tc_conf.rxout_src_dnum,
		(void *)tc_priv->soc_tc_conf.rxout_src_dbase_vir,
		tc_priv->soc_tc_conf.rxout_src_dbase_phy);

	/* SG RX descriptor list */
	dma_free_coherent(dev, pchan->rch_dnum * FW_DMA_DES_SIZE,
		(void *)tc_priv->soc_tc_conf.rch_dbase_vir,
		tc_priv->soc_tc_conf.rch_dbase_phy);
	/* SG TX descriptor list */
	dma_free_coherent(dev, pchan->tch_dnum * FW_DMA_DES_SIZE,
		(void *)tc_priv->soc_tc_conf.tch_dbase_vir,
		tc_priv->soc_tc_conf.tch_dbase_phy);
	buffer_lst = (struct dma_data_ptr_lst *)tc_priv->soc_tc_conf.rxout_data_ptr_lst_vir;
	/* DMA1TX pointer address */
	for (i = 0; i < tc_priv->soc_tc_conf.rxout_src_dnum; i++, buffer_lst++) {
		if (buffer_lst->vir) {
			//dma_unmap_single(dev, buffer_lst->phy,
				//buffer_lst->len, DMA_FROM_DEVICE);
			dma_free_coherent(dev, buffer_lst->len,
					(void *)buffer_lst->vir,
					buffer_lst->phy);
		}
	}
	/* tc_priv->soc_tc_conf.rxout_data_ptr_lst_vir */
	dma_free_coherent(dev,
		tc_priv->soc_tc_conf.rxout_dst_dnum * sizeof(struct dma_data_ptr_lst),
		(void *)tc_priv->soc_tc_conf.rxout_data_ptr_lst_vir,
		tc_priv->soc_tc_conf.rxout_data_ptr_lst_phy);
	/* tc_priv->soc_tc_conf.cache_rxout_ptr_vir */
	dma_free_coherent(dev,
		tc_priv->soc_tc_conf.rxout_dst_dnum * sizeof(struct dma_data_ptr_lst),
		(void *)tc_priv->soc_tc_conf.cache_rxout_ptr_vir,
		tc_priv->soc_tc_conf.cache_rxout_ptr_phy);
}
#endif



