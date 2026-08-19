// SPDX-License-Identifier: GPL-2.0
/*
 * Intel Combo-PHY driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 *
 */
#include <dt-bindings/phy/intel,phy-combo.h>
#include <linux/firmware.h>
#include <linux/iopoll.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/phy/phy.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/reset.h>

#include "phy-intel-lgm-combo.h"

#define REG_CLK_DISABLE(x)	((x) * 0x200 + 0x124)

/* CHIP TOP */
#define PAD_DISABLE_CFG		0x174

/* APP register */
#define PCIE_PHY_GEN_CTRL	0x0
#define CLK_DIV2_EN		2
#define PCIE_PHY_CLK_PAD	17
#define PCIE_PHY_SRAM_CSR	0x8
#define SRAM_BYPASS		0
#define SRAM_EXT_LD_DONE	1
#define SRAM_INIT_DONE		2
#define PCIE_PHY_MPLLA_CTRL	0x10
#define PCIE_PHY_MPLLB_CTRL	0x14
#define PCIE_PHY_MPLLA_STAT	31
#define XPCS_CTRL		0x130
#define XPCS_CTRL_XPCS_MASTER_EN_OFFSET		0
#define XPCS_CTRL_XPCS_MASTER_EN_MASK		0x1
#define XPCS_CTRL_XPCS_MASTER_SEL_OFFSET	1
#define XPCS_CTRL_XPCS_MASTER_SEL_MASK		0x1

/* CR registers */
#define RX_EQ_ASIC_IN(x)		(0x1013 + (x))
#define PCS_XF_ATE_OVRD_IN_2		0x3008
#define ADAPT_REQ			4
#define ADAPT_REQ_OVRD_EN		5
#define PCS_XF_RX_ADAPT_ACK		0x3010
#define RX_ADAPT_ACK			0
#define RX_ADPTCTL_ATT_STATUS		0x106b
#define RX_ADPTCTL_VGA_STATUS		0x106c
#define RX_ADPTCTL_CTLE_STATUS		0x106d
#define RX_ADPTCTL_DFE_TAP1_STATUS	0x106e
#define RX_ADPTCTL_STAT(i)		(RX_ADPTCTL_ATT_STATUS + (i))
#define PCS_XF_AT_OVRD_IN		(0xa016 << 2) /* Broadcast address */
#define PCS_RESET_OVRD			(BIT(0) | BIT(2))
#define PCS_RESET_OVRD_EN		(BIT(1) | BIT(3))

#define CR_ADDR(addr, lane)		(((addr) + (lane) * 0x100) << 2)

#define COMBO_PHY_ID(x)	((x)->parent->id)
#define PHY_ID(x)	((x)->id)

static int intel_combo_phy_init(struct phy *phy);
static int intel_combo_phy_exit(struct phy *phy);
static int intel_combo_phy_power_on(struct phy *phy);
static int intel_combo_phy_power_off(struct phy *phy);
static int intel_combo_phy_calibrate(struct phy *phy);

static const char * const intel_phy_names[] = {"pcie", "xpcs", "sata"};

static struct regmap *get_phy_cr_base(struct phy_ctx *iphy)
{
	return iphy->parent->iphy[PHY_0]->cr_base;
}

static struct regmap *get_phy_app_base(struct phy_ctx *iphy)
{
	return iphy->parent->iphy[PHY_0]->app_base;
}

static unsigned int phy_fw_addr_off(unsigned int off, unsigned int phy_type)
{
	if (phy_type == PCIE_E16_PHY)
		return (0xC000 + off) << 2;
	else
		return (0x6000 + off) << 2;
}

static inline u32 cbphy_r32(struct regmap *base, u32 reg)
{
	u32 val = 0;

	if (regmap_read(base, reg, &val))
		pr_debug("%s: read regmap %p reg 0x%x failed\n",
			 __func__, (void *)base, reg);

	return val;
}

static inline void cbphy_w32(struct regmap *base, u32 val, unsigned int reg)
{
	if (regmap_write(base, reg, val))
		pr_debug("%s: write val 0x%x to regmap %p reg 0x%x failed\n",
			 __func__, val, (void *)base, reg);
}

static inline void cbphy_w32_off_mask(struct regmap *base, u32 off,
				      u32 mask, u32 set, unsigned int reg)
{
	u32 val;

	val = cbphy_r32(base, reg) & (~(mask << off));
	val |= (set & mask) << off;
	cbphy_w32(base, val, reg);
}

static inline void cbphy_w32_mask(struct regmap *base, u32 clr,
				  u32 set, unsigned int reg)
{
	cbphy_w32(base, (cbphy_r32(base, reg) & ~(clr)) | set, reg);
}

static const struct phy_ops intel_cbphy_ops = {
	.init = intel_combo_phy_init,
	.exit = intel_combo_phy_exit,
	.power_on = intel_combo_phy_power_on,
	.power_off = intel_combo_phy_power_off,
	.calibrate = intel_combo_phy_calibrate,
};

/* driver functions */
static unsigned int count_phy_num(struct intel_combo_phy *priv)
{
	return priv->iphy[0]->power_on + priv->iphy[1]->power_on;
}

static struct phy_ctx *intel_iphy_get_peripheral(struct phy_ctx *iphy)
{
	return iphy->peripheral ? (struct phy_ctx *)dev_get_drvdata(iphy->peripheral) : NULL;
}

static struct phy_ctx *intel_iphy_get_controller(struct phy_ctx *iphy)
{
	return iphy->controller ? (struct phy_ctx *)dev_get_drvdata(iphy->controller) : NULL;
}

static bool intel_iphy_require_peripheral(struct phy_ctx *iphy)
{
	struct phy_ctx *peripheral = intel_iphy_get_peripheral(iphy);

	if (peripheral &&
	    iphy->parent->phy_type == PCIE_C10_PHY &&
	    iphy->parent->combo_mode == PHY_TYPE_XPCS_DL)
		return true;
	else
		return false;
}

static bool intel_iphy_has_active_controller(struct phy_ctx *iphy)
{
	struct phy_ctx *controller = intel_iphy_get_controller(iphy);

	if (controller &&
	    controller->parent->phy_type == PCIE_C10_PHY &&
	    controller->parent->combo_mode == PHY_TYPE_XPCS_DL)
		return true;
	else
		return false;
}

static void
intel_combo_phy_report(enum intel_phy_mode mode, unsigned int id)
{
	if (!id)
		return;

	id -= 1;
	switch (mode) {
	case PHY_PCIE_MODE:
		epu_notifier_blocking_chain(COMBO_EVENT_PCIE(id), NULL);
		break;
	case PHY_SATA_MODE:
		epu_notifier_blocking_chain(COMBO_EVENT_SATA(id), NULL);
		break;
	case PHY_XPCS_MODE:
		epu_notifier_blocking_chain(COMBO_EVENT_ETH(id), NULL);
		break;
	default:
		break;
	}
}

static int __maybe_unused intel_phy_set_xpcs_master(struct phy_ctx *iphy)
{
	struct regmap *app_base = get_phy_app_base(iphy);

	cbphy_w32_off_mask(app_base,
			   XPCS_CTRL_XPCS_MASTER_EN_OFFSET,
			   XPCS_CTRL_XPCS_MASTER_EN_MASK,
			   1,
			   XPCS_CTRL);

	cbphy_w32_off_mask(app_base,
			   XPCS_CTRL_XPCS_MASTER_SEL_OFFSET,
			   XPCS_CTRL_XPCS_MASTER_SEL_MASK,
			   0,
			   XPCS_CTRL);

	dev_dbg(iphy->dev, "%s PHY[%u:%u] XPCS_CTRL %x\n",
		__func__, COMBO_PHY_ID(iphy), PHY_ID(iphy),
		cbphy_r32(app_base, XPCS_CTRL));

	return 0;
}

/* set: 1-enable, 0-disable */
static void intel_phy_pcie_refclk_enable(struct phy_ctx *iphy)
{
	u32 bitn;
	struct intel_combo_phy *priv = iphy->parent;
	struct regmap *app_base = get_phy_app_base(iphy);

	if (priv->refclkid != 0xff)
		bitn = priv->refclkid & 0xff;
	else
		bitn = priv->id * 2 + iphy->id;

	if (!priv->refclkwr || !count_phy_num(priv)) {
		/* CHIPTOP refclk PAD disablegg config */
		regmap_update_bits(priv->syscfg, PAD_DISABLE_CFG,
				   BIT(bitn), 0);
		dev_dbg(iphy->dev, "Enable phy[%u:%u] refclk\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
	}

	/* Aragio pads bug that don't meet PCIe specs unless we
	 * combine reference clocks to improve the swing
	 */
	if (priv->refclkwr && !count_phy_num(priv)) {
		bitn = priv->id * 2 + (iphy->id ^ 1);
		regmap_update_bits(priv->syscfg, PAD_DISABLE_CFG,
				   BIT(bitn), 0);
		dev_dbg(iphy->dev, "refclkwr: set sibling's refclk\n");
	}

	/* choose internal clock or pad clock */
	cbphy_w32_off_mask(app_base, PCIE_PHY_CLK_PAD,
			   1, priv->padclk, PCIE_PHY_GEN_CTRL);
}

/* set: 1-enable, 0-disable */
static void intel_phy_lane_clk_on(struct phy_ctx *iphy, int set)
{
	struct intel_combo_phy *priv = iphy->parent;
	u32 val, bitn;

	if (intel_iphy_require_peripheral(iphy))
		return;

	if (intel_iphy_has_active_controller(iphy) &&
	    iphy->phy_mode != PHY_XPCS_MODE) {
		bitn = PHY_XPCS_MODE * 2 + iphy->id;
		val = set ? 0 : BIT(bitn);
		regmap_update_bits(priv->hsiocfg,
				   REG_CLK_DISABLE(priv->bid), BIT(bitn), val);
		dev_dbg(iphy->dev,
			"set per lane %s:phy[%u:%u]: 0x%x, mask: 0x%lx, off: 0x%x\n",
			set ? "enable" : "disable",
			COMBO_PHY_ID(iphy), PHY_ID(iphy),
			val, BIT(bitn), REG_CLK_DISABLE(priv->bid));
	}

	bitn = iphy->phy_mode * 2 + iphy->id;

	val =  set ? 0 : BIT(bitn);

	regmap_update_bits(priv->hsiocfg,
			   REG_CLK_DISABLE(priv->bid), BIT(bitn), val);

	dev_dbg(iphy->dev, "set per lane %s:phy[%u:%u]: 0x%x, mask: 0x%lx, off: 0x%x\n",
		set ? "enable" : "disable",
		COMBO_PHY_ID(iphy), PHY_ID(iphy),
		val, BIT(bitn), REG_CLK_DISABLE(priv->bid));
}

static void intel_phy_fw_write(struct phy_ctx *iphy, int idx, const u16 *fw)
{
	struct regmap *cr_base = get_phy_cr_base(iphy);
	unsigned int reg, val;

	val = (unsigned int)be16_to_cpu(*fw);
	reg = phy_fw_addr_off(idx, iphy->parent->phy_type);
	cbphy_w32(cr_base, val, reg);
}

static int intel_phy_fw_download(struct phy_ctx *iphy)
{
	struct intel_combo_phy *priv = iphy->parent;
	struct device *dev = priv->dev;
	const struct firmware *fw;
	const u8 *fw_ptr;
	int err, i;

	if (priv->fw_load_done)
		return 0;

	dev_dbg(dev, "Download FW: %s, iphy type: %d\n", priv->fwname, iphy->parent->phy_type);
	err = request_firmware_direct(&fw, priv->fwname, dev);
	if (err < 0) {
		dev_err(dev, "Failed to load combophy FW: %s\n", priv->fwname);
		return err;
	}

	for (i = 0, fw_ptr = fw->data; fw->size > fw_ptr - fw->data;
	     i++, fw_ptr += sizeof(u16))
		intel_phy_fw_write(iphy, i, (const u16 *)fw_ptr);

	priv->fw_load_done = 1;

	release_firmware(fw);

	return 0;
}

static void intel_phy_pcs_rst_ovrd(struct phy_ctx *iphy)
{
	struct regmap *cr_base = get_phy_cr_base(iphy);

	if (iphy->parent->phy_type != PCIE_E16_PHY)
		return;

	cbphy_w32_mask(cr_base, PCS_RESET_OVRD, PCS_RESET_OVRD_EN,
		       PCS_XF_AT_OVRD_IN);
}

static void intel_phy_pcs_rst_ovrd_restore(struct phy_ctx *iphy)
{
	struct regmap *cr_base = get_phy_cr_base(iphy);

	if (iphy->parent->phy_type != PCIE_E16_PHY)
		return;

	cbphy_w32_mask(cr_base, PCS_RESET_OVRD_EN, 0, PCS_XF_AT_OVRD_IN);
}

static int intel_phy_pfm_sram_init_check(struct phy_ctx *iphy)
{
	u32 val;
	int ret;
	struct regmap *app_base = get_phy_app_base(iphy);

	ret = regmap_read_poll_timeout(app_base, PCIE_PHY_SRAM_CSR,
				       val, val & BIT(SRAM_INIT_DONE),
				       1, 1000);
	if (ret < 0) {
		dev_err(iphy->dev, "PHY[%d] wait for init done timeout!\n",
			iphy->id);
		return ret;
	}

	dev_dbg(iphy->dev, "sram init done!\n");
	return 0;
}

static int intel_phy_pfm_sram_load_xpcs(struct phy_ctx *iphy)
{
	int ret;
	struct regmap *app_base = get_phy_app_base(iphy);

	ret = intel_phy_pfm_sram_init_check(iphy);
	if (ret < 0)
		return ret;

	ret = intel_phy_fw_download(iphy);
	if (ret < 0)
		return ret;

	cbphy_w32_off_mask(app_base, SRAM_EXT_LD_DONE,
			   1, 1, PCIE_PHY_SRAM_CSR);

	/* 15ms is required for the new FW to take effect after FW download */
	mdelay(15);

	dev_dbg(iphy->dev, "%s PHY%d SRAM_CSR %x\n",
		__func__, iphy->id,
		cbphy_r32(app_base, PCIE_PHY_SRAM_CSR));

	return 0;
}

static int intel_phy_pfm_sram_load_pcie(struct phy_ctx *iphy)
{
	u32 val;
	int ret;
	struct regmap *app_base = get_phy_app_base(iphy);

	ret = intel_phy_pfm_sram_init_check(iphy);
	if (ret < 0)
		return ret;

	ret = intel_phy_fw_download(iphy);
	if (ret < 0)
		return ret;

	/* Generally, all overrides should be before LD_DONE */
	intel_phy_pcs_rst_ovrd(iphy);

	/* Ensure override value to be taken effect first */
	wmb();

	cbphy_w32_off_mask(app_base, SRAM_EXT_LD_DONE,
			   1, 1, PCIE_PHY_SRAM_CSR);

	/* 15ms is required for the new FW to take effect after FW download */
	mdelay(15);
	dev_dbg(iphy->dev, "%s PHY%d SRAM_CSR %x\n",
		__func__, iphy->id,
		cbphy_r32(app_base, PCIE_PHY_SRAM_CSR));

	ret = regmap_read_poll_timeout(app_base, PCIE_PHY_MPLLA_CTRL,
				       val, val & BIT(PCIE_PHY_MPLLA_STAT),
				       50, 1000);
	if (ret < 0) {
		dev_err(iphy->dev, "PHY[%d] wait for MPLLA state timeout!\n",
			iphy->id);
		return ret;
	}

	intel_phy_pcs_rst_ovrd_restore(iphy);

	return 0;
}

static int intel_phy_pfm_sram_load(struct phy_ctx *iphy)
{
	if (iphy->phy_mode == PHY_PCIE_MODE)
		return intel_phy_pfm_sram_load_pcie(iphy);
	else
		return intel_phy_pfm_sram_load_xpcs(iphy); /* xpcs and sata */
}

static int intel_phy_xpcs_calibration(struct phy_ctx *iphy)
{
	struct device *dev = iphy->dev;
	struct regmap *cr_base = get_phy_cr_base(iphy);
	int i, ret, id;

	id = PHY_ID(iphy);

	/* debug print RX value before Adapatation */
	for (i = 0; i < 2; i++)
		dev_dbg(dev, "LANEN_DIG_ASIC_RX_EQ_ASIC_IN_%d: 0x%08x\n",
			i, cbphy_r32(cr_base, CR_ADDR(RX_EQ_ASIC_IN(i), id)));

	/* trigger auto RX adaptation */
	cbphy_w32_off_mask(cr_base, ADAPT_REQ, 3, 3,
			   CR_ADDR(PCS_XF_ATE_OVRD_IN_2, id));

	/* Wait RX Adaptation finish */
	ret = regmap_read_poll_timeout(cr_base, CR_ADDR(PCS_XF_RX_ADAPT_ACK, id),
				       i, i & BIT(RX_ADAPT_ACK),
				       10, 5000);
	if (ret)
		dev_err(dev, "RX Adaptation failed!\n");
	else
		dev_info(dev, "RX Adaptation success!\n");

	/* Stop RX adaptation */
	cbphy_w32_off_mask(cr_base, ADAPT_REQ, 3, 0,
			   CR_ADDR(PCS_XF_ATE_OVRD_IN_2, id));

	/* Debug information */
	/* Display RX Adapt value in PMA */
	dev_dbg(dev, "LANEN_DIG_RX_ADPTCTL_ATT_STATUS: 0x%08x\n",
		cbphy_r32(cr_base, CR_ADDR(RX_ADPTCTL_ATT_STATUS, id)));

	dev_dbg(dev, "LANEN_DIG_RX_ADPTCTL_VGA_STATUS: 0x%08x\n",
		cbphy_r32(cr_base, CR_ADDR(RX_ADPTCTL_VGA_STATUS, id)));

	dev_dbg(dev, "LANEN_DIG_RX_ADPTCTL_CTLE_STATUS: 0x%08x\n",
		cbphy_r32(cr_base, CR_ADDR(RX_ADPTCTL_CTLE_STATUS, id)));

	dev_dbg(dev, "LANEN_DIG_RX_ADPTCTL_DFE_TAP1_STATUS: 0x%08x\n",
		cbphy_r32(cr_base, CR_ADDR(RX_ADPTCTL_DFE_TAP1_STATUS, id)));

	/* print RX value after Adapatation */
	for (i = 0; i < 2; i++)
		dev_dbg(dev, "LANEN_DIG_ASIC_RX_EQ_ASIC_IN_%d: 0x%08x\n",
			i, cbphy_r32(cr_base, CR_ADDR(RX_EQ_ASIC_IN(i), id)));

	return ret;
}

static int intel_iphy_app_rst_assert(struct phy_ctx *iphy)
{
	enum intel_phy_mode phy_mode = iphy->phy_mode;

	if (iphy->app_rst[phy_mode]) {
		if (reset_control_assert(iphy->app_rst[phy_mode]))
			return -EINVAL;

		dev_dbg(iphy->dev, "%s: PHY[%u:%u] %s\n",
			__func__, COMBO_PHY_ID(iphy), PHY_ID(iphy),
			intel_phy_names[phy_mode]);
	}

	udelay(1);

	return 0;
}

static int intel_iphy_app_rst_deassert(struct phy_ctx *iphy)
{
	enum intel_phy_mode phy_mode = iphy->phy_mode;

	if (iphy->app_rst[phy_mode]) {
		if (reset_control_deassert(iphy->app_rst[phy_mode]))
			return -EINVAL;

		dev_dbg(iphy->dev, "%s: PHY[%u:%u] %s\n",
			__func__, COMBO_PHY_ID(iphy), PHY_ID(iphy),
			intel_phy_names[phy_mode]);
	}

	udelay(1);

	return 0;
}

static int intel_iphy_app_init(struct phy_ctx *iphy)
{
	struct intel_combo_phy *priv = iphy->parent;

	if (iphy->phy_mode == PHY_PCIE_MODE)
		intel_phy_pcie_refclk_enable(iphy);

	if (priv->phy_app_init++)
		return 0;

	return intel_phy_pfm_sram_load(iphy);
}

static enum intel_phy_mode cbmode_to_phymode(struct phy_ctx *iphy,
					     const unsigned int mode)
{
	if (iphy->parent->phy_type == PCIE_E16_PHY) {
		switch (mode) {
		case PHY_TYPE_PCIE_SL:
		case PHY_TYPE_PCIE_DL:
			return PHY_PCIE_MODE;
		case PHY_TYPE_XPCS_DL:
		case PHY_TYPE_XPCS_SL:
			return PHY_XPCS_MODE;
		case PHY_TYPE_SATA_SL:
			return PHY_SATA_MODE;
		default:
			return PHY_MAX_MODE;
		}
	} else if (iphy->parent->phy_type == PCIE_C10_PHY) {
		switch (mode) {
		case PHY_TYPE_PCIE_SL:
		case PHY_TYPE_PCIE_DL:
			return PHY_PCIE_MODE;
		case PHY_TYPE_XPCS_DL:
			return PHY_XPCS_MODE;
		default:
			return PHY_MAX_MODE;
		}
	} else {
		return PHY_MAX_MODE;
	}
}

/* Utility function to initialize combophy of peripheral phy
 * that is required for SGMII over C10 PHY.
 * This function does not set the combophy combo_mode.
 */
static int intel_combo_peripheral_phy_init(struct phy *phy)
{
	struct intel_combo_phy *priv;
	struct phy_ctx *iphy;
	struct device *dev;
	struct phy_ctx *controller;
	int ret;

	iphy = phy_get_drvdata(phy);
	priv = iphy->parent;
	dev = iphy->dev;

	controller = intel_iphy_get_controller(iphy);

	if (!intel_iphy_has_active_controller(iphy)) {
		dev_err(iphy->dev, "PHY[%u:%u] does not have active controller\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return -EINVAL;
	}

	/* There is a requirement that PCIE and SATA devices should be probed
	 * before XPCS devices. If PCIE and SATA devices are already probed,
	 * iphy->phy_mode and its parent's combo_mode should have been assigned.
	 *
	 * If iphy->phy_mode is PHY_MAX_MODE, it may be one of following cases
	 * (assume cb0phy1 is current peripheral phy):
	 * 1. cb0phy0 is PCIE dual lane. cb0phy1 is not used.
	 * 2. cb0phy0 is SATA. cb0phy1 is not used.
	 * 3. cb0phy0 is XPCS. cb0phy1 is not used. cb0phy0 may or may not be
	 *    initialized by now.
	 *
	 * In these cases, the iphy->phy_mode for cb0phy1 should be set based
	 * on the combophy combo_mode.
	 */
	if (iphy->phy_mode == PHY_MAX_MODE) {
		if (priv->combo_mode == PHY_TYPE_MAX)
			iphy->phy_mode = PHY_XPCS_MODE;
		else
			iphy->phy_mode = cbmode_to_phymode(iphy,
							   iphy->parent->combo_mode);
		dev_dbg(dev, "PHY[%u:%u] phy_mode assigned to %u\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy),
			iphy->phy_mode);
	}

	if (priv->ops.clk_enable(priv, iphy->phy_mode, 1)) {
		dev_err(dev, "PHY[%u:%u] clock enable failed\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return -EINVAL;
	}

	intel_phy_lane_clk_on(iphy, 1);

	priv->ops.lock(priv);

	if (!count_phy_num(priv)) {
		priv->ops.phy_rst(priv, 1);
		priv->ops.phy_rst(priv, 0);
	}

	if (priv->combo_mode == PHY_TYPE_XPCS_SL ||
	    priv->combo_mode == PHY_TYPE_MAX)
		intel_phy_set_xpcs_master(iphy);

	if (iphy->app_rst[PHY_XPCS_MODE]) {
		if (reset_control_deassert(iphy->app_rst[PHY_XPCS_MODE])) {
			priv->ops.unlock(priv);
			return -EINVAL;
		}
		dev_dbg(iphy->dev, "%s: PHY[%u:%u] %s app rst deasserted\n",
			__func__, COMBO_PHY_ID(iphy), PHY_ID(iphy),
			intel_phy_names[PHY_XPCS_MODE]);
	}

	ret = intel_phy_pfm_sram_init_check(iphy);
	if (ret) {
		dev_err(dev, "PHY(%u:%u) phy sram load failed!\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		priv->ops.unlock(priv);
		return ret;
	}

	iphy->power_on++;

	priv->ops.unlock(priv);

	dev_dbg(dev, "PHY[%u:%u]: peripheral phy init done! power_on: %u\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy), iphy->power_on);

	return 0;
}

static int intel_combo_phy_init(struct phy *phy)
{
	struct intel_combo_phy *priv;
	struct phy_ctx *iphy;
	struct device *dev;
	struct phy_ctx *peripheral;
	int ret;

	iphy = phy_get_drvdata(phy);
	priv = iphy->parent;
	dev = iphy->dev;

	peripheral = intel_iphy_get_peripheral(iphy);

	if (priv->ops.clk_enable(priv, iphy->phy_mode, 1)) {
		dev_err(dev, "PHY[%u:%u] clock enable failed\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return -EINVAL;
	}

	priv->ops.set_mode(priv);

	intel_phy_lane_clk_on(iphy, 1);

	priv->ops.lock(priv);

	if (!count_phy_num(priv)) {
		priv->ops.phy_rst(priv, 1);
		priv->ops.phy_rst(priv, 0);
	}

	ret = intel_iphy_app_rst_deassert(iphy);
	if (ret) {
		dev_err(dev, "PHY(%u:%u) phy deassert failed!\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		goto init_err;
	}

	if (intel_iphy_require_peripheral(iphy)) {
		struct phy_ctx *peripheral_iphy;

		peripheral_iphy = phy_get_drvdata(peripheral->phy);
		if (priv == peripheral_iphy->parent) {
			dev_err(dev, "PHY[%u:%u] peripheral phy is not valid\n",
				COMBO_PHY_ID(iphy), PHY_ID(iphy));
			goto init_err;
		}

		/* Although priv in intel_combo_phy_init() and priv in
		 * intel_combo_peripheral_phy_init() are different, one points to
		 * combophy0, the other points to combophy1, kernel considers
		 * the priv->ops.lock(priv) in the 2 functions as recursive lock.
		 * So, we need to unlock priv before calling
		 * intel_combo_peripheral_phy_init() and lock it again afterward.
		 */
		priv->ops.unlock(priv);
		intel_combo_peripheral_phy_init(peripheral->phy);
		priv->ops.lock(priv);
	} else {
		ret = intel_iphy_app_init(iphy);
		if (ret) {
			dev_err(dev, "PHY(%u:%u) phy sram load failed!\n",
				COMBO_PHY_ID(iphy), PHY_ID(iphy));
			goto init_err;
		}
	}
	iphy->power_on++;

	priv->ops.unlock(priv);

	dev_dbg(dev, "PHY[%u:%u]: phy init done! power_on: %u\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy), iphy->power_on);

	return 0;

init_err:
	priv->ops.unlock(priv);
	return ret;
}

static int intel_combo_phy_exit(struct phy *phy)
{
	struct phy_ctx *iphy = phy_get_drvdata(phy);
	struct intel_combo_phy *priv;
	int i;

	priv = iphy->parent;

	if (--priv->phy_app_init)
		return 0;

	priv->ops.lock(priv);
	for (i = 0; i < PHY_MAX_NUM; i++) {
		iphy = priv->iphy[i];
		if (!iphy->power_on)
			continue;

		iphy->power_on--;
		if (!iphy->power_on) {
			intel_iphy_app_rst_assert(iphy);
			intel_phy_lane_clk_on(iphy, 0);
			priv->ops.clk_enable(priv, iphy->phy_mode, 0);
			dev_dbg(iphy->dev, "phy[%u:%u]: phy exit done!\n",
				COMBO_PHY_ID(iphy), PHY_ID(iphy));
		}
	}
	if (!count_phy_num(priv))
		priv->ops.phy_rst(priv, 1);

	priv->ops.unlock(priv);

	return 0;
}

static int intel_combo_phy_power_on(struct phy *phy)
{
	struct phy_ctx *iphy = phy_get_drvdata(phy);

	dev_dbg(iphy->dev, "phy[%u:%u] power on!\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy));

	return 0;
}

static int intel_combo_phy_power_off(struct phy *phy)
{
	struct phy_ctx *iphy = phy_get_drvdata(phy);

	dev_dbg(iphy->dev, "phy[%u:%u] power off!\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy));

	return 0;
}

static int intel_combo_phy_calibrate(struct phy *phy)
{
	struct phy_ctx *iphy;

	iphy = phy_get_drvdata(phy);

	if (iphy->phy_mode == PHY_XPCS_MODE)
		return intel_phy_xpcs_calibration(iphy);

	return 0;
}

static int intel_iphy_mem_resource(struct phy_ctx *iphy)
{
	iphy->cr_base = syscon_regmap_lookup_by_phandle(iphy->np, "mxl,cr");
	iphy->app_base = syscon_regmap_lookup_by_phandle(iphy->np, "mxl,app");
	if (IS_ERR_OR_NULL(iphy->cr_base) || IS_ERR_OR_NULL(iphy->app_base)) {
		dev_err(iphy->dev, "PHY[%u:%u] failed to get syscon regmap!\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return -EINVAL;
	}

	dev_dbg(iphy->dev, "PHY[%u:%u] cr_base: %p, app_base: %p\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy),
		iphy->cr_base, iphy->app_base);

	return 0;
}

static int intel_iphy_get_reset(struct phy_ctx *iphy)
{
	int i;
	struct device *dev = iphy->dev;

	for (i = PHY_PCIE_MODE; i < PHY_MAX_MODE; i++) {
		iphy->app_rst[i] =
			devm_reset_control_get_optional(dev,
							intel_phy_names[i]);
		if (IS_ERR(iphy->app_rst[i])) {
			dev_err(dev, "PHY[%u:%u] Get %s reset ctrl Err!\n",
				COMBO_PHY_ID(iphy), PHY_ID(iphy),
				intel_phy_names[i]);
			return -EINVAL;
		} else if (iphy->app_rst[i]) {
			dev_dbg(dev, "PHY[%u:%u] Get %s reset ctrl!\n",
				COMBO_PHY_ID(iphy), PHY_ID(iphy),
				intel_phy_names[i]);
		}
	}

	return 0;
}

static int intel_iphy_dt_parse(struct phy_ctx *iphy)
{
	struct platform_device *pdev;
	struct device_node *node;
	struct phy_ctx *peripheral;

	if (intel_iphy_mem_resource(iphy))
		return -EINVAL;

	if (intel_iphy_get_reset(iphy))
		return -EINVAL;

	node = of_parse_phandle(iphy->dev->of_node, "mxl,peripheral", 0);
	if (node) {
		pdev = of_find_device_by_node(node);
		of_node_put(node);
		if (pdev) {
			iphy->peripheral = &pdev->dev;
			peripheral = dev_get_drvdata(&pdev->dev);
			if (peripheral)
				peripheral->controller = iphy->dev;
		}
	}

	dev_dbg(iphy->dev, "PHY(%u:%u) dt parse success\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy));

	return 0;
}

static struct phy *
of_phy_intel_xlate(struct device *dev, struct of_phandle_args *args)
{
	unsigned int mode;
	enum intel_phy_mode phy_mode;
	struct phy_ctx *iphy = dev_get_drvdata(dev);
	struct phy_ctx *peripheral = intel_iphy_get_peripheral(iphy);
	struct phy_ctx *controller = intel_iphy_get_controller(iphy);
	struct intel_combo_phy *priv = iphy->parent;

	if (args->args_count < 1) {
		dev_err(dev, "Invalid number of arguments\n");
		return ERR_PTR(-EINVAL);
	}

	dev_dbg(dev, "PHY[%u:%u]: peripheral[%u:%u] controller:[%u:%u]\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy),
		peripheral ? COMBO_PHY_ID(peripheral) : 0xFFFF,
		peripheral ? PHY_ID(peripheral) : 0xFFFF,
		controller ? COMBO_PHY_ID(controller) : 0xFFFF,
		controller ? PHY_ID(controller) : 0xFFFF);

	mode = args->args[0];
	if (mode >= PHY_TYPE_MAX) {
		dev_err(dev, "Request mode %u not supported\n", mode);
		return ERR_PTR(-EINVAL);
	}

	phy_mode = cbmode_to_phymode(iphy, mode);
	if (!(priv->phy_cap & BIT(phy_mode))) {
		dev_err(dev, "PHY[%u:%u]: Request phy mode %s not supported\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy),
			intel_phy_names[phy_mode]);
		return ERR_PTR(-EINVAL);
	}

	if (iphy->phy_mode != PHY_MAX_MODE) {
		dev_err(dev, "PHY[%u:%u]: Already initialized\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return ERR_PTR(-EINVAL);
	}

	/* If combo_mode for C10 PHY is PHY_TYPE_XPCS_DL, then peripheral
	 * can't be NULL.
	 */
	if (!peripheral &&
	    iphy->parent->phy_type == PCIE_C10_PHY &&
	    mode == PHY_TYPE_XPCS_DL) {
		dev_err(dev, "PHY[%u:%u]: peripheral not valid\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return ERR_PTR(-EINVAL);
	}

	if (iphy->parent->phy_type == PCIE_C10_PHY &&
	    mode == PHY_TYPE_XPCS_DL &&
	    PHY_ID(iphy) != 0) {
		dev_err(dev, "PHY[%u:%u]: PHY1 can't be configured as XPCS\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return ERR_PTR(-EINVAL);
	}

	priv->ops.lock(priv);
	if (priv->combo_mode != PHY_TYPE_MAX && priv->combo_mode != mode) {
		dev_err(dev, "Request for different mode: %u->%u\n",
			priv->combo_mode, mode);
		priv->ops.unlock(priv);
		return ERR_PTR(-EINVAL);
	}

	priv->combo_mode = mode;
	priv->ops.unlock(priv);

	iphy->phy_mode = phy_mode;
	intel_combo_phy_report(phy_mode, priv->epu_idx);

	return iphy->phy;
}

static int intel_phy_create(struct phy_ctx *iphy)
{
	struct device *dev = iphy->dev;
	struct phy_provider *phy_provider;

	iphy->phy = devm_phy_create(dev, iphy->np, &intel_cbphy_ops);
	if (IS_ERR(iphy->phy)) {
		dev_err(dev, "PHY[%u:%u]: create PHY instance failed!\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return PTR_ERR(iphy->phy);
	}

	phy_set_drvdata(iphy->phy, iphy);

	phy_provider = devm_of_phy_provider_register(dev, of_phy_intel_xlate);
	if (IS_ERR(phy_provider)) {
		dev_err(dev, "PHY[%u:%u]: register phy provider failed!\n",
			COMBO_PHY_ID(iphy), PHY_ID(iphy));
		return PTR_ERR(phy_provider);
	}
	iphy->provider = phy_provider;
	dev_dbg(dev, "Create and register PHY[%u:%u]\n",
		COMBO_PHY_ID(iphy), PHY_ID(iphy));

	return 0;
}

static int intel_iphy_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct phy_ctx *iphy;
	struct device_node *np, *child;
	struct platform_device *ppdev;
	struct intel_combo_phy *priv;

	iphy = devm_kzalloc(dev, sizeof(*iphy), GFP_KERNEL);
	if (!iphy)
		return -ENOMEM;

	np = of_get_parent(dev->of_node);
	if (!np) {
		dev_err(dev, "Failed to find parent node!\n");
		return -EINVAL;
	}

	ppdev = of_find_device_by_node(np);
	if (!ppdev) {
		dev_err(dev, "Failed to find parent pdev\n");
		return -EINVAL;
	}

	priv = platform_get_drvdata(ppdev);
	if (!priv) {
		dev_err(dev, "Parent device is not initialized!\n");
		return -EPROBE_DEFER;
	}

	iphy->id = 0;
	for_each_child_of_node(np, child) {
		if (child == dev->of_node) {
			of_node_put(child);
			break;
		}
		iphy->id++;
	}

	WARN_ON(iphy->id >= PHY_MAX_NUM);

	iphy->pdev = pdev;
	iphy->dev = dev;
	iphy->parent = priv;
	iphy->phy_mode = PHY_MAX_MODE;
	iphy->np = dev->of_node;
	priv->iphy[iphy->id] = iphy;
	platform_set_drvdata(pdev, iphy);

	if (intel_iphy_dt_parse(iphy))
		return -EINVAL;

	if (intel_phy_create(iphy))
		return -EINVAL;

	return 0;
}

static const struct of_device_id of_intel_iphy_match[] = {
	{ .compatible = "intel,cbphy"},
	{}
};
MODULE_DEVICE_TABLE(of, of_intel_iphy_match);

static struct platform_driver intel_iphy_driver = {
	.probe = intel_iphy_probe,
	.driver = {
		.name = "intel-iphy",
		.of_match_table = of_match_ptr(of_intel_iphy_match),
	}
};

module_platform_driver(intel_iphy_driver);

MODULE_LICENSE("GPL");
