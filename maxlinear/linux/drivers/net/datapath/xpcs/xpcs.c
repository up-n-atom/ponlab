// SPDX-License-Identifier: GPL-2.0
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

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/phy/phy.h>
#include <linux/reset.h>
#include <linux/netdevice.h>
#include <linux/clk.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/firmware.h>
#include "xpcs.h"
#include "xpcs_regs.h"
#if IS_ENABLED(CONFIG_MXL_P34X_FWDL)
#include "../p34x/p34x_phy.h"
#endif

#define REG_DUMP(x)		dev_dbg(pdata->dev,			\
					#x " (0x%06x) = 0x%08x\n",	\
					x, XPCS_RGRD(pdata, x))
#define REG_DUMP_PAIR(x)	dev_dbg(pair_pdata->dev,		\
					#x " (0x%06x) = 0x%08x\n",	\
					x, XPCS_RGRD(pair_pdata, x))
#define REG_DUMP_NAME(n, x)	dev_dbg(pdata->dev,			\
					"%s (0x%06x) = 0x%08x\n",	\
					n, x, XPCS_RGRD(pdata, x))
#define DBG_UPDATE_CFG		0

#define MAX_BUSY_RETRY		2000
#define XPCS_MOD_NAME		"xpcs-name"
#define XPCS_MODE_NAME		"xpcs-mode"
#define XPCS_CL73		"xpcs-cl73"
#define XPCS_CL37		"xpcs-cl37"
#define XPCS_SLAVE		"slave"
#define XPCS_MPLLB		"mpllb"
#define XPCS_WAN_WA		"wan_wa"
#define XPCS_E16_FW_NAME	"e16_fw_name"

#define XPCS_RX_ATTN_LVL	"rx_attn_lvl"
#define XPCS_CTLE_BOOST		"rx_ctle_boost"
#define XPCS_CTLE_POLE		"rx_ctle_pole"
#define XPCS_VGA1_GAIN		"rx_vga1_gain"
#define XPCS_VGA2_GAIN		"rx_vga2_gain"
#define XPCS_DFE_BYPASS		"rx_dfe_bypass"
#define XPCS_ADAPT_EN		"cont_adapt_en"
#define XPCS_RX_CALI		"rx_cali"
#define XPCS_TX_VBOOST_EN	"tx_vboost_en"
#define XPCS_TX_VBOOST_LVL	"tx_vboost_lvl"
#define XPCS_TX_IBOOST_LVL	"tx_iboost_lvl"
#define XPCS_TX_EQ_MAIN		"tx_eq_main"
#define XPCS_TX_EQ_PRE		"tx_eq_pre"
#define XPCS_TX_EQ_POST		"tx_eq_post"
#define XPCS_TX_EQ_OVRRIDE	"tx_eq_ovrride"

#define XGMII_CLK_SYSCON	"lineclk,syscon"
#define XGMII_CLK_OFFSET	"lineclk,offset"
#define XGMII_CLK_BIT		"lineclk,bit"

#define XPCS_CBPHY_APP_SYSCON	"mxl,cbphy_app"

#if IS_ENABLED(CONFIG_PHY_MXL_WAN_XPCS)
extern void phy_serdes_dig_ovrd(struct phy *phy);
#endif

#if IS_ENABLED(CONFIG_MXL_P34X_FWDL)
static int xpcs_p34x_phy(struct platform_device *pdev)
{
	return pdev ? of_device_is_compatible(pdev->dev.of_node, P34X_COMPATIBLE)
		    : 0;
}
#endif

static void xpcs_an(struct xpcs_prv_data *pdata);
static int xpcs_rxtx_stable(struct xpcs_prv_data *pdata);
static int xpcs_vs_reset(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_sgmii(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_1000basex(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_2500basex(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_3000basex(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_5gbaser(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_10gbaser(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_susxgmii(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_10gkr(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_qusxgmii(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_invalid(struct xpcs_prv_data *pdata);
static int xpcs_set_mode_run(struct xpcs_prv_data *pdata);

static const struct xpcs_serdes_cfg lane_cfg_tbl[XPCS_MODE_MAX] = {
	[XPCS_MODE_SGMII] = {
		/* XPCS_MODE_SGMII */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x4452 */
		.rx_ctle_boost = 18,
		.rx_ctle_pole = 0,
		.rx_vga1_gain = 4,
		.rx_vga2_gain = 4,

		.rx_dfe_bypass = 1,
		.cont_adapt = 0,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 16,
		.tx_eq_pre = 4,
		.tx_eq_post = 4,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_1000BASEX] = {
		/* XPCS_MODE_1000BASEX */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x4452 */
		.rx_ctle_boost = 18,
		.rx_ctle_pole = 0,
		.rx_vga1_gain = 4,
		.rx_vga2_gain = 4,

		.rx_dfe_bypass = 1,
		.cont_adapt = 0,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 16,
		.tx_eq_pre = 4,
		.tx_eq_post = 4,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_2500BASEX] = {
		/* XPCS_MODE_2500BASEX */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x4412 */
		.rx_ctle_boost = 18,
		.rx_ctle_pole = 0,
		.rx_vga1_gain = 4,
		.rx_vga2_gain = 4,

		.rx_dfe_bypass = 1,
		.cont_adapt = 0,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 19,
		.tx_eq_pre = 4,
		.tx_eq_post = 4,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_5GBASER] = {
		/* XPCS_MODE_5GBASER */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x5500 */
		.rx_ctle_boost = 16,
		.rx_ctle_pole = 2,
		.rx_vga1_gain = 5,
		.rx_vga2_gain = 5,

		.rx_dfe_bypass = 0,
		.cont_adapt = 1,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 28,
		.tx_eq_pre = 16,
		.tx_eq_post = 32,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_10GBASER] = {
		/* XPCS_MODE_10GBASER */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x5500 */
		.rx_ctle_boost = 16,
		.rx_ctle_pole = 2,
		.rx_vga1_gain = 5,
		.rx_vga2_gain = 5,

		.rx_dfe_bypass = 0,
		.cont_adapt = 1,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 28,
		.tx_eq_pre = 16,
		.tx_eq_post = 32,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_SUSXGMII] = {
		/* XPCS_MODE_SUSXGMII */
		.rx_attn_lvl = 7,
		/* RX_EQ_CTRL0 = 0x0000 */
		.rx_ctle_boost = 0,
		.rx_ctle_pole = 2,
		.rx_vga1_gain = 5,
		.rx_vga2_gain = 5,

		.rx_dfe_bypass = 1,
		.cont_adapt = 1,

		.rx_cali = 0,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 40,
		.tx_eq_pre = 0,
		.tx_eq_post = 0,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_10GKR] = {
		/* XPCS_MODE_10GKR */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x5500 */
		.rx_ctle_boost = 16,
		.rx_ctle_pole = 2,
		.rx_vga1_gain = 5,
		.rx_vga2_gain = 5,

		.rx_dfe_bypass = 0,
		.cont_adapt = 1,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 28,
		.tx_eq_pre = 16,
		.tx_eq_post = 32,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_QUSXGMII] = {
		/* XPCS_MODE_QUSXGMII */
		.rx_attn_lvl = 7,
		/* RX_EQ_CTRL0 = 0x0000 */
		.rx_ctle_boost = 0,
		.rx_ctle_pole = 2,
		.rx_vga1_gain = 5,
		.rx_vga2_gain = 5,

		.rx_dfe_bypass = 1,
		.cont_adapt = 1,

		.rx_cali = 0,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 40,
		.tx_eq_pre = 0,
		.tx_eq_post = 0,
		.tx_eq_ovrride = 1,
	},
	[XPCS_MODE_3000BASEX] = {
		/* XPCS_MODE_3000BASEX */
		.rx_attn_lvl = 0,
		/* RX_EQ_CTRL0 = 0x4452 */
		.rx_ctle_boost = 18,
		.rx_ctle_pole = 0,
		.rx_vga1_gain = 4,
		.rx_vga2_gain = 4,

		.rx_dfe_bypass = 1,
		.cont_adapt = 0,

		.rx_cali = 1,

		.tx_vboost_en = 1,
		.tx_vboost_lvl = 5,
		.tx_iboost_lvl = 15,
		.tx_eq_main = 19,
		.tx_eq_pre = 4,
		.tx_eq_post = 4,
		.tx_eq_ovrride = 1,
	},
};

static const struct xpcs_mode_cfg mode_cfg_tbl[XPCS_MODE_MAX] = {
	[XPCS_MODE_SGMII] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x20,
		.mplla_bw = 0xA035,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		.mpllb_mult = 0x30,
		.mpllb_bw = 0xA017,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 1,
		.mpllb_div_mult = 30,

		.vco_ld_val = 0x0540,
		.vco_ref_ld = 0x002a,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 3,
		.rx_rate = 3,
		.tx_width = 1,	/* 2'b01 - 10 bit */
		.rx_width = 1,	/* 2'b01 - 10 bit */
		.rx_misc = 22,
		.rx_delta_iq = 0,

		.los_thr = 4,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_SGMII],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd17 */

		.set_mode = xpcs_set_mode_sgmii,
	},
	[XPCS_MODE_1000BASEX] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x20,
		.mplla_bw = 0xA035,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		.mpllb_mult = 0x30,
		.mpllb_bw = 0xA017,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 1,
		.mpllb_div_mult = 30,

		.vco_ld_val = 0x0540,
		.vco_ref_ld = 0x002a,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 3,
		.rx_rate = 3,
		.tx_width = 1,	/* 2'b01 - 10 bit */
		.rx_width = 1,	/* 2'b01 - 10 bit */
		.rx_misc = 22,
		.rx_delta_iq = 0,

		.los_thr = 4,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_1000BASEX],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd17 */

		.set_mode = xpcs_set_mode_1000basex,
	},
	[XPCS_MODE_2500BASEX] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1, /* REF_RPT_CLK_EN was enabled in FMX */

		.mplla_mult = 0x28,
		.mplla_bw = 0xA017,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		/* can not select MPLL B */
		.mpllb_mult = 0x28,
		.mpllb_bw = 0xA017,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.vco_ld_val = 0x550,
		.vco_ref_ld = 0x22,
		.vco_temp_comp_en = 0,
		.vco_step_ctrl = 1,
		.vco_frqband = 2,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 2, /* 3'b010 : baud/4 */
		.rx_rate = 2, /* 2'b10 : baud/4 */
		.tx_width = 1, /* 2'b01: 10-bit; 2'b11: 20-bit */
		.rx_width = 1, /* 2'b01: 10-bit; 2'b11: 20-bit */
		.rx_misc = 23,
		.rx_delta_iq = 0,

		.los_thr = 4,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_2500BASEX],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd15 */

		.set_mode = xpcs_set_mode_2500basex,
	},
	[XPCS_MODE_5GBASER] = {
		.pma_type = PMA_TYPE_10G_BASE_KR,
		.pcs_type = PCS_TYPE_10G_BASE_R,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x21,
		.mplla_bw = 0xA016,
		.mplla_div165_clk_en = 1,
		.mplla_div10_clk_en = 0,
		.mplla_div8_clk_en = 1,

		/* can not select MPLL B */
		.mpllb_mult = 0,
		.mpllb_bw = 0,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 0,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.vco_ld_val = 0x549,
		.vco_ref_ld = 0x29,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 3,
		.tx_rate = 0,
		.rx_rate = 0,
		.tx_width = 2,	/* 2'b10 - 16 bit */
		.rx_width = 2,	/* 2'b10 - 16 bit */
		.rx_misc = 18,
		.rx_delta_iq = 0,

		.los_thr = 7,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_5GBASER],

		.rx_vref_ctrl = 17,

		.set_mode = xpcs_set_mode_5gbaser,
	},
	[XPCS_MODE_10GBASER] = {
		.pma_type = PMA_TYPE_10G_BASE_KR,
		.pcs_type = PCS_TYPE_10G_BASE_R,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x21,
		.mplla_bw = 0xA016,
		.mplla_div165_clk_en = 1,
		.mplla_div10_clk_en = 0,
		.mplla_div8_clk_en = 1,

		/* can not select MPLL B */
		.mpllb_mult = 0,
		.mpllb_bw = 0,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 0,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.vco_ld_val = 0x549,
		.vco_ref_ld = 0x29,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 3,
		.afe_en = 1,
		.dfe_en = 1,
		.tx_rate = 0,
		.rx_rate = 0,
		.tx_width = 2,	/* 2'b10 - 16 bit */
		.rx_width = 2,	/* 2'b10 - 16 bit */
		.rx_misc = 18,
		.rx_delta_iq = 0,

		.los_thr = 7,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_10GBASER],

		.rx_vref_ctrl = 17,

		.set_mode = xpcs_set_mode_10gbaser,
	},
	[XPCS_MODE_SUSXGMII] = {
		.pma_type = PMA_TYPE_10G_BASE_KR,
		.pcs_type = PCS_TYPE_10G_BASE_R,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x21,
		.mplla_bw = 0xA016,
		.mplla_div165_clk_en = 1,
		.mplla_div10_clk_en = 0,
		.mplla_div8_clk_en = 1,

		/* can not select MPLL B */
		.mpllb_mult = 0,
		.mpllb_bw = 0,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 0,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.vco_ld_val = 0x549,
		.vco_ref_ld = 0x29,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 3,
		.afe_en = 1,
		.dfe_en = 1,
		.tx_rate = 0,
		.rx_rate = 0,
		.tx_width = 2,	/* 2'b10 - 16 bit */
		.rx_width = 2,	/* 2'b10 - 16 bit */
		.rx_misc = 18,
		.rx_delta_iq = 0,

		.los_thr = 7,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_SUSXGMII],

		.rx_vref_ctrl = 17,

		.set_mode = xpcs_set_mode_susxgmii,
	},
	[XPCS_MODE_10GKR] = {
		.pma_type = PMA_TYPE_10G_BASE_KR,
		.pcs_type = PCS_TYPE_10G_BASE_R,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x21,
		.mplla_bw = 0xA016,
		.mplla_div165_clk_en = 1,
		.mplla_div10_clk_en = 0,
		.mplla_div8_clk_en = 1,

		/* can not select MPLL B */
		.mpllb_mult = 0,
		.mpllb_bw = 0,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 0,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.vco_ld_val = 0x549,
		.vco_ref_ld = 0x29,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 3,
		.afe_en = 1,
		.dfe_en = 1,
		.tx_rate = 0,
		.rx_rate = 0,
		.tx_width = 2,	/* 2'b10 - 16 bit */
		.rx_width = 2,	/* 2'b10 - 16 bit */
		.rx_misc = 18,
		.rx_delta_iq = 0,

		.los_thr = 7,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_10GKR],

		.rx_vref_ctrl = 17,

		.set_mode = xpcs_set_mode_10gkr,
	},
	[XPCS_MODE_QUSXGMII] = {
		.pma_type = PMA_TYPE_10G_BASE_KR,
		.pcs_type = PCS_TYPE_10G_BASE_R,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x21,
		.mplla_bw = 0xA016,
		.mplla_div165_clk_en = 1,
		.mplla_div10_clk_en = 0,
		.mplla_div8_clk_en = 1,

		/* can not select MPLL B */
		.mpllb_mult = 0,
		.mpllb_bw = 0,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 0,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.vco_ld_val = 0x549,
		.vco_ref_ld = 0x29,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 3,
		.tx_rate = 0,
		.rx_rate = 0,
		.tx_width = 2,	/* 2'b10 - 16 bit */
		.rx_width = 2,	/* 2'b10 - 16 bit */
		.rx_misc = 18,
		.rx_delta_iq = 0,

		.los_thr = 7,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_QUSXGMII],

		.rx_vref_ctrl = 17,

		.set_mode = xpcs_set_mode_qusxgmii,
	},
	[XPCS_MODE_3000BASEX] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x0F1,

		.mplla_mult = 0x20,
		.mplla_bw = 0xA035,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		.mpllb_mult = 0x30,
		.mpllb_bw = 0xA017,
		.mpllb_tx_clk_div = 1,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 1,
		.mpllb_div_mult = 10,

		.vco_ld_val = 0x0540,
		.vco_ref_ld = 0x001C,
		.vco_temp_comp_en = 0,
		.vco_step_ctrl = 0,
		.vco_frqband = 2,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 1,
		.rx_rate = 2,
		.tx_width = 1,	/* 2'b01 - 10 bit */
		.rx_width = 1,	/* 2'b01 - 10 bit */
		.rx_misc = 20,
		.rx_delta_iq = 0,

		.los_thr = 4,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 0,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_3000BASEX],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd17 */

		.set_mode = xpcs_set_mode_3000basex,
	},
};

static const struct xpcs_mode_cfg mode_cfg_tbl_c10[XPCS_MODE_MAX] = {
	[XPCS_MODE_SGMII] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x094,

		.mplla_mult = 0x20,
		.mplla_bw = 0xA035,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		.mpllb_mult = 0x60,
		.mpllb_bw = 0x3D,
		.mpllb_tx_clk_div = 4,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 30,

		.mpllb_tx_clk_div_pcie3_app = 4,
		.mpllb_div_mult_pcie3_app = 0,
		.mpllb_force_en_ovrd_pcie3_app = 0,
		.mpllb_ssc_clk_sel_pcie3_app = 0,
		.mpllb_cdr_vco_low_freq_pcie3_app = 0,

		.vco_ld_val = 0x0540,
		.vco_ref_ld = 0x0015,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 2,
		.rx_rate = 3,
		.tx_width = 1,	/* 2'b01 - 10 bit */
		.rx_width = 1,	/* 2'b01 - 10 bit */
		.rx_misc = 6,
		.rx_delta_iq = 0,

		.los_thr = 3,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_SGMII],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd17 */

		.set_mode = xpcs_set_mode_sgmii,
	},
	[XPCS_MODE_1000BASEX] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x094,

		.mplla_mult = 0x20,
		.mplla_bw = 0xA035,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		.mpllb_mult = 0x60,
		.mpllb_bw = 0x3D,
		.mpllb_tx_clk_div = 4,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 1,
		.mpllb_div_mult = 30,

		.mpllb_tx_clk_div_pcie3_app = 4,
		.mpllb_div_mult_pcie3_app = 0,
		.mpllb_force_en_ovrd_pcie3_app = 0,
		.mpllb_ssc_clk_sel_pcie3_app = 0,
		.mpllb_cdr_vco_low_freq_pcie3_app = 0,

		.vco_ld_val = 0x0540,
		.vco_ref_ld = 0x0015,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 2,
		.rx_rate = 3,
		.tx_width = 1,	/* 2'b01 - 10 bit */
		.rx_width = 1,	/* 2'b01 - 10 bit */
		.rx_misc = 6,
		.rx_delta_iq = 0,

		.los_thr = 3,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_1000BASEX],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd17 */

		.set_mode = xpcs_set_mode_1000basex,
	},
	[XPCS_MODE_2500BASEX] = {
		.pma_type = PMA_TYPE_1G_BASE_KX,
		.pcs_type = PCS_TYPE_10G_BASE_X,
		.lane = 1,

		.ref_clk_ctrl = 0x094, /* REF_RPT_CLK_EN was enabled in FMX */

		.mplla_mult = 0x28,
		.mplla_bw = 0xA017,
		.mplla_div165_clk_en = 0,
		.mplla_div10_clk_en = 1,
		.mplla_div8_clk_en = 0,

		/* can not select MPLL B */
		.mpllb_mult = 0x78,
		.mpllb_bw = 0x4D,
		.mpllb_tx_clk_div = 0,
		.mpllb_div10_clk_en = 1,
		.mpllb_div8_clk_en = 0,
		.mpllb_div_clk_en = 0,
		.mpllb_div_mult = 0,

		.mpllb_tx_clk_div_pcie3_app = 6,
		.mpllb_div_mult_pcie3_app = 0,
		.mpllb_force_en_ovrd_pcie3_app = 0,
		.mpllb_ssc_clk_sel_pcie3_app = 0,
		.mpllb_cdr_vco_low_freq_pcie3_app = 1,

		.vco_ld_val = 0x550,
		.vco_ref_ld = 0x22,
		.vco_temp_comp_en = 1,
		.vco_step_ctrl = 1,
		.vco_frqband = 1,
		.adpt_sel = 0,
		.adpt_mode = 0,
		.tx_rate = 0,
		.rx_rate = 1,
		.tx_width = 1,
		.rx_width = 1,
		.rx_misc = 7,
		.rx_delta_iq = 0,

		.los_thr = 3,
		.cdr_ppm_max = 18,
		.rx_dfe_tap1 = 0,
		.rx_margin_iq = 0,
		.rx_div16p5_clk_en = 1,

		.lane_cfg = &lane_cfg_tbl[XPCS_MODE_2500BASEX],

		.rx_vref_ctrl = 17,	/* Setting BIT12:8 = 5'd15 */

		.set_mode = xpcs_set_mode_2500basex,
	},
	[XPCS_MODE_5GBASER] = {
		.set_mode = xpcs_set_mode_invalid,
	},
	[XPCS_MODE_10GBASER] = {
		.set_mode = xpcs_set_mode_invalid,
	},
	[XPCS_MODE_SUSXGMII] = {
		.set_mode = xpcs_set_mode_invalid,
	},
	[XPCS_MODE_10GKR] = {
		.set_mode = xpcs_set_mode_invalid,
	},
	[XPCS_MODE_QUSXGMII] = {
		.set_mode = xpcs_set_mode_invalid,
	},
	[XPCS_MODE_3000BASEX] = {
		.set_mode = xpcs_set_mode_invalid,
	},
};

static const u32 def_cl73[XPCS_MODE_MAX] = {
	[XPCS_MODE_SGMII] = 0,
	[XPCS_MODE_1000BASEX] = 0,
	[XPCS_MODE_2500BASEX] = 0,
	[XPCS_MODE_5GBASER] = 0,
	[XPCS_MODE_10GBASER] = 0,
	[XPCS_MODE_SUSXGMII] = 0,
	[XPCS_MODE_10GKR] = 1,
	[XPCS_MODE_QUSXGMII] = 0,
	[XPCS_MODE_3000BASEX] = 0,
};

static const u32 def_cl37[XPCS_MODE_MAX] = {
	[XPCS_MODE_SGMII] = CONN_TYPE_ANEG_MAC,
	[XPCS_MODE_1000BASEX] = CONN_TYPE_ANEG_BX,
	[XPCS_MODE_2500BASEX] = CONN_TYPE_ANEG_DIS,
	[XPCS_MODE_5GBASER] = CONN_TYPE_ANEG_DIS,
	[XPCS_MODE_10GBASER] = CONN_TYPE_ANEG_DIS,
	[XPCS_MODE_SUSXGMII] = CONN_TYPE_ANEG_MAC,
	[XPCS_MODE_10GKR] = CONN_TYPE_ANEG_DIS,
	[XPCS_MODE_QUSXGMII] = CONN_TYPE_ANEG_MAC,
	[XPCS_MODE_3000BASEX] = CONN_TYPE_ANEG_DIS,
};

static void xpcs_phy_config(struct xpcs_prv_data *pdata, unsigned int reg,
			    unsigned int offset, unsigned int mask,
			    unsigned int val)
{
	if (xpcs_hw_is_c10(pdata) && !IS_ERR_OR_NULL(pdata->cbphy_app)) {
		dev_dbg(pdata->dev, "%s: %s reg 0x%x offset %d mask 0x%x val 0x%x\n",
			pdata->name, __func__, reg, offset, mask, val);
		regmap_update_bits(pdata->cbphy_app, reg, mask << offset, val << offset);
	}
}

static void xpcs_phy_read(struct xpcs_prv_data *pdata, unsigned int reg,
			  unsigned int *val)
{
	if (xpcs_hw_is_c10(pdata) && !IS_ERR_OR_NULL(pdata->cbphy_app)) {
		regmap_read(pdata->cbphy_app, reg, val);
		dev_dbg(pdata->dev, "%s: %s reg 0x%x val 0x%x\n",
			pdata->name, __func__, reg, *val);
	}
}

static void xpcs_set_mode(struct xpcs_prv_data *pdata, u32 mode)
{
	pdata->mode = mode;
	if (mode < XPCS_MODE_MAX)
		pdata->mode_cfg = xpcs_hw_is_c10(pdata) ?
				  &mode_cfg_tbl_c10[mode] : &mode_cfg_tbl[mode];

	dev_dbg(pdata->dev, "%s: %s c10_hw: %d\n",
		pdata->name, __func__, xpcs_hw_is_c10(pdata));
}

static int xpcs_cr_busy(struct xpcs_prv_data *pdata)
{
	int i;

	for (i = 0; i < MAX_BUSY_RETRY; i++) {
		if (!(XPCS_RGRD(pdata, PMA_SNPS_CR_CTRL) &
		      BIT(PMA_SNPS_CR_CTRL_START_BUSY_POS)))
			return 0;
		usleep_range(1, 2);
	}

	dev_err(pdata->dev, "CR Port busy\n");

	return -EBUSY;
}

static int xpcs_cr_access(struct xpcs_prv_data *pdata, u32 addr,
			  u32 data, u32 write)
{
	u32 val = 0;

	if (!xpcs_cr_busy(pdata)) {
		XPCS_RGWR(pdata, PMA_SNPS_CR_ADDR, addr);
		if (write)
			XPCS_RGWR(pdata, PMA_SNPS_CR_DATA, data);

		XPCS_SET_VAL(val, PMA_SNPS_CR_CTRL, WR_RDN, write);
		XPCS_SET_VAL(val, PMA_SNPS_CR_CTRL, START_BUSY, 1);
		XPCS_RGWR(pdata, PMA_SNPS_CR_CTRL, val);

		if (!xpcs_cr_busy(pdata) && !write)
			val = XPCS_RGRD(pdata, PMA_SNPS_CR_DATA);
	}

	return val;
}

static void sram_ld_fw(struct xpcs_prv_data *pdata)
{
	int i, ret;
	u16 *fw_data;
	u32 cr_addr;
	const struct firmware *fw;

	ret = request_firmware(&fw, pdata->e16_fw_name, pdata->dev);
	if (ret) {
		dev_err(pdata->dev, "failed to load e16 fw: %s\n", pdata->e16_fw_name);
		return;
	}

	fw_data = (u16 *)fw->data;
	for (i = 0; i < fw->size/2; i++) {
		cr_addr = E16_RAWMEM_DIG_RAM_CMN0_B0_R0 + i;
		xpcs_cr_access(pdata, cr_addr, fw_data[i], 1);
	}

	release_firmware(fw);
	dev_dbg(pdata->dev, "E16 FW loading done\n");
}

static int sram_init_chk(struct xpcs_prv_data *pdata)
{
	const struct xpcs_hw_config *cfg = xpcs_hw_config(pdata);
	u32 val;
	int i;

	if (cfg && !cfg->has_sram)
		return 0;

	for (i = 0; i < MAX_BUSY_RETRY; i++) {
		if (xpcs_hw_is_c10(pdata)) {
			/* PCIE_PHY_SRAM_CSR: 0x8 - SRAM_INIT_DONE(2): 1 */
			xpcs_phy_read(pdata, 0x8, &val);
			if (val & BIT(2))
				break;
		} else {
			val = XPCS_RGRD(pdata, PMA_SRAM);
			if (val & BIT(0))
				break;
		}
		usleep_range(10, 20);
	}

	if (i >= MAX_BUSY_RETRY) {
		dev_err(pdata->dev,
			"%s: SRAM init check timeout: PMA_SRAM (0x%06x) = 0x%08x, loops %i\n",
			pdata->name, PMA_SRAM, val, i);
		return -EIO;
	}

	dev_dbg(pdata->dev, "%s: SRAM init check succeed\n", pdata->name);
	return 0;
}

static void sram_ext_ld_done(struct xpcs_prv_data *pdata, int done)
{
	const struct xpcs_hw_config *cfg = xpcs_hw_config(pdata);

	if (cfg && !cfg->has_sram)
		return;

	if (xpcs_hw_is_c10(pdata))
		/* PCIE_PHY_SRAM_CSR: 0x8 - SRAM_EXT_LD_DONE(1): 1 */
		xpcs_phy_config(pdata, 0x8, 1, 0x1, done ? 0x1 : 0x0);
	else
		xpcs_reg_update(pdata, PMA_SRAM, BIT(1), done ? BIT(1) : 0);
}

static void sram_bypass_dis(struct xpcs_prv_data *pdata, int disable)
{
	xpcs_reg_update(pdata, PCS_GPIO, BIT(14), disable ? BIT(14) : 0);
}

static int xpcs_synphy_reset_sts(struct xpcs_prv_data *pdata)
{
	u32 reset = 0;
	u32 idx = 0;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	/* Make sure Synopsys PHY is out of reset */
	do {
		reset = XPCS_RGRD_VAL(pdata, PCS_CTRL1, RST);
		if (reset == 0)
			return 0;

		idx++;

		/* To put a small delay and make sure previous operations
		 * are complete
		 */
		usleep_range(10, 20);
	} while (idx <= MAX_BUSY_RETRY);

	dev_err(pdata->dev, "%s: Synopsys PHY reset check reached %d times\n",
		pdata->name, MAX_BUSY_RETRY);
	return -EIO;
}

static int xpcs_byp_pwrupseq(struct xpcs_prv_data *pdata)
{
	int ret = 0;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, BYP_PWRUP, 1);

	if (xpcs_hw_is_e12(pdata)) {
		ret = xpcs_vs_reset(pdata);
		if (ret)
			return ret;

		ret = xpcs_rxtx_stable(pdata);
	} else if (xpcs_hw_is_c10(pdata)) {
		ret = xpcs_synphy_reset_sts(pdata);
	}

	return ret;
}

static int xpcs_enable_pwrupseq(struct xpcs_prv_data *pdata)
{
	int ret = 0;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, BYP_PWRUP, 0);

	if (xpcs_hw_is_e12(pdata)) {
		ret = xpcs_vs_reset(pdata);
		if (ret)
			return ret;

		/* This Synopsys PHY reset is selfcleared when PSEQ_STATE
		 * is equal to 3'b100, that is, Tx/Rx clocks are stable and in
		 * Power_Good state.
		 */
		ret = xpcs_synphy_reset_sts(pdata);
	}
	return ret;
}

static int xpcs_init_byp_pwrup(struct xpcs_prv_data *pdata)
{
	int ret = 0;

	ret = xpcs_byp_pwrupseq(pdata);
	if (ret)
		return ret;

	if (xpcs_hw_is_e12(pdata)) {
		/* Use internal clock */
		XPCS_RGWR_VAL(pdata, PMA_REF_CLK_CTRL, REF_USE_PAD, 0);

		/* Speed selection to config DWC_xpcs is in the 10GBASE-X PCS */
		XPCS_RGWR_VAL(pdata, PMA_CTRL1, SS13, 1);
		REG_DUMP(PMA_CTRL1);

		/* Set PMA_TYPE to 10GBASE-R PMA Type */
		XPCS_RGWR_VAL(pdata, PMA_CTRL2, PMA_TYPE,
			      pdata->mode_cfg->pma_type);
		REG_DUMP(PMA_CTRL2);
	}

	ret = xpcs_enable_pwrupseq(pdata);
	if (ret)
		return ret;

	return ret;
}

static int xpcs_rxtx_stable(struct xpcs_prv_data *pdata)
{
	int i = 0;
	u32 pseq_state = 0;
	u32 val;

	do {
		val = XPCS_RGRD_VAL(pdata, PCS_DIG_CTRL1, VR_RST);
		if (val) {
			usleep_range(10, 20);
			continue;
		}

		pseq_state = XPCS_RGRD_VAL(pdata, PCS_DIG_STS, PSEQ_STATE);

		if (pseq_state >= 4) {
			switch (pseq_state) {
			case 4:
				dev_dbg(pdata->dev,
					"%s: Tx/Rx Power Good state\n",
					pdata->name);
				break;
			case 5:
				dev_dbg(pdata->dev,
					"%s: Tx/Rx Power Save state\n",
					pdata->name);
				break;
			case 6:
				dev_dbg(pdata->dev,
					"%s: Tx/Rx Power Down state\n",
					pdata->name);
				break;
			default:
				dev_dbg(pdata->dev,
					"%s: Tx/Rx Power Unknown state %u\n",
					pdata->name, pseq_state);
			}
			break;
		}

		usleep_range(10, 20);
	} while (++i < MAX_BUSY_RETRY);

	if (i >= MAX_BUSY_RETRY) {
		dev_err(pdata->dev,
			"%s: TX/RX Stable timeout: PCS_DIG_CTRL1 (0x%06x) = 0x%08x, PCS_DIG_STS (0x%06x) = 0x%08x, loops %d\n",
			pdata->name,
			PCS_DIG_CTRL1, XPCS_RGRD(pdata, PCS_DIG_CTRL1),
			PCS_DIG_STS, XPCS_RGRD(pdata, PCS_DIG_STS), i);
		return -EIO;
	}

	dev_dbg(pdata->dev, "%s: %s succeed, total loop %d\n",
		pdata->name, __func__, i);

	return 0;
}

/* XPCS Vendor Specific Soft Reset
 * When the host sets this bit, the CSR block triggers the software reset
 * process in which all internal blocks are reset, except the Management
 * Interface block and CSR block.
 * When this bit is set, it also resets the PHY.
 * This bit is self cleared
 */
static int xpcs_vs_reset(struct xpcs_prv_data *pdata)
{
	int ret;
	u32 idx = 0;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	sram_ext_ld_done(pdata, 0);

	if (pdata->wan_wa)
		sram_bypass_dis(pdata, 1);

	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, VR_RST, 1);

	ret = sram_init_chk(pdata);
	if (ret)
		return ret;
	sram_ext_ld_done(pdata, 1);

#if IS_ENABLED(CONFIG_PHY_MXL_WAN_XPCS)
	if (pdata->wan_wa) {
		/* Re-calibate workaround for Resistor Tuning Calibration.
		 * (WAN only)
		 */
		BUG_ON(!pdata->phy);
		phy_serdes_dig_ovrd(pdata->phy);
		udelay(10);
	}
#endif

	do {
		if (!XPCS_RGRD_VAL(pdata, PCS_DIG_CTRL1, VR_RST))
			break;

		/* To put a small delay and make sure previous operations
		 * are complete
		 */
		usleep_range(10, 20);
	} while (++idx < MAX_BUSY_RETRY);

	if (idx >= MAX_BUSY_RETRY) {
		dev_err(pdata->dev,
			"%s: XPCS Vendor Specific Soft Reset (VR_RST) timeout: PCS_DIG_CTRL1 (0x%06x) = 0x%08x, loops %u\n",
			pdata->name, PCS_DIG_CTRL1,
			XPCS_RGRD(pdata, PCS_DIG_CTRL1), idx);
		return -EIO;
	}

	ret = xpcs_rxtx_stable(pdata);

	dev_dbg(pdata->dev, "%s: %s return %d\n",
		pdata->name, __func__, ret);

	return ret;
}

static void xpcs_cfg_mplla(struct xpcs_prv_data *pdata,
			   const struct xpcs_mode_cfg *mode_cfg)
{
	u32 val;

	XPCS_RGWR_VAL(pdata, PMA_MPLLA_C0, MPLLA_MULTIPLIER,
		      mode_cfg->mplla_mult);
	XPCS_RGWR_VAL(pdata, PMA_MPLLA_C0, MPLLA_CAL_DISABLE, 0);

	XPCS_RGWR_VAL(pdata, PMA_MPLLA_C3, MPPLA_BANDWIDTH,
		      mode_cfg->mplla_bw);

	val = XPCS_RGRD(pdata, PMA_MPLLA_C2);

	XPCS_SET_VAL(val, PMA_MPLLA_C2, MPLLA_DIV16P5_CLK_EN,
		     mode_cfg->mplla_div165_clk_en);
	XPCS_SET_VAL(val, PMA_MPLLA_C2, MPLLA_DIV10_CLK_EN,
		     mode_cfg->mplla_div10_clk_en);
	XPCS_SET_VAL(val, PMA_MPLLA_C2, MPLLA_DIV8_CLK_EN,
		     mode_cfg->mplla_div8_clk_en);

	XPCS_RGWR(pdata, PMA_MPLLA_C2, val);

	REG_DUMP(PMA_MPLLA_C0);
	REG_DUMP(PMA_MPLLA_C3);
	REG_DUMP(PMA_MPLLA_C2);
}

static void xpcs_cfg_mpllb(struct xpcs_prv_data *pdata,
			   const struct xpcs_mode_cfg *mode_cfg)
{
	u32 val;

	XPCS_RGWR_VAL(pdata, PMA_MPLLB_C0, MPLLB_MULTIPLIER,
		      mode_cfg->mpllb_mult);
	XPCS_RGWR_VAL(pdata, PMA_MPLLB_C0, MPLLB_CAL_DISABLE, 0);

	XPCS_RGWR_VAL(pdata, PMA_MPLLB_C3, MPPLB_BANDWIDTH,
		      mode_cfg->mpllb_bw);

	val = XPCS_RGRD(pdata, PMA_MPLLB_C2);

	XPCS_SET_VAL(val, PMA_MPLLB_C2, MPLLB_TX_CLK_DIV,
		     mode_cfg->mpllb_tx_clk_div);
	XPCS_SET_VAL(val, PMA_MPLLB_C2, MPLLB_DIV10_CLK_EN,
		     mode_cfg->mpllb_div10_clk_en);
	XPCS_SET_VAL(val, PMA_MPLLB_C2, MPLLB_DIV8_CLK_EN,
		     mode_cfg->mpllb_div8_clk_en);
	XPCS_SET_VAL(val, PMA_MPLLB_C2, MPLLB_DIV_CLK_EN,
		     mode_cfg->mpllb_div_clk_en);
	XPCS_SET_VAL(val, PMA_MPLLB_C2, MPLLB_DIV_MULT,
		     mode_cfg->mpllb_div_mult);

	XPCS_RGWR(pdata, PMA_MPLLB_C2, val);

	XPCS_RGWR_VAL(pdata, PMA_MPLLB_C1, MPLLB_SSC_EN, 0);
	XPCS_RGWR_VAL(pdata, PMA_MPLLB_C1, MPLLB_FRACN_CTRL, 0);

	REG_DUMP(PMA_MPLLB_C0);
	REG_DUMP(PMA_MPLLB_C3);
	REG_DUMP(PMA_MPLLB_C2);
	REG_DUMP(PMA_MPLLB_C1);
}

static void xpcs_cfg_clk(struct xpcs_prv_data *pdata)
{
	u32 val;

	dev_dbg(pdata->dev, "%s: %s, mpllb %u\n",
		pdata->name, __func__, pdata->mpllb);

	val = pdata->mode_cfg->ref_clk_ctrl;
	XPCS_RGWR(pdata, PMA_REF_CLK_CTRL, val);
	REG_DUMP(PMA_REF_CLK_CTRL);

	if (pdata->mpllb)
		xpcs_cfg_mpllb(pdata, pdata->mode_cfg);
	else
		xpcs_cfg_mplla(pdata, pdata->mode_cfg);

	val = pdata->lane_cfg[pdata->mode].tx_vboost_lvl;
	XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, VBOOST_LVL, val);
	REG_DUMP(PMA_TX_GENCTRL1);
}

static void xpcs_cfg_clk_pair(struct xpcs_prv_data *pdata,
			      struct xpcs_prv_data *pair_pdata)
{
	const struct xpcs_mode_cfg *mplla_mode_cfg = NULL;
	const struct xpcs_mode_cfg *mpllb_mode_cfg = NULL;
	u32 val;

	dev_dbg(pdata->dev, "%s: %s, mpllb %u\n",
		pdata->name, __func__, pdata->mpllb);

	val = pdata->mode_cfg->ref_clk_ctrl;
	XPCS_RGWR(pdata, PMA_REF_CLK_CTRL, val);
	REG_DUMP(PMA_REF_CLK_CTRL);

	val = pair_pdata->mode_cfg->ref_clk_ctrl;
	XPCS_RGWR(pair_pdata, PMA_REF_CLK_CTRL, val);
	REG_DUMP_PAIR(PMA_REF_CLK_CTRL);

	if (pdata->mpllb) {
		mpllb_mode_cfg = pdata->mode_cfg;
		if (!pair_pdata->mpllb)
			mplla_mode_cfg = pair_pdata->mode_cfg;
	} else {
		mplla_mode_cfg = pdata->mode_cfg;
		if (pair_pdata->mpllb)
			mpllb_mode_cfg = pair_pdata->mode_cfg;
	}

	if (mplla_mode_cfg) {
		xpcs_cfg_mplla(pdata, mplla_mode_cfg);
		xpcs_cfg_mplla(pair_pdata, mplla_mode_cfg);
	}

	if (mpllb_mode_cfg) {
		xpcs_cfg_mpllb(pdata, mpllb_mode_cfg);
		xpcs_cfg_mpllb(pair_pdata, mpllb_mode_cfg);
	}

	val = pdata->lane_cfg[pdata->mode].tx_vboost_lvl;
	XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, VBOOST_LVL, val);
	REG_DUMP(PMA_TX_GENCTRL1);

	val = pair_pdata->lane_cfg[pair_pdata->mode].tx_vboost_lvl;
	XPCS_RGWR_VAL(pair_pdata, PMA_TX_GENCTRL1, VBOOST_LVL, val);
	REG_DUMP_PAIR(PMA_TX_GENCTRL1);
}

static void xpcs_cfg_table(struct xpcs_prv_data *pdata)
{
	const struct xpcs_mode_cfg *mode_cfg = pdata->mode_cfg;
	const struct xpcs_serdes_cfg *lane_cfg = &pdata->lane_cfg[pdata->mode];
	u32 lane;
	u32 boost_gain;
	unsigned int val;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	XPCS_RGWR(pdata, PMA_MPLL_CMN_CTRL, 0);
	XPCS_RGWR(pdata, PMA_VCO_CAL_REF0, 0);
	XPCS_RGWR(pdata, PMA_VCO_CAL_REF1, 0);
	if (xpcs_hw_is_e16(pdata) || xpcs_hw_is_c10(pdata))
		XPCS_RGWR(pdata, PMA_RX_PPM_CTRL0, 0);
	XPCS_RGWR(pdata, PMA_RX_GENCTRL3, 0);

	/* lane configuration */
	for (lane = 0; lane < mode_cfg->lane; lane++) {
		/* Select MPLLB for 10G-XAUI/10GBASE-KX4/2.5G-SGMII
		 * When this bit is set, PHY selects MPLLB to generate
		 * Tx analog clocks on lane 0
		 */
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_MPLL_CMN_CTRL, MPLL_EN_0,
				    1, lane);
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_MPLL_CMN_CTRL, MPLLB_SEL_0,
				    pdata->mpllb ? 1 : 0, lane);

		XPCS_RGWR_VAL_OFF(pdata, PMA_VCO_CAL_LD0, VCO_LD_VAL_0,
				  mode_cfg->vco_ld_val, lane);
		XPCS_RGWR_VAL_OFF_SHIFT(pdata, PMA_VCO_CAL_REF0, VCO_REF_LD_0,
					mode_cfg->vco_ref_ld,
					lane / 2, (lane & 1) * 8);

		XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_EQ_CTRL4, CONT_ADAPT_0,
				    lane_cfg->cont_adapt, lane);

		if (pdata->mpllb && pdata->mode != XPCS_MODE_3000BASEX &&
		    !xpcs_hw_is_c10(pdata)) {
			/* MPLL VCO 7.5G - 1G bit[2:0] = 3'b110 : baud/? */
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_TX_RATE_CTRL, TX_RATE_0,
					    6, lane * 4);
			dev_dbg(pdata->dev, "%s: %s TX_RATE_0 fixed to 6\n",
				pdata->name, __func__);
		} else {
			/* MPLL VCO 5G - 1G bit[2:0] = 3'b011 : baud/8 */
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_TX_RATE_CTRL, TX_RATE_0,
					    mode_cfg->tx_rate, lane * 4);
			dev_dbg(pdata->dev, "%s: %s TX_RATE_0 = %d\n",
				pdata->name, __func__, mode_cfg->tx_rate);
		}

		XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_RATE_CTRL, RX_RATE_0,
				    mode_cfg->rx_rate, lane * 4);

		XPCS_RGWR_VAL_SHIFT(pdata, PMA_TX_GENCTRL2, TX_WIDTH_0,
				    mode_cfg->tx_width, lane * 2);
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_GENCTRL2, RX_WIDTH_0,
				    mode_cfg->rx_width, lane * 2);

		/* Tx Voltage Boost Maximum level,
		 * maximum achievable Tx swing in the PHY
		 */
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_TX_GENCTRL1, VBOOST_EN_0,
				    lane_cfg->tx_vboost_en, lane);

		/* PHY RX attenuation Level */
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_ATTN_CTRL, LVL,
				    lane_cfg->rx_attn_lvl, lane * 4);

		/* PHY ctle_pole and boost and gain register */
		boost_gain = ((lane_cfg->rx_vga1_gain & 0x0F) << 12)
			     | ((lane_cfg->rx_vga2_gain & 0x0F) << 8)
			     | ((lane_cfg->rx_ctle_pole & 0x07) << 5)
			     | (lane_cfg->rx_ctle_boost & 0x1F);
		XPCS_RGWR(pdata, PMA_RX_EQ_CTRL0 + lane * 4, boost_gain);

		if (xpcs_hw_is_e12(pdata)) {
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_AFE_DFE_EN_CTRL, AFE,
					    mode_cfg->afe_en, lane);

			XPCS_RGWR_VAL_SHIFT(pdata, PMA_AFE_DFE_EN_CTRL, DFE,
					    mode_cfg->dfe_en, lane);
		}

		if (xpcs_hw_is_e16(pdata) || xpcs_hw_is_c10(pdata)) {
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_PPM_CTRL0, CDR_PPM_MAX,
					    mode_cfg->cdr_ppm_max, lane * 8);

			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_CDR_CTRL1, VCO_TEMP_COMP_EN,
					    mode_cfg->vco_temp_comp_en, lane);
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_CDR_CTRL1, VCO_STEP_CTRL,
					    mode_cfg->vco_step_ctrl, lane);
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_CDR_CTRL1, VCO_FRQBAND,
					    mode_cfg->vco_frqband, lane * 2);

			XPCS_RGWR_VAL_OFF_SHIFT(pdata, PMA_RX_MISC_CTRL0, MISC,
						mode_cfg->rx_misc, lane / 2,
						(lane & 1) * 8);

			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_GEN_CTRL4, DFE_BYP,
					    lane_cfg->rx_dfe_bypass, lane);
			XPCS_RGWR_VAL_OFF_SHIFT(pdata, PMA_DFE_TAP_CTRL0, DFE_TAP1,
						mode_cfg->rx_dfe_tap1, lane / 2,
						(lane & 1) * 8);

			XPCS_RGWR_VAL_OFF(pdata, PMA_RX_IQ_CTRL, DELTA_IQ,
					  mode_cfg->rx_delta_iq, lane);
			XPCS_RGWR_VAL_OFF(pdata, PMA_RX_IQ_CTRL, MARGIN_IQ,
					  mode_cfg->rx_margin_iq, lane);

			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_EQ_CTRL5, ADPT_SEL,
					    mode_cfg->adpt_sel, lane);
			XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_EQ_CTRL5, ADPT_MODE,
					    mode_cfg->adpt_mode, lane);
		}

		XPCS_RGWR_VAL_SHIFT(pdata, PMA_TX_BOOST_CTRL, TX0_IBOOST,
				    lane_cfg->tx_iboost_lvl, lane * 4);

		/* PHY LOS threshold register */
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_RX_GENCTRL3, LOS_TRSHLD,
				    mode_cfg->los_thr, lane * 3);
	}

	/* Rx Biasing Current Control for Rx analog front end */
	XPCS_RGWR_VAL(pdata, PMA_MISC_C0, RX_VREF_CTRL,
		      mode_cfg->rx_vref_ctrl);

	XPCS_RGWR_VAL(pdata, PMA_RX_GENCTRL1, RX_DIV16P5_CLK_EN_0,
		      mode_cfg->rx_div16p5_clk_en);

	if (xpcs_hw_is_e16(pdata) || xpcs_hw_is_c10(pdata))
		XPCS_RGWR_VAL(pdata, PMA_MISC_CTRL2, SUP_MISC, 1);

	if (xpcs_hw_is_c10(pdata)) {
		/* PCIE_PHY_EXT_MPLLA_CTRL_4: 0x68 - PCIE_EXT_MPLLA_BANDWIDTH(0-15): 14 */
		xpcs_phy_config(pdata, 0x68, 14, 0x1, mode_cfg->mpllb_force_en_ovrd_pcie3_app);
		/* PCIE_PHY_EXT_MPLLA_CTRL_4: 0x68 - PCIE_EXT_MPLLA_BANDWIDTH(0-15): 11-13 */
		xpcs_phy_config(pdata, 0x68, 11, 0x7, mode_cfg->mpllb_ssc_clk_sel_pcie3_app);
		/* PCIE_PHY_EXT_MPLLA_CTRL_4: 0x68 - PCIE_EXT_MPLLA_BANDWIDTH(0-15): 2 */
		xpcs_phy_config(pdata, 0x68, 2, 0x1, mode_cfg->mpllb_cdr_vco_low_freq_pcie3_app);

		/* PCIE_PHY_EXT_MPLLA_CTRL_4: 0x68 - PCIE_EXT_MPLLA_FRACN_CTRL(16-26): 0-2 */
		xpcs_phy_config(pdata, 0x68, 16, 0x7, mode_cfg->mpllb_tx_clk_div_pcie3_app);

		/* PCIE_PHY_EXT_MPLLA_CTRL_4: 0x68 - PCIE_EXT_MPLLA_BANDWIDTH(0-15): 3-10 */
		xpcs_phy_config(pdata, 0x68, 3, 0xFF, mode_cfg->mpllb_div_mult_pcie3_app);
		xpcs_phy_read(pdata, 0x68, &val);

		xpcs_phy_config(pdata, 0x3c, 12, 0x7, 0);
		xpcs_phy_read(pdata, 0x3c, &val);

		xpcs_phy_config(pdata, 0x74, 16, 0x7, 2);
		xpcs_phy_config(pdata, 0x74, 19, 0x7, 2);
		xpcs_phy_read(pdata, 0x74, &val);

		xpcs_phy_config(pdata, 0x6c, 27, 0x1, 0);
		xpcs_phy_read(pdata, 0x6c, &val);

		xpcs_phy_config(pdata, 0x14, 0, 0x1, 0);
		xpcs_phy_read(pdata, 0x14, &val);

		xpcs_phy_config(pdata, 0x40, 0, 0x1, 0);
		xpcs_phy_config(pdata, 0x40, 1, 0x1, 0);
		xpcs_phy_read(pdata, 0x40, &val);
	}

	REG_DUMP(PMA_MPLL_CMN_CTRL);
	REG_DUMP(PMA_VCO_CAL_LD0);
	REG_DUMP(PMA_VCO_CAL_REF0);
	REG_DUMP(PMA_RX_EQ_CTRL4);
	REG_DUMP(PMA_TX_RATE_CTRL);
	REG_DUMP(PMA_RX_RATE_CTRL);
	REG_DUMP(PMA_TX_GENCTRL2);
	REG_DUMP(PMA_RX_GENCTRL2);
	REG_DUMP(PMA_TX_GENCTRL1);
	REG_DUMP(PMA_RX_GENCTRL1);
	REG_DUMP(PMA_RX_ATTN_CTRL);
	REG_DUMP(PMA_RX_EQ_CTRL0);
	REG_DUMP(PMA_DFE_TAP_CTRL0);
	REG_DUMP(PMA_TX_BOOST_CTRL);
	if (xpcs_hw_is_e16(pdata) || xpcs_hw_is_c10(pdata)) {
		REG_DUMP(PMA_RX_PPM_CTRL0);
		REG_DUMP(PMA_RX_CDR_CTRL1);
		REG_DUMP(PMA_RX_MISC_CTRL0);
		REG_DUMP(PMA_RX_GEN_CTRL4);
		REG_DUMP(PMA_RX_IQ_CTRL);
		REG_DUMP(PMA_RX_EQ_CTRL5);
	}
	REG_DUMP(PMA_RX_GENCTRL3);
	REG_DUMP(PMA_MISC_C0);
	if (xpcs_hw_is_e16(pdata) || xpcs_hw_is_c10(pdata))
		REG_DUMP(PMA_MISC_CTRL2);
}

static void tx_eq_config(struct xpcs_prv_data *pdata)
{
	const struct xpcs_mode_cfg *mode_cfg = pdata->mode_cfg;
	const struct xpcs_serdes_cfg *lane_cfg = &pdata->lane_cfg[pdata->mode];
	u32 tx_eq0, tx_eq1;
	u32 lane;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	tx_eq0 = XPCS_RGRD(pdata, PMA_TX_EQ_C0);

	XPCS_SET_VAL(tx_eq0, PMA_TX_EQ_C0, TX_EQ_MAIN,
		     lane_cfg->tx_eq_main);
	XPCS_SET_VAL(tx_eq0, PMA_TX_EQ_C0, TX_EQ_PRE,
		     lane_cfg->tx_eq_pre);

	XPCS_RGWR(pdata, PMA_TX_EQ_C0, tx_eq0);

	REG_DUMP(PMA_TX_EQ_C0);

	tx_eq1 = XPCS_RGRD(pdata, PMA_TX_EQ_C1);

	XPCS_SET_VAL(tx_eq1, PMA_TX_EQ_C1, TX_EQ_POST,
		     lane_cfg->tx_eq_post);
	XPCS_SET_VAL(tx_eq1, PMA_TX_EQ_C1, TX_EQ_OVR_RIDE,
		     lane_cfg->tx_eq_ovrride);

	XPCS_RGWR(pdata, PMA_TX_EQ_C1, tx_eq1);

	REG_DUMP(PMA_TX_EQ_C1);

	for (lane = 0; lane < mode_cfg->lane; lane++) {
		XPCS_RGWR_VAL_SHIFT(pdata, PMA_TX_BOOST_CTRL, TX0_IBOOST,
				    lane_cfg->tx_iboost_lvl, lane * 4);
	}

	REG_DUMP(PMA_TX_BOOST_CTRL);
}

static void kr_workaround(struct xpcs_prv_data *pdata)
{
	u32 val;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, CLK_RDY, 1);

	REG_DUMP(PMA_TX_GENCTRL1);

	val = XPCS_RGRD(pdata, PCS_DBG_CTRL);
	XPCS_SET_VAL(val, PCS_DBG_CTRL, RX_DT_EN, 1);
	XPCS_SET_VAL(val, PCS_DBG_CTRL, SUPRESS_LOST_DET, 1);
	XPCS_RGWR(pdata, PCS_DBG_CTRL, val);

	REG_DUMP(PCS_DBG_CTRL);
}

/* Switch to 10G KR Mode in 12G gen5 PHY */
static int xpcs_set_mode_10gkr(struct xpcs_prv_data *pdata)
{
	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	/* 10G Base-KR PCS (0) */
	XPCS_RGWR_VAL(pdata, PCS_CTRL2, PCS_TYPE_SEL, 0);
	REG_DUMP(PCS_CTRL2);

	return 0;
}

/* Speed     mode  SS5 SS6 SS13
 * 1000 Mbps SGMII 0   1   0
 * 100 Mbps  SGMII 0   0   1
 * 10 Mbps   SGMII 0   0   0
 */
/* 1G XAUI Mode */
static int xpcs_set_mode_basex(struct xpcs_prv_data *pdata, u32 en_2p5, u32 spd)
{
	const struct xpcs_mode_cfg *mode_cfg = pdata->mode_cfg;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	/* Setting SS13 bit PMA side */
	XPCS_RGWR_VAL(pdata, PMA_CTRL1, SS13, 0);
	REG_DUMP(PMA_CTRL1);

	/* PMA_TYPE_1G_BASE_KX */
	XPCS_RGWR_VAL(pdata, PMA_CTRL2, PMA_TYPE, mode_cfg->pma_type);
	REG_DUMP(PMA_CTRL2);

	/* When this field is written with 2'b00, the KR
	 * speed mode is selected. When this field is written with a value other
	 * than 2'b00, the KR speed mode is de-selected and non-KR speed
	 * mode (KX4 or KX) is selected
	 */
	XPCS_RGWR_VAL(pdata, PCS_CTRL2, PCS_TYPE_SEL, mode_cfg->pcs_type);
	REG_DUMP(PCS_CTRL2);

	/* XAUI Mode to 0 to disable RXAUI Mode */
	XPCS_RGWR_VAL(pdata, PCS_XAUI_CTRL, XAUI_MODE, 0);
	REG_DUMP(PCS_XAUI_CTRL);

	/* Setting SS13 bit of PCS side */
	XPCS_RGWR_VAL(pdata, PCS_CTRL1, SS13, 0);
	REG_DUMP(PCS_CTRL1);

	/* Ensure 2.5G mode is disabled for 1G and 3G */
	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, EN_2_5G_MODE, en_2p5);
	REG_DUMP(VR_MII_DIG_CTRL1);

	/* Full duplex by default */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, DUPLEX_MODE, 1);

	/* 1Gbps SGMII Mode or speed for Non-ANEG mode */
	/* 0 - 10M, 1 - 100M, 2 - 1G */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, SS13, spd & BIT(0));
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, SS6, (spd >> 1) & BIT(0));
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, SS5, 0);
	REG_DUMP(SR_MII_CTRL);

	if (xpcs_hw_is_c10(pdata)) {
		XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, CLK_RDY, 0);
		REG_DUMP(PMA_TX_GENCTRL1);
	}

	return 0;
}

/* SGMII Mode */
static int xpcs_set_mode_sgmii(struct xpcs_prv_data *pdata)
{
	u32 spd = 2;

	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	if (pdata->cl37[pdata->mode] == CONN_TYPE_ANEG_DIS) {
		if (pdata->speed == SPEED_10)
			spd = 0;
		else if (pdata->speed == SPEED_100)
			spd = 1;
	}

	return xpcs_set_mode_basex(pdata, 0, spd);
}

/* 2500Base-X Mode */
static int xpcs_set_mode_2500basex(struct xpcs_prv_data *pdata)
{
	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	return xpcs_set_mode_basex(pdata, 1, 2);
}

/* 3000Base-X Mode */
static int xpcs_set_mode_3000basex(struct xpcs_prv_data *pdata)
{
	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	return xpcs_set_mode_basex(pdata, 0, 2);
}

static int xpcs_set_mode_qusxgmii(struct xpcs_prv_data *pdata)
{
	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	/* 10G Base-KR PCS (0) */
	XPCS_RGWR_VAL(pdata, PCS_CTRL2, PCS_TYPE_SEL, 0);
	REG_DUMP(PCS_CTRL2);

	//XPCS_RGWR_VAL(pdata, PCS_AM_CTRL, AM_CNT, 0x1F40);
	REG_DUMP(PCS_AM_CTRL);

	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, USXG_EN, 1);
	REG_DUMP(PCS_DIG_CTRL1);

	/* 0:  10G-SXGMII
	 * 1:   5G-SXGMII
	 * 2: 2.5G-SXGMII
	 * 5:  10G-QXGMII (QUAD mode)
	 */
	XPCS_RGWR_VAL(pdata, PCS_KR_CTRL, USXG_MODE, 5);
	REG_DUMP(PCS_KR_CTRL);

	/* TX clock not ready until restored in kr_workaround after VR Reset */
	XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, CLK_RDY, 0);

	return 0;
}

static int xpcs_set_mode_1000basex(struct xpcs_prv_data *pdata)
{
	return xpcs_set_mode_sgmii(pdata);
}

static int xpcs_set_mode_5gbaser(struct xpcs_prv_data *pdata)
{
	return xpcs_set_mode_10gkr(pdata);
}

static int xpcs_set_mode_10gbaser(struct xpcs_prv_data *pdata)
{
	return xpcs_set_mode_10gkr(pdata);
}

static int xpcs_set_mode_susxgmii(struct xpcs_prv_data *pdata)
{
	dev_dbg(pdata->dev, "%s: %s\n", pdata->name, __func__);

	/* 10G Base-KR PCS (0) */
	XPCS_RGWR_VAL(pdata, PCS_CTRL2, PCS_TYPE_SEL, 0);
	REG_DUMP(PCS_CTRL2);

	REG_DUMP(PCS_AM_CTRL);

	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, USXG_EN, 1);
	REG_DUMP(PCS_DIG_CTRL1);

	/* 0:  10G-SXGMII
	 * 1:   5G-SXGMII
	 * 2: 2.5G-SXGMII
	 * 5:  10G-QXGMII (QUAD mode)
	 */
	XPCS_RGWR_VAL(pdata, PCS_KR_CTRL, USXG_MODE, 0);
	REG_DUMP(PCS_KR_CTRL);

	/* TX clock not ready until restored in kr_workaround after VR Reset */
	XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, CLK_RDY, 0);

	return 0;
}

static int xpcs_set_mode_invalid(struct xpcs_prv_data *pdata)
{
	dev_err(pdata->dev, "%s: %s\n", pdata->name, __func__);
	return -EINVAL;
}

static int xpcs_set_mode_run(struct xpcs_prv_data *pdata)
{
	dev_dbg(pdata->dev, "%s: %s c10_hw: %d\n",
		pdata->name, __func__, xpcs_hw_is_c10(pdata));

	if (xpcs_hw_is_c10(pdata))
		return mode_cfg_tbl_c10[pdata->mode].set_mode(pdata);
	else
		return mode_cfg_tbl[pdata->mode].set_mode(pdata);
}

void xpcs_disable_an(struct xpcs_prv_data *pdata)
{
	/* Disable CL73 Autoneg */
	XPCS_RGWR_VAL(pdata, AN_CTRL, AN_EN, 0);
	/* Disable Next page */
	XPCS_RGWR_VAL(pdata, AN_CTRL, EXP_NP_CTL, 0);
	/* Disable Backplane CL37 Autonegotiation */
	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, CL37_BP, 0);
	/* Disable CL37 Autoneg */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, AN_ENABLE, 0);
	/* Disable Auto Speed Mode Change after CL37 AN */
	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, MAC_AUTO_SW, 0);
	/* Reset MII Ctrl register */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, PCS_MODE, 0);
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, TX_CONFIG, 0);
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_AN_INTR_EN, 0);
	/* 8-BIT MII Interface for XGMAC */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_CONTROL, 1);
}

/* Programming seq to enable Clause 37 Autonegotiation */
void xpcs_cl37_an(struct xpcs_prv_data *pdata)
{
	/* Disable CL73 Autoneg */
	XPCS_RGWR_VAL(pdata, AN_CTRL, AN_EN, 0);

	/* Enable Backplane CL37 Autonegotiation */
	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, CL37_BP, 1);

	/* Disable CL37 Autoneg */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, AN_ENABLE, 0);

	if (pdata->cl37[pdata->mode] == CONN_TYPE_ANEG_BX)
		/* PCS_Mode = 0 1000/2500-BaseX Mode */
		XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, PCS_MODE, 0);
	else
		/* PCS_Mode = 2 SGMII Mode */
		XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, PCS_MODE, 2);

	/* Enables the generation of Clause 37 autonegotiation complete
	 * interrupt output
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_AN_INTR_EN, 1);

	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, MAC_AUTO_SW, 0);

	if (pdata->cl37[pdata->mode] == CONN_TYPE_ANEG_PHY) {
		/* PHY Side SGMII */
		XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, TX_CONFIG, 1);
		/* Autonegotiation will advertise the values programmed to:
		 * SGMII_LINK_STS of VR_MII_AN_CTRL, SS13, SS6 of SR_MII_CTRL
		 * and FD of SR_MII_AN_ADV
		 */
		XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, PHY_MODE_CTRL, 0);
		/* Enable SGMII Link */
		XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, SGMII_LINK_STS, 1);
		/* SR_MII_AN_ADV must be written after Link Status and
		 * Link Speed according to Synopsis Notes
		 */
		/* 1 Full Duplex, 0 Half Duplex */
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, FD, 1);
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, HD, 0);
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, RF, 0);
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, PAUSE, 0);
	} else if (pdata->cl37[pdata->mode] == CONN_TYPE_ANEG_MAC) {
		/* MAC Side SGMII */
		XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, TX_CONFIG, 0);
		/* Enable SGMII Link */
		XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, SGMII_LINK_STS, 1);
		/* MAC side SGMII Automatic Speed Mode Change after CL37 AN,
		 * xpcs automatically  switches to the negotiated
		 * SGMII/USXGMII/QSGMII(port0) speed, SGMII get ANEG Config
		 * from PHY about the link state change after the completion
		 * of Clause 37 auto-negotiation between PHY and Link Partner
		 */
		XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, MAC_AUTO_SW, 1);
	} else {
		/* 1 Full Duplex, 0 Half Duplex */
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, FD, 1);
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, HD, 0);
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, RF, 0);
		XPCS_RGWR_VAL(pdata, SR_MII_AN_ADV, PAUSE, 0);
	}

	/* 8-BIT MII Interface */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_CONTROL, 1);

	/* 2.5G Mode */
	if (pdata->mode == XPCS_MODE_2500BASEX) {
		if (pdata->cl37[pdata->mode] == CONN_TYPE_ANEG_BX)
			/* Link timer runs for 10 ms in BaseX mode */
			XPCS_RGWR(pdata, VR_MII_LINK_TIMER_CTRL, 0x2FAF);
		else
			/* Link timer runs for 1.6 ms in SGMII mode */
			XPCS_RGWR(pdata, VR_MII_LINK_TIMER_CTRL, 0x07A1);

		/* If this bit is set, the value programmed to
		 * VR MII MMD Link Timer Control Register will be used to
		 * compute the duration of Link Timer.
		 */
		XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, CL37_TMR_OVR_RIDE, 1);
	}

	/* Enable CL37 Autoneg */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, AN_ENABLE, 1);
}

static void xpcs_susxgmii_an(struct xpcs_prv_data *pdata)
{
	int timeout = 0;
	u32 val;
	int i;

	dev_dbg(pdata->dev, "%s: USXGMII AN (step 8~18)\n", pdata->name);

	/* Disable CL73 Autoneg */
	XPCS_RGWR_VAL(pdata, AN_CTRL, AN_EN, 0);
	REG_DUMP(AN_CTRL);

	/* Enable Backplane CL37 Autonegotiation */
	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, CL37_BP, 1);
	REG_DUMP(PCS_DIG_CTRL1);

	/* this has been configured in kr_workaround */
	REG_DUMP(PCS_DBG_CTRL);

	/* Enables the generation of Clause 37 autonegotiation complete
	 * interrupt output
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_AN_INTR_EN, 1);
	REG_DUMP(VR_MII_AN_CTRL);

	/* MAC Side SGMII */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, TX_CONFIG, 0);
	REG_DUMP(VR_MII_AN_CTRL);

	/* LINK UP */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, SGMII_LINK_STS, 1);
	REG_DUMP(VR_MII_AN_CTRL);

	/* 8-BIT MII Interface */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_CONTROL, 1);
	REG_DUMP(VR_MII_AN_CTRL);

	/* Automatic Speed Mode Change after CL37 AN, xpcs automatically
	 * switches to the negotiated SGMII/USXGMII/QSGMII(port0) speed, after
	 * the completion of Clause 37 auto-negotiation.
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, MAC_AUTO_SW, 1);
	REG_DUMP(VR_MII_DIG_CTRL1);

	/* Link timer runs for 1.6 ms */
	XPCS_RGWR(pdata, VR_MII_LINK_TIMER_CTRL, 0x3d1);
	REG_DUMP(VR_MII_LINK_TIMER_CTRL);

	/* If this bit is set, the value programmed to
	 * VR MII MMD Link Timer Control Register will be used to
	 * compute the duration of Link Timer.
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, CL37_TMR_OVR_RIDE, 1);
	REG_DUMP(VR_MII_DIG_CTRL1);

	/* 16. Program the correct speed */
	/* Full duplex by default */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, DUPLEX_MODE, 1);
	/* 10G USXGMII Mode */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, SS13, 1);
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, SS6, 1);
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, SS5, 0);
	REG_DUMP(SR_MII_CTRL);

	/* 12. Enable Clause 37 auto-negotiation by programming
	 *     bit [12] of SR_MII_CTRL Register to 1
	 */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, AN_ENABLE, 1);
	REG_DUMP(SR_MII_CTRL);

	dev_dbg(pdata->dev,
		"%s: step 15) Clear the Interrupt by writing 0 to bit [0] of VR_MII_AN_INTR_STS Register\n",
		pdata->name);
	for (i = 0; i <= MAX_BUSY_RETRY; i++) {
		val = XPCS_RGRD(pdata, VR_MII_AN_INTR_STS);
		if (val & BIT(0)) {
			REG_DUMP_NAME("VR_MII_AN_INTR_STS", VR_MII_AN_INTR_STS);
			xpcs_reg_update(pdata, VR_MII_AN_INTR_STS, BIT(0), 0);
			break;
		}
		udelay(500);
	}
	REG_DUMP_NAME("VR_MII_AN_INTR_STS", VR_MII_AN_INTR_STS);
	if (i > MAX_BUSY_RETRY) {
		timeout++;
		dev_info(pdata->dev,
			 "%s: USXGMII ANEG complete check reached %d times\n",
			 pdata->name, i);
	}

	/* 17. Wait for some time so that clocks at clk_xgmii_tx_i and
	 * clk_xgmii_rx_i ports get stabilized at the desired frequencies.
	 */
	mdelay(100);

	xpcs_reg_update(pdata, PCS_DIG_CTRL1, 0, BIT(10));
	for (i = 0; i <= MAX_BUSY_RETRY; i++) {
		val = XPCS_RGRD(pdata, PCS_DIG_CTRL1);
		if (!(val & BIT(10)))
			break;

		udelay(10);
	}
	REG_DUMP_NAME("PCS_DIG_CTRL1", PCS_DIG_CTRL1);
	if (i > MAX_BUSY_RETRY) {
		timeout++;
		dev_err(pdata->dev,
			"%s: USXGMII Rate Adaptor Reset check reached %d times\n",
			pdata->name, i);
	}

	if (timeout) {
		dev_info(pdata->dev, "%s: %s failed\n",
			 pdata->name, __func__);
	} else {
		dev_info(pdata->dev, "%s: %s succeeded\n",
			 pdata->name, __func__);
	}
}

static void xpcs_qusxgmii_an(struct xpcs_prv_data *pdata)
{
	int timeout = 0;
	u32 val;
	int i, j;

	dev_dbg(pdata->dev, "%s: USXGMII AN (step 8~18)\n", pdata->name);

	/* Disable CL73 Autoneg */
	XPCS_RGWR_VAL(pdata, AN_CTRL, AN_EN, 0);
	REG_DUMP(AN_CTRL);

	/* Enable Backplane CL37 Autonegotiation */
	XPCS_RGWR_VAL(pdata, PCS_DIG_CTRL1, CL37_BP, 1);
	REG_DUMP(PCS_DIG_CTRL1);

	/* this has been configured in kr_workaround */
	REG_DUMP(PCS_DBG_CTRL);

	/* Enables the generation of Clause 37 autonegotiation complete
	 * interrupt output
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_AN_INTR_EN, 1);
	REG_DUMP(VR_MII_AN_CTRL);

	/* MAC Side SGMII */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, TX_CONFIG, 0);
	REG_DUMP(VR_MII_AN_CTRL);

	/* LINK UP */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, SGMII_LINK_STS, 1);
	REG_DUMP(VR_MII_AN_CTRL);

	/* 8-BIT MII Interface */
	XPCS_RGWR_VAL(pdata, VR_MII_AN_CTRL, MII_CONTROL, 1);
	REG_DUMP(VR_MII_AN_CTRL);

	/* same MII configuration on QUAD Port 1/2/3 */
	val = XPCS_RGRD(pdata, VR_MII_1_AN_CTRL);
	val &= ~0x119;
	val |= 0x111;
	XPCS_RGWR(pdata, VR_MII_1_AN_CTRL, val);
	REG_DUMP(VR_MII_1_AN_CTRL);

	val = XPCS_RGRD(pdata, VR_MII_2_AN_CTRL);
	val &= ~0x119;
	val |= 0x111;
	XPCS_RGWR(pdata, VR_MII_2_AN_CTRL, val);
	REG_DUMP(VR_MII_2_AN_CTRL);

	val = XPCS_RGRD(pdata, VR_MII_3_AN_CTRL);
	val &= ~0x119;
	val |= 0x111;
	XPCS_RGWR(pdata, VR_MII_3_AN_CTRL, val);
	REG_DUMP(VR_MII_3_AN_CTRL);

	/* Automatic Speed Mode Change after CL37 AN, xpcs automatically
	 * switches to the negotiated SGMII/USXGMII/QSGMII(port0) speed, after
	 * the completion of Clause 37 auto-negotiation.
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, MAC_AUTO_SW, 1);
	REG_DUMP(VR_MII_DIG_CTRL1);

	/* same MII configuration on QUAD Port 1/2/3 */
	val = XPCS_RGRD(pdata, VR_MII_1_DIG_CTRL1);
	val |= BIT(VR_MII_DIG_CTRL1_MAC_AUTO_SW_POS);
	XPCS_RGWR(pdata, VR_MII_1_DIG_CTRL1, val);
	REG_DUMP(VR_MII_1_DIG_CTRL1);

	val = XPCS_RGRD(pdata, VR_MII_2_DIG_CTRL1);
	val |= BIT(VR_MII_DIG_CTRL1_MAC_AUTO_SW_POS);
	XPCS_RGWR(pdata, VR_MII_2_DIG_CTRL1, val);
	REG_DUMP(VR_MII_2_DIG_CTRL1);

	val = XPCS_RGRD(pdata, VR_MII_3_DIG_CTRL1);
	val |= BIT(VR_MII_DIG_CTRL1_MAC_AUTO_SW_POS);
	XPCS_RGWR(pdata, VR_MII_3_DIG_CTRL1, val);
	REG_DUMP(VR_MII_3_DIG_CTRL1);

	/* Link timer runs for 1.6 ms */
	XPCS_RGWR(pdata, VR_MII_LINK_TIMER_CTRL, 0x3d1);
	REG_DUMP(VR_MII_LINK_TIMER_CTRL);

	/* If this bit is set, the value programmed to
	 * VR MII MMD Link Timer Control Register will be used to
	 * compute the duration of Link Timer.
	 */
	XPCS_RGWR_VAL(pdata, VR_MII_DIG_CTRL1, CL37_TMR_OVR_RIDE, 1);
	REG_DUMP(VR_MII_DIG_CTRL1);

	val = XPCS_RGRD(pdata, VR_MII_1_DIG_CTRL1);
	val |= BIT(VR_MII_DIG_CTRL1_CL37_TMR_OVR_RIDE_POS);
	XPCS_RGWR(pdata, VR_MII_1_DIG_CTRL1, val);
	REG_DUMP(VR_MII_1_DIG_CTRL1);

	val = XPCS_RGRD(pdata, VR_MII_2_DIG_CTRL1);
	val |= BIT(VR_MII_DIG_CTRL1_CL37_TMR_OVR_RIDE_POS);
	XPCS_RGWR(pdata, VR_MII_2_DIG_CTRL1, val);
	REG_DUMP(VR_MII_2_DIG_CTRL1);

	val = XPCS_RGRD(pdata, VR_MII_3_DIG_CTRL1);
	val |= BIT(VR_MII_DIG_CTRL1_CL37_TMR_OVR_RIDE_POS);
	XPCS_RGWR(pdata, VR_MII_3_DIG_CTRL1, val);
	REG_DUMP(VR_MII_3_DIG_CTRL1);

	/* 12. Enable Clause 37 auto-negotiation by programming
	 *     bit [12] of SR_MII_CTRL Register to 1
	 */
	XPCS_RGWR_VAL(pdata, SR_MII_CTRL, AN_ENABLE, 1);
	REG_DUMP(SR_MII_CTRL);

	val = XPCS_RGRD(pdata, SR_MII_1_CTRL);
	val |= BIT(SR_MII_CTRL_AN_ENABLE_POS);
	XPCS_RGWR(pdata, SR_MII_1_CTRL, val);
	REG_DUMP(SR_MII_1_CTRL);

	val = XPCS_RGRD(pdata, SR_MII_2_CTRL);
	val |= BIT(SR_MII_CTRL_AN_ENABLE_POS);
	XPCS_RGWR(pdata, SR_MII_2_CTRL, val);
	REG_DUMP(SR_MII_2_CTRL);

	val = XPCS_RGRD(pdata, SR_MII_3_CTRL);
	val |= BIT(SR_MII_CTRL_AN_ENABLE_POS);
	XPCS_RGWR(pdata, SR_MII_3_CTRL, val);
	REG_DUMP(SR_MII_3_CTRL);

	dev_dbg(pdata->dev,
		"%s: step 15) Clear the Interrupt by writing 0 to bit [0] of VR_MII_AN_INTR_STS Register\n",
		pdata->name);
	for (j = 0; j < 4; j++) {
		const char * const name[] = {"VR_MII_AN_INTR_STS",
				      "VR_MII_1_AN_INTR_STS",
				      "VR_MII_2_AN_INTR_STS",
				      "VR_MII_3_AN_INTR_STS"};
		const u32 addr[] = {VR_MII_AN_INTR_STS, VR_MII_1_AN_INTR_STS,
				    VR_MII_2_AN_INTR_STS, VR_MII_3_AN_INTR_STS};
		for (i = 0; i <= MAX_BUSY_RETRY; i++) {
			val = XPCS_RGRD(pdata, addr[j]);
			if (val & 1) {
				REG_DUMP_NAME(name[j], addr[j]);
				xpcs_reg_update(pdata, addr[j], BIT(0), 0);
				break;
			}
			udelay(500);
		}
		REG_DUMP_NAME(name[j], addr[j]);
		if (i > MAX_BUSY_RETRY) {
			timeout++;
			dev_info(pdata->dev,
				 "%s: USXGMII ANEG (%d) complete check reached %d times\n",
				 pdata->name, j, i);
		}
	}

	/* 17. Wait for some time so that clocks at clk_xgmii_tx_i and
	 * clk_xgmii_rx_i ports get stabilized at the desired frequencies.
	 */
	mdelay(100);

	for (j = 0; j < 4; j++) {
		const char * const name[] = {"PCS_DIG_CTRL1", "VR_MII_1_DIG_CTRL1",
				    "VR_MII_2_DIG_CTRL1", "VR_MII_3_DIG_CTRL1"};
		const u32 addr[] = {PCS_DIG_CTRL1, VR_MII_1_DIG_CTRL1,
				    VR_MII_2_DIG_CTRL1, VR_MII_3_DIG_CTRL1};
		const u32 bit[] = {BIT(10), BIT(5), BIT(5), BIT(5)};

		xpcs_reg_update(pdata, addr[j], 0, bit[j]);
		for (i = 0; i <= MAX_BUSY_RETRY; i++) {
			val = XPCS_RGRD(pdata, addr[j]);
			if (!(val & bit[j]))
				break;

			udelay(10);
		}
		REG_DUMP_NAME(name[j], addr[j]);
		if (i > MAX_BUSY_RETRY) {
			timeout++;
			dev_err(pdata->dev,
				"%s: USXGMII Rate Adaptor Reset (%d) check reached %d times\n",
				pdata->name, j, i);
		}
	}

	if (timeout) {
		dev_info(pdata->dev, "%s: %s failed\n",
			 pdata->name, __func__);
	} else {
		dev_info(pdata->dev, "%s: %s succeeded\n",
			 pdata->name, __func__);
	}
}

/* If this bit is set before or during Clause 73 auto-negotiation, the
 * training starts after all pages have been exchanged during Clause
 * 73 auto-negotiation.
 */
static void xpcs_cl72_startup(struct xpcs_prv_data *pdata)
{
	/* Enable the 10G-Base-KR start up protocol */
	XPCS_RGWR_VAL(pdata, PMA_KR_PMD_CTRL, TR_EN, 1);
}

/* Programming seq to enable Clause 73 Autonegotiation */
static void xpcs_cl73_an(struct xpcs_prv_data *pdata)
{
	/* Enable Autoneg */
	XPCS_RGWR_VAL(pdata, AN_CTRL, AN_EN, 1);

	/* Link Fail Inhibit/Autoneg Wait Timer Value for
	 * Clause 73 autonegotiation.
	 */
	XPCS_RGWR_VAL(pdata, AN_TIMER_CTRL1, INHBT_OR_WAIT_TIME, 0xF);

	/* Over-Ride Control for Clause 73 Auto-negotiation Timers. */
	XPCS_RGWR_VAL(pdata, AN_DIG_CTRL, CL73_TMR_OVR_RIDE, 1);

	/* Enable the AN Complete Interrupt, An Incompatible Link
	 * Interrupt, An page received Interrupt enable
	 */
	XPCS_RGWR(pdata, AN_INTR_MSK, 7);

	/* Restart Autoneg */
	XPCS_RGWR_VAL(pdata, AN_CTRL, RSTRT_AN, 1);

	/* Start Training */
	xpcs_cl72_startup(pdata);
}

static void xpcs_an(struct xpcs_prv_data *pdata)
{
	xpcs_disable_an(pdata);

	if (pdata->cl73[pdata->mode])
		xpcs_cl73_an(pdata);

	if (pdata->cl37[pdata->mode]) {
		if (pdata->mode == XPCS_MODE_QUSXGMII)
			xpcs_qusxgmii_an(pdata);
		else if (pdata->mode == XPCS_MODE_SUSXGMII)
			xpcs_susxgmii_an(pdata);
		else
			xpcs_cl37_an(pdata);
	}
}

static void xgmii_clk(struct xpcs_prv_data *pdata, int enable)
{
	if (IS_ERR_OR_NULL(pdata->syscon))
		return;

	regmap_write_bits(pdata->syscon, pdata->syscon_offset,
			  BIT(pdata->syscon_bit),
			  enable ? BIT(pdata->syscon_bit) : 0);
	dev_dbg(pdata->dev, "%s: %s %s\n", pdata->name,
		__func__, enable ? "enable" : "disable");
}

static int xpcs_init(struct xpcs_prv_data *pdata)
{
	int ret = 0;
	struct xpcs_prv_data *pair_pdata = NULL;

	dev_dbg(pdata->dev, "%s: %s mode: %u\n", pdata->name, __func__, pdata->mode);

	if (pdata->slave) {
		if (!pdata->pair) {
			dev_err(pdata->dev,
				"Can't find master device paired with %s",
				pdata->name);
			return -EINVAL;
		}

		pair_pdata = pdata;
		pdata = dev_get_drvdata(pdata->pair);
		if (!pdata) {
			dev_err(pair_pdata->dev,
				"Master device paired with %s is not initialized\n",
				pair_pdata->name);
			return -ENODEV;
		}
	} else if (pdata->pair) {
		pair_pdata = dev_get_drvdata(pdata->pair);
	}

	if (pdata->mode == XPCS_MODE_SGMII ||
	    pdata->mode == XPCS_MODE_1000BASEX)
		pdata->mpllb = pdata->mpllb_dts;
	else if (pdata->mode == XPCS_MODE_3000BASEX ||
		 xpcs_hw_is_c10(pdata))
		pdata->mpllb = 1;
	else
		pdata->mpllb = 0;

	if (pair_pdata) {
		if (pair_pdata->mode == XPCS_MODE_SGMII ||
		    pair_pdata->mode == XPCS_MODE_1000BASEX)
			pair_pdata->mpllb = pair_pdata->mpllb_dts;
		else if (pair_pdata->mode == XPCS_MODE_3000BASEX ||
			 xpcs_hw_is_c10(pair_pdata))
			pair_pdata->mpllb = 1;
		else
			pair_pdata->mpllb = 0;
	}

	if (pair_pdata)
		xgmii_clk(pair_pdata, 0);
	xgmii_clk(pdata, 0);

	if (pair_pdata &&
	    (xpcs_hw_is_c10(pair_pdata) || xpcs_hw_is_e12(pair_pdata)))
		xpcs_init_byp_pwrup(pair_pdata);

	if (xpcs_hw_is_c10(pdata) || xpcs_hw_is_e12(pdata)) {
		ret = xpcs_init_byp_pwrup(pdata);
		if (ret)
			goto EXIT;
	} else {
		ret = sram_init_chk(pdata);
		if (ret)
			goto EXIT;
		if (pdata->e16_fw_name)
			sram_ld_fw(pdata);
		sram_ext_ld_done(pdata, 1);
	}

#if IS_ENABLED(CONFIG_MXL_P34X_FWDL)
	/* Workaround specific to lan_xpcs and P34X PHY. After URX probes
	 * lan_xpcs sram INIT_DN and set EXT_LD_DN to 1, reset p34x to avoid
	 * USXGMII auto-negotiation failure.
	 */
	if (xpcs_p34x_phy(pdata->phydev) &&
	    pdata->mode == XPCS_MODE_QUSXGMII &&
	    pdata->phy_reset)
		if (pdata->phy_reset(pdata->phydev))
			dev_warn(pdata->dev, "%s: failed to reset p34x\n",
				 __func__);
#endif

#if IS_ENABLED(CONFIG_PHY_MXL_WAN_XPCS)
	if (pdata->wan_wa) {
		/* Re-calibate workaround for Resistor Tuning Calibration.
		 * (WAN only)
		 */
		phy_serdes_dig_ovrd(pdata->phy);
	}
#endif

	if (pair_pdata) {
		ret = xpcs_rxtx_stable(pair_pdata);
		if (ret)
			goto EXIT;

		/* Disable Autoneg */
		xpcs_disable_an(pair_pdata);

		ret = xpcs_set_mode_run(pair_pdata);
		if (ret) {
			dev_err(pair_pdata->dev, "%s: mode %u config failed\n",
				pair_pdata->name, pair_pdata->mode);
			goto EXIT;
		}
	}

	ret = xpcs_rxtx_stable(pdata);
	if (ret)
		goto EXIT;

	/* Disable Autoneg */
	xpcs_disable_an(pdata);

	ret = xpcs_set_mode_run(pdata);
	if (ret) {
		dev_err(pdata->dev, "%s: mode %u config failed\n",
			pdata->name, pdata->mode);
		goto EXIT;
	}

	if (pair_pdata)
		xpcs_cfg_clk_pair(pdata, pair_pdata);
	else
		xpcs_cfg_clk(pdata);

	if (pair_pdata)
		xpcs_cfg_table(pair_pdata);

	xpcs_cfg_table(pdata);

	if (pair_pdata && xpcs_hw_is_c10(pair_pdata))
		xpcs_vs_reset(pair_pdata);

	ret = xpcs_vs_reset(pdata);
	if (ret)
		goto EXIT;

	tx_eq_config(pdata);

	if (pdata->mode == XPCS_MODE_10GBASER ||
	    pdata->mode == XPCS_MODE_SUSXGMII ||
	    pdata->mode == XPCS_MODE_10GKR ||
	    pdata->mode == XPCS_MODE_QUSXGMII)
		kr_workaround(pdata);

	if (pair_pdata) {
		tx_eq_config(pair_pdata);

		if (pair_pdata->mode == XPCS_MODE_10GBASER ||
		    pair_pdata->mode == XPCS_MODE_SUSXGMII ||
		    pair_pdata->mode == XPCS_MODE_10GKR ||
		    pair_pdata->mode == XPCS_MODE_QUSXGMII)
			kr_workaround(pair_pdata);
	}

	if (pair_pdata && xpcs_hw_is_c10(pair_pdata)) {
		XPCS_RGWR_VAL(pair_pdata, PMA_TX_GENCTRL1, CLK_RDY, 1);
		REG_DUMP(PMA_TX_GENCTRL1);
	}
	if (xpcs_hw_is_c10(pdata)) {
		XPCS_RGWR_VAL(pdata, PMA_TX_GENCTRL1, CLK_RDY, 1);
		REG_DUMP(PMA_TX_GENCTRL1);
	}

	/* need 20ms delay? */
	xpcs_an(pdata);

	if (pair_pdata)
		xpcs_an(pair_pdata);

	if (pdata->phy && pdata->lane_cfg[pdata->mode].rx_cali) {
		usleep_range(10, 20);
		ret = phy_calibrate(pdata->phy);
		if (ret)
			dev_err(pdata->dev,
				"%s: SERDES RX calibration failed: %d\n",
				pdata->name, ret);
	}

	if (pair_pdata && pair_pdata->phy &&
	    pair_pdata->lane_cfg[pair_pdata->mode].rx_cali) {
		usleep_range(10, 20);
		ret = phy_calibrate(pair_pdata->phy);
		if (ret)
			dev_err(pair_pdata->dev,
				"%s: SERDES RX calibration failed: %d\n",
				pair_pdata->name, ret);
	}

	xgmii_clk(pdata, 1);
	if (pair_pdata)
		xgmii_clk(pair_pdata, 1);

	/* Ignore SERDES RX calibration failures */
	ret = 0;

EXIT:
	dev_dbg(pdata->dev, "%s: %s return %d\n",
		pdata->name, __func__, ret);
	return ret;
}

#ifdef CONFIG_OF

static void xpcs_update_cfg(struct device *dev, struct xpcs_serdes_cfg *lane_cfg)
{
	struct {
		const char *name;
		int num;
		u32 val[XPCS_MODE_MAX];
	} cfgs[15] = {
		{XPCS_RX_ATTN_LVL,	0, {0}},
		{XPCS_CTLE_BOOST,	0, {0}},
		{XPCS_CTLE_POLE,	0, {0}},
		{XPCS_VGA1_GAIN,	0, {0}},
		{XPCS_VGA2_GAIN,	0, {0}},
		{XPCS_DFE_BYPASS,	0, {0}},
		{XPCS_ADAPT_EN,		0, {0}},
		{XPCS_RX_CALI,		0, {0}},
		{XPCS_TX_VBOOST_EN,	0, {0}},
		{XPCS_TX_VBOOST_LVL,	0, {0}},
		{XPCS_TX_IBOOST_LVL,	0, {0}},
		{XPCS_TX_EQ_MAIN,	0, {0}},
		{XPCS_TX_EQ_PRE,	0, {0}},
		{XPCS_TX_EQ_POST,	0, {0}},
		{XPCS_TX_EQ_OVRRIDE,	0, {0}}
	};
	size_t n;
	int i;

	for (n = 0; n < ARRAY_SIZE(cfgs); n++) {
		int num;

		cfgs[n].num = device_property_read_u32_array(dev, cfgs[n].name,
							     NULL, 0);
		if (cfgs[n].num <= 0)
			continue;

		num = min(cfgs[n].num, XPCS_MODE_MAX);
		if (device_property_read_u32_array(dev, cfgs[n].name,
						   cfgs[n].val, num))
			cfgs[n].num = 0;
	}

#if DBG_UPDATE_CFG
	for (n = 0; n < ARRAY_SIZE(cfgs); n++) {
		char buf[256];
		int len;

		if (cfgs[n].num <= 0) {
			dev_info(dev, "%2zu. %s:\n", n, cfgs[n].name);
			continue;
		}

		len = 0;
		for (i = 0; i < cfgs[n].num; i++)
			len += snprintf(buf + len, ARRAY_SIZE(buf) - len - 1,
					"\t%u", cfgs[n].val[i]);
		buf[len] = 0;
		dev_info(dev, "%2zu. %s:%s\n", n, cfgs[n].name, buf);
	}
#endif

	for (i = 0; i < cfgs[0].num; i++)
		lane_cfg[i].rx_attn_lvl = cfgs[0].val[i];

	for (i = 0; i < cfgs[1].num; i++)
		lane_cfg[i].rx_ctle_boost = cfgs[1].val[i];

	for (i = 0; i < cfgs[2].num; i++)
		lane_cfg[i].rx_ctle_pole = cfgs[2].val[i];

	for (i = 0; i < cfgs[3].num; i++)
		lane_cfg[i].rx_vga1_gain = cfgs[3].val[i];

	for (i = 0; i < cfgs[4].num; i++)
		lane_cfg[i].rx_vga2_gain = cfgs[4].val[i];

	for (i = 0; i < cfgs[5].num; i++)
		lane_cfg[i].rx_dfe_bypass = cfgs[5].val[i];

	for (i = 0; i < cfgs[6].num; i++)
		lane_cfg[i].cont_adapt = cfgs[6].val[i];

	for (i = 0; i < cfgs[7].num; i++)
		lane_cfg[i].rx_cali = cfgs[7].val[i];

	for (i = 0; i < cfgs[8].num; i++)
		lane_cfg[i].tx_vboost_en = cfgs[8].val[i];

	for (i = 0; i < cfgs[9].num; i++)
		lane_cfg[i].tx_vboost_lvl = cfgs[9].val[i];

	for (i = 0; i < cfgs[10].num; i++)
		lane_cfg[i].tx_iboost_lvl = cfgs[10].val[i];

	for (i = 0; i < cfgs[11].num; i++)
		lane_cfg[i].tx_eq_main = cfgs[11].val[i];

	for (i = 0; i < cfgs[12].num; i++)
		lane_cfg[i].tx_eq_pre = cfgs[12].val[i];

	for (i = 0; i < cfgs[13].num; i++)
		lane_cfg[i].tx_eq_post = cfgs[13].val[i];

	for (i = 0; i < cfgs[14].num; i++)
		lane_cfg[i].tx_eq_ovrride = cfgs[14].val[i];
}

static struct device *get_pair(struct device *dev)
{
	struct device_node *node;
	struct platform_device *pair_pdev;

	node = of_parse_phandle(dev->of_node, "pair", 0);
	if (!node) {
		dev_dbg(dev, "no pair node\n");
		return NULL;
	}

	pair_pdev = of_find_device_by_node(node);
	of_node_put(node);
	if (!pair_pdev) {
		dev_notice(dev, "can not find device by pair node\n");
		return NULL;
	}

	return &pair_pdev->dev;
}

static int xpcs_parse_dts(struct platform_device *pdev,
			  struct xpcs_prv_data *pdata)
{
	struct device *dev = &pdev->dev;
	const struct xpcs_hw_config *hw_cfg = of_device_get_match_data(dev);
	u32 prop = 0;
	int num;
	u32 val[XPCS_MODE_MAX] = {0};
	int i;

	/* Retrieve the xpcs name */
	if (device_property_read_string(dev, XPCS_MOD_NAME, &pdata->name)) {
		dev_err(dev, "Cannot get property " XPCS_MOD_NAME "\n");
		return -EINVAL;
	}

	/* Retrieve the xpcs mode */
	if (device_property_read_u32(dev, XPCS_MODE_NAME, &prop)) {
		dev_err(dev, "%s: cannot get property " XPCS_MODE_NAME "\n",
			pdata->name);
		return -EINVAL;
	}
	if (prop >= XPCS_MODE_MAX) {
		dev_err(dev, "%s: " XPCS_MODE_NAME " %u is invalid\n",
			pdata->name, prop);
		return -EINVAL;
	}
	switch (prop) {
	case XPCS_MODE_SUSXGMII:
		if (hw_cfg && !hw_cfg->usxg_sport) {
			dev_err(dev, "%s: " XPCS_MODE_NAME " not supported\n",
				pdata->name);
			return -EINVAL;
		}
		break;

	case XPCS_MODE_QUSXGMII:
		if (hw_cfg && !hw_cfg->usxg_qport) {
			dev_err(dev, "%s: " XPCS_MODE_NAME " not supported\n",
				pdata->name);
			return -EINVAL;
		}
		break;

	default:
		break;
	}
	xpcs_set_mode(pdata, prop);

	switch (pdata->mode) {
	case XPCS_MODE_SGMII:
	case XPCS_MODE_1000BASEX:
		pdata->speed = SPEED_1000;
		break;
	case XPCS_MODE_2500BASEX:
	case XPCS_MODE_QUSXGMII:
		pdata->speed = SPEED_2500;
		break;
	case XPCS_MODE_3000BASEX:
		pdata->speed = 3000;
		break;
	case XPCS_MODE_5GBASER:
		pdata->speed = SPEED_5000;
		break;
	case XPCS_MODE_10GBASER:
	case XPCS_MODE_SUSXGMII:
	case XPCS_MODE_10GKR:
		pdata->speed = SPEED_10000;
		break;
	default:
		pdata->speed = SPEED_1000;
	};
	pdata->duplex = DUPLEX_FULL;

	memcpy(pdata->cl73, def_cl73, sizeof(pdata->cl73));
	num = device_property_read_u32_array(dev, XPCS_CL73, NULL, 0);
	if (num > 0 &&
	    !device_property_read_u32_array(dev, XPCS_CL73, val,
					       min(num, XPCS_MODE_MAX))) {
		for (i = 0; i < num && i < XPCS_MODE_MAX; i++) {
			if (val[i] >= CONN_TYPE_MAX) {
				dev_err(dev, "%s: " XPCS_CL73 " %u is invalid\n",
					pdata->name, val[i]);
				return -EINVAL;
			}
			pdata->cl73[i] = val[i];
		}
	}

	memcpy(pdata->cl37, def_cl37, sizeof(pdata->cl37));
	num = device_property_read_u32_array(dev, XPCS_CL37, NULL, 0);
	if (num > 0 &&
	    !device_property_read_u32_array(dev, XPCS_CL37, val,
					       min(num, XPCS_MODE_MAX))) {
		for (i = 0; i < num && i < XPCS_MODE_MAX; i++) {
			if (val[i] >= CONN_TYPE_MAX) {
				dev_err(dev, "%s: " XPCS_CL37 " %u is invalid\n",
					pdata->name, val[i]);
				return -EINVAL;
			}
			pdata->cl37[i] = val[i];
		}
	}

	if (!device_property_read_u32(dev, XPCS_SLAVE, &prop))
		pdata->slave = prop;

	if (!device_property_read_u32(dev, XPCS_MPLLB, &prop))
		pdata->mpllb_dts = prop;

	if (!device_property_read_u32(dev, XPCS_WAN_WA, &prop))
		pdata->wan_wa = prop;

	device_property_read_string(dev, XPCS_E16_FW_NAME, &pdata->e16_fw_name);

	memcpy(&pdata->lane_cfg[0], lane_cfg_tbl, sizeof(pdata->lane_cfg));
	xpcs_update_cfg(dev, &pdata->lane_cfg[0]);

	if (!pdata->slave) {
		pdata->pair = get_pair(dev);
		if (pdata->pair && dev_get_drvdata(pdata->pair)) {
			struct xpcs_prv_data *pair_pdata;

			pair_pdata = dev_get_drvdata(pdata->pair);

			if (!pair_pdata->slave) {
				dev_err(dev,
					"Master device (%s) paired device (%s) is not slave device\n",
					pdata->name, pair_pdata->name);
				return -EINVAL;
			}
			pair_pdata->pair = dev;
		}
	}

	pdata->syscon = syscon_regmap_lookup_by_phandle(dev->of_node,
							XGMII_CLK_SYSCON);
	if (IS_ERR_OR_NULL(pdata->syscon) ||
	    device_property_read_u32(dev, XGMII_CLK_BIT, &pdata->syscon_bit) ||
	    device_property_read_u32(dev, XGMII_CLK_OFFSET,
				     &pdata->syscon_offset)) {
		dev_dbg(dev, "No XGMII line-clock gate.\n");
	} else {
		dev_info(dev, "XGMII Clock Gate at offset 0x%04X bit %u\n",
			 pdata->syscon_offset, pdata->syscon_bit);
	}

	pdata->cbphy_app = syscon_regmap_lookup_by_phandle(dev->of_node,
							   XPCS_CBPHY_APP_SYSCON);

	platform_set_drvdata(pdev, (void *)pdata);

	return 0;
}

static bool is_clk_required(struct device *dev)
{
	return of_find_property(dev->of_node, "clocks", NULL) ? true : false;
}

static bool is_phy_required(struct device *dev)
{
	return of_find_property(dev->of_node, "phy", NULL) ? true : false;
}
#else
static int xpcs_parse_dts(struct platform_device *pdev,
			  struct xpcs_prv_data *pdata)
{
	return -EINVAL;
}

static bool is_clk_required(struct device *dev)
{
	return false;
}

static bool is_phy_required(struct device *dev)
{
	return false;
}
#endif

static int xpcs_assert_reset(struct xpcs_prv_data *pdata)
{
	int cnt = 0;
	int ret, i;

	for (i = 0; i < ARRAY_SIZE(pdata->xpcs_rst); i++) {
		if (!pdata->xpcs_rst[i])
			continue;
		ret = reset_control_assert(pdata->xpcs_rst[i]);
		dev_dbg(pdata->dev, "%s: reset_control_assert(%d) returned %d\n",
			pdata->name, i, ret);
		if (ret)
			return ret;
		cnt++;
	}
	return cnt;
}

static int xpcs_deassert_reset(struct xpcs_prv_data *pdata)
{
	int cnt = 0;
	int ret, i;

	for (i = 0; i < ARRAY_SIZE(pdata->xpcs_rst); i++) {
		if (!pdata->xpcs_rst[i])
			continue;
		ret = reset_control_deassert(pdata->xpcs_rst[i]);
		dev_dbg(pdata->dev,
			"%s: reset_control_deassert(%d) returned %d\n",
			pdata->name, i, ret);
		if (ret)
			return ret;
		cnt++;
	}
	return cnt;
}

static int xpcs_reset(struct device *dev)
{
	struct xpcs_prv_data *pdata = dev_get_drvdata(dev);
	int ret;

	ret = xpcs_assert_reset(pdata);
	if (ret > 0) {
		udelay(10);
		ret = xpcs_deassert_reset(pdata);
		if (ret > 0) {
			mdelay(10);
			ret = 0;
		}
	}

	return ret;
}

static int xpcs_ethtool_ksettings_get(struct device *dev,
				      struct ethtool_link_ksettings *cmd)
{
	struct xpcs_prv_data *pdata;

	if (!dev)
		return -EINVAL;

	pdata = dev_get_drvdata(dev);
	if (!pdata) {
		dev_err(dev, "XPCS is not initialized\n");
		return -ENODEV;
	}

	ethtool_link_ksettings_zero_link_mode(cmd, supported);
	ethtool_link_ksettings_zero_link_mode(cmd, advertising);
	ethtool_link_ksettings_zero_link_mode(cmd, lp_advertising);

	ethtool_link_ksettings_add_link_mode(cmd, supported, 1000baseX_Full);
	ethtool_link_ksettings_add_link_mode(cmd, supported, 10000baseKR_Full);
	ethtool_link_ksettings_add_link_mode(cmd, supported, 2500baseX_Full);

	if (pdata->mode == XPCS_MODE_SGMII ||
	    pdata->mode == XPCS_MODE_1000BASEX) {
		cmd->base.speed = SPEED_1000;
	} else if (pdata->mode == XPCS_MODE_2500BASEX ||
		 pdata->mode == XPCS_MODE_QUSXGMII) {
		cmd->base.speed = SPEED_2500;
	} else if (pdata->mode == XPCS_MODE_3000BASEX) {
		cmd->base.speed = 3000;
	} else if (pdata->mode == XPCS_MODE_5GBASER) {
		cmd->base.speed = SPEED_5000;
	} else if (pdata->mode == XPCS_MODE_10GBASER ||
		 pdata->mode == XPCS_MODE_SUSXGMII ||
		 pdata->mode == XPCS_MODE_10GKR) {
		cmd->base.speed = SPEED_10000;
	} else {
		dev_warn(dev, "Unhandled xpcs mode: %d\n", pdata->mode);
		cmd->base.speed = SPEED_1000;
	}

	cmd->base.duplex = DUPLEX_FULL;
	cmd->base.port = PORT_DA;

	return 0;
}

static int xpcs_ethtool_ksettings_set(struct device *dev,
				      const struct ethtool_link_ksettings *cmd)
{
	u32 speed = cmd->base.speed;
	u32 mode;
	struct xpcs_prv_data *pdata;

	if (!dev)
		return -EINVAL;

	pdata = dev_get_drvdata(dev);
	if (!pdata) {
		dev_err(dev, "XPCS is not initialized\n");
		return -ENODEV;
	}

	if (pdata->mode == XPCS_MODE_QUSXGMII) {
		dev_err(dev,
			"%s: Force speed is not supported in USXGMII QUAD mode.\n",
			pdata->name);
		return -ENOTSUPP;
	}

	/* Ignore settings with speed = 0, which can happen if settings were get
	 * before a valid interface speed was detected and then applied back.
	 */
	if (speed == 0)
		return 0;

	if (speed != SPEED_10000 &&
	    speed != SPEED_1000 &&
	    speed != SPEED_100 &&
	    speed != SPEED_10 &&
	    speed != SPEED_2500) {
		dev_err(dev, "%s: XPCS invalid speed %d\n", pdata->name, speed);
		return -EINVAL;
	}

	if (speed == SPEED_1000 || speed == SPEED_100 || speed == SPEED_10)
		mode = XPCS_MODE_SGMII;
	else if (speed == SPEED_10000)
		mode = XPCS_MODE_10GKR;
	else if (speed == SPEED_2500)
		mode = XPCS_MODE_2500BASEX;
	else if (speed == SPEED_5000)
		mode = XPCS_MODE_5GBASER;
	else
		return -EINVAL;

	/* Restart Xpcs & PHY if mode or speed changes */
	if (mode != pdata->mode || speed != pdata->speed) {
		pdata->speed = speed;
		xpcs_reinit(pdata->dev, mode);
	}

	return 0;
}

static int xpcs_power_off(struct device *dev)
{
	struct xpcs_prv_data *pdata;
	struct xpcs_prv_data *pair_pdata = NULL;

	if (!dev)
		return -ENODEV;

	pdata = dev_get_drvdata(dev);
	if (!pdata) {
		dev_err(dev, "XPCS is not initialized\n");
		return -ENODEV;
	}

	if (!pdata->state)
		return 0;

	if (pdata->slave) {
		if (!pdata->pair) {
			dev_err(pdata->dev,
				"Can't find master device paired with %s",
				pdata->name);
			return -EINVAL;
		}

		pair_pdata = pdata;
		pdata = dev_get_drvdata(pdata->pair);
		if (!pdata) {
			dev_err(dev,
				"Master device paired with %s is not initialized\n",
				pair_pdata->name);
			return -ENODEV;
		}
	} else if (pdata->pair) {
		pair_pdata = dev_get_drvdata(pdata->pair);
	}

	pdata->state = 0;
	if (pair_pdata)
		pair_pdata->state = 0;

	/* Xpcs reset assert */
	xpcs_assert_reset(pdata);
	if (pair_pdata)
		xpcs_assert_reset(pair_pdata);

	/* By right, PHY is controlled by master.
	 * Just in case any special things need by done by combophy driver.
	 */
	if (pair_pdata && pair_pdata->phy) {
		/* RCU reset PHY */
		phy_power_off(pair_pdata->phy);
		/* Exit PHY */
		phy_exit(pair_pdata->phy);
	}

	if (pdata->phy) {
		/* RCU reset PHY */
		phy_power_off(pdata->phy);
		/* Exit PHY */
		phy_exit(pdata->phy);
	}

	if (pdata->clk)
		clk_disable_unprepare(pdata->clk);
	if (pair_pdata && pair_pdata->clk)
		clk_disable_unprepare(pair_pdata->clk);

	return 0;
}

static int xpcs_power_on(struct device *dev)
{
	struct xpcs_prv_data *pdata;
	struct xpcs_prv_data *pair_pdata = NULL;
	int ret, ret1;

	if (!dev)
		return -ENODEV;

	pdata = dev_get_drvdata(dev);
	if (!pdata) {
		dev_err(dev, "XPCS is not initialized\n");
		return -ENODEV;
	}

	if (pdata->state)
		return 0;

	if (pdata->slave) {
		if (!pdata->pair) {
			dev_err(pdata->dev,
				"Can't find master device paired with %s",
				pdata->name);
			return -EINVAL;
		}

		pair_pdata = pdata;
		pdata = dev_get_drvdata(pdata->pair);
		if (!pdata) {
			dev_err(dev,
				"Master device paired with %s is not initialized\n",
				pair_pdata->name);
			return -ENODEV;
		}
	} else if (pdata->pair) {
		pair_pdata = dev_get_drvdata(pdata->pair);
	}

	if (pair_pdata && pair_pdata->clk) {
		ret = clk_prepare_enable(pair_pdata->clk);
		if (ret) {
			dev_err(dev, "%s: cannot enable clock\n",
				pair_pdata->name);
			return ret;
		}
	}

	if (pdata->clk) {
		ret = clk_prepare_enable(pdata->clk);
		if (ret) {
			dev_err(dev, "%s: cannot enable clock\n", pdata->name);
			goto ERROR1;
		}
	}

	if (pdata->phy) {
		/* Init PHY */
		ret = phy_init(pdata->phy);
		if (ret < 0) {
			dev_err(pdata->dev, "%s: phy_init failed: %d\n",
				pdata->name, ret);
			goto ERROR2;
		}

		/* Power ON PHY */
		phy_power_on(pdata->phy);
	}

	/* By right, PHY is controlled by master.
	 * Just in case any special things need by done by combophy driver.
	 */
	if (pair_pdata && pair_pdata->phy) {
		/* Init PHY */
		ret = phy_init(pair_pdata->phy);
		if (ret < 0) {
			dev_err(pair_pdata->dev, "%s: phy_init failed: %d\n",
				pair_pdata->name, ret);
			goto ERROR3;
		}

		/* Power ON PHY */
		phy_power_on(pair_pdata->phy);
	}

	/* Xpcs reset deassert */
	ret1 = 0;
	if (pair_pdata) {
		ret = xpcs_deassert_reset(pair_pdata);
		if (ret < 0)
			goto ERROR4;
		ret1 = ret;
	}
	ret = xpcs_deassert_reset(pdata);
	if (ret < 0)
		goto ERROR5;
	if (ret || ret1)
		mdelay(10);

	/* Power ON XPCS */
	ret = xpcs_init(pdata);
	if (!ret) {
		pdata->state = 1;
		if (pair_pdata)
			pair_pdata->state = 1;

		return 0;
	}
	dev_err(pdata->dev, "%s: xpcs_init failed: %d\n", pdata->name, ret);

	xpcs_assert_reset(pdata);
ERROR5:
	if (pair_pdata)
		xpcs_assert_reset(pair_pdata);

ERROR4:
	if (pair_pdata && pair_pdata->phy) {
		/* RCU reset PHY */
		phy_power_off(pair_pdata->phy);
		/* Exit PHY */
		phy_exit(pair_pdata->phy);
	}
ERROR3:
	if (pdata->phy) {
		/* RCU reset PHY */
		phy_power_off(pdata->phy);
		/* Exit PHY */
		phy_exit(pdata->phy);
	}
ERROR2:
	if (pdata->clk)
		clk_disable_unprepare(pdata->clk);
ERROR1:
	if (pair_pdata && pair_pdata->clk)
		clk_disable_unprepare(pair_pdata->clk);

	dev_err(dev, "%s: %s failed: %d\n", pdata->name, __func__, ret);
	return ret;
}

int xpcs_reinit(struct device *dev, u32 mode)
{
	int ret;

	if (!dev) {
		dev_err(dev, "XPCS Reinit: cannot get dev\n");
		return -ENODEV;
	}

	ret = xpcs_power_off(dev);
	if (ret)
		return ret;

	/* Change mode to new mode */
	xpcs_set_mode(dev_get_drvdata(dev), mode);

	return xpcs_power_on(dev);
}

static int _xpcs_reinit(struct device *dev, u32 mode)
{
	struct xpcs_prv_data *pdata;

	if (!dev)
		return -EINVAL;

	pdata = dev_get_drvdata(dev);

	return mode == pdata->mode ? 0 : xpcs_reinit(dev, mode);
}

static int xpcs_get_mode(struct device *dev, u32 *mode_p)
{
	struct xpcs_prv_data *pdata;

	if (!dev || !mode_p)
		return -EINVAL;

	pdata = dev_get_drvdata(dev);

	return *mode_p = pdata->mode;
}

#if IS_ENABLED(CONFIG_MXL_P34X_FWDL)
static void xpcs_get_phy_reset(struct xpcs_prv_data *pdata)
{
	struct device_node *np = NULL;
	struct platform_device *pdev = NULL;
	struct p34x_ops *ops = NULL;

	pdata->phy_reset = NULL;
	pdata->phydev = NULL;

	np = of_get_parent(pdata->dev->of_node);
	if (np) {
		pdev = of_find_device_by_node(np);
		if (xpcs_p34x_phy(pdev)) {
			ops = p34x_get_ops(pdev);
			if (ops) {
				pdata->phydev = pdev;
				pdata->phy_reset = ops->phy_reset;
			}
		}
		of_node_put(np);
	}
}
#endif

static int xpcs_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct device *dev = &pdev->dev;
	int ret = 0;
	struct xpcs_prv_data *pdata = NULL;
	int i;

	dev_dbg(dev, "XPCS probe start\n");

	if (dev->of_node) {
		pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;
	} else { /* Read private data from end point */
		pdata = dev_get_drvdata(dev);
		if (!pdata) {
			dev_err(dev,
				"Get private data from end point failed!\n");
			return -EINVAL;
		}
	}

	pdata->id = pdev->id;
	pdata->dev = dev;
	pdata->ops.ethtool_ksettings_get = xpcs_ethtool_ksettings_get;
	pdata->ops.ethtool_ksettings_set = xpcs_ethtool_ksettings_set;
	pdata->ops.power_off = xpcs_power_off;
	pdata->ops.power_on = xpcs_power_on;
	pdata->ops.reinit_xpcs = _xpcs_reinit;
	pdata->ops.xpcs_get_mode = xpcs_get_mode;

	if (dev->of_node) {
		ret = xpcs_parse_dts(pdev, pdata);
		if (ret) {
			dev_err(dev, "xpcs dt parse failed (or deferred)!\n");
			return ret;
		}
	}

#if IS_ENABLED(CONFIG_MXL_P34X_FWDL)
	xpcs_get_phy_reset(pdata);
#endif

	pdata->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(pdata->clk)) {
		if (is_clk_required(dev)) {
			dev_err(dev, "%s: cannot get clock, error %ld",
				pdata->name, PTR_ERR(pdata->clk));
			return PTR_ERR(pdata->clk);
		}
		pdata->clk = NULL;
	}

	if (pdata->clk) {
		ret = clk_prepare_enable(pdata->clk);
		if (ret) {
			dev_err(dev, "%s: cannot enable clock, error %d\n",
				pdata->name, ret);
			return ret;
		}
	}

	ret = platform_get_irq(pdev, 0);
	dev_dbg(dev, "%s: irq_num: %d\n", pdata->name, ret);
	if (ret <= 0) {
		dev_err(dev, "%s: cannot get xpcs irq!\n", pdata->name);
		return !ret ? -ENODEV : ret;
	}
	pdata->irq_num = (u32)ret;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	dev_dbg(dev, "%s: IORESOURCE_MEM = %pr\n", pdata->name, res);
	if (!res) {
		dev_err(dev, "%s: failed to get iomem res (index 0)!\n",
			pdata->name);
		return -ENOENT;
	}
	dev_dbg(dev, "%s: %s iomem %pr\n", pdata->name, res->name, res);

	pdata->addr_base = devm_ioremap_resource(dev, res);
	dev_dbg(dev, "%s: base address %p\n", pdata->name, pdata->addr_base);
	if (IS_ERR(pdata->addr_base)) {
		dev_err(dev, "%s: failed to ioremap resource: %pr !\n",
			pdata->name, res);
		return -EINVAL;
	}

	pdata->phy = devm_phy_get(&pdev->dev, "phy");
	dev_dbg(dev, "%s: phy %p\n", pdata->name, pdata->phy);
	if (IS_ERR(pdata->phy)) {
		if (is_phy_required(dev)) {
			dev_err(dev, "%s: cannot get phy, error %ld\n",
				pdata->name, PTR_ERR(pdata->phy));
			return PTR_ERR(pdata->phy);
		}
		pdata->phy = NULL;
	}

	if (pdata->phy) {
		ret = phy_init(pdata->phy);
		if (ret < 0) {
			dev_err(dev, "%s: phy_init failed: %d\n",
				pdata->name, ret);
			return ret;
		}

		phy_power_on(pdata->phy);
	}

	for (i = 0; i < ARRAY_SIZE(pdata->xpcs_rst); i++) {
		pdata->xpcs_rst[i] = devm_reset_control_get_by_index(dev, i);
		if (IS_ERR(pdata->xpcs_rst[i]))
			pdata->xpcs_rst[i] = NULL;
	}

	if (xpcs_reset(dev)) {
		dev_err(dev, "Failed to do %s reset!\n", pdata->name);
		return -EINVAL;
	}

	if (!pdata->slave) {
		ret = xpcs_init(pdata);
		if (ret) {
			dev_err(dev, "%s: Initialization Failed: %d!!\n",
				pdata->name, ret);
		} else {
			pdata->state = 1;
			if (pdata->pair) {
				struct xpcs_prv_data *pair_pdata;

				pair_pdata = dev_get_drvdata(pdata->pair);
				pair_pdata->state = 1;
			}
		}
	}

	if (xpcs_sysfs_init(pdata)) {
		dev_err(dev, "%s: sysfs init failed!\n", pdata->name);
		return -EINVAL;
	}

	dev_info(dev, "%s: XPCS init succeed\n", pdata->name);
	return 0;
}

static int xpcs_release(struct platform_device *pdev)
{
	struct xpcs_prv_data *pdata = platform_get_drvdata(pdev);

	if (!pdata)
		return 0;

	xpcs_assert_reset(pdata);

	if (pdata->phy) {
		phy_power_off(pdata->phy);
		phy_exit(pdata->phy);
	}

	if (pdata->clk)
		clk_disable_unprepare(pdata->clk);

	return 0;
}

static const struct xpcs_hw_config config_lgm = {
	.phy_type = XPCS_PHY_TYPE_16G,
	.has_sram = true,
	.usxg_sport = true,
	.usxg_qport = true,
	.rsvdparam11 = true,
};

static const struct xpcs_hw_config config_lgm_c10 = {
	.phy_type = XPCS_PHY_TYPE_8G,
	.has_sram = true,
	.usxg_sport = true,
	.usxg_qport = true,
	.rsvdparam11 = true,
};

static const struct xpcs_hw_config config_prx = {
	.phy_type = XPCS_PHY_TYPE_12G,
};

static const struct of_device_id mxl_xpcs_dev_match[] = {
	{ .compatible = "mxl,lgm-xpcs", .data = &config_lgm, },
	{ .compatible = "mxl,lgm-xpcs-c10", .data = &config_lgm_c10, },
	{ .compatible = "mxl,prx-xpcs", .data = &config_prx, },
	{},
};
MODULE_DEVICE_TABLE(of, mxl_xpcs_dev_match);

static struct platform_driver mxl_xpcs_driver = {
	.probe = xpcs_probe,
	.remove = xpcs_release,
	.driver = {
		.name = XPCS_DEV_NAME,
		.of_match_table = of_match_ptr(mxl_xpcs_dev_match),
	}
};

module_platform_driver(mxl_xpcs_driver);

MODULE_AUTHOR("Xu Liang");
MODULE_DESCRIPTION("Maxlinear XPCS Device driver");
MODULE_LICENSE("GPL v2");
