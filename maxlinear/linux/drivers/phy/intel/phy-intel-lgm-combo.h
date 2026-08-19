/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 */

#ifndef _PHY_INTEL_COMBO_H
#define _PHY_INTEL_COMBO_H

enum {
	PHY_TYPE_NONE,
	PCIE_E16_PHY,
	PCIE_C10_PHY,
};

enum {
	PHY_0,
	PHY_1,
	PHY_MAX_NUM,
};

enum intel_phy_mode {
	PHY_PCIE_MODE,
	PHY_XPCS_MODE,
	PHY_SATA_MODE,
	PHY_MAX_MODE
};

enum {
	PHY_PCIE_CAP = BIT(PHY_PCIE_MODE),
	PHY_XPCS_CAP = BIT(PHY_XPCS_MODE),
	PHY_SATA_CAP = BIT(PHY_SATA_MODE),
};

enum cbphy_version {
	CBPHY_LGMB0,
	CBPHY_LGMC0,
};

struct intel_combo_phy;
struct phy_ctx;

struct cbphy_ops {
	int (*clk_enable)(struct intel_combo_phy *priv,
			  enum intel_phy_mode mode, int en);
	void (*set_mode)(struct intel_combo_phy *priv);
	void (*phy_rst)(struct intel_combo_phy *priv, int rst);
	void (*lock)(struct intel_combo_phy *priv);
	void (*unlock)(struct intel_combo_phy *priv);
};

struct cbphy_data {
	enum cbphy_version cbphy_ver;
	bool xpcs_with_c10;
};

struct phy_ctx {
	unsigned int id; /* Internal PHY idx 0 - 1 */
	unsigned int power_on;
	struct intel_combo_phy *parent;
	enum intel_phy_mode phy_mode;
	struct phy *phy;
	struct regmap *cr_base;
	struct regmap *app_base;
	struct reset_control *app_rst[PHY_MAX_MODE];
	struct device	*dev;
	struct device *peripheral;	/* peripheral device to support SGMII */
	struct device *controller;	/* controller device to support SGMII */
	struct platform_device *pdev;
	struct device_node *np;
	struct phy_provider *provider;
};

struct intel_combo_phy {
	const struct cbphy_data *data;
	unsigned int id; /* Physical COMBO PHY Index */
	unsigned int phy_cap; /* phy capability, depends on SoC and PHY ID */
	unsigned int bid; /* Bus ID */
	unsigned int epu_idx; /* real idx + 1, 0 means no need to report */
	unsigned int padclk; /* 0 - use intenal clk, 1 - use pad clk */
	unsigned int refclkwr; /* 1 - refclk enable at same time for duallane */
	unsigned int refclkid; /* LGMC only has 4 refclks, they must be distributed between pcie10/11/20/21/30/40 */
	unsigned int phy_app_init; /* phy app only need init once */
	const char *fwname;
	int fw_load_done;
	struct mutex mutex; /* lock to protect iphys concurrently access */
	struct clk *phy_clk;
	unsigned int combo_mode;
	unsigned int phy_type;
	struct platform_device *pdev;
	struct device *dev;
	struct device_node *np;
	struct regmap *syscfg;
	struct regmap *hsiocfg;
	struct reset_control *phy_rst;
	struct reset_control *combo_rst;
	struct phy_ctx *iphy[PHY_MAX_NUM];
	struct cbphy_ops ops;
};

#endif /* _PHY_INTEL_COMBO_H */
