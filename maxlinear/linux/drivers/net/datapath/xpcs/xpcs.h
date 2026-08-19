/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Maxlinear XPCS driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020, Intel Corporation.
 * Xu Liang <lxu@maxlinear.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _XPCS_H_
#define _XPCS_H_

#include <linux/types.h>
#include <linux/ethtool.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

#define XPCS_DEV_NAME			"mxl_xpcs"

enum xpcs_phy_type {
	XPCS_PHY_TYPE_6G = 0, /* unsupported */
	XPCS_PHY_TYPE_12G = 1,
	XPCS_PHY_TYPE_16G = 2,
	XPCS_PHY_TYPE_25G = 3, /* unsupported */
	XPCS_PHY_TYPE_8G = 4,
};

enum xpcs_mode_t {
	XPCS_MODE_SGMII = 0,
	XPCS_MODE_1000BASEX,
	XPCS_MODE_2500BASEX,
	XPCS_MODE_5GBASER,
	XPCS_MODE_10GBASER,
	XPCS_MODE_SUSXGMII,
	XPCS_MODE_10GKR,
	XPCS_MODE_QUSXGMII,
	XPCS_MODE_3000BASEX,
	XPCS_MODE_MAX,
};

enum {
	CONN_TYPE_ANEG_DIS = 0,
	CONN_TYPE_ANEG_MAC = 1,
	CONN_TYPE_ANEG_PHY = 2,
	CONN_TYPE_ANEG_BX = 3,
	CONN_TYPE_MAX,
};

enum {
	PMA_TYPE_10G_BASE_CX4 = 0,
	PMA_TYPE_10G_BASE_EW,
	PMA_TYPE_10G_BASE_LW,
	PMA_TYPE_10G_BASE_SW,
	PMA_TYPE_10G_BASE_LX4,
	PMA_TYPE_10G_BASE_ER,
	PMA_TYPE_10G_BASE_LR,
	PMA_TYPE_10G_BASE_SR,
	PMA_TYPE_10G_BASE_LRM,
	PMA_TYPE_10G_BASE_T,
	PMA_TYPE_10G_BASE_KX4,
	PMA_TYPE_10G_BASE_KR,
	PMA_TYPE_1G_BASE_T,
	PMA_TYPE_1G_BASE_KX,
	PMA_TYPE_100_BASE_TX,
	PMA_TYPE_10_BASE_T
};

enum {
	PCS_TYPE_10G_BASE_R = 0,
	PCS_TYPE_10G_BASE_X,
	PCS_TYPE_10G_BASE_W,
	PCS_TYPE_RESERVED,
};

struct xpcs_prv_data;

/* description of the hardware configuration */
struct xpcs_hw_config {
	enum xpcs_phy_type phy_type;
	bool has_sram;
	bool usxg_sport;
	bool usxg_qport;
	bool rsvdparam11;
};

struct xpcs_serdes_cfg {
	/* RX Analog Front End (AFE) */
	/* Attenuator */
	u32 rx_attn_lvl;	/* Attenuation */
	/* Continuous Time-Linear Equalizer */
	u32 rx_ctle_boost;	/* Boost level */
	u32 rx_ctle_pole;	/* Boost pole frequency */
	/* Voltage Gain Amplifiers */
	u32 rx_vga1_gain;
	u32 rx_vga2_gain;

	/* RX Decision Feedback Equalizer (DFE) */
	u32 rx_dfe_bypass;	/* Bypass (disable) DFE */
	u32 cont_adapt;		/* Continuous adaptation enable */

	/* Calibrate RX AFE and DFE
	 * Startup calibration is followed by startup adaptation
	 */
	u32 rx_cali;		/* Enable RX startup calibration */

	/* TX Amplitude Control
	 * Vswing is peak-to-peak output swing of the TX,
	 * and nominally equal to the power supply (vptx).
	 * However, the TX supports the boost function, which is enabled
	 * by setting tx_vboost_en.
	 * Formula:
	 *  Vswing (Vppd) = vptx + 1/16 * (1 + tx_iboost_lvl[3:0])
	 *                         * max(4*tx_vboost_vref - vptx, 0)
	 *  12G PHY:
	 *   tx_vboost_vref = vbg * (10 + 4*tx_vboost_lvl[2:0]) / 127
	 *   vbg = 1.23V
	 *   vptx = Analog Transmitter Termination Voltage Supply, usually 0.90V
	 *  16G PHY:
	 *   tx_vboost_vref = vbg * (tx_vboost_lvl[2:0] + 6) / 32
	 *   vbg = 0.8V
	 *   vptx = Analog Transmitter Termination Voltage Supply, 0.8V or 0.85V
	 */
	u32 tx_vboost_en;	/* Enable TX voltage boost */
	u32 tx_vboost_lvl;	/* TX voltage boost maximum level,
				 * default value 5
				 */
	u32 tx_iboost_lvl;	/* Transmitter current boost level */

	/* TX Equalization
	 * TX driver is implemented with a 3-tap feed-forward equalization.
	 *  C0  - main tap coefficient
	 *  C-1 - pre-cursor tap coefficient, negative value
	 *  C1  - post-cursor tap coefficient, negative value
	 *  C0    = tx_eq_main[5:0]
	 *  |C-1| = tx_eq_pre[5:0]/4
	 *  |C1|  = tx_eq_post[5:0]/4
	 *                                                 ___        ___
	 *                   __________                   /   \      /
	 *     _________    /          \           __    /     \    /
	 *  __/         \  /            \_________/  \  /       \  /
	 *  Pre-Amplitude\/Post-Amplitude DC-Swing    \/Max-Swing\/
	 *  __           /\              _________    /\         /\
	 *    \_________/  \            /         \__/  \       /  \
	 *                  \__________/                 \     /    \
	 *                                                \___/      \___
	 *
	 *  pre-amplitude  = Vswing * (C0 + |C-1| - |C1|)/40
	 *  post-amplitude = Vswing * (C0 - |C-1| + |C1|)/40
	 *  dc-swing       = Vswing * (C0 - |C-1| - |C1|)/40
	 *  max-swing      = Vswing * (C0 + |C-1| + |C1|)/40
	 */
	u32 tx_eq_main;
	u32 tx_eq_pre;
	u32 tx_eq_post;

	u32 tx_eq_ovrride;
};

/* 10G XAUI Mode Switching */
struct xpcs_mode_cfg {
	u32 pma_type;
	u32 pcs_type;
	u32 lane;

	u32 ref_clk_ctrl;

	u32 mplla_mult;
	u32 mplla_bw;
	u32 mplla_tx_clk_div;	/* default value is used for all modes so far */
	u32 mplla_div165_clk_en;
	u32 mplla_div10_clk_en;
	u32 mplla_div8_clk_en;

	u32 mpllb_mult;
	u32 mpllb_bw;
	u32 mpllb_tx_clk_div;
	u32 mpllb_div10_clk_en;
	u32 mpllb_div8_clk_en;
	u32 mpllb_div_clk_en;
	u32 mpllb_div_mult;

	u32 mpllb_tx_clk_div_pcie3_app;
	u32 mpllb_div_mult_pcie3_app;
	u32 mpllb_force_en_ovrd_pcie3_app;
	u32 mpllb_ssc_clk_sel_pcie3_app;
	u32 mpllb_cdr_vco_low_freq_pcie3_app;

	/* Lane Configure */
	u32 vco_ld_val;
	u32 vco_ref_ld;
	u32 vco_temp_comp_en;
	u32 vco_step_ctrl;
	u32 vco_frqband;
	u32 adpt_sel;
	u32 adpt_mode;
	u32 afe_en; /* only used on 12G PHY */
	u32 dfe_en; /* only used on 12G PHY */
	u32 tx_rate;
	u32 rx_rate;	/* Boost bandwidth */
	u32 tx_width;
	u32 rx_width;
	u32 rx_misc;
	u32 rx_delta_iq;

	u32 los_thr;
	u32 cdr_ppm_max;
	u32 rx_dfe_tap1;
	u32 rx_margin_iq;
	u32 rx_div16p5_clk_en;

	const struct xpcs_serdes_cfg *lane_cfg;

	u32 rx_vref_ctrl;

	int (*set_mode)(struct xpcs_prv_data *pdata);
};

struct xpcs_ops {
	int (*ethtool_ksettings_get)(struct device *dev,
				     struct ethtool_link_ksettings *cmd);
	int (*ethtool_ksettings_set)(struct device *dev,
				     const struct ethtool_link_ksettings *cmd);
	int (*power_off)(struct device *dev);
	int (*power_on)(struct device *dev);
	int (*reinit_xpcs)(struct device *dev, u32 mode);
	int (*xpcs_get_mode)(struct device *dev, u32 *mode_p);
};

struct xpcs_prv_data {
	u32 id;

	/* XPCS registers for indirect accessing */
	void __iomem *addr_base;

	struct device *dev;
	struct device *pair;	/* paired device to share same MPLL */
	u32 slave;		/* indicate paired device is slave device */
	struct phy *phy;
	struct clk *clk;

	/* syscon for XGMII clock */
	struct regmap *syscon;
	u32 syscon_offset;
	u32 syscon_bit;

	struct regmap *cbphy_app;

	/* XPCS Interface Name */
	const char *name;

	/* XPCS Mode 10G-KR, 1G SGMII, 2.5G SGMII, USXGMII-Quad */
	u32 mode;
	/* Mode configure table pointer, used by sysfs */
	const struct xpcs_mode_cfg *mode_cfg;

	/* IRQ number */
	u32 irq_num;

	/* Reset Signal */
	struct reset_control *xpcs_rst[4];

	/* power state
	 * 1 - power on
	 * 0 - power off
	 */
	int state;

	u32 speed;
	u32 duplex;

	u32 mpllb;
	u32 mpllb_dts;

	/* ANEG mode
	 * 0 - disable
	 * 1 - CL37 enabled and configured in SGMII MAC Side
	 * 2 - CL37 enabled and configured in SGMII PHY Side
	 * 3 - CL37 enabled (BaseX mode)
	 */
	u32 cl37[XPCS_MODE_MAX];

	/* ANEG mode
	 * 0 - disable
	 * 1 - CL73 enable
	 */
	u32 cl73[XPCS_MODE_MAX];

	/* SERDES RX/TX EQ configures */
	struct xpcs_serdes_cfg lane_cfg[XPCS_MODE_MAX];

	/* WAN workaround */
	u32 wan_wa;

	const char *e16_fw_name;

	struct xpcs_ops ops;
#if IS_ENABLED(CONFIG_MXL_P34X_FWDL)
	struct platform_device *phydev;
	int (*phy_reset)(struct platform_device *pdev);
#endif
};

#define xpcs_r32(reg)		readl(reg)
#define xpcs_w32(val, reg)	writel(val, reg)

static inline u32 XPCS_RGRD(struct xpcs_prv_data *pdata, u32 reg)
{
	u32 reg_val;
	u32 shift_addr;

	void __iomem *addr_reg  =
		(void __iomem *)((unsigned long)pdata->addr_base + (0xFF << 2));
	void __iomem *data_reg  =
		(void __iomem *)((unsigned long)pdata->addr_base | (reg & 0x000003FF));

	shift_addr = (u32)(((reg >> 2) & 0x001FFF00) >> 8);

	xpcs_w32(shift_addr, addr_reg);
	reg_val = xpcs_r32(data_reg);

	/* Dummy register read */
	xpcs_w32(shift_addr, addr_reg);
	reg_val = xpcs_r32(data_reg);

	return reg_val;
}

static inline void XPCS_RGWR(struct xpcs_prv_data *pdata, u32 reg, u32 val)
{
	u32 shift_addr;
	u32 reg_val;
	void __iomem *addr_reg  =
		(void __iomem *)((unsigned long)pdata->addr_base + (0xFF << 2));
	void __iomem *data_reg  =
		(void __iomem *)((unsigned long)pdata->addr_base | (reg & 0x000003FF));

	shift_addr = (u32)(((reg >> 2) & 0x001FFF00) >> 8);

	xpcs_w32(shift_addr, addr_reg);
	xpcs_w32(val, data_reg);

	/* Dummy register read */
	xpcs_w32(shift_addr, addr_reg);
	reg_val = xpcs_r32(data_reg);
}

#define GET_N_BITS(reg, pos, n)						\
	(((reg) >> (pos)) & ((BIT(n)) - 1))

#define SET_N_BITS(reg, pos, n, val)					\
	do {								\
		(reg) &= ~((BIT(n) - 1) << (pos));			\
		(reg) |= (((val) & (BIT(n) - 1)) << (pos));		\
	} while (0)

#define XPCS_GET_VAL(var, reg, field)					\
	GET_N_BITS((var),						\
		   reg##_##field##_POS,					\
		   reg##_##field##_WIDTH)

#define XPCS_SET_VAL(var, reg, field, val)				\
	SET_N_BITS((var),						\
		   reg##_##field##_POS,					\
		   reg##_##field##_WIDTH, (val))

#define XPCS_RGRD_VAL(pdata, reg, field)				\
	GET_N_BITS(XPCS_RGRD(pdata, reg),				\
		   reg##_##field##_POS,					\
		   reg##_##field##_WIDTH)

#define XPCS_RGWR_VAL(pdata, reg, field, _val)				\
	do {								\
		u32 reg_val = XPCS_RGRD(pdata, reg);			\
		SET_N_BITS(reg_val,					\
			   reg##_##field##_POS,				\
			   reg##_##field##_WIDTH, (_val));		\
		XPCS_RGWR(pdata, reg, reg_val);				\
	} while (0)

#define XPCS_RGWR_VAL_OFF_SHIFT(pdata, reg, field, _val, off, shift)	\
	do {								\
		u32 reg_val = XPCS_RGRD(pdata, (reg) + (off) * 4);	\
		SET_N_BITS(reg_val,					\
			   reg##_##field##_POS + (shift),		\
			   reg##_##field##_WIDTH, (_val));		\
		XPCS_RGWR(pdata, (reg) + (off) * 4, reg_val);		\
	} while (0)

#define XPCS_RGWR_VAL_OFF(pdata, reg, field, _val, off)			\
	do {								\
		u32 reg_val = XPCS_RGRD(pdata, (reg) + (off) * 4);	\
		SET_N_BITS(reg_val,					\
			   reg##_##field##_POS,				\
			   reg##_##field##_WIDTH, (_val));		\
		XPCS_RGWR(pdata, (reg) + (off) * 4, reg_val);		\
	} while (0)

#define XPCS_RGWR_VAL_SHIFT(pdata, reg, field, _val, shift)		\
	do {								\
		u32 reg_val = XPCS_RGRD(pdata, reg);			\
		SET_N_BITS(reg_val,					\
			   reg##_##field##_POS + (shift),		\
			   reg##_##field##_WIDTH, (_val));		\
		XPCS_RGWR(pdata, reg, reg_val);				\
	} while (0)

static inline u32 xpcs_reg_update(struct xpcs_prv_data *pdata, u32 reg,
				  u32 mask, u32 val)
{
	u32 reg_val = XPCS_RGRD(pdata, reg);

	reg_val = (reg_val & ~mask) | val;
	XPCS_RGWR(pdata, reg, reg_val);

	return reg_val;
}

void xpcs_disable_an(struct xpcs_prv_data *pdata);
void xpcs_cl37_an(struct xpcs_prv_data *pdata);
int xpcs_reinit(struct device *dev, u32 mode);
int xpcs_sysfs_init(struct xpcs_prv_data *priv);

static inline struct xpcs_ops *xpcs_pdev_ops(struct platform_device *pdev)
{
	struct xpcs_prv_data *pdata;

	if (!pdev)
		return ERR_PTR(-EINVAL);

	pdata = platform_get_drvdata(pdev);
	return pdata ? &pdata->ops : NULL;
}

static inline const struct xpcs_hw_config *xpcs_hw_config(struct xpcs_prv_data *pdata)
{
	return of_device_get_match_data(pdata->dev);
}

static inline bool xpcs_hw_is_e12(struct xpcs_prv_data *pdata)
{
	const struct xpcs_hw_config *cfg = xpcs_hw_config(pdata);

	return cfg && cfg->phy_type == XPCS_PHY_TYPE_12G;
}

static inline bool xpcs_hw_is_e16(struct xpcs_prv_data *pdata)
{
	const struct xpcs_hw_config *cfg = xpcs_hw_config(pdata);

	return cfg && cfg->phy_type == XPCS_PHY_TYPE_16G && cfg->rsvdparam11;
}

static inline bool xpcs_hw_is_c10(struct xpcs_prv_data *pdata)
{
	const struct xpcs_hw_config *cfg = xpcs_hw_config(pdata);

	return cfg && cfg->phy_type == XPCS_PHY_TYPE_8G;
}

#endif
