// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
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
#include <linux/interrupt.h>
#include <linux/mfd/syscon.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define RCU_RST_REQ_ETHNOC	0x00000070
#define RCU_RST_REQ_PPNOC	0x00000080

#define RCU_RST_STAT_ETHNOC	0x00000074
#define RCU_RST_STAT_PPNOC	0x00000084

#define ETHNOC_ADP_SUP_RST_EN	BIT(30)
#define ETHNOC_ADP_SUP_RST_DIS	0x0
#define ETHNOC_ADP_SUP_RST_MASK BIT(30)

#define EPU_DEV_PM_CTL_0	0x24
#define PM_CTL_0_PPV4_MSK	GENMASK(1, 0)
#define PM_CTL_0_PPV4_D1	0x1
#define PM_CTL_0_GSWIP_MSK	GENMASK(3, 2)
#define PM_CTL_0_GSWIP_D1	(0x1 << 2)
#define PM_CTL_0_CQM_MSK	GENMASK(5, 4)
#define PM_CTL_0_CQM_D1		(0x1 << 4)

#define EPU_DEV_PM_CTL_1	0x28
#define PM_CTL_1_USB1_MSK	GENMASK(11, 10)
#define PM_CTL_1_USB1_D1	(0x1 << 10)
#define PM_CTL_1_USB2_MSK	GENMASK(13, 12)
#define PM_CTL_1_USB2_D1	(0x1 << 12)

#define EPU_DEV_PM_CTL_2	0x2c
#define PM_CTL_2_ARC_EM4_MSK	GENMASK(15, 14)
#define PM_CTL_2_ARC_EM4_D1	(0x1 << 14)
#define PM_CTL_2_XPCS5_MSK	GENMASK(21, 20)
#define PM_CTL_2_XPCS5_D1	(0x1 << 20)

#define EPU_DEV_PM_ST_0		0x34
#define EPU_DEV_PM_ST_1		0x38
#define EPU_DEV_PM_ST_2		0x3c

#define EPU_ADP_PM_CTL		0x1c
#define EPU_ADP_HW_DFS_MSK	BIT(0)
#define EPU_ADP_HW_DFS_EN	0x1
#define EPU_ADP_HW_DFS_DIS	0x0
#define EPU_ADP_DFS_LVL_MSK	GENMASK(3, 1)

#define EPU_ADP_PM_ST		0x20
#define EPU_ADP_SW_DFS_LVL_MSK	GENMASK(5, 3)

#define CGU_CGU_CLKGSR3		0x330

#define TOP_NOC_IMEMAXI		0x2308
#define TOP_NOC_IE197W		0x6308
#define TOP_NOC_IE197R		0x6408
#define TOP_NOC_ITOET0		0x6a08
#define TOP_NOC_ITOEDR0		0x6b08
#define TOP_NOC_ITOEDR1		0x6d08
#define TOP_NOC_INSP20		0x18408
#define TOP_NOC_INSP30		0x18508
#define TOP_NOC_INSP50		0x18608
#define TOP_NOC_INSP60		0x18708

#define ETH_NOC_IDMAR1		0x2508
#define ETH_NOC_IDMAT2		0x6108
#define ETH_NOC_IDMAT3		0x6308
#define ETH_NOC_IDMAT1		0x6b08
#define ETH_NOC_IUSB30R		0x7108
#define ETH_NOC_IUSB30W		0x7208
#define ETH_NOC_IUSB31R		0x7308
#define ETH_NOC_IUSB31W		0x7408

#define FRW_INT_EN		0x800
#define FRW_INT_MASK		0x804

#define TIMEOUT_SLEEP_US	200
#define TIMEOUT_ALL_US		200000

enum adp_level {
	ADP_DVFS_LVL_0,
	ADP_DVFS_LVL_1,
	ADP_DVFS_LVL_2,
	ADP_DVFS_LVL_MAX
};

struct adp_device {
	struct device *dev;
	struct regmap *ethfw_addr;
	struct regmap *sec_reg;
	struct regmap *top_reg;
	struct regmap *rcu_reg;
	struct regmap *epu_reg;
	struct regmap *cgu_reg;
	struct regmap *top_noc_reg;
	struct regmap *eth_noc_reg;
};

static void
adp_reg_config(struct regmap *regmap, u32 req, u32 stat, u32 mask, u32 data)
{
	u32 val;
	int ret;

	regmap_update_bits(regmap, req, mask, data);

	ret = regmap_read_poll_timeout(regmap, stat, val, (val & mask) == data,
				       TIMEOUT_SLEEP_US, TIMEOUT_ALL_US);
	if (ret)
		pr_warn("ADP/EPU reg %x config timeout orig %x return %x\n",
			req, data, (val & mask));
}

static void epu_set_pm_d1(struct adp_device *adp)
{
	struct regmap *regmap = adp->epu_reg;
	u32 val, mask;

	if (IS_ENABLED(CONFIG_REGMAP_ICC))
		return;

	/* GSWIP first, then PPv4, finally CQM */
	adp_reg_config(regmap, EPU_DEV_PM_CTL_0, EPU_DEV_PM_ST_0,
		       PM_CTL_0_GSWIP_MSK, PM_CTL_0_GSWIP_D1);

	adp_reg_config(regmap, EPU_DEV_PM_CTL_0, EPU_DEV_PM_ST_0,
		       PM_CTL_0_PPV4_MSK, PM_CTL_0_PPV4_D1);

	adp_reg_config(regmap, EPU_DEV_PM_CTL_0, EPU_DEV_PM_ST_0,
		       PM_CTL_0_CQM_MSK, PM_CTL_0_CQM_D1);

	/* USB1/USB2 */
	val = PM_CTL_1_USB1_D1 | PM_CTL_1_USB2_D1;
	mask = PM_CTL_1_USB1_MSK | PM_CTL_1_USB2_MSK;
	adp_reg_config(regmap, EPU_DEV_PM_CTL_1, EPU_DEV_PM_ST_1, mask, val);

	/* XPCS5 and ARC EM4 must be written together */
	val = PM_CTL_2_ARC_EM4_D1 | PM_CTL_2_XPCS5_D1;
	mask = PM_CTL_2_XPCS5_MSK | PM_CTL_2_ARC_EM4_MSK;
	adp_reg_config(regmap, EPU_DEV_PM_CTL_2, EPU_DEV_PM_ST_2, mask, val);
}

static void epu_set_pm_d0(struct adp_device *adp)
{
	struct regmap *regmap = adp->epu_reg;
	u32 mask;

	/* CQM -> PPv4 -> GSWIP */
	adp_reg_config(regmap, EPU_DEV_PM_CTL_0, EPU_DEV_PM_ST_0,
		       PM_CTL_0_CQM_MSK, 0);

	/* PPv4 always has a timeout */
	adp_reg_config(regmap, EPU_DEV_PM_CTL_0, EPU_DEV_PM_ST_0,
		       PM_CTL_0_PPV4_MSK, 0);

	adp_reg_config(regmap, EPU_DEV_PM_CTL_0, EPU_DEV_PM_ST_0,
		       PM_CTL_0_GSWIP_MSK, 0);

	/* USB1/USB2 */
	mask = PM_CTL_1_USB1_MSK | PM_CTL_1_USB2_MSK;
	adp_reg_config(regmap, EPU_DEV_PM_CTL_1, EPU_DEV_PM_ST_1, mask, 0);

	/* VPN ARC EM4, also timeout, must together with XPCS5 */
	mask = PM_CTL_2_ARC_EM4_MSK | PM_CTL_2_XPCS5_MSK;
	adp_reg_config(regmap, EPU_DEV_PM_CTL_2, EPU_DEV_PM_ST_2, mask, 0);
}

static void init_ethnoc_fw(struct adp_device *adp)
{
	struct regmap *fw_addr = adp->ethfw_addr;
	struct regmap *sec_reg = adp->sec_reg;
	u32 x, i;
	const u32 idx[] = {0x9, 0x19, 0x49, 0x59, 0x7f};

	/* master access */
	for (i = 0; i < 64; i++) {
		if (i < 5)
			x = idx[i];
		else if (i >= 12 && i <= 46)
			regmap_read(sec_reg, i << 2, &x);
		else
			continue;

		regmap_write(fw_addr, i << 2, 0xffffff);
		regmap_write(fw_addr, 0x100 + (i << 2), 0x0);
		regmap_write(fw_addr, 0x200 + (i << 2), 0xe);
		regmap_write(fw_addr, 0x300 + (i << 2), x);
		regmap_write(fw_addr, 0x404 + ((x >> 1) << 2), x);
	}

	/* lock all firewalls */
	regmap_write(fw_addr, 0x400, 0x1);
}

#define CHIP_TOP_ECO_SPARE_0		0x1c0
#define EC0_SW_PPV4_DVFS_LVL_MSK	GENMASK(10, 8)

static int adp_set_adp_lvl(struct adp_device *adp, u32 lvl)
{
	struct regmap *epu_reg = adp->epu_reg;
	struct regmap *top_reg = adp->top_reg;
	u32 stat;
	int ret = 0;

	regmap_update_bits(epu_reg, EPU_ADP_PM_CTL,
				   EPU_ADP_HW_DFS_MSK, EPU_ADP_HW_DFS_DIS);
	regmap_update_bits(top_reg, CHIP_TOP_ECO_SPARE_0, BIT(0), 0);

	regmap_update_bits(epu_reg, EPU_ADP_PM_CTL,
			   EPU_ADP_DFS_LVL_MSK,
			   FIELD_PREP(EPU_ADP_DFS_LVL_MSK, lvl));

	ret = regmap_read_poll_timeout(epu_reg, EPU_ADP_PM_ST, stat,
				       FIELD_GET(EPU_ADP_SW_DFS_LVL_MSK, stat)
				       == lvl,
				       TIMEOUT_SLEEP_US, TIMEOUT_ALL_US);

	regmap_update_bits(top_reg, CHIP_TOP_ECO_SPARE_0,
			   EC0_SW_PPV4_DVFS_LVL_MSK,
			   FIELD_PREP(EC0_SW_PPV4_DVFS_LVL_MSK, lvl));

	return ret;
}

static void adp_enter_d1(struct adp_device *adp, u32 *eth, u32 *pp)
{
	int i;
	struct regmap *rcu_reg = adp->rcu_reg;
	struct regmap *ethfw_addr = adp->ethfw_addr;

	if (!eth || !pp)
		return;

	if (!IS_ENABLED(CONFIG_REGMAP_ICC)) {
		regmap_write(ethfw_addr, FRW_INT_EN, 0x0);
		regmap_write(ethfw_addr, FRW_INT_MASK, 0x1f);
	}

	/* Back up ethnoc and ppnoc reset settings for D0 usage */
	regmap_read(rcu_reg, RCU_RST_STAT_ETHNOC, eth);

	/*
	 * WAR: RST_STAT_PPNOC is the reset status. 1st read will return
	 * a value of 1 if the PPNOC is in reset(default case). Since it is
	 * already in reset, the record value should not be 1, read will clear
	 * the bit, 2nd read return to 0 which matches the hw behavior
	 */
	for (i = 0; i < 2; i++)
		regmap_read(rcu_reg, RCU_RST_STAT_PPNOC, pp);

	/* Ethnoc components out of reset */
	adp_reg_config(rcu_reg, RCU_RST_REQ_ETHNOC, RCU_RST_STAT_ETHNOC,
		       0x7fc39b0f, 0);

	/* PPnoc components out of reset */
	adp_reg_config(rcu_reg, RCU_RST_REQ_PPNOC, RCU_RST_STAT_PPNOC,
		       0x80000000, 0);

	epu_set_pm_d1(adp);

	/* Assert ADP Super Reset */
	adp_reg_config(rcu_reg, RCU_RST_REQ_ETHNOC, RCU_RST_STAT_ETHNOC,
		       ETHNOC_ADP_SUP_RST_MASK, ETHNOC_ADP_SUP_RST_EN);
}

static void adp_enter_d0(struct adp_device *adp, u32 eth, u32 pp)
{
	struct regmap *rcu_reg = adp->rcu_reg;
	struct regmap *cgu_reg = adp->cgu_reg;
	struct regmap *ethfw_addr = adp->ethfw_addr;
	u32 val, stat;
	int ret;

	epu_set_pm_d0(adp);

	/* EPU D0 will change CGU register status, sanity check */
	val = 0xc000073d;
	ret = regmap_read_poll_timeout(cgu_reg, CGU_CGU_CLKGSR3, stat,
				       (stat & val) == val,
				       TIMEOUT_SLEEP_US, TIMEOUT_ALL_US);
	if (ret)
		dev_warn(adp->dev, "cgu enable timeout\n");

	/* Deassert ADP Super Reset */
	adp_reg_config(rcu_reg, RCU_RST_REQ_ETHNOC, RCU_RST_STAT_ETHNOC,
		       ETHNOC_ADP_SUP_RST_MASK, ETHNOC_ADP_SUP_RST_DIS);

	if (!IS_ENABLED(CONFIG_REGMAP_ICC))
		init_ethnoc_fw(adp);

	/* Restore Ethnoc reset settings */
	adp_reg_config(rcu_reg, RCU_RST_REQ_ETHNOC, RCU_RST_STAT_ETHNOC,
		       0x3fc39b0f, eth & 0x3fc39b0f);

	/* Restore PPnoc reset settings */
	adp_reg_config(rcu_reg, RCU_RST_REQ_PPNOC, RCU_RST_STAT_PPNOC,
		       0x80000000, pp & 0x80000000);

	if (!IS_ENABLED(CONFIG_REGMAP_ICC)) {
		regmap_write(ethfw_addr, FRW_INT_EN, 0x1f);
		regmap_write(ethfw_addr, FRW_INT_MASK, 0x0);
	}
}

static void eth_top_noc_setting_save(struct adp_device *adp,
				     u32 *top_noc, u32 *eth_noc)
{
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_IMEMAXI, &top_noc[0], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_IE197W, &top_noc[5], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_IE197R, &top_noc[10], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_ITOET0, &top_noc[15], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_ITOEDR0, &top_noc[20], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_ITOEDR1, &top_noc[25], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_INSP20, &top_noc[30], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_INSP30, &top_noc[35], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_INSP50, &top_noc[40], 5);
	regmap_bulk_read(adp->top_noc_reg, TOP_NOC_INSP60, &top_noc[45], 5);

	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IDMAR1, &eth_noc[0], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IDMAT2, &eth_noc[5], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IDMAT3, &eth_noc[10], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IDMAT1, &eth_noc[15], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IUSB30R, &eth_noc[20], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IUSB30W, &eth_noc[25], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IUSB31R, &eth_noc[30], 5);
	regmap_bulk_read(adp->eth_noc_reg, ETH_NOC_IUSB31W, &eth_noc[35], 5);
}

static void eth_top_noc_setting_restore(struct adp_device *adp,
					u32 *top_noc, u32 *eth_noc)
{
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_IMEMAXI, &top_noc[0], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_IE197W, &top_noc[5], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_IE197R, &top_noc[10], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_ITOET0, &top_noc[15], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_ITOEDR0, &top_noc[20], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_ITOEDR1, &top_noc[25], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_INSP20, &top_noc[30], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_INSP30, &top_noc[35], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_INSP50, &top_noc[40], 5);
	regmap_bulk_write(adp->top_noc_reg, TOP_NOC_INSP60, &top_noc[45], 5);

	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IDMAR1, &eth_noc[0], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IDMAT2, &eth_noc[5], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IDMAT3, &eth_noc[10], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IDMAT1, &eth_noc[15], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IUSB30R, &eth_noc[20], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IUSB30W, &eth_noc[25], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IUSB31R, &eth_noc[30], 5);
	regmap_bulk_write(adp->eth_noc_reg, ETH_NOC_IUSB31W, &eth_noc[35], 5);
}

static void adp_reset(struct adp_device *adp)
{
	u32 eth, pp, top_noc[50], eth_noc[40];

	dev_info(adp->dev, "Adp reset ...\n");

	if (!IS_ENABLED(CONFIG_REGMAP_ICC))
		eth_top_noc_setting_save(adp, &top_noc[0], &eth_noc[0]);

	adp_enter_d1(adp, &eth, &pp);

	adp_enter_d0(adp, eth, pp);

	if (!IS_ENABLED(CONFIG_REGMAP_ICC))
		eth_top_noc_setting_restore(adp, &top_noc[0], &eth_noc[0]);
}

static void adp_shutdown(struct adp_device *adp)
{
	dev_info(adp->dev, "Adp shutdown ...\n");

	adp_enter_d1(adp, NULL, NULL);
}

static const struct regmap_config adp_eth_config = {
	.name		= "eth",
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
};

static const struct regmap_config adp_sec_config = {
	.name		= "sec",
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
};

static const struct regmap_config adp_top_noc_config = {
	.name		= "top_noc",
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
};

static const struct regmap_config adp_eth_noc_config = {
	.name		= "eth_noc",
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
};

static int adp_reset_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct adp_device *adp;
	struct resource *res;
	void __iomem *io_base;
	int ret;

	adp = devm_kzalloc(dev, sizeof(*adp), GFP_KERNEL);
	if (!adp)
		return -ENOMEM;
	adp->dev = dev;
	platform_set_drvdata(pdev, adp);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "eth_fw");
	io_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(io_base))
		return PTR_ERR(io_base);

#if IS_ENABLED(CONFIG_REGMAP_ICC)
		adp->ethfw_addr = devm_regmap_init_icc(dev, io_base,
						       res->start,
						       &adp_eth_config);
#else
		adp->ethfw_addr = devm_regmap_init_mmio(dev, io_base,
							&adp_eth_config);
#endif
	if (IS_ERR(adp->ethfw_addr)) {
		ret = PTR_ERR(adp->ethfw_addr);
		dev_err(dev, "failed to get ethernet fw address: %d\n", ret);
		return ret;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "top_noc");
	io_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(io_base))
		return PTR_ERR(io_base);

	adp->top_noc_reg = devm_regmap_init_mmio(dev, io_base,
						 &adp_top_noc_config);
	if (IS_ERR(adp->top_noc_reg)) {
		ret = PTR_ERR(adp->top_noc_reg);
		dev_err(dev, "failed to get top noc address: %d\n", ret);
		return ret;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "eth_noc");
	io_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(io_base))
		return PTR_ERR(io_base);

	adp->eth_noc_reg = devm_regmap_init_mmio(dev, io_base,
						 &adp_eth_noc_config);
	if (IS_ERR(adp->eth_noc_reg)) {
		ret = PTR_ERR(adp->eth_noc_reg);
		dev_err(dev, "failed to get top noc address: %d\n", ret);
		return ret;
	}

	adp->sec_reg = syscon_regmap_lookup_by_phandle(dev->of_node,
						       "intel,sec-syscon");
	if (IS_ERR(adp->sec_reg)) {
		ret = PTR_ERR(adp->sec_reg);
		dev_err(dev, "failed to get security top address: %d\n", ret);
		return ret;
	}

	adp->top_reg = syscon_regmap_lookup_by_phandle(dev->of_node,
						       "intel,top-syscon");
	if (IS_ERR(adp->top_reg)) {
		ret = PTR_ERR(adp->top_reg);
		dev_err(dev, "failed to get top register map: %d\n", ret);
		return ret;
	}

	adp->rcu_reg = syscon_regmap_lookup_by_phandle(dev->of_node,
						       "intel,rcu-syscon");
	if (IS_ERR(adp->rcu_reg)) {
		ret = PTR_ERR(adp->rcu_reg);
		dev_err(dev, "failed to get rcu register map: %d\n", ret);
		return ret;
	}

	adp->epu_reg = syscon_regmap_lookup_by_phandle(dev->of_node,
						       "intel,epu-syscon");
	if (IS_ERR(adp->epu_reg)) {
		ret = PTR_ERR(adp->epu_reg);
		dev_err(dev, "failed to get epu register map: %d\n", ret);
		return ret;
	}

	adp->cgu_reg = syscon_regmap_lookup_by_phandle(dev->of_node,
						       "intel,cgu-syscon");
	if (IS_ERR(adp->cgu_reg)) {
		ret = PTR_ERR(adp->cgu_reg);
		dev_err(dev, "failed to get cgu register map: %d\n", ret);
		return ret;
	}

	adp_reset(adp);

	ret = adp_set_adp_lvl(adp, ADP_DVFS_LVL_2);
	if (ret)
		dev_warn(adp->dev, "Timeout to set adp lvl to 2\n");

	ret = adp_set_adp_lvl(adp, ADP_DVFS_LVL_0);
	if (ret)
		dev_warn(adp->dev, "Timeout to set adp lvl to 0\n");

	return 0;
}

static int adp_reset_remove(struct platform_device *pdev)
{
	struct adp_device *adp = platform_get_drvdata(pdev);
	u32 stat;
	int ret;

	regmap_read(adp->epu_reg, EPU_ADP_PM_ST, &stat);

	adp_reset(adp);

	ret = adp_set_adp_lvl(adp, ADP_DVFS_LVL_2);
	if (ret)
		dev_warn(adp->dev, "Timeout to set adp lvl to 2\n");

	if (stat != ADP_DVFS_LVL_2 && stat < ADP_DVFS_LVL_MAX) {
		ret = adp_set_adp_lvl(adp, stat);
		if (ret)
			dev_warn(adp->dev,
				 "Timeout to set adp lvl to %d\n", stat);
	}

	return 0;
}

static void adp_reset_shutdown(struct platform_device *pdev)
{
	struct adp_device *adp = platform_get_drvdata(pdev);
	int ret;

	adp_shutdown(adp);

	ret = adp_set_adp_lvl(adp, ADP_DVFS_LVL_2);
	if (ret)
		dev_warn(adp->dev, "Timeout to set adp lvl to 2\n");
}

static const struct of_device_id adp_reset_dt_ids[] = {
	{.compatible = "intel,adp_reset"},
	{}
};

static struct platform_driver adp_reset_driver = {
	.probe    =	adp_reset_probe,
	.remove   =	adp_reset_remove,
	.shutdown =	adp_reset_shutdown,
	.driver = {
		.name = "intel_adp_reset",
		.of_match_table = of_match_ptr(adp_reset_dt_ids),
	},
};

static int __init adp_reset_register(void)
{
	return platform_driver_register(&adp_reset_driver);
}
rootfs_initcall(adp_reset_register);
