// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation.
 * Lei Chuanhua <lchuanhua@maxlinear.com>
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

#include <linux/bitfield.h>
#include <linux/init.h>
#include <linux/mfd/syscon.h>
#include <linux/of_device.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/platform_data/lgm_epu_rcu.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/reset-controller.h>

#define RCU_RST_STAT	0x0024
#define RCU_RST_REQ	0x0048

#define REG_OFFSET_MASK	GENMASK(31, 16)
#define BIT_OFFSET_MASK	GENMASK(15, 8)
#define STAT_BIT_OFFSET_MASK	GENMASK(7, 0)

#define to_reset_data(x)	container_of(x, struct intel_reset_data, rcdev)

struct intel_reset_soc {
	bool legacy;
	u32 reset_cell_count;
};

struct intel_reset_data {
	struct reset_controller_dev rcdev;
	struct notifier_block restart_nb;
	struct notifier_block epu_nb;
	const struct intel_reset_soc *soc_data;
	struct regmap *regmap;
	struct device *dev;
	u32 reboot_id;
};

/*
 * Reset status register offset relative to
 * the reset control register(X) is X + 4
 */
static u32 id_to_reg_and_bit_offsets(struct intel_reset_data *data,
				     unsigned long id, u32 *rst_req,
				     u32 *req_bit, u32 *stat_bit)
{
	*rst_req = FIELD_GET(REG_OFFSET_MASK, id);
	*req_bit = FIELD_GET(BIT_OFFSET_MASK, id);

	if (data->soc_data->legacy)
		*stat_bit = FIELD_GET(STAT_BIT_OFFSET_MASK, id);
	else
		*stat_bit = *req_bit;

	if (data->soc_data->legacy && *rst_req == RCU_RST_REQ)
		return RCU_RST_STAT;
	else
		return *rst_req + 0x4;
}

static int intel_set_timeout(struct intel_reset_data *data, u32 reg, u32 val, u32 set, u32 mask)
{
	int ret;

	if (in_atomic()) {
		ret = regmap_read_poll_timeout_atomic(data->regmap, reg, val,
						      set == !!(val & mask), 20, 200);
	
	} else {
		ret = regmap_read_poll_timeout(data->regmap, reg, val,
				               set == !!(val & mask), 20, 200);
	}

	return ret;
}

static int intel_set_clr_bits(struct intel_reset_data *data, unsigned long id,
			      bool set)
{
	u32 rst_req, req_bit, rst_stat, stat_bit, val;
	int ret;

	rst_stat = id_to_reg_and_bit_offsets(data, id, &rst_req,
					     &req_bit, &stat_bit);

	val = set ? BIT(req_bit) : 0;
	ret = regmap_update_bits(data->regmap, rst_req,  BIT(req_bit), val);
	if (ret)
		return ret;

	return intel_set_timeout(data, rst_stat, val, set, BIT(stat_bit));
}

static int intel_assert_device_rcu(struct reset_controller_dev *rcdev,
				   unsigned long id)
{
	struct intel_reset_data *data = to_reset_data(rcdev);
	int ret;

	ret = intel_set_clr_bits(data, id, true);
	if (ret)
		dev_err(data->dev, "Reset assert failed %d\n", ret);

	return ret;
}

static int intel_deassert_device_rcu(struct reset_controller_dev *rcdev,
				     unsigned long id)
{
	struct intel_reset_data *data = to_reset_data(rcdev);
	int ret;

	ret = intel_set_clr_bits(data, id, false);
	if (ret)
		dev_err(data->dev, "Reset deassert failed %d\n", ret);

	return ret;
}

static int intel_reset_status(struct reset_controller_dev *rcdev,
			      unsigned long id)
{
	struct intel_reset_data *data = to_reset_data(rcdev);
	u32 rst_req, req_bit, rst_stat, stat_bit, val;
	int ret;

	rst_stat = id_to_reg_and_bit_offsets(data, id, &rst_req,
					     &req_bit, &stat_bit);
	ret = regmap_read(data->regmap, rst_stat, &val);
	if (ret)
		return ret;

	return !!(val & BIT(stat_bit));
}

static int intel_deassert_device(struct reset_controller_dev *rcdev,
				 unsigned long id)
{
	struct intel_reset_data *data = to_reset_data(rcdev);
	int epuid;
	int ret;

	if (!intel_reset_status(rcdev, id)) {
		dev_dbg(data->dev, "deassert ignored for id: 0x%lx\n", id);
		return 0;
	}

	epuid = lgm_rcu_to_epu_id(id);
	if (epuid >= 0) {
		dev_dbg(data->dev, "rcu deassert set epu %u control to D0\n",
			epuid);
		ret = epu_notifier_raw_chain(RCU_EVENT_SET_D0(epuid), 0);
		if (notifier_to_errno(ret)) {
			dev_err(data->dev, "EPU control set %u to D0 failed, rcu id: 0x%lx\n",
				epuid, id);
			return ret;
		}
	} else {
		dev_dbg(data->dev, "NO EPU ID for RCU 0x%lx\n", id);
	}

	ret = intel_deassert_device_rcu(rcdev, id);
	if (ret) {
		dev_err(data->dev,
			"Failed to set deassert rcu id 0x%lx\n", id);
		return ret;
	}

	if (epuid >= 0) {
		dev_dbg(data->dev, "rcu deassert check epu %u status to D0\n",
			epuid);
		ret = epu_notifier_raw_chain(RCU_EVENT_GET_D0(epuid), 0);
		if (notifier_to_errno(ret)) {
			dev_err(data->dev, "EPU status set %u to D0 failed, rcu id: 0x%lx\n",
				epuid, id);
			return ret;
		}
	}

	return 0;
}

static int intel_assert_device(struct reset_controller_dev *rcdev,
			       unsigned long id)
{
	int ret;
	int epuid;
	struct intel_reset_data *data = to_reset_data(rcdev);

	if (intel_reset_status(rcdev, id)) {
		dev_dbg(data->dev, "assert ignored for id: 0x%lx\n", id);
		return 0;
	}

	epuid = lgm_rcu_to_epu_id(id);
	if (epuid >= 0) {
		dev_dbg(data->dev, "rcu assert set epu %u to D1\n", epuid);
		ret = epu_notifier_raw_chain(RCU_EVENT_SET_D1(epuid), 0);
		if (notifier_to_errno(ret)) {
			dev_err(data->dev, "EPU set %d to D1 failed, rcu id: 0x%lx\n",
				epuid, id);
			return ret;
		}
	} else {
		dev_dbg(data->dev, "NO EPU ID for RCU 0x%lx\n", id);
	}

	return intel_assert_device_rcu(rcdev, id);
}


static const struct reset_control_ops intel_reset_ops = {
	.assert =	intel_assert_device,
	.deassert =	intel_deassert_device,
	.status	=	intel_reset_status,
};

static int intel_reset_xlate(struct reset_controller_dev *rcdev,
			     const struct of_phandle_args *spec)
{
	struct intel_reset_data *data = to_reset_data(rcdev);
	u32 id;

	if (spec->args[1] > 31)
		return -EINVAL;

	id = FIELD_PREP(REG_OFFSET_MASK, spec->args[0]);
	id |= FIELD_PREP(BIT_OFFSET_MASK, spec->args[1]);

	if (data->soc_data->legacy) {
		if (spec->args[2] > 31)
			return -EINVAL;

		id |= FIELD_PREP(STAT_BIT_OFFSET_MASK, spec->args[2]);
	}

	/* dummy status read to clear the default status */
	intel_reset_status(rcdev, id);

	return id;
}

static int intel_reset_restart_handler(struct notifier_block *nb,
				       unsigned long action, void *data)
{
	struct intel_reset_data *reset_data;

	reset_data = container_of(nb, struct intel_reset_data, restart_nb);
	intel_assert_device(&reset_data->rcdev, reset_data->reboot_id);

	return NOTIFY_DONE;
}

static int intel_reset_epu_handler(struct notifier_block *nb,
				   unsigned long epuid, void *data)
{
	struct intel_reset_data *reset_data =
		container_of(nb, struct intel_reset_data, epu_nb);
	const int *rcuids = lgm_epu_to_rcu_id(epuid);
	int i;

	if (!rcuids) {
		dev_dbg(reset_data->dev, "EPU %lu does not have RCU id\n",
			epuid);
		return 0;
	}

	dev_dbg(reset_data->dev, "epu %lu deassert rcuid: 0x%x\n",
		epuid, rcuids[0]);

	for (i = 0; rcuids[i] != 0; i++) {
		if (!intel_reset_status(&reset_data->rcdev, rcuids[i]))
			continue;
		if (intel_deassert_device_rcu(&reset_data->rcdev, rcuids[i]))
			return -EIO;
	}

	return 0;
}


static int intel_reset_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device *dev = &pdev->dev;
	struct intel_reset_data *data;
	void __iomem *base;
	u32 rb_id[3];
	int ret;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->soc_data = of_device_get_match_data(dev);
	if (!data->soc_data)
		return -ENODEV;

	base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(base))
		return PTR_ERR(base);

	data->regmap = syscon_node_to_regmap(np);
	if (IS_ERR(data->regmap)) {
		dev_err(dev, "regmap initialization failed\n");
		return PTR_ERR(data->regmap);
	}

	ret = device_property_read_u32_array(dev, "intel,global-reset", rb_id,
					     data->soc_data->reset_cell_count);
	if (ret) {
		dev_err(dev, "Failed to get global reset offset!\n");
		return ret;
	}

	data->dev =			dev;
	data->rcdev.of_node =		np;
	data->rcdev.owner =		dev->driver->owner;
	data->rcdev.ops	=		&intel_reset_ops;
	data->rcdev.of_xlate =		intel_reset_xlate;
	data->rcdev.of_reset_n_cells =	data->soc_data->reset_cell_count;
	ret = devm_reset_controller_register(&pdev->dev, &data->rcdev);
	if (ret)
		return ret;

	data->reboot_id = FIELD_PREP(REG_OFFSET_MASK, rb_id[0]);
	data->reboot_id |= FIELD_PREP(BIT_OFFSET_MASK, rb_id[1]);

	if (data->soc_data->legacy)
		data->reboot_id |= FIELD_PREP(STAT_BIT_OFFSET_MASK, rb_id[2]);

	data->restart_nb.notifier_call =	intel_reset_restart_handler;
	data->restart_nb.priority =		128;
	register_restart_handler(&data->restart_nb);

	data->epu_nb.notifier_call = intel_reset_epu_handler;
	ret = epu_rcu_notify_register(&data->epu_nb);
	if (ret)
		dev_warn(dev, "Failed to register epu nb handler\n");

	return 0;
}

static const struct intel_reset_soc xrx200_data = {
	.legacy =		true,
	.reset_cell_count =	3,
};

static const struct intel_reset_soc lgm_data = {
	.legacy =		false,
	.reset_cell_count =	2,
};

static const struct of_device_id intel_reset_match[] = {
	{ .compatible = "intel,rcu-lgm", .data = &lgm_data },
	{ .compatible = "intel,rcu-xrx200", .data = &xrx200_data },
	{}
};

static struct platform_driver intel_reset_driver = {
	.probe = intel_reset_probe,
	.driver = {
		.name = "intel-reset",
		.of_match_table = intel_reset_match,
	},
};

static int __init intel_reset_init(void)
{
	return platform_driver_register(&intel_reset_driver);
}

subsys_initcall(intel_reset_init);
