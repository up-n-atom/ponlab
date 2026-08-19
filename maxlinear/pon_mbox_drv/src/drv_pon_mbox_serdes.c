/******************************************************************************
 *
 * Copyright (c) 2021 - 2025 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <linux/err.h>
#include <linux/firmware.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/types.h>

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox_ikm.h>

#include <drv_pon_mbox_serdes.h>

/** \addtogroup PON_MBOX_SERDES
 *  @{
 */

static int pon_mbox_pon_mode_check(struct pon_mbox *pon)
{
	switch (pon->mode) {
	case PON_MODE_984_GPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
	case PON_MODE_987_XGPON:
		break;
	default:
		dev_err(pon->dev, "invalid PON mode %d\n", pon->mode);
		return -EINVAL;
	}

	return 0;
}

static void pon_mbox_pon_app_write(struct pon_mbox *pon, u32 value, u32 reg)
{
	if (pon->pon_app) {
		__raw_writel(value, pon->pon_app + reg);
		dev_dbg(pon->dev, "app w 0x%08x 0x%08x:\n", reg, value);
	}
}

static u32 pon_mbox_pon_app_read(struct pon_mbox *pon, u32 reg)
{
	if (pon->pon_app)
		return __raw_readl(pon->pon_app + reg);

	return 0;
}

static void pon_mbox_pon_app_mask(struct pon_mbox *pon, u32 clr, u32 set,
				  u32 reg)
{
	u32 val;

	val = pon_mbox_pon_app_read(pon, reg);
	val &= ~clr;
	val |= set;
	pon_mbox_pon_app_write(pon, val, reg);
}

static void pon_mbox_pon_apb_app_write(struct pon_mbox *pon, u32 value, u32 reg)
{
	if (pon->pon_apb_app) {
		__raw_writel(value, pon->pon_apb_app + reg);
		dev_dbg(pon->dev, "apb w 0x%08x 0x%08x:\n", reg, value);
	}
}

static void pon_mbox_serdes_write(struct pon_mbox *pon, u32 value, u32 reg)
{
	if (pon->serdes) {
		__raw_writel(value, pon->serdes + reg);
		dev_dbg(pon->dev, "serdes w 0x%08x 0x%08x:\n", reg, value);
	}
}

/* SerDes has 16 bit registers but SW representation is 32 bit wide.
 * Offset between subsequent 16 bit words is then 4 bytes
 */
static void pon_mbox_serdes_fw_write(struct pon_mbox *pon, const u16 *value,
				     u32 offset)
{
	u32 val = (u32)be16_to_cpu(*value);

	if (pon->serdes_sram) {
		__raw_writel(val, pon->serdes_sram + offset);
		/* dev_dbg(pon->dev, "serdes fw w 0x%x 0x%x\n", offset, val); */
	}
}

static u32 pon_mbox_serdes_read(struct pon_mbox *pon, u32 reg)
{
	if (pon->serdes)
		return __raw_readl(pon->serdes + reg);

	return 0;
}

static void pon_mbox_serdes_mask(struct pon_mbox *pon, u32 clr, u32 set,
				 u32 reg)
{
	u32 val;

	val = pon_mbox_serdes_read(pon, reg);
	val &= ~clr;
	val |= set;
	pon_mbox_serdes_write(pon, val, reg);
}

static const char *pon_mbox_get_serdes_fw_filename(struct pon_mbox *pon)
{
	/* same serdes FW for URX-B and C */
	if (pon->hw_ver == PON_MBOX_HW_VER_URX_B_TYPE ||
	    pon->hw_ver == PON_MBOX_HW_VER_URX_C_TYPE)
		return SRDS_FW_URX800_B;
	else if (pon->hw_ver == PON_MBOX_HW_VER_URX_A_TYPE)
		return SRDS_FW_URX800_A;
	else if (pon->hw_ver == PON_MBOX_HW_VER_B_TYPE)
		return SRDS_FW_PRX300_B;
	else
		return NULL;
}

static int pon_mbox_serdes_fw_download(struct pon_mbox *pon)
{
	const struct firmware *fw_entry;
	const u16 *fw_ptr;
	int ret;
	int i;
	const char *fw_name;

	if (!pon->serdes_sram) {
		dev_err(pon->dev, "SerDes sram not available\n");
		return -ENODEV;
	}

	fw_name = pon_mbox_get_serdes_fw_filename(pon);
	if (!fw_name) {
		dev_err(pon->dev, "No SerDes FW\n");
		return -ENOENT;
	}

	dev_dbg(pon->dev, "Download SerDes FW: %s\n", fw_name);
	ret = request_firmware_direct(&fw_entry, fw_name, pon->dev);
	if (ret) {
		dev_err(pon->dev, "failed to get SerDes FW file %s: ret: %i\n",
			fw_name, ret);
		return ret;
	}

	/* 32bit values written but upper 16 bits are unused ->
	 * 4 byte offset to be used within subsequent writes
	 */
	fw_ptr = (u16 *)fw_entry->data;
	dev_dbg(pon->dev, "FW size: %zd\n", fw_entry->size);
	for (i = 0; i < (fw_entry->size / 2); i++, fw_ptr++)
		pon_mbox_serdes_fw_write(pon, fw_ptr, i << 2);

	release_firmware(fw_entry);

	return 0;
}

int urx800_pon_shell_init(struct pon_mbox *pon)
{
	u32 set, clr;

	dev_dbg(pon->dev, "executing PON shell init\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	clr = PON_SHELL_GEN_CTRL_PON_CLK_GATE |
	      PON_SHELL_GEN_CTRL_CR_PARA_CLK_GATE;

	set = PON_SHELL_GEN_CTRL_PON_RST |
	      PON_SHELL_GEN_CTRL_CR_PARA_CLK_DIV2_EN;

	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_GEN_CTRL);

	return 0;
}

int urx800_serdes_basic_init(struct pon_mbox *pon)
{
	u32 set, clr;

	dev_dbg(pon->dev, "executing basic init for SerDes\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->pon_apb_app) {
		dev_err(pon->dev, "pon apb not available\n");
		return -ENODEV;
	}

	pon_mbox_pon_apb_app_write(pon, PON_SHELL_ARB_TIMEOUT_CTRL_VAL,
				   PON_SHELL_ARB);

	pon_mbox_pon_app_mask(pon, PON_SHELL_SRAM_CSR_SRAM_BYPASS, 0,
			      PON_SHELL_SRAM_CSR);

	clr = PON_SHELL_GEN_CTRL_REF_USE_PAD | PON_SHELL_GEN_CTRL_PHY_RST |
	      PON_SHELL_GEN_CTRL_XPCS_RST | PON_SHELL_GEN_CTRL_REF_CLK_EN |
	      PON_SHELL_GEN_CTRL_RX_DATA_EN | PON_SHELL_GEN_CTRL_PON_CLK_GATE |
	      PON_SHELL_GEN_CTRL_CR_PARA_CLK_GATE;

	set = PON_SHELL_GEN_CTRL_TX_RESET | PON_SHELL_GEN_CTRL_RX_RESET |
	      PON_SHELL_GEN_CTRL_PON_RST | PON_SHELL_GEN_CTRL_AON_CLK_GATE |
	      PON_SHELL_GEN_CTRL_CR_PARA_CLK_DIV2_EN;

	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_GEN_CTRL);

	pon_mbox_pon_app_mask(pon, PON_SHELL_MODE_SEL_PON_ETH_SEL, 0,
			      PON_SHELL_MODE_SEL);

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_EXT_CTRL_SEL_PHY_CTRL_SEL,
			      PON_SHELL_EXT_CTRL_SEL);

	return 0;
}

int urx800_ref_clk_sel(struct pon_mbox *pon)
{
	int ret;

	dev_dbg(pon->dev, "executing reference clock selection\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->cgu) {
		dev_err(pon->dev, "cgu-syscon not available\n");
		return -ENODEV;
	}

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_MODE_SEL_LOOP_SYNCE_SEL,
			      PON_SHELL_MODE_SEL);

	ret = regmap_update_bits(pon->cgu, URX_CGU_IF_CLK2,
				 URX_CGU_IF_CLK2_P2_SEL, 0);
	if (ret)
		return ret;

	pon_mbox_pon_app_mask(pon, PON_SHELL_MODE_SEL_10GEPON_SEL, 0,
			      PON_SHELL_MODE_SEL);

	pon_mbox_pon_app_mask(pon, PON_SHELL_MODE_SEL_PON_SHELL_REFCLK_EN, 0,
			      PON_SHELL_MODE_SEL);

	return 0;
}

static int urx8x_pll5_init(struct pon_mbox *pon, bool frefcmlen)
{
	u32 set, clr, val;
	int ret;

	dev_dbg(pon->dev, "executing PLL5 init\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->lt_cfg.valid) {
		dev_err(pon->dev, "loop timing config not valid\n");
		return -EPERM;
	}

	set = (1 << PON_SHELL_LOOP_PLL_CFG2_REFDIV_SHIFT);
	/* LGM-A and B require this bit 1, LGM-C inverted */
	if (frefcmlen)
		set |= PON_SHELL_LOOP_PLL_CFG2_FREFCMLEN;
	clr = PON_SHELL_LOOP_PLL_CFG2_FBDIV_MASK |
	      PON_SHELL_LOOP_PLL_CFG2_REFDIV_MASK |
	      PON_SHELL_LOOP_PLL_CFG2_FREFCMLEN;
	/* FBDIV */
	switch (pon->mode) {
	case PON_MODE_984_GPON:
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		if (!pon->lt_cfg.loop_ps_en)
			set |= 248;
		else
			set |= 124;
		break;
	default:
		return -EINVAL;
	}

	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_LOOP_PLL_CFG2);

	clr = PON_SHELL_LOOP_PLL_CFG0_FRAC_MASK |
	      PON_SHELL_LOOP_PLL_CFG0_DSMEN | PON_SHELL_LOOP_PLL_CFG0_DACEN;
	/* No implementation for PRBS generation - to be done in FW */
	switch (pon->mode) {
	case PON_MODE_984_GPON:
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		if (!pon->lt_cfg.loop_ps_en)
			set = PON_SHELL_LOOP_PLL_CFG0_DSMEN |
			      PON_SHELL_LOOP_PLL_CFG0_DACEN |
			      0xD4FDF3 << PON_SHELL_LOOP_PLL_CFG0_FRAC_SHIFT;
		else
			set = PON_SHELL_LOOP_PLL_CFG0_DSMEN |
			      PON_SHELL_LOOP_PLL_CFG0_DACEN |
			      0x6A7EFA << PON_SHELL_LOOP_PLL_CFG0_FRAC_SHIFT;
		break;
	default:
		return -EINVAL;
	}

	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_LOOP_PLL_CFG0);

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_LOOP_PLL_CFG0_PLLEN,
			      PON_SHELL_LOOP_PLL_CFG0);

	/* wait for PLL5 lock */
	ret = readl_poll_timeout(pon->pon_app + PON_SHELL_LOOP_PLL_CFG0, val,
				 val & PON_SHELL_LOOP_PLL_CFG0_LOCK, 10, 50000);
	if (!ret) {
		dev_dbg(pon->dev, "pll5 locked successfully (0x%x)\n", val);
	} else {
		val = pon_mbox_pon_app_read(pon, PON_SHELL_LOOP_PLL_CFG0);
		dev_err(pon->dev, "pll5 not locked: %i (0x%x)\n", ret, val);
		return ret;
	}

	/* POST_DIV0A */
	set = 7 | PON_SHELL_LOOP_PLL_CFG1_FOUT0EN |
	      PON_SHELL_LOOP_PLL_CFG1_FOUT1EN;
	if (!pon->lt_cfg.loop_ps_en)
		set |= 1 << PON_SHELL_LOOP_PLL_CFG1_POST_DIV0B_SHIFT |
		       3 << PON_SHELL_LOOP_PLL_CFG1_POST_DIV1A_SHIFT;
	else
		set |= 0 << PON_SHELL_LOOP_PLL_CFG1_POST_DIV0B_SHIFT |
		       1 << PON_SHELL_LOOP_PLL_CFG1_POST_DIV1A_SHIFT;

	clr = PON_SHELL_LOOP_PLL_CFG1_FOUT0EN |
	      PON_SHELL_LOOP_PLL_CFG1_FOUT1EN |
	      PON_SHELL_LOOP_PLL_CFG1_POST_DIV0PRE |
	      PON_SHELL_LOOP_PLL_CFG1_POST_DIV0A_MASK |
	      PON_SHELL_LOOP_PLL_CFG1_POST_DIV0B_MASK |
	      PON_SHELL_LOOP_PLL_CFG1_POST_DIV1A_MASK |
	      PON_SHELL_LOOP_PLL_CFG1_POST_DIV1B_MASK;

	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_LOOP_PLL_CFG1);

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_MODE_SEL_PON_SHELL_REFCLK_EN,
			      PON_SHELL_MODE_SEL);

	return 0;
}

int urx800_pll5_init(struct pon_mbox *pon)
{
	return urx8x_pll5_init(pon, true);
}

int urx800c_pll5_init(struct pon_mbox *pon)
{
	return urx8x_pll5_init(pon, false);
}

#if 0
/* Currently not used but kept as it might be needed in case power
 * have to be toggled.
 */
static int urx800_serdes_rxtx_stable_check(struct pon_mbox *pon)
{
	u32 val;
	int ret;

	if (!pon->serdes) {
		dev_err(pon->dev, "serdes not available\n");
		return -ENODEV;
	}

	/* check PHY_RESET bit is deasserted */
	ret = readl_poll_timeout(pon->serdes + PON_PHY_SUP_DIG_ASIC_IN, val,
				 (val & PON_PHY_SUP_DIG_ASIC_IN_PHY_RESET) == 0,
				 10, 50000);

	if (!ret) {
		val = pon_mbox_serdes_read(pon, PON_PHY_SUP_DIG_ASIC_IN);
		dev_dbg(pon->dev, "PHY RESET is deassered: 0x%x 0x%x\n",
			PON_PHY_SUP_DIG_ASIC_IN, val);
	} else {
		val = pon_mbox_serdes_read(pon, PON_PHY_SUP_DIG_ASIC_IN);
		dev_err(pon->dev, "PHY RESET not deasserted: %i (0x%x)\n", ret,
			val);
		return ret;
	}

	/* wait rx power stable */
	ret = readl_poll_timeout(
		pon->serdes + PON_PHY_SUP_DIG_ASIC_RX_IN_0, val,
		(val & PON_PHY_SUP_DIG_ASIC_RX_IN_0_PSTATE_MASK) != 0x60, 10,
		50000);

	if (!ret) {
		val = pon_mbox_serdes_read(pon, PON_PHY_SUP_DIG_ASIC_RX_IN_0);
		dev_err(pon->dev, "RX power stable: 0x%x 0x%x\n",
			PON_PHY_SUP_DIG_ASIC_RX_IN_0, val);
	} else {
		val = pon_mbox_serdes_read(pon, PON_PHY_SUP_DIG_ASIC_RX_IN_0);
		dev_dbg(pon->dev, "RX power not stable: %i (0x%x)\n", ret, val);
		return ret;
	}

	/* wait tx power stable */
	ret = readl_poll_timeout(
		pon->serdes + PON_PHY_SUP_DIG_ASIC_TX_IN_0, val,
		(val & PON_PHY_SUP_DIG_ASIC_TX_IN_0_PSTATE_MASK) != 0xC0, 10,
		50000);

	if (!ret) {
		val = pon_mbox_serdes_read(pon, PON_PHY_SUP_DIG_ASIC_TX_IN_0);
		dev_dbg(pon->dev, "TX power stable: 0x%x 0x%x\n",
			PON_PHY_SUP_DIG_ASIC_TX_IN_0, val);
	} else {
		val = pon_mbox_serdes_read(pon, PON_PHY_SUP_DIG_ASIC_TX_IN_0);
		dev_err(pon->dev, "TX power not stable: %i (0x%x)\n", ret, val);
		return ret;
	}

	return 0;
}
#endif

static int urx800_serdes_cal_ovrd_workaround_step_a(struct pon_mbox *pon)
{
	u32 set, clr;
	u32 val;
	int ret;

	if (!pon->serdes) {
		dev_err(pon->dev, "SerDes not available\n");
		return -ENODEV;
	}

	clr = PON_PHY_SUP_DIG_SUP_OVRD_IN_0_RES_REQ_IN;
	set = PON_PHY_SUP_DIG_SUP_OVRD_IN_0_RES_ACK_IN |
	      PON_PHY_SUP_DIG_SUP_OVRD_IN_0_RES_OVRD_EN;

	pon_mbox_serdes_mask(pon, clr, set, PON_PHY_SUP_DIG_SUP_OVRD_IN_0);

	/* wait for FW PWRUP done */
	ret = readl_poll_timeout(pon->serdes + PON_PHY_RAWCMN_DIG_FW_PWRUP_DONE,
				 val,
				 val & PON_PHY_RAWCMN_DIG_FW_PWRUP_DONE_DONE,
				 10, 50000);

	if (!ret) {
		val = pon_mbox_serdes_read(pon,
					   PON_PHY_RAWCMN_DIG_FW_PWRUP_DONE);
		dev_dbg(pon->dev, "FW PWRUP done indication: 0x%x 0x%x\n",
			PON_PHY_RAWCMN_DIG_FW_PWRUP_DONE, val);
	} else {
		val = pon_mbox_serdes_read(pon,
					   PON_PHY_RAWCMN_DIG_FW_PWRUP_DONE);
		dev_err(pon->dev,
			"Missing FW PWRUP done indication: %i (0x%x)\n", ret,
			val);
		return ret;
	}

	/* wait for common calibration done */
	ret = readl_poll_timeout(
		pon->serdes + PON_PHY_RAWCMN_DIG_AON_CMNCAL_STATUS, val,
		val & PON_PHY_RAWCMN_DIG_AON_CMNCAL_STATUS_CMNCAL_DONE, 10,
		50000);

	if (!ret) {
		val = pon_mbox_serdes_read(
			pon, PON_PHY_RAWCMN_DIG_AON_CMNCAL_STATUS);
		dev_dbg(pon->dev, "Calibration done indication: r 0x%x 0x%x\n",
			PON_PHY_RAWCMN_DIG_AON_CMNCAL_STATUS, val);
	} else {
		val = pon_mbox_serdes_read(
			pon, PON_PHY_RAWCMN_DIG_AON_CMNCAL_STATUS);
		dev_err(pon->dev,
			"Missing calibration done indication: %i (0x%x)\n", ret,
			val);
		return ret;
	}

	return 0;
}

/**
 * Function to check for reference clk bit level
 */
static int urx800_ref_clk_wait(struct pon_mbox *pon, u8 lvl)
{
	int ret;
	u32 val;

	if (lvl)
		/* wait for ref clock req to go high*/
		ret = readl_poll_timeout(pon->pon_app + PON_SHELL_GEN_CTRL, val,
					 val & PON_SHELL_GEN_CTRL_REF_CLK_REQ,
					 10, 50000);
	else
		/* wait for ref clock req to go low */
		ret = readl_poll_timeout(pon->pon_app + PON_SHELL_GEN_CTRL, val,
					 (val &
					  PON_SHELL_GEN_CTRL_REF_CLK_REQ) !=
						 PON_SHELL_GEN_CTRL_REF_CLK_REQ,
					 10, 50000);
	if (ret) {
		val = pon_mbox_pon_app_read(pon, PON_SHELL_GEN_CTRL);
		dev_err(pon->dev, "Ref clock failure: %i (0x%x)\n", ret, val);
		return ret;
	}

	return 0;
}

/** Mode dependent Serdes specific settings for URX family. */
static const struct pon_mbox_serdes_init pon_mbox_serdes_urx800_init_val[6] = {
	/** Dummy for unknown mode */
	{ .ref_range = 0,
	  .mplla_bandwidth = 0,
	  .mplla_multiplier = 0,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0,
	  .rx_ref_ld_val = 0,
	  .tx_rate = 0,
	  .rx_rate = 0 },
	/** ITU-T G.984 (GPON). */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0xA035,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 3,
	  .rx_rate = 2 },
	/** ITU - T G.987 (XG - PON) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0xA017,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 2,
	  .rx_rate = 0 },
	/** ITU-T G.9807 (XGS-PON) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0xA016,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 0,
	  .rx_rate = 0 },
	/** ITU-T G.989 (NG-PON2 2.5G upstream) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0xA017,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 2,
	  .rx_rate = 0 },
	/** ITU-T G.989 (NG-PON2 10G upstream) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0xA016,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 0,
	  .rx_rate = 0 },
};

int urx800_serdes_init(struct pon_mbox *pon)
{
	u32 set, clr, val;
	int ret;
	enum pon_mode use_mode = pon->mode;

	dev_dbg(pon->dev, "executing SerDes init\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->serdes) {
		dev_err(pon->dev, "SerDes not available\n");
		return -ENODEV;
	}

	if (!pon->serdes_cfg.valid) {
		dev_err(pon->dev, "no valid SerDes config\n");
		return -EINVAL;
	}

	if (!pon->iop_cfg.valid) {
		dev_err(pon->dev, "no valid IOP config\n");
		return -EINVAL;
	}

	if (pon_mbox_pon_mode_check(pon))
		return -EINVAL;

	if (pon->mode == PON_MODE_987_XGPON && pon->iop_cfg.msk.iop5) {
		use_mode = PON_MODE_9807_XGSPON;
		dev_dbg(pon->dev, "IOP bit 5 -> XGSPON settings used\n");
	}

	clr = PON_SHELL_GEN_CTRL_RX_CDR_SSC_EN |
	      PON_SHELL_GEN_CTRL_RX_TERM_ACDC |
	      PON_SHELL_GEN_CTRL_TX_RATE_MASK | PON_SHELL_GEN_CTRL_RX_RATE_MASK;
	set = PON_SHELL_GEN_CTRL_RX_TERM_ACDC |
	      (pon_mbox_serdes_urx800_init_val[use_mode].tx_rate
	       << PON_SHELL_GEN_CTRL_TX_RATE_SHIFT) |
	      (pon_mbox_serdes_urx800_init_val[use_mode].rx_rate
	       << PON_SHELL_GEN_CTRL_RX_RATE_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_GEN_CTRL);

	clr = PON_SHELL_REF_CLK_CTRL_MPLLA_DIV2_EN |
	      PON_SHELL_REF_CLK_CTRL_DIV2_EN |
	      PON_SHELL_REF_CLK_CTRL_REF_RANGE_MASK;
	set = PON_SHELL_REF_CLK_CTRL_MPLLA_DIV2_EN |
	      (pon_mbox_serdes_urx800_init_val[use_mode].ref_range
	       << PON_SHELL_REF_CLK_CTRL_REF_RANGE_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_REF_CLK_CTRL);

	clr = PON_SHELL_EXT_MPLLA_CTRL_0_BANDWIDTH_MASK |
	      PON_SHELL_EXT_MPLLA_CTRL_0_TX_CLK_DIV_MASK;
	set = pon_mbox_serdes_urx800_init_val[use_mode].mplla_bandwidth;
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_MPLLA_CTRL_0);

	clr = PON_SHELL_EXT_MPLLA_CTRL_1_DIV10_CLK_EN |
	      PON_SHELL_EXT_MPLLA_CTRL_1_DIV16P5_CLK_EN |
	      PON_SHELL_EXT_MPLLA_CTRL_1_DIV8_CLK_EN |
	      PON_SHELL_EXT_MPLLA_CTRL_1_DIV_CLK_EN |
	      PON_SHELL_EXT_MPLLA_CTRL_1_MULTIPLIER_MASK |
	      PON_SHELL_EXT_MPLLA_CTRL_1_DIV_MULTIPLIER_MASK;
	set = PON_SHELL_EXT_MPLLA_CTRL_1_DIV8_CLK_EN |
	      (pon_mbox_serdes_urx800_init_val[use_mode].mplla_multiplier
	       << PON_SHELL_EXT_MPLLA_CTRL_1_MULTIPLIER_SHIFT);

	if (pon_mbox_serdes_urx800_init_val[use_mode].mplla_div16p5_clk_en)
		set |= PON_SHELL_EXT_MPLLA_CTRL_1_DIV16P5_CLK_EN;

	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_MPLLA_CTRL_1);

	clr = PON_SHELL_EXT_CALI_CTRL_1_RX_VCO_LD_VAL_MASK |
	      PON_SHELL_EXT_CALI_CTRL_1_RX_REF_LD_VAL_MASK;
	set = pon_mbox_serdes_urx800_init_val[use_mode].rx_vco_ld_val |
	      (pon_mbox_serdes_urx800_init_val[use_mode].rx_ref_ld_val
	       << PON_SHELL_EXT_CALI_CTRL_1_RX_REF_LD_VAL_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_CALI_CTRL_1);

	/* not part of the spec - has been added during debugging */
	pon_mbox_pon_app_mask(pon, 0,
			      PON_SHELL_EXT_MPLLB_CTRL_MPLLB_INIT_CAL_DISABLE,
			      PON_SHELL_EXT_MPLLB_CTRL_0);

	clr = PON_SHELL_EXT_MISC_CTRL0_RX_VREF_CTRL_MASK |
	      PON_SHELL_EXT_MISC_CTRL0_REF_REPEAT_CLK_EN |
	      PON_SHELL_EXT_MISC_CTRL0_SUP_MISC_MASK |
	      PON_SHELL_EXT_MISC_CTRL0_TX_VBOOST_LVL_MASK |
	      PON_SHELL_EXT_MISC_CTRL0_TX_IBOOST_LVL_MASK;
	set = (17 << PON_SHELL_EXT_MISC_CTRL0_RX_VREF_CTRL_SHIFT) |
	      (1 << PON_SHELL_EXT_MISC_CTRL0_SUP_MISC_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_VBOOST_LVL]
	       << PON_SHELL_EXT_MISC_CTRL0_TX_VBOOST_LVL_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_IBOOST_LVL]
	       << PON_SHELL_EXT_MISC_CTRL0_TX_IBOOST_LVL_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_MISC_CTRL0);

	clr = PON_SHELL_EXT_MISC_CTRL1_TX_WIDTH_MASK |
	      PON_SHELL_EXT_MISC_CTRL1_TX_MPLL_EN |
	      PON_SHELL_EXT_MISC_CTRL1_TX_MPLLB_SEL |
	      PON_SHELL_EXT_MISC_CTRL1_TX_CLK_RDY |
	      PON_SHELL_EXT_MISC_CTRL1_TX_VBOOST_EN |
	      PON_SHELL_EXT_MISC_CTRL1_TX_EQ_PRE_MASK |
	      PON_SHELL_EXT_MISC_CTRL1_TX_EQ_MAIN_MASK |
	      PON_SHELL_EXT_MISC_CTRL1_TX_EQ_POST_MASK;
	set = (2 << PON_SHELL_EXT_MISC_CTRL1_TX_WIDTH_SHIFT) |
	      PON_SHELL_EXT_MISC_CTRL1_TX_MPLL_EN |
	      PON_SHELL_EXT_MISC_CTRL1_TX_CLK_RDY |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_TX_EQ_PRE]
	       << PON_SHELL_EXT_MISC_CTRL1_TX_EQ_PRE_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_TX_EQ_MAIN]
	       << PON_SHELL_EXT_MISC_CTRL1_TX_EQ_MAIN_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_TX_EQ_POST]
	       << PON_SHELL_EXT_MISC_CTRL1_TX_EQ_POST_SHIFT);
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_VBOOST_EN])
		set |= PON_SHELL_EXT_MISC_CTRL1_TX_VBOOST_EN;
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_MISC_CTRL1);

	clr = PON_SHELL_EXT_MISC_CTRL2_RX_WIDTH_MASK |
	      PON_SHELL_EXT_MISC_CTRL2_RX_ADAPT_IN_PROG |
	      PON_SHELL_EXT_MISC_CTRL2_RX_ADAPT_MODE_MASK |
	      PON_SHELL_EXT_MISC_CTRL2_RX_MISC_MASK |
	      PON_SHELL_EXT_MISC_CTRL2_RX_ADAPT_CONT;
	set = (2 << PON_SHELL_EXT_MISC_CTRL2_RX_WIDTH_SHIFT) |
	      PON_SHELL_EXT_MISC_CTRL2_RX_TERM_EN |
	      PON_SHELL_EXT_MISC_CTRL2_RX_DIV16P5_CLK_EN |
	      PON_SHELL_EXT_MISC_CTRL2_RX_OFFCAN_COUNT |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_EQ_ADAPT_MODE]
	       << PON_SHELL_EXT_MISC_CTRL2_RX_ADAPT_MODE_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_MISC]
	       << PON_SHELL_EXT_MISC_CTRL2_RX_MISC_SHIFT);
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_ADAPT_CONT])
		set |= PON_SHELL_EXT_MISC_CTRL2_RX_ADAPT_CONT;
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_MISC_CTRL2);

	clr = PON_SHELL_EXT_CDR_CTRL_RX_CDR_PPM_MAX_MASK |
	      PON_SHELL_EXT_CDR_CTRL_RX_DFE_BYPASS |
	      PON_SHELL_EXT_CDR_CTRL_RX_ADAPT_SEL |
	      PON_SHELL_EXT_CDR_CTRL_RX_CDR_VCO_FREQBAND_MASK |
	      PON_SHELL_EXT_CDR_CTRL_RX_CDR_VCO_STEP_CTRL |
	      PON_SHELL_EXT_CDR_CTRL_RX_DELTA_IQ_MASK |
	      PON_SHELL_EXT_CDR_CTRL_RX_MARGIN_IQ_MASK;
	set = (18 << PON_SHELL_EXT_CDR_CTRL_RX_CDR_PPM_MAX_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_VCO_FRQBAND]
	       << PON_SHELL_EXT_CDR_CTRL_RX_CDR_VCO_FREQBAND_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_DELTA_IQ]
	       << PON_SHELL_EXT_CDR_CTRL_RX_DELTA_IQ_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_MARGIN_IQ]
	       << PON_SHELL_EXT_CDR_CTRL_RX_MARGIN_IQ_SHIFT);
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_DFE_BYPASS])
		set |= PON_SHELL_EXT_CDR_CTRL_RX_DFE_BYPASS;
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_EQ_ADAPT_SEL])
		set |= PON_SHELL_EXT_CDR_CTRL_RX_ADAPT_SEL;
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_VCO_STEP_CTRL])
		set |= PON_SHELL_EXT_CDR_CTRL_RX_CDR_VCO_STEP_CTRL;
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_VCO_TEMP_COMP_EN])
		set |= PON_SHELL_EXT_CDR_CTRL_RX_CDR_VCO_TEMP_COMP_EN;
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_CDR_CTRL);

	clr = PON_SHELL_EXT_EQ_CTRL_ATT_LVL_G1_MASK |
	      PON_SHELL_EXT_EQ_CTRL_CTLE_BOOST_G1_MASK |
	      PON_SHELL_EXT_EQ_CTRL_CTLE_POLE_MASK |
	      PON_SHELL_EXT_EQ_CTRL_DFE_TAP1_G1_MASK |
	      PON_SHELL_EXT_EQ_CTRL_VGA1_GAIN_G1_MASK |
	      PON_SHELL_EXT_EQ_CTRL_VGA2_GAIN_G1_MASK;
	set = (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_EQ_ATT_LVL]
	       << PON_SHELL_EXT_EQ_CTRL_ATT_LVL_G1_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_EQ_CTLE_BOOST]
	       << PON_SHELL_EXT_EQ_CTRL_CTLE_BOOST_G1_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_CTLE_POLE]
	       << PON_SHELL_EXT_EQ_CTRL_CTLE_POLE_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_DFE_TAP1]
	       << PON_SHELL_EXT_EQ_CTRL_DFE_TAP1_G1_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_VGA1_GAIN]
	       << PON_SHELL_EXT_EQ_CTRL_VGA1_GAIN_G1_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_VGA2_GAIN]
	       << PON_SHELL_EXT_EQ_CTRL_VGA2_GAIN_G1_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_EQ_CTRL_1);

	clr = PON_SHELL_EXT_BS_CTRL_RX_LEVEL |
	      PON_SHELL_EXT_BS_CTRL_TX_LOWSWING;
	set = 9 | PON_SHELL_EXT_BS_CTRL_RX_BIGSWING;
	pon_mbox_pon_app_mask(pon, clr, set, PON_SHELL_EXT_BS_CTRL);

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_GEN_CTRL_REF_CLK_EN,
			      PON_SHELL_GEN_CTRL);

	ret = urx800_ref_clk_wait(pon, 1);
	if (ret)
		return ret;

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_GEN_CTRL_PHY_RST,
			      PON_SHELL_GEN_CTRL);

	pon_mbox_pon_app_mask(pon, PON_SHELL_SRAM_CSR_SRAM_BYPASS, 0,
			      PON_SHELL_SRAM_CSR);

	/* wait for SRAM init done */
	ret = readl_poll_timeout(pon->pon_app + PON_SHELL_SRAM_CSR, val,
				 val & PON_SHELL_SRAM_CSR_SRAM_INIT_DONE, 10,
				 50000);
	if (!ret) {
		dev_dbg(pon->dev, "SRAM init successfully (0x%x)\n", val);
	} else {
		val = pon_mbox_pon_app_read(pon, PON_SHELL_SRAM_CSR);
		dev_err(pon->dev, "SRAM init failed: %i (0x%x)\n", ret, val);
		return ret;
	}

	/* check for SerDes FW to be downloaded*/
	if (pon->serdes_fw_dl) {
		pon_mbox_pon_app_mask(pon, PON_SHELL_SRAM_CSR_SRAM_EXT_LD_DONE,
				      0, PON_SHELL_SRAM_CSR);

		ret = pon_mbox_serdes_fw_download(pon);
	}

	pon_mbox_pon_app_mask(pon, 0, PON_SHELL_SRAM_CSR_SRAM_EXT_LD_DONE,
			      PON_SHELL_SRAM_CSR);

#if 0 /* Currently not used */
	ret = urx800_serdes_rxtx_stable_check(pon);
	if (ret)
		return ret;
#endif
	clr = PON_SHELL_GEN_CTRL_TX_RESET | PON_SHELL_GEN_CTRL_RX_RESET;
	pon_mbox_pon_app_mask(pon, clr, 0, PON_SHELL_GEN_CTRL);

	/* Override RES_ACK workaround for step A.
	 */
	if (pon->hw_ver == PON_MBOX_HW_VER_URX_A_TYPE) {
		ret = urx800_serdes_cal_ovrd_workaround_step_a(pon);
		if (ret)
			return ret;

		/* wait for tx and rx acknowledge to go low */
		ret = readl_poll_timeout(
			pon->pon_app + PON_SHELL_GEN_CTRL, val,
			(val & PON_SHELL_GEN_CTRL_RX_ACK) !=
					PON_SHELL_GEN_CTRL_RX_ACK &&
				(val & PON_SHELL_GEN_CTRL_TX_ACK) !=
					PON_SHELL_GEN_CTRL_TX_ACK,
			10, 50000);
		if (ret) {
			val = pon_mbox_pon_app_read(pon, PON_SHELL_GEN_CTRL);
			dev_err(pon->dev, "Missing tx rx ack: %i (0x%x)\n", ret,
				val);
			return ret;
		}
	}

	/* Disable drift detector */
	pon_mbox_serdes_mask(pon, PON_PHY_LANE0_DIG_AON_RX_CDR_DETECTOR_CTL_EN,
			     0, PON_PHY_LANE0_DIG_AON_RX_CDR_DETECTOR_CTL);

	dev_dbg(pon->dev, "SerDes initialization finished successfully\n");

	return 0;
}

int prx300_serdes_basic_init(struct pon_mbox *pon)
{
	u32 set, clr;
	int ret;

	dev_dbg(pon->dev, "executing basic init for SerDes\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->pon_apb_app) {
		dev_err(pon->dev, "pon apb not available\n");
		return -ENODEV;
	}

	if (!pon->cgu) {
		dev_err(pon->dev, "cgu-syscon not available\n");
		return -ENODEV;
	}

	if (!pon->chiptop) {
		dev_err(pon->dev, "chiptop-syscon not available\n");
		return -ENODEV;
	}

	pon_mbox_pon_apb_app_write(pon, WAN_APB_TIMEOUT_CTRL_VAL,
				   WAN_APB_TIMEOUT_CTRL);

	/* TODO: Might not be required on FLM */
	pon_mbox_pon_app_mask(pon, WAN_PHY_SRAM_CSR_BYPASS, 0,
			      WAN_PHY_SRAM_CSR);

	clr = WAN_PHY_GEN_CTRL_REF_USE_PAD | WAN_PHY_GEN_CTRL_REF_CLK_EN |
	      WAN_PHY_GEN_CTRL_RX_DATA_EN | WAN_PHY_GEN_CTRL_CR_PARA_CLK_GATE;

	set = WAN_PHY_GEN_CTRL_TX_RESET | WAN_PHY_GEN_CTRL_RX_RESET |
	      WAN_PHY_GEN_CTRL_CR_PARA_CLK_DIV2_EN;

	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_GEN_CTRL);

	ret = regmap_update_bits(pon->chiptop, CHIPTOP_IFMUX_CFG,
				 CHIPTOP_IFMUX_CFG_WAN_MUX, 0);
	if (ret)
		return ret;

	ret = regmap_update_bits(pon->cgu, PRX_CGU_CLKGCR1_A,
				 PRX_CGU_CLKGCR1_A_PON, 1);
	if (ret)
		return ret;

	return regmap_update_bits(pon->cgu, PRX_CGU_CLKGCR1_A,
				  PRX_CGU_CLKGCR1_A_AON, 0);
}

int prx300_ref_clk_sel(struct pon_mbox *pon)
{
	u32 clr;
	int ret;

	dev_dbg(pon->dev, "executing reference clock selection\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->cgu) {
		dev_err(pon->dev, "cgu-syscon not available\n");
		return -ENODEV;
	}

	ret = regmap_update_bits(pon->cgu, PRX_CGU_IF_CLK,
				 PRX_CGU_IF_CLK_PLL5_PLL3_SEL,
				 PRX_CGU_IF_CLK_PLL5_PLL3_SEL);
	if (ret)
		return ret;

	ret = regmap_update_bits(pon->cgu, PRX_CGU_IF_CLK,
				 PRX_CGU_IF_CLK_P2_SEL, 0);
	if (ret)
		return ret;

	clr = PRX_CGU_IF_CLK_SW_REFCLK_EN | PRX_CGU_IF_CLK_SW_HW_REFCLK_EN_SEL;
	return regmap_update_bits(pon->cgu, PRX_CGU_IF_CLK, clr, 0);
}

int prx300_pll5_init(struct pon_mbox *pon)
{
	u32 msk, val;
	int ret;

	dev_dbg(pon->dev, "executing PLL5 init with loop_ps_en = %d\n",
		pon->lt_cfg.loop_ps_en);

	if (!pon->cgu) {
		dev_err(pon->dev, "cgu-syscon not available\n");
		return -ENODEV;
	}

	if (!pon->lt_cfg.valid) {
		dev_err(pon->dev, "loop timing config not valid\n");
		return -EPERM;
	}

	msk = PRX_CGU_PLL5_CFG2_REFDIV_MASK | PRX_CGU_PLL5_CFG2_FREFCMLEN |
	      PRX_CGU_PLL5_CFG2_FBDIV_MASK;

	val = (1 << PRX_CGU_PLL5_CFG2_REFDIV_SHIFT) |
	      PRX_CGU_PLL5_CFG2_FREFCMLEN;

	/* FBDIV */
	/* No implementation for PRBS generation - to be done in FW */
	switch (pon->mode) {
	case PON_MODE_984_GPON:
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		if (!pon->lt_cfg.loop_ps_en)
			val |= 248;
		else
			val |= 124;
		break;
	default:
		return -EINVAL;
	}

	ret = regmap_update_bits(pon->cgu, PRX_CGU_PLL5_CFG2, msk, val);
	if (ret)
		return ret;

	msk = PRX_CGU_PLL5_CFG0_FRAC_MASK | PRX_CGU_PLL5_CFG0_DSMEN |
	      PRX_CGU_PLL5_CFG0_DACEN;

	switch (pon->mode) {
	case PON_MODE_984_GPON:
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		if (!pon->lt_cfg.loop_ps_en)
			val = (0xD4FDF3 << PRX_CGU_PLL5_CFG0_FRAC_SHIFT) |
			      PRX_CGU_PLL5_CFG0_DSMEN | PRX_CGU_PLL5_CFG0_DACEN;
		else
			val = (0x6A7EFA << PRX_CGU_PLL5_CFG0_FRAC_SHIFT) |
			      PRX_CGU_PLL5_CFG0_DSMEN | PRX_CGU_PLL5_CFG0_DACEN;
		ret = regmap_update_bits(pon->cgu, PRX_CGU_PLL5_CFG0, msk, val);
		if (ret)
			return ret;

		/* Clear LD bit at first to ensure that subsequent command
		 * triggers a load action. Required on PRX126.
		 */
		ret = regmap_update_bits(pon->cgu, PRX_CGU_PLL5_CFG0,
					 PRX_CGU_PLL5_CFG0_LD, 0);
		if (ret)
			return ret;

		ret = regmap_update_bits(pon->cgu, PRX_CGU_PLL5_CFG0,
					 PRX_CGU_PLL5_CFG0_LD,
					 PRX_CGU_PLL5_CFG0_LD);
		if (ret)
			return ret;
		ret = regmap_update_bits(pon->cgu, PRX_CGU_PLL5_CFG0,
					 PRX_CGU_PLL5_CFG0_PLLEN,
					 PRX_CGU_PLL5_CFG0_PLLEN);
		if (ret)
			return ret;
		/* check for LD self clearing */
		ret = regmap_read_poll_timeout(
			pon->cgu, PRX_CGU_PLL5_CFG0, val,
			(val & PRX_CGU_PLL5_CFG0_LD) == 0, 10, 1000);
		if (!ret) {
			dev_dbg(pon->dev, "LD self clearing done(0x%x)\n", val);
		} else {
			dev_err(pon->dev,
				"LD self clearing timeout %i (0x%x)\n", ret,
				val);
			return ret;
		}
		break;
	default:
		return -EINVAL;
	}

	/* wait for PLL5 lock */
	ret = regmap_read_poll_timeout(pon->cgu, PRX_CGU_PLL5_CFG0, val,
				       val & PRX_CGU_PLL5_CFG0_LOCK, 10, 50000);
	if (!ret) {
		dev_dbg(pon->dev, "pll5 locked successfully (0x%x)\n", val);
	} else {
		dev_err(pon->dev, "pll5 not locked: %i (0x%x)\n", ret, val);
		return ret;
	}

	/* POST_DIV0A */
	val = 7 | PRX_CGU_PLL5_CFG1_FOUT0EN | PRX_CGU_PLL5_CFG1_FOUT1EN;
	if (!pon->lt_cfg.loop_ps_en)
		val |= 1 << PRX_CGU_PLL5_CFG1_POST_DIV0B_SHIFT |
		       3 << PRX_CGU_PLL5_CFG1_POST_DIV1A_SHIFT;
	else
		val |= 0 << PRX_CGU_PLL5_CFG1_POST_DIV0B_SHIFT |
		       1 << PRX_CGU_PLL5_CFG1_POST_DIV1A_SHIFT;

	msk = PRX_CGU_PLL5_CFG1_FOUT0EN | PRX_CGU_PLL5_CFG1_FOUT1EN |
	      PRX_CGU_PLL5_CFG1_POST_DIV0PRE |
	      PRX_CGU_PLL5_CFG1_POST_DIV0A_MASK |
	      PRX_CGU_PLL5_CFG1_POST_DIV0B_MASK |
	      PRX_CGU_PLL5_CFG1_POST_DIV1A_MASK |
	      PRX_CGU_PLL5_CFG1_POST_DIV1B_MASK;

	ret = regmap_update_bits(pon->cgu, PRX_CGU_PLL5_CFG1, msk, val);
	if (ret)
		return ret;

	ret = regmap_update_bits(pon->cgu, PRX_CGU_IF_CLK,
				 PRX_CGU_IF_CLK_SW_REFCLK_EN,
				 PRX_CGU_IF_CLK_SW_REFCLK_EN);
	if (ret)
		return ret;

	return 0;
}

/**
 * Function to check for reference clk bit level
 */
static int prx300_ref_clk_wait(struct pon_mbox *pon, u8 lvl)
{
	int ret;
	u32 val;

	if (lvl)
		/* wait for ref clock req to go high*/
		ret = readl_poll_timeout(pon->pon_app + WAN_PHY_GEN_CTRL, val,
					 val & WAN_PHY_GEN_CTRL_REF_CLK_REQ, 10,
					 50000);
	else
		/* wait for ref clock req to go low */
		ret = readl_poll_timeout(pon->pon_app + WAN_PHY_GEN_CTRL, val,
					 (val & WAN_PHY_GEN_CTRL_REF_CLK_REQ) !=
						 WAN_PHY_GEN_CTRL_REF_CLK_REQ,
					 10, 50000);
	if (ret) {
		val = pon_mbox_pon_app_read(pon, WAN_PHY_GEN_CTRL);
		dev_err(pon->dev, "Ref clock failure: %i (0x%x)\n", ret, val);
		return ret;
	}

	return 0;
}

/** Serdes mode specific settings for PRX family. */
static const struct pon_mbox_serdes_init pon_mbox_serdes_prx300_init_val[6] = {
	/** Dummy for unknown mode */
	{ .ref_range = 0,
	  .mplla_bandwidth = 0,
	  .mplla_multiplier = 0,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0,
	  .rx_ref_ld_val = 0,
	  .tx_rate = 0,
	  .rx_rate = 0 },
	/** ITU-T G.984 (GPON). */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0x6E,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 3,
	  .rx_rate = 2 },
	/** ITU - T G.987 (XG - PON) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0x56,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 2,
	  .rx_rate = 0 },
	/** ITU-T G.9807 (XGS-PON) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0x7B,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 0,
	  .rx_rate = 0 },
	/** ITU-T G.989 (NG-PON2 2.5G upstream) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0x56,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 2,
	  .rx_rate = 0 },
	/** ITU-T G.989 (NG-PON2 10G upstream) */
	{ .ref_range = 5,
	  .mplla_bandwidth = 0x7B,
	  .mplla_multiplier = 32,
	  .mplla_div16p5_clk_en = false,
	  .rx_vco_ld_val = 0x540,
	  .rx_ref_ld_val = 0x2A,
	  .tx_rate = 0,
	  .rx_rate = 0 },
};

int prx300_serdes_init(struct pon_mbox *pon)
{
	u32 set, clr, val;
	int ret;
	enum pon_mode use_mode = pon->mode;

	dev_dbg(pon->dev, "executing SerDes init\n");

	if (!pon->pon_app) {
		dev_err(pon->dev, "pon app not available\n");
		return -ENODEV;
	}

	if (!pon->serdes) {
		dev_err(pon->dev, "SerDes not available\n");
		return -ENODEV;
	}

	if (!pon->serdes_cfg.valid) {
		dev_err(pon->dev, "no valid SerDes config\n");
		return -EINVAL;
	}

	if (!pon->iop_cfg.valid) {
		dev_err(pon->dev, "no valid IOP config\n");
		return -EINVAL;
	}

	if (pon_mbox_pon_mode_check(pon))
		return -EINVAL;

	if (pon->mode == PON_MODE_987_XGPON && pon->iop_cfg.msk.iop5) {
		use_mode = PON_MODE_9807_XGSPON;
		dev_dbg(pon->dev, "IOP bit 5 -> XGSPON settings used\n");
	}

	clr = WAN_PHY_GEN_CTRL_RX_CDR_SSC_EN | WAN_PHY_GEN_CTRL_RX_TERM_ACDC |
	      WAN_PHY_GEN_CTRL_RX_RATE_MASK | WAN_PHY_GEN_CTRL_TX_RATE_MASK;

	set = WAN_PHY_GEN_CTRL_RX_TERM_ACDC |
	      (pon_mbox_serdes_prx300_init_val[use_mode].tx_rate
	       << WAN_PHY_GEN_CTRL_TX_RATE_SHIFT) |
	      (pon_mbox_serdes_prx300_init_val[use_mode].rx_rate
	       << WAN_PHY_GEN_CTRL_RX_RATE_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_GEN_CTRL);

	clr = WAN_PHY_REF_CLK_CTRL_MPLLA_DIV2_EN |
	      WAN_PHY_REF_CLK_CTRL_DIV2_EN |
	      WAN_PHY_REF_CLK_CTRL_REF_RANGE_MASK;
	set = WAN_PHY_REF_CLK_CTRL_MPLLA_DIV2_EN |
	      (pon_mbox_serdes_prx300_init_val[use_mode].ref_range
	       << WAN_PHY_REF_CLK_CTRL_REF_RANGE_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_REF_CLK_CTRL);

	clr = WAN_PHY_EXT_MPLLA_CTRL1_BANDWIDTH_MASK |
	      WAN_PHY_EXT_MPLLA_CTRL1_DIV10_CLK_EN |
	      WAN_PHY_EXT_MPLLA_CTRL1_DIV16P5_CLK_EN |
	      WAN_PHY_EXT_MPLLA_CTRL1_DIV8_CLK_EN |
	      WAN_PHY_EXT_MPLLA_CTRL1_DIV_MULTIPLIER_MASK |
	      WAN_PHY_EXT_MPLLA_CTRL1_MULTIPLIER_MASK;
	set = WAN_PHY_EXT_MPLLA_CTRL1_DIV8_CLK_EN |
	      (pon_mbox_serdes_prx300_init_val[use_mode].mplla_multiplier
	       << WAN_PHY_EXT_MPLLA_CTRL1_MULTIPLIER_SHIFT) |
	      pon_mbox_serdes_prx300_init_val[use_mode].mplla_bandwidth;

	if (pon_mbox_serdes_prx300_init_val[use_mode].mplla_div16p5_clk_en)
		set |= WAN_PHY_EXT_MPLLA_CTRL1_DIV16P5_CLK_EN;
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_EXT_MPLLA_CTRL1);

	clr = WAN_PHY_EXT_CALI_CTRL1_RX_VCO_LD_VAL_MASK |
	      WAN_PHY_EXT_CALI_CTRL1_RX_REF_LD_VAL_MASK;
	set = pon_mbox_serdes_prx300_init_val[use_mode].rx_vco_ld_val |
	      (pon_mbox_serdes_prx300_init_val[use_mode].rx_ref_ld_val
	       << WAN_PHY_EXT_CALI_CTRL1_RX_REF_LD_VAL_SHIFT) |
	      WAN_PHY_EXT_CALI_CTRL1_MPLLB_INIT_CAL_DISABLE;
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_EXT_CALI_CTRL1);

	clr = WAN_PHY_EXT_MISC_CTRL0_TX_IBOOST_LVL_MASK |
	      WAN_PHY_EXT_MISC_CTRL0_RX_VREF_CTRL_MASK |
	      WAN_PHY_EXT_MISC_CTRL0_TX_VBOOST_LVL_MASK |
	      WAN_PHY_EXT_MISC_CTRL0_REF_REPEAT_CLK_EN;
	set = (0xF << WAN_PHY_EXT_MISC_CTRL0_RX_VREF_CTRL_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_VBOOST_LVL]
	       << WAN_PHY_EXT_MISC_CTRL0_TX_VBOOST_LVL_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_IBOOST_LVL]
	       << WAN_PHY_EXT_MISC_CTRL0_TX_IBOOST_LVL_SHIFT);
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_EXT_MISC_CTRL0);

	clr = WAN_PHY_EXT_MISC_CTRL1_TX_WIDTH_MASK |
	      WAN_PHY_EXT_MISC_CTRL1_TX_MPLL_EN |
	      WAN_PHY_EXT_MISC_CTRL1_TX_MPLLB_SEL |
	      WAN_PHY_EXT_MISC_CTRL1_TX_CLK_RDY |
	      WAN_PHY_EXT_MISC_CTRL1_TX_EQ_PRE_MASK |
	      WAN_PHY_EXT_MISC_CTRL1_TX_EQ_MAIN_MASK |
	      WAN_PHY_EXT_MISC_CTRL1_TX_EQ_POST_MASK;
	set = (2 << WAN_PHY_EXT_MISC_CTRL1_TX_WIDTH_SHIFT) |
	      WAN_PHY_EXT_MISC_CTRL1_TX_MPLL_EN |
	      WAN_PHY_EXT_MISC_CTRL1_TX_CLK_RDY |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_TX_EQ_PRE]
	       << WAN_PHY_EXT_MISC_CTRL1_TX_EQ_PRE_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_TX_EQ_MAIN]
	       << WAN_PHY_EXT_MISC_CTRL1_TX_EQ_MAIN_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_TX_EQ_POST]
	       << WAN_PHY_EXT_MISC_CTRL1_TX_EQ_POST_SHIFT);
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_VBOOST_EN])
		set |= WAN_PHY_EXT_MISC_CTRL1_TX_VBOOST_EN;
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_EXT_MISC_CTRL1);

	clr = WAN_PHY_EXT_MISC_CTRL2_RX_WIDTH_MASK |
	      WAN_PHY_EXT_MISC_CTRL2_RX_ADAPT_CONT;
	set = (2 << WAN_PHY_EXT_MISC_CTRL2_RX_WIDTH_SHIFT) |
	      WAN_PHY_EXT_MISC_CTRL2_RX_TERM_EN |
	      WAN_PHY_EXT_MISC_CTRL2_RX_DIV16P5_CLK_EN |
	      WAN_PHY_EXT_MISC_CTRL2_RX_OFFCAN_COUNT |
	      WAN_PHY_EXT_MISC_CTRL2_RX_CDR_TRACK_EN |
	      WAN_PHY_EXT_MISC_CTRL2_RX_CDR_VCO_LOWFREQ;
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_ADAPT_CONT])
		set |= WAN_PHY_EXT_MISC_CTRL2_RX_ADAPT_CONT;
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_EXT_MISC_CTRL2);

	clr = WAN_PHY_EXT_EQ_CTRL1_RX_ADAPT_AFE_EN |
	      WAN_PHY_EXT_EQ_CTRL1_RX_ADAPT_DFE_EN |
	      WAN_PHY_EXT_EQ_CTRL1_RX_EQ_ATT_LVL_MASK |
	      WAN_PHY_EXT_EQ_CTRL1_RX_EQ_CTLE_BOOST_MASK |
	      WAN_PHY_EXT_EQ_CTRL1_RX_EQ_CTLE_POLE_MASK |
	      WAN_PHY_EXT_EQ_CTRL1_RX_EQ_DFE_TAP1_MASK |
	      WAN_PHY_EXT_EQ_CTRL1_RX_EQ_VGA1_GAIN_MASK |
	      WAN_PHY_EXT_EQ_CTRL1_RX_EQ_VGA2_GAIN_MASK;
	set = (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_EQ_ATT_LVL]
	       << WAN_PHY_EXT_EQ_CTRL1_RX_EQ_ATT_LVL_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_EQ_CTLE_BOOST]
	       << WAN_PHY_EXT_EQ_CTRL1_RX_EQ_CTLE_BOOST_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_CTLE_POLE]
	       << WAN_PHY_EXT_EQ_CTRL1_RX_EQ_CTLE_POLE_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_DFE_TAP1]
	       << WAN_PHY_EXT_EQ_CTRL1_RX_EQ_DFE_TAP1_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_VGA1_GAIN]
	       << WAN_PHY_EXT_EQ_CTRL1_RX_EQ_VGA1_GAIN_SHIFT) |
	      (pon->serdes_cfg.param[PON_MBOX_SRDS_EQ_VGA2_GAIN]
	       << WAN_PHY_EXT_EQ_CTRL1_RX_EQ_VGA2_GAIN_SHIFT);
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_ADAPT_AFE_EN])
		set |= WAN_PHY_EXT_EQ_CTRL1_RX_ADAPT_AFE_EN;
	if (pon->serdes_cfg.param[PON_MBOX_SRDS_RX_ADAPT_DFE_EN])
		set |= WAN_PHY_EXT_EQ_CTRL1_RX_ADAPT_DFE_EN;
	pon_mbox_pon_app_mask(pon, clr, set, WAN_PHY_EXT_EQ_CTRL1);

	pon_mbox_pon_app_mask(pon, 0, WAN_PHY_GEN_CTRL_REF_CLK_EN,
			      WAN_PHY_GEN_CTRL);

	ret = prx300_ref_clk_wait(pon, 1);
	if (ret)
		return ret;

	ret = reset_control_deassert(pon->reset_serdes);
	if (ret) {
		dev_err(pon->dev, "SerDes reset deassert failed: %i", ret);
		return ret;
	}

	/* TODO: Might not be required on FLM */
	pon_mbox_pon_app_mask(pon, WAN_PHY_SRAM_CSR_BYPASS, 0,
			      WAN_PHY_SRAM_CSR);

	/* wait for SRAM init done */
	ret = readl_poll_timeout(pon->pon_app + WAN_PHY_SRAM_CSR, val,
				 val & WAN_PHY_SRAM_CSR_INIT_DONE, 10, 50000);
	if (!ret) {
		dev_dbg(pon->dev, "SRAM init successfully (0x%x)\n", val);
	} else {
		val = pon_mbox_pon_app_read(pon, WAN_PHY_SRAM_CSR);
		dev_err(pon->dev, "SRAM init failed: %i (0x%x)\n", ret, val);
		return ret;
	}

	/* check for SerDes FW to be downloaded*/
	if (pon->serdes_fw_dl) {
		pon_mbox_pon_app_mask(pon, WAN_PHY_SRAM_CSR_EXT_LD_DONE, 0,
				      WAN_PHY_SRAM_CSR);

		ret = pon_mbox_serdes_fw_download(pon);
	}

	pon_mbox_pon_app_mask(pon, 0, WAN_PHY_SRAM_CSR_EXT_LD_DONE,
			      WAN_PHY_SRAM_CSR);

	pon_mbox_serdes_mask(pon, 0,
			     SRDS_RAWLANE0_DIG_PCS_XF_RX_OVRD_IN_3_CONT_OVRD_EN,
			     SRDS_RAWLANE0_DIG_PCS_XF_RX_OVRD_IN_3);

	clr = WAN_PHY_GEN_CTRL_TX_RESET | WAN_PHY_GEN_CTRL_RX_RESET;
	pon_mbox_pon_app_mask(pon, clr, 0, WAN_PHY_GEN_CTRL);

	/* wait for tx and rx acknowledge to go low */
	ret = readl_poll_timeout(pon->pon_app + WAN_PHY_GEN_CTRL, val,
				 (val & WAN_PHY_GEN_CTRL_RX_ACK) !=
						 WAN_PHY_GEN_CTRL_RX_ACK &&
					 (val & WAN_PHY_GEN_CTRL_TX_ACK) !=
						 WAN_PHY_GEN_CTRL_TX_ACK,
				 10, 50000);
	if (ret) {
		val = pon_mbox_pon_app_read(pon, WAN_PHY_GEN_CTRL);
		dev_err(pon->dev, "Missing tx rx ack: %i (0x%x)\n", ret, val);
		return ret;
	}

	/*
	 * Put the WAN XPCS into reset now, it is not used when PON is active.
	 * We can only do this after the PON IP was started otherwise we will
	 * only get into O23. When we get the FW download IRQ from the PON BIOS
	 * we should be able to put the WAN XPCS into reset.
	 */
	if (pon->reset_xpcs && !pon->reset_xpcs_done) {
		ret = reset_control_assert(pon->reset_xpcs);
		if (ret) {
			dev_err(pon->dev, "WAN XPCS reset assert failed: %i",
				ret);
			return ret;
		}
		pon->reset_xpcs_done = true;
	}

	dev_dbg(pon->dev, "SerDes initialization finished successfully\n");

	return 0;
}

MODULE_FIRMWARE(SRDS_FW_URX800_A);
MODULE_FIRMWARE(SRDS_FW_URX800_B);

/** @} */
