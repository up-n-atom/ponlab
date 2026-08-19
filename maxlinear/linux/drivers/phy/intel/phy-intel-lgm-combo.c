// SPDX-License-Identifier: GPL-2.0
/*
 * Intel Combo-PHY driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 *
 * Single Lane mode
 * iphy0 and iphy1 work independently
 *         |---iphy0 (PCIe/XPS/SATA)
 * cbphy----
 *         |---iphy1 (PCIe/XPCS/SATA)
 *
 * Dual Lane mode
 * iphy0 is master while iphy1 is slave
 * Only iphy0 is working and accessible
 *         |---iphy0 (PCIe/XPS Master)
 * cbphy----
 *         |---iphy1 (slave)
 *
 */
#include <dt-bindings/phy/intel,phy-combo.h>
#include <linux/clk.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/regmap.h>
#include <linux/reset.h>

#include "phy-intel-lgm-combo.h"

#define REG_COMBO_MODE(x)	((x) * 0x200)

#define CLK_100MHZ	100000000	/* 100MHZ */
#define CLK_156MHZ	156250000	/* 156.25Mhz */

static const char * const cbphy_mode_names[] = {
	"PCIe single lane mode",
	"PCIe Dual lane mode",
	"RXAUI mode",
	"XFI single lane mode",
	"SATA single lane mode",
	"PHY mode not supported"
};

static const unsigned long intel_phy_clk[] = {
	CLK_100MHZ, CLK_156MHZ, CLK_100MHZ
};

static ssize_t
combo_phy_info_show(struct device *dev,
		    struct device_attribute *attr, char *buf)
{
	struct intel_combo_phy *priv;
	int off;

	priv = dev_get_drvdata(dev);

	/* combo phy mode */
	off = sprintf(buf, "PHY: %u, mode: %s\n",
		      priv->id, cbphy_mode_names[priv->combo_mode]);

	return off;
}

static DEVICE_ATTR_RO(combo_phy_info);

static struct attribute *combo_phy_attrs[] = {
	&dev_attr_combo_phy_info.attr,
	NULL,
};

ATTRIBUTE_GROUPS(combo_phy);

static void intel_combo_phy_rst_assert(struct intel_combo_phy *priv)
{
	if (priv->combo_rst)
		reset_control_assert(priv->combo_rst);

	if (priv->phy_rst)
		reset_control_assert(priv->phy_rst);
}

static void intel_combo_phy_rst_deassert(struct intel_combo_phy *priv)
{
	if (priv->combo_rst)
		reset_control_deassert(priv->combo_rst);

	if (priv->phy_rst)
		reset_control_deassert(priv->phy_rst);

	usleep_range(10, 20);
}

/* rst: 1-assert   2-deassert */
static void intel_combo_phy_rst(struct intel_combo_phy *priv, int rst)
{
	if (rst)
		intel_combo_phy_rst_assert(priv);
	else
		intel_combo_phy_rst_deassert(priv);

	dev_dbg(priv->dev, "Combo phy %u reset: %s\n",
		priv->id, rst ? "assert" : "deassert");
}

static void intel_combo_phy_set_mode(struct intel_combo_phy *priv)
{
	regmap_write(priv->hsiocfg, REG_COMBO_MODE(priv->bid),
		     priv->combo_mode);
	dev_dbg(priv->dev, "Set combo mode: combophy[%d]: mode: %s\n",
		priv->id, cbphy_mode_names[priv->combo_mode]);
}

static void intel_combo_phy_lock(struct intel_combo_phy *priv)
{
	mutex_lock(&priv->mutex);
}

static void intel_combo_phy_unlock(struct intel_combo_phy *priv)
{
	mutex_unlock(&priv->mutex);
}

static int
intel_cbphy_clk_enable(struct intel_combo_phy *priv,
		       enum intel_phy_mode mode, int en)
{
	if (!en) {
		clk_disable_unprepare(priv->phy_clk);
		dev_dbg(priv->dev, "Disable combphy clock\n");

		return 0;
	}

	if (clk_prepare_enable(priv->phy_clk))
		return -EINVAL;

	if (clk_set_rate(priv->phy_clk, intel_phy_clk[mode]))
		return -EINVAL;

	dev_dbg(priv->dev, "Enable and set phy clk to %lu\n",
		intel_phy_clk[mode]);

	return 0;
}

static int intel_combo_phy_dt_parse(struct intel_combo_phy *priv)
{
	struct device *dev = priv->dev;
	struct device_node *np = priv->np;

	/* get combophy clock */
	priv->phy_clk = devm_clk_get(dev, NULL);
	if (IS_ERR(priv->phy_clk)) {
		dev_err(dev, "CBPHY %u get PHY clk failed!\n", priv->id);
		return PTR_ERR(priv->phy_clk);
	}

	/* get phy reset */
	priv->phy_rst = devm_reset_control_get_optional(dev, "phy");
	if (IS_ERR(priv->phy_rst)) {
		dev_err(dev, "CBPHY %u get phy reset err!\n", priv->id);
		return PTR_ERR(priv->phy_rst);
	}

	/* get combo reset */
	priv->combo_rst = devm_reset_control_get_optional(dev, "combo");
	if (IS_ERR(priv->combo_rst)) {
		dev_err(dev, "CBPHY %u get combo reset err!\n", priv->id);
		return PTR_ERR(priv->combo_rst);
	}

	/* get combophy capability */
	if (device_property_read_bool(dev, "intel,cbphy-pcie")) {
		priv->phy_type = PCIE_C10_PHY;
		if (priv->data->xpcs_with_c10)
			priv->phy_cap = PHY_PCIE_CAP | PHY_XPCS_CAP;
		else
			priv->phy_cap = PHY_PCIE_CAP;
	} else {
		priv->phy_type = PCIE_E16_PHY;
		priv->phy_cap = PHY_PCIE_CAP | PHY_XPCS_CAP | PHY_SATA_CAP;
	}

	/* get chiptop regmap */
	priv->syscfg = syscon_regmap_lookup_by_phandle(np, "intel,syscon");
	if (IS_ERR_OR_NULL(priv->syscfg)) {
		dev_err(dev, "No syscon phandle for chiptop\n");
		return -EINVAL;
	}

	/* get hsio regmap */
	priv->hsiocfg = syscon_regmap_lookup_by_phandle(np, "intel,hsio");
	if (IS_ERR(priv->hsiocfg)) {
		dev_err(dev, "No syscon phandle for hsio\n");
		return -EINVAL;
	}

	if (device_property_read_u32(dev, "intel,bid", &priv->bid)) {
		dev_err(dev, "NO intel,bid provided!\n");
		return -EINVAL;
	}

	/* Not all the cobophy need report to EPU */
	if (device_property_read_u32(dev, "intel,epu-idx", &priv->epu_idx))
		dev_info(dev, "Not EPU index on combophy %d\n", priv->id);

	if (device_property_read_string(dev, "intel,phyfw", &priv->fwname)) {
		priv->fw_load_done = 1;
		dev_info(dev, "No combophy FW required!\n");
	}

	/* use intenal clock or pad clock */
	if (device_property_read_bool(dev, "intel,padclk"))
		priv->padclk = 1;
	else
		priv->padclk = 0;

	/* Refclk duallan WR for A3 */
	if (device_property_read_bool(dev, "intel,refclkwr"))
		priv->refclkwr = 1;
	else
		priv->refclkwr = 0;

	if (device_property_read_u32(dev, "intel,refclkid", &priv->refclkid))
		priv->refclkid = 0xff;

	return 0;
}

static int intel_combo_phy_sysfs_init(struct intel_combo_phy *priv)
{
	return devm_device_add_groups(priv->dev, combo_phy_groups);
}

static void intel_combo_phy_func_setup(struct intel_combo_phy *priv)
{
	priv->ops.clk_enable = intel_cbphy_clk_enable;
	priv->ops.phy_rst = intel_combo_phy_rst;
	priv->ops.set_mode = intel_combo_phy_set_mode;
	priv->ops.lock = intel_combo_phy_lock;
	priv->ops.unlock = intel_combo_phy_unlock;
}

static int intel_combo_phy_probe(struct platform_device *pdev)
{
	int id;
	struct device *dev = &pdev->dev;
	struct intel_combo_phy *priv;
	struct device_node *np = dev->of_node;

	if (!dev->of_node)
		return -ENODEV;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	id = of_alias_get_id(np, "cbphy");
	if (id < 0) {
		dev_err(dev, "failed to get alias id, errno %d\n", id);
		return -EINVAL;
	}

	priv->data = of_device_get_match_data(dev);
	if (!priv->data) {
		dev_err(dev, "failed to get match data\n");
		return -EINVAL;
	}

	priv->id = id;
	priv->pdev = pdev;
	priv->dev = dev;
	priv->np = np;
	priv->combo_mode = PHY_TYPE_MAX;
	mutex_init(&priv->mutex);
	platform_set_drvdata(pdev, priv);

	if (intel_combo_phy_dt_parse(priv))
		return -EINVAL;

	intel_combo_phy_func_setup(priv);

	if (intel_combo_phy_sysfs_init(priv))
		return -EINVAL;

	return 0;
}

static const struct cbphy_data intel_combo_phy_data = {
	.cbphy_ver = CBPHY_LGMB0,
	.xpcs_with_c10 = false,
};

static const struct cbphy_data mxl_combo_phy_lgmc0_data = {
	.cbphy_ver = CBPHY_LGMC0,
	.xpcs_with_c10 = true,
};

static const struct of_device_id of_intel_combo_phy_match[] = {
	{ .compatible = "intel,combo-phy", .data = &intel_combo_phy_data},
	{ .compatible = "mxl,combo-phy-lgmc", .data = &mxl_combo_phy_lgmc0_data},
	{}
};
MODULE_DEVICE_TABLE(of, of_intel_combo_phy_match);

static struct platform_driver intel_combo_phy_driver = {
	.probe = intel_combo_phy_probe,
	.driver = {
		.name = "intel-combo-phy",
		.of_match_table = of_match_ptr(of_intel_combo_phy_match),
	}
};

module_platform_driver(intel_combo_phy_driver);

MODULE_LICENSE("GPL");
