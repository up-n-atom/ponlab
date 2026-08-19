// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2021-2022 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <dt-bindings/power/lgm-power.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/platform_device.h>
#include <linux/sys_soc.h>

#define MPS_CHIPID	0x4
#define ID_CFG		0x14
#define FAB_PVT		0x28
#define CIP_FUSE	0x50

#define CIP_MASK	0x20

enum {
	LGM_A2 = 0x1,
	LGM_A3 = 0x2,
	LGM_B0 = 0x3,
	LGM_C0 = 0x4,
};

enum {
	PN_URX651        = 0x80,
	PN_URX850        = 0x83,
	PN_PUMA8_CE2800  = 0x84,
	PN_PUMA8_CE2810  = 0x85,
	PN_PUMA8_CE2850  = 0x86,
	PN_PUMA8_CE2800I = 0x87,
	PN_PUMA8_CE2850I = 0x88,
	PN_URX851        = 0x90,
	PN_URX641        = 0xA0,
	PN_URX640        = 0xA2,
};

#define PN_NAME(a)	#a

struct lgm_soc_priv {
	struct platform_device *pdev;
	struct device *dev;
	void __iomem *membase;
	unsigned int ver;
	unsigned int pnum;
	unsigned int cip;
};

static const unsigned int epu_fuse[32] = {
	[1]  = LGM_EPU_PD_VAULT130,
	[18] = LGM_EPU_PD_EIP197,
	[21] = LGM_EPU_PD_EMMC,
	[22] = LGM_EPU_PD_SDXC,
	[24] = LGM_EPU_PD_PON,
	[25] = LGM_EPU_PD_USB1,
	[26] = LGM_EPU_PD_USB2,
	[27] = LGM_EPU_PD_HSIO1,
	[28] = LGM_EPU_PD_HSIO2,
	[29] = LGM_EPU_PD_HSIO3,
	[30] = LGM_EPU_PD_HSIO4,
};

enum {
	CHIP_SPEED_SLOW,
	CHIP_SPEED_TYPICAL,
	CHIP_SPEED_FAST,
};

static int lgm_chip_speed(unsigned long speed, int cip)
{
	const unsigned long slow_speed = 290000000UL;
	const unsigned long fast_speed = cip == 1 ? 310000000UL: 305000000UL;

	if (speed < slow_speed)
		return CHIP_SPEED_SLOW;
	if (speed > fast_speed)
		return CHIP_SPEED_FAST;

	return CHIP_SPEED_TYPICAL;
}

/**
 * speed calculate formula:
 * speed_val = (pvt_read * A * B * FCLK) / W;
 * pvt: (bit 0 - 9)
 * A: 16,  B: 4,  FCLK: 5Mhz,  W: 255
 */
static void lgm_print_chip_speed(struct lgm_soc_priv *priv)
{
	u32 pvt_read;
	unsigned long speed;

	pvt_read = readl(priv->membase + FAB_PVT) & GENMASK(9, 0);
	speed = (unsigned long)pvt_read * 16 * 4 * 5000000;
	do_div(speed, 255);

	if (priv->cip)
		dev_info(priv->dev, "This chip is a CIP device\n");

	switch (lgm_chip_speed(speed, priv->cip)) {
	case CHIP_SPEED_SLOW:
		dev_info(priv->dev, "chip speed slow(%ld)!\n", speed);
		break;
	case CHIP_SPEED_TYPICAL:
		dev_info(priv->dev, "chip speed typical(%ld)!\n", speed);
		break;
	case CHIP_SPEED_FAST:
		dev_info(priv->dev, "chip speed fast(%ld)!\n", speed);
		break;
	default:
		break;
	}
}

static const char *lgm_print_ver(struct lgm_soc_priv *priv)
{
	static const char * const soc_ver[] = {
		"A0",
		"A2",
		"A3",
		"B0",
		"C0"

	};
	unsigned int ver = priv->ver;

	if (ver < ARRAY_SIZE(soc_ver)) {
		dev_info(priv->dev, "LGM SoC Verion: %s\n", soc_ver[ver]);
		return soc_ver[ver];
	}
	else {
		dev_err(priv->dev, "LGM SoC Verion: %u not supported\n", ver);
		return "Unknown Revision";
	}
}

static char *lgm_print_pnum(struct lgm_soc_priv *priv)
{
	char *product_name;

	switch (priv->pnum) {
	case PN_URX651:
		product_name = PN_NAME(PN_URX651);
		break;
	case PN_URX850:
		product_name = PN_NAME(PN_URX850);
		break;
	case PN_URX851:
		product_name = PN_NAME(PN_URX851);
		break;
	case PN_URX641:
		product_name = PN_NAME(PN_URX641);
		break;
	case PN_URX640:
		product_name = PN_NAME(PN_URX640);
		break;
	case PN_PUMA8_CE2800:
		product_name = PN_NAME(PN_PUMA8_CE2800);
		break;
	case PN_PUMA8_CE2810:
		product_name = PN_NAME(PN_PUMA8_CE2810);
		break;
	case PN_PUMA8_CE2850:
		product_name = PN_NAME(PN_PUMA8_CE2850);
		break;
	case PN_PUMA8_CE2800I:
		product_name = PN_NAME(PN_PUMA8_CE2800I);
		break;
	case PN_PUMA8_CE2850I:
		product_name = PN_NAME(PN_PUMA8_CE2850I);
		break;
	default:
		product_name = "not supported\n";
		dev_err(priv->dev, "product not supported id: 0x%x", priv->pnum);
		break;
	}

	dev_info(priv->dev, "LGM product name: %s\n", product_name);
	return product_name;
}

static void lgm_notify_epu_fused_domain(struct lgm_soc_priv *priv)
{
	u32 fuse = readl(priv->membase + ID_CFG);
	int i;

	for (i = 0; i < sizeof(fuse) * BITS_PER_BYTE; i++)
		if ((BIT(i) & fuse) && epu_fuse[i])
			epu_notifier_blocking_chain(SOC_PD_ID(epu_fuse[i]), 0);
}

static const char *lgm_print_dt_model_name(struct lgm_soc_priv *priv)
{
	struct device_node *np;
	const char *val;
	int i, cnt;

	np = of_find_node_by_path("/");
	if (!of_property_read_bool(np, "model")) {
		dev_err(priv->dev, "Can't find DT model info!\n");
		return NULL;
	}

	cnt = of_property_count_strings(np, "model");
	dev_info(priv->dev, "model: \n");
	for (i = 0; i < cnt; i++) {
		of_property_read_string_index(np, "model", i, &val);
		dev_info(priv->dev, "\t%s\n", val);
	}
	return val;
}

static int lgm_soc_probe(struct platform_device *pdev)
{
	struct soc_device_attribute *soc_dev_attr;
	struct soc_device *soc_dev;
	struct lgm_soc_priv *priv;
	struct resource *res;
	struct device *dev = &pdev->dev;
	u32 id;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	soc_dev_attr = devm_kzalloc(dev, sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		return -ENOMEM;

	priv->pdev = pdev;
	priv->dev = dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->membase = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->membase)) {
		dev_err(dev, "ioremap mem resource failed!\n");
		return PTR_ERR(priv->membase);
	}

	id = readl(priv->membase + MPS_CHIPID);
	priv->ver = (id & GENMASK(31, 28)) >> 28;
	priv->pnum = (id & GENMASK(27, 12)) >> 12;
	id = readl(priv->membase + CIP_FUSE);
	priv->cip = id & CIP_MASK ? 1 : 0;
	soc_dev_attr->family = "LGM";
	soc_dev_attr->revision = lgm_print_ver(priv);
	soc_dev_attr->soc_id = lgm_print_pnum(priv);
	soc_dev_attr->machine = lgm_print_dt_model_name(priv);
	lgm_notify_epu_fused_domain(priv);
	lgm_print_chip_speed(priv);

	/* please note that the actual registration will be deferred */
	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev))
		return PTR_ERR(soc_dev);

	platform_set_drvdata(pdev, soc_dev);
	dev_set_drvdata(soc_device_to_device(soc_dev), priv);

	return 0;
}

static int lgm_soc_remove(struct platform_device *pdev)
{
	struct soc_device *soc_dev = platform_get_drvdata(pdev);

	soc_device_unregister(soc_dev);

	return 0;
}

static const struct of_device_id of_lgm_soc_match[] = {
	{ .compatible = "mxl,chipid" },
	{}
};
MODULE_DEVICE_TABLE(of, of_lgm_soc_match);

static struct platform_driver lgm_soc_driver = {
	.probe = lgm_soc_probe,
	.remove = lgm_soc_remove,
	.driver = {
		.name = "lgm_soc",
		.of_match_table = of_match_ptr(of_lgm_soc_match),
	}
};

module_platform_driver(lgm_soc_driver);

MODULE_LICENSE("GPL");
