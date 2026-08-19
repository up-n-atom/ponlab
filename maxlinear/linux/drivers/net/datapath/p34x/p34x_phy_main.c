// SPDX-License-Identifier: GPL-2.0
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
#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/bitfield.h>
#include <linux/of.h>
#include <linux/reset.h>
#include <linux/clk.h>
#include <linux/gpio/consumer.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/of_mdio.h>
#include <linux/of_address.h>
#include <linux/pinctrl/consumer.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#if (IS_ENABLED(CONFIG_HWMON))
#include <linux/hwmon.h>
#include <linux/thermal.h>
#endif
#include <net/sock.h>

#include "p34x_phy.h"
#if defined(TEST_INTEGRATED_FW) && TEST_INTEGRATED_FW
#include "p34x_phy_fw.h"
#endif

#if defined(DEBUG_SGMII) && DEBUG_SGMII
#define REG_DUMP(pdata, fmt, ...)	dev_info(&pdata->pdev->dev, fmt, ##__VA_ARGS__)
#else
#define REG_DUMP(pdata, fmt, ...)	do {} while (0)
#endif

#define ENABLE_POWER_DOWN	1
#define ENABLE_MAXLINEAR_HWMON	0
#define P34X_FW_VERSION_OFFSET	0x11FE
#define FW_WR_DEF_TIMEOUT	5000

#define PHY_REG_GPIO2_ALTSEL1    0x00D3CE30
#define PHY_REG_GPIO2_ALTSEL0    0x00D3CE2C
#define PHY_REG_GPIO2_DIR        0x00D3CE28
#define PHY_REG_GPIO2_OUT        0x00D3CE20
#define PHY_REG_GPIO_ALTSEL1     0x00D3CE10
#define PHY_REG_GPIO_ALTSEL0     0x00D3CE0C
#define PHY_REG_GPIO_DIR     0x00D3CE08
#define PHY_REG_GPIO_OUT     0x00D3CE00
#define PHY_REG_PVT_ADDR     0xD288B4

static int (*p34x_reg_read)(struct p34x_priv_data *pdata, u32 ahb_addr);
static int (*p34x_reg_write)(struct p34x_priv_data *pdata, u32 ahb_addr, u16 data);

static const char *fw_pathname_unsigned[P34X_MAX_STEP_COUNT] = {
	"ltq_fw_PHY34X_URx8xx_Axx.bin",
	"ltq_fw_PHY34X_URx8xx_Bxx.bin"
};

static const char *fw_pathname_signed[P34X_MAX_STEP_COUNT] = {
	"mxl_fw_PHY34X_URx6xx_Bxx.bin",
	"mxl_fw_PHY34X_URx6xx_Bxx.bin"
};

static void p34x_set_fw_pathname(struct p34x_priv_data *pdata, enum p34x_fw_type type)
{
	int i;

	if (type == P34X_FW_SIGNED)
		for (i = 0; i < P34X_MAX_STEP_COUNT; i++)
			pdata->fw_pathname[i] = fw_pathname_signed[i];
	else
		for (i = 0; i < P34X_MAX_STEP_COUNT; i++)
			pdata->fw_pathname[i] = fw_pathname_unsigned[i];
}

static int p34x_mbox_busy(struct p34x_priv_data *pdata, u8 action)
{
	int ret = 0;
	u32 value = 0;
	u32 busyRetry = MBOX_MAX_BUSY_RETRY;

	do {
		ret = mmd_read(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD);
		if (ret < 0)
			return ret;

		value = ret & VSPEC1_MBOX_CMD_READY_MASK;
	} while ((!value) && (--busyRetry));

	if ((!value && !busyRetry)) {
		dev_err(&pdata->pdev->dev,
			"ERROR: MBOX busy for too long\n");
		if (action == RETURN_FROM_FUNC)
			return -1;

		ret = -1;
	}

	return ret;
}

static int p34x_mbox_patch_fail(struct p34x_priv_data *pdata)
{
	int ret = 0;
	u32 value = 0;
	u32 busyRetry = MBOX_MAX_BUSY_RETRY;

	do {
		ret = mmd_read(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD);
		if (ret < 0)
			return ret;

		value = ret & VSPEC1_MBOX_CMD_FAIL_MASK;
	} while ((!value) && (--busyRetry));

	if ((!value && !busyRetry)) {
		dev_err(&pdata->pdev->dev,
			"ERROR: MBOX patch fail for too long\n");
		ret = -1;
	}

	return ret;
}

static int p34x_mbox_read(struct p34x_priv_data *pdata, u32 ahb_addr)
{
	u16 addr_low = 0;
	u16 addr_high = 0;
	u16 mbox_cmd = 0;
	int ret = 0;

	if (!pdata)
		return -EINVAL;

	addr_low = ahb_addr & 0xFFFF;
	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_ADDRLO, addr_low);
	if (ret < 0)
		return ret;

	addr_high = ahb_addr >> 16;
	mbox_cmd = (addr_high & 0xFF) & (~0xF00);
	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD, mbox_cmd);
	if (ret < 0)
		return ret;

	if (p34x_mbox_busy(pdata, RETURN_FROM_FUNC) < 0)
		return -EBUSY;

	ret = mmd_read(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA);
	if (ret < 0)
		return ret;

	return ret;
}

static int p34x_mbox_write(struct p34x_priv_data *pdata, u32 ahb_addr, u16 data)
{
	int ret = 0;
	u16 mbox_cmd = 0;
	u16 addr_high = 0;
	u16 addr_low = 0;

	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA, data);
	if (ret < 0)
		return ret;

	addr_low = ahb_addr & 0xFFFF;
	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_ADDRLO, addr_low);
	if (ret < 0)
		return ret;

	addr_high = ahb_addr >> 16;
	mbox_cmd = ((addr_high & 0xFF) & (~0xF00)) | (1 << 8 & 0xF00);
	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD, mbox_cmd);
	if (ret < 0)
		return ret;

	if (p34x_mbox_busy(pdata, RETURN_FROM_FUNC) < 0)
		return -EBUSY;

	return 0;
}

static int p34x_smdio_read(struct p34x_priv_data *pdata, u32 ahb_addr)
{
	return smdio_read(pdata, AHB_TO_PDI(ahb_addr));
}

static int p34x_smdio_write(struct p34x_priv_data *pdata, u32 ahb_addr, u16 data)
{
	return smdio_write(pdata, AHB_TO_PDI(ahb_addr), data);
}

static int __fabric_wr(struct p34x_priv_data *pdata, u32 data)
{
	int ret;
	u16 _data_low, _data_hi;

	_data_hi = (data >> 16) & 0xFFFF;
	_data_low = data & 0xFFFF;

	/* Fabric indirect access data High register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_DATAH, _data_hi);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_DATAH write fails: %d\n", ret);
		return ret;
	}

	/* Fabric indirect access data Low register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_DATAL, _data_low);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_DATAL write fails: %d\n", ret);
		return ret;
	}

	return 0;
}

static int __fabric_rd(struct p34x_priv_data *pdata, u32 *data)
{
	int ret;
	u32 _data_low, _data_hi;

	/* Fabric indirect access Access Control register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_CTRL, 0x1);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_CTRL read fails\n");
		return ret;
	}

	/* Fabric indirect access data High register */
	ret = p34x_reg_read(pdata, AHB_FABRIC_PDI_DATAH);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_DATAH read fails\n");
		return ret;
	}

	_data_hi = ret & 0xFFFF;

	/* Fabric indirect access data Low register */
	ret = p34x_reg_read(pdata, AHB_FABRIC_PDI_DATAL);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_DATAL read fails\n");
		return ret;
	}

	_data_low = ret & 0xFFFF;

	*data = (_data_hi << 16) | _data_low;

	return 0;
}

static int __fabric_wraddr(struct p34x_priv_data *pdata, u32 regaddr)
{
	int ret;
	u16 _base_low, _base_hi;

	_base_hi = (regaddr >> 16) & 0xFFFF;
	_base_low = (regaddr & 0xFFFF);

	/* Fabric indirect access Access Control register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_CTRL, 0x2);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_CTRL write fails: %d\n", ret);
		return ret;
	}

	/* Fabric indirect access address High register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_ADDRH, _base_hi);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_ADDRH write fails: %d\n", ret);
		return ret;
	}

	/* Fabric indirect access address Low register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_ADDRL, _base_low);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_ADDRL write fails: %d\n", ret);
		return ret;
	}

	return 0;
}

static int __fabric_rdaddr(struct p34x_priv_data *pdata, u32 regaddr)
{
	int ret;
	u16 _base_low, _base_hi;

	_base_hi = (regaddr >> 16) & 0xFFFF;
	_base_low = (regaddr & 0xFFFF);

	/* Fabric indirect access address High register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_ADDRH, _base_hi);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_ADDRH read fails: %d\n", ret);
		return ret;
	}

	/* Fabric indirect access address Low register */
	ret = p34x_reg_write(pdata, AHB_FABRIC_PDI_ADDRL, _base_low);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: FABRIC_PDI_ADDRL read fails: %d\n", ret);
		return ret;
	}

	return 0;
}

static int __usxgmii_fabric_rd(struct p34x_priv_data *pdata, u32 regaddr)
{
	int ret;
	u32 data;
	u32 usxgmii_addr;
	u32 address_base, address_offset;

	address_base = regaddr >> 8;
	address_offset = regaddr & 0xFF;

	/* Set the USXGMII base address to read */
	ret = __fabric_wraddr(pdata, USXGMII_PDI_BASEADRR);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "USXGMII_PDI_BASEADRR write fails\n");
		return ret;
	}

	/* Set the address_base */
	ret = __fabric_wr(pdata, address_base);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "api __fabric_wr fails\n");
		return ret;
	}

	/* Set the USXGMII PDI indirect address */
	usxgmii_addr = USXGMII_PDI_INDIRECT + (4 * address_offset);
	ret = __fabric_rdaddr(pdata, usxgmii_addr);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "USXGMII_PDI_INDIRECT write fails\n");
		return ret;
	}

	/* Get the data */
	ret = __fabric_rd(pdata, &data);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "api __fabric_rd fails\n");
		return ret;
	}

	return data;
}

static int __usxgmii_fabric_wr(struct p34x_priv_data *pdata, u32 regaddr, u32 data)
{
	int ret;
	u32 usxgmii_addr;
	u32 address_base, address_offset;

	address_base = regaddr >> 8;
	address_offset = regaddr & 0xFF;

	/* Set the USXGMII base address to write */
	ret = __fabric_wraddr(pdata, USXGMII_PDI_BASEADRR);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "USXGMII_PDI_BASEADRR write fails\n");
		return ret;
	}

	/* Set the address_base */
	ret = __fabric_wr(pdata, address_base);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "api __fabric_wr fails\n");
		return ret;
	}

	/* Set the USXGMII PDI indirect address */
	usxgmii_addr = USXGMII_PDI_INDIRECT + (4 * address_offset);
	ret = __fabric_wraddr(pdata, usxgmii_addr);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "USXGMII_PDI_INDIRECT write fails\n");
		return ret;
	}

	/* Set the data */
	ret = __fabric_wr(pdata, data);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "api __fabric_wr fails\n");
		return ret;
	}

	return ret;
}

int p34x_xpcs_rd(struct p34x_priv_data *pdata, u32 off)
{
	return __usxgmii_fabric_rd(pdata, off);
}

int p34x_xpcs_wr(struct p34x_priv_data *pdata, u32 off, u32 data)
{
	return __usxgmii_fabric_wr(pdata, off, data);
}

#if !SHORT_TRACE
static void __usxgmii_phy_wr(struct p34x_priv_data *pdata, u32 phy_address, u32 phy_data)
{
	u32 addr = 0x0;
	u32 read_val = 0x1;
	int timeout;

	addr = 0x180a0;	//VR_XS_PMA_SNPS_CR_CTRL

	timeout = 0;
	while (read_val != 0 && timeout++ < CR_CTRL_TIMEOUT) {
		REG_DUMP(pdata, "Check if bit 0 VR_XS_PMA_SNPS_CR_CTRL is 0\n");
		read_val = __usxgmii_fabric_rd(pdata, addr);
		read_val = read_val & 0x1;
	}

	REG_DUMP(pdata, "write Phy address to VR_XS_PMA_SNPS_CR_ADDR\n");
	addr = 0x180a1;	//VR_XS_PMA_SNPS_CR_ADDR
	__usxgmii_fabric_wr(pdata, addr, phy_address);

	REG_DUMP(pdata, "write to VR_XS_PMA_SNPS_CR_DATA\n");
	addr = 0x180a2;	//VR_XS_PMA_SNPS_CR_DATA
	__usxgmii_fabric_wr(pdata, addr, phy_data);

	REG_DUMP(pdata, "write 1 to bit 1 of VR_XS_PMA_SNPS_CR_CTRL\n");
	addr = 0x180a0;	//VR_XS_PMA_SNPS_CR_CTRL
	read_val = __usxgmii_fabric_rd(pdata, addr);
	read_val = read_val | (0x1 << 1);
	__usxgmii_fabric_wr(pdata, addr, read_val);

	REG_DUMP(pdata, "write 1 to bit 0 of VR_XS_PMA_SNPS_CR_CTRL\n");
	read_val = __usxgmii_fabric_rd(pdata, addr);
	read_val = read_val | 0x1;
	__usxgmii_fabric_wr(pdata, addr, read_val);

	read_val = 0x1;

	timeout = 0;
	while (read_val != 0 && timeout++ < CR_CTRL_TIMEOUT) {
		REG_DUMP(pdata, "Check if bit 0 VR_XS_PMA_SNPS_CR_CTRL is 0\n");
		read_val = __usxgmii_fabric_rd(pdata, addr);
		read_val = read_val & 0x1;
	}
}
#endif

static void __usxgmii_usra_reset(struct p34x_priv_data *pdata)
{
	u32 addr = 0x0;
	u32 read_val = 0x0;

	//p34x.usxgmii.usra_reset()
	REG_DUMP(pdata, "### USRA RESET PORT 0 ###\n");
	addr = 0x38000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 10);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	REG_DUMP(pdata, "CSR_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));

	REG_DUMP(pdata, "### USRA RESET PORT 1 ###\n");
	addr = 0x1a8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 5);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	REG_DUMP(pdata, "CSR_MII_1_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8010));

	REG_DUMP(pdata, "### USRA RESET PORT 2 ###\n");
	addr = 0x1b8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 5);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_2_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	REG_DUMP(pdata, "CSR_MII_2_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8010));

	REG_DUMP(pdata, "### USRA RESET PORT 3 ###\n");
	addr = 0x1c8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 5);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_3_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	REG_DUMP(pdata, "CSR_MII_3_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8010));
}

static void __usxgmii_vr_reset(struct p34x_priv_data *pdata)
{
	u32 addr = 0x0;
	u32 read_val = 0x0;
	int i = 0;

	REG_DUMP(pdata, "### VR_SOFT_RESET ###\n");
	addr = 0x38000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val & 0xFFFF;
	read_val = read_val | (0x1 << 15);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	for (i = 0; i < SRAM_INIT_TIMEOUT; i++) {
		if (__usxgmii_fabric_rd(pdata, 0x1809b)&BIT(0))	//check INIT_DN
			break;

		udelay(10);
	}
	if (i >= SRAM_INIT_TIMEOUT)
		dev_err(&pdata->pdev->dev, "SRAM init timeout\n");

	for (i = 0; i < VR_RESET_TIMEOUT; i++) {
		if (!(__usxgmii_fabric_rd(pdata, addr)&BIT(15)))	//value after reset cleared 0x0
			break;

		udelay(10);
	}

	if (i >= VR_RESET_TIMEOUT)
		dev_err(&pdata->pdev->dev, "XPCS VR RESET fail\n");
	else
		dev_info(&pdata->pdev->dev, "XPCS VR RESET success\n");

	//wait_for_powerup_xpcs5();
	for (i = 0; i < XPCS5_POWER_TIMEOUT; i++) {
		read_val = __usxgmii_fabric_rd(pdata, 0x38010);	//CSR_DIG_STS	//VR_XS_PCS_DIG_STS[4:2]
		read_val = read_val >> 2;	// bit[4:2] (PSEQ_STATE)
		read_val &= 0x7;

		if (read_val == 0x4) {	//3b100 Tx/Rx Stable (Power_Good state)
			dev_info(&pdata->pdev->dev, "wait_for_powerup_xpcs5...success\n");
			break;
		}

		mdelay(10);
	}
	if (i >= XPCS5_POWER_TIMEOUT)
		dev_err(&pdata->pdev->dev, "XPCS5 power up timeout\n");
}

static void __usxgmii_set_speed(struct p34x_priv_data *pdata, int dump)
{
	int data;
	int i;

	/* Speed selection
	 * 2.5G=f9
	 * 1G=f5
	 * 100M=f3
	 * 10M=f1
	 */
	for (i = 0; i < 4; i++) {
		p34x_reg_write(pdata, AHB_TOP_SLICE_IF, i);
		if (!dump)
			p34x_reg_write(pdata, AHB_SLICE_CTRL_PDI_USXGMII, 0x99);
		data = p34x_reg_read(pdata, AHB_SLICE_CTRL_PDI_USXGMII);
		REG_DUMP(pdata,
			"Slice %d: 2.5G=f9, 1G=f5, 100M=f3, 10M=f1 ...: 0x%x\n",
			i, data);
	}

	p34x_reg_write(pdata, AHB_TOP_SLICE_IF, 0x0);
}

static void __usxgmii_en(struct p34x_priv_data *pdata)
{
	u32 addr = 0x0, data;
	u32 read_val = 0x0, read_data = 0x0;
	int i = 0;

	__usxgmii_set_speed(pdata, 0);	//set all slice speed 2.5G
	REG_DUMP(pdata, "USXGMII Configuration started\n");
	p34x_reg_write(pdata, AHB_LJPLL_CFG0L, 0x1);

	for (i = 0; i < CGU_PLLLOCK_TIMEOUT; i++) {
		data = p34x_reg_read(pdata, AHB_LJPLL_CFG0L);

		if (data & BIT(1))
			break;

		udelay(10);
	}
	if (i >= CGU_PLLLOCK_TIMEOUT)
		dev_err(&pdata->pdev->dev, "CGU PLL lock timeout\n");

	data = p34x_reg_read(pdata, AHB_LJPLL_CFG0L);
	REG_DUMP(pdata, "CGU_PDI.LJPLL_CFG0L.PLLEN: 0x%x\n", data);

	p34x_reg_write(pdata, AHB_CGU_PDI_CLK_EN, 0x1);
	data = p34x_reg_read(pdata, AHB_CGU_PDI_CLK_EN);
	REG_DUMP(pdata, "CGU_PDI.CGU_PDI_CLK_EN.SEL(USXGMII_CLK_EN): 0x%x\n", data);
	mdelay(10);	//let the clock more time to lock

	/* Release the all usxgmii iddq,pdata,xpcs bit 8,9,10 */
	data = p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ);
	REG_DUMP(pdata, "before USGMII IDDQ release: %04x\n", data);
	data &= ~BIT(10);
	p34x_reg_write(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ, data);
	udelay(25);
	REG_DUMP(pdata, "after USGMII IDDQ release %04x\n", p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ));

	data &= ~(BIT(9) | BIT(8));
	p34x_reg_write(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ, data);
	data = p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ);
	REG_DUMP(pdata, ">>>USXGMII released... value:0x%x\n", data);
	mdelay(10);	//wait after USXGMII release

	data = p34x_reg_read(pdata, AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0);
	REG_DUMP(pdata, "SLICE_RCU_PDI_REGISTER.SLICE_PERSISTENT before:0x%x\n", data);
	p34x_reg_write(pdata, AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0, 0x4);
	data = p34x_reg_read(pdata, AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0);
	REG_DUMP(pdata, "SLICE_RCU_PDI_REGISTER.SLICE_PERSISTENT after:0x%x\n", data);

	for (i = 0; i < SRAM_INIT_TIMEOUT; i++) {
		if (__usxgmii_fabric_rd(pdata, 0x1809b)&BIT(0))	//check INIT_DN
			break;

		udelay(10);
	}
	if (i >= SRAM_INIT_TIMEOUT)
		dev_err(&pdata->pdev->dev, "SRAM init timeout\n");

	read_val = __usxgmii_fabric_rd(pdata, 0x1809b);
	read_val = read_val | 0x2;	//write EXT_LD_DN
	__usxgmii_fabric_wr(pdata, 0x1809b, read_val);	//VR_XS_PMA_Gen5_12G_16G_SRAM
	mdelay(10);

	REG_DUMP(pdata, "READ CSR_RAM: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1809b));

	data = p34x_reg_read(pdata, AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0);
	REG_DUMP(pdata, "SLICE_RCU_PDI_REGISTER.SLICE_PERSISTENT before:0x%x\n", data);
	p34x_reg_write(pdata, AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0, 0x5);
	data = p34x_reg_read(pdata, AHB_SLICE_RCU_PDI_REGISTERS_STORAGE_0);
	REG_DUMP(pdata, "SLICE_RCU_PDI_REGISTER.SLICE_PERSISTENT after:0x%x\n", data);

	//wait_for_powerup_xpcs5();
	for (i = 0; i < XPCS5_POWER_TIMEOUT; i++) {
		read_val = __usxgmii_fabric_rd(pdata, 0x38010);	//CSR_DIG_STS	//VR_XS_PCS_DIG_STS[4:2]
		read_val = read_val >> 2;	// bit[4:2] (PSEQ_STATE)
		read_val &= 0x7;

		if (read_val == 0x4) {	//3b100 Tx/Rx Stable (Power_Good state)
			REG_DUMP(pdata, "wait_for_powerup_xpcs5...success\n");
			break;
		}

		udelay(10);
	}
	if (i >= XPCS5_POWER_TIMEOUT)
		dev_err(&pdata->pdev->dev, "XPCS5 power up timeout\n");

	REG_DUMP(pdata, "READ VR_PMA_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18000));
	REG_DUMP(pdata, "READ CSR_DIG_STS: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));

	//p34x.usxgmii.disable_an()
	REG_DUMP(pdata, "### DISABLE AN ###\n");
	addr = 0x70000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "READ CSR_AN_CTRL before: 0x%x\n", read_val);
	read_val = read_val & ~(0x1 << 12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "READ CSR_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	//p34x.usxgmii.enable_kr()
	REG_DUMP(pdata, "### ENABLE KR ###\n");
	addr = 0x30007;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_PCS_CTRL2 before: 0x%x\n", read_val);
	read_val = read_val & 0xFFFF;
	read_val = read_val & ~(0xF);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_PCS_CTRL2 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	//p34x.usxgmii.configure_usxgmii("10GQ",is_emu=is_emulation)
	//usxgmii_phy_config()
	REG_DUMP(pdata, "USXGMII PHY CONFIG \n>");
	addr = 0x18091;	//CSR_E12_PMA_REF_CLK_CTRL
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data |= 0x00000040;	//bit[6]
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18071; //CSR_E12_PMA_MPLLA_CTRL0
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFFF00;
	read_data |= 0x00000021; //bit[7:0] 8'd33;
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18077; //CSR_E12_PMA_MPLLA_CTRL3
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFF0000;
	read_data |= 0x0000A016; //bit[15:0] 16'd40982;
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18076; //CSR_E12_PMA_MPLLA_CTRL2
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data |= (1 << 10); //bit[10] mplla_div16p5_clk_en = 1
	read_data |= (1 << 8); //bit[8] mplla_div8_clk_en = 1
	read_data &= ~(1 << 9); //bit[9] mplla_div10_clk_en = 0
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18031; //CSR_E12_PMA_TXGENERAL_CTRL1
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFF8FF;
	read_data |= 0x00000500; //wr_data[10:8] = 3'h5; // VBOOST_LVL // [NEW] Based on 1.03b_cust2 E16 X2 spec
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18092; //CSR_E12_PMA_VCO_LD_CTRL0
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFE000;
	read_data |= 0x00000549; //wr_data[12:0] = 13'd1353;
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18096; //CSR_E12_PMA_VCO_REF_CTRL0
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFFF80;
	read_data |= 0x00000029; //wr_data[6:0] = 7'd41;
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x1805D; //CSR_E12_PMA_AFE_DFE_EN
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data |= (1 << 4); //bit[4] dfe_en = 1
	read_data |= (1 << 0); //bit[0] afe_en = 1
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x1805C; //CSR_E12_PMA_RX_EQ_CTRL4
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data |= (1 << 0); //bit[0] adapt_cont = 1
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18034; //CSR_E12_PMA_TX_RATE_CTRL
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= ~(7 << 0); //wr_data[2:0] = 3'b000; // 10G
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18054; //CSR_E12_PMA_RX_RATE_CTRL
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= ~(3 << 0); //wr_data[1:0] = 2'b00; // 10G
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18032; //CSR_E12_PMA_TXGENERAL_CTRL
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFFCFF;
	read_data |= 0x00000200; //wr_data[9:8] = 2'b10; // 16-bit interface
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18052; //CSR_E12_PMA_RX_GENERAL_CTRL2
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFFCFF;
	read_data |= 0x00000200; //wr_data[9:8] = 2'b10; // 16-bit interface
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18053; //CSR_E12_PMA_RX_GENERAL_CTRL3
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data &= 0xFFFFFFF8;
	read_data |= 0x00000007; //wr_data[2:0] = 3'd7; // LOS_TRSHLD_0
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18031; //CSR_E12_PMA_TXGENERAL_CTRL1
	read_data = __usxgmii_fabric_rd(pdata, addr);
	read_data |= (1 << 4); //wr_data[4] = 1'b1; // VBOOST_EN_0
	__usxgmii_fabric_wr(pdata, addr, read_data);

	addr = 0x18057; //VR_XS_PMA_Gen5_12G_16G_RX_ATTN_CTRL
	read_data = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_RX_ATTN_CTRL before: 0x%x\n", read_data);
	read_data &= 0xFFFFFFF8;
	read_data |= 7; //BIT2:0 RX0_EQ_ATT_LVL, RX_10G_SHORT - 7, RX_10G - 0, RX_ADAPT - 7
	__usxgmii_fabric_wr(pdata, addr, read_data);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_RX_ATTN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x18058; //VR_XS_PMA_Gen5_12G_RX_EQ_CTRL0
	read_data = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_RX_EQ_CTRL0 before: 0x%x\n", read_data);
	// RX_10G_SHORT - rx_ctle_boost =  0x0, rx_ctle_pole = 0x0, rx_vga2_gain = 0x0, rx_vga1_gain = 0x0;
	// RX_10G       - rx_ctle_boost = 0x10, rx_ctle_pole = 0x2, rx_vga2_gain = 0x5, rx_vga1_gain = 0x5;
	// RX_ADAPT     - rx_ctle_boost =  0x0, rx_ctle_pole = 0x2, rx_vga2_gain = 0x0, rx_vga1_gain = 0x0;
	// BIT  4:0 - rx_ctle_boost
	// BIT  6:5 - rx_ctle_pole
	// BIT 10:8 - rx_vga2_gain
	// BIT14:12 - rx_vga1_gain
	read_data &= 0xFFFF0000;
	__usxgmii_fabric_wr(pdata, addr, read_data);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_RX_EQ_CTRL0 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	//postdebug
	addr = 0x1805d; //VR_XS_PMA_Gen5_12G_AFE_DFE_EN_CTRL
	read_data = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_AFE_DFE_EN_CTRL before: 0x%x\n", read_data);
	// RX_10G_SHORT - 0
	// RX_10G       - 0xFF
	// RX_ADAPT     - 0xFF
	read_data &= 0xFFFFFF00;
	__usxgmii_fabric_wr(pdata, addr, read_data);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_AFE_DFE_EN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	REG_DUMP(pdata, "### CONFIGURE_USXGMII ###\n");
	addr = 0x38000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val & 0xFFFF;
	read_val = read_val | (0x1 << 9);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x38007;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_VR_RPCS_TPC before: 0x%x\n", read_val);
	read_val = read_val & 0xFFFF;
	read_val = read_val & ~(0x7 << 10);
	read_val = read_val | (0x5 << 10); //phy mode = 10GD
	//read_val = read_val | (0x2 << 10);	//change to 2.5G
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_VR_RPCS_TPC after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	__usxgmii_vr_reset(pdata);

	// TX EQ
	addr = 0x18036;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL0 before: 0x%x\n", read_val);
	read_val &= ~(0x3F << 0); //Clear BIT5:0 EQ_PRE
	read_val &= ~(0x3F << 8); //Clear BIT13:8 EQ_MAIN
	read_val |= 0x0 << 0; //Set BIT5:0 EQ_PRE
	read_val |= 0x14 << 8; //SetBIT13:8 EQ_MAIN
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL0 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x18037;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL1 before: 0x%x\n", read_val);
	read_val &= ~(0x3F << 0); //Clear BIT5:0 EQ_PRE
	read_val &= ~(0x1 << 6); //Clear BIT6 EQ_MAIN
	read_val |= 0x0 << 0; //Set BIT5:0 TX_EQ_POST
	read_val |= 0x1 << 6; //Set BIT6 TX_EQ_OVR_RIDE
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x18033;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_TX_BOOST_CTRL before: 0x%x\n", read_val);
	read_data &= ~(0xF << 0); //Clear BIT3:0 TX0_IBOOST
	read_data |= 0x5 << 0; //Set BIT3:0 TX0_IBOOST
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "VR_XS_PMA_Gen5_12G_16G_TX_BOOST_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	//p34x.usxgmii.disable_an()	//todo added as issue seen after VR_reset
	REG_DUMP(pdata, "### DISABLE AN 73 ###\n");
	addr = 0x70000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "READ CSR_AN_CTRL before: 0x%x\n", read_val);
	read_val = read_val & ~(0x1<<12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "READ CSR_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	REG_DUMP(pdata, "### DISABLE CL37_AN BIT ###\n");
	addr = 0x1f0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_CTRL before: 0x%x\n", read_val);
	read_val = read_val & ~(0x1<<12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1a0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_1_CTRL before: 0x%x\n", read_val);
	read_val = read_val & ~(0x1<<12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_1_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1b0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_2_CTRL before: 0x%x\n", read_val);
	read_val = read_val & ~(0x1<<12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_2_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1c0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_3_CTRL before: 0x%x\n", read_val);
	read_val = read_val & ~(0x1<<12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_3_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

#if 0
	//p34x.usxgmii.configure_usxgmii_los()
	REG_DUMP(pdata, "### CONFIGURE USXGMII LOS ###\n");
	addr = 0x380005;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DEBUG_CTL before: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	read_val = read_val | (0x1 << 4);
	read_val = read_val | (0x1 << 6);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_DEBUG_CTL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
#endif

	//p34x.usxgmii.vr_datapath_init()
	REG_DUMP(pdata, "### VR_DATAPATH_INIT ###\n");
	addr = 0x38000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 8);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	REG_DUMP(pdata, "CSR_DIG_STS read to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));

	//p34x.usxgmii.configure_usxgmii_an_ctrl()
	REG_DUMP(pdata, "### CONFIGURE USXGMII AN_CTRL PORT 0 ###\n");
	addr = 0x1f8001;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_AN_CTRL before: 0x%x\n", read_val);
	read_val = read_val | 0x1;
	read_val = read_val & ~(0x1 << 8);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | (0x1 << 4);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_AN_CTRL after: 0x%x\n", read_val);

	REG_DUMP(pdata, "### CONFIGURE USXGMII AN_CTRL PORT 1 ###\n");
	addr = 0x1a8001;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_AN_CTRL before: 0x%x\n", read_val);
	read_val = read_val | 0x1;
	read_val = read_val & ~(0x1 << 8);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | (0x1 << 4);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_1_AN_CTRL after: 0x%x\n", read_val);

	REG_DUMP(pdata, "### CONFIGURE USXGMII AN_CTRL PORT 2 ###\n");
	addr = 0x1b8001;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_AN_CTRL before: 0x%x\n", read_val);
	read_val = read_val | 0x1;
	read_val = read_val & ~(0x1 << 8);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | (0x1 << 4);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_2_AN_CTRL after: 0x%x\n", read_val);

	REG_DUMP(pdata, "### CONFIGURE USXGMII AN_CTRL PORT 3 ###\n");
	addr = 0x1c8001;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_AN_CTRL before: 0x%x\n", read_val);
	read_val = read_val | 0x1;
	read_val = read_val & ~(0x1 << 8);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | (0x1 << 4);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_3_AN_CTRL after: 0x%x\n", read_val);

	REG_DUMP(pdata, "### SEE IF RX LINK UP ###\n");
	read_val = __usxgmii_fabric_rd(pdata, 0x30001);
	REG_DUMP(pdata, "0x%x\n", read_val);

	//p34x.usxgmii.usxgmii_cl37_an_enable_io_mode()
	REG_DUMP(pdata, "### cl37 AN Enable ###\n");
	addr = 0x1f800a;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "### SET LINK TIMER ###\n");
	REG_DUMP(pdata, "CSR_MII_LINK_TIMER before: 0x%x\n", read_val);
	// 0x000 - 1.6ms
	// 0x100 - 0.42ms
	// 0x263 - 1.0ms
	// 0x3d1 - 1.6ms (default)
	// 0x7a1 - 3.2ms (SGMII 2.5G mode)
	// 0x1000 - 6.71ms
	__usxgmii_fabric_wr(pdata, addr, 0x3d1);
	REG_DUMP(pdata, "CSR_MII_LINK_TIMER after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	REG_DUMP(pdata, "CSR_MII_1_LINK_TIMER after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a800a));
	REG_DUMP(pdata, "CSR_MII_2_LINK_TIMER after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b800a));
	REG_DUMP(pdata, "CSR_MII_3_LINK_TIMER after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c800a));

	addr = 0x1f8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "### SET OVERRIDE BIT in ALL PORTS ###\n");
	REG_DUMP(pdata, "CSR_MII_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | 0x1;
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1a8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | 0x1;
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1b8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_2_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | 0x1;
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_2_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1c8000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_3_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 3);
	read_val = read_val | 0x1;
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_3_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	// redo speed config
	REG_DUMP(pdata, "\n### Redo SPEED CONFIG for all ports ###\n");
	addr = 0x1f0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_CTRL before: 0x%x\n",read_val);
	read_val |=  (0x1 << 5);	//SS5	= 1
	read_val &= ~(0x1 << 6);	//SS6	= 0
	read_val &= ~(0x1 << 13);	//SS13	= 0
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1a0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_1_CTRL before: 0x%x\n",read_val);
	read_val |=  (0x1 << 5);	//SS5	= 1
	read_val &= ~(0x1 << 6);	//SS6	= 0
	read_val &= ~(0x1 << 13);	//SS13	= 0
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_1_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1b0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_2_CTRL before: 0x%x\n",read_val);
	read_val |=  (0x1 << 5);	//SS5	= 1
	read_val &= ~(0x1 << 6);	//SS6	= 0
	read_val &= ~(0x1 << 13);	//SS13	= 0
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_2_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1c0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_3_CTRL before: 0x%x\n",read_val);
	read_val |=  (0x1 << 5);	//SS5	= 1
	read_val &= ~(0x1 << 6);	//SS6	= 0
	read_val &= ~(0x1 << 13);	//SS13	= 0
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_3_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	REG_DUMP(pdata, "### ENABLE CL37_AN BIT ###\n");
	addr = 0x38000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	REG_DUMP(pdata, "### ENABLE AN BIT for all ports ###\n");
	addr = 0x1f0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_CTRL before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1a0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_1_CTRL before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_1_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1b0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_2_CTRL before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_2_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	addr = 0x1c0000;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_MII_3_CTRL before: 0x%x\n", read_val);
	read_val = read_val | (0x1 << 12);
	__usxgmii_fabric_wr(pdata, addr, read_val);
	REG_DUMP(pdata, "CSR_MII_3_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	//ALIGNMENT TIMER CONFIG
	addr = 0x3800a;
	REG_DUMP(pdata, "CSR_VR_PCS_AM_CTRL before: 0x%x\n",__usxgmii_fabric_rd(pdata, addr));
#if 0
	// use default 0x4010
	__usxgmii_fabric_wr(pdata, addr, 0x1f40);
	REG_DUMP(pdata, "CSR_VR_PCS_AM_CTRL after: 0x%x\n",__usxgmii_fabric_rd(pdata, addr));
#endif

	//udelay(10);
	mdelay(100);
	__usxgmii_usra_reset(pdata);
	udelay(1);

	REG_DUMP(pdata, "VR_XS_PCS_DIG_STS now: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));

	REG_DUMP(pdata, "SR_XS_PCS_STS1 now: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x30001));
	REG_DUMP(pdata, "VR_PMA_CTRL now: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18000));

	read_val = __usxgmii_fabric_rd(pdata, 0x1f8002); //VR_MII_AN_INTR_STS
	REG_DUMP(pdata, "ANEG Complete for Port 0: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1a8002); //VR_MII_1_AN_INTR_STS
	REG_DUMP(pdata, "ANEG Complete for Port 1: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1b8002); //VR_MII_2_AN_INTR_STS
	REG_DUMP(pdata, "ANEG Complete for Port 2: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1c8002); //VR_MII_3_AN_INTR_STS
	REG_DUMP(pdata, "ANEG Complete for Port 3: 0x%x\n", (read_val & 0x1));

	REG_DUMP(pdata, "New Register");
	REG_DUMP(pdata, "READ CSR_RAM: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1809b));
	REG_DUMP(pdata, "READ VR_PMA_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18000));
	REG_DUMP(pdata, "READ CSR_DIG_STS: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));
	REG_DUMP(pdata, "READ CSR_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x70000));
	REG_DUMP(pdata, "CSR_PCS_CTRL2 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x30007));

	REG_DUMP(pdata, "### READ CONFIGURATION_USXGMII ####");
	REG_DUMP(pdata, "VR_XS_PCS_DIG_CTRL1 before: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "CSR_VR_RPCS_TPC after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38007));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "CSR_VR_PCS_AM_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x3800a));
	REG_DUMP(pdata, "CSR_DEBUG_CTL before: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x380005));
	REG_DUMP(pdata, "CSR_DEBUG_CTL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x380005));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "CSR_DIG_STS read to flush 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));
	REG_DUMP(pdata, "CSR_MII_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f8001));
	REG_DUMP(pdata, "CSR_MII_1_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8001));
	REG_DUMP(pdata, "CSR_MII_2_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8001));
	REG_DUMP(pdata, "CSR_MII_3_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8001));
	REG_DUMP(pdata, "RX LINK UP:0x%x\n", __usxgmii_fabric_rd(pdata, 0x30001));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "CSR_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8000));
	REG_DUMP(pdata, "CSR_MII_1_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8010));
	REG_DUMP(pdata, "CSR_MII_2_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8000));
	REG_DUMP(pdata, "CSR_MII_2_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8010));
	REG_DUMP(pdata, "CSR_MII_3_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8000));
	REG_DUMP(pdata, "CSR_MII_3_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8010));
	REG_DUMP(pdata, "RX LINK UP:0x%x\n", __usxgmii_fabric_rd(pdata, 0x30001));
	REG_DUMP(pdata, "CSR_MII_LINK_TIMER after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f800a));
	REG_DUMP(pdata, "VR_MII_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f8000));
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8000));
	REG_DUMP(pdata, "CSR_MII_2_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8000));
	REG_DUMP(pdata, "CSR_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8000));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "CSR_MII_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f0000));
	REG_DUMP(pdata, "CSR_MII_1_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a0000));
	REG_DUMP(pdata, "CSR_MII_2_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b0000));
	REG_DUMP(pdata, "CSR_MII_3_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c0000));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "CSR_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));
	REG_DUMP(pdata, "CSR_MII_1_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8000));
	REG_DUMP(pdata, "CSR_MII_1_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8010));
	REG_DUMP(pdata, "CSR_MII_2_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8000));
	REG_DUMP(pdata, "CSR_MII_2_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8010));
	REG_DUMP(pdata, "CSR_MII_3_DIG_CTRL1 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8000));
	REG_DUMP(pdata, "CSR_MII_3_DIG_STS to flush: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8010));
	REG_DUMP(pdata, "SR_XS_PCS_STS1 now: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x30001));
	REG_DUMP(pdata, "VR_PMA_CTRL now: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18000));
	REG_DUMP(pdata, "VR_XS_PCS_DEBUG_CTRL now: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38005));

	REG_DUMP(pdata, "VR_XS_PCS_DEBUG_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38005));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_STS: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));
	REG_DUMP(pdata, "VR_XS_PCS_DEBUG_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38005));
	REG_DUMP(pdata, "VR_XS_PCS_DIG_STS: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38000));

	REG_DUMP(pdata, "6.250 VR_XS_PMA_Gen5_10G_MISC_CTR: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f809f));
	//p34x.usxgmii.fabric_write(0x1f809f, 0x0001)
	REG_DUMP(pdata, "6.250 VR_XS_PMA_Gen5_10G_MISC_CTR: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f809f));

	//p34x.usxgmii.fabric_write(0x1f809f, 0x0001)
	REG_DUMP(pdata, "6.250 VR_XS_PMA_Gen5_10G_MISC_CTR: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18090));
	//p34x.usxgmii.fabric_write(0x18090, 0x5101)
	REG_DUMP(pdata, "6.250 VR_XS_PMA_Gen5_10G_MISC_CTR: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18090));
	REG_DUMP(pdata, "6.251 VR_XS_PMA_SNPS_CR_CTRL 0x%x\n", __usxgmii_fabric_rd(pdata, 0x0180a0));

	REG_DUMP(pdata, "### RX LINK UP seen ###\n");
#if !SHORT_TRACE
	// RX adaption is not required for short trace
	__usxgmii_phy_wr(pdata, 0x1002, 0xA000);
	__usxgmii_phy_wr(pdata, 0x1003, 0x2248);
	__usxgmii_phy_wr(pdata, 0x3008, 0x1800);
#endif
	REG_DUMP(pdata, "\n###>>>USXGMII CONFIG DONE :)###\n");

	addr = 0x38006;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_EEE_MODE_CTRL before: 0x%x\n", read_val);
	addr = 0x3800b;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "CSR_EEE_MODE_CTRL1 before: 0x%x\n", read_val);

	//__usxgmii_fabric_wr(pdata, 0x10000, 0x0041); //internal loopback
	REG_DUMP(pdata, "READ 6.1_SR_PMA_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x10000));

#if 0
	//CHANGED TIME TO 0x100 to match with LGM
	REG_DUMP(pdata, "### cl37 AN Enable ###\n");
	addr = 0x1f800a;
	read_val = __usxgmii_fabric_rd(pdata, addr);
	REG_DUMP(pdata, "### SET LINK TIMER ###\n");
	REG_DUMP(pdata, "CSR_MII_LINK_TIMER before: 0x%x\n", read_val);
	__usxgmii_fabric_wr(pdata, addr, 0x100);	//originally it was 0x2
	REG_DUMP(pdata, ">>>CSR_MII_LINK_TIMER after: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));

	//ALIGNMENT TIMER CONFIG
	addr = 0x3800a;
	REG_DUMP(pdata, "CSR_VR_PCS_AM_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
	__usxgmii_fabric_wr(pdata, addr, 0x1f40);
	REG_DUMP(pdata, ">>>CSR_VR_PCS_AM_CTRL now: 0x%x\n", __usxgmii_fabric_rd(pdata, addr));
#endif

	REG_DUMP(pdata, "Slice0 Speed: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f0000));
	REG_DUMP(pdata, "Slice1 Speed: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a0000));
	REG_DUMP(pdata, "Slice2 Speed: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b0000));
	REG_DUMP(pdata, "Slice3 Speed: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c0000));

	// dump speed only
	__usxgmii_set_speed(pdata, 1);

	REG_DUMP(pdata, ">>>CSR_VR_PCS_AM_CTRL now: 0x%x\n",__usxgmii_fabric_rd(pdata, 0x3800a));

	read_val = __usxgmii_fabric_rd(pdata, 0x1f8002);	//VR_MII_AN_INTR_STS
	REG_DUMP(pdata, "ANEG reg value Port 0: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f8002));
	REG_DUMP(pdata, "ANEG Complete for Port 0: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1a8002);	//VR_MII_1_AN_INTR_STS
	REG_DUMP(pdata, "ANEG reg value Port 1: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8002));
	REG_DUMP(pdata, "ANEG Complete for Port 1: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1b8002);	//VR_MII_2_AN_INTR_STS
	REG_DUMP(pdata, "ANEG reg value Port 2: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8002));
	REG_DUMP(pdata, "ANEG Complete for Port 2: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1c8002);	//VR_MII_3_AN_INTR_STS
	REG_DUMP(pdata, "ANEG reg value Port 3: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8002));
	REG_DUMP(pdata, "ANEG Complete for Port 3: 0x%x\n", (read_val & 0x1));

	REG_DUMP(pdata, "New Register");
	REG_DUMP(pdata, "READ CSR_RAM: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1809b));
	REG_DUMP(pdata, "READ VR_PMA_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x18000));
	REG_DUMP(pdata, "READ CSR_DIG_STS: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x38010));
	REG_DUMP(pdata, "READ CSR_AN_CTRL after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x70000));
	REG_DUMP(pdata, "CSR_PCS_CTRL2 after: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x30007));

	mdelay(100);
}

#if defined(TEST_USXGMII_LB) && TEST_USXGMII_LB
static void __usxgmii_lb_en(struct p34x_priv_data *pdata)
{
	u32 read_val = 0x0;
	u32 addr = 0x10000;	//internal loopback
	int loopback_en = 1;

	if (loopback_en == 1) {	//to enable bit 0
		__usxgmii_fabric_wr(pdata, addr, 0x0041);
		dev_info(&pdata->pdev->dev, "Enabling USXGMII P34X PMA loopback...\n");
	}

	if (loopback_en == 2) {	//to disable bit 0
		__usxgmii_fabric_wr(pdata, addr, 0x400040);
		dev_info(&pdata->pdev->dev, "Disabling USXGMII P34X PMA loopback...\n");
	}

	REG_DUMP "READ 6.1_SR_PMA_CTRL: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x10000));
	REG_DUMP "ANEG complete status (VR_MII_AN_INTR_STS) ...\n");

	read_val = __usxgmii_fabric_rd(pdata, 0x1f8002); //VR_MII_AN_INTR_STS
	REG_DUMP "ANEG reg value Port 0: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1f8002));
	REG_DUMP "ANEG Complete for Port 0: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1a8002); //VR_MII_1_AN_INTR_STS
	REG_DUMP "ANEG reg value Port 1: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1a8002));
	REG_DUMP "ANEG Complete for Port 1: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1b8002); //VR_MII_2_AN_INTR_STS
	REG_DUMP "ANEG reg value Port 2: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1b8002));
	REG_DUMP "ANEG Complete for Port 2: 0x%x\n", (read_val & 0x1));

	read_val = __usxgmii_fabric_rd(pdata, 0x1c8002); //VR_MII_3_AN_INTR_STS
	REG_DUMP "ANEG reg value Port 3: 0x%x\n", __usxgmii_fabric_rd(pdata, 0x1c8002));
	REG_DUMP "ANEG Complete for Port 3: 0x%x\n", (read_val & 0x1));
}
#endif

int gpy2xx_fw_fwr_page(struct p34x_priv_data *pdata, const u8 *buf, size_t filesize)
{
	int ret = -EINVAL;
	int i = 0, j = 0;
	u8 *p;
	u16 data;
	u32 wrsize;
	u32 timeout = FW_WR_DEF_TIMEOUT;

	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD, VSPEC1_PATCH_LOAD_FAST_FLASH);
	if (ret < 0) {
		dev_err(&pdata->pdev->dev, "ERROR: Entering fast firmware download mode fails\n");
		return ret;
	}

	do {
		msleep(500);
		ret = mmd_read(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA_HIGH);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Reading erase status fails\n");
			return ret;
		}
		timeout--;
	} while (!(ret & VSPEC1_MBOX_CMD_READY_MASK) && timeout > 0);

	if (timeout == 0) {
		dev_err(&pdata->pdev->dev, "ERROR: Erase timeout\n");
		ret = -EIO;
		return ret;
	}

	while (filesize > 0) {
		wrsize = min(filesize, (size_t)256);
		ret = mmd_read(pdata, 0, MDIO_MMD_VEND2, VPSPEC2_CTL * 2);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Reading VPSPEC2_CTL fails\n");
			return ret;
		}

		data = ret & VPSPEC2_MASK;
		ret = mmd_write(pdata, 0, MDIO_MMD_VEND2, VPSPEC2_CTL * 2, data);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Write VPSPEC2_CTL fails\n");
			return ret;
		}
		for (i = 0, p = (u8 *)(buf + j*256); i < 256; i += 4, p += 4) {
			ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA, ((u32)p[0] << 8) | p[1]);

			if (ret < 0) {
				dev_err(&pdata->pdev->dev, "ERROR: Write fails\n");
				return ret;
			}

			ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD, ((u32)p[2] << 8) | p[3]);

			if (ret < 0) {
				dev_err(&pdata->pdev->dev, "ERROR: Write fails\n");
				return ret;
			}
		}

		j++;
		data |= 1;
		ret = mmd_write(pdata, 0, MDIO_MMD_VEND2, VPSPEC2_CTL * 2, data);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Write VPSPEC2_CTL fails\n");
			return ret;
		}

		timeout = 10 * FW_WR_DEF_TIMEOUT;
		do {
			msleep(100);
			ret = mmd_read(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA_HIGH);

			if (ret < 0) {
				dev_err(&pdata->pdev->dev, "ERROR: Reading page program status fails\n");
				return ret;
			}

			timeout--;
		} while (!(ret == VSPEC1_MBOX_CMD_READY_MASK) && (timeout > 0));

		if (timeout <= 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Page program timeout\n");
			ret = -EIO;
			return ret;
		}
		pr_cont("...");
		filesize -= wrsize;
		if (j % 20 == 0)
			pr_cont("\n");
	}
	pr_cont(" done\n");

	return ret;
}

static int gpy2xx_sram_fw_dl(struct p34x_priv_data *pdata, const u8 *buf,
			     size_t filesize)
{
	int ret = -EINVAL;
	u32 i = 0, hex_word;

	pr_debug("Entering func name :- %s\n", __func__);

	if (pdata == NULL) {
		pr_err("Mxl P34X %s, NULL input pointer(s)\n", __func__);
		return -EINVAL;
	}

#if defined(TEST_XMDIO_MMD) && TEST_XMDIO_MMD
	pr_info("\nBefore RST_REQ reset\n");

	ret = p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ);

	if (ret < 0)
		pr_err("RST_REQ read via smdio failed\n");

	pr_info("RST_REQ register val %x\n", ret);

	ret = p34x_reg_read(pdata, AHB_SMDIO_PDI_SMDIO_REGISTERS_SMDIO_CFG);

	if (ret < 0)
		pr_err("SMDIO_CFG read failed\n");

	pr_info("SMDIO_CFG register val %x\n", ret);
#endif

	/* Reset all slices */
	dev_dbg(&pdata->pdev->dev, "before all reset %04x\n", p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ));
	ret = p34x_reg_write(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ, 0x1778);
	dev_dbg(&pdata->pdev->dev, "after all reset %04x\n", p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ));

	if (ret < 0) {
		dev_err(&pdata->pdev->dev, "Reset MASTER slice fails\n");
		goto END;
	}

#if defined(TEST_XMDIO_MMD) && TEST_XMDIO_MMD
	pr_info("\nAfter RST_REQ reset\n");

	ret = p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ);

	if (ret < 0)
		pr_err("RST_REQ read via smdio failed\n");

	pr_info("RST_REQ register val %x\n", ret);

	ret = p34x_reg_read(pdata, AHB_SMDIO_PDI_SMDIO_REGISTERS_SMDIO_CFG);

	if (ret < 0)
		pr_err("SMDIO_CFG read failed\n");

	pr_info("SMDIO_CFG register val %x\n", ret);
#endif

	dev_info(&pdata->pdev->dev, "FW size: %zu\n", filesize);
	dev_info(&pdata->pdev->dev, "Programming ");

	/* Se the SRAM base base address to write */
	ret = __fabric_wraddr(pdata, 0x000A0000); //SRAM base

	if (ret < 0) {
		dev_err(&pdata->pdev->dev, "api __fabric_wraddr fails\n");
		goto END;
	}

	while (filesize > 0) {
		//hex_word = p34x_fw[i];
		hex_word = __le32_to_cpu(((const u32 *)buf)[i]);
		/* Write the SRAM with 4 bytes */
		ret = __fabric_wr(pdata, hex_word);

		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "api __fabric_wr fails at iteration %u\n", i);
			goto END;
		}

		if (i % 256 == 0)
			pr_cont(".");

		filesize -= 4;
		i++;
	}

	pr_cont(" done\n");

END:

	pr_debug("Exiting func name :- %s\n", __func__);
	return ret;
}

static int gpy2xx_sram_fw_dl_mbox(struct p34x_priv_data *pdata, const u8 *buf,
				  size_t filesize)
{
	u32 busyRetry = MBOX_MAX_BUSY_RETRY;
	u32 file_fw_ver = 0;
	int ret = -EINVAL;
	u32 i = 0;
	u8 *p;
	int fw_ver_polling_cnt = 5;

	pr_debug("Entering func name :- %s\n", __func__);

	if (!pdata) {
		pr_err("Mxl P34X %s, NULL input pointer(s)\n", __func__);
		return -EINVAL;
	}

	/* Init sram download by writing 0x0900 to VSPEC MBOX CMD Register */
	ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD, VSPEC1_PATCH_LOAD_FAST_SRAM);
	if (ret < 0)
		return -EIO;

	do {
		ret = mmd_read(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA_HIGH);
	} while (ret != VSPEC1_MBOX_DATA_HIGH_FWDL_MASK && (--busyRetry));
	if (ret != VSPEC1_MBOX_DATA_HIGH_FWDL_MASK) {
		dev_err(&pdata->pdev->dev,
			"VSPEC1_MBOX_DATA_HIGH unexpected: 0x%x\n", ret);
		return -EIO;
	}

	if (filesize > P34X_FW_VERSION_OFFSET + 1)
		file_fw_ver = (u32)buf[P34X_FW_VERSION_OFFSET] << 8 | buf[P34X_FW_VERSION_OFFSET + 1];

	dev_info(&pdata->pdev->dev, "FW size: %zu\n", filesize);
	dev_info(&pdata->pdev->dev, "FW version: 0x%x\n", file_fw_ver);
	dev_info(&pdata->pdev->dev, "Programming ");

	for (i = 0, p = (u8 *)buf; i < filesize; i += 4, p += 4) {
		ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_DATA, ((u32)p[1] << 8) | p[0]);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Write fails\n");
			return -EIO;
		}

		ret = mmd_write(pdata, 0, MDIO_MMD_VEND1, VSPEC1_MBOX_CMD, ((u32)p[3] << 8) | p[2]);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "ERROR: Write fails\n");
			return -EIO;
		}
		if (i % 256 == 0)
			pr_cont(".");
	}
	/* stop sram download by writing RST to STD_CTRL */
	mdio_write(pdata, 0, 0, 0x8000);
	pr_cont(" done\n");

	for (i = 0; i < fw_ver_polling_cnt; i++) {
		msleep(100);

		ret = mdio_read(pdata, 0, 0x1E);
		if ((u32)ret == file_fw_ver)
			break;
	}
	dev_info(&pdata->pdev->dev, "FW version 0x%x\n", ret);

	pr_debug("Exiting func name :- %s\n", __func__);
	return ret;
}

int gpy2xx_flash_fw_dl_mbox(struct p34x_priv_data *pdata, const u8 *buf,
				  size_t filesize)
{
	int i;
	int ret = -EINVAL;
	u32 file_fw_ver = 0;
	int fw_ver_polling_cnt = 5;

	pr_debug("Entering func name :- %s\n", __func__);

	if (filesize > P34X_FW_VERSION_OFFSET + 1)
		file_fw_ver = (u32)buf[P34X_FW_VERSION_OFFSET] << 8 | buf[P34X_FW_VERSION_OFFSET + 1];

	ret = mdio_read(pdata, 0, MDIO_FW_VERSION);
	if ((u32)ret >= file_fw_ver) {
		dev_info(&pdata->pdev->dev, "FW version: 0x%x\n", ret);
		dev_info(&pdata->pdev->dev, "No need to download p34x FW\n");
	} else {
		dev_info(&pdata->pdev->dev, "FW version: 0x%x\n", file_fw_ver);
		dev_info(&pdata->pdev->dev, "Programming ");
		ret = gpy2xx_fw_fwr_page(pdata, buf, filesize);
		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "gpy2xx_fw_fwr_page failed: %d\n", ret);
		}

		mdio_write(pdata, 0, 0, MDIO_CTL_RST);
		for (i = 0; i < fw_ver_polling_cnt; i++) {
			msleep(500);
			ret = mdio_read(pdata, 0, MDIO_FW_VERSION);
			if ((u32)ret == file_fw_ver)
				break;
		}
		if ((u32)ret != file_fw_ver)
			dev_info(&pdata->pdev->dev, "FW update failed, current version 0x%x\n", ret);
	}

	pr_debug("Exiting func name :- %s\n", __func__);
	return ret;
}

static int __release_slices(struct p34x_priv_data *pdata)
{
	int ret;
	u32 val;
	int i;

	pr_debug("Entering func name :- %s\n", __func__);

	/* Set BOOTSEL = RAM */
	ret = p34x_reg_write(pdata, AHB_TOP_FCR, 0x4000); //TOP_FCR

	if (ret < 0) {
		dev_err(&pdata->pdev->dev, "Setting TOP_FCR.BOOTSEL fails\n");
		return ret;
	}

	/* Release the master slice */
	ret = p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ);
	dev_dbg(&pdata->pdev->dev, "before release slices: %04x\n", ret);
	if (ret < 0)
		return ret;
	val = (u32)ret;
	for (i = 3; i < 4; i++) {
		val &= ~BIT(6 - i);
		ret = p34x_reg_write(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ, val);

		if (ret < 0) {
			dev_err(&pdata->pdev->dev, "Release slice %d fails\n", i);
			return ret;
		}

		udelay(10);
		dev_dbg(&pdata->pdev->dev, "after release slice %d: %04x\n", i, p34x_reg_read(pdata, AHB_RCU_PDI_REGISTERS_RST_REQ));
	}

	/* Get BOOTROM & BOOT Loader Exit Path */
	ret = p34x_reg_read(pdata, AHB_TOP_INDICATOR);

	if (ret < 0)
		dev_err(&pdata->pdev->dev, "Reading TOP_INDICATOR fails\n");
	else
		dev_info(&pdata->pdev->dev,
			 "Top indicator value - after update: %x\n", ret);

	pr_debug("Exiting func name :- %s\n", __func__);

	return 0;
}

#if defined(TEST_DLFW_READ) && TEST_DLFW_READ
static int gpy2xx_sram_fw_rd(struct p34x_priv_data *pdata)
{
	int ret = -EINVAL;
	u32 i = 0, filesize, _top_ind, hex_word;

	pr_info("Entering func name :- %s\n", __func__);

	if (!pdata) {
		pr_warning("WARN: NULL input pointer(s)\n");
		return -EINVAL;
	}

	/* Get BOOTROM & BOOT Loader Exit Path */
	ret = p34x_reg_read(pdata, AHB_TOP_INDICATOR);

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: Reading TOP_INDICATOR fails\n");
		goto END;
	}

	_top_ind = ret;

	dev_crit(&pdata->pdev->dev, "Top indicator value - before read: %x\n", _top_ind);

	filesize = 128 * 1024;

	dev_crit(&pdata->pdev->dev, "Programming ");

	/* Se the SRAM base base address to read */
	ret = __fabric_rdaddr(pdata, 0x000A0000); //SRAM base

	if (ret < 0) {
		dev_crit(&pdata->pdev->dev, "ERROR: api __fabric_rdaddr fails\n");
		goto END;
	}

	while (filesize > 0) {
		/* Read the SRAM with 4 bytes */
		ret = __fabric_rd(pdata, &hex_word);

		if (ret < 0) {
			dev_crit(&pdata->pdev->dev, "ERROR: api __fabric_wr fails at iteration %u\n", i);
			goto END;
		}

		//p34x_rd[i] = hex_word;
		pr_info("%x\n", hex_word);

		if (i % 256 == 0)
			dev_crit(&pdata->pdev->dev, ".");

		filesize -= 4;
		i++;
	}

	dev_crit(&pdata->pdev->dev, " done\n");

END:

	pr_info("Exiting func name :- %s\n", __func__);
	return ret;
}
#endif

#if !defined(TEST_INTEGRATED_FW) || !TEST_INTEGRATED_FW
static int _gpy2xx_fw_load_smdio(struct p34x_priv_data *pdata)
{
	int ret;
	struct device *dev = &pdata->pdev->dev;
	const char *pname;
	const struct firmware *fw;

	ret = p34x_reg_read(pdata, AHB_CHIPID_MPS_CHIPID_M);
	if (ret < 0) {
		dev_err(dev, "MPS_CHIPID_M read failed: %d\n", ret);
		return -EIO;
	}

	ret = FIELD_GET(GENMASK(14, 12), ret);
	dev_info(dev, "Chip Version: %d (%c-step)\n", ret,
		 ret ? 'B' : 'A');

	pname = pdata->fw_pathname[ret ? 1 : 0];
	if (!pname) {
		dev_err(dev, "missing firmware filename in device tree.\n");
		return -EINVAL;
	}

	dev_info(dev, "requesting %s\n", pname);

	ret = request_firmware(&fw, pname, dev);
	if (ret) {
		dev_err(dev, "failed to load firmware: %s\n", pname);
		return ret;
	}

	ret = gpy2xx_sram_fw_dl(pdata, fw->data, fw->size);

	release_firmware(fw);
	return ret;
}

static int _gpy2xx_fw_load_mbox(struct p34x_priv_data *pdata)
{
	int ret;
	struct device *dev = &pdata->pdev->dev;
	const char *pname;
	const struct firmware *fw;

	ret = p34x_mbox_read(pdata, AHB_CHIPID_MPS_CHIPID_M);
	if (ret < 0) {
		dev_err(dev, "MPS_CHIPID_M read failed: %d\n", ret);
		return -EIO;
	}

	ret = FIELD_GET(GENMASK(14, 12), ret);
	dev_info(dev, "Chip Version: %d (%c-step)\n", ret,
		 ret ? 'B' : 'A');

	pname = pdata->fw_pathname[ret ? 1 : 0];
	if (!pname) {
		dev_err(dev, "missing firmware filename in device tree.\n");
		return -EINVAL;
	}

	dev_info(dev, "requesting %s\n", pname);

	ret = request_firmware(&fw, pname, dev);
	if (ret) {
		dev_err(dev, "failed to load firmware: %s\n", pname);
		return ret;
	}

	ret = gpy2xx_sram_fw_dl_mbox(pdata, fw->data, fw->size);

	release_firmware(fw);
	return ret;
}

static int _gpy2xx_flash_fw_load_mbox(struct p34x_priv_data *pdata)
{
	int ret;
	struct device *dev = &pdata->pdev->dev;
	const char *pname;
	const struct firmware *fw;

	ret = p34x_mbox_read(pdata, AHB_CHIPID_MPS_CHIPID_M);
	if (ret < 0) {
		dev_err(dev, "MPS_CHIPID_M read failed: %d\n", ret);
		return -EIO;
	}

	ret = FIELD_GET(GENMASK(14, 12), ret);
	dev_info(dev, "Chip Version: %d (%c-step)\n", ret,
		 ret ? 'B' : 'A');

	pname = pdata->fw_pathname[ret ? 1 : 0];
	if (!pname) {
		dev_err(dev, "missing firmware filename in device tree.\n");
		return -EINVAL;
	}

	dev_info(dev, "requesting %s\n", pname);

	ret = request_firmware(&fw, pname, dev);
	if (ret) {
		dev_err(dev, "failed to load firmware: %s\n", pname);
		return ret;
	}

	ret = gpy2xx_flash_fw_dl_mbox(pdata, fw->data, fw->size);

	release_firmware(fw);
	return ret;
}

#endif

static irqreturn_t mdint(int irq, void *dev_id)
{
	struct p34x_priv_data *pdata = (struct p34x_priv_data *)dev_id;
	struct device *dev = &pdata->pdev->dev;
	int i;
	for (i = 0 ; i < 4; i++) {
		dev_info(dev, "interrupt status is: %04x \n", mdio_read(pdata, (u32)i, 0x1a));
        }
	/* MDINT triggering and clearing can take up to 2ms in some FW versions
	 * which can lead to interrupt flooding in level triggered mode
	 */
	udelay(5000);
	return IRQ_RETVAL(1);
}

static void create_sub_devices(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;
	struct device_node *np = NULL;
	struct platform_device *child_pdev;
	int devnum = 0;

	for_each_child_of_node(dev->of_node, np) {
		struct resource r = {0};

		child_pdev = of_find_device_by_node(np);
		if (child_pdev) {
			platform_device_put(child_pdev);
			continue;
		}

		ret = of_address_to_resource(np, 0, &r);
		if (ret < 0) {
			dev_warn(dev, "failed in getting IOMEM of sub device %s: %d\n",
				 np->full_name, ret);
			continue;
		}

		child_pdev = platform_device_alloc(np->full_name, devnum);
		if (!child_pdev) {
			dev_warn(dev, "failed in allocating sub device %s\n",
				 np->full_name);
			continue;
		}

		child_pdev->dev.parent = dev;
		child_pdev->dev.of_node = np;

		ret = platform_device_add_resources(child_pdev, &r, 1);
		if (ret) {
			dev_warn(dev, "failed in adding resource for sub device %s: %d\n",
				 np->full_name, ret);
			platform_device_put(child_pdev);
			continue;
		}

		ret = platform_device_add(child_pdev);
		if (ret) {
			dev_warn(dev, "failed in adding sub device %s: %d\n",
				 np->full_name, ret);
			platform_device_put(child_pdev);
			continue;
		}

		devnum++;
	}
}

static void p34x_reset(struct platform_device *pdev)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(&pdev->dev);
	int i;

	for (i = 0; i < ARRAY_SIZE(pdata->rst); i++) {
		if (!pdata->rst[i])
			continue;
		reset_control_assert(pdata->rst[i]);
		pr_err("reset_control_assert(%d)\n", i);
	}
	mdelay(1);
	for (i = 0; i < ARRAY_SIZE(pdata->rst); i++) {
		if (!pdata->rst[i])
			continue;
		reset_control_deassert(pdata->rst[i]);
		pr_err("reset_control_deassert(%d)\n", i);
	}
	mdelay(10);

	if (pdata->reset_gpio) {
		gpiod_set_value_cansleep(pdata->reset_gpio, 1);
		mdelay(10);
		gpiod_set_value_cansleep(pdata->reset_gpio, 0);
		mdelay(10);
	}
}

static void p34x_post_op(struct platform_device *pdev, int is_2p5g)
{
	struct p34x_priv_data *pdata = dev_get_drvdata(&pdev->dev);
	u32 addr, val, i;

	for (i = 0; i < 4; i++) {
		/* disable EEE of 1G/100M */
		pr_info("Disable EEE of 1G/100M.\n");
		mmd_write(pdata, i, MDIO_MMD_AN, MDIO_AN_EEE_ADV, 0);

		addr = MII_ADDR_C45 | (MDIO_MMD_AN << 16) | MDIO_AN_10GBT_CTRL;
		val = is_2p5g ? 0x40A2 : 0x4002;
		mmd_write(pdata, i, MDIO_MMD_AN, MDIO_AN_10GBT_CTRL, val);

#if ENABLE_POWER_DOWN
		/* power down */
		val = mdio_read(pdata, i, 0);
		val |= BIT(11);
		mdio_write(pdata, i, 0, val);
#else
		/* restart aneg */
		val = mdio_read(pdata, i, 0);
		val |= BIT(9) | BIT(12);
		mdio_write(pdata, i, 0, val);
#endif
	}
}

static void p34x_usxgmii_set(struct platform_device *pdev)
{
#if 0
	/* disable the sideband signal toggling */
#else
	struct p34x_priv_data *pdata = dev_get_drvdata(&pdev->dev);
	const u32 an_expn[4] = {0x1F0006, 0x1A0006, 0x1B0006, 0x1C0006};
	int ret;
	u32 miistat;
	u32 sel;
	u32 usxgmii;
	int timeout;
	int i, val;

	for (i = 0; i < 4; i++) {
		ret = mdio_read(pdata, (u32)i, 0x18);
		if (ret < 0) {
			pr_err("failed in reading MII_STATE of %d\n", i);
			continue;
		}
		miistat = (u32)ret;
		pr_err("slice %d MII STAT 0x%04Xn", i, miistat);

		ret = p34x_reg_read(pdata, AHB_TOP_SLICE_IF);
		if (ret < 0) {
			pr_err("failed in reading slice selection %d\n", i);
			continue;
		}
		sel = ((u32)ret & 0xFFC0) | i;
		p34x_reg_write(pdata, AHB_TOP_SLICE_IF, sel);

		ret = p34x_reg_read(pdata, AHB_SLICE_CTRL_PDI_USXGMII);
		if (ret < 0) {
			pr_err("failed in reading slice %d USXGMII sideband\n", i);
			continue;
		}
		usxgmii = (u32)ret;
		if ((ret & 0x0E) == 8) {
			ret = p34x_xpcs_rd(pdata, an_expn[i]);
			p34x_reg_write(pdata, AHB_SLICE_CTRL_PDI_USXGMII, 0x94);
			timeout = 5000;
			do {
				msleep(1);
				ret = p34x_xpcs_rd(pdata, an_expn[i]);
				if (ret > 0 && (ret & BIT(1)))
					break;
			} while (--timeout > 0);
			msleep(10);
			p34x_reg_write(pdata, AHB_TOP_SLICE_IF, sel);
			p34x_reg_write(pdata, AHB_SLICE_CTRL_PDI_USXGMII, 0x99);
			/* restart aneg */
			val = mdio_read(pdata, i, 0);
			val |= BIT(9) | BIT(12);
			mdio_write(pdata, i, 0, val);
			pr_info("slice %d timeout %d\n", i, timeout);
		}
		pr_info("slide %d USXGMII sideband 0x%04X\n", i, usxgmii);
	}
#endif
}

#if (IS_ENABLED(CONFIG_HWMON))
static int p34x_temp_trend_init(struct p34x_priv_data *pdata)
{
	if (!pdata)
		return -EINVAL;

	INIT_LIST_HEAD(&pdata->t_trend.dataset);

	pdata->t_trend.ds_size = P34X_TEMP_TREND_DS_SIZE;
	pdata->t_trend.ds_cnt = 0;
	pdata->t_trend.tround = P34X_TEMP_TREND_TEMP_ROUNDUP;

	spin_lock_init(&pdata->t_trend.lock);

	return 0;
}

static void p34x_temp_trend_uninit(struct p34x_priv_data *pdata)
{
	struct p34x_temp_trend_ds_entry *entry, *n;

	if (!pdata)
		return;

	list_for_each_entry_safe(entry, n, &pdata->t_trend.dataset, list) {
		list_del(&entry->list);
		kfree(entry);
	}
}

static umode_t p34x_is_visible(const void *data, enum hwmon_sensor_types type,
			       u32 attr, int channel)
{
	switch (type) {
	case hwmon_temp:
		if (attr == hwmon_temp_input)
			return S_IRUGO;
		else
			return 0;
	default:
		return 0;
	}
}

static int p34x_read(struct device *dev, enum hwmon_sensor_types type, u32 attr,
		     int channel, long *val)
{
	struct p34x_priv_data *pdata;
	struct p34x_temp_trend_data *trend;
	struct p34x_temp_trend_ds_entry *entry = NULL, *first;
	int ret;
	u32 pvt_data;
	size_t i;

	if (!dev || !val)
		return -EINVAL;

	if (type != hwmon_temp || attr != hwmon_temp_input)
		return -EOPNOTSUPP;

	pdata = dev_get_drvdata(dev);
	for (i = 0; i < 4; i++) {
		ret = mmd_read(pdata, i, MDIO_MMD_VEND1, 0x0E);
		if (ret == 0xFFFF) {
			dev_warn(dev, "Read 0xFFFF from P34X temperature register. MDIO access may have problem.\n");
			continue;
		}
		if (ret > 0 && (ret & 0x3FF))
			break;
	}

	if (i == 4)
		return -EIO;

	pvt_data = (u32)ret & 0x3FF;
	{
		/* Temperature interpolation */
		/* Y = f(x) = a4*(x^4) + a3*(x^3) + a2*(x^2) + a1*x + a0 */
		s64 a4 = -258;
		s64 a3 = 973000;
		s64 a2 = -1920000000;
		s64 a1 = 3080000000000;
		s64 a0 = -520000000000000;
		s64 t4;
		s64 t3;
		s64 t2;
		s64 t1;

		t1 = a1 * pvt_data;
		t2 = a2 * pvt_data * pvt_data;
		t3 = a3 * pvt_data * pvt_data * pvt_data;
		t4 = a4 * pvt_data * pvt_data * pvt_data * pvt_data;
		*val = (t4 + t3 + t2 + t1 + a0) / 10000000000; /* returns °mC */
	}

	trend = &pdata->t_trend; /* temp trend data */

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	/* round-up temp value to tround factor */
	if ((*val % trend->tround) == 0)
		entry->temp = *val;
	else
		entry->temp = *val + trend->tround - (*val % trend->tround);

	INIT_LIST_HEAD(&entry->list);

	spin_lock(&trend->lock);

	if (trend->ds_cnt == trend->ds_size) {
		/* Remove data entries above dataset size. In case the user
		 * changes dataset size in runtime
		 */
		while (trend->ds_cnt >= trend->ds_size) {
			first = list_first_entry(&trend->dataset,
						 struct p34x_temp_trend_ds_entry,
						 list);
			list_del(&first->list);
			kfree(first);
			trend->ds_cnt--;
		}
	}

	dev_dbg(dev, "New dataset entry %ld (ds_size %d ds_cnt %d)\n",
		entry->temp, trend->ds_size, trend->ds_cnt);

	list_add_tail(&entry->list, &trend->dataset);
	trend->ds_cnt++;

	spin_unlock(&trend->lock);

	return 0;
}

#if ENABLE_MAXLINEAR_HWMON
static int p34x_get_trend(struct device *dev, enum hwmon_sensor_types type,
			  u32 attr, int channel, long *val)
{
	struct p34x_priv_data *pdata;
	struct p34x_temp_trend_data *trend;

	s64 a /*intercept*/, b /*slope*/;
	u32 sum_x = 0, sum_xsq = 0;
	s64 sum_y = 0, sum_xy = 0;

	struct p34x_temp_trend_ds_entry *entry;
	int i = 0; /* size in elements of the dataset */
	long t_low, t_high; /* interpolated values for the edge
			     *  dataset temperatures
			     */

	if (!dev || !val)
		return -EINVAL;

	if (type != hwmon_temp || attr != hwmon_temp_input)
		return -EOPNOTSUPP;

	pdata = dev_get_drvdata(dev);
	trend = &pdata->t_trend;

	/* Fore each dataset entry calculate necessary sums */
	list_for_each_entry(entry, &trend->dataset, list) {
		i++;
		sum_y += entry->temp;
		sum_x += i;
		sum_xsq += i * i;
		sum_xy += entry->temp * i;
	}

	if (i > 2) { /* we need at least 2 entries in dataset */
		a = ((sum_y * sum_xsq) - (sum_x * sum_xy)) /
		    ((i * sum_xsq) - (sum_x * sum_x));
		b = ((i * sum_xy) - (sum_x * sum_y)) /
		    ((i * sum_xsq) - (sum_x * sum_x));

		t_low = a + (b * 1);
		t_high = a + (b * i);

		if (t_high > t_low)
			*val = THERMAL_TREND_RAISING;
		else if (t_high < t_low)
			*val = THERMAL_TREND_DROPPING;
		else
			*val = THERMAL_TREND_STABLE;

		dev_dbg(dev, "Trend %ld (a %lld b %lld t_low %ld t_high %ld)",
			*val, a, b, t_low, t_high);
	} else {
		*val = THERMAL_TREND_STABLE;
	}

	return 0;
}
#endif

static const struct hwmon_ops p34x_hwmon_ops = {
	.is_visible = p34x_is_visible,
	.read = p34x_read,
#if ENABLE_MAXLINEAR_HWMON
	.get_trend = p34x_get_trend,
#endif
};

static const u32 p34x_temp_config[] = {
	HWMON_T_INPUT,
	0
};

static const struct hwmon_channel_info p34x_temp = {
	.type = hwmon_temp,
	.config = p34x_temp_config,
};

static const u32 p34x_chip_config[] = {
	HWMON_C_REGISTER_TZ,
	0
};

static const struct hwmon_channel_info p34x_chip = {
	.type = hwmon_chip,
	.config = p34x_chip_config,
};

static const struct hwmon_channel_info *p34x_hwmon_chs[] = {
	&p34x_chip,
	&p34x_temp,
	NULL
};

static struct hwmon_chip_info p34x_hwmon_chip = {
	.ops = &p34x_hwmon_ops,
	.info = p34x_hwmon_chs
};
#endif

static void p34x_post_fwdl(struct platform_device *pdev)
{
	struct p34x_priv_data *pdata = NULL;
	struct phy_device *phydev = NULL;
	int j;

	if (!pdev) {
		pr_err("NULL input pointer(s)\n");
		return;
	}
	pdata = (struct p34x_priv_data *)platform_get_drvdata(pdev);

	p34x_post_op(pdev, 1);

	create_sub_devices(pdev);

	for (j = 0; j < 4; j++) {
		phydev = mdiobus_scan(pdata->miibus, j);
		if (!IS_ERR(phydev))
			phy_device_free(phydev);
	}
	p34x_ptp_init(pdata);
}

/* To ensure succssful USXGMII auto negotiation, P34X need to be reset
 * after lan_xpcs writes sram EXT_LD_DN bit to 1. As RCU HRST will reset
 * other components and using GPIO to reset P34X is board specific, writing
 * 0x30DA to register 1e.08 is used to reset P34X.
 */
int p34x_usxgmii_reset(struct platform_device *pdev)
{
	struct p34x_priv_data *pdata = NULL;
	int ret = 0;

	if (!pdev) {
		pr_err("NULL input pointer(s)\n");
		return -EINVAL;
	}
	pdata = platform_get_drvdata(pdev);

	ret = mmd_write(pdata, 0, 0x1e, 0x8, 0x30da);
	if (ret < 0) {
		dev_err(&pdev->dev, "P34X USXGMII reset failed: %d\n", ret);
		return ret;
	}

	pr_debug("P34X USXGMII reset done\n");

	return ret;
}

static int p34x_fwdl_mbox(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev;
	struct p34x_priv_data *pdata = NULL;

	if (!pdev) {
		pr_err("NULL input pointer(s)\n");
		return -EINVAL;
	}
	dev = &pdev->dev;
	pdata = (struct p34x_priv_data *)platform_get_drvdata(pdev);

	/* Call the FW downloader into SRAM */
	ret = _gpy2xx_fw_load_mbox(pdata);
	if (ret < 0) {
		dev_err(dev, "FW download into SRAM failed: %d\n", ret);
		return ret;
	}

	return ret;
}

static int p34x_fwdl_flash_mbox(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev;
	struct p34x_priv_data *pdata = NULL;

	if (!pdev) {
		pr_err("NULL input pointer(s)\n");
		return -EINVAL;
	}
	dev = &pdev->dev;
	pdata = (struct p34x_priv_data *)platform_get_drvdata(pdev);

	/* Call the FW downloader into FLASH */
	ret = _gpy2xx_flash_fw_load_mbox(pdata);
	if (ret < 0) {
		dev_err(dev, "FW download into SRAM failed: %d\n", ret);
		return ret;
	}

	return ret;
}

static int p34x_fwdl_smdio(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev;
	struct p34x_priv_data *pdata = NULL;
	struct phy_device *phydev = NULL;
	int j;

	if (!pdev) {
		pr_err("NULL input pointer(s)\n");
		return -EINVAL;
	}
	dev = &pdev->dev;
	pdata = (struct p34x_priv_data *)platform_get_drvdata(pdev);

	ret = p34x_reg_read(pdata, AHB_SMDIO_PDI_SMDIO_REGISTERS_SMDIO_CFG);
	if (ret <= 0 || ret == 0xFFFF) {
		dev_err(dev, "SMDIO_CFG read failed: %d\n", ret);
		return -EIO;
	} else
		dev_info(dev, "SMDIO_CFG is 0x%04x\n", ret);

	ret = device_property_read_u32(dev, "rateadaptation", &pdata->rate_adaptation);
	if (ret) {
		dev_warn(dev, "Can not get rateadaptation setting, considered as rateadatation is disabled: %d\n", ret);
		pdata->rate_adaptation = 0;
	}
	dev_info(dev, "rateadaptation is %u\n", pdata->rate_adaptation);

	if (pdata->rate_adaptation) {
		ret = mmd_read(pdata, 0, 0x1e, 0x8);
		if (ret < 0) {
			dev_err(dev, "VSPEC1_SGMII_CTRL read failed: %d\n", ret);

		} else {
			dev_info(dev, "VSPEC1_SGMII_CTRL read register %x\n", ret);
			ret &= 0xffdf;
			ret |= 0x1 << 5;
			mmd_write(pdata, 0, 0x1e, 0x8, ret);

			ret = mmd_read(pdata, 0, 0x1e, 0x8);
			if (ret & (0x1 << 5))
				dev_info(dev, "p34x rate adatation is enabled for Slice 0 ,VSPEC1_SGMII_CTRL read register %x\n", ret);
			else
				dev_info(dev, "p34x rate adatation is not enabled for Slice 0 ,VSPEC1_SGMII_CTRL read register %x\n", ret);
		}
	}

#if defined(TEST_XMDIO_MMD) && TEST_XMDIO_MMD
	dev_info(dev, "\nBefore calling fw-dl\n");

	ret = mdio_read(pdata, 0, 0x1E);

	if (ret < 0)
		dev_err(dev, "FW version read failed: %d\n", ret);

	dev_info(dev, "\nFW version %x\n", ret);

	dev_info(dev, "SMDIO_CFG register val %x\n", ret);
	if (ret != 0x1F5) {
		dev_err(dev, "failed in reading SMDIO, force quit\n");
		pr_debug("Exiting %s\n", __FUNCTION__);
		return 0;
	}

	ret = mmd_read(pdata, 0, 0x1e, 0);

	if (ret < 0)
		dev_err(dev, "VSPEC1_LEDC read failed: %d\n", ret);

	dev_info(dev, "VSPEC1_LEDC read register %x\n", ret);

	ret = mmd_read(pdata, 0, 0x1e, 1);

	if (ret < 0)
		dev_err(dev, "VSPEC1_LED0 read failed: %d\n", ret);

	dev_info(dev, "VSPEC1_LED0 read register %x\n", ret);
#endif

	/* Call the FW downloader into SRAM */
#if defined(TEST_INTEGRATED_FW) && TEST_INTEGRATED_FW
	ret = gpy2xx_sram_fw_dl(pdata, (const u8 *)p34x_fw, sizeof(p34x_fw));
#else
	ret = _gpy2xx_fw_load_smdio(pdata);
#endif
	if (ret < 0) {
		dev_err(dev, "FW download into SRAM failed: %d\n", ret);
		return ret;
	}

#if defined(TEST_DLFW_READ) && TEST_DLFW_READ
	/* Call the FW read from SRAM */
	ret = gpy2xx_sram_fw_rd(pdata);

	if (ret < 0)
		dev_err(dev, "FW read from SRAM failed\n");

#endif

	__usxgmii_en(pdata);

#if defined(TEST_USXGMII_LB) && TEST_USXGMII_LB
	__usxgmii_lb_en(pdata);
#endif

	__release_slices(pdata);

	ret = mdio_read(pdata, 0, 0x1E);

	if (ret < 0)
		dev_err(dev, "FW version read failed: %d\n", ret);
	else {
		dev_info(dev, "FW version %x\n", ret);
		p34x_post_op(pdev, 1);
	}

#if defined(TEST_XMDIO_MMD) && TEST_XMDIO_MMD
	ret = p34x_reg_read(pdata, AHB_SMDIO_PDI_SMDIO_REGISTERS_SMDIO_CFG);

	if (ret < 0)
		dev_err(dev, "SMDIO_CFG read via smdio failed: %d\n", ret);

	dev_info(dev, "SMDIO_CFG register val %x\n", ret);

	ret = mmd_read(pdata, 0, 0x1e, 0);

	if (ret < 0)
		dev_err(dev, "VSPEC1_LEDC read failed: %d\n", ret);

	dev_info(dev, "VSPEC1_LEDC read register %x\n", ret);

	ret = mmd_read(pdata, 0, 0x1e, 1);

	if (ret < 0)
		dev_err(dev, "VSPEC1_LED0 read failed: %d\n", ret);

	dev_info(dev, "VSPEC1_LED0 read register %x\n", ret);

	ret = mmd_read(pdata, 0, 0x1e, 0x2);

	if (ret < 0)
		dev_err(dev, "VSPEC1_LED1 read failed: %d\n", ret);

	dev_info(dev, "VSPEC1_LED1 value %x\n", ret);

	ret = mmd_read(pdata, 0, 0x1f, 0x0);

	if (ret < 0)
		dev_err(dev, "VSPEC2-0 read failed: %d\n", ret);

	dev_info(dev, "VSPEC2-0 value %x\n", ret);
#endif

	create_sub_devices(pdev);

	for (j = 0; j < 4; j++) {
		phydev = mdiobus_scan(pdata->miibus, j);
		if (!IS_ERR(phydev))
			phy_device_free(phydev);
	}

	p34x_ptp_init(pdata);

	p34x_sysfs_init(pdata);

#if (IS_ENABLED(CONFIG_HWMON))
	ret = p34x_temp_trend_init(pdata);
	if (ret < 0)
		dev_err(dev, "failed to init temp trend data (ret %d)\n", ret);

	pdata->hwmon = devm_hwmon_device_register_with_info(dev,
							    "p34x",
							    pdata,
							    &p34x_hwmon_chip,
							    NULL);
	if (IS_ERR(pdata->hwmon))
		dev_err(dev, "failed in register temperature sensor: %p, %ld\n",
			pdata->hwmon, PTR_ERR(pdata->hwmon));
#endif

	msleep(100);
	p34x_usxgmii_set(pdev);
	for (j = 0; j < 4; j++)
		mmd_write(pdata, j, MDIO_MMD_AN, MDIO_AN_EEE_ADV2, MDIO_EEE_2_5GT);

	return ret;
}

void p34x_mb_modify(struct p34x_priv_data *pdata, u32 mb_addr, u32 bit_pos, u32 bit_value)
{
	int data;
	data = p34x_mbox_read(pdata, mb_addr);

	data &= ~(1 << bit_pos);
	data |= (bit_value << bit_pos);

	p34x_mbox_write(pdata, mb_addr, data);
}

void p34x_set_voltage(struct p34x_priv_data *pdata)
{
	int gpio2_val = 0;
	int gpio26_val = 0;
	int pvt_value = 0;

	pvt_value = p34x_mbox_read(pdata, PHY_REG_PVT_ADDR);

	/*
		Case 	PVT Code	 Core Voltage 	 GPIO2/GPIO26
		1		< = 301			0.85V			0 / 0
		2		302-324			0.90V			0 / 1
		3		325-346			0.95V			1 / 0
		4		> 346			1.00V			1 / 1
	*/
	if (pvt_value <= 301) {
		gpio2_val = 0;
		gpio26_val = 0;
	} else if ((pvt_value >= 302) && (pvt_value <= 324)) {
		gpio2_val = 0;
		gpio26_val = 1;
	} else if ((pvt_value >= 325) && (pvt_value <= 346)) {
		gpio2_val = 1;
		gpio26_val = 0;
	} else {
		gpio2_val = 1;
		gpio26_val = 1;
	}

	/*set p34x gpio26*/
	p34x_mb_modify(pdata, PHY_REG_GPIO2_ALTSEL1, 10, 0);
	p34x_mb_modify(pdata, PHY_REG_GPIO2_ALTSEL0, 10, 0);
	p34x_mb_modify(pdata, PHY_REG_GPIO2_DIR, 10, 1);
	p34x_mb_modify(pdata, PHY_REG_GPIO2_OUT, 10, gpio26_val);
	/*set p34x gpio2*/
	p34x_mb_modify(pdata, PHY_REG_GPIO_ALTSEL1, 2, 0);
	p34x_mb_modify(pdata, PHY_REG_GPIO_ALTSEL0, 2, 0);
	p34x_mb_modify(pdata, PHY_REG_GPIO_DIR, 2, 1);
	p34x_mb_modify(pdata, PHY_REG_GPIO_OUT, 2, gpio2_val);
}

static int p34x_fwdl_probe(struct platform_device *pdev)
{
	int ret;

	struct device *dev;
	struct device_node *mdiobus;
	struct p34x_priv_data *pdata = NULL;
	int chip_id_cfg;
	size_t i;

	pr_debug("Entering %s\n", __FUNCTION__);

	if (!pdev) {
		pr_err("NULL input pointer(s)\n");
		return -EINVAL;
	}

	dev = &pdev->dev;

	/* Create the driver data here */
	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);

	if (!pdata) {
		dev_err(dev, "falied in allocating private data\n");
		return -ENOMEM;
	}

	/* Set this driver data in platform device structure */
	pdata->pdev = pdev;
	pdata->ops.phy_reset = p34x_usxgmii_reset;
	platform_set_drvdata(pdev, pdata);

	pdata->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(pdata->clk)) {
		dev_err(dev, "failed in getting clock: %ld\n",
			PTR_ERR(pdata->clk));
		return PTR_ERR(pdata->clk);
	}

	clk_prepare_enable(pdata->clk);
	ret = clk_set_rate(pdata->clk, 25000000);
	if (ret) {
		dev_err(dev, "failed in set 25MHz clock: %d\n", ret);
		return ret;
	}

	for (i = 0; i < ARRAY_SIZE(pdata->rst); i++) {
		pdata->rst[i] = devm_reset_control_get_by_index(dev, i);
		if (IS_ERR(pdata->rst[i]))
			pdata->rst[i] = NULL;
	}

	pdata->reset_gpio = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR_OR_NULL(pdata->reset_gpio)) {
		dev_dbg(dev, "No reset gpio for p34x\n");
		pdata->reset_gpio = NULL;
	}

	p34x_reset(pdev);

	mdiobus = of_parse_phandle(dev->of_node, "mdio", 0);

	if (!mdiobus) {
		dev_err(dev, "No mdio node defined in DTS\n");
		return -EINVAL;
	}

	pdata->miibus = of_mdio_find_bus(mdiobus);

	if (!pdata->miibus) {
		dev_err(dev, "Can't find mii-bus\n");
		return -EINVAL;
	}

	dev_info(dev, "Found MDIO bus \"%s\" ref\n", pdata->miibus->name);

	ret = device_property_read_u32(dev, "smdio", &pdata->smdio_addr);
	if (ret) {
		dev_warn(dev, "Can not get SMDIO address, use 0x1F instead: %d\n", ret);
		pdata->smdio_addr = 0x1F;
	}
	dev_info(dev, "SMDIO address is 0x%02x\n", pdata->smdio_addr);

	ret = device_property_read_u32(dev, "phyaddr", &pdata->phy_addr);
	if (ret) {
		dev_warn(dev, "Can not get PHY address of slice 0, use 0 instead: %d\n", ret);
		pdata->phy_addr = 0;
	}
	dev_info(dev, "PHY address of slice 0 is %u\n", pdata->phy_addr);

	chip_id_cfg = smdio_read(pdata, AHB_TO_PDI(AHB_CHIP_ID_CFG));
	if (chip_id_cfg <= 0 || chip_id_cfg == 0xFFFF) {
		/* SMDIO interface fails to read CHIP_ID_CFG.
		 * It may be MXL86249. Use MBOX to read CHIP_ID_CFG.
		 */
		ret = p34x_mbox_patch_fail(pdata);
		if (ret < 0)
			dev_err(dev, "Mbox PATCH fail is not 1\n");

		chip_id_cfg = p34x_mbox_read(pdata, AHB_CHIP_ID_CFG);
		if (chip_id_cfg < 0 || chip_id_cfg == 0xFFFF)
			dev_err(dev, "CHIP_ID_CFG read failed: %d\n", chip_id_cfg);
	}
	dev_info(dev, "chip_id_cfg: 0x%x\n", chip_id_cfg);

	switch (chip_id_cfg) {
	case CHIP_ID_CFG_MCM:
	case CHIP_ID_CFG_MXL86249:
	case CHIP_ID_CFG_MXL86249C:
		p34x_reg_read = p34x_mbox_read;
		p34x_reg_write = p34x_mbox_write;
		p34x_set_fw_pathname(pdata, P34X_FW_SIGNED);
		p34x_fwdl_mbox(pdev);
		p34x_post_fwdl(pdev);
		break;
	case CHIP_ID_CFG_GPY241:
		p34x_reg_read = p34x_mbox_read;
		p34x_reg_write = p34x_mbox_write;
		p34x_set_fw_pathname(pdata, P34X_FW_SIGNED);
		p34x_fwdl_flash_mbox(pdev);
		p34x_post_fwdl(pdev);
		g241_sysfs_init(pdata);
		p34x_set_voltage(pdata);
#if (IS_ENABLED(CONFIG_HWMON))
		ret = p34x_temp_trend_init(pdata);
		if (ret < 0)
			dev_err(dev, "failed to init temp trend data (ret %d)\n", ret);
		pdata->hwmon = devm_hwmon_device_register_with_info(dev,
									"p34x",
									pdata,
									&p34x_hwmon_chip,
									NULL);
		if (IS_ERR(pdata->hwmon))
			dev_err(dev, "failed in register temperature sensor: %p, %ld\n",
				pdata->hwmon, PTR_ERR(pdata->hwmon));
#endif
		break;
	case CHIP_ID_CFG_LEGACY_MCM:
	default:
		p34x_reg_read = p34x_smdio_read;
		p34x_reg_write = p34x_smdio_write;
		p34x_set_fw_pathname(pdata, P34X_FW_UNSIGNED);
		p34x_fwdl_smdio(pdev);
		break;
	}

	ret = of_irq_get(dev->of_node, 0);
	if (ret == -EPROBE_DEFER) {
		dev_info(dev, "of_irq_get returns -EPROBE_DEFER\n");
		return ret;
	} else if (ret <= 0)
		pdata->irqnum = 0;
	else {
		pdata->irqnum = (unsigned int)ret;
		ret = devm_request_threaded_irq(dev, pdata->irqnum,
								NULL,
								mdint,
								IRQF_ONESHOT,
								"MDINT",
								pdata);
		if (ret) {
			dev_err(dev, "failed in requesting IRQ: %d\n", ret);
			return ret;
		}
	}
	dev_info(dev, "IRQ number %d\n", pdata->irqnum);

	pr_debug("Exiting %s\n", __FUNCTION__);
	return 0;
}

static int p34x_fwdl_remove(struct platform_device *pdev)
{
	struct p34x_priv_data *pdata;

	pr_debug("Entering %s\n", __FUNCTION__);

	pdata =  platform_get_drvdata(pdev);

	p34x_ptp_uninit(pdata);

	if (pdata->irqnum) {
		disable_irq(pdata->irqnum);
		devm_free_irq(&pdata->pdev->dev, pdata->irqnum, pdata);
	}

#if (IS_ENABLED(CONFIG_HWMON))
	p34x_temp_trend_uninit(pdata);
#endif

	pr_debug("Exiting %s\n", __FUNCTION__);
	return 0;
}

static struct of_device_id of_p34x_fwdl_dev_match[] = {
	{ .compatible = P34X_COMPATIBLE, },
	{},
};
MODULE_DEVICE_TABLE(of, of_p34x_fwdl_dev_match);

static struct platform_driver p34x_fwdl_driver = {
	.probe = p34x_fwdl_probe,
	.remove = p34x_fwdl_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_p34x_fwdl_dev_match,
	},
};

module_platform_driver(p34x_fwdl_driver);

MODULE_AUTHOR("Govindaiah Mudepalli");
MODULE_DESCRIPTION("Maxlinear P34X Quad-PHY FW download driver.");
MODULE_LICENSE("GPL v2");
