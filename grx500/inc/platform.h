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
#include <net/datapath_api.h>
struct plat_dma {
	u32 chan; /* CHAN IID */
	u32 dma_chan; /* CONTROLLER/PORT/CHAN ID */
	u32 ds_dnum; /* DS descriptor number */
};

struct plat_umt {
	u32 id;
	u32 cbm_id;
	u32 period;
	u32 dst;
};

struct tc_req {
	struct work_struct work;
	u32 id;
	enum dsl_tc_mode tc_mode;
};
struct plat_priv {
	struct tc_priv *tc_priv;
	struct module *owner;
	u32 port_id;
	dp_cb_t	cb;
	struct plat_umt umt[EP_MAX_NUM];
	struct plat_dma dma[EP_MAX_NUM];
	struct ltq_mei_atm_showtime_info dsl_ops;
	struct tc_req req_work;
	u32 cbm_pid;
};

extern size_t plat_priv_sz(void);
extern int platform_init(struct tc_priv *, const char *);
extern void platform_exit(void);
extern void simu_tc_request(const u8, int);
extern void platform_dsl_exit(void);
extern struct plat_priv *g_plat_priv;

#ifdef FIX_DMA_BYTE_ALIGNMENT
struct dma_data_ptr_lst {
	u32 vir;
	dma_addr_t phy;
	u32 len;
};
extern int plat_ex_mips_tc_rxout_src(struct device *dev,
	dma_addr_t *new_rxout_phy,
	dma_addr_t rxout_dst_phy,
	u32 rxout_dst_vir,
	int rxout_dnum,
	struct dma_data_ptr_lst **buffer_lst
	);
extern int plat_init_soc_fw(struct device *dev);
extern void plat_deinit_soc_fw(struct device *dev);
#endif /* FIX_DMA_BYTE_ALIGNMENT */
