// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for the Pericom PI4IOE5V6408 GPIO Expander.
 *
 * Copyright (C) 2018 Google, LLC.
 * Copyright (C) 2023 Maxlinear INC.
 */
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/regmap.h>

#define PI4IO_CHIP_ID			0x1
#define PI4IO_IO_DIRECTION		0x3
#define PI4IO_OUTPUT			0x5
#define PI4IO_OUTPUT_HI_IMPEDANCE	0x7
#define PI4IO_INPUT_DFT_STATE		0x9
#define PI4IO_PULL_UPDOWN_ENABLE	0xB
#define PI4IO_PULL_UPDOWN_SELECT	0xD
#define PI4IO_INPUT_STATUS		0xF
#define PI4IO_INTERRUPT_MASK		0x11
#define PI4IO_INTERRUPT_STATUS		0x13

#define PI4IO_CHIP_ID_VAL		0xA0
#define PI4IO_CHIP_ID_MASK		0xFC

#define PI4IO_DIRECTION_TO_GPIOD(x) ((x) ? GPIOF_DIR_OUT : GPIOF_DIR_IN)
#define GPIOD_DIRECTION_TO_PI4IO(x) ((x) == GPIOF_DIR_OUT ? 1 : 0)

#define GPIO_OUT_LOW 0
#define GPIO_OUT_HIGH 1

static bool pi4io_readable_reg(struct device *dev, unsigned int reg)
{
	/* All readable registers are odd-numbered. */
	return reg & 0x1;
}

static bool pi4io_writeable_reg(struct device *dev, unsigned int reg)
{
	/* All odd-numbered registers are writable except for 0xF and 0x13. */
	if (reg & 0x1) {
		if (reg != PI4IO_INPUT_STATUS && reg != PI4IO_INTERRUPT_STATUS) {
			return true;
		}
	}

	return (reg & 0x1) && (reg != PI4IO_INPUT_STATUS && reg != PI4IO_INTERRUPT_STATUS);
}

static bool pi4io_volatile_reg(struct device *dev, unsigned int reg)
{
	if (reg == PI4IO_INPUT_STATUS || reg == PI4IO_INTERRUPT_STATUS) {
		return true;
	}

	return false;
}

static const struct regmap_config pi4io_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = 0x13,
	.writeable_reg = pi4io_writeable_reg,
	.readable_reg = pi4io_readable_reg,
	.volatile_reg = pi4io_volatile_reg,
};

struct pi4io_priv {
	struct i2c_client *i2c;
	struct regmap *regmap;
	struct gpio_chip gpio;
	int reset_gpio;
};

static int pi4io_gpio_get_direction(struct gpio_chip *chip, unsigned offset)
{
	int ret;
	unsigned int dir;
	struct pi4io_priv *pi4io = gpiochip_get_data(chip);
	struct device *dev = &pi4io->i2c->dev;

	ret = regmap_read(pi4io->regmap, PI4IO_IO_DIRECTION, &dir);
	if (ret) {
		dev_err(dev, "Failed to read I/O direction: %d", ret);
		return ret;
	}

	return PI4IO_DIRECTION_TO_GPIOD((dir >> offset) & 1);
}

static int pi4io_gpio_enable_output(struct gpio_chip *chip, unsigned offset, bool enable)
{
	int ret, value;
	struct pi4io_priv *pi4io = gpiochip_get_data(chip);
	struct device *dev = &pi4io->i2c->dev;

	/* enable output: set corresponding bit to 0 */
	value = enable ? 0: BIT(offset);
	ret = regmap_update_bits(pi4io->regmap, PI4IO_OUTPUT_HI_IMPEDANCE,
				 BIT(offset), value);
	if (ret)
		dev_err(dev, "Failed to enable output: pin: %u, %d\n", offset, ret);

	return ret;
}

static int
pi4io_gpio_set_direction(struct gpio_chip *chip, unsigned offset, int direction)
{
	int ret;
	struct pi4io_priv *pi4io = gpiochip_get_data(chip);
	struct device *dev = &pi4io->i2c->dev;

	ret = regmap_update_bits(pi4io->regmap, PI4IO_IO_DIRECTION, BIT(offset),
				 GPIOD_DIRECTION_TO_PI4IO(direction) << offset);
	if (ret) {
		dev_err(dev, "Failed to set direction: %d\n", ret);
		return ret;
	}

	if (direction == GPIOF_DIR_OUT)
		pi4io_gpio_enable_output(chip, offset, true);
	else
		pi4io_gpio_enable_output(chip, offset, false);

	return ret;
}

static int pi4io_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	int ret;
	unsigned int dir, val;
	struct pi4io_priv *pi4io = gpiochip_get_data(chip);
	struct device *dev = &pi4io->i2c->dev;

	/* get gpio direction */
	dir = pi4io_gpio_get_direction(chip, offset);
	if (dir == GPIOF_DIR_OUT) {
		ret = regmap_read(pi4io->regmap, PI4IO_OUTPUT, &val);
		if (ret) {
			dev_err(dev, "Failed to read output: GPIO %d: %d\n", offset, ret);
			return ret;
		}

	} else {
		ret = regmap_read(pi4io->regmap, PI4IO_INPUT_STATUS, &val);
		if (ret) {
			dev_err(dev, "Failed to read input: GPIO: %d: %d\n", offset, ret);
			return ret;
		}
	}

	return !!(val & BIT(offset));
}

static void pi4io_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	int ret;
	struct pi4io_priv *pi4io = gpiochip_get_data(chip);
	struct device *dev = &pi4io->i2c->dev;

	value = value ? BIT(offset) : 0;
	ret = regmap_update_bits(pi4io->regmap, PI4IO_OUTPUT,
				 BIT(offset), value);
	if (ret)
		dev_err(dev, "Failed to write output: pin %u: %d\n", offset, ret);
}

static int pi4io_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	return pi4io_gpio_set_direction(chip, offset, GPIOF_DIR_IN);
}

static int
pi4io_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	int ret;
	struct pi4io_priv *pi4io = gpiochip_get_data(chip);
	struct device *dev = &pi4io->i2c->dev;

	pi4io_gpio_set(chip, offset, value);

	ret = pi4io_gpio_set_direction(chip, offset, GPIOF_DIR_OUT);
	if (ret) {
		dev_err(dev, "Failed to set direction: %d\n", ret);
		return ret;
	}

	return 0;
}

static int pi4io_gpio_setup(struct pi4io_priv *pi4io)
{
	int ret;
	struct device *dev = &pi4io->i2c->dev;
	struct gpio_chip *gc = &pi4io->gpio;

	gc->ngpio = 8;
	gc->label = pi4io->i2c->name;
	gc->parent = &pi4io->i2c->dev;
	gc->owner = THIS_MODULE;
	gc->base = -1;
	gc->can_sleep = true;
	gc->get_direction = pi4io_gpio_get_direction;
	gc->direction_input = pi4io_gpio_direction_input;
	gc->direction_output = pi4io_gpio_direction_output;
	gc->get = pi4io_gpio_get;
	gc->set = pi4io_gpio_set;

	ret = devm_gpiochip_add_data(dev, gc, pi4io);
	if (ret) {
		dev_err(dev, "devm_gpiochip_add_data failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int
pi4io_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret, chip_id;
	struct device *dev = &client->dev;
	struct pi4io_priv *pi4io;

	pi4io = devm_kzalloc(dev, sizeof(struct pi4io_priv), GFP_KERNEL);
	if (!pi4io)
		return -ENOMEM;

	i2c_set_clientdata(client, pi4io);
	pi4io->i2c = client;

	pi4io->reset_gpio = of_get_named_gpio(dev->of_node, "reset-gpios", 0);
	if (gpio_is_valid(pi4io->reset_gpio)) {
		ret = devm_gpio_request_one(dev, pi4io->reset_gpio, GPIOF_OUT_INIT_HIGH, "i2c-extender-reset");
		if (ret) {
			dev_err(dev, "Could not get i2c-extender-reset gpio\n");
			return -ENODEV;
		}
	} else {
		dev_info(dev, "NO i2c-extender-reset gpio\n");
	}

	/* reset pi4io */
	if (gpio_is_valid(pi4io->reset_gpio)) {
		gpio_set_value(pi4io->reset_gpio, 0);
		udelay(200);
		gpio_set_value(pi4io->reset_gpio, 1);
	}

	pi4io->regmap = devm_regmap_init_i2c(client, &pi4io_regmap);
	ret = regmap_read(pi4io->regmap, PI4IO_CHIP_ID, &chip_id);
	if (ret < 0) {
		dev_err(dev, "Failed to read Chip ID: %d\n", ret);
		return ret;
	}
	dev_info(dev, "chip id is 0x%x\n", chip_id);

	if ((chip_id & PI4IO_CHIP_ID_MASK) != PI4IO_CHIP_ID_VAL) {
		dev_err(dev, "Invalid Chip ID!\n");
		return -EINVAL;
	}

	ret = pi4io_gpio_setup(pi4io);
	if (ret < 0) {
		dev_err(dev, "Failed to setup GPIOs: %d\n", ret);
		return ret;
	}

	dev_dbg(dev, "PI4IO probe finished\n");
	return 0;
}

static const struct i2c_device_id pi4io_id_table[] = {
	{ "pi4io" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(i2c, pi4io_id_table);

static const struct of_device_id pi4io_of_match[] = {
	{ .compatible = "diodes,pi4io" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, pi4io_of_match);

static struct i2c_driver pi4io_driver = {
	.driver = {
		.name = "pi4io-gpio",
		.of_match_table = pi4io_of_match,
	},
	.probe = pi4io_probe,
	.id_table = pi4io_id_table,
};
module_i2c_driver(pi4io_driver);
MODULE_AUTHOR("Alex Van Damme <atv@google.com>");
MODULE_DESCRIPTION("PI4IOE5V6408");
MODULE_LICENSE("GPL v2");
