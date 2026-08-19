/* SPDX-License-Identifier: GPL-2.0 */
/*
 * P34X FW Download Driver
 *
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2019-2020, Intel Corporation.
 * Govindaiah Mudepalli <gmudepalli@maxlinear.com>
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

#ifndef _P34X_PHY_FWDL_H_
#define _P34X_PHY_FWDL_H_

#include <linux/list.h>
#include <linux/phy.h>
#include <linux/mdio.h>
#include <linux/ptp_clock_kernel.h>

#define SHORT_TRACE		1

#define TEST_USXGMII_LB		0
#define TEST_XMDIO_MMD		0
#define TEST_INTEGRATED_FW	0
#define TEST_DLFW_READ		0
#define DEBUG_SGMII		0

#define DRIVER_NAME		"p34x_lgm"
#define FW_PATHNAME		"fw"
#define P34X_COMPATIBLE		"intel,p34x"

#define MAX_RESET		4

#define NUM_OF_SLICES		4

#define AHB_TO_PDI(arc_addr)	(((arc_addr) / 4) & 0xFFFFu)

#define AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0	0x00A49408 /* PDI: 0x2502 */
#define AHB_SLICE_CTRL_PDI_USXGMII		0x00A4CF5C /* PDI: 0x33D7 */
#define AHB_TOP_SLICE_IF			0x00D28808 /* PDI: 0xA202 */
#define AHB_TOP_FCR				0x00D2880C /* PDI: 0xA203 */
#define AHB_TOP_INDICATOR			0x00D28814 /* PDI: 0xA205 */
#define AHB_CHIPID_MPS_CHIPID_M			0x00D28888 /* PDI: 0xA222 */
#define AHB_CHIP_ID_CFG				0x00D288C0 /* PDI: 0xA230 */
#define AHB_LJPLL_CFG0L				0x00D2900C /* PDI: 0xA403 */
#define AHB_CGU_PDI_CLK_EN			0x00D29038 /* PDI: 0xA40E */
#define AHB_RCU_PDI_REGISTERS_RST_REQ		0x00D29404 /* PDI: 0xA501 */
#define AHB_SMDIO_PDI_SMDIO_REGISTERS_SMDIO_CFG	0x00D3D200 /* PDI: 0xF480 */
#define AHB_FABRIC_PDI_CTRL			0x00D3DB40 /* PDI: 0xF6D0 */
#define AHB_FABRIC_PDI_ADDRL			0x00D3DB44 /* PDI: 0xF6D1 */
#define AHB_FABRIC_PDI_ADDRH			0x00D3DB48 /* PDI: 0xF6D2 */
#define AHB_FABRIC_PDI_DATAL			0x00D3DB4C /* PDI: 0xF6D3 */
#define AHB_FABRIC_PDI_DATAH			0x00D3DB50 /* PDI: 0xF6D4 */

#define VSPEC1_MBOX_DATA	0x5
#define VSPEC1_MBOX_ADDRLO	0x6
#define VSPEC1_MBOX_CMD		0x7
#define VSPEC1_MBOX_DATA_HIGH	0xD
#define VSPEC1_MBOX_CMD_FAIL_MASK 0x4000u
#define VSPEC1_MBOX_CMD_READY_MASK 0x8000u
#define VSPEC1_MBOX_DATA_HIGH_FWDL_MASK 0xA5A5
#define VSPEC1_PATCH_LOAD_FAST_SRAM	0x900
#define VSPEC1_PATCH_LOAD_FAST_FLASH 0x800

#define MBOX_MAX_BUSY_RETRY	1000
#define RETURN_FROM_FUNC	1
#define CHIP_ID_CFG_LEGACY_MCM	0xFFFF
#define CHIP_ID_CFG_GPY241	0xF800
#define CHIP_ID_CFG_MXL86249	0x8001
#define CHIP_ID_CFG_MCM		0x401
#define CHIP_ID_CFG_MXL86249C	0x8401

#define USXGMII_PDI_BASEADRR	0x00D3DFFC
#define USXGMII_PDI_INDIRECT	0x00D3DC00

#define SMDIO_BADR              0x1F
#define SMDIO_TOFF              0x00

#define MDIO_CTL_RST	0x8000
#define MDIO_FW_VERSION	0x1E
#define VPSPEC2_CTL	0x0F01
#define VPSPEC2_MASK	0xFFFE

/* GPHY_USXGMII TIMEOUT */
#define SRAM_INIT_TIMEOUT	1000
#define CGU_PLLLOCK_TIMEOUT	1000
#define VR_RESET_TIMEOUT	1000
#define XPCS5_POWER_TIMEOUT	1000
#define CR_CTRL_TIMEOUT		1000

enum p34x_step {
	P34X_STEP_A = 0,
	P34X_STEP_B,
	P34X_MAX_STEP_COUNT
};

enum p34x_fw_type {
	P34X_FW_UNSIGNED = 0,
	P34X_FW_SIGNED
};

struct p34x_priv_data;

struct slice_ptp {
	struct p34x_priv_data *parent;
	u32 idx;

	spinlock_t lock;
	struct ptp_clock *clock;
	struct ptp_clock_info info;
	struct work_struct tx_work;
	struct sk_buff *pskb_tx;

	/* PTP config data */
	u32 def_addend;
};

#if (IS_ENABLED(CONFIG_HWMON))
#define P34X_TEMP_TREND_DS_SIZE 20
#define P34X_TEMP_TREND_TEMP_ROUNDUP 1

struct p34x_temp_trend_data {
	struct list_head dataset;
	int ds_size;
	int ds_cnt;
	int tround;
	spinlock_t lock; /* to lock access to ds_size/tround */
};

struct p34x_temp_trend_ds_entry {
	struct list_head list;
	long temp;
};
#endif

struct p34x_ops {
	int (*phy_reset)(struct platform_device *pdev);
};

struct p34x_priv_data {
	struct platform_device *pdev;
	struct mii_bus *miibus;
	struct pinctrl *pinctrl;
	struct pinctrl_state *gpio_state;
	struct pinctrl_state *mdio_state;
	struct clk *clk;
	struct reset_control *rst[MAX_RESET];
	struct gpio_desc     *reset_gpio;
#if IS_ENABLED(CONFIG_HWMON)
	struct device *hwmon;
	struct p34x_temp_trend_data t_trend;
#endif
	struct slice_ptp ptp[NUM_OF_SLICES];
	u32 smdio_addr;
	u32 phy_addr;
	u32 rate_adaptation;
	unsigned int irqnum;
	const char *fw_pathname[P34X_MAX_STEP_COUNT];
	struct p34x_ops ops;
};

static inline struct p34x_ops *p34x_get_ops(struct platform_device *pdev)
{
	struct p34x_priv_data *pdata = platform_get_drvdata(pdev);

	return &pdata->ops;
}

static inline int smdio_read(struct p34x_priv_data *pdata, u32 regaddr)
{
	int ret;

	ret = mdiobus_write(pdata->miibus, pdata->smdio_addr, SMDIO_BADR,
			    regaddr);

	if (ret < 0)
		return ret;

	return mdiobus_read(pdata->miibus, pdata->smdio_addr, SMDIO_TOFF);
}

static inline int smdio_write(struct p34x_priv_data *pdata,
			      u32 regaddr, u16 data)
{
	int ret;

	ret = mdiobus_write(pdata->miibus, pdata->smdio_addr, SMDIO_BADR,
			    regaddr);

	if (ret)
		return ret;

	return mdiobus_write(pdata->miibus, pdata->smdio_addr, SMDIO_TOFF,
			     data);
}

static inline int mdio_read(struct p34x_priv_data *pdata, u32 idx, u32 regaddr)
{
	return mdiobus_read(pdata->miibus, pdata->phy_addr + idx, regaddr);
}

static inline int mdio_write(struct p34x_priv_data *pdata, u32 idx, u32 regaddr,
			     u16 data)
{
	return mdiobus_write(pdata->miibus, pdata->phy_addr + idx, regaddr,
			     data);
}

static inline int mmd_read(struct p34x_priv_data *pdata, u32 idx, u32 devtype,
			   u32 regaddr)
{
	regaddr = MII_ADDR_C45 | ((devtype & 0x1f) << 16) | (regaddr & 0xffff);
	return mdiobus_read(pdata->miibus, pdata->phy_addr + idx, regaddr);
}

static inline int mmd_write(struct p34x_priv_data *pdata, u32 idx, int devtype,
			    u32 regaddr, u16 data)
{
	regaddr = MII_ADDR_C45 | ((devtype & 0x1f) << 16) | (regaddr & 0xffff);
	return mdiobus_write(pdata->miibus, pdata->phy_addr + idx, regaddr,
			     data);
}

static inline int mbox_ready(struct p34x_priv_data *pdata, u32 idx)
{
	int timeout = 3000;
	int ret;

	do {
		ret = mmd_read(pdata, idx, MDIO_MMD_VEND1, 7);
		if (ret < 0)
			return ret;
		if (ret & BIT(15))
			return 0;
	} while (timeout-- > 0);
	return -EBUSY;
}

static inline int mbox16_read(struct p34x_priv_data *pdata, u32 idx,
			      u32 regaddr)
{
	int ret;
	u32 cmd;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		return ret;

	cmd = (regaddr >> 16) & 0xFF;
	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 7, cmd);
	if (ret < 0)
		return ret;

	ret = mbox_ready(pdata, idx);
	if (ret)
		return ret;

	return mmd_read(pdata, idx, MDIO_MMD_VEND1, 5);
}

static inline int mbox16_write(struct p34x_priv_data *pdata, u32 idx,
			       u32 regaddr, u32 data)
{
	int ret;
	u32 cmd;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 5, data);
	if (ret < 0)
		return ret;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		return ret;

	cmd = ((regaddr >> 16) & 0xFF) | (1 << 8);
	return mmd_write(pdata, idx, MDIO_MMD_VEND1, 7, cmd);
}

static inline u32 mbox32_read(struct p34x_priv_data *pdata, u32 idx,
			      u32 regaddr)
{
	int ret;
	u32 cmd;
	u32 data;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		goto ERROR;

	cmd = ((regaddr >> 16) & 0xFF) | (4 << 8);
	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 7, cmd);
	if (ret < 0)
		goto ERROR;

	ret = mbox_ready(pdata, idx);
	if (ret)
		goto ERROR;

	ret = mmd_read(pdata, idx, MDIO_MMD_VEND1, 13);
	if (ret < 0)
		goto ERROR;
	data = ret << 16;

	ret = mmd_read(pdata, idx, MDIO_MMD_VEND1, 5);
	if (ret < 0)
		goto ERROR;
	data |= ret & 0xFFFF;

	return data;

ERROR:
	pr_err("Critical error happened during mailbox 32-bit read.\n");
	return 0;
}

static inline int mbox32_write(struct p34x_priv_data *pdata, u32 idx,
			       u32 regaddr, u32 data)
{
	int ret;
	u32 cmd;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 5, data & 0xFFFF);
	if (ret < 0)
		return ret;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 13, data >> 16);
	if (ret < 0)
		return ret;

	ret = mmd_write(pdata, idx, MDIO_MMD_VEND1, 6, regaddr & 0xFFFF);
	if (ret < 0)
		return ret;

	cmd = ((regaddr >> 16) & 0xFF) | (5 << 8);
	return mmd_write(pdata, idx, MDIO_MMD_VEND1, 7, cmd);
}

static inline u32 pm_read(struct p34x_priv_data *pdata, u32 idx,
			  u32 regaddr)
{
	int ret;
	u32 data;

	ret = smdio_write(pdata, 0xA202, idx);
	if (ret < 0)
		goto ERROR;

	ret = smdio_write(pdata, 0xE070, regaddr & 0xFFFF);
	if (ret < 0)
		goto ERROR;

	ret = smdio_write(pdata, 0xE071, regaddr >> 16);
	if (ret < 0)
		goto ERROR;

	ret = smdio_write(pdata, 0xE06F, 1);
	if (ret < 0)
		goto ERROR;

	ret = smdio_read(pdata, 0xE072);
	if (ret < 0)
		goto ERROR;
	data = ret & 0xFFFF;

	ret = smdio_read(pdata, 0xE073);
	if (ret < 0)
		goto ERROR;
	data |= ret << 16;

	return data;

ERROR:
	pr_err("Critical error happened during Packet Manager (PM) 32-bit read.\n");
	return 0;
}

extern int p34x_xpcs_rd(struct p34x_priv_data *pdata, u32 off);
extern int p34x_xpcs_wr(struct p34x_priv_data *pdata, u32 off, u32 data);

extern int p34x_ptp_init(struct p34x_priv_data *priv);
extern void p34x_ptp_uninit(struct p34x_priv_data *priv);

extern int p34x_sysfs_init(struct p34x_priv_data *priv);
extern int g241_sysfs_init(struct p34x_priv_data *priv);
#endif /* _P34X_PHY_FWDL_H_ */
