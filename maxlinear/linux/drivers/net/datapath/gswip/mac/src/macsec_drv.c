// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for MACsec Engine inside Maxlinear GSWIP or GPHY
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019 - 2020 Intel Corporation
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#include "mac_cfg.h"

#define MACSEC_DEV_NAME		"gsw_macsec"

#ifdef CONFIG_OF
static int eip160_parse_dts(struct platform_device *pdev,
			  struct macsec_pvt_data **pdata)
{
	struct device *dev = &pdev->dev;
	u32 prop = 0;

	(*pdata) = devm_kzalloc(dev, sizeof(struct macsec_pvt_data),
				GFP_KERNEL);

	if (!(*pdata))
		return -ENOMEM;

	/* Retrieve the macsec devid */
	if (!device_property_read_u32(dev, "e160-devid", &prop)) {
		(*pdata)->devid = prop;
		return 0;
	}

	dev_err(dev, "macsec devid: cannot get property\n");
	return -EINVAL;
}

#else
static int eip160_parse_dts(struct platform_device *pdev,
			  struct macsec_pvt_data **pdata)
{
	return -1;
}
#endif

static int eip160_macsec_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct macsec_pvt_data *pdata = NULL;
	struct device *dev = &pdev->dev;

	if (dev->of_node) {
		if (eip160_parse_dts(pdev, &pdata)) {
			dev_err(dev, "e160 dt parse failed!\n");
			return -EINVAL;
		}

		platform_set_drvdata(pdev, pdata);
	} else { /* Read private data from end point */
		pdata = dev_get_drvdata(dev);

		if (!pdata) {
			dev_err(dev,
				"Get private data from end point failed!\n");
			return -EINVAL;
		}
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "e160_reg");

	if (!res) {
		dev_err(dev, "Failed to get %s iomem res!\n", "e160_reg");
		return -ENOENT;
	}

	pdata->eip160_base = devm_ioremap_resource(&pdev->dev, res);

	if (IS_ERR(pdata->eip160_base)) {
		dev_err(dev, "Failed to ioremap resource: %pr !\n", res);
		return -EINVAL;
	}

	pdata->irq_num = platform_get_irq_byname(pdev, "e160_irq");

	if (pdata->irq_num <= 0) {
		dev_err(dev, "Cannot get eip160_irq !!\n");
		return -ENODEV;
	}

	dev_info(dev, "EIP160 %d: Initialized. Dev base 0x%p, IRQ num %d\n", pdata->devid, pdata->eip160_base, pdata->irq_num);

	return 0;
}

static int eip160_macsec_release(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id eip160_macsec_match[] = {
	{ .compatible = "intel,gsw_macsec" },
	{},
};
MODULE_DEVICE_TABLE(of, eip160_macsec_match);

static struct platform_driver eip160_macsec_driver = {
	.probe = eip160_macsec_probe,
	.remove = eip160_macsec_release,
	.driver = {
		.name = MACSEC_DEV_NAME,
		.of_match_table = of_match_ptr(eip160_macsec_match),
	}
};

module_platform_driver(eip160_macsec_driver);

MODULE_AUTHOR("Joby Thampan");
MODULE_DESCRIPTION("Intel GSW Macsec driver");
MODULE_LICENSE("GPL v2");

