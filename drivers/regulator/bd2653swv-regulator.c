// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation.
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
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_irq.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>

#define VEND_ID		0x0

#define BUCK_VID(n)	(0x0a + ((n) << 2))
#define BUCK_VID_MASK	0xff

#define ROHM_GPIO0_OUT_CTRL	0x90
#define GPIO0_OUT_MODE_MSK	BIT(3)
#define GPIO0_OUT_MSK		BIT(0)

#define ROHM_CPU_IRQLVL1	0x84
#define ROHM_CPU_IRQLVL1_MSK	0x85
#define ROHM_THERM_IRQ_MSK	BIT(0)
#define ROHM_REQ_IRQ_MSK	BIT(1)
#define ROHM_PBTN_IRQ_MSK	BIT(2)

#define ROHM_CPU_IRQLVL2_THERM	0x86
#define ROHM_IRQLVL2_THERM_MSK	0x87
#define PMICHOT_IRQ_MSK		BIT(0)

#define ROHM_CPU_IRQLVL2_REQ	0x88
#define ROHM_IRQLVL2_REQ_MSK	0x89
#define REQ_FAIL_IRQ_MSK	BIT(0)

#define ROHM_CPU_IRQLVL2_PBTN	0x8a
#define ROHM_IRQLVL2_PBTN_MSK	0x8b
#define PRESS_OFF_IRQ_MSK	BIT(0)

#define ROHM_VENDOR_SPARE0	0x94
#define ROHM_BUCK3_CR_MSK	BIT(3)
#define ROHM_BUCK3_CR_KEEP	BIT(3)

#define ROHM_VENDOR_ID		0x1f
#define RICHTEK_VENDOR_ID	0x82
#define MPS_VENDOR_ID		0x01

static bool roc_rail_power_on;

static const struct linear_range rohm_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(0, 0x0, 0x0, 0),
	REGULATOR_LINEAR_RANGE(500000, 0x1, 0xab, 5000),
};

static const struct linear_range richtek_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(0, 0x0, 0x0, 0),
	REGULATOR_LINEAR_RANGE(500000, 0x1, 0x7f, 6250),
};

static const struct linear_range mps_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(0, 0x0, 0x0, 0),
	REGULATOR_LINEAR_RANGE(500000, 0x1, 0x7f, 10000),
};

static const struct regulator_ops _regulator_ops = {
	.list_voltage		= regulator_list_voltage_linear_range,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
};

#define BUCK(_name, _of_match, _index, _ranges)	\
{\
	.name		= _name,		\
	.of_match	= _of_match,		\
	.ops		= &_regulator_ops,	\
	.type		= REGULATOR_VOLTAGE,	\
	.linear_ranges	= _ranges,		\
	.n_linear_ranges = ARRAY_SIZE(_ranges),	\
	.vsel_reg	= BUCK_VID(_index),	\
	.vsel_mask	= BUCK_VID_MASK,	\
	.owner		= THIS_MODULE,		\
}

static const struct regulator_desc rohm_regulators[] = {
	BUCK("BUCK0", "buck0", 0, rohm_voltage_ranges),
	BUCK("BUCK1", "buck1", 1, rohm_voltage_ranges),
	BUCK("BUCK2", "buck2", 2, rohm_voltage_ranges),
	BUCK("BUCK3", "buck3", 3, rohm_voltage_ranges),
};

static const struct regulator_desc richtek_regulators[] = {
	BUCK("BUCK0", "buck0", 0, richtek_voltage_ranges),
	BUCK("BUCK1", "buck1", 1, richtek_voltage_ranges),
	BUCK("BUCK2", "buck2", 2, richtek_voltage_ranges),
	BUCK("BUCK3", "buck3", 3, richtek_voltage_ranges),
	BUCK("BUCK4", "buck4", 4, richtek_voltage_ranges),
};

static const struct regulator_desc mps_regulators[] = {
	BUCK("BUCK0", "buck0", 0, mps_voltage_ranges),
	BUCK("BUCK1", "buck1", 1, mps_voltage_ranges),
	BUCK("BUCK2", "buck2", 2, mps_voltage_ranges),
	BUCK("BUCK3", "buck3", 3, mps_voltage_ranges),
};

static irqreturn_t pmic_isr(int irq, void *dev_id)
{
	struct regmap *regmap = (struct regmap *)dev_id;
	u32 val;
	irqreturn_t ret = IRQ_NONE;

	regmap_read(regmap, ROHM_CPU_IRQLVL1, &val);
	pr_emerg("pmic interrupt is triggered, CPU_IRQLVL1 is 0x%x\n", val);

	if (val & ROHM_THERM_IRQ_MSK) {
		regmap_write(regmap, ROHM_CPU_IRQLVL2_THERM, 1);
		ret = IRQ_HANDLED;
		pr_emerg("pmic is too hot, restarting system!\n");
		kernel_restart(NULL);
	}

	if (val & ROHM_REQ_IRQ_MSK) {
		regmap_write(regmap, ROHM_CPU_IRQLVL2_REQ, 1);
		ret = IRQ_HANDLED;
		pr_info("pmic req interrupt is triggered\n");
	}

	if (val & ROHM_PBTN_IRQ_MSK) {
		regmap_write(regmap, ROHM_CPU_IRQLVL2_PBTN, 1);
		ret = IRQ_HANDLED;
		pr_info("pmic pbtn interrupt is triggered\n");
	}

	return ret;
}

static const struct regmap_config pmic_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

static int pmic_i2c_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct regmap *regmap;
	const struct regulator_desc *desc;
	char *vend_name;
	int num_regulators;
	struct regulator_config config = { };
	struct regulator_dev *rdev;
	u32 val;
	int i, ret;
	u64 vol;

	regmap = devm_regmap_init_i2c(client, &pmic_regmap_config);
	if (IS_ERR(regmap)) {
		ret = PTR_ERR(regmap);
		dev_err(dev, "Failed to allocate register map: %d\n", ret);
		return ret;
	}

	ret = regmap_read(regmap, VEND_ID, &val);
	if (ret < 0) {
		dev_err(dev, "Failed to read vendor ID: %d\n", ret);
		return ret;
	}

	switch (val) {
	case ROHM_VENDOR_ID:
		desc = &rohm_regulators[0];
		num_regulators = ARRAY_SIZE(rohm_regulators);
		vend_name = "ROHM";
		vol = 500000ULL << 32 | 5000ULL;
		break;
	case RICHTEK_VENDOR_ID:
		desc = &richtek_regulators[0];
		num_regulators = ARRAY_SIZE(richtek_regulators);
		vend_name = "RICHTEK";
		vol = 500000ULL << 32 | 6250ULL;
		break;
	case MPS_VENDOR_ID:
		desc = &mps_regulators[0];
		num_regulators = ARRAY_SIZE(mps_regulators);
		vend_name = "MPS";
		vol = 500000ULL << 32 | 10000ULL;
		break;
	default:
		dev_err(dev, "PMIC vendor ID error: %x\n", val);
		return -EINVAL;
	}

	dev_info(dev, "Vendor name is %s, id is 0x%x\n", vend_name, val);

	if (val == ROHM_VENDOR_ID) {
		int irq;

		regmap_update_bits(regmap, ROHM_GPIO0_OUT_CTRL,
				   GPIO0_OUT_MODE_MSK | GPIO0_OUT_MSK, 0x9);

		if (roc_rail_power_on) {
			regmap_update_bits(regmap, ROHM_VENDOR_SPARE0,
					   ROHM_BUCK3_CR_MSK,
					   ROHM_BUCK3_CR_KEEP);
		} else {
			regmap_update_bits(regmap, ROHM_VENDOR_SPARE0,
					   ROHM_BUCK3_CR_MSK, 0);
		}

		irq = of_irq_get(dev->of_node, 0);
		if (irq < 0) {
			ret = irq;
			dev_warn(dev, "failed to get irq: %d\n", irq);
			goto create_regulator;
		}

		ret = devm_request_threaded_irq(dev, irq, NULL, pmic_isr,
						IRQF_ONESHOT |
						IRQF_TRIGGER_HIGH,
						"lgm-pmic", regmap);
		if (ret < 0) {
			dev_warn(dev,
				 "can't register ISR for IRQ %u (ret=%i)\n",
				 irq, ret);
			goto create_regulator;
		}

		regmap_write(regmap, ROHM_CPU_IRQLVL1_MSK,
			     FIELD_PREP(ROHM_THERM_IRQ_MSK, 0) |
			     FIELD_PREP(ROHM_REQ_IRQ_MSK, 0) |
			     FIELD_PREP(ROHM_PBTN_IRQ_MSK, 0));

		regmap_update_bits(regmap, ROHM_IRQLVL2_THERM_MSK,
				   PMICHOT_IRQ_MSK,
				   FIELD_PREP(PMICHOT_IRQ_MSK, 0));

		regmap_update_bits(regmap, ROHM_IRQLVL2_REQ_MSK,
				   REQ_FAIL_IRQ_MSK,
				   FIELD_PREP(REQ_FAIL_IRQ_MSK, 0));

		regmap_update_bits(regmap, ROHM_IRQLVL2_PBTN_MSK,
				   PRESS_OFF_IRQ_MSK,
				   FIELD_PREP(PRESS_OFF_IRQ_MSK, 0));
	}

create_regulator:
	config.dev = dev;
	config.regmap = regmap;

	for (i = 0; i < num_regulators; i++) {
		rdev = devm_regulator_register(dev, desc++, &config);
		if (IS_ERR(rdev)) {
			dev_err(dev, "failed to register %s\n", desc->name);
			return PTR_ERR(rdev);
		}
	}

	epu_notifier_blocking_chain(PMIC_EVENT_REG, &vol);

	return 0;
}

static int __init roc_power_on_setup(char *__unused)
{
	roc_rail_power_on = true;

	return 1;
}
__setup("roc_power_on", roc_power_on_setup);

static const struct of_device_id pmic_ids[] = {
	{ .compatible = "bd2653swv" },
	{ },
};
MODULE_DEVICE_TABLE(of, pmic_ids);

static struct i2c_driver pmic_regulator_driver = {
	.driver = {
		.name = "lgm-pmic",
		.of_match_table = of_match_ptr(pmic_ids),
	},
	.probe	= pmic_i2c_probe,
};
module_i2c_driver(pmic_regulator_driver);
