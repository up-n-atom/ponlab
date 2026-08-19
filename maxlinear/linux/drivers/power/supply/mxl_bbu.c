/* SPDX-License-Identifier: GPL-2.0 */
/*
 * This module simulates backup battery presence in the system.
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/of.h>

static int bbu_online = 0;  /* 0 = offline, 1 = online */
struct power_supply *psy;

static int bbu_get_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val)
{
	int *online = power_supply_get_drvdata(psy);

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = *online;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int bbu_set_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    const union power_supply_propval *val)
{
	int *online  = power_supply_get_drvdata(psy);

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		*online = val->intval;
		power_supply_changed(psy);
		return 0;
	default:
		return -EINVAL;
	}

	return 0;
}

static int bbu_prop_writeable(struct power_supply *psy,
						enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		return 1;
	default:
		return 0;
	}
}

static enum power_supply_property bbu_props[] = {
	POWER_SUPPLY_PROP_ONLINE
};

static const struct power_supply_desc bbu_desc = {
	.name			="bbu-psy",
	.type			= POWER_SUPPLY_TYPE_BATTERY,
	.get_property		= bbu_get_property,
	.set_property		= bbu_set_property,
	.property_is_writeable	= bbu_prop_writeable,
	.properties		= bbu_props,
	.num_properties		= ARRAY_SIZE(bbu_props),
};

static int bbu_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct power_supply_config psy_cfg = {};

	if (!dev->of_node) {
		dev_err(dev, "No device node for BBU\n");
		return -ENOENT;
	}

	psy_cfg.of_node = dev->of_node;
	psy_cfg.drv_data = &bbu_online;

	platform_set_drvdata(pdev, &bbu_online);

	psy = devm_power_supply_register(dev, &bbu_desc, &psy_cfg);

	if (IS_ERR(psy)) {
		dev_err(dev, "Failed to register power supply\n");
		return PTR_ERR(psy);
	}

	return 0;
}

static const struct of_device_id bbu_match[] = {
	{ .compatible = "mxl,power-bbu" },
	{ /* sentinel */ },
};

static struct platform_driver bbu_driver = {
	.probe = bbu_probe,
	.driver = {
		.name		= "bbu",
		.of_match_table	= bbu_match,
	},
};

builtin_platform_driver(bbu_driver);
