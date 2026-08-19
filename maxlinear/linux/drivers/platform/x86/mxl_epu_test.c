// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 MaxLinear, Inc.
 */

#include <linux/debugfs.h>
#include <linux/devm-helpers.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define DRV_NAME "epu_test"

struct epu_test_priv {
	struct device *dev;
	struct dentry *debugfs;
	unsigned int pvt_int_en;
	struct work_struct pvt_work;
};

static int pvt_int_set(void *data, u64 val)
{
	struct epu_test_priv *priv = data;

	priv->pvt_int_en = !!val;
	schedule_work(&priv->pvt_work);

	return 0;
}

static int pvt_int_get(void *data, u64 *val)
{
	struct epu_test_priv *priv = data;

	*val = (u64)priv->pvt_int_en;

	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(pvt_int_fops, pvt_int_get,
			 pvt_int_set, "%llu\n");
void epu_test_debugfs_init(struct epu_test_priv *priv)
{
	struct dentry *dir;

	dir = debugfs_create_dir(DRV_NAME, NULL);

	if (!dir)
		return;

	debugfs_create_file("pvt_interrupt", 0644, dir, priv,
			    &pvt_int_fops);

	priv->debugfs = dir;
}

static void pvt_alarm_work(struct work_struct *work)
{
	struct epu_test_priv *priv = container_of(work, struct epu_test_priv, pvt_work);
	int cpu = 0;

	epu_notifier_blocking_chain(I2C_SEM_EVENT_REQUEST, &cpu);
	epu_notifier_blocking_chain(I2C_SEM_EVENT_RELEASE, NULL);

	if (priv->pvt_int_en) {
		schedule();
		schedule_work(&priv->pvt_work);
	}
}

static int epu_test_probe(struct platform_device *pdev)
{
	struct epu_test_priv *priv;
	struct device *dev = &pdev->dev;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->dev = dev;
	epu_test_debugfs_init(priv);
	devm_work_autocancel(dev, &priv->pvt_work, pvt_alarm_work);
	platform_set_drvdata(pdev, priv);

	return 0;
}

static int epu_test_remove(struct platform_device *pdev)
{
	struct epu_test_priv *priv = platform_get_drvdata(pdev);

	debugfs_remove_recursive(priv->debugfs);

	return 0;
}

static const struct of_device_id epu_test_of_match[] = {
	{ .compatible = "mxl,epu-test" },
	{ }
};
MODULE_DEVICE_TABLE(of, epu_test_of_match);

static struct platform_driver epu_test_driver = {
	.driver = {
		.name = "epu-test",
		.of_match_table = epu_test_of_match,
	},
	.probe = epu_test_probe,
	.remove = epu_test_remove,
};
module_platform_driver(epu_test_driver);

MODULE_DESCRIPTION(DRV_NAME);
MODULE_LICENSE("GPL v2");
