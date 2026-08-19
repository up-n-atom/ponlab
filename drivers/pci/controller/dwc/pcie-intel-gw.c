// SPDX-License-Identifier: GPL-2.0
/*
 * PCIe host controller driver for Intel Gateway SoCs
 *
 * Copyright (c) 2019 Intel Corporation.
 * Copyright (c) 2022 MaxLinear
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/gpio/consumer.h>
#include <linux/iopoll.h>
#include <linux/pci_regs.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/reset.h>

#include "../../pci.h"
#include "pcie-designware.h"

/* FTS is the number of Fast Training Sequences.
 * Both PCIe partners advertise this value in the TS1 sequence
 * to let the other partner know what the expected training sequence number is.
 * The training sequence assures bit lock and symbol lock.
 * The value depends on the hardware design. it works for URX and PRX.
 */
#define PORT_AFR_N_FTS_GEN12_DFT	210
#define PORT_AFR_N_FTS_GEN3		190
#define PORT_AFR_N_FTS_GEN4		196

/* PCIe RC Configuration Register */
#define PCIE_RC_VDID			0x00

/* PCIe Application logic Registers */
#define PCIE_APP_CCR			0x10
#define PCIE_APP_CCR_LTSSM_ENABLE	BIT(0)

#define PCIE_APP_MSG_CR			0x30
#define PCIE_APP_MSG_XMT_PM_TURNOFF	BIT(0)

#define PCIE_APP_PMC			0x44
#define PCIE_APP_PMC_IN_L2		BIT(20)

#define PCIE_APP_IRNEN			0xF4
#define PCIE_APP_IRNCR			0xF8
#define PCIE_APP_IRN_AER_REPORT		BIT(0)
#define PCIE_APP_IRN_PME		BIT(2)
#define PCIE_APP_IRN_HOTPLUG		BIT(3)
#define PCIE_APP_IRN_RX_VDM_MSG		BIT(4)
#define PCIE_APP_IRN_FRS		BIT(5)
#define PCIE_APP_IRN_CFG_DRS		BIT(6)
#define PCIE_APP_IRN_DRS_FRS		BIT(7)
#define PCIE_APP_IRN_RBAR_CTRL_UPDATE	BIT(8)
#define PCIE_APP_IRN_PM_TO_ACK		BIT(9)
#define PCIE_APP_IRN_PM_TURNOFF_ACK	BIT(10)
#define PCIE_APP_IRN_LINK_AUTO_BW_STAT	BIT(11)
#define PCIE_APP_IRN_BW_MGT		BIT(12)
#define PCIE_APP_IRN_INTA		BIT(13)
#define PCIE_APP_IRN_INTB		BIT(14)
#define PCIE_APP_IRN_INTC		BIT(15)
#define PCIE_APP_IRN_INTD		BIT(16)
#define PCIE_APP_IRN_WAKEUP		BIT(17)
#define PCIE_APP_IRN_MSG_LTR		BIT(18)
#define PCIE_APP_IRN_REQ_RST		BIT(26) /* Link down from EP */
#define PCIE_APP_IRN_SYS_INT		BIT(28)
#define PCIE_APP_IRN_SYS_ERR_RC		BIT(29)
#define PCIE_APP_INTX_OFST		12

#define PCIE_APP_IRN_INT \
	(PCIE_APP_IRN_AER_REPORT | PCIE_APP_IRN_PME | \
	PCIE_APP_IRN_RX_VDM_MSG | PCIE_APP_IRN_SYS_ERR_RC | \
	PCIE_APP_IRN_PM_TO_ACK | PCIE_APP_IRN_MSG_LTR | \
	PCIE_APP_IRN_BW_MGT | PCIE_APP_IRN_LINK_AUTO_BW_STAT | \
	PCIE_APP_IRN_INTA | PCIE_APP_IRN_INTB | PCIE_APP_IRN_INTC | \
	PCIE_APP_IRN_INTD | PCIE_APP_IRN_REQ_RST | PCIE_APP_IRN_SYS_ERR_RC)

#define BUS_IATU_OFFSET			SZ_256M
#define RESET_INTERVAL_MS		100

#define PCIE_ATU_IB_WINDOWS		8

enum {
	PCIE_LINK_SPEED_AUTO = 0,
	PCIE_LINK_SPEED_GEN1,
	PCIE_LINK_SPEED_GEN2,
	PCIE_LINK_SPEED_GEN3,
	PCIE_LINK_SPEED_GEN4,
};

typedef enum {
	PCIE_CELLULAR_MODULE_NONE,
	PCIE_CELLULAR_MODULE_RM502Q_AE,
	PCIE_CELLULAR_MODULE_RM520N_GL,
} CELLULAR_MODULE_MODEL;

struct intel_pcie_soc {
	unsigned int	pcie_ver;
};

struct ib_region {
	phys_addr_t	base;
	size_t		size;
};

struct intel_pcie_port {
	struct dw_pcie		pci;
	void __iomem		*app_base;
	struct gpio_desc	*reset_gpio;
	u32			rst_intrvl;
	struct clk		*core_clk;
	struct reset_control	*core_rst;
	struct phy		*phy;
	bool			has_bus_iatu;
	char			irq_name[16];
	int			id;
	/* ib_window_map is shared resource */
	DECLARE_BITMAP(ib_window_map, PCIE_ATU_IB_WINDOWS);
	spinlock_t		ib_window_lock; /* inbound window cfg lock */
	struct ib_region	region[PCIE_ATU_IB_WINDOWS];
	u32			max_speed;
	u32			link_gen;
	u32			max_width;
	u32			n_fts;
	CELLULAR_MODULE_MODEL   cellular_module;
	struct gpio_desc        *module_reset_gpio;
	struct gpio_desc        *module_power_on;

};

static void pcie_update_bits(void __iomem *base, u32 ofs, u32 mask, u32 val)
{
	u32 old;


	old = readl(base + ofs);
	val = (old & ~mask) | (val & mask);

	if (val != old)
		writel(val, base + ofs);
}

static inline u32 pcie_app_rd(struct intel_pcie_port *lpp, u32 ofs)
{
	return readl(lpp->app_base + ofs);
}

static inline void pcie_app_wr(struct intel_pcie_port *lpp, u32 ofs, u32 val)
{
	writel(val, lpp->app_base + ofs);
}

static void pcie_app_wr_mask(struct intel_pcie_port *lpp, u32 ofs,
			     u32 mask, u32 val)
{
	pcie_update_bits(lpp->app_base, ofs, mask, val);
}

static inline u32 pcie_rc_cfg_rd(struct intel_pcie_port *lpp, u32 ofs)
{
	return dw_pcie_readl_dbi(&lpp->pci, ofs);
}

static inline void pcie_rc_cfg_wr(struct intel_pcie_port *lpp, u32 ofs, u32 val)
{
	dw_pcie_writel_dbi(&lpp->pci, ofs, val);
}

static void pcie_rc_cfg_wr_mask(struct intel_pcie_port *lpp, u32 ofs,
				u32 mask, u32 val)
{
	pcie_update_bits(lpp->pci.dbi_base, ofs, mask, val);
}

static void intel_pcie_ltssm_enable(struct intel_pcie_port *lpp)
{
	pcie_app_wr_mask(lpp, PCIE_APP_CCR, PCIE_APP_CCR_LTSSM_ENABLE,
			 PCIE_APP_CCR_LTSSM_ENABLE);
}

static void intel_pcie_ltssm_disable(struct intel_pcie_port *lpp)
{
	pcie_app_wr_mask(lpp, PCIE_APP_CCR, PCIE_APP_CCR_LTSSM_ENABLE, 0);
}

static void intel_pcie_vendor_id_overwrite(struct intel_pcie_port *lpp)
{
	pcie_rc_cfg_wr_mask(lpp, PCIE_MISC_CONTROL_1_OFF,
			    PCIE_DBI_RO_WR_EN,
			    PCIE_DBI_RO_WR_EN);
	pcie_rc_cfg_wr(lpp, PCIE_RC_VDID, 0x008017e6);
	pcie_rc_cfg_wr_mask(lpp, PCIE_MISC_CONTROL_1_OFF,
			    PCIE_DBI_RO_WR_EN, 0);
}

static void intel_pcie_link_setup(struct intel_pcie_port *lpp)
{
	u32 val;
	u8 offset = dw_pcie_find_capability(&lpp->pci, PCI_CAP_ID_EXP);

	val = pcie_rc_cfg_rd(lpp, offset + PCI_EXP_LNKCAP);
	lpp->max_speed = FIELD_GET(PCI_EXP_LNKCAP_SLS, val);
	lpp->max_width = FIELD_GET(PCI_EXP_LNKCAP_MLW, val);

	val |= PCI_EXP_LNKCAP_ASPMS;
	pcie_rc_cfg_wr_mask(lpp, PCIE_MISC_CONTROL_1_OFF,
			    PCIE_DBI_RO_WR_EN,
			    PCIE_DBI_RO_WR_EN);
	pcie_rc_cfg_wr(lpp, offset + PCI_EXP_LNKCAP, val);
	pcie_rc_cfg_wr_mask(lpp, PCIE_MISC_CONTROL_1_OFF,
			    PCIE_DBI_RO_WR_EN, 0);

	val = pcie_rc_cfg_rd(lpp, offset + PCI_EXP_LNKCTL);
	val &= ~(PCI_EXP_LNKCTL_LD | PCI_EXP_LNKCTL_ASPMC);
	pcie_rc_cfg_wr(lpp, offset + PCI_EXP_LNKCTL, val);
}

static void intel_pcie_init_n_fts(struct dw_pcie *pci)
{
	switch (pci->link_gen) {
	case 3:
		pci->n_fts[1] = PORT_AFR_N_FTS_GEN3;
		break;
	case 4:
		pci->n_fts[1] = PORT_AFR_N_FTS_GEN4;
		break;
	default:
		pci->n_fts[1] = PORT_AFR_N_FTS_GEN12_DFT;
		break;
	}
	pci->n_fts[0] = PORT_AFR_N_FTS_GEN12_DFT;
}

static int intel_pcie_ep_rst_init(struct intel_pcie_port *lpp)
{
	struct device *dev = lpp->pci.dev;
	int ret;

	if (lpp->cellular_module != PCIE_CELLULAR_MODULE_NONE) {
		lpp->module_power_on = devm_gpiod_get(dev, "power_on", GPIOD_OUT_LOW);
		if (IS_ERR(lpp->module_power_on)) {
			ret = PTR_ERR(lpp->module_power_on);
			dev_err(dev, "failed to request module_power_on GPIO: %d\n", ret);
			return ret;
		}
		lpp->module_reset_gpio = devm_gpiod_get(dev, "module_reset", GPIOD_OUT_HIGH);
		if (IS_ERR(lpp->module_reset_gpio)) {
			ret = PTR_ERR(lpp->module_reset_gpio);
			dev_err(dev, "failed to request module_reset GPIO: %d\n", ret);
			return ret;
		}
		lpp->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
		if (IS_ERR(lpp->reset_gpio)) {
			ret = PTR_ERR(lpp->reset_gpio);
			dev_err(dev, "failed to request module PCIe reset GPIO: %d\n", ret);
			return ret;
		}

		if (lpp->cellular_module == PCIE_CELLULAR_MODULE_RM502Q_AE) {
			msleep(50);
			gpiod_set_value_cansleep(lpp->module_reset_gpio, 0);
			msleep(200);
			gpiod_set_value_cansleep(lpp->module_power_on, 1);
			msleep(2000);
		} else if (lpp->cellular_module == PCIE_CELLULAR_MODULE_RM520N_GL) {
			msleep(1000);
			gpiod_set_value_cansleep(lpp->module_power_on, 1);
			msleep(2000);
		} else {
			dev_err(dev, "does not support cellular module:%d\n", lpp->cellular_module);
			return -EINVAL;
		}
	} else {
		lpp->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
		if (IS_ERR(lpp->reset_gpio)) {
			ret = PTR_ERR(lpp->reset_gpio);
			if (ret != -EPROBE_DEFER)
				dev_err(dev, "Failed to request PCIe GPIO: %d\n", ret);
			return ret;
		}

		/* Make initial reset last for 100us */
		usleep_range(100, 200);
	}

	return 0;
}

static const char *pcie_link_gen_to_str(int gen)
{
	switch (gen) {
	case PCIE_LINK_SPEED_GEN1:
		return "2.5";
	case PCIE_LINK_SPEED_GEN2:
		return "5.0";
	case PCIE_LINK_SPEED_GEN3:
		return "8.0";
	case PCIE_LINK_SPEED_GEN4:
		return "16.0";
	default:
		break;
	}
	return "???";
}

static ssize_t
pcie_link_status_show(struct device *dev, struct device_attribute *attr,
		      char *buf)
{
	u32 reg, width, gen;
	struct intel_pcie_port *lpp;
	struct dw_pcie *pci;
	u8 offset;

	lpp = dev_get_drvdata(dev);
	pci = &lpp->pci;
	offset = dw_pcie_find_capability(pci, PCI_CAP_ID_EXP);

	reg = pcie_rc_cfg_rd(lpp, offset + PCI_EXP_LNKCTL) >> 16;
	width = FIELD_GET(PCI_EXP_LNKSTA_NLW, reg);
	gen = FIELD_GET(PCI_EXP_LNKSTA_CLS, reg);

	if (gen > pci->link_gen)
		return -EINVAL;

	return sprintf(buf, "Port %2u Width x%u Speed %s GT/s\n", lpp->id,
		       width, pcie_link_gen_to_str(gen));
}
static DEVICE_ATTR_RO(pcie_link_status);

static ssize_t pcie_speed_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
	struct intel_pcie_port *lpp = dev_get_drvdata(dev);
	unsigned long val;
	int ret;

	ret = kstrtoul(buf, 10, &val);
	if (ret)
		return ret;

	/* Let configuration /dt control this */
	if (val > lpp->pci.link_gen)
		return -EINVAL;

	lpp->link_gen = val;
	dw_pcie_link_set_max_speed(&lpp->pci, val);
	/* disable/enable speed to let the change take effect */
	dw_pcie_link_speed_change(&lpp->pci, false);
	dw_pcie_link_speed_change(&lpp->pci, true);

	return len;
}
static DEVICE_ATTR_WO(pcie_speed);

/*
 * Link width change on the fly is not always successful.
 * It also depends on the partner.
 */
static ssize_t pcie_width_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
	struct intel_pcie_port *lpp = dev_get_drvdata(dev);
	u8 offset = dw_pcie_find_capability(&lpp->pci, PCI_CAP_ID_EXP);
	unsigned long val;
	int ret;

	ret = kstrtoul(buf, 10, &val);
	if (ret)
		return ret;

	if (val > lpp->max_width)
		return -EINVAL;

	/* HW auto bandwidth negotiation must be enabled */
	pcie_rc_cfg_wr_mask(lpp, PCI_EXP_LNKCTL_HAWD, 0,
			    offset + PCI_EXP_LNKCTL);
	dw_pcie_link_width_resize(&lpp->pci, val);

	return len;
}

static DEVICE_ATTR_WO(pcie_width);

static struct attribute *pcie_cfg_attrs[] = {
	&dev_attr_pcie_link_status.attr,
	&dev_attr_pcie_speed.attr,
	&dev_attr_pcie_width.attr,
	NULL,
};

ATTRIBUTE_GROUPS(pcie_cfg);
static int intel_pcie_sysfs_init(struct intel_pcie_port *lpp)
{
	return devm_device_add_groups(lpp->pci.dev, pcie_cfg_groups);
}

static void intel_pcie_core_rst_assert(struct intel_pcie_port *lpp)
{
	reset_control_assert(lpp->core_rst);
}

static void intel_pcie_core_rst_deassert(struct intel_pcie_port *lpp)
{
	/*
	 * One micro-second delay to make sure the reset pulse
	 * wide enough so that core reset is clean.
	 */
	udelay(1);
	reset_control_deassert(lpp->core_rst);

	/*
	 * Some SoC core reset also reset PHY, more delay needed
	 * to make sure the reset process is done.
	 */
	usleep_range(1000, 2000);
}

static void intel_pcie_device_rst_assert(struct intel_pcie_port *lpp)
{
	gpiod_set_value_cansleep(lpp->reset_gpio, 1);
}

static void intel_pcie_device_rst_deassert(struct intel_pcie_port *lpp)
{
	msleep(lpp->rst_intrvl);
	gpiod_set_value_cansleep(lpp->reset_gpio, 0);
}

static void intel_pcie_core_irq_enable(struct intel_pcie_port *lpp)
{
	pcie_app_wr(lpp, PCIE_APP_IRNEN, 0);
	pcie_app_wr(lpp, PCIE_APP_IRNCR, PCIE_APP_IRN_INT);
	pcie_app_wr(lpp, PCIE_APP_IRNEN, PCIE_APP_IRN_INT);
}

static void intel_pcie_core_irq_disable(struct intel_pcie_port *lpp)
{
	pcie_app_wr(lpp, PCIE_APP_IRNEN, 0);
	pcie_app_wr(lpp, PCIE_APP_IRNCR, PCIE_APP_IRN_INT);
}

static int intel_pcie_get_resources(struct platform_device *pdev)
{
	struct intel_pcie_port *lpp = platform_get_drvdata(pdev);
	struct dw_pcie *pci = &lpp->pci;
	struct device *dev = pci->dev;
	int ret;

	lpp->core_clk = devm_clk_get(dev, NULL);
	if (IS_ERR(lpp->core_clk)) {
		ret = PTR_ERR(lpp->core_clk);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "Failed to get clks: %d\n", ret);
		return ret;
	}

	lpp->core_rst = devm_reset_control_get(dev, NULL);
	if (IS_ERR(lpp->core_rst)) {
		ret = PTR_ERR(lpp->core_rst);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "Failed to get resets: %d\n", ret);
		return ret;
	}

	ret = device_property_read_u32(dev, "reset-assert-ms",
				       &lpp->rst_intrvl);
	if (ret)
		lpp->rst_intrvl = RESET_INTERVAL_MS;

	lpp->has_bus_iatu = device_property_read_bool(dev, "intel,has_bus_iatu");

	lpp->app_base = devm_platform_ioremap_resource_byname(pdev, "app");
	if (IS_ERR(lpp->app_base))
		return PTR_ERR(lpp->app_base);

	lpp->phy = devm_phy_get(dev, "pcie");
	if (IS_ERR(lpp->phy)) {
		ret = PTR_ERR(lpp->phy);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "Couldn't get pcie-phy: %d\n", ret);
		return ret;
	}

	/* for 5GNR module */
	ret = device_property_read_u32(dev, "intel,cellular_module", &lpp->cellular_module);
	if (ret)
		lpp->cellular_module = PCIE_CELLULAR_MODULE_NONE;

	return 0;
}

static int intel_pcie_wait_l2(struct intel_pcie_port *lpp)
{
	u32 value;
	int ret;
	struct dw_pcie *pci = &lpp->pci;

	if (pci->link_gen < 3)
		return 0;

	/* Send PME_TURN_OFF message */
	pcie_app_wr_mask(lpp, PCIE_APP_MSG_CR, PCIE_APP_MSG_XMT_PM_TURNOFF,
			 PCIE_APP_MSG_XMT_PM_TURNOFF);

	/* Read PMC status and wait for falling into L2 link state */
	ret = readl_poll_timeout(lpp->app_base + PCIE_APP_PMC, value,
				 value & PCIE_APP_PMC_IN_L2, 20,
				 jiffies_to_usecs(1 * HZ));
	if (ret)
		dev_err(lpp->pci.dev, "PCIe link enter L2 timeout!\n");

	return ret;
}

static void intel_pcie_turn_off(struct intel_pcie_port *lpp)
{
	if (dw_pcie_link_up(&lpp->pci))
		intel_pcie_wait_l2(lpp);

	/* Put endpoint device in reset state */
	intel_pcie_device_rst_assert(lpp);
	pcie_rc_cfg_wr_mask(lpp, PCI_COMMAND, PCI_COMMAND_MEMORY, 0);
}

static int intel_pcie_host_setup(struct intel_pcie_port *lpp)
{
	int ret, id;
	struct dw_pcie *pci = &lpp->pci;
	struct device *dev = pci->dev;

	id = of_alias_get_id(dev->of_node, "pcie");
	if (id < 0) {
		dev_err(dev, "failed to get domain id, errno: %d\n", id);
		return id;
	}
	lpp->id = id;

	intel_pcie_core_rst_assert(lpp);
	intel_pcie_device_rst_assert(lpp);
	intel_pcie_core_rst_deassert(lpp);

	/* Controller clock must be provided earlier than PHY */
	ret = clk_prepare_enable(lpp->core_clk);
	if (ret) {
		dev_err(dev, "Core clock enable failed: %d\n", ret);
		goto clk_err;
	}

	ret = phy_init(lpp->phy);
	if (ret)
		return ret;

	intel_pcie_vendor_id_overwrite(lpp);
	intel_pcie_ltssm_disable(lpp);
	intel_pcie_link_setup(lpp);
	intel_pcie_init_n_fts(pci);
	dw_pcie_upconfig_setup(pci);

	/* Enable integrated interrupts */
	intel_pcie_core_irq_enable(lpp);

	return 0;

clk_err:
	intel_pcie_core_rst_assert(lpp);
	phy_exit(lpp->phy);

	return ret;
}

static void __intel_pcie_remove(struct intel_pcie_port *lpp)
{
	intel_pcie_core_irq_disable(lpp);
	intel_pcie_turn_off(lpp);
	clk_disable_unprepare(lpp->core_clk);
	intel_pcie_core_rst_assert(lpp);
	phy_exit(lpp->phy);
}

static int intel_pcie_remove(struct platform_device *pdev)
{
	struct intel_pcie_port *lpp = platform_get_drvdata(pdev);
	struct pcie_port *pp = &lpp->pci.pp;

	dw_pcie_host_deinit(pp);
	__intel_pcie_remove(lpp);

	return 0;
}

static void intel_pcie_shutdown(struct platform_device *pdev)
{
	struct intel_pcie_port *lpp = platform_get_drvdata(pdev);

	__intel_pcie_remove(lpp);
}

static int __maybe_unused intel_pcie_suspend_noirq(struct device *dev)
{
	struct intel_pcie_port *lpp = dev_get_drvdata(dev);
	int ret;

	intel_pcie_core_irq_disable(lpp);
	ret = intel_pcie_wait_l2(lpp);
	if (ret)
		return ret;

	phy_exit(lpp->phy);
	clk_disable_unprepare(lpp->core_clk);
	return ret;
}

static int __maybe_unused intel_pcie_resume_noirq(struct device *dev)
{
	struct intel_pcie_port *lpp = dev_get_drvdata(dev);

	return intel_pcie_host_setup(lpp);
}

static int intel_pcie_rc_init(struct pcie_port *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct intel_pcie_port *lpp = dev_get_drvdata(pci->dev);

	return intel_pcie_host_setup(lpp);
}

static irqreturn_t intel_pcie_core_isr(int irq, void *arg)
{
	struct intel_pcie_port *lpp = arg;
	struct device *dev = lpp->pci.dev;
	u32 reg;

	reg = pcie_app_rd(lpp, PCIE_APP_IRNCR);

	if (reg & PCIE_APP_IRN_AER_REPORT)
		dev_dbg(dev, "AER interrupt received\n");

	if (reg & PCIE_APP_IRN_PME)
		dev_dbg(dev, "Power Management Event received\n");

	if (reg & PCIE_APP_IRN_HOTPLUG)
		dev_dbg(dev, "Hotplug interrupt received\n");

	if (reg & PCIE_APP_IRN_RX_VDM_MSG)
		dev_dbg(dev, "Vendor Message received\n");

	if (reg & PCIE_APP_IRN_PM_TO_ACK)
		dev_dbg(dev, "PM ack interrupt received\n");

	if (reg & PCIE_APP_IRN_PM_TURNOFF_ACK)
		dev_dbg(dev, "PM turn off ack interrupt received\n");

	if (reg & PCIE_APP_IRN_LINK_AUTO_BW_STAT)
		dev_dbg(dev, "Link auto bandwidth interrupt received\n");

	if (reg & PCIE_APP_IRN_BW_MGT)
		dev_dbg(dev, "Bandwidth management interrupt received\n");

	if (reg & PCIE_APP_IRN_WAKEUP)
		dev_dbg(dev, "Wake up interrupt received\n");

	if (reg & PCIE_APP_IRN_MSG_LTR)
		dev_dbg(dev, "Long tolerance interrupt received\n");

	if (reg & PCIE_APP_IRN_REQ_RST)
		dev_dbg(dev, "Link reset (down) received from device\n");

	if (reg & PCIE_APP_IRN_SYS_ERR_RC)
		dev_dbg(dev, "System err rc interrupt received\n");

	if (reg & PCIE_APP_IRN_SYS_INT)
		dev_dbg(dev, "System err system received\n");

	pcie_app_wr(lpp, PCIE_APP_IRNCR, reg);

	return IRQ_HANDLED;
}

static bool intel_pcie_overlaps_region(phys_addr_t base1, size_t size1,
				       phys_addr_t base2, size_t size2)
{
	return ((base1 < (base2 + size2)) && (base2 < (base1 + size1)));
}

static bool intel_pcie_ib_overlaps_region(struct intel_pcie_port *lpp,
					  phys_addr_t start, size_t size)
{
	int i;
	phys_addr_t ib_base;
	size_t ib_size;

	for (i = 0; i < ARRAY_SIZE(lpp->region); i++) {
		if (!test_bit(i, lpp->ib_window_map))
			continue;
		ib_base = lpp->region[i].base;
		ib_size = lpp->region[i].size;
		if (intel_pcie_overlaps_region(ib_base, ib_size, start, size))
			return true;
	}

	return false;
}

int intel_pcie_dc_iatu_inbound_add(void *sysdata, u32 start,
				   size_t size, u32 *atu_index)
{
	struct pcie_port *pp = sysdata;
	struct dw_pcie *pci;
	struct intel_pcie_port *lpp;
	u32 free_win;
	u64 target_addr;
	int ret;

	if (!pp || !start || !size)
		return -EINVAL;

	if (ALIGN(start, SZ_4K) != start || ALIGN(size, SZ_4K) != size)
		return -EINVAL;

	pci = to_dw_pcie_from_pp(pp);
	lpp = dev_get_drvdata(pci->dev);

	spin_lock(&lpp->ib_window_lock);
	free_win = find_first_zero_bit(lpp->ib_window_map, PCIE_ATU_IB_WINDOWS);
	if (free_win >= PCIE_ATU_IB_WINDOWS) {
		ret = -ENODEV;
		dev_err(pci->dev, "No free inbound window\n");
		goto ib_fail;
	}

	if (intel_pcie_ib_overlaps_region(lpp, start, size)) {
		ret = -EINVAL;
		dev_err(pci->dev, "inbound region overlaps\n");
		goto ib_fail;
	}

	/* External device only support 4GB, top 4 bits fixed as 0xC */
	target_addr = 0xC00000000 + start;
	ret = dwc_pcie_rc_prog_inbound_atu_unroll(pci, free_win, start, target_addr, size);
	if (ret) {
		dev_err(pci->dev, "Failed to program IB window\n");
		goto ib_fail;
	}

	set_bit(free_win, lpp->ib_window_map);
	lpp->region[free_win].base = start;
	lpp->region[free_win].size = size;
	spin_unlock(&lpp->ib_window_lock);

	*atu_index = free_win;
	dev_dbg(pci->dev, "Region %u addr %x size %zx translated\n",
		free_win, start, size);

	return 0;

ib_fail:
	spin_unlock(&lpp->ib_window_lock);
	return ret;
}
EXPORT_SYMBOL_GPL(intel_pcie_dc_iatu_inbound_add);

int intel_pcie_dc_iatu_inbound_remove(void *sysdata, u32 atu_index)
{
	struct pcie_port *pp = sysdata;
	struct dw_pcie *pci;
	struct intel_pcie_port *lpp;

	if (!sysdata)
		return -EINVAL;

	pci = to_dw_pcie_from_pp(pp);
	lpp = dev_get_drvdata(pci->dev);

	if (!test_bit(atu_index, lpp->ib_window_map))
		return -EINVAL;

	dw_pcie_disable_atu(pci, atu_index, DW_PCIE_REGION_INBOUND);

	spin_lock(&lpp->ib_window_lock);
	clear_bit(atu_index, lpp->ib_window_map);
	spin_unlock(&lpp->ib_window_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(intel_pcie_dc_iatu_inbound_remove);

struct device *intel_pcie_firewall_device(void *sysdata)
{
	struct pcie_port *pp = sysdata;
	struct dw_pcie *pci;

	if (!sysdata)
		return ERR_PTR(-EINVAL);

	pci = to_dw_pcie_from_pp(pp);

	return pci->dev;
}
EXPORT_SYMBOL_GPL(intel_pcie_firewall_device);

static int intel_pcie_msi_host_init(struct pcie_port *pp)
{
	struct platform_device *pdev;
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct intel_pcie_port *lpp = dev_get_drvdata(pci->dev);
	int irq, ret;

	pdev = to_platform_device(pci->dev);

	irq = platform_get_irq_byname(pdev, "ir");
	if (irq < 0) {
		dev_err(pci->dev, "missing sys integrated irq resource\n");
		return irq;
	}
	snprintf(lpp->irq_name, sizeof(lpp->irq_name) - 1,
		 "pcie_ir%d", lpp->id);

	ret = devm_request_irq(pci->dev, irq, intel_pcie_core_isr,
			       IRQF_SHARED, lpp->irq_name, lpp);
	if (ret) {
		dev_err(pci->dev, "request irq %d failed\n", irq);
		return ret;
	}

	return 0;
}

static u64 intel_pcie_cpu_addr(struct dw_pcie *pci, u64 cpu_addr)
{
	struct intel_pcie_port *lpp = dev_get_drvdata(pci->dev);

	if (lpp->has_bus_iatu)
		return cpu_addr + BUS_IATU_OFFSET;

	return cpu_addr;
}

static int intel_pcie_start_link(struct dw_pcie *pci)
{
	struct intel_pcie_port *lpp = dev_get_drvdata(pci->dev);

	intel_pcie_device_rst_deassert(lpp);
	intel_pcie_ltssm_enable(lpp);

	return 0;
}

static const struct dw_pcie_ops intel_pcie_ops = {
	.cpu_addr_fixup = intel_pcie_cpu_addr,
	.start_link = intel_pcie_start_link,
};

static const struct dw_pcie_host_ops intel_pcie_dw_ops = {
	.host_init = intel_pcie_rc_init,
	.msi_host_init = intel_pcie_msi_host_init,
};

static const struct intel_pcie_soc pcie_data = {
	.pcie_ver = 0x520A,
};

static int intel_pcie_probe(struct platform_device *pdev)
{
	const struct intel_pcie_soc *data;
	struct device *dev = &pdev->dev;
	struct intel_pcie_port *lpp;
	struct pcie_port *pp;
	struct dw_pcie *pci;
	int ret;

	lpp = devm_kzalloc(dev, sizeof(*lpp), GFP_KERNEL);
	if (!lpp)
		return -ENOMEM;

	platform_set_drvdata(pdev, lpp);
	pci = &lpp->pci;
	pci->dev = dev;
	pp = &pci->pp;

	ret = intel_pcie_get_resources(pdev);
	if (ret)
		return ret;

	ret = intel_pcie_ep_rst_init(lpp);
	if (ret)
		return ret;

	data = device_get_match_data(dev);
	if (!data)
		return -ENODEV;

	pci->ops = &intel_pcie_ops;
	pci->version = data->pcie_ver;
	pp->ops = &intel_pcie_dw_ops;

	ret = dw_pcie_host_init(pp);
	if (ret) {
		dev_err(dev, "Cannot initialize host\n");
		return ret;
	}

	ret = intel_pcie_sysfs_init(lpp);
	if (ret) {
		dev_err(dev, "Cannot add sysfs\n");
		return ret;
	}

	return 0;
}

static const struct dev_pm_ops intel_pcie_pm_ops = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(intel_pcie_suspend_noirq,
				      intel_pcie_resume_noirq)
};

static const struct of_device_id of_intel_pcie_match[] = {
	{ .compatible = "intel,lgm-pcie", .data = &pcie_data },
	{}
};

static struct platform_driver intel_pcie_driver = {
	.probe = intel_pcie_probe,
	.remove = intel_pcie_remove,
	.shutdown = intel_pcie_shutdown,
	.driver = {
		.name = "intel-gw-pcie",
		.of_match_table = of_intel_pcie_match,
		.pm = &intel_pcie_pm_ops,
	},
};

module_platform_driver(intel_pcie_driver);
MODULE_LICENSE("GPL");
