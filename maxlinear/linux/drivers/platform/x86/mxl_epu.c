// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
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
#include <linux/interrupt.h>
#include <linux/iopoll.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mfd/syscon.h>
#include <linux/notifier.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/pm_domain.h>
#include <linux/pm_qos.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/suspend.h>
#include <dt-bindings/power/lgm-power.h>

#include "mxl_epu_regs.h"

/*
 * Per device domain specific EPU PM information
 * It is used to access registers in EPU
 */
struct epu_domain_info {
	u32 id;
	u32 ctl;
	u32 stat;
	u32 power_off;
	u32 bbu_idl;  /* BBU idle */
	u32 bbu_sby;  /* BBU standby */
	u32 pos;
	u32 mask;
	u32 valid;
	bool active_wakeup;
};

/*
 * Data attached to dev so that dev start/stop can use it
 * dev_start only takes device as parameter, we have to
 * pass one extra edev to access register
 */
struct epu_domain_data {
	const struct epu_domain_info	*pd_info; /* Register info */
	const struct generic_pm_domain	*genpd;
	struct epu_device		*edev; /* Base */
};

/*
 * EPU Power domain strcuture which holds genpd info and EPU register info
 */
struct epu_pm_domain {
	struct generic_pm_domain	genpd;
	struct dev_power_governor	*gov; /* XXX */
	const struct epu_domain_info	*pd_info; /* Register info */
	struct epu_device		*edev;
	/* Todo more tuning parameters */
};

struct epu_data {
	int num_domains;
	const struct epu_domain_info *domain_info;
};

/* EPU Unit control */
struct epu_device {
	struct device	*dev;
	struct regmap	*aon_regmap;
	struct regmap	*gen_regmap;
	struct regmap   *top_regmap;
	struct clk	*clk;
	const struct epu_data *soc_data;
	struct notifier_block blocking_notifier;
	struct notifier_block raw_notifier;
	struct regulator	*vdd_cpu[2];
	struct regulator	*vdd_adp;
	int			irq[2];
	u32			vol_base;
	u32			vol_step;
	unsigned long		irq_bitmap;
	u32			xpcs_set;
#if IS_ENABLED(CONFIG_DEBUG_FS)
	struct dentry *debugfs_dir;
#endif
	raw_spinlock_t	i2c_lock; /* lock to protect i2c HW lock */
	int		i2c_hwlock_cnt;
	struct generic_pm_domain *domains[];
};

#define DOMAIN(_id, _idx, _off, _bbu, _nbit, _wk)	\
{							\
	.id = _id,					\
	.ctl = EPU_DEV_PM_CTL_0 + ((_idx) << 2),	\
	.stat = EPU_DEV_PM_ST_0 + ((_idx) << 2),	\
	.power_off = (_off) << (_nbit),			\
	.bbu_idl = _bbu,				\
	.bbu_sby = _bbu == EPU_D2 ? EPU_D0 : _bbu,	\
	.pos = _nbit,					\
	.mask = GENMASK((_nbit) + 1, (_nbit)),		\
	.valid = 1,					\
	.active_wakeup = _wk,				\
}

#define to_epu_pd(gpd) container_of(gpd, struct epu_pm_domain, genpd)

#define REG_NUM	2
#define EPU_POLL_DELAY_US	200
#define EPU_POLL_TIMEOUT	20000

static RAW_NOTIFIER_HEAD(epu_raw_chain);
static RAW_NOTIFIER_HEAD(rcu_chain);
static BLOCKING_NOTIFIER_HEAD(epu_blocking_chain);
static BLOCKING_NOTIFIER_HEAD(adp_chain);

#define SOC_OPC_INT_SET_EN(n)		(0x6010 + (n) * 0x2000)
#define SOC3_OPC_INT_SET_EN		0x20010
#define OPC_INT_TEMP_CHANGE		BIT_ULL(6)

#define PASEQ3_PASEQ_IDX_TAB0		0x16800

#define SOC_OPT_VFT_TBL_MEM0(n)		(0x7c00 + (n) * 0x2000)
#define SOC3_OPT_VFT_TBL_MEM0		0x21c00

#define UNCORE_PDC_POWER_DOWN_TIMER0	0xa60
#define PDC_POWER_DOWN_TIMER0_ISO_MSK	GENMASK_ULL(31, 24)

#define UNCORE_PDC_POWER_DOWN_TIMER1	0xa68
#define PDC_POWER_DOWN_TIMER1_PSO_MSK	GENMASK_ULL(63, 56)

#define INDEX_TAB(s, i, j, k, l, m, n) ((u64)(s) | ((u64)(i) << 16) |\
					((u64)(j) << 24) | ((u64)(k) << 32) | \
					((u64)(l) << 40) | ((u64)(m) << 48) | \
					((u64)(n) << 56))
#define OPC_VFT_TAB(rpi0, rpi1, intr, tmp) ((u64)(tmp) | \
					    ((u64)(intr) << 11) | \
					    ((u64)(rpi0) << 16) | \
					    ((u64)(rpi1) << 24))

static struct epu_device *epu_dev;

static const u64 adp_gp_reg_tab[] = {
	0xe0500000, 0x12, 0x0, 0x802d, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static const u64 cpum0_gp_reg_tab[] = {
	0xe0500000, 0xa, 0x0, 0x812d, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static const u64 cpum1_gp_reg_tab[] = {
	0xe0500000, 0xe, 0x0, 0x822d, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static const u64 soc_gp_reg_tab[] = {
	0xe0500000, 0x16, 0x0, 0x832d, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static const u64 adp_paseq0_tab[] = {
	INDEX_TAB(0x0,  0x0,  0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x0,  0x29, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x0,  0x2d, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x0,  0x3d, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x0,  0x4d, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x24, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x2c, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
};

static const u64 cpum0_paseq_tab[] = {
	INDEX_TAB(0x100, 0x0,  0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x2d, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x55, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x56, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x57, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x58, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x6f, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x100, 0x70, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x124, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x12c, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
};

static const u64 cpum1_paseq_tab[] = {
	INDEX_TAB(0x200, 0x0,  0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x2d, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x55, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x56, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x57, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x58, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x6f, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x200, 0x70, 0x2, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x224, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x22c, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
};

static const u64 soc_paseq_tab[] = {
	INDEX_TAB(0x3f, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x24, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
	INDEX_TAB(0x2c, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0),
};

static const u64 adp_opc_vft_tab[] = {
	OPC_VFT_TAB(0x6, 0x6, 0x1, 0xbb), OPC_VFT_TAB(0x5, 0x5, 0x0, 0xb7),
	OPC_VFT_TAB(0x0, 0x0, 0x0, 0xa0), OPC_VFT_TAB(0x6, 0x6, 0x1, 0x9b),
	OPC_VFT_TAB(0x5, 0x5, 0x5, 0x97), OPC_VFT_TAB(0x1, 0x1, 0x0, 0x80),
	OPC_VFT_TAB(0x6, 0x6, 0x1, 0x5b), OPC_VFT_TAB(0x5, 0x5, 0x0, 0x57),
	OPC_VFT_TAB(0x2, 0x2, 0x0, 0x40), OPC_VFT_TAB(0x6, 0x6, 0x1, 0x3b),
	OPC_VFT_TAB(0x5, 0x5, 0x0, 0x37), OPC_VFT_TAB(0x3, 0x3, 0x0, 0x20),
	OPC_VFT_TAB(0x6, 0x6, 0x1, 0x1b), OPC_VFT_TAB(0x5, 0x5, 0x0, 0x17),
	OPC_VFT_TAB(0x4, 0x4, 0x0, 0x0),  OPC_VFT_TAB(0x2, 0x2, 0x0, 0x0),
};

static const u64 cpum_opc_vft_tab[] = {
	OPC_VFT_TAB(0x9, 0x0, 0x1, 0xfb), OPC_VFT_TAB(0x8, 0x0, 0x0, 0xf7),
	OPC_VFT_TAB(0x7, 0x0, 0x0, 0xe0), OPC_VFT_TAB(0x9, 0x0, 0x1, 0xdb),
	OPC_VFT_TAB(0x8, 0x0, 0x0, 0xd7), OPC_VFT_TAB(0x6, 0x0, 0x0, 0xc0),
	OPC_VFT_TAB(0x9, 0x0, 0x1, 0xbb), OPC_VFT_TAB(0x8, 0x0, 0x0, 0xb7),
	OPC_VFT_TAB(0x5, 0x0, 0x0, 0xa0), OPC_VFT_TAB(0x9, 0x0, 0x1, 0x9b),
	OPC_VFT_TAB(0x8, 0x0, 0x0, 0x97), OPC_VFT_TAB(0x4, 0x0, 0x0, 0x80),
	OPC_VFT_TAB(0x9, 0x0, 0x1, 0x7b), OPC_VFT_TAB(0x8, 0x0, 0x0, 0x77),
	OPC_VFT_TAB(0x3, 0x0, 0x0, 0x60), OPC_VFT_TAB(0x9, 0x0, 0x1, 0x5b),
	OPC_VFT_TAB(0x8, 0x0, 0x0, 0x57), OPC_VFT_TAB(0x2, 0x0, 0x0, 0x40),
	OPC_VFT_TAB(0x9, 0x0, 0x1, 0x3b), OPC_VFT_TAB(0x8, 0x0, 0x0, 0x37),
	OPC_VFT_TAB(0x1, 0x0, 0x0, 0x20), OPC_VFT_TAB(0x9, 0x0, 0x1, 0x1b),
	OPC_VFT_TAB(0x8, 0x0, 0x0, 0x17), OPC_VFT_TAB(0x00, 0x0, 0x0, 0x0),
	OPC_VFT_TAB(0x2, 0x4, 0x0, 0x40)
};

static const u64 soc_opc_vft_tab[] = {
	OPC_VFT_TAB(0x2, 0x0, 0x1, 0x1b), OPC_VFT_TAB(0x1, 0x0, 0x0, 0x17),
	OPC_VFT_TAB(0x0, 0x0, 0x0, 0x0)
};

static void __maybe_unused epu_adp_init(struct regmap	*regmap)
{
	regmap_bulk_write(regmap, PASEQ0_PASEQ_GP_REG0, &adp_gp_reg_tab[0],
			  ARRAY_SIZE(adp_gp_reg_tab) * REG_NUM);

	regmap_bulk_write(regmap, PASEQ0_IDX_TBL(0), &adp_paseq0_tab[0],
			  ARRAY_SIZE(adp_paseq0_tab) * REG_NUM);

	regmap_bulk_write(regmap, ADP_OPT_VFT_TBL_MEM0, &adp_opc_vft_tab[0],
			  ARRAY_SIZE(adp_opc_vft_tab) * REG_NUM);
}

static void __maybe_unused epu_cpu0_init(struct regmap	*regmap)
{
	u64 val;

	regmap_bulk_write(regmap, PASEQ1_PASEQ_GP_REG0, &cpum0_gp_reg_tab[0],
			  ARRAY_SIZE(cpum0_gp_reg_tab) * REG_NUM);

	regmap_bulk_write(regmap, PASEQM_IDX_TBL(0, 0), &cpum0_paseq_tab[0],
			  ARRAY_SIZE(cpum0_paseq_tab) * REG_NUM);

	regmap_bulk_write(regmap, CPUM0_OPT_VFT_TBL_MEM0, &cpum_opc_vft_tab[0],
			  ARRAY_SIZE(cpum_opc_vft_tab) * REG_NUM);

	regmap_bulk_read(regmap, EPU_CPUM0_OPC_IRQ_SET_EN, &val, REG_NUM);
	val |= OPC_INT_TEMP_CHANGE;
	regmap_bulk_write(regmap, EPU_CPUM0_OPC_IRQ_SET_EN, &val, REG_NUM);
}

static void __maybe_unused epu_cpu1_init(struct regmap	*regmap)
{
	u64 val;

	regmap_bulk_write(regmap, PASEQ2_PASEQ_GP_REG0, &cpum1_gp_reg_tab,
			  ARRAY_SIZE(cpum1_gp_reg_tab) * REG_NUM);

	regmap_bulk_write(regmap, PASEQM_IDX_TBL(1, 0), &cpum1_paseq_tab[0],
			  ARRAY_SIZE(cpum1_paseq_tab) * REG_NUM);

	regmap_bulk_write(regmap, CPUM1_OPT_VFT_TBL_MEM0, &cpum_opc_vft_tab[0],
			  ARRAY_SIZE(cpum_opc_vft_tab) * REG_NUM);

	regmap_bulk_read(regmap, EPU_CPUM1_OPC_IRQ_SET_EN, &val, REG_NUM);
	val |= OPC_INT_TEMP_CHANGE;
	regmap_bulk_write(regmap, EPU_CPUM1_OPC_IRQ_SET_EN, &val, REG_NUM);
}

static void __maybe_unused epu_soc_init(struct regmap	*regmap)
{
	u64 val;
	int i;

	regmap_bulk_write(regmap, PASEQ3_PASEQ_GP_REG0, &soc_gp_reg_tab,
			  ARRAY_SIZE(soc_gp_reg_tab) * REG_NUM);

	regmap_bulk_write(regmap, PASEQ3_PASEQ_IDX_TAB0, &soc_paseq_tab[0],
			  ARRAY_SIZE(soc_paseq_tab) * REG_NUM);

	for (i = 0; i < 3; i++)
		regmap_bulk_write(regmap, SOC_OPT_VFT_TBL_MEM0(i),
				  &soc_opc_vft_tab[0],
				  ARRAY_SIZE(soc_opc_vft_tab) * REG_NUM);

	regmap_bulk_write(regmap, SOC3_OPT_VFT_TBL_MEM0, &soc_opc_vft_tab[0],
			  ARRAY_SIZE(soc_opc_vft_tab) * REG_NUM);

	for (i = 0; i < 3; i++) {
		regmap_bulk_read(regmap, SOC_OPC_INT_SET_EN(i), &val, REG_NUM);
		val |= OPC_INT_TEMP_CHANGE;
		regmap_bulk_write(regmap, SOC_OPC_INT_SET_EN(i),
				  &val, REG_NUM);
	}

	regmap_bulk_read(regmap, SOC3_OPC_INT_SET_EN, &val, REG_NUM);
	val |= OPC_INT_TEMP_CHANGE;
	regmap_bulk_write(regmap, SOC3_OPC_INT_SET_EN, &val, REG_NUM);

	regmap_bulk_read(regmap, UNCORE_PDC_POWER_DOWN_TIMER0, &val, REG_NUM);
	val &= ~PDC_POWER_DOWN_TIMER0_ISO_MSK;
	val |= FIELD_PREP(PDC_POWER_DOWN_TIMER0_ISO_MSK, 0x50);
	regmap_bulk_write(regmap, UNCORE_PDC_POWER_DOWN_TIMER0, &val, REG_NUM);

	regmap_bulk_read(regmap, UNCORE_PDC_POWER_DOWN_TIMER1, &val, REG_NUM);
	val &= ~PDC_POWER_DOWN_TIMER1_PSO_MSK;
	val |= FIELD_PREP(PDC_POWER_DOWN_TIMER1_PSO_MSK, 0x50);
	regmap_bulk_write(regmap, UNCORE_PDC_POWER_DOWN_TIMER1, &val, REG_NUM);
}

static void epu_enable_soc_opc(struct regmap *gen)
{
	int i;
	u64 val = OPC_CONTROL_RUN;

	for (i = 0; i < 3; i++)
		regmap_bulk_write(gen, EPU_SOC_OPC_CONTROL(i), &val, REG_NUM);

	regmap_bulk_write(gen, EPU_SOC3_OPC_CONTROL, &val, REG_NUM);
}

static int epu_poll_opc(struct epu_device *edev, u32 reg, int time)
{
	struct regmap *gen = edev->gen_regmap;
	u64 val;

	regmap_bulk_read(gen, reg, &val, REG_NUM);

	while (!(val & OPC_STATUS_IDLE_MASK) && time--) {
		usleep_range(1000, 2000);
		regmap_bulk_read(gen, reg, &val, REG_NUM);
	}

	if (time < 0) {
		dev_err(edev->dev, "Timeout to poll opc(0x%x)\n", reg);
		return -ETIMEDOUT;
	}

	return 0;
}

static int epu_set_opcs(struct epu_device *edev, bool en)
{
	struct regmap *gen = edev->gen_regmap;
	int i, ret;
	u64 val = 0;

	if (en)
		val = OPC_CONTROL_RUN | OPC_CONTROL_HLT_ON_TIMEOUT;

	if (!en) {
		ret = epu_poll_opc(edev, EPU_SYSTEM_OPC_STATUS, 15);
		if (ret)
			return ret;
		regmap_bulk_write(gen, EPU_SYSTEM_OPC_CONTROL, &val, REG_NUM);
	}

	if (!en) {
		ret = epu_poll_opc(edev, EPU_ADP_OPC_STATUS, 15);
		if (ret)
			return ret;
	}
	regmap_bulk_write(gen, EPU_ADP_OPC_CONTROL, &val, REG_NUM);

	for (i = 0; i < 2; i++) {
		if (!en) {
			ret = epu_poll_opc(edev, EPU_CPUM_OPC_STATUS(i), 15);
			if (ret)
				return ret;
		}
		regmap_bulk_write(gen, EPU_CPUM_OPC_CONTROL(i), &val, REG_NUM);
	}

	if (en) {
		regmap_bulk_write(gen, EPU_SYSTEM_OPC_CONTROL, &val, REG_NUM);
		return 0;
	}

	/* Poll the opc status after all the opcs are disabled */
	ret = epu_poll_opc(edev, EPU_SYSTEM_OPC_STATUS, 15);
	if (ret)
		return ret;

	for (i = 0; i < 2; i++) {
		ret = epu_poll_opc(edev, EPU_CPUM_OPC_STATUS(i), 15);
		if (ret)
			return ret;
	}

	return epu_poll_opc(edev, EPU_ADP_OPC_STATUS, 15);
}

static void epu_hw_init(struct epu_device *edev)
{
	//struct regmap *gen = edev->gen_regmap;

	epu_set_opcs(edev, false);

#if 0
	regmap_bulk_write(gen, PASEQM_IDX_TBL(0, 0), &cpum0_paseq_tab[0],
			  ARRAY_SIZE(cpum0_paseq_tab) * REG_NUM);

	regmap_bulk_write(gen, PASEQM_IDX_TBL(1, 0), &cpum1_paseq_tab[0],
			  ARRAY_SIZE(cpum1_paseq_tab) * REG_NUM);
#endif
	//epu_adp_init(gen);

	//epu_cpu0_init(gen);

	//epu_cpu1_init(gen);

	//epu_soc_init(gen);
}

#if IS_ENABLED(CONFIG_DEBUG_FS)
static int opc_vft_tbl_mem_print(struct seq_file *s,
				 char *table_name,
				 u32 start_adr, u32 size)
{
	u64 val;
	u32 offset;
	u32 rpi0, rpi1, int_bit, temp_l5, temp_h3, temp;
	u8 i;
	struct epu_device *edev = s->private;
	struct regmap     *regmap = edev->gen_regmap;

	seq_printf(s, "\n %s_OPC_VFT_TABLE OFFSET(0x%x)\n"
			, table_name, start_adr);
	seq_puts(s, "---------------------------------------------------------\n");
	seq_puts(s, "| index| rpi0 | rpi1 | int | temp | temp[7:5]| temp[4:0]|\n");
	for (i = 0; i < size; i++) {
		offset = start_adr + (i << 3);
		regmap_bulk_read(regmap, offset, &val, REG_NUM);
		rpi0 = FIELD_GET(OPC_VFT_TBL_MEM_RPI0_MSK, val);
		rpi1 = FIELD_GET(OPC_VFT_TBL_MEM_RPI1_MSK, val);
		int_bit = FIELD_GET(OPC_VFT_TBL_MEM_INT_MSK, val);
		temp = FIELD_GET(OPC_VFT_TBL_MEM_TEMP_MSK, val);
		temp_l5 = FIELD_GET(OPC_VFT_TBL_MEM_TEMP_MSK_L5, val);
		temp_h3 = FIELD_GET(OPC_VFT_TBL_MEM_TEMP_MSK_H3, val);
		seq_puts(s, "---------------------------------------------------------\n");
		seq_printf(s, "| %4d | 0x%02x | 0x%02x | %3d | 0x%02x |    0x%02x  |    0x%02x  |\n",
			   i, rpi0, rpi1, int_bit, temp, temp_h3, temp_l5);
	}

	return 0;
}

static int paseq_idx_tbl_print(struct seq_file *s,
			       char *table_name, u32 start_adr, u32 size)
{
	u64 val;
	u32 offset;
	u32 start, byte0, byte1, byte2, byte3, byte4, byte5, voltage;
	u8 i;
	struct epu_device *edev = s->private;
	struct regmap     *regmap = edev->gen_regmap;

	seq_printf(s, "\n PASEQ_%s_INDEX_TABLE OFFSET(0x%x)\n",
		   table_name, start_adr);
	seq_puts(s, "-------------------------------------------------------------------------\n");
	seq_puts(s, "|index|      byte0      | byte1 | byte2 | byte3 | byte4 | byte5 | start |\n");
	for (i = 0; i < size; i++) {
		offset = start_adr + (i << 3);
		regmap_bulk_read(regmap, offset, &val, REG_NUM);
		start = FIELD_GET(PASEQ_IDX_TBL_START_MSK, val);
		byte0 = FIELD_GET(PASEQ_IDX_TBL_BYE0_MSK, val);
		byte1 = FIELD_GET(PASEQ_IDX_TBL_BYE1_MSK, val);
		byte2 = FIELD_GET(PASEQ_IDX_TBL_BYE2_MSK, val);
		byte3 = FIELD_GET(PASEQ_IDX_TBL_BYE3_MSK, val);
		byte4 = FIELD_GET(PASEQ_IDX_TBL_BYE4_MSK, val);
		byte5 = FIELD_GET(PASEQ_IDX_TBL_BYE5_MSK, val);
		voltage = 0;
		if (byte0)
			voltage = (edev->vol_base +
				   edev->vol_step * (byte0 - 1)) / 1000;

		seq_puts(s, "-------------------------------------------------------------------------\n");
		if (voltage)
			seq_printf(s,
				   "| %3d |  0x%02x    %-4dmV |  0x%02x |  0x%02x |  0x%02x |  0x%02x |  0x%02x | 0x%03x |\n",
				   i, byte0, voltage, byte1, byte2, byte3, byte4, byte5, start);
		else
			seq_printf(s,
				   "| %3d |  0x%02x           |  0x%02x |  0x%02x |  0x%02x |  0x%02x |  0x%02x | 0x%03x |\n",
				   i, byte0, byte1, byte2, byte3, byte4, byte5, start);

		if (!strncmp(table_name, "ADP", 3))
			continue;

		switch (i) {
		case 2:
			seq_printf(s,
				   "|     |  624MHz  780 MHz|       |       |       |       |       |       |\n");
			seq_printf(s,
				   "|     |  936MHz  1092MHz|       |       |       |       |       |       |\n");
			break;
		case 3:
			seq_printf(s,
				   "|     |  1248MHz 1404MHz|       |       |       |       |       |       |\n");
			seq_printf(s,
				   "|     |          1560MHz|       |       |       |       |       |       |\n");
			break;
		case 4:
			seq_printf(s,
				   "|     |          1716MHz|       |       |       |       |       |       |\n");
			break;
		case 5:
			seq_printf(s,
				   "|     |          1872MHz|       |       |       |       |       |       |\n");
			break;
		case 6:
			seq_printf(s,
				   "|     |          2028MHz|       |       |       |       |       |       |\n");
			break;
		default:
			break;
		}
	}

	return 0;
}

static int adp_opc_resource_vft0_print(struct seq_file *s,
				       char *table_name,
				       u32 start_adr_cur, u32 start_adr_tgt)
{
	u64 val;
	u32 offset;
	u32 cur_rpi0, cur_rpi1, busy0, busy1, target_rpi0, target_rpi1;

	struct epu_device *edev = s->private;
	struct regmap     *regmap = edev->gen_regmap;

	offset = start_adr_cur;
	regmap_bulk_read(regmap, offset, &val, REG_NUM);
	cur_rpi0 = FIELD_GET(ADP_OPC_CUR_VFT0_RPI0_MSK, val);
	cur_rpi1 = FIELD_GET(ADP_OPC_CUR_VFT0_RPI1_MSK, val);
	busy0 = FIELD_GET(ADP_OPC_CUR_VFT0_BUSY0_MSK, val);
	busy1 = FIELD_GET(ADP_OPC_CUR_VFT0_BUSY1_MSK, val);
	seq_puts(s, "------------------------------------------------------\n");
	seq_printf(s, "| %5s(current)| 0x%04x| 0x%04x|  %d  |  %d  | 0x%05x|\n",
		   table_name, cur_rpi0, cur_rpi1, busy0, busy1, offset);

	offset = start_adr_tgt;
	regmap_bulk_read(regmap, offset, &val, REG_NUM);
	target_rpi0 = FIELD_GET(ADP_OPC_TARGET_VFT0_RPI0_MSK, val);
	target_rpi1 = FIELD_GET(ADP_OPC_TARGET_VFT0_RPI1_MSK, val);

	seq_puts(s, "-----------------------------------------------------\n");
	seq_printf(s, "| %5s(target) | 0x%04x| 0x%04x|  x  |  x  | 0x%05x|\n",
		   table_name, target_rpi0, target_rpi1, offset);

	return 0;
}

static int cpum_opc_resource_vft0_print(struct seq_file *s,
					char *table_name,
					u32 start_adr_cur, u32 start_adr_tgt)
{
	u64 val;
	u32 offset;
	u32 cur_rpi0, cur_rpi1, busy0, busy1, target_rpi0, target_rpi1;

	struct epu_device *edev = s->private;
	struct regmap     *regmap = edev->gen_regmap;

	offset = start_adr_cur;
	regmap_bulk_read(regmap, offset, &val, REG_NUM);
	cur_rpi0 = FIELD_GET(CPUM_OPC_CUR_VFT0_RPI0_MSK, val);
	cur_rpi1 = FIELD_GET(CPUM_OPC_CUR_VFT0_RPI1_MSK, val);
	busy0 = FIELD_GET(CPUM_OPC_CUR_VFT0_BUSY0_MSK, val);
	busy1 = FIELD_GET(CPUM_OPC_CUR_VFT0_BUSY1_MSK, val);
	seq_puts(s, "------------------------------------------------------\n");
	seq_printf(s, "| %5s(current)| 0x%04x| 0x%04x|  %d  |  %d  | 0x%05x|\n",
		   table_name, cur_rpi0, cur_rpi1, busy0, busy1, offset);

	offset = start_adr_tgt;
	regmap_bulk_read(regmap, offset, &val, REG_NUM);
	target_rpi0 = FIELD_GET(CPUM_OPC_TARGET_VFT0_RPI0_MSK, val);
	target_rpi1 = FIELD_GET(CPUM_OPC_TARGET_VFT0_RPI1_MSK, val);

	seq_puts(s, "-----------------------------------------------------\n");
	seq_printf(s, "| %5s(target) | 0x%04x| 0x%04x|  x  |  x  | 0x%05x|\n",
		   table_name, target_rpi0, target_rpi1, offset);

	return 0;
}

static int adp_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "ADP", ADP_OPT_VFT_TBL_MEM0, 16);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(adp_opc_vft_tbl_mem);

static int cpum0_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "CPUM0", CPUM0_OPT_VFT_TBL_MEM0, 25);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(cpum0_opc_vft_tbl_mem);

static int cpum1_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "CPUM1", CPUM1_OPT_VFT_TBL_MEM0, 25);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(cpum1_opc_vft_tbl_mem);

static int soc0_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "SOC0", SOC_OPT_VFT_TBL_MEM0(0), 3);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(soc0_opc_vft_tbl_mem);

static int soc1_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "SOC1", SOC_OPT_VFT_TBL_MEM0(1), 3);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(soc1_opc_vft_tbl_mem);

static int soc2_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "SOC2", SOC_OPT_VFT_TBL_MEM0(2), 3);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(soc2_opc_vft_tbl_mem);

static int soc3_opc_vft_tbl_mem_show(struct seq_file *s, void *v)
{
	opc_vft_tbl_mem_print(s, "SOC3", SOC3_OPT_VFT_TBL_MEM0, 3);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(soc3_opc_vft_tbl_mem);

static int paseq0_adp_idx_tbl_show(struct seq_file *s, void *v)
{
	paseq_idx_tbl_print(s, "ADP", PASEQ0_ADP_IDX_TBL_ADR, 16);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(paseq0_adp_idx_tbl);

static int paseq1_cpu0_idx_tbl_show(struct seq_file *s, void *v)
{
	paseq_idx_tbl_print(s, "CPU0", PASEQ1_CPU0_IDX_TBL_ADR, 16);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(paseq1_cpu0_idx_tbl);

static int paseq2_cpu1_idx_tbl_show(struct seq_file *s, void *v)
{
	paseq_idx_tbl_print(s, "CPU1", PASEQ2_CPU1_IDX_TBL_ADR, 16);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(paseq2_cpu1_idx_tbl);

static int opc_res_vft_info_show(struct seq_file *s, void *v)
{
	seq_puts(s, "------------------------------------------------------\n");
	seq_printf(s,
		   "|               |  rpi0 |  rpi1 | bsy0| bsy1|  offset|\n");
	adp_opc_resource_vft0_print(s, "ADP",
				    ADP_OPC_CUR_VFT0, ADP_OPC_TARGET_VFT0);
	cpum_opc_resource_vft0_print(s, "CPUM0", CPUM0_OPC_CUR_VFT0,
				     CPUM0_OPC_TARGET_VFT0);
	cpum_opc_resource_vft0_print(s, "CPUM1", CPUM1_OPC_CUR_VFT0,
				     CPUM1_OPC_TARGET_VFT0);
	return 0;
}

DEFINE_SHOW_ATTRIBUTE(opc_res_vft_info);

static void epu_pd_debugfs_exit(struct epu_device *edev)
{
	debugfs_remove_recursive(edev->debugfs_dir);
	kfree(edev);
	edev->debugfs_dir = NULL;
}

static int epu_pd_debugfs_init(struct epu_device *edev)
{
	struct dentry *epu_dir, *file;

	epu_dir = debugfs_create_dir(KBUILD_MODNAME, NULL);
	if (!epu_dir)
		return -ENOMEM;

	edev->debugfs_dir = epu_dir;

	file = debugfs_create_file("cpum0_opc_vft_tbl", 0444, epu_dir, edev,
				   &cpum0_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("cpum1_opc_vft_tbl", 0444, epu_dir, edev,
				   &cpum1_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("adp_opc_vft_tbl", 0444, epu_dir, edev,
				   &adp_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("soc0_opc_vft_tbl", 0444, epu_dir, edev,
				   &soc0_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("soc1_opc_vft_tbl", 0444, epu_dir, edev,
				   &soc1_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("soc2_opc_vft_tbl", 0444, epu_dir, edev,
				   &soc2_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("soc3_opc_vft_tbl", 0444, epu_dir, edev,
				   &soc3_opc_vft_tbl_mem_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("paseq0_adp_idx_tbl", 0444, epu_dir, edev,
				   &paseq0_adp_idx_tbl_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("paseq1_cpu0_idx_tbl", 0444, epu_dir, edev,
				   &paseq1_cpu0_idx_tbl_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("paseq2_cpu1_idx_tbl", 0444, epu_dir, edev,
				   &paseq2_cpu1_idx_tbl_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("opc_res_vft_info", 0444, epu_dir, edev,
				   &opc_res_vft_info_fops);
	if (!file)
		goto err;

	return 0;
err:
	epu_pd_debugfs_exit(edev);
	return -ENOMEM;
}
#else
static int epu_pd_debugfs_init(struct epu_device *edev)
{
	return 0;
}

static void epu_pd_debugfs_exit(struct epu_device *edev)
{
}
#endif

static void epu_set_combo_mode(struct regmap *regmap, u32 event)
{
	u32 val, mask;

	switch (event) {
	case COMBO_EVENT_PCIE(0):
	case COMBO_EVENT_SATA(0):
	case COMBO_EVENT_ETH(0):
		mask = EPU_COMBO_MODE_L_MSK;
		val = FIELD_PREP(EPU_COMBO_MODE_L_MSK, event & 0xff);
		break;
	case COMBO_EVENT_PCIE(1):
	case COMBO_EVENT_SATA(1):
	case COMBO_EVENT_ETH(1):
		mask = EPU_COMBO_MODE_R_MSK;
		val = FIELD_PREP(EPU_COMBO_MODE_R_MSK, event & 0xff);
		break;
	default:
		return;
	}

	regmap_update_bits(regmap, EPU_SYS_PM_CTL, mask, val);
}

static int epu_update_table_nolock(struct epu_device *edev, int cpu)
{
	struct regmap *gen = edev->gen_regmap;
	u32 i;
	int voltage;
	u64 val_1, val_2;
	u64 rpi0, vid;

	if (!edev->vdd_cpu[cpu])
		return 0;

	for (i = 2; i <= 6; i++) {
		regmap_bulk_read(gen, PASEQM_IDX_TBL(cpu, i),
				 &val_1, REG_NUM);
		regmap_bulk_read(gen, PASEQM_IDX_TBL(cpu, i + 8),
				 &val_2, REG_NUM);

		regmap_bulk_write(gen, PASEQM_IDX_TBL(cpu, i),
				  &val_2, REG_NUM);
		regmap_bulk_write(gen, PASEQM_IDX_TBL(cpu, i + 8),
				  &val_1, REG_NUM);
	}

	mb();

	/* Update the current CPU voltage */
	regmap_bulk_read(gen, EPU_CPUM_OPC_TARGET_VFT0(cpu), &rpi0, REG_NUM);
	rpi0 &= EPU_CPUM_VFT0_RPI0_MSK;

	regmap_bulk_read(gen, PASEQM_IDX_TBL(cpu, rpi0), &vid, REG_NUM);
	vid = FIELD_GET(PASEQ_IDX_TBL_BYE0_MSK, vid);

	voltage = 0;
	if (vid--)
		voltage = edev->vol_base + edev->vol_step * vid;

	return voltage;
}

/**
 * Function to get I2C HW lock.
 * This function has no lock to protect it
 */
static int _get_i2c_hw_lock(struct epu_device *edev)
{
	struct regmap *aon = edev->aon_regmap;
	int ret, stat;

	regmap_update_bits(aon, EPU_DEV_PM_CTL_0,
			   SW_PRIO_MSK | MSK_PUNIT_MSK,
			   FIELD_PREP(SW_PRIO_MSK, SW_PRIO_EN) |
			   FIELD_PREP(MSK_PUNIT_MSK, MSK_PUNIT_EN));

	regmap_write(aon, EPU_SW_I2C_REQ, EPU_SW_I2C_REQ_SET);
	ret = regmap_read_poll_timeout_atomic(aon, EPU_HW_I2C_REL,
					      stat, stat & EPU_HW_I2C_REL_SET,
					      EPU_POLL_DELAY_US,
					      EPU_POLL_TIMEOUT);
	if (ret) {
		regmap_update_bits(aon, EPU_DEV_PM_CTL_0,
				   SW_PRIO_MSK | MSK_PUNIT_MSK, 0);
		WARN_ONCE(ret, "Timeout to get i2c HW semaphore\n");
		return -EIO;
	}

	return 0;
}

static void cpu_wakeup_from_idle(void)
{
	cpu_idle_poll_ctrl(true);
	smp_mb();
	kick_all_cpus_sync();
}

static void cpu_release_to_idle(void)
{
	cpu_idle_poll_ctrl(false);
}

raw_spinlock_t *epu_i2c_raw_spinlock(void)
{
	WARN(!epu_dev, "Get EPU i2c spinlock before EPU init!\n");

	return &epu_dev->i2c_lock;
	
}
EXPORT_SYMBOL_GPL(epu_i2c_raw_spinlock);

/**
 * Wake up all CPUs to prevent i2c hw lock blocking CPU auto waking up.
 * spin_lock_irqsave is used to prevent intel_state driver
 * updating CPU frequency that will use i2c in HW PATH.
 */
static int spinlock_get_i2c_hw_lock(struct epu_device *edev)
{
	int ret;
	unsigned long flags;

	cpu_wakeup_from_idle();
	raw_spin_lock_irqsave(&edev->i2c_lock, flags);
	edev->i2c_hwlock_cnt += 1;
	if (edev->i2c_hwlock_cnt > 1) { /* SW already grabbed I2C HW lock */
		raw_spin_unlock_irqrestore(&edev->i2c_lock, flags);
		return 0;
	}

	ret = _get_i2c_hw_lock(edev);

	raw_spin_unlock_irqrestore(&edev->i2c_lock, flags);

	return ret;
}

/**
 * Release I2C HW lock.
 * This function has no lock to protect it.
 */
static void _release_i2c_hw_lock(struct epu_device *edev)
{
	struct regmap *aon = edev->aon_regmap;
	int ret, stat;

	regmap_update_bits(aon, EPU_DEV_PM_CTL_0, SW_PRIO_MSK | MSK_PUNIT_MSK, 0);
	regmap_write(aon, EPU_SW_I2C_REQ, EPU_SW_I2C_REQ_CLR);
	ret = regmap_read_poll_timeout_atomic(aon, EPU_HW_I2C_REL, stat,
					      !(stat & EPU_HW_I2C_REL_SET),
					      EPU_POLL_DELAY_US,
					      EPU_POLL_TIMEOUT);
	if (ret) {
		WARN_ONCE(ret, "Timeout to release i2c semaphore\n");
		return;
	}

	return;
}

/**
 * spin_lock_irqsave is used to prevent intel_state driver
 * updating CPU frequency that will use i2c in HW PATH.
 */
static void spinlock_release_i2c_hw_lock(struct epu_device *edev)
{
	unsigned long flags;

	cpu_release_to_idle();

	raw_spin_lock_irqsave(&edev->i2c_lock, flags);
	edev->i2c_hwlock_cnt -= 1;
	if (edev->i2c_hwlock_cnt > 0) { /* SW still hold lock  */
		raw_spin_unlock_irqrestore(&edev->i2c_lock, flags);
		return;
	}

	WARN_ON(edev->i2c_hwlock_cnt < 0);
	_release_i2c_hw_lock(edev);
	raw_spin_unlock_irqrestore(&edev->i2c_lock, flags);

	return;
}

/**
 * Update epu table to adapt to temperature change.
 * Apply corresponding voltage according to new EPU table.
 * EPU table setting guarantee there's no duplicated voltage
 * value in these 2 settings.
 * spin_lock_irqsave to protect the epu table update & I2C HW lock
 * from intel_pstate driver update voltage from HW path.
 * Note: Caller MUST manually release I2C HW lock
 */
static int spinlock_update_epu_table(struct epu_device *edev, int cpu)
{
	int voltage, ret = 0;
	unsigned long flags;
	struct regulator *regulator;

	cpu_wakeup_from_idle();

	raw_spin_lock_irqsave(&edev->i2c_lock, flags);
	edev->i2c_hwlock_cnt += 1;
	if (edev->i2c_hwlock_cnt == 1) { /* SW haven't get i2c HW lock */
		ret = _get_i2c_hw_lock(edev);

		if (ret) { /* grab lock failed */
			/**
			 * NO counter restore here as caller MUST call i2c hw lock release function
			 * even in failed case as caller can't tell the failure due to get hw i2c lock
			 */
			raw_spin_unlock_irqrestore(&edev->i2c_lock, flags);
			dev_err(edev->dev, "Failed to update epu table due to I2C HW lock failure!\n");
			return ret;
		}
	}

	voltage = epu_update_table_nolock(edev, cpu);

	/**
	 * Release cpu_cpum_lock once i2c Hw lock has been grabbed.
	 * PUNIt won't be able to update voltage until we release it.
	 */
	raw_spin_unlock_irqrestore(&edev->i2c_lock, flags);

	regulator = edev->vdd_cpu[cpu];
	if (voltage > 0 && regulator)
		ret = regulator_set_voltage(regulator, voltage, voltage);

	return ret;
}

static int epu_i2c_sem(struct epu_device *edev, u32 event, void *data)
{
	int cpu, ret = 0;

	switch (event) {
	case I2C_SEM_EVENT_REQUEST:
		if (data) {
			cpu = *(u32 *)data & 1;
			ret = spinlock_update_epu_table(edev, cpu);
		} else {
			ret = spinlock_get_i2c_hw_lock(edev);
		}

		break;
	case I2C_SEM_EVENT_RELEASE:
		spinlock_release_i2c_hw_lock(edev);
		
		break;
	default:
		return NOTIFY_DONE;
	}

	return notifier_from_errno(ret);
}

#define CHIP_TOP_ECO_SPARE_0		0x1c0
#define EC0_SW_PPV4_DVFS_LVL_MSK	GENMASK(10, 8)

static int epu_set_adp_lvl(struct epu_device *edev, u32 lvl)
{
	struct regmap *aon_regmap = edev->aon_regmap;
	struct regmap *top_regmap = edev->top_regmap;
	u32 ctrl, stat;
	int ret = 0;

	regmap_read(aon_regmap, EPU_ADP_PM_ST, &stat);
	regmap_read(aon_regmap, EPU_ADP_PM_CTL, &ctrl);

	if ((FIELD_GET(EPU_ADP_SW_DFS_LVL_MSK, stat) == lvl) &&
	    (FIELD_GET(EPU_ADP_DFS_LVL_MSK, ctrl) == lvl))
		return ret;

	if (ctrl & EPU_ADP_HW_DFS_EN) {
		regmap_update_bits(aon_regmap, EPU_ADP_PM_CTL,
				   EPU_ADP_HW_DFS_MSK, EPU_ADP_HW_DFS_DIS);
		regmap_update_bits(top_regmap, CHIP_TOP_ECO_SPARE_0, BIT(0), 0);
	}

	regmap_update_bits(aon_regmap, EPU_ADP_PM_CTL,
			   EPU_ADP_DFS_LVL_MSK,
			   FIELD_PREP(EPU_ADP_DFS_LVL_MSK, lvl));

	ret = regmap_read_poll_timeout_atomic(aon_regmap, EPU_ADP_PM_ST, stat,
					      FIELD_GET(EPU_ADP_SW_DFS_LVL_MSK, stat) == lvl,
					      EPU_POLL_DELAY_US,
					      EPU_POLL_TIMEOUT);

	regmap_update_bits(top_regmap, CHIP_TOP_ECO_SPARE_0,
			   EC0_SW_PPV4_DVFS_LVL_MSK,
			   FIELD_PREP(EC0_SW_PPV4_DVFS_LVL_MSK, lvl));

	if (ctrl & EPU_ADP_HW_DFS_EN) {
		regmap_update_bits(aon_regmap, EPU_ADP_PM_CTL,
				   EPU_ADP_HW_DFS_MSK, EPU_ADP_HW_DFS_EN);
		regmap_update_bits(top_regmap, CHIP_TOP_ECO_SPARE_0, BIT(0), BIT(0));
	}

	blocking_notifier_call_chain(&adp_chain, lvl, NULL);

	return ret;
}

static void epu_interrupt_init(struct epu_device *edev);

static int epu_pmic(struct epu_device *edev, u32 event, void *data)
{
	int ret = NOTIFY_OK;
	u64 vol;

	if (event != PMIC_EVENT_REG || !data)
		return NOTIFY_BAD;

	vol = *(u64 *)data;

	if (!edev->vdd_cpu[0]) {
		edev->vdd_cpu[0] = devm_regulator_get(edev->dev, "vdd-cpu0");
		if (IS_ERR(edev->vdd_cpu[0])) {
			ret = PTR_ERR(edev->vdd_cpu[0]);
			dev_err(edev->dev,
				"unable to get cpu0 regulator, ret = %d\n",
				ret);
			edev->vdd_cpu[0] = NULL;
			ret = NOTIFY_BAD;
		}
	}

	if (!edev->vdd_cpu[1]) {
		edev->vdd_cpu[1] = devm_regulator_get(edev->dev, "vdd-cpu1");
		if (IS_ERR(edev->vdd_cpu[1])) {
			ret = PTR_ERR(edev->vdd_cpu[1]);
			dev_err(edev->dev,
				"unable to get cpu1 regulator, ret = %d\n",
				ret);
			edev->vdd_cpu[1] = NULL;
			ret = NOTIFY_BAD;
		}
	}

	if (!edev->vdd_adp) {
		edev->vdd_adp = devm_regulator_get(edev->dev, "vdd-adp");
		if (IS_ERR(edev->vdd_adp)) {
			ret = PTR_ERR(edev->vdd_adp);
			dev_err(edev->dev,
				"unable to get adp regulator, ret = %d\n",
				ret);
			edev->vdd_adp = NULL;
			ret = NOTIFY_BAD;
		}
	}

	edev->vol_base = (u32)(vol >> 32);
	edev->vol_step = (u32)vol;

	epu_interrupt_init(edev);

	/* Any of power rail showing down during reboot could cause system hang,
	 * keep all power rail one during global SW reset, hence bypass PMIC
	 */
#if 0
	regmap_write(edev->aon_regmap, EPU_PMIC_POWERGOOD_TOGGLE,
		     EPU_PMIC_POWERGOOD_TOGGLE_SET);
#endif

	return ret;
}

static int epu_adp(struct epu_device *edev, u32 event, void *data)
{
	int ret;

	ret = epu_set_adp_lvl(edev, event & 0xff);
	if (ret) {
		dev_warn(edev->dev, "fail to set adp lvl to 0 ");
		ret = NOTIFY_BAD;
	}

	return NOTIFY_OK;
}

static int epu_soc_event(struct epu_device *edev, u32 event, void *data)
{
	u32 id = event & 0xff;
	struct epu_domain_info *pd_info;

	pd_info = (struct epu_domain_info *)&edev->soc_data->domain_info[id];

	pd_info->valid = 0;
	dev_dbg(edev->dev, "pd[%u] is fused!\n", pd_info->id);

	return NOTIFY_OK;
}

static int epu_rcu(struct epu_device *edev, u32 event, void *data)
{
	struct regmap *regmap = edev->aon_regmap;
	u32 id = event & 0xff;
	const struct epu_domain_info *pd_info = &edev->soc_data->domain_info[id];
	u32 val, target_state, dstate, op;
	int ret;

	if (edev->xpcs_set && id == LGM_EPU_PD_PON)
		return NOTIFY_OK;

	op = event >> 12 & 0xf;
	dstate = event >> 8 & 0xf;
	target_state = dstate << pd_info->pos;

	switch (op) {
	case EPU_SET:
		regmap_update_bits(regmap, pd_info->ctl, pd_info->mask, target_state);

		if (dstate == EPU_D0)
			return NOTIFY_OK;

		break;
	case EPU_GET:
		break;
	default:
		return NOTIFY_DONE;
	}

	ret = regmap_read_poll_timeout_atomic(regmap, pd_info->stat, val,
					      (val & pd_info->mask) == target_state,
					      EPU_POLL_DELAY_US,
					      EPU_POLL_TIMEOUT);
	if (ret) {
		dev_err(edev->dev, "set device(%d) to D%u timeout\n", pd_info->id, dstate);
		return notifier_from_errno(EIO);
	}

	return NOTIFY_OK;
}

static int epu_xpcs(struct epu_device *edev, u32 event, void *data)
{
	if (event != XPCS_EVENT_SET)
		return NOTIFY_BAD;

	edev->xpcs_set = 1;
	return NOTIFY_OK;
}


#define EVENT_ID(event) (((event) >> 16) & 0xFF)
#define CPU_EVENT_SUBID(event) (((event) >> 8) & 0xFF)
#define CPU_EVENT_CPUID(event) ((event) & 0xFF)

static int epu_blocking_notify(struct notifier_block *nb, unsigned long event,
			       void *data)
{
	struct epu_device *edev;
	int ret = NOTIFY_OK;

	edev = container_of(nb, struct epu_device, blocking_notifier);

	switch (EVENT_ID(event)) {
	case COMBO_EVENT_ID:
		epu_set_combo_mode(edev->aon_regmap, event);
		break;
	case I2C_SEM_EVENT_ID:
		ret = epu_i2c_sem(edev, event, data);
		break;
	case PMIC_EVENT_ID:
		ret = epu_pmic(edev, event, data);
		break;
	case ADP_EVENT_ID:
		ret = epu_adp(edev, event, data);
		break;
	case SOC_EVENT_ID:
		ret = epu_soc_event(edev, event, data);
		break;
	case XPCS_EVENT_ID:
		ret = epu_xpcs(edev, event, data);
		break;
	default:
		return NOTIFY_DONE;
	}

	return ret;
}

static int epu_raw_notify(struct notifier_block *nb, unsigned long event,
			  void *data)
{
	struct epu_device *edev;
	int ret = NOTIFY_OK;
	u32 cpu, eid;

	edev = container_of(nb, struct epu_device, raw_notifier);

	eid = EVENT_ID(event);
	if (eid == CPU_EVENT_ID)
		eid = CPU_EVENT_SUBID(event);

	cpu = CPU_EVENT_CPUID(event);

	switch (eid) {
	case CPU_REQ:
		regmap_write(edev->aon_regmap, EPU_CORE_PM_CTRL(cpu),
			     EPU_CORE_C7_REQ | EPU_SOFT_BIA_IDLE_REQ);
		break;
	case CPU_REL:
		regmap_write(edev->aon_regmap, EPU_CORE_PM_CTRL(cpu), 0);
		break;
	case RCU_EVENT_ID:
		ret = epu_rcu(edev, event, data);
		break;
	default:
		return NOTIFY_DONE;
	}

	return ret;
}

int epu_notifier_blocking_chain(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&epu_blocking_chain, val, v);
}
EXPORT_SYMBOL_GPL(epu_notifier_blocking_chain);

int epu_notifier_raw_chain(unsigned long val, void *v)
{
	return raw_notifier_call_chain(&epu_raw_chain, val, v);
}
EXPORT_SYMBOL_GPL(epu_notifier_raw_chain);

int epu_adp_lvl_notify_register(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&adp_chain, nb);
}
EXPORT_SYMBOL_GPL(epu_adp_lvl_notify_register);

int epu_adp_lvl_notify_unregister(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&adp_chain, nb);
}
EXPORT_SYMBOL_GPL(epu_adp_lvl_notify_unregister);

int epu_rcu_notify_register(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&rcu_chain, nb);
}
EXPORT_SYMBOL_GPL(epu_rcu_notify_register);

int epu_rcu_notify_unregister(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(&rcu_chain, nb);
}
EXPORT_SYMBOL_GPL(epu_rcu_notify_unregister);

/*
 * epu_attach_dev() - callback to attach a device to genpd. SOC specific
 * Device Power management information is initialized here
 */
static int epu_attach_dev(struct generic_pm_domain *genpd, struct device *dev)
{
	struct generic_pm_domain_data *genpd_data;
	struct epu_pm_domain *pd = to_epu_pd(genpd);
	struct epu_domain_data *pd_data;

	dev_dbg(dev, "attaching to power domain '%s'\n", genpd->name);

	pd_data = kzalloc(sizeof(*pd_data), GFP_KERNEL);
	if (!pd_data)
		return -ENOMEM;

	pd_data->pd_info = pd->pd_info;
	pd_data->genpd = genpd;
	pd_data->edev = pd->edev;
	genpd_data = dev_gpd_data(dev);
	genpd_data->data = pd_data;
	return 0;
}

/*
 * init epu_detach_dev() - callback when device is detached
 * Device Power Management data is released
 */
static void epu_detach_dev(struct generic_pm_domain *genpd, struct device *dev)
{
	struct generic_pm_domain_data *genpd_data = dev_gpd_data(dev);

	dev_dbg(dev, "detaching from power domain '%s'\n", genpd->name);
	kfree(genpd_data->data);
	genpd_data->data = NULL;
}

/*
 * epu_dev_start() - this function is called when device runtime_resume/resume
 * is called. Uses Device specific PM information to place device in D0 state
 */
static int epu_dev_start(struct device *dev)
{
	int ret;
	u32 val;
	struct generic_pm_domain_data *genpd_data = dev_gpd_data(dev);
	struct epu_domain_data *pd_data = genpd_data->data;
	struct epu_device *edev = pd_data->edev;
	const struct epu_domain_info *pd_info = pd_data->pd_info;
	struct regmap *regmap = edev->aon_regmap;

	regmap_update_bits(regmap, pd_info->ctl, pd_info->mask,
			   EPU_D0 << pd_info->pos);

	ret = regmap_read_poll_timeout_atomic(regmap, pd_info->stat, val,
					      ((val & pd_info->mask) >> pd_info->pos)
					      == EPU_D0, EPU_POLL_DELAY_US,
					      EPU_POLL_TIMEOUT);
	if (ret)
		dev_err(edev->dev, "dev start timeout\n");
	dev_dbg(dev, "epu start device %s\n", dev_name(dev));

	return ret;
}

/*
 * epu_dev_stop() - this function is called when device runtime_suspend/suspend
 * is called. It calculates target power state of the device using devpm_qos
 * value and then places in device in right state using Device specific PM
 * information
 */
static int epu_dev_stop(struct device *dev)
{
	int ret;
	u32 val;
	s32 target_latency, target_state;
	struct generic_pm_domain_data *genpd_data = dev_gpd_data(dev);
	struct epu_domain_data *pd_data = genpd_data->data;
	struct epu_device *edev = pd_data->edev;
	const struct epu_domain_info *pd_info = pd_data->pd_info;
	const struct generic_pm_domain *genpd = pd_data->genpd;
	struct regmap *regmap = edev->aon_regmap;

	if (genpd->device_count > 1)
		return 0;

	/* Deterime target state based on device pm_qos */
	target_latency = dev_pm_qos_read_value(dev, DEV_PM_QOS_RESUME_LATENCY);
	if (target_latency == 0) {
		target_state = EPU_D0;
	} else {
		/* target_state = latency_to_state(target_latency);
		 * need to define 'latency to state' comparing each available
		 * state with constraints
		 */
		target_state = EPU_D1;
	}

	regmap_update_bits(regmap, pd_info->ctl, pd_info->mask,
			   target_state << pd_info->pos);
	ret = regmap_read_poll_timeout_atomic(regmap, pd_info->stat, val,
					      ((val & pd_info->mask) >> pd_info->pos)
					      == target_state, EPU_POLL_DELAY_US,
					      EPU_POLL_TIMEOUT);
	if (ret)
		dev_err(edev->dev, "dev stop timeout\n");
	dev_dbg(dev, "epu stop device %s\n", dev_name(dev));

	return ret;
}

static bool is_pd_hsio(u32 id)
{
	switch (id) {
	case LGM_EPU_PD_HSIO1:
	case LGM_EPU_PD_HSIO2:
	case LGM_EPU_PD_HSIO3:
	case LGM_EPU_PD_HSIO4:
		return true;
	default:
		return false;
	};
}

static int epu_dev_set_d0(struct epu_device *edev,
			  const struct epu_domain_info *pd_info)
{
	struct regmap *regmap = edev->aon_regmap;
	u32 target_state = EPU_D0 << pd_info->pos;
	u32 val;
	int ret;

	regmap_update_bits(regmap, pd_info->ctl, pd_info->mask, target_state);

	raw_notifier_call_chain(&rcu_chain, pd_info->id, NULL);

	ret = regmap_read_poll_timeout_atomic(regmap, pd_info->stat, val,
					      (val & pd_info->mask) == target_state,
					      EPU_POLL_DELAY_US, EPU_POLL_TIMEOUT);
	if (ret) {
		dev_err(edev->dev, "set device(%d) to D0 timeout\n", pd_info->id);
		return ret;
	}

	return 0;
}

static int epu_hsio_d3_prepare(struct epu_device *edev, u32 id)
{
	const struct epu_domain_info *pd_info = edev->soc_data->domain_info;
	u32 val = 0, id_0, id_1;
	int ret;

	regmap_read(edev->aon_regmap, EPU_SYS_PM_CTL, &val);

	switch (id) {
	case LGM_EPU_PD_HSIO1:
		val = FIELD_GET(EPU_COMBO_MODE_L_MSK, val);
		switch (val) {
		case EPU_COMBO_MODE_PCIE:
			id_0 = LGM_EPU_PD_PCIE30;
			id_1 = LGM_EPU_PD_PCIE40;
			break;
		case EPU_COMBO_MODE_SATA:
			id_0 = LGM_EPU_PD_SATA0;
			id_1 = LGM_EPU_PD_SATA1;
			break;
		case EPU_COMBO_MODE_ETH:
			id_0 = LGM_EPU_PD_XPCS10;
			id_1 = LGM_EPU_PD_XPCS11;
			break;
		default:
			return -EINVAL;
		}
		break;
	case LGM_EPU_PD_HSIO2:
		val = FIELD_GET(EPU_COMBO_MODE_R_MSK, val);
		switch (val) {
		case EPU_COMBO_MODE_PCIE:
			id_0 = LGM_EPU_PD_PCIE31;
			id_1 = LGM_EPU_PD_PCIE41;
			break;
		case EPU_COMBO_MODE_SATA:
			id_0 = LGM_EPU_PD_SATA2;
			id_1 = LGM_EPU_PD_SATA3;
			break;
		case EPU_COMBO_MODE_ETH:
			id_0 = LGM_EPU_PD_XPCS20;
			id_1 = LGM_EPU_PD_XPCS21;
			break;
		default:
			return -EINVAL;
		}
		break;
	case LGM_EPU_PD_HSIO3:
		id_0 = LGM_EPU_PD_PCIE10;
		id_1 = LGM_EPU_PD_PCIE20;
		break;
	case LGM_EPU_PD_HSIO4:
		id_0 = LGM_EPU_PD_PCIE11;
		id_1 = LGM_EPU_PD_PCIE21;
		break;
	default:
		return 0;
	};

	ret = epu_dev_set_d0(edev, &pd_info[id_0]);
	if (ret)
		return ret;

	return epu_dev_set_d0(edev, &pd_info[id_1]);
}

/**
 * PON can't be put int D3 if nobody is using it.
 * It's because pon shell has internal reset register
 * that caused it default in reset.
 *
 * EPU can't communicate with PON if it is in reset and
 * EPU will stuck if no response from PON.
 *
 * This function is to make it out of reset so that EPU
 * can communicate with PON and put it into D3.
 */
#define PON_SHELL_GEN_CTRL	0
#define PON_RST_N		4
#define PON_CLK_GATE		12
static int epu_pon_shell_deassert(struct epu_device *edev)
{
	struct device_node *np = edev->dev->of_node;
	struct device_node *pon_node;
	void __iomem *ponshell_base;
	struct resource res;
	int idx;
	u32 val;

	pon_node = of_parse_phandle(np, "mxl,pon-shell", 0);
	if (!pon_node)
		return -ENODEV;

	idx = of_property_match_string(pon_node, "reg-names", "pon_shell");
	if (idx < 0)
		return -ENODATA;

	of_address_to_resource(pon_node, idx, &res);
	ponshell_base = ioremap(res.start, resource_size(&res));
	of_node_put(pon_node);

	val = readl(ponshell_base + PON_SHELL_GEN_CTRL);
	val |= BIT(PON_RST_N); /* 1 means reset inactive */
	val &= ~BIT(PON_CLK_GATE); /* 0 means no clock gate */
	writel(val, ponshell_base + PON_SHELL_GEN_CTRL);

	iounmap(ponshell_base);

	return 0;
}

/*
 * epu_pd_power_down() - This function is called when power domain is to
 * be powered off.
 * Uses power domain EPU registers to power off the domain
 */
static int epu_pd_power_off(struct generic_pm_domain *genpd)
{
	struct epu_pm_domain *pd = to_epu_pd(genpd);
	struct regmap *regmap = pd->edev->aon_regmap;
	const struct epu_domain_info *pd_info = pd->pd_info;
	u32 target_state = pd_info->power_off;
	u32 val, cur_state;
	int ret;

	if (!pd_info->valid) {
		dev_dbg(pd->edev->dev, "power off ignore fused %s\n",
			genpd->name);
		return 0;
	}

	regmap_read(regmap, pd_info->stat, &val);
	cur_state = val & pd_info->mask;

	if (cur_state == target_state)
		return 0;

	/* set to D0 */
	if (is_pd_hsio(pd_info->id))
		ret = epu_hsio_d3_prepare(pd->edev, pd_info->id);
	else
		ret = epu_dev_set_d0(pd->edev, pd_info);

	if (ret)
		return ret;

	if (pd_info->id == LGM_EPU_PD_PON) {
		ret = epu_pon_shell_deassert(pd->edev);
		if (ret)
			return ret;
	}

	/* set to target state */
	regmap_update_bits(regmap, pd_info->ctl, pd_info->mask, target_state);
	ret = regmap_read_poll_timeout_atomic(regmap, pd_info->stat, val,
					      (val & pd_info->mask) == target_state,
					      EPU_POLL_DELAY_US, EPU_POLL_TIMEOUT);
	if (ret) {
		dev_err(pd->edev->dev, "%s power off timeout\n", genpd->name);
		return ret;
	}

	dev_dbg(pd->edev->dev, "%s power off\n", genpd->name);

	return 0;
}

/*
 * epu_pd_power_up() - This function is called when power domain is to be
 * powered on. Uses power domain EPU registers to power on the domain
 */
static int epu_pd_power_on(struct generic_pm_domain *genpd)
{
	struct epu_pm_domain *pd = to_epu_pd(genpd);
	struct regmap *regmap = pd->edev->aon_regmap;
	const struct epu_domain_info *pd_info = pd->pd_info;
	u32 target_state = EPU_D0 << pd_info->pos;
	u32 val;
	int ret;

	if (!pd_info->valid) {
		dev_err(pd->edev->dev, "power on fused %s\n", genpd->name);
		return -ENODEV;
	}

	regmap_update_bits(regmap, pd_info->ctl, pd_info->mask, target_state);

	regmap_read(regmap, pd_info->stat, &val);
	if ((val & pd_info->mask) >> pd_info->pos != EPU_D3)
		raw_notifier_call_chain(&rcu_chain, pd_info->id, NULL);

	ret = regmap_read_poll_timeout_atomic(regmap, pd_info->stat, val,
					      (val & pd_info->mask) == target_state,
					      EPU_POLL_DELAY_US, EPU_POLL_TIMEOUT);
	if (ret) {
		dev_err(pd->edev->dev, "%s power on timeout\n", genpd->name);
		return ret;
	}

	dev_dbg(pd->edev->dev, "%s power on\n", genpd->name);

	return 0;
}

/*
 * init epu_add_one_pm_domain() - set up a power domain with necessary
 * callback registration
 * @np:    device_node pointer
 * @pd:    epu power domain pointer
 */
static int epu_add_one_pm_domain(struct epu_device *edev,
				 struct device_node *node,
				 struct generic_pm_domain *genpd_parent)
{
	int ret;
	u32 id;
	struct epu_pm_domain *pd;
	const struct epu_domain_info *pd_info;
	struct device_node *child;

	ret = of_property_read_u32(node, "reg", &id);
	if (ret) {
		dev_err(edev->dev,
			"%s: failed to retrieve domain id (reg): %d\n",
			node->name, ret);
		return -EINVAL;
	}

	if (id >= edev->soc_data->num_domains) {
		dev_err(edev->dev, "%s: invalid domain id %d\n",
			node->name, id);
		return -EINVAL;
	}

	pd_info = &edev->soc_data->domain_info[id];
	if (!pd_info) {
		dev_err(edev->dev, "%s: undefined domain id %d\n",
			node->name, id);
		return -EINVAL;
	}

	pd = devm_kzalloc(edev->dev, sizeof(*pd), GFP_KERNEL);
	if (!pd)
		return -ENOMEM;

	pd->pd_info = pd_info;
	pd->edev = edev;

	pd->genpd.name = node->name;
	pd->genpd.power_off = epu_pd_power_off;
	pd->genpd.power_on = epu_pd_power_on;
	pd->genpd.attach_dev = epu_attach_dev;
	pd->genpd.detach_dev = epu_detach_dev;
	pd->genpd.dev_ops.start = epu_dev_start;
	pd->genpd.dev_ops.stop = epu_dev_stop;
	pd->genpd.flags = GENPD_FLAG_IRQ_SAFE;
	if (pd_info->active_wakeup)
		pd->genpd.flags |= GENPD_FLAG_ACTIVE_WAKEUP;

	ret = pm_genpd_init(&pd->genpd, NULL, false);
	if (ret)
		return ret;

	edev->domains[id] = &pd->genpd;

	if (genpd_parent)
		pm_genpd_add_subdomain(genpd_parent, &pd->genpd);

	for_each_child_of_node(node, child) {
		ret = epu_add_one_pm_domain(edev, child, &pd->genpd);
		if (ret) {
			dev_err(edev->dev, "failed to handle node %s: %d\n",
				node->name, ret);
			of_node_put(node);
			return ret;
		}
	}

	return of_genpd_add_provider_simple(node, &pd->genpd);
}

static void epu_pm_remove_one_domain(struct epu_pm_domain *pd)
{
	int ret;

	/*
	 * We're in the error cleanup already, so we only complain,
	 * but won't emit another error on top of the original one.
	 */
	ret = pm_genpd_remove(&pd->genpd);
	if (ret < 0)
		dev_err(pd->edev->dev,
			"failed to remove domain '%s' : %d - state may be inconsistent\n",
			pd->genpd.name, ret);
	/* devm will free our memory */
}

static void epu_pm_domain_cleanup(struct epu_device *edev)
{
	struct generic_pm_domain *genpd;
	struct epu_pm_domain *pd;
	int i;

	for (i = 0; i < edev->soc_data->num_domains; i++) {
		genpd = edev->domains[i];
		if (genpd) {
			pd = to_epu_pd(genpd);
			epu_pm_remove_one_domain(pd);
		}
	}
	/* devm will free our memory */
}

#ifdef CONFIG_SUSPEND
static int epu_pm_valid(suspend_state_t state)
{
	return state == PM_SUSPEND_TO_IDLE;
}

static const struct platform_suspend_ops epu_suspend_ops = {
	.valid = epu_pm_valid,
};
#endif

#define EPU_ADP_INT_ID		44
#define EPU_CPUM0_INT_ID	45
#define EPU_CPUM1_INT_ID	58
#define EPU_MAX_INT_CHECK_NUM	64
#define EPU_INVLD_INT_ID	0xffff

#define EPU_IRQ_PRIO_D		U64_C(0xd)
#define EPU_IRQ_PRIO_E		U64_C(0xe)

#define EPU_IRQ_CPUM_BIT(i)	(0UL + (i))
#define EPU_IRQ_ADP_BIT		2UL

static irqreturn_t epu_timeout_thread_fn(int irq, void *dev_id)
{
	struct epu_device *edev = (struct epu_device *)dev_id;
	struct regmap *gen = edev->gen_regmap;
	int i, voltage, cur_voltage;
	u64 rpi0, cur_rpi0, vid, cur_vid;
	u64 val = OPC_CONTROL_RUN | OPC_CONTROL_HLT_ON_TIMEOUT;

	for_each_set_bit(i, &edev->irq_bitmap, 2) {
		clear_bit(i, &edev->irq_bitmap);

		regmap_bulk_read(gen, EPU_CPUM_OPC_TARGET_VFT0(i),
				 &rpi0, REG_NUM);
		rpi0 &= EPU_CPUM_VFT0_RPI0_MSK;

		regmap_bulk_read(gen, PASEQM_IDX_TBL(i, rpi0),
				 &vid, REG_NUM);
		vid = FIELD_GET(PASEQ_IDX_TBL_BYE0_MSK, vid);

		voltage = 0;
		if (vid--)
			voltage = edev->vol_base + edev->vol_step * vid;

		if (edev->vdd_cpu[i])
			regulator_set_voltage(edev->vdd_cpu[i],
					      voltage, voltage);
		else
			regmap_bulk_write(gen, EPU_CPUM_OPC_CONTROL(i),
					  &val, REG_NUM);

		regmap_bulk_read(gen, EPU_CPUM_OPC_CUR_VFT0(i),
				 &cur_rpi0, REG_NUM);
		cur_rpi0 &= EPU_CPUM_VFT0_RPI0_MSK;

		regmap_bulk_read(gen, PASEQM_IDX_TBL(i, cur_rpi0),
				 &cur_vid, REG_NUM);
		cur_vid = FIELD_GET(PASEQ_IDX_TBL_BYE0_MSK, cur_vid);

		cur_voltage = 0;
		if (cur_vid--)
			cur_voltage = edev->vol_base + edev->vol_step * cur_vid;

		dev_err(edev->dev,
			"CPU%d timeout, current voltage is %d mv, tareget voltage is %d mv\n",
			i, cur_voltage / 1000, voltage / 1000);
	}

	if (test_and_clear_bit(EPU_IRQ_ADP_BIT, &edev->irq_bitmap)) {
		regmap_bulk_read(gen, EPU_ADP_OPC_TARGET_VFT0,
				 &rpi0, REG_NUM);
		rpi0 &= EPU_ADP_VFT0_RPI0_MSK;

		regmap_bulk_read(gen, PASEQ0_IDX_TBL(rpi0),
				 &vid, REG_NUM);
		vid = FIELD_GET(PASEQ_IDX_TBL_BYE0_MSK, vid);

		voltage = 0;
		if (vid--)
			voltage = edev->vol_base + edev->vol_step * vid;

		if (edev->vdd_adp)
			regulator_set_voltage(edev->vdd_adp, voltage, voltage);
		else
			regmap_bulk_write(gen, EPU_ADP_OPC_CONTROL,
					  &val, REG_NUM);

		regmap_bulk_read(gen, ADP_OPC_CUR_VFT0, &cur_rpi0, REG_NUM);
		cur_rpi0 &= ADP_OPC_CUR_VFT0_RPI0_MSK;

		regmap_bulk_read(gen, PASEQ0_IDX_TBL(cur_rpi0),
				 &cur_vid, REG_NUM);
		cur_vid = FIELD_GET(PASEQ_IDX_TBL_BYE0_MSK, cur_vid);

		cur_voltage = 0;
		if (cur_vid--)
			cur_voltage = edev->vol_base + edev->vol_step * cur_vid;

		dev_err(edev->dev,
			"ADP timeout,  current voltage is %d mv, tareget voltage is %d mv\n",
			cur_voltage / 1000, voltage / 1000);
	}

	return IRQ_HANDLED;
}

static irqreturn_t epu_interrupt_0(int irq, void *dev_id)
{
	struct epu_device *edev = (struct epu_device *)dev_id;
	struct regmap *regmap = edev->gen_regmap;
	u32 counter = EPU_MAX_INT_CHECK_NUM;
	u32 cpu;
	u64 id, val;
	irqreturn_t ret = IRQ_NONE;

	regmap_bulk_read(regmap, EPU_ICE_IC_IRQ_ACK, &id, REG_NUM);
	id &= EPU_ICE_IC_IRQ_ACK_MSK;

	while ((id != EPU_INVLD_INT_ID) && counter--) {
		switch (id) {
		case EPU_CPUM0_INT_ID:
		case EPU_CPUM1_INT_ID:
			cpu = (id == EPU_CPUM0_INT_ID) ? 0 : 1;
			regmap_bulk_read(regmap,
					 EPU_CPUM_OPC_IRQ_PENDING(cpu),
					 &val, REG_NUM);
			if (val & EPU_OPC_IRQ_TIMEOUT) {
				set_bit(EPU_IRQ_CPUM_BIT(cpu),
					&edev->irq_bitmap);
				ret = IRQ_WAKE_THREAD;
			}

			regmap_bulk_write(regmap,
					  EPU_CPUM_OPC_IRQ_CLR_PENDING(cpu),
					  &val, REG_NUM);
			break;
		case EPU_ADP_INT_ID:
			regmap_bulk_read(regmap, EPU_ADP_OPC_IRQ_PENDING,
					 &val, REG_NUM);
			if (val & EPU_OPC_IRQ_TIMEOUT) {
				set_bit(EPU_IRQ_ADP_BIT, &edev->irq_bitmap);
				ret = IRQ_WAKE_THREAD;
			}

			regmap_bulk_write(regmap, EPU_ADP_OPC_IRQ_CLR_PENDING,
					  &val, REG_NUM);
			break;
		default:
			break;
		}

		regmap_bulk_write(regmap, EPU_ICE_IC_END_OF_IRQ, &id, REG_NUM);
		regmap_bulk_read(regmap, EPU_ICE_IC_IRQ_ACK, &id, REG_NUM);
		id &= EPU_ICE_IC_IRQ_ACK_MSK;
	}

	return ret;
}

static irqreturn_t epu_interrupt_1(int irq, void *dev_id)
{
	return IRQ_NONE;
}

static void epu_interrupt_init(struct epu_device *edev)
{
	struct regmap *gen_regmap = edev->gen_regmap;
	struct regmap *aon_regmap = edev->aon_regmap;
	unsigned long rate = clk_get_rate(edev->clk);
	u64 val;

	val = EPU_ICE_IC_CTL_GRP0_EN | EPU_ICE_IC_CTL_GRP0_RT0;
	regmap_bulk_write(gen_regmap, EPU_ICE_IC_CTL, &val, REG_NUM);

	val = 0xf << EPU_ICE_IC_PRIO_MSK_SFT;
	regmap_bulk_write(gen_regmap, EPU_ICE_IC_PRIO_MSK, &val, REG_NUM);

	val = 0;
	regmap_bulk_write(gen_regmap, EPU_ICE_IC_GRP0, &val, REG_NUM);

	val = BIT_ULL(EPU_ADP_INT_ID) |
	      BIT_ULL(EPU_CPUM0_INT_ID) |
	      BIT_ULL(EPU_CPUM1_INT_ID);
	regmap_bulk_write(gen_regmap, EPU_ICE_IC_SET_EN0, &val, REG_NUM);

	regmap_bulk_read(gen_regmap, EPU_ICE_IC_PRIO5, &val, REG_NUM);
	val |= EPU_IRQ_PRIO_E << (((EPU_ADP_INT_ID - 40) << 3) + 4) |
	       EPU_IRQ_PRIO_D << (((EPU_CPUM0_INT_ID - 40) << 3) + 4);
	regmap_bulk_write(gen_regmap, EPU_ICE_IC_PRIO5, &val, REG_NUM);

	regmap_bulk_read(gen_regmap, EPU_ICE_IC_PRIO7, &val, REG_NUM);
	val |= EPU_IRQ_PRIO_D << (((EPU_CPUM1_INT_ID - 56) << 3) + 4);
	regmap_bulk_write(gen_regmap, EPU_ICE_IC_PRIO7, &val, REG_NUM);

	regmap_bulk_read(gen_regmap, EPU_ADP_OPC_IRQ_SET_EN, &val, REG_NUM);
	val |= EPU_OPC_IRQ_TIMEOUT;
	regmap_bulk_write(gen_regmap, EPU_ADP_OPC_IRQ_SET_EN, &val, REG_NUM);
	regmap_bulk_read(gen_regmap, EPU_CPUM0_OPC_IRQ_SET_EN, &val, REG_NUM);
	val |= EPU_OPC_IRQ_TIMEOUT;
	regmap_bulk_write(gen_regmap, EPU_CPUM0_OPC_IRQ_SET_EN, &val, REG_NUM);
	regmap_bulk_read(gen_regmap, EPU_CPUM1_OPC_IRQ_SET_EN, &val, REG_NUM);
	val |= EPU_OPC_IRQ_TIMEOUT;
	regmap_bulk_write(gen_regmap, EPU_CPUM1_OPC_IRQ_SET_EN, &val, REG_NUM);

	/* Set 5ms to timeout value for ACK from PMIC to EPU */
	val = FIELD_PREP(EPU_OPC_TIMEOUT_MSK, (rate * 20) / (1000 * 4096)) |
	      FIELD_PREP(EPU_OPC_TIMEOUT_PRE_MSK, EPU_OPC_TIMEOUT_PRE_4096);
	regmap_bulk_write(gen_regmap, EPU_CPUM0_OPC_TIMEOUT, &val, REG_NUM);
	regmap_bulk_write(gen_regmap, EPU_CPUM1_OPC_TIMEOUT, &val, REG_NUM);
	regmap_bulk_write(gen_regmap, EPU_ADP_OPC_TIMEOUT, &val, REG_NUM);

	/* Disable timeout feature for ACK from EPU to PUNIT */
	regmap_write(aon_regmap, EPU_PMIC_CPU0_TIMEOUT, 0xffffffff);
	regmap_write(aon_regmap, EPU_PMIC_CPU1_TIMEOUT, 0xffffffff);
	regmap_write(aon_regmap, EPU_PMIC_ADP_TIMEOUT, 0xffffffff);
	regmap_write(aon_regmap, EPU_PMIC_REQ_TIME, 0xffffffff);
}

#define dstate_show(_name, num)					\
static ssize_t _name##_show						\
(struct device *dev, struct device_attribute *attr, char *buf)		\
{									\
	struct epu_device *epu = dev_get_drvdata(dev);			\
	u32 val;							\
									\
	regmap_read(epu->aon_regmap, EPU_DEV_PM_ST_0 + ((num) << 2), &val); \
									\
	return sprintf(buf, "0x%x\n", val);				\
}

dstate_show(dstate0, 0);
dstate_show(dstate1, 1);
dstate_show(dstate2, 2);
dstate_show(dstate3, 3);

static DEVICE_ATTR_RO(dstate0);
static DEVICE_ATTR_RO(dstate1);
static DEVICE_ATTR_RO(dstate2);
static DEVICE_ATTR_RO(dstate3);

#define telemetry_show(_name, reg)		\
static ssize_t _name##_show						\
(struct device *dev, struct device_attribute *attr, char *buf)		\
{									\
	struct epu_device *epu = dev_get_drvdata(dev);			\
	u32 hi, lo;							\
	u64 val;							\
									\
	regmap_read(epu->aon_regmap, reg, &lo);				\
	regmap_read(epu->aon_regmap, reg + 4, &hi);			\
									\
	val = (u64)hi << 32 | lo;					\
									\
	return sprintf(buf, "0x%llx\n", val);				\
}

telemetry_show(core0_C7_cnt, EPU_CORE_C7_CN_L(0));
telemetry_show(core1_C7_cnt, EPU_CORE_C7_CN_L(1));
telemetry_show(core2_C7_cnt, EPU_CORE_C7_CN_L(2));
telemetry_show(core3_C7_cnt, EPU_CORE_C7_CN_L(3));

telemetry_show(adp_sw_dvfs_req_cnt, EPU_ADP_SW_DVFS_REQ_CNL);
telemetry_show(adp_sw_dvfs_grant_cnt, EPU_ADP_SW_DVFS_GNT_CNL);
telemetry_show(adp_hw_dvfs_req_cnt, EPU_ADP_HW_DVFS_REQ_CNL);
telemetry_show(adp_hw_dvfs_grant_cnt, EPU_ADP_HW_DVFS_GNT_CNL);

telemetry_show(adp_dvfs_level_0_residency, EPU_ADP_DVFS_LVL_CNL(0));
telemetry_show(adp_dvfs_level_1_residency, EPU_ADP_DVFS_LVL_CNL(1));
telemetry_show(adp_dvfs_level_2_residency, EPU_ADP_DVFS_LVL_CNL(2));

telemetry_show(s0i3_pattern_match_cnt, EPU_S0I3_PAT_MTH_CNL);
telemetry_show(s0i3_pattern_mismatch_cnt, EPU_S0I3_PAT_MISMTH_CNL);
telemetry_show(s0i3_pattern_residency_cnt, EPU_S0I3_RESIDENCY_CNL);

static DEVICE_ATTR_RO(core0_C7_cnt);
static DEVICE_ATTR_RO(core1_C7_cnt);
static DEVICE_ATTR_RO(core2_C7_cnt);
static DEVICE_ATTR_RO(core3_C7_cnt);
static DEVICE_ATTR_RO(adp_sw_dvfs_req_cnt);
static DEVICE_ATTR_RO(adp_sw_dvfs_grant_cnt);
static DEVICE_ATTR_RO(adp_hw_dvfs_req_cnt);
static DEVICE_ATTR_RO(adp_hw_dvfs_grant_cnt);
static DEVICE_ATTR_RO(adp_dvfs_level_0_residency);
static DEVICE_ATTR_RO(adp_dvfs_level_1_residency);
static DEVICE_ATTR_RO(adp_dvfs_level_2_residency);
static DEVICE_ATTR_RO(s0i3_pattern_match_cnt);
static DEVICE_ATTR_RO(s0i3_pattern_mismatch_cnt);
static DEVICE_ATTR_RO(s0i3_pattern_residency_cnt);

static ssize_t adp_scaling_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct epu_device *epu = dev_get_drvdata(dev);

	if (!strncmp(buf, "on", strlen("on"))) {
		regmap_update_bits(epu->aon_regmap, EPU_ADP_PM_CTL,
				   EPU_ADP_HW_DFS_MSK, EPU_ADP_HW_DFS_EN);
		regmap_update_bits(epu->top_regmap, CHIP_TOP_ECO_SPARE_0,
				   BIT(0), BIT(0));
	} else if (!strncmp(buf, "off", strlen("off"))) {
		regmap_update_bits(epu->aon_regmap, EPU_ADP_PM_CTL,
				   EPU_ADP_HW_DFS_MSK, EPU_ADP_HW_DFS_DIS);
		regmap_update_bits(epu->top_regmap, CHIP_TOP_ECO_SPARE_0,
				   BIT(0), 0);
	} else {
		return -EINVAL;
	}

	return count;
}

static ssize_t adp_scaling_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct epu_device *epu = dev_get_drvdata(dev);
	u32 val;

	regmap_read(epu->aon_regmap, EPU_ADP_PM_CTL, &val);

	if ((val & EPU_ADP_HW_DFS_MSK) == EPU_ADP_HW_DFS_EN)
		return sprintf(buf, "on\n");
	else
		return sprintf(buf, "off\n");
}
static DEVICE_ATTR_RW(adp_scaling);

static ssize_t adp_dvfs_level_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct epu_device *edev = dev_get_drvdata(dev);
	struct regmap *aon_regmap = edev->aon_regmap;
	u32 val, stat;
	int ret;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

	if (val > EPU_ADP_DFS_LVL2)
		return -EINVAL;

	regmap_read(aon_regmap, EPU_ADP_PM_ST, &stat);

	if (FIELD_GET(EPU_ADP_SW_DFS_LVL_MSK, stat) == val)
		return count;

	ret = epu_set_adp_lvl(edev, val);
	if (ret)
		return ret;

	return count;
}

static ssize_t adp_dvfs_level_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct epu_device *edev = dev_get_drvdata(dev);
	u32 val;

	regmap_read(edev->aon_regmap, EPU_ADP_PM_ST, &val);

	return sprintf(buf, "%lu\n", val & EPU_ADP_CUR_DFS_LVL_MSK);
}
static DEVICE_ATTR_RW(adp_dvfs_level);

static ssize_t adp_level_config_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	struct epu_device *edev = dev_get_drvdata(dev);
	u32 val;
	ssize_t ret;

	ret = sprintf(buf, "sw_adp_dvfs_level_set: ");

	regmap_read(edev->aon_regmap, EPU_ADP_PM_CTL, &val);
	switch (FIELD_GET(EPU_ADP_DFS_LVL_MSK, val)) {
	case 0:
		ret += sprintf(buf + ret, "high\n");
		break;
	case 1:
		ret += sprintf(buf + ret, "med\n");
		break;
	case 2:
		ret += sprintf(buf + ret, "low\n");
		break;
	default:
		ret += sprintf(buf + ret, "\n");
	}

	ret += sprintf(buf + ret, "hw_adp_dfs_set: ");
	switch (FIELD_GET(EPU_ADP_HW_DFS_MSK, val)) {
	case 0:
		ret += sprintf(buf + ret, "off\n");
		break;
	case 1:
		ret += sprintf(buf + ret, "on\n");
		break;
	}

	return ret;
}
static DEVICE_ATTR_RO(adp_level_config);

static ssize_t ppv4_clock_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct epu_device *edev = dev_get_drvdata(dev);
	struct regmap *top_regmap = edev->top_regmap;
	u32 val, stat;
	int ret;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

	if (val > 1)
		return -EINVAL;

	regmap_read(top_regmap, CHIP_TOP_ECO_SPARE_0, &stat);

	if ((stat & BIT(0)) == val)
		return count;

	regmap_update_bits(top_regmap, CHIP_TOP_ECO_SPARE_0, BIT(0), val);

	return count;
}

static ssize_t ppv4_clock_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct epu_device *edev = dev_get_drvdata(dev);
	u32 val;

	regmap_read(edev->top_regmap, CHIP_TOP_ECO_SPARE_0, &val);

	return sprintf(buf, "%lu\n", val & BIT(0));
}
static DEVICE_ATTR_RW(ppv4_clock);

static struct attribute *epu_telemetry_attrs[] = {
	&dev_attr_core0_C7_cnt.attr,
	&dev_attr_core1_C7_cnt.attr,
	&dev_attr_core2_C7_cnt.attr,
	&dev_attr_core3_C7_cnt.attr,
	&dev_attr_adp_sw_dvfs_req_cnt.attr,
	&dev_attr_adp_sw_dvfs_grant_cnt.attr,
	&dev_attr_adp_hw_dvfs_req_cnt.attr,
	&dev_attr_adp_hw_dvfs_grant_cnt.attr,
	&dev_attr_adp_dvfs_level_0_residency.attr,
	&dev_attr_adp_dvfs_level_1_residency.attr,
	&dev_attr_adp_dvfs_level_2_residency.attr,
	&dev_attr_s0i3_pattern_match_cnt.attr,
	&dev_attr_s0i3_pattern_mismatch_cnt.attr,
	&dev_attr_s0i3_pattern_residency_cnt.attr,
	NULL,
};

static const struct attribute_group epu_telemetry_attr_group = {
	.name = "telemetry",
	.attrs = epu_telemetry_attrs,
};

static struct attribute *epu_adp_attrs[] = {
	&dev_attr_adp_scaling.attr,
	&dev_attr_adp_dvfs_level.attr,
	&dev_attr_adp_level_config.attr,
	&dev_attr_ppv4_clock.attr,
	&dev_attr_dstate0.attr,
	&dev_attr_dstate1.attr,
	&dev_attr_dstate2.attr,
	&dev_attr_dstate3.attr,
	NULL,
};

static const struct attribute_group epu_adp_attr_group = {
	.attrs = epu_adp_attrs,
};

static const struct attribute_group *epu_attr_groups[] = {
	&epu_adp_attr_group,
	&epu_telemetry_attr_group,
	NULL,
};

static void epu_init(struct epu_device *edev)
{
	struct regmap *gen_regmap = edev->gen_regmap;
	struct regmap *aon_regmap = edev->aon_regmap;
	unsigned long rate = clk_get_rate(edev->clk);
	u64 val;

	/* Set 2ms to timeout value for ACK from PMIC to PUNIT */
	regmap_write(aon_regmap, EPU_PMIC_CPU0_TIMEOUT, (rate * 2) / 1000);
	regmap_write(aon_regmap, EPU_PMIC_CPU1_TIMEOUT, (rate * 2) / 1000);
	regmap_write(aon_regmap, EPU_PMIC_ADP_TIMEOUT, (rate * 2) / 1000);

	regmap_write(aon_regmap, EPU_PMIC_REQ_TIME, (rate * 20) / 1000);
	regmap_write(aon_regmap, EPU_POWERGOOD_TIMEOUT, (rate * 20) / 1000);

	/* PMIC_PG + 10ms = 1510ms */
	regmap_write(aon_regmap, EPU_POWERGOOD_TIMEOUT_2, (rate * 1510) / 1000);

	regmap_update_bits(aon_regmap, EPU_DEV_PM_CTL_0, BIT(25), BIT(25));

	epu_enable_soc_opc(gen_regmap);
	epu_set_opcs(edev, true);

	val = 0x0800;
	regmap_bulk_write(gen_regmap, XPCS5_PDC_POWER_DOWN_TIMER0,
			  &val, REG_NUM);

	/* delay the power switch ON request to isolation disable
	 * in PON power domain, according to worst-case delays
	 * implemented in the netlist.
	 */
	val = 0x2aULL << 56;
	regmap_bulk_write(gen_regmap, EPU_PON_PDC_PWR_UP_TMR1, &val, REG_NUM);

	regmap_write(aon_regmap, EPU_PMIC_POWERGOOD_TOGGLE,
		     EPU_PMIC_POWERGOOD_TOGGLE_CLR);
}

static const struct regmap_config gen_regmap_config = {
	.name		= "epu_gen",
	.reg_bits       = 32,
	.reg_stride     = 4,
	.val_bits       = 32,
};

static int epu_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct device_node *node;
	struct resource *res;
	void __iomem *io_base;
	const struct epu_data *soc_data;
	struct epu_device *edev;
	int ret, irq0, irq1;

	if (!np) {
		dev_err(dev, "device tree node not found\n");
		return -ENODEV;
	}

	soc_data = of_device_get_match_data(dev);
	if (WARN_ON(!soc_data))
		return -EINVAL;

	edev = devm_kzalloc(dev, sizeof(*edev) +
			    soc_data->num_domains *
			    sizeof(edev->domains[0]), GFP_KERNEL);
	if (!edev)
		return -ENOMEM;

	edev->soc_data = soc_data;
	edev->dev = dev;
	raw_spin_lock_init(&edev->i2c_lock);

	edev->aon_regmap = device_node_to_regmap(np);
	if (IS_ERR(edev->aon_regmap)) {
		ret = PTR_ERR(edev->aon_regmap);
		dev_err(dev, "failed to init epu_aon register map: %d\n", ret);
		return ret;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "epu_gen");
	io_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(io_base))
		return PTR_ERR(io_base);

#if IS_ENABLED(CONFIG_REGMAP_ICC)
		edev->gen_regmap = devm_regmap_init_icc(dev, io_base,
							res->start,
							&gen_regmap_config);
#else
		edev->gen_regmap = devm_regmap_init_mmio(dev, io_base,
							 &gen_regmap_config);
#endif

	if (IS_ERR(edev->gen_regmap)) {
		ret = PTR_ERR(edev->gen_regmap);
		dev_err(dev, "failed to init epu_gen register map: %d\n", ret);
		return ret;
	}

	edev->top_regmap = syscon_regmap_lookup_by_phandle(dev->of_node,
							   "mxl,top-syscon");
	if (IS_ERR(edev->top_regmap)) {
		ret = PTR_ERR(edev->top_regmap);
		dev_err(dev, "failed to get top register map: %d\n", ret);
		return ret;
	}

	irq0 = platform_get_irq_byname(pdev, "epu_irq0");
	if (irq0 < 0) {
		dev_err(dev, "failed to get epu irq0\n");
		return irq0;
	}

	irq1 = platform_get_irq_byname(pdev, "epu_irq1");
	if (irq1 < 0) {
		dev_err(dev, "failed to get epu irq1\n");
		return irq1;
	}

	ret = devm_request_threaded_irq(dev, irq0, epu_interrupt_0,
					epu_timeout_thread_fn, IRQF_ONESHOT,
					"epu_intr0", edev);
	if (ret < 0) {
		dev_err(dev, "can't register ISR for IRQ %u (ret=%i)\n",
			irq0, ret);
		return ret;
	}
	edev->irq[0] = irq0;

	ret = devm_request_irq(dev, irq1, epu_interrupt_1, 0,
			       "epu_intr1", edev);
	if (ret < 0) {
		dev_err(dev, "can't register ISR for IRQ %u (ret=%i)\n",
			irq1, ret);
		return ret;
	}
	edev->irq[1] = irq1;

	edev->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(edev->clk)) {
		ret = PTR_ERR(edev->clk);
		dev_err(dev, "failed to get clock: %d\n", ret);
		return ret;
	}

	ret = devm_device_add_groups(dev, epu_attr_groups);
	if (ret) {
		dev_err(dev, "unable to create sysfs version group\n");
		return ret;
	}

	ret = clk_prepare_enable(edev->clk);
	if (ret < 0) {
		dev_err(dev, "failed to enable clock\n");
		return ret;
	}

	epu_hw_init(edev);

	for_each_available_child_of_node(np, node) {
		ret = epu_add_one_pm_domain(edev, node, NULL);
		if (ret) {
			dev_err(dev, "failed to handle node %s: %d\n",
				node->name, ret);
			of_node_put(node);
			goto err_out;
		}
	}

	platform_set_drvdata(pdev, edev);

	edev->blocking_notifier.notifier_call = epu_blocking_notify;
	blocking_notifier_chain_register(&epu_blocking_chain,
					 &edev->blocking_notifier);

	edev->raw_notifier.notifier_call = epu_raw_notify;
	raw_notifier_chain_register(&epu_raw_chain, &edev->raw_notifier);

	epu_init(edev);
#ifdef CONFIG_SUSPEND
	suspend_set_ops(&epu_suspend_ops);
#endif
	ret = epu_pd_debugfs_init(edev);
	if (ret)
		goto err_out;

	epu_dev = edev;
	dev_info(dev, "EPU init done.\n");

	return 0;

err_out:
	clk_disable_unprepare(edev->clk);
	epu_pm_domain_cleanup(edev);
	return ret;
}

static void epu_shutdown(struct platform_device *pdev)
{
	struct epu_device *edev = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < 2; i++)
		devm_free_irq(edev->dev, edev->irq[i], edev);

	for (i = 0; i < 2; i++) {
		devm_regulator_put(edev->vdd_cpu[i]);
		edev->vdd_cpu[i] = 0;
	}

	devm_regulator_put(edev->vdd_adp);
	edev->vdd_adp = 0;

	blocking_notifier_chain_unregister(&epu_blocking_chain,
					 &edev->blocking_notifier);
	raw_notifier_chain_unregister(&epu_raw_chain, &edev->raw_notifier);

	/* Hold HW lock to block EPU HW access */
	if (spinlock_get_i2c_hw_lock(edev))
		dev_err(edev->dev, "EPU shutdown: Failed to get I2C HW lock\n");
}

/*
 * Refer to table 44 in MAS and wakeup refer to HAS table 55
 * Subject to change
 */
static const struct epu_domain_info lgm_pm_domains[] = {
	/* group 0 */
	[LGM_EPU_PD_PPV4]	= DOMAIN(LGM_EPU_PD_PPV4, 0, EPU_D1,
					 EPU_D2, 0, false),
	[LGM_EPU_PD_GSW]	= DOMAIN(LGM_EPU_PD_GSW, 0, EPU_D1,
					 EPU_D2, 2, false),
	[LGM_EPU_PD_CQM]	= DOMAIN(LGM_EPU_PD_CQM, 0, EPU_D1,
					 EPU_D2, 4, false),
	[LGM_EPU_PD_VOICE]	= DOMAIN(LGM_EPU_PD_VOICE, 0, EPU_D3,
					 EPU_D0, 6, true),
	[LGM_EPU_PD_PON]	= DOMAIN(LGM_EPU_PD_PON, 0, EPU_D3,
					 EPU_D3, 8, true),
	[LGM_EPU_PD_HSIO1]	= DOMAIN(LGM_EPU_PD_HSIO1, 0, EPU_D3,
					 EPU_D3, 10, true),
	[LGM_EPU_PD_HSIO2]	= DOMAIN(LGM_EPU_PD_HSIO2, 0, EPU_D3,
					 EPU_D0, 12, true),
	[LGM_EPU_PD_HSIO3]	= DOMAIN(LGM_EPU_PD_HSIO3, 0, EPU_D3,
					 EPU_D3, 14, true),
	[LGM_EPU_PD_HSIO4]	= DOMAIN(LGM_EPU_PD_HSIO4, 0, EPU_D3,
					 EPU_D3, 16, true),
	[LGM_EPU_PD_TEP]	= DOMAIN(LGM_EPU_PD_TEP, 0, EPU_D1,
					 EPU_D1, 18, false),
	[LGM_EPU_PD_EIP197]	= DOMAIN(LGM_EPU_PD_EIP197, 0, EPU_D1,
					 EPU_D1, 20, false),
	[LGM_EPU_PD_VAULT130]	= DOMAIN(LGM_EPU_PD_VAULT130, 0, EPU_D1,
					 EPU_D1, 22, false),
	[LGM_EPU_PD_TOE]	= DOMAIN(LGM_EPU_PD_TOE, 0, EPU_D1,
					 EPU_D1, 26, false),
	[LGM_EPU_PD_SDXC]	= DOMAIN(LGM_EPU_PD_SDXC, 0, EPU_D1,
					 EPU_D1, 28, true),
	[LGM_EPU_PD_EMMC]	= DOMAIN(LGM_EPU_PD_EMMC, 0, EPU_D1,
					 EPU_D1, 30, false),

	/* group 1 */
	[LGM_EPU_PD_DDR_CTL0]	= DOMAIN(LGM_EPU_PD_DDR_CTL0, 1, EPU_D0,
					 EPU_D0, 0, false),
	[LGM_EPU_PD_RTIT]	= DOMAIN(LGM_EPU_PD_RTIT, 1, EPU_D1,
					 EPU_D1, 2, false),
	[LGM_EPU_PD_QSPI]	= DOMAIN(LGM_EPU_PD_QSPI, 1, EPU_D1,
					 EPU_D1, 4, false),
	[LGM_EPU_PD_ROM]	= DOMAIN(LGM_EPU_PD_ROM, 1, EPU_D1,
					 EPU_D1, 6, false),
	[LGM_EPU_PD_SSB]	= DOMAIN(LGM_EPU_PD_SSB, 1, EPU_D1,
					 EPU_D1, 8, false),
	[LGM_EPU_PD_USB1]	= DOMAIN(LGM_EPU_PD_USB1, 1, EPU_D1,
					 EPU_D1, 10, true),
	[LGM_EPU_PD_USB2]	= DOMAIN(LGM_EPU_PD_USB2, 1, EPU_D1,
					 EPU_D1, 12, true),
	[LGM_EPU_PD_ASC0]	= DOMAIN(LGM_EPU_PD_ASC0, 1, EPU_D1,
					 EPU_D1, 14, false),
	[LGM_EPU_PD_ASC1]	= DOMAIN(LGM_EPU_PD_ASC1, 1, EPU_D1,
					 EPU_D1, 16, false),
	[LGM_EPU_PD_ASC2]	= DOMAIN(LGM_EPU_PD_ASC2, 1, EPU_D1,
					 EPU_D1, 18, false),
	[LGM_EPU_PD_SSC0]	= DOMAIN(LGM_EPU_PD_SSC0, 1, EPU_D1,
					 EPU_D1, 20, false),
	[LGM_EPU_PD_SSC1]	= DOMAIN(LGM_EPU_PD_SSC1, 1, EPU_D1,
					 EPU_D1, 22, false),
	[LGM_EPU_PD_SSC2]	= DOMAIN(LGM_EPU_PD_SSC2, 1, EPU_D1,
					 EPU_D1, 24, false),
	[LGM_EPU_PD_SSC3]	= DOMAIN(LGM_EPU_PD_SSC3, 1, EPU_D1,
					 EPU_D1, 26, false),
	[LGM_EPU_PD_I2C1]	= DOMAIN(LGM_EPU_PD_I2C1, 1, EPU_D1,
					 EPU_D1, 28, false),
	[LGM_EPU_PD_I2C2]	= DOMAIN(LGM_EPU_PD_I2C2, 1, EPU_D1,
					 EPU_D1, 30, false),

	/* group 2 */
	[LGM_EPU_PD_I2C3]	= DOMAIN(LGM_EPU_PD_I2C3, 2, EPU_D1,
					 EPU_D1, 0, false),
	[LGM_EPU_PD_LEDC0]	= DOMAIN(LGM_EPU_PD_LEDC0, 2, EPU_D1,
					 EPU_D1, 2, false),
	[LGM_EPU_PD_PCM0]	= DOMAIN(LGM_EPU_PD_PCM0, 2, EPU_D1,
					 EPU_D1, 4, false),
	[LGM_EPU_PD_EBU]	= DOMAIN(LGM_EPU_PD_EBU, 2, EPU_D1,
					 EPU_D1, 6, false),
	[LGM_EPU_PD_I2S0]	= DOMAIN(LGM_EPU_PD_I2S0, 2, EPU_D1,
					 EPU_D1, 8, false),
	[LGM_EPU_PD_I2S1]	= DOMAIN(LGM_EPU_PD_I2S1, 2, EPU_D1,
					 EPU_D1, 10, false),
	[LGM_EPU_PD_DDR_CTL1]	= DOMAIN(LGM_EPU_PD_DDR_CTL1, 2, EPU_D0,
					 EPU_D0, 12, false),
	[LGM_EPU_PD_EM4]	= DOMAIN(LGM_EPU_PD_EM4, 2, EPU_D1,
					 EPU_D1, 14, false),
	[LGM_EPU_PD_PCM1]	= DOMAIN(LGM_EPU_PD_PCM1, 2, EPU_D1,
					 EPU_D1, 16, false),
	[LGM_EPU_PD_PCM2]	= DOMAIN(LGM_EPU_PD_PCM2, 2, EPU_D1,
					 EPU_D1, 18, false),
	[LGM_EPU_PD_XPCS5]	= DOMAIN(LGM_EPU_PD_XPCS5, 2, EPU_D1,
					 EPU_D1, 20, false),
	[LGM_EPU_PD_LEDC1]	= DOMAIN(LGM_EPU_PD_LEDC1, 2, EPU_D1,
					 EPU_D1, 22, false),
	[LGM_EPU_PD_PCIE30]     = DOMAIN(LGM_EPU_PD_PCIE30, 2, EPU_D1,
					 EPU_D1, 24, false),
	[LGM_EPU_PD_PCIE40]     = DOMAIN(LGM_EPU_PD_PCIE40, 2, EPU_D1,
					 EPU_D1, 26, false),
	[LGM_EPU_PD_SATA0]      = DOMAIN(LGM_EPU_PD_SATA0, 2, EPU_D1,
					 EPU_D1, 28, false),
	[LGM_EPU_PD_SATA1]      = DOMAIN(LGM_EPU_PD_SATA1, 2, EPU_D1,
					 EPU_D1, 30, false),

	/* group 3
	 * pice, sata and xpcs should use hsio domain,
	 * pcie, sata and xpcs domains are only for internal use
	 */
	[LGM_EPU_PD_PCIE31]     = DOMAIN(LGM_EPU_PD_PCIE31, 3, EPU_D1,
					 EPU_D1, 0, false),
	[LGM_EPU_PD_PCIE41]     = DOMAIN(LGM_EPU_PD_PCIE41, 3, EPU_D1,
					 EPU_D1, 2, false),
	[LGM_EPU_PD_SATA2]      = DOMAIN(LGM_EPU_PD_SATA2, 3, EPU_D1,
					 EPU_D1, 4, false),
	[LGM_EPU_PD_SATA3]      = DOMAIN(LGM_EPU_PD_SATA3, 3, EPU_D1,
					 EPU_D1, 6, false),
	[LGM_EPU_PD_PCIE10]     = DOMAIN(LGM_EPU_PD_PCIE10, 3, EPU_D1,
					 EPU_D1, 8, false),
	[LGM_EPU_PD_PCIE20]     = DOMAIN(LGM_EPU_PD_PCIE20, 3, EPU_D1,
					 EPU_D1, 10, false),
	[LGM_EPU_PD_PCIE11]     = DOMAIN(LGM_EPU_PD_PCIE11, 3, EPU_D1,
					 EPU_D1, 12, false),
	[LGM_EPU_PD_PCIE21]     = DOMAIN(LGM_EPU_PD_PCIE21, 3, EPU_D1,
					 EPU_D1, 14, false),

	[LGM_EPU_PD_XPCS10]     = DOMAIN(LGM_EPU_PD_XPCS10, 2, EPU_D1,
					 EPU_D1, 24, false),
	[LGM_EPU_PD_XPCS11]     = DOMAIN(LGM_EPU_PD_XPCS11, 2, EPU_D1,
					 EPU_D1, 26, false),
	[LGM_EPU_PD_XPCS20]     = DOMAIN(LGM_EPU_PD_XPCS20, 3, EPU_D1,
					 EPU_D1, 0, false),
	[LGM_EPU_PD_XPCS21]     = DOMAIN(LGM_EPU_PD_XPCS21, 3, EPU_D1,
					 EPU_D1, 2, false),
};

static const struct epu_data lgm_domain_info = {
	.num_domains = ARRAY_SIZE(lgm_pm_domains),
	.domain_info = lgm_pm_domains,
};

static const struct of_device_id epu_pd_dt_ids[] = {
	{.compatible = "mxl,lgm-epu", .data = &lgm_domain_info},
	{ },
};

static struct platform_driver epu_driver = {
	.probe = epu_probe,
	.shutdown = epu_shutdown,
	.driver = {
		.name = "mxl-epu",
		.of_match_table = of_match_ptr(epu_pd_dt_ids),
		/*
		 * We can't forcibly eject devices form power domain,
		 * so we can't really remove power domains once they
		 * were added.
		 */
		.suppress_bind_attrs = true,
	},
};

static int __init epu_drv_register(void)
{
	return platform_driver_register(&epu_driver);
}
fs_initcall(epu_drv_register);

