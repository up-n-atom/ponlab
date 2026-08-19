// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
 * Zhu Yixin <yzhu@maxlinear.com>
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

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <uapi/linux/icc/drv_icc.h>

#include "internal.h"

struct regmap_icc_context {
	void __iomem *regs;
	phys_addr_t  paddr;
	unsigned int val_bytes;
	bool         attached_clk;
	const char   *name;
	struct clk   *clk;
	int (*reg_write)(struct regmap_icc_context *ctx,
			 unsigned int reg, unsigned int val);
	int (*reg_read)(struct regmap_icc_context *ctx,
			unsigned int reg, unsigned int *val);
	int (*reg_update_bits)(struct regmap_icc_context *ctx,
			       unsigned int reg, unsigned int mask,
			       unsigned int val);
};

static struct dentry *regmap_debugfs;

static int regmap_icc_regbits_check(size_t reg_bits)
{
	switch (reg_bits) {
	case 32:
		return 0;
	case 8:
	case 16:
	case 64:
	default:
		pr_err("regbits %zu not supported by regmap icc\n", reg_bits);
		return -EINVAL;
	}
}

static int regmap_icc_get_min_stride(size_t val_bits)
{
	int min_stride;

	switch (val_bits) {
	case 32:
		min_stride = 4;
		break;
	case 8:
	case 16:
	case 64:
	default:
		return -EINVAL;
	}

	return min_stride;
}

static int
regmap_icc_write32le(struct regmap_icc_context *ctx, unsigned int reg,
		     unsigned int val)
{
	pr_debug("ICC write: addr: 0x%llx, val: 0x%x",
		 ctx->paddr + reg, val);
	return icc_regmap_sync_write(ctx->paddr + reg, val);
}

static int regmap_icc_write(void *context, unsigned int reg, unsigned int val)
{
	struct regmap_icc_context *ctx = context;
	int ret;

	if (!IS_ERR(ctx->clk)) {
		ret = clk_enable(ctx->clk);
		if (ret < 0)
			return ret;
	}

	ret = ctx->reg_write(ctx, reg, val);
	if (ret)
		pr_err("icc write err %d: addr: 0x%llx, val: 0x%x\n",
		       ret, ctx->paddr + reg, val);

	if (!IS_ERR(ctx->clk))
		clk_disable(ctx->clk);

	return ret;
}

static int
regmap_icc_read32le(struct regmap_icc_context *ctx, unsigned int reg,
		    unsigned int *val)
{
	pr_debug("ICC read: addr: 0x%llx\n", ctx->paddr + reg);

	return icc_regmap_sync_read(ctx->paddr + reg, val);
}

static int regmap_icc_read(void *context, unsigned int reg, unsigned int *val)
{
	struct regmap_icc_context *ctx = context;
	int ret;

	if (!IS_ERR(ctx->clk)) {
		ret = clk_enable(ctx->clk);
		if (ret < 0)
			return ret;
	}

	ret = ctx->reg_read(ctx, reg, val);
	if (ret)
		pr_err("icc read err %d: addr: 0x%llx\n", ret, ctx->paddr + reg);

	if (!IS_ERR(ctx->clk))
		clk_disable(ctx->clk);

	return ret;
}

static int
regmap_icc_update_bits32le(struct regmap_icc_context *ctx, unsigned int reg,
			   unsigned int mask, unsigned int val)
{
	pr_debug("ICC Update bits: addr: 0x%llx, mask: 0x%x, val: 0x%x",
		 ctx->paddr + reg, mask, val);

	return icc_regmap_sync_update_bits(ctx->paddr + reg, mask, val);
}

static int regmap_icc_update_bits(void *context, unsigned int reg,
				  unsigned int mask, unsigned int val)
{
	struct regmap_icc_context *ctx = context;
	int ret;

	if (!IS_ERR(ctx->clk)) {
		ret = clk_enable(ctx->clk);
		if (ret < 0)
			return ret;
	}

	ret = ctx->reg_update_bits(ctx, reg, mask, val);
	if (ret)
		pr_err("icc write mask err %d: addr: 0x%llx, mask: 0x%x, val: 0x%x\n",
		       ret, ctx->paddr + reg, mask, val);

	if (!IS_ERR(ctx->clk))
		clk_disable(ctx->clk);

	return ret;
}

static void regmap_icc_free_context(void *context)
{
	struct regmap_icc_context *ctx = context;

	if (!IS_ERR(ctx->clk)) {
		clk_unprepare(ctx->clk);
		if (!ctx->attached_clk)
			clk_put(ctx->clk);
	}
	kfree(context);
}

static const struct regmap_bus regmap_icc = {
	.fast_io                   = 1,
	.reg_write                 = regmap_icc_write,
	.reg_read                  = regmap_icc_read,
	.reg_update_bits           = regmap_icc_update_bits,
	.free_context              = regmap_icc_free_context,
	.val_format_endian_default = REGMAP_ENDIAN_LITTLE,
};

static struct regmap_icc_context
*regmap_icc_gen_context(struct device *dev, const char *clk_id,
			void __iomem *regs, phys_addr_t paddr,
			const struct regmap_config *config)
{
	struct regmap_icc_context *ctx;
	int min_stride;
	int ret;

	ret = regmap_icc_regbits_check(config->reg_bits);
	if (ret)
		return ERR_PTR(ret);

	if (config->pad_bits)
		return ERR_PTR(-EINVAL);

	min_stride = regmap_icc_get_min_stride(config->val_bits);
	if (min_stride < 0)
		return ERR_PTR(min_stride);

	if (config->reg_stride < min_stride)
		return ERR_PTR(-EINVAL);

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return ERR_PTR(-ENOMEM);

	ctx->regs = regs;
	ctx->paddr = paddr;
	ctx->val_bytes = config->val_bits / 8;
	ctx->clk = ERR_PTR(-ENODEV);
	ctx->name = config->name;

	switch (regmap_get_val_endian(dev, &regmap_icc, config)) {
	case REGMAP_ENDIAN_DEFAULT:
	case REGMAP_ENDIAN_LITTLE:
#ifdef __LITTLE_ENDIAN
	case REGMAP_ENDIAN_NATIVE:
#endif
		ctx->reg_read = regmap_icc_read32le;
		ctx->reg_write = regmap_icc_write32le;
		ctx->reg_update_bits = regmap_icc_update_bits32le;
		break;
	default:
		ret = -EINVAL;
		goto err_free;
	}

	if (!clk_id)
		return ctx;

	ctx->clk = clk_get(dev, clk_id);
	if (IS_ERR(ctx->clk)) {
		ret = PTR_ERR(ctx->clk);
		goto err_free;
	}

	ret = clk_prepare(ctx->clk);
	if (ret < 0) {
		clk_put(ctx->clk);
		goto err_free;
	}

	return ctx;

err_free:
	kfree(ctx);

	return ERR_PTR(ret);
}

struct regmap *__regmap_init_icc_clk(struct device *dev, const char *clk_id,
				     void __iomem *regs, phys_addr_t paddr,
				     const struct regmap_config *config,
				     struct lock_class_key *lock_key,
				     const char *lock_name)
{
	struct regmap_icc_context *ctx;

	ctx = regmap_icc_gen_context(dev, clk_id, regs, paddr, config);
	if (IS_ERR(ctx))
		return ERR_CAST(ctx);

	return __regmap_init(dev, &regmap_icc, ctx, config,
			     lock_key, lock_name);
}
EXPORT_SYMBOL_GPL(__regmap_init_icc_clk);

struct regmap *__devm_regmap_init_icc_clk(struct device *dev,
					  const char *clk_id,
					  void __iomem *regs, phys_addr_t paddr,
					  const struct regmap_config *config,
					  struct lock_class_key *lock_key,
					  const char *lock_name)
{
	struct regmap_icc_context *ctx;

	ctx = regmap_icc_gen_context(dev, clk_id, regs, paddr, config);
	if (IS_ERR(ctx))
		return ERR_CAST(ctx);

	return __devm_regmap_init(dev, &regmap_icc, ctx, config,
				  lock_key, lock_name);
}
EXPORT_SYMBOL_GPL(__devm_regmap_init_icc_clk);

int regmap_icc_attach_clk(struct regmap *map, struct clk *clk)
{
	struct regmap_icc_context *ctx = map->bus_context;

	ctx->clk = clk;
	ctx->attached_clk = true;

	return clk_prepare(ctx->clk);
}
EXPORT_SYMBOL_GPL(regmap_icc_attach_clk);

void regmap_icc_detach_clk(struct regmap *map)
{
	struct regmap_icc_context *ctx = map->bus_context;

	clk_unprepare(ctx->clk);

	ctx->attached_clk = false;
	ctx->clk = NULL;
}
EXPORT_SYMBOL_GPL(regmap_icc_detach_clk);

/* debugfs support */
#if IS_ENABLED(CONFIG_DEBUG_FS)

#include <linux/debugfs.h>

enum{
	REGMAP_DBG_READ,
	REGMAP_DBG_WRITE,
	REGMAP_DBG_UPDATE_BITS,
};

static ssize_t
regmap_reg_dbg(struct file *s, const char __user *buffer,
	       size_t count, loff_t *pos)
{
	char buf[128] = {0};
	char *cact, *caddr, *cval, *cmask;
	u32 reg_addr, reg_val, reg_mask, action;
	int i, len;

	len = count < sizeof(buf) ? count : sizeof(buf) - 1;
	if (copy_from_user(buf, buffer, len))
		return -EFAULT;

	if (strcmp(buf, "help") == 0)
		goto __err_help;

	pr_info("buf: %s\n", buf);

	cval = NULL;
	cmask = NULL;
	cact = buf;
	for (i = 0; buf[i] != ' ' && buf[i] != 0; i++)
		;

	if (buf[i] == 0)
		goto __err_help;

	buf[i++] = 0;

	pr_info("cact: %s\n", cact);
	if (!strcmp(cact, "r"))
		action = REGMAP_DBG_READ;
	else if (!strcmp(cact, "w"))
		action = REGMAP_DBG_WRITE;
	else if (!strcmp(cact, "u"))
		action = REGMAP_DBG_UPDATE_BITS;
	else
		goto __err_help;

	caddr = &buf[i];
	for (; buf[i] != ' ' && buf[i] != 0; i++)
		;

	if (buf[i] == 0)
		goto __action;
	buf[i++] = 0;

	pr_info("addr: %s\n", caddr);

	cval = &buf[i];
	for (; buf[i] != ' ' && buf[i] != 0; i++)
		;
	if (buf[i] == 0)
		goto __action;
	buf[i++] = 0;
	pr_info("cval: %s\n", cval);

	cmask = &buf[i];
	for (; buf[i] != ' ' && buf[i] != 0; i++)
		;
	buf[i++] = 0;
	pr_info("cmask: %s\n", cmask);

__action:
	switch (action) {
	case REGMAP_DBG_UPDATE_BITS:
		if (!cmask || kstrtou32(cmask, 0, &reg_mask)) {
			pr_err("Failed to parse reg mask!\n");
			goto __err_help;
		}
		break;
	case REGMAP_DBG_WRITE:
		if (!cval || kstrtou32(cval, 0, &reg_val)) {
			pr_err("Failed to parse reg value!\n");
			goto __err_help;
		}
		break;
	case REGMAP_DBG_READ:
		if (!caddr || kstrtou32(caddr, 0, &reg_addr)) {
			pr_err("Failed to parse reg address!\n");
			goto __err_help;
		}
		break;
	default:
		break;
	}

	switch (action) {
	case REGMAP_DBG_UPDATE_BITS:
		pr_info("update_bits: regaddr: 0x%08x, regval: 0x%08x regmask: 0x%08x\n",
			reg_addr, reg_val, reg_mask);
		icc_regmap_sync_update_bits(reg_addr, reg_mask, reg_val);
		break;
	case REGMAP_DBG_WRITE:
		pr_info("write: regaddr: 0x%08x, regval: 0x%08x\n",
			reg_addr, reg_val);
		icc_regmap_sync_write(reg_addr, reg_val);
		break;
	case REGMAP_DBG_READ:
		pr_info("icc read addr: 0x%08x\n", reg_addr);
		icc_regmap_sync_read(reg_addr, &reg_val);
		pr_info("read addr: 0x%08x, val: 0x%08x\n",
			reg_addr, reg_val);
		break;
	}

	return count;

__err_help:
	pr_info("echo r <addr> > dbg\n");
	pr_info("echo w <addr> <value> > dbg\n");
	pr_info("echo u <addr> <value> <mask> > dbg\n");
	return count;
}

static const struct file_operations dbgfs_reg_dbg_fops = {
	.write = regmap_reg_dbg,
	.llseek = noop_llseek,
};

static int __init regmap_icc_debugfs_init(void)
{
	regmap_debugfs = debugfs_create_dir("regmap-icc", NULL);
	if (IS_ERR_OR_NULL(regmap_debugfs))
		return PTR_ERR(regmap_debugfs);

	debugfs_create_file("dbg", 0200, regmap_debugfs,
			    NULL, &dbgfs_reg_dbg_fops);

	return 0;
}
device_initcall(regmap_icc_debugfs_init);
#endif /* debugfs support function */

MODULE_LICENSE("GPL v2");
