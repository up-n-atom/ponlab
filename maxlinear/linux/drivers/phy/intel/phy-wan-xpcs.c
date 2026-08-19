// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/phy/phy.h>
#include <linux/clk.h>
#include <linux/reset.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

/* chiptop aon/pon config; this is platform specific */
#define CHIP_TOP_IFMUX_CFG		0x120
#define WAN_MUX_AON			0x1
#define WAN_MUX_MASK			0x1
#define TX_MAIN_CUR			0x4008
#define TX_MAIN_CUR_MASK		0x3F
#define TX_MAIN_CUR_OFF			9
#define MAIN_CURSOR_OVRD		15
#define TX_PRE_CUR			0x400C
#define TX_PRE_CUR_MASK			0x1F
#define TX_PRE_CUR_OFF			0
#define POST_OVRD_EN			6
#define TX_POST_CUR			0x400C
#define TX_POST_CUR_MASK		0x3F
#define TX_POST_CUR_OFF			7
#define PRE_OVRD_EN			13
#define PON_SHELL_MODE_SEL		0x58
#define PON_ETH_SEL_MASK		BIT(0)
#define PON_ETH_SEL_ETH_ON		1
#define PON_ETH_SEL_ETH_OFF		0
#define PON_SHELL_REFCLK_MASK		BIT(2)
#define PON_SHELL_REFCLK_EN		1
#define PON_SHELL_REFCLK_DIS		0
#define PON_SHELL_GEN_CTRL		0x0
#define PHY_RST_N_MASK			BIT(5)
#define PHY_RST_N_EN			0x0
#define PHY_RST_N_DIS			0x1
#define XPCS_RST_N_MASK			BIT(6)
#define XPCS_RST_N_EN			0x0
#define XPCS_RST_N_DIS			0x1

/* PON IP SHELL APB APP */
#define APB_TIMEOUT_CTRL		0x0
#define APB_CLK_FRQ_MASK		GENMASK(9, 0)

/* PON SHELL APP */
#define GEN_CTRL			0x0
#define PON_RST_N			BIT(4)
#define PHY_RST_N			BIT(5)
#define XPCS_RST_N			BIT(6)
#define AON_CLK_GATE			BIT(11)
#define PON_CLK_GATE			BIT(12)
#define PARA_CLK_GATE			BIT(13)
#define REF_CLK_EN			BIT(16)
#define SRAM_SCR			0x8
#define EXT_PG_CTRL			0x50
#define PMA_PWR_STABLE			BIT(3)
#define PCS_PWR_STABLE			BIT(5)
#define MODE_SEL			0x58
#define PON_ETH_SEL			BIT(0)
#define LOOP_SYNC_SEL			BIT(1)
#define REFCLK_EN			BIT(2)
#define GEPON_SEL			BIT(3)

/* CR register */
#define RX_EQ_ASIC_IN(x)		(0x1013 + (x))
#define PCS_XF_ATE_OVRD_IN_2		0x3008
#define ADAPT_REQ			BIT(4)
#define ADAPT_REQ_OVRD_EN		BIT(5)
#define PCS_XF_RX_ADAPT_ACK		0x3010
#define RX_ADAPT_ACK			BIT(0)
#define RX_ADPTCTL_ATT_STATUS		0x106b
#define RX_ADPTCTL_VGA_STATUS		0x106c
#define RX_ADPTCTL_CTLE_STATUS		0x106d
#define RX_ADPTCTL_DFE_TAP1_STATUS	0x106e

#define CR_ADDR(addr)			((addr) << 2)

enum {
	PON_RST,
	PHY_RST = PON_RST,
	WANSS_RST,
	MAX_RST,
};

struct wan_xpcs_phy;

static const struct regmap_config pon_shell_regmap_cfg = {
	.name		= "pon_shell",
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
};

struct wan_xpcs_soc_data {
	char *name;
	char *rst_name[MAX_RST];
	int (*reg_init)(struct wan_xpcs_phy *priv);
	int (*phy_init)(struct wan_xpcs_phy *priv);
	int (*phy_exit)(struct wan_xpcs_phy *priv);
	int (*power_on)(struct wan_xpcs_phy *priv, bool enable);
	int (*calibrate)(struct wan_xpcs_phy *priv);
};

struct wan_xpcs_phy {
	struct phy				*phy;
	struct platform_device			*pdev;
	struct device				*dev;
	struct clk				*clk;
	u32					clk_freq;
	struct regmap				*syscfg;
	struct reset_control			*resets[MAX_RST];
	void __iomem				*base;
	void __iomem				*cr_base;
	void __iomem				*app_base;
	struct regmap				*pon_shell_regmap;
	struct dentry				*debugfs;
	const struct wan_xpcs_soc_data	*soc_data;
};

static u32 wan_xpcs_r32(void __iomem *base, u32 reg)
{
	return readl(base + reg);
}

static void wan_xpcs_w32(void __iomem *base, u32 val,  u32 reg)
{
	writel(val, base + reg);
}

static void wan_xpcs_w32_off_mask(void __iomem *base, u32 off,
					u32 mask, u32 set, u32 reg)
{
	u32 val;

	val = wan_xpcs_r32(base, reg) & (~(mask << off));
	val |= (set & mask) << off;
	wan_xpcs_w32(base, val, reg);
}

static void wan_xpcs_reg_bit_set(void __iomem *base, u32 off, u32 reg)
{
	wan_xpcs_w32_off_mask(base, off, 1, 1, reg);
}

#if defined(CONFIG_DEBUG_FS)

static ssize_t
wan_xpcs_phy_serdes_write(struct file *s, const char __user *buffer,
				size_t count, loff_t *pos)
{
	struct wan_xpcs_phy *priv = file_inode(s)->i_private;
	u32 main_cur = 0, pre_cur = 0, post_cur = 0;
	char buf[32] = {0};
	size_t buf_size;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (count > sizeof(buf) - 1)
		return -EINVAL;

	memset(buf, 0, sizeof(buf));
	buf_size = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, buffer, buf_size))
		return -EFAULT;

	buf[buf_size] = '\0';

	if (strcmp(buf, "help") == 0)
		goto __write_help;

	if (sscanf(buf, "%u %u %u", &main_cur, &pre_cur, &post_cur) != 3)
		goto __write_help;

	/* Cursor */
	wan_xpcs_w32_off_mask(priv->base, TX_MAIN_CUR_OFF,
				    TX_MAIN_CUR_MASK, main_cur, TX_MAIN_CUR);
	wan_xpcs_w32_off_mask(priv->base, TX_PRE_CUR_OFF,
				    TX_PRE_CUR_MASK, pre_cur, TX_PRE_CUR);
	wan_xpcs_w32_off_mask(priv->base, TX_POST_CUR_OFF,
				    TX_POST_CUR_MASK, post_cur, TX_POST_CUR);

	/* Override */
	wan_xpcs_reg_bit_set(priv->base, MAIN_CURSOR_OVRD,
				   TX_MAIN_CUR);
	wan_xpcs_reg_bit_set(priv->base, POST_OVRD_EN, TX_PRE_CUR);
	wan_xpcs_reg_bit_set(priv->base, PRE_OVRD_EN, TX_POST_CUR);

	return count;

__write_help:
	dev_info(priv->dev, "echo <main_cur> <pre_cur> <post_cur> > /sys/kernel/debug/phy/wan_xpcs_serdes\n");
	return count;
}

static int wan_xpcs_phy_seq_read(struct seq_file *s, void *v)
{
	struct wan_xpcs_phy *priv = s->private;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	seq_printf(s, "TX_MAIN_CUR\t%u\n",
		   ((wan_xpcs_r32(priv->base, TX_MAIN_CUR) &
		   (TX_MAIN_CUR_MASK << TX_MAIN_CUR_OFF)) >> TX_MAIN_CUR_OFF));
	seq_printf(s, "TX_PRE_CUR\t%u\n",
		   ((wan_xpcs_r32(priv->base, TX_PRE_CUR) &
		   (TX_PRE_CUR_MASK << TX_PRE_CUR_OFF)) >> TX_PRE_CUR_OFF));
	seq_printf(s, "TX_POST_CUR\t%u\n",
		   ((wan_xpcs_r32(priv->base, TX_POST_CUR) &
		   (TX_POST_CUR_MASK << TX_POST_CUR_OFF)) >> TX_POST_CUR_OFF));

	return 0;
}

static int wan_xpcs_phy_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, wan_xpcs_phy_seq_read, inode->i_private);
}

static const struct file_operations wan_xpcs_fops = {
	.owner =	THIS_MODULE,
	.open =		wan_xpcs_phy_seq_open,
	.read =		seq_read,
	.write =	wan_xpcs_phy_serdes_write,
	.llseek =	seq_lseek,
	.release =	single_release,
};

static int wan_xpcs_phy_debugfs_init(struct wan_xpcs_phy *priv)
{
	char wan_xpcs_dir[64] = {0};

	strlcpy(wan_xpcs_dir, priv->dev->of_node->name, sizeof(wan_xpcs_dir));
	priv->debugfs = debugfs_create_dir(wan_xpcs_dir, NULL);

	if (IS_ERR_OR_NULL(priv->debugfs))
		return -ENOMEM;

	if (IS_ERR_OR_NULL(debugfs_create_file("wan_xpcs_serdes", 0644, priv->debugfs,
					       priv, &wan_xpcs_fops)))
		goto __debugfs_err;

	return 0;

__debugfs_err:
	debugfs_remove_recursive(priv->debugfs);
	return -ENOMEM;
}

static int wan_xpcs_phy_debugfs_exit(struct wan_xpcs_phy *priv)
{
	debugfs_remove_recursive(priv->debugfs);
	priv->debugfs = NULL;
	return 0;
}

#else
static int wan_xpcs_phy_debugfs_init(struct wan_xpcs_phy *priv)
{
	return 0;
}

static int wan_xpcs_phy_debugfs_exit(struct wan_xpcs_phy *priv)
{
	return 0;
}
#endif /* CONFIG_DEBUG_FS */

#define PCIE_PHY_POLL_CNT      5000
void phy_serdes_dig_ovrd(struct phy *phy)
{
	struct wan_xpcs_phy *priv = phy_get_drvdata(phy);
	void __iomem *phy_cr_base = priv->cr_base;
	struct device *dev = priv->dev;
	int i, val;

	for (i = 0; i < PCIE_PHY_POLL_CNT; i++) {
		val = readl(phy_cr_base + CR_ADDR(0x0e));

		val &= ~BIT(2);
		val |= BIT(3) | BIT(4);
		writel(val, phy_cr_base + CR_ADDR(0x0e));
		val = readl(phy_cr_base + CR_ADDR(0x0e));
		if (((val & 0x18) == 0x18) &&
		    !!(readl(phy_cr_base + CR_ADDR(0x2014)) & BIT(0)))
			break;
		usleep_range(10, 20);
	}

	if (i >= PCIE_PHY_POLL_CNT)
		dev_err(dev, "SUP_DIG_SUP_OVRD_IN_0 0x%08x\n",
			readl(phy_cr_base + CR_ADDR(0x0e)));

	dev_dbg(dev, " SUP_DIG_SUP_OVRD_IN_0 cnt %d\n", i);
	/* RAWLANEN_DIG_AON_INIT_PWRUP_DOWN */
	val = readl(phy_cr_base + CR_ADDR(0x2014));
	dev_dbg(dev, "RAWLANEN_DIG_AON_INIT_PWRUP_DOWN PWR_DONE %d\n",
		!!(val & BIT(0)));

	for (i = 0; i < PCIE_PHY_POLL_CNT; i++) {
		if (!!(readl(phy_cr_base + CR_ADDR(0x203b)) & BIT(1)))
			break;
		udelay(5);
	}

	if (i >= PCIE_PHY_POLL_CNT) {
		dev_err(dev, "WAN XPCS PHY calibration failed\n");
	} else {
		dev_dbg(dev, "WAN XPCS calibration cnt %d\n", i);
		dev_dbg(dev, "WAN XPCS PHY calibration succeed\n");
	}
}
EXPORT_SYMBOL(phy_serdes_dig_ovrd);

static void
wan_phy_update_bits(void __iomem *base, unsigned int reg, unsigned int mask,
		    unsigned int set_val)
{
	unsigned int reg_val, val;

	reg_val = readl(base + reg);
	val = reg_val & ~mask;
	val |= set_val & mask;
	if (reg_val != val)
		writel(val, base + reg);
}

static int wan_xpcs_phy_trigger_reset(struct wan_xpcs_phy *priv,
				      bool reset_assert)
{
	struct device *dev = priv->dev;
	int ret;
	int i;

	for (i = 0; i < MAX_RST; i++) {
		if (!priv->soc_data->rst_name[i])
			break;
		if (reset_assert) {
			ret = reset_control_assert(priv->resets[i]);
			if (ret) {
				dev_err(dev, "Failed to assert %s reset\n",
					priv->soc_data->rst_name[i]);
				return ret;
			}
		} else {
			ret = reset_control_deassert(priv->resets[i]);
			if (ret) {
				dev_err(dev, "Failed to deassert %s reset\n",
					priv->soc_data->rst_name[i]);
				return ret;
			}
		}
	}

	if (!reset_assert)
		udelay(2);

	return 0;
}

static int wan_xpcs_phy_init(struct phy *phy)
{
	struct wan_xpcs_phy *priv = phy_get_drvdata(phy);

	dev_dbg(priv->dev, "Initializing wan xpcs phy\n");

	if (priv->clk && clk_prepare_enable(priv->clk) < 0) {
		dev_err(priv->dev, "Failed to enable phy clock!\n");
		return -ENODEV;
	}
	priv->soc_data->phy_init(priv);

	return 0;
}

static int wan_xpcs_phy_exit(struct phy *phy)
{
	struct wan_xpcs_phy *priv = phy_get_drvdata(phy);

	dev_dbg(priv->dev, "Exit wan xpcs phy\n");

	if (priv->soc_data->phy_exit)
		return priv->soc_data->phy_exit(priv);

	if (priv->clk)
		clk_disable_unprepare(priv->clk);

	return 0;
}

static int wan_xpcs_phy_power_on(struct phy *phy)
{
	struct wan_xpcs_phy *priv = phy_get_drvdata(phy);

	dev_dbg(priv->dev, "Power on wan xpcs phy\n");

	/* enable clk and deassert reset */
	if (priv->soc_data->power_on)
		return priv->soc_data->power_on(priv, true);

	return 0;
}

static int wan_xpcs_phy_power_off(struct phy *phy)
{
	struct wan_xpcs_phy *priv = phy_get_drvdata(phy);

	dev_dbg(priv->dev, "Power off xpcs phy\n");

	/*disable clk and assert reset*/
	if (priv->soc_data->power_on)
		return priv->soc_data->power_on(priv, false);

	return 0;
}

static int wan_xpcs_phy_calibrate(struct phy *phy)
{
	struct wan_xpcs_phy *priv = phy_get_drvdata(phy);

	dev_dbg(priv->dev, "Calibrate xpcs phy\n");

	if (priv->soc_data->calibrate)
		return priv->soc_data->calibrate(priv);

	return 0;
}

static int wan_xpcs_phy_dt_parse(struct wan_xpcs_phy *priv)
{
	struct device *dev = priv->dev;
	int i;

	priv->soc_data = of_device_get_match_data(dev);
	if (!priv->soc_data) {
		dev_err(dev, "Failed to get soc data\n");
		return -EINVAL;
	}

	priv->clk = devm_clk_get(priv->dev, NULL);
	if (IS_ERR(priv->clk))
		priv->clk = NULL;

	if (device_property_read_u32(dev, "clock-frequency", &priv->clk_freq))
		priv->clk_freq = 0;

	for (i = 0; i < MAX_RST; i++) {
		char *rst_n = priv->soc_data->rst_name[i];

		if (!rst_n)
			break;
		priv->resets[i] = devm_reset_control_get(dev, rst_n);
		if (IS_ERR(priv->resets[i])) {
			dev_err(dev, "Failed to get rst %s control\n", rst_n);
			return PTR_ERR(priv->resets[i]);
		}
	}
	priv->soc_data->reg_init(priv);

	return 0;
}

static const struct phy_ops ops = {
	.init		= wan_xpcs_phy_init,
	.exit		= wan_xpcs_phy_exit,
	.power_on	= wan_xpcs_phy_power_on,
	.power_off	= wan_xpcs_phy_power_off,
	.calibrate	= wan_xpcs_phy_calibrate,
	.owner		= THIS_MODULE,
};

static int wan_xpcs_phy_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct wan_xpcs_phy *priv;
	struct phy_provider *phy_provider;
	int ret;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->dev = dev;
	priv->pdev = pdev;
	platform_set_drvdata(pdev, priv);

	ret = wan_xpcs_phy_dt_parse(priv);
	if (ret)
		return ret;

	priv->phy = devm_phy_create(dev, dev->of_node, &ops);
	if (IS_ERR(priv->phy)) {
		dev_err(dev, "Failed to create PHY\n");
		return PTR_ERR(priv->phy);
	}

	phy_set_drvdata(priv->phy, priv);

	phy_provider = devm_of_phy_provider_register(dev, of_phy_simple_xlate);
	if (IS_ERR(phy_provider)) {
		dev_err(dev, "Failed to register phy provider!\n");
		return PTR_ERR(phy_provider);
	}

	if (wan_xpcs_phy_debugfs_init(priv))
		return -EINVAL;

	dev_info(priv->dev, "wan-xpcs init is done!\n");

	return 0;
}

static int wan_xpcs_phy_remove(struct platform_device *pdev)
{
	return wan_xpcs_phy_debugfs_exit(platform_get_drvdata(pdev));
}

static int prx300_wan_xpcs_phy_reg_init(struct wan_xpcs_phy *priv)
{
	struct device *dev = priv->dev;
	struct device_node *np = dev->of_node;
	struct resource *res;

	/* get chiptop regmap */
	priv->syscfg = syscon_regmap_lookup_by_phandle(np, "mxl,syscon");
	if (IS_ERR(priv->syscfg)) {
		dev_err(dev, "No phandle specified for xpcs-phy syscon\n");
		return PTR_ERR(priv->syscfg);
	}

	res = platform_get_resource(priv->pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "Failed to get wan_serdes iomem resource!\n");
		return PTR_ERR(res);
	}

	priv->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->base)) {
		dev_err(dev, "Failed to ioremap resource: %pr\n", res);
		return PTR_ERR(priv->base);
	}

	return 0;
}

static int prx300_wan_xpcs_phy_init(struct wan_xpcs_phy *priv)
{
	/* set WAN_MUX to AON mode */
	regmap_update_bits(priv->syscfg, CHIP_TOP_IFMUX_CFG, WAN_MUX_MASK,
			   WAN_MUX_AON);
	return 0;
}

static int prx300_wan_xpcs_phy_power_on(struct wan_xpcs_phy *priv,
					bool enable)
{
	int ret = 0;

	priv->clk = devm_clk_get(priv->dev, NULL);

	if (IS_ERR(priv->clk)) {
		dev_err(priv->dev, "Failed to retrieve clk\n");
		return PTR_ERR(priv->clk);
	}

	if (enable) {
		ret = clk_prepare_enable(priv->clk);
		if (ret) {
			dev_err(priv->dev, "Failed to enable PHY gate clock\n");
			return ret;
		}

		if (priv->clk_freq)
			clk_set_rate(priv->clk, priv->clk_freq);

		/* trigger reset at RCU */
		ret = wan_xpcs_phy_trigger_reset(priv, false);
		if (ret != 0)
			return ret;

	} else {
		/* trigger reset at RCU */
		ret = wan_xpcs_phy_trigger_reset(priv, true);
		if (ret != 0)
			return ret;

		clk_disable_unprepare(priv->clk);
	}
	return ret;
}

static int lgm_wan_xpcs_phy_reg_init(struct wan_xpcs_phy *priv)
{
	struct resource *res;
	struct device *dev = priv->dev;
	int ret = 0;

	res = platform_get_resource_byname(priv->pdev,
					   IORESOURCE_MEM, "pon_shell");
	if (!res) {
		dev_err(dev, "Failed to get pon_shell iomem resource!\n");
		return PTR_ERR(res);
	}

	priv->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->base)) {
		dev_err(dev, "Failed to ioremap resource: %pr\n", res);
		return PTR_ERR(priv->base);
	}

	priv->pon_shell_regmap = devm_regmap_init_mmio(dev,
						       priv->base,
						       &pon_shell_regmap_cfg);
	if (IS_ERR(priv->pon_shell_regmap)) {
		ret = PTR_ERR(priv->pon_shell_regmap);
		dev_err(dev,
			"failed to init pon_shell register map: %d\n", ret);
		return ret;
	}

	/* serdes cr address */
	res = platform_get_resource_byname(priv->pdev,
					   IORESOURCE_MEM, "cr");
	if (!res) {
		dev_err(dev, "Failed to get serdes iomem resource!\n");
		return PTR_ERR(res);
	}

	priv->cr_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->cr_base)) {
		dev_err(dev, "Failed to ioremap cr resource: %pr\n", res);
		return PTR_ERR(priv->cr_base);
	}

	/* PON SHELL APB APP address */
	res = platform_get_resource_byname(priv->pdev,
					   IORESOURCE_MEM, "app");
	if (!res) {
		dev_err(dev, "Failed to get PON shell apb app iomem resource!\n");
		return PTR_ERR(res);
	}

	priv->app_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->app_base)) {
		dev_err(dev, "Failed to ioremap app resource: %pr\n", res);
		return PTR_ERR(priv->app_base);
	}

	return 0;
}

static void pon_aon_clk_gate_cfg(struct wan_xpcs_phy *priv, int en)
{
	int set;

	/* AON gate clk enable */
	set = en ? 0 : AON_CLK_GATE;
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL, AON_CLK_GATE, set);
	/* PON gate clk disable */
	set = en ? PON_CLK_GATE : 0;
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL, PON_CLK_GATE, set);
	/* PARA gate clock enable */
	set = en ? 0 : PARA_CLK_GATE;
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL,
			   PARA_CLK_GATE, set);

	usleep_range(10, 20);
	regmap_read(priv->pon_shell_regmap, GEN_CTRL, &set);
	dev_dbg(priv->dev, "AON CLK %s, PON CLK %s, PARA clk %s\n",
		set & AON_CLK_GATE  ? "Disabled" : "Enabled",
		set & PON_CLK_GATE  ? "Disabled" : "Enabled",
		set & PARA_CLK_GATE ? "Disabled" : "Enabled");
}

static void ponwanphy_mode(struct wan_xpcs_phy *priv)
{
	unsigned int val;

	regmap_update_bits(priv->pon_shell_regmap, MODE_SEL,
			   PON_ETH_SEL, PON_ETH_SEL);
	regmap_update_bits(priv->pon_shell_regmap, MODE_SEL,
			   LOOP_SYNC_SEL, 0);
	regmap_update_bits(priv->pon_shell_regmap, MODE_SEL,
			   GEPON_SEL, 0);
	regmap_read(priv->pon_shell_regmap, MODE_SEL, &val);

	dev_dbg(priv->dev, "WAN MODE: PON_SHELL_MODE SEL: 0x%x\n", val);
}

static void pon_wan_xpcs_assert(struct wan_xpcs_phy *priv)
{
	/* XPCS rst assert, 1 mean deassert */
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL, XPCS_RST_N, 0);
	/* PHY rst assert */
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL, PHY_RST_N, 0);
	/* PON rst assert */
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL, PON_RST_N, 0);

	usleep_range(5, 10);
}

static void pon_wan_xpcs_deassert(struct wan_xpcs_phy *priv)
{
	/* XPCS rst deassert, 1 mean deassert */
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL,
			   XPCS_RST_N, XPCS_RST_N);
	/* PHY rst deassert */
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL,
			   PHY_RST_N, PHY_RST_N);

	usleep_range(5, 10);
}

static int lgm_wan_xpcs_phy_init(struct wan_xpcs_phy *priv)
{
	struct device *dev = priv->dev;
	unsigned int val;
	int ret = 0;

	/* deassert reset at RCU before access to PON_SHELL reg*/
	ret = wan_xpcs_phy_trigger_reset(priv, false);
	if (ret < 0)
		return ret;

	/* APB timer APB CLKFREQ must be 200 */
	wan_phy_update_bits(priv->app_base, APB_TIMEOUT_CTRL,
			    APB_CLK_FRQ_MASK, 200);

	/* Disable SRAM ByPASS */
	regmap_update_bits(priv->pon_shell_regmap, SRAM_SCR, 1, 0);
	regmap_read(priv->pon_shell_regmap, SRAM_SCR, &val);
	dev_dbg(dev, "Disable SRAM BYPASS: 0x%x\n", val);

	/* Enable internal PAD clock */
	regmap_update_bits(priv->pon_shell_regmap, GEN_CTRL,
			   REF_CLK_EN, REF_CLK_EN);

	/* Disable AON CLK GATE and CR_PARA CLK GATE,  enable clk for xpcs */
	pon_aon_clk_gate_cfg(priv, 1);

	/* Configure WAN mode */
	ponwanphy_mode(priv);

	/* reset assert */
	pon_wan_xpcs_assert(priv);

	/* has to reset clk gate after reset */
	pon_aon_clk_gate_cfg(priv, 1);

	/* PMA and PCS power stable */
	regmap_update_bits(priv->pon_shell_regmap, EXT_PG_CTRL,
			   PMA_PWR_STABLE, PMA_PWR_STABLE);
	regmap_update_bits(priv->pon_shell_regmap, EXT_PG_CTRL,
			   PCS_PWR_STABLE, PCS_PWR_STABLE);
	usleep_range(5, 10);
	regmap_read(priv->pon_shell_regmap, EXT_PG_CTRL, &val);
	dev_dbg(dev, "PON SHELL EXT PG CTRL: PMA PCS PWR Stable: 0x%x\n", val);

	/* Output Enable for CML MUX to PHY REFCLK */
	regmap_update_bits(priv->pon_shell_regmap, MODE_SEL,
			   REFCLK_EN, REFCLK_EN);
	regmap_read(priv->pon_shell_regmap, MODE_SEL, &val);
	dev_dbg(dev, "Enable CML MUX Output: 0x%x\n", val);

	/* reset deassert */
	pon_wan_xpcs_deassert(priv);

	regmap_read(priv->pon_shell_regmap, GEN_CTRL, &val);
	dev_info(dev, "wan xpcs init done!\n");

	return 0;
}

static int lgm_wan_xpcs_phy_exit(struct wan_xpcs_phy *priv)
{
	pon_wan_xpcs_assert(priv);

	return wan_xpcs_phy_trigger_reset(priv, true);
}

static void
cr_reg_dump(struct wan_xpcs_phy *priv, char *str, unsigned int off)
{
	struct device *dev = priv->dev;
	u32 val;

	val = readl(priv->cr_base + CR_ADDR(off));
	dev_dbg(dev, "%s: 0x%08x\n", str, val);
}

static int lgm_wan_xpcs_phy_calibrate(struct wan_xpcs_phy *priv)
{
	struct device *dev = priv->dev;
	int i, ret;
	u32 val;

	/* print RX value before Adapatation */
	for (i = 0; i < 2; i++)
		cr_reg_dump(priv, "LANEN_DIG_ASIC_RX_EQ_ASIC_IN",
			    RX_EQ_ASIC_IN(i));

	/* trigger auto RX adaptation */
	val = readl(priv->cr_base + CR_ADDR(PCS_XF_ATE_OVRD_IN_2));
	val |= ADAPT_REQ | ADAPT_REQ_OVRD_EN;
	writel(val, priv->cr_base + CR_ADDR(PCS_XF_ATE_OVRD_IN_2));

	/* Wait RX Adaptation finish */
	ret = readl_poll_timeout(priv->cr_base + CR_ADDR(PCS_XF_RX_ADAPT_ACK),
				 val, val & RX_ADAPT_ACK, 10, 50000);
	if (ret)
		dev_err(dev, "RX Adaptation failed!\n");
	else
		dev_info(dev, "RX Adaptation success!\n");

	/* Stop RX adaptation */
	val = readl(priv->cr_base + CR_ADDR(PCS_XF_ATE_OVRD_IN_2));
	val &= ~(ADAPT_REQ | ADAPT_REQ_OVRD_EN);
	writel(val, priv->cr_base + CR_ADDR(PCS_XF_ATE_OVRD_IN_2));
	usleep_range(10, 20);

	/* Debug information */
	/* Display RX Adapt value in PMA */
	cr_reg_dump(priv, "LANEN_DIG_RX_ADPTCTL_ATT_STATUS",
		    RX_ADPTCTL_ATT_STATUS);

	cr_reg_dump(priv, "LANEN_DIG_RX_ADPTCTL_VGA_STATUS",
		    RX_ADPTCTL_VGA_STATUS);

	cr_reg_dump(priv, "LANEN_DIG_RX_ADPTCTL_CTLE_STATUS",
		    RX_ADPTCTL_CTLE_STATUS);

	cr_reg_dump(priv, "LANEN_DIG_RX_ADPTCTL_DFE_TAP1_STATUS",
		    RX_ADPTCTL_DFE_TAP1_STATUS);

	/* print RX value after Adapatation */
	for (i = 0; i < 2; i++)
		cr_reg_dump(priv, "LANEN_DIG_ASIC_RX_EQ_ASIC_IN",
			    RX_EQ_ASIC_IN(i));

	return 0;
}

static const struct wan_xpcs_soc_data prx300_wan_xpcs_data = {
	.name         = "prx300-wan-xpcs",
	.rst_name[0]  = "wanss",
	.rst_name[1]  = "phy",
	.reg_init     = prx300_wan_xpcs_phy_reg_init,
	.phy_init     = prx300_wan_xpcs_phy_init,
	.power_on     = prx300_wan_xpcs_phy_power_on,
};

static const struct wan_xpcs_soc_data lgm_wan_xpcs_data = {
	.name        = "lgm-wan-xpcs",
	.rst_name[0] = "pon",
	.reg_init    = lgm_wan_xpcs_phy_reg_init,
	.phy_init    = lgm_wan_xpcs_phy_init,
	.phy_exit    = lgm_wan_xpcs_phy_exit,
	.calibrate   = lgm_wan_xpcs_phy_calibrate,
};

static const struct of_device_id of_wan_xpcs_phy_match[] = {
	{ .compatible = "mxl,prx300-wanxpcsphy",
	  .data =	&prx300_wan_xpcs_data },
	{ .compatible = "mxl,lgm-wanxpcsphy",
	  .data =	&lgm_wan_xpcs_data },
	{}
};

MODULE_DEVICE_TABLE(of, of_wan_xpcs_phy_match);

static struct platform_driver wan_xpcs_phy_driver = {
	.probe =	wan_xpcs_phy_probe,
	.remove =	wan_xpcs_phy_remove,
	.driver = {
		.name = "mxl-wan-xpcs-phy",
		.of_match_table = of_match_ptr(of_wan_xpcs_phy_match),
	}
};

module_platform_driver(wan_xpcs_phy_driver);

MODULE_AUTHOR("Peter Harliman Liem <peter.harliman.liem@maxlinear.com>");
MODULE_DESCRIPTION("Maxlinear WAN XPCS PHY driver");
MODULE_LICENSE("GPL v2");
