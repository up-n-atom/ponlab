// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2016-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dma/intel_mcpy.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/skbuff.h>

/* MCPY register */
#define MCPY_CMD_ADDR(x, i)	\
	(((i) < 4) ? ((x) * 0x1000 + (i) * 4) : 0x400 + (x) * 0x1000)
#define MCPY_GCTRL		0x200
#define MCR3_ADDR_SRC(i)	(0x230 + 0x1000 * (i))
#define MCR3_ADDR_DST(i)	(0x240 + 0x1000 * (i))
#define MAT_CTRL(i)		(0x244 + 0x1000 * (i))
#define MCPY_INT_MASK		0x8000
#define MCPY_INTL_INT_MASK	0x8004
#define MCPY_INT_EN		0x8008
#define MCPY_INTL_INT_EN	0x800c
#define MCPY_INT_STAT		0x8010
#define MCPY_RSP_ADDR(x)	(0x104 + (x) * 0x1000)
#define MCPY_AT_SRC(x, i)	((x) * 0x1000 + 0x420 + (i) * 4)

/* MPCY CTRL BITS */
#define MCPY_EN			0
#define MCPY_RST		4
#define MCPY_PORT_EN		8
#define MCPY_DONE		31
#define MCPY_ERR		30

#define MCPY_MAX_LEN		BIT(20)

struct intel_mcpy_priv;

struct intel_mcpy_port {
	struct intel_mcpy_priv *parent;
	char name[32];
	int pid;
	int irq_id;
	int in_use;
	struct dma_chan *rxch;
	struct dma_chan *txch;
	struct completion cmp;
};

struct intel_mcpy_priv {
	struct platform_device *pdev;
	struct device *dev;
	u32 port_num;
	void __iomem *membase;
	unsigned long ports_inuse;
	struct clk *clk;
	u8 def_chunk_sz;
	u8 max_chunk_sz;
	struct intel_mcpy_port *ports;
	spinlock_t lock; /* lock for mcpy */
	u32 chk_comp[MCPY_PORT_CNT];
};

static struct intel_mcpy_priv *mcpy_priv;
static const char mcpy_name[] = "mcpy driver";

static void
mcpy_update_bits(struct intel_mcpy_priv *priv, u32 reg,
		 u32 set, u32 width, u8 off)
{
	u32 mask, val, reg_val;

	mask = BIT(width) - 1;
	val = (set & mask) << off;
	mask = ~(mask << off);

	reg_val = readl(priv->membase + reg);
	reg_val = (reg_val & mask) | val;
	writel(reg_val, priv->membase + reg);
}

static inline struct intel_mcpy_port *
mcpy_get_port(struct intel_mcpy_priv *priv, u32 pid)
{
	return &priv->ports[pid];
}

static int
mcpy_dt_probe(struct intel_mcpy_priv *priv)
{
	struct device *dev = priv->dev;
	struct resource *res;

	res = platform_get_resource(priv->pdev, IORESOURCE_MEM, 0);
	priv->membase = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->membase)) {
		dev_err(dev, "ioremap mem resource failed!\n");
		return PTR_ERR(priv->membase);
	}

	if (device_property_read_u32(dev, "intel,mcpy-portnum",
				     &priv->port_num)) {
		dev_err(dev, "Get mcpy port num failed!\n");
		return -EINVAL;
	}

	if (device_property_read_u8(dev, "intel,mcpy-maxchksz",
				    &priv->max_chunk_sz))
		priv->max_chunk_sz = MCPY_CHUNK_SZ_MASK;

	if (device_property_read_u8(dev, "intel,mcpy-defchksz",
				    &priv->def_chunk_sz))
		priv->def_chunk_sz = MCPY_CHUNK_2K;

	if (priv->def_chunk_sz > priv->max_chunk_sz)
		priv->def_chunk_sz = MCPY_CHUNK_2K;

	dev_dbg(dev, "mcpy port num: %u\n", priv->port_num);
	dev_dbg(dev, "mcpy max chunk size: %u, def chunk size: %u\n",
		priv->max_chunk_sz, priv->def_chunk_sz);

	priv->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(priv->clk))
		priv->clk = NULL;

	return 0;
}

static int mcpy_dma_init(struct intel_mcpy_priv *priv)
{
	int i;
	struct intel_mcpy_port *port;
	char rxch_name[32], txch_name[32];

	for (i = 0; i < priv->port_num; i++) {
		sprintf(rxch_name, "p%drx", i);
		sprintf(txch_name, "p%dtx", i);

		port = &priv->ports[i];
		port->rxch = dma_request_slave_channel(priv->dev, rxch_name);
		port->txch = dma_request_slave_channel(priv->dev, txch_name);
		if (!port->rxch || !port->txch) {
			dev_err(priv->dev,
				"request dma channel failed! rx: %s, tx: %s\n",
				rxch_name, txch_name);
			return -ENODEV;
		}
	}

	return 0;
}

static void hw_mcpy_reset(struct intel_mcpy_priv *priv)
{
	mcpy_update_bits(priv, MCPY_GCTRL, 1, 1, MCPY_RST);
	udelay(1);
	mcpy_update_bits(priv, MCPY_GCTRL, 0, 1, MCPY_RST);
}

void hw_mcpy_cr3_init(struct intel_mcpy_priv *priv, unsigned int pid)
{
	unsigned long cr3_reg = __native_read_cr3();
	u32 val, cr3_h;
	unsigned long cr4_reg;

	dev_dbg(priv->dev, "cr3: 0x%lx\n", cr3_reg);

	val = cr3_reg;
	writel(val, priv->membase + MCR3_ADDR_SRC(pid));
	writel(val, priv->membase + MCR3_ADDR_DST(pid));

	cr3_h = (cr3_reg >> 32) & 0xff;
	val = cr3_h | (cr3_h << 8);
	if (BITS_PER_LONG > 32)
		val |= BIT(30);
	val |= BIT(31);  /* support 2M */
	writel(val, priv->membase + MAT_CTRL(pid));

	dev_dbg(priv->dev, "cr3 src:0x%x cr3 dst: 0x%x, mtl: 0x%x\n",
		readl(priv->membase + MCR3_ADDR_SRC(pid)),
		readl(priv->membase + MCR3_ADDR_DST(pid)),
		readl(priv->membase + MAT_CTRL(pid)));

	cr4_reg = __read_cr4();
	dev_dbg(priv->dev, "cr4: 0x%lx\n", cr4_reg);
}

static int mcpy_ctrl_init(struct intel_mcpy_priv *priv)
{
	u32 bitmap;

	if (priv->clk)
		clk_prepare_enable(priv->clk);

	bitmap = BIT(priv->port_num) - 1;
	hw_mcpy_reset(priv);
	/* enable port bitmap */
	mcpy_update_bits(priv, MCPY_GCTRL, bitmap,
			 priv->port_num, MCPY_PORT_EN);
	/* enable interrupt bitmap */
	writel(0, priv->membase + MCPY_INTL_INT_MASK);
	writel(0, priv->membase + MCPY_INT_MASK);
	/* interrupt including yield so num * 2 */
	bitmap = BIT(priv->port_num * 2) - 1;
	writel(bitmap, priv->membase + MCPY_INTL_INT_EN);
	writel(bitmap, priv->membase + MCPY_INT_EN);

	/* enable mcpy */
	mcpy_update_bits(priv, MCPY_GCTRL, 1, 1, MCPY_EN);

	spin_lock_init(&priv->lock);

	return 0;
}

int mcpy_get_pid(void)
{
	unsigned int pid;
	struct intel_mcpy_port *port;
	struct intel_mcpy_priv *priv = mcpy_priv;

	if (!priv) {
		pr_err("mcpy is not initialized\n");
		return -ENODEV;
	}

	/* Quick check */
	if (priv->ports_inuse == BIT(priv->port_num) - 1)
		return -ENOSPC;

	spin_lock(&priv->lock);
	for (pid = 0; pid < priv->port_num; pid++)
		if (!(BIT(pid) & priv->ports_inuse))
			break;

	if (unlikely(pid >= priv->port_num)) {
		spin_unlock(&priv->lock);
		return -ENOSPC;
	}

	dev_dbg(priv->dev, "found pid: %u\n", pid);
	port = mcpy_get_port(priv, pid);
	port->in_use = 1;
	priv->ports_inuse |= BIT(pid);
	priv->chk_comp[pid] = 0;
	spin_unlock(&priv->lock);

	dmaengine_resume(port->rxch);
	dmaengine_resume(port->txch);

	return pid;
}
EXPORT_SYMBOL(mcpy_get_pid);

void mcpy_free_pid(u32 pid)
{
	struct intel_mcpy_port *port;
	struct intel_mcpy_priv *priv = mcpy_priv;

	if (!priv) {
		pr_err("mcpy is not initialized\n");
		return;
	}

	spin_lock(&priv->lock);
	port = mcpy_get_port(priv, pid);
	port->in_use = 0;
	clear_bit(pid, &priv->ports_inuse);
	spin_unlock(&priv->lock);

	dmaengine_pause(port->rxch);
	dmaengine_pause(port->txch);
}
EXPORT_SYMBOL(mcpy_free_pid);

static inline u32
setup_cmd0(int sioc, int dioc, int ie, int rxirq,
	   int g, int last, int chk_sz, int len)
{
	unsigned int sphy, dphy;

	WARN_ON(len >= 0x100000L);

	sphy = !sioc;
	dphy = !dioc;

	return (sphy << 31) | (dphy << 30) | (ie << 29) |
	       (rxirq << 26) | (g << 25) | (last << 24) |
	       (chk_sz << 20) | len;
}

static u32 setup_cmd1(dma_addr_t src)
{
	return (u32)src;
}

static u32 setup_cmd2(dma_addr_t dst)
{
	return (u32)dst;
}

static u32
setup_cmd3(int dioc, int sioc, int at, dma_addr_t src, int ctx)
{
	return  (dioc << 30) | (sioc << 29) | (at << 25) |
		(((src >> 32) & 0xffff) << 9) | (ctx & 0x1ff);
}

static u32 setup_cmd4(dma_addr_t dst)
{
	return (dst >> 32) & 0xffff;
}

static void
hw_xmcpy_cmd(struct intel_mcpy_priv *priv, unsigned int pid, int g, int last,
	     u32 size, dma_addr_t src, dma_addr_t dst,
	     int sioc, int dioc, int chk_sz, int ie, int at)
{
	u32 cmd0, cmd1, cmd2, cmd3, cmd4;

	if (g && !last)
		ie = 0;

	cmd0 = setup_cmd0(sioc, dioc, ie, pid, g, last, chk_sz, size);
	cmd1 = setup_cmd1(src);
	cmd2 = setup_cmd2(dst);
	cmd3 = setup_cmd3(dioc, sioc, at, src, 0);
	cmd4 = setup_cmd4(dst);

	dev_dbg(priv->dev, "g: %d, last: %d, size: %u, src: %pad, dst: %pad\n",
		g, last, size, &src, &dst);
	dev_dbg(priv->dev,
		"cmd0: 0x%x, cmd1: 0x%x, cmd2: 0x%x, cmd3:0x%x, cmd4: 0x%x\n",
		cmd0, cmd1, cmd2, cmd3, cmd4);

	/*check own bit */
	WARN_ON_ONCE(!(readl(priv->membase + MCPY_CMD_ADDR(pid, 3)) & BIT(31)));

	/* cmd3 must be the last to write */
	writel(cmd4, priv->membase + MCPY_CMD_ADDR(pid, 4));
	writel(cmd0, priv->membase + MCPY_CMD_ADDR(pid, 0));
	writel(cmd1, priv->membase + MCPY_CMD_ADDR(pid, 1));
	writel(cmd2, priv->membase + MCPY_CMD_ADDR(pid, 2));
	wmb(); /* must be done before cmd3 */
	writel(cmd3, priv->membase + MCPY_CMD_ADDR(pid, 3));
}

static void
hw_mcpy_parm(struct intel_mcpy_priv *priv, int *dioc, int *sioc,
	     int *chunk_sz, int *ie, int *algn, int *wait_bef_next_copy,
	     unsigned long flag)
{
	int sz;

	sz = priv->def_chunk_sz;
	if (flag & MCPY_CHUNK_SIZE)
		sz = flag & MCPY_CHUNK_SZ_MASK;
	if (sz > priv->max_chunk_sz)
		sz = priv->def_chunk_sz;

	*chunk_sz = sz;
	if (flag & MCPY_DST_PADDR)
		*dioc = 0;
	else
		*dioc = 1;

	if (flag & MCPY_SRC_PADDR)
		*sioc = 0;
	else
		*sioc = 1;

	if (flag & MCPY_INT_ENABLE)
		*ie = 1;
	else
		*ie = 0;

	if (algn) {
		if (flag & MCPY_ADDR_ALGN)
			*algn = 1;
		else
			*algn = 0;
	}

	if (flag & MCPY_WAIT_BEF_NEXT_COPY)
		*wait_bef_next_copy = 1;
	else
		*wait_bef_next_copy = 0;
}

static void mcpy_prep_cpl(struct intel_mcpy_priv *priv, int pid, int ie)
{
	if (!ie)
		return;

	init_completion(&priv->ports[pid].cmp);
}

static void mcpy_wait_cpl(struct intel_mcpy_priv *priv, int pid, int ie)
{
	if (ie) {
		wait_for_completion(&priv->ports[pid].cmp);
		return;
	}

	while (!(readl(priv->membase + MCPY_RSP_ADDR(pid)) & BIT(MCPY_DONE)))
		cpu_relax();
	writel(0, priv->membase + MCPY_RSP_ADDR(pid));
}

/* align needed criteria:
 *   Both dst and src address are 64B unaligned.
 *   copy length larger than 2048
 */
int mcpy_align_check(void *dst_addr, void *src_addr, unsigned int len)
{
	dma_addr_t dst, src;

	if (len <= 2048)
		return 0;

	dst = (dma_addr_t)dst_addr;
	src = (dma_addr_t)src_addr;

	if ((dst & 0x3f) && (src & 0x3f))
		return 1;

	return 0;
}

int intel_hw_mcpy(void *dst, void *src, unsigned int pid,
		  unsigned int len, unsigned long flag)
{
	struct intel_mcpy_priv *priv = mcpy_priv;
	int dioc, sioc, chk_sz, ie, algn, g, last, wait_bef_next_copy;
	dma_addr_t algn_src;
	unsigned int algn_len;

	if (!priv || !dst || !src || len >= MCPY_MAX_LEN)
		return -EINVAL;

	dev_dbg(priv->dev,
		"dst: %p, src: %p, pid:%u, len: %u, flag: 0x%lx\n",
		dst, src, pid, len, flag);

	if (virt_addr_valid(dst) || virt_addr_valid(src)) {
		dev_err(priv->dev, "virtual address is not supported\n");
		return -EINVAL;
	}

	pm_runtime_get_sync(priv->dev);

	hw_mcpy_parm(priv, &dioc, &sioc, &chk_sz, &ie, &algn, &wait_bef_next_copy, flag);
	dev_dbg(priv->dev,
		"Get dioc: %d, sioc: %d, chk_sz:%d, ie: %d, algn: %d wbnc: %d chk_comp: %u!\n",
		dioc, sioc, chk_sz, ie, algn, wait_bef_next_copy, priv->chk_comp[pid]);

	if (dioc || sioc)
		hw_mcpy_cr3_init(priv, pid);

	if (wait_bef_next_copy && priv->chk_comp[pid])
		mcpy_wait_cpl(priv, pid, ie);

	mcpy_prep_cpl(priv, pid, ie);

	if (algn && mcpy_align_check(dst, src, len)) {
		/* make src address 64B alignment */
		g = 1;
		algn_src = (((dma_addr_t)src + 63) >> 6) << 6;
		algn_len = algn_src - (dma_addr_t)src;
		hw_xmcpy_cmd(priv, pid, g, 0, algn_len,
			     (dma_addr_t)src, (dma_addr_t)dst,
			     sioc, dioc, chk_sz, ie, 0);
		last = 1;
		len -= algn_len;
		src  = (void *)algn_src;
	} else {
		last = 0;
		g = 0;
	}

	hw_xmcpy_cmd(priv, pid, g, last, len,
		     (dma_addr_t)src, (dma_addr_t)dst,
		     sioc, dioc, chk_sz, ie, 0);
	priv->chk_comp[pid] = priv->chk_comp[pid] ||
			      !(readl(priv->membase + MCPY_RSP_ADDR(pid)) & BIT(MCPY_ERR));

	if (!wait_bef_next_copy)
		mcpy_wait_cpl(priv, pid, ie);

	pm_runtime_mark_last_busy(priv->dev);
	pm_runtime_put_autosuspend(priv->dev);

	return 0;
}
EXPORT_SYMBOL(intel_hw_mcpy);

/**
 * HW garantee no more interrupt for the same port.
 * This function can be re-entry.
 */
static irqreturn_t mcpy_irq_handler(int irq, void *data)
{
	struct intel_mcpy_port *port = data;
	struct intel_mcpy_priv *priv = port->parent;
	u8 id;
	u32 val, mask;

	id = port->pid;

	/* clear done bit and HW will continue issue next cmd */
	val = readl(priv->membase + MCPY_RSP_ADDR(id));
	if (!(val & BIT(MCPY_DONE)) || (val & BIT(MCPY_ERR)))
		dev_err(priv->dev, "mcpy response err: 0x%x\n", val);

	writel(0, priv->membase + MCPY_RSP_ADDR(id));
	val = readl(priv->membase + MCPY_RSP_ADDR(id));

	spin_lock(&priv->lock);
	/* Disable interrupt */
	mask = 3 << (id << 1);
	writel(mask, priv->membase + MCPY_INT_MASK);

	/* Clear interrupt */
	val = readl(priv->membase + MCPY_INT_STAT);
	val = val & (3 << (id << 1));
	writel(val, priv->membase + MCPY_INT_STAT);

	/* Enable interrupt */
	writel(0, priv->membase + MCPY_INT_MASK);
	spin_unlock(&priv->lock);

	complete(&port->cmp);

	return IRQ_HANDLED;
}

static int mcpy_port_init(struct intel_mcpy_priv *priv)
{
	int i, size;
	struct intel_mcpy_port *port;

	size = sizeof(*priv->ports) * priv->port_num;
	priv->ports = devm_kzalloc(priv->dev, size, GFP_KERNEL);
	if (!priv->ports)
		return -ENOMEM;

	i = platform_irq_count(priv->pdev);
	if (i < priv->port_num) {
		dev_err(priv->dev, "irq num: %d less than port num %d\n",
			i, priv->port_num);
		return -EINVAL;
	}

	for (i = 0; i < priv->port_num; i++) {
		port = &priv->ports[i];
		port->parent = priv;
		port->pid = i;
		sprintf(port->name, "mcpy port%u", i);
		port->irq_id = platform_get_irq(priv->pdev, port->pid);
		dev_dbg(priv->dev, "port: %u, irq: %d\n",
			port->pid, port->irq_id);

		if (devm_request_irq(priv->dev, port->irq_id,
				     mcpy_irq_handler, 0,
				     port->name, port))
			return -ENODEV;
	}

	return 0;
}

static int intel_mcpy_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct intel_mcpy_priv *priv;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->pdev = pdev;
	priv->dev = dev;
	if (mcpy_dt_probe(priv))
		return -EINVAL;

	if (mcpy_ctrl_init(priv) < 0) {
		dev_err(dev, "mcpy ctrl init failed!\n");
		return -EINVAL;
	}
	if (mcpy_port_init(priv) < 0) {
		dev_err(dev, "mcpy port init failed!\n");
		return -EINVAL;
	}
	if (mcpy_dma_init(priv) < 0) {
		dev_err(dev, "mcpy dma init failed!\n");
		return -EINVAL;
	}

	platform_set_drvdata(pdev, priv);
	mcpy_priv = priv;

	pm_runtime_get_noresume(dev);
	pm_runtime_set_active(dev);
	pm_runtime_irq_safe(dev);
	pm_runtime_set_autosuspend_delay(dev, 2000);
	pm_runtime_use_autosuspend(dev);
	pm_runtime_enable(dev);
	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);

	dev_info(dev, "mcpy driver init done!\n");

	return 0;
}

static int __maybe_unused mcpy_runtime_suspend(struct device *dev)
{
	return 0;
}

static int __maybe_unused mcpy_runtime_resume(struct device *dev)
{
	return 0;
}

static const struct dev_pm_ops mcpy_pm_ops = {
	SET_RUNTIME_PM_OPS(mcpy_runtime_suspend, mcpy_runtime_resume, NULL)
};

static const struct of_device_id intel_mcpy_dt_match[] = {
	{ .compatible = "intel,lgm-mcpy" },
	{},
};

static struct platform_driver intel_mcpy_driver = {
	.probe	= intel_mcpy_probe,
	.driver	= {
		.name	= "lgm-mcpy",
		.of_match_table = intel_mcpy_dt_match,
		.pm = &mcpy_pm_ops,
	},
};

static int __init intel_mcpy_drv_register(void)
{
	return platform_driver_register(&intel_mcpy_driver);
}
device_initcall(intel_mcpy_drv_register);
