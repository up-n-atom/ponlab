// SPDX-License-Identifier: GPL-2.0
/*
 * Intel GPIO SSO driver
 *
 * Copyright (C) 2019 Intel Corporation.
 * Zhu YiXin <yixin.zhu@intel.com>
 * Wu ZhiXian <zhixian.wu@intel.com>
 */
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/reset.h>

/* reg definition */
#define DUTY_CYCLE(x)	(0x8 + ((x) * 4))
#define SSO_CON0	0x2B0
#define SSO_CON1	0x2B4
#define SSO_CPU		0x2B8
#define SSO_CON2	0x2C4
#define SSO_CON3	0x2C8

/* CON0 */
#define SWU		31
#define BLINK_R		30
#define RZFL		26

/* CON1 */
#define US		30
#define US_MASK		3
#define MAX_PIN_NUM_PER_BANK	32
#define MAX_GROUP_NUM		4
#define PINS_PER_GROUP		8

static const char * const sso_gpio_drv_name = "intel-sso-gpio";

/**
 * struct sso_gpio_priv
 *
 * @mmap: SSO mapped memory resource
 * @dev: sso basic device
 * @pdev: sso platform device
 * @gclk: sso gate clock
 * @rst:  sso reset controller
 * @rst_gpio: sso rst gpio pin (optional pin)
 * @chip: gpio controller chip
 * @pins: pin number of the gpio chip
 * @edge: output update clock edge
 * @alloc_bitmap: bitmap for allocated pins
 */
struct sso_gpio_priv {
	struct regmap *mmap;
	struct device *dev;
	struct platform_device *pdev;
	struct clk *gclk;
	struct reset_control *rst;
	struct gpio_desc *rst_gpio;
	struct gpio_chip chip;
	u32 pins;
	int gpio_base;
	int edge;
	u32 alloc_bitmap;
};

static int sso_gpio_write_mask(struct regmap *map, u32 reg, u32 off,
			       u32 mask, u32 val)
{
	u32 reg_val;

	if (regmap_read(map, reg, &reg_val))
		return -EINVAL;

	reg_val = (reg_val & ~(mask << off)) | ((val & mask) << off);

	return regmap_write(map, reg, reg_val);
}

static int sso_gpio_update_bit(struct regmap *map, u32 reg, u32 off, u32 val)
{
	bool bit;

	bit = !!val;
	return regmap_update_bits(map, reg, BIT(off), (bit << off));
}

static void sso_gpio_output_delay(struct sso_gpio_priv *priv)
{
	u32 val;

	mdelay(10); /* Delay is MUST before polling */
	if (regmap_read_poll_timeout_atomic(priv->mmap, SSO_CON0, val, (val & BIT(SWU)) == 0, 1000, 10000))
		pr_err("sso wait time out\n");
}

static void sso_gpio_set_value(struct sso_gpio_priv *priv, unsigned int offset,
			       int value)
{
	sso_gpio_update_bit(priv->mmap, SSO_CPU, offset, value);

	/* delay is required before SW issue output */
	sso_gpio_output_delay(priv);
	sso_gpio_update_bit(priv->mmap, SSO_CON0, SWU, 1);
}

static int sso_gpio_request(struct gpio_chip *chip, unsigned int offset)
{
	struct sso_gpio_priv *priv = gpiochip_get_data(chip);

	if (priv->alloc_bitmap & BIT(offset))
		return -EINVAL;

	priv->alloc_bitmap |= BIT(offset);
	regmap_write(priv->mmap, DUTY_CYCLE(offset), 0xFF);

	return 0;
}

static void sso_gpio_free(struct gpio_chip *chip, unsigned int offset)
{
	struct sso_gpio_priv *priv = gpiochip_get_data(chip);

	priv->alloc_bitmap &= ~BIT(offset);
}

static int sso_gpio_get_dir(struct gpio_chip *chip, unsigned int offset)
{
	return GPIOF_DIR_OUT;
}

static int sso_gpio_dir_out(struct gpio_chip *chip, unsigned int offset,
			    int value)
{
	struct sso_gpio_priv *priv = gpiochip_get_data(chip);

	sso_gpio_set_value(priv, offset, value);

	return 0;
}

static int sso_gpio_get(struct gpio_chip *chip, unsigned int offset)
{
	struct sso_gpio_priv *priv = gpiochip_get_data(chip);
	u32 reg_val;

	regmap_read(priv->mmap, SSO_CPU, &reg_val);
	return !!(reg_val & BIT(offset));
}

static void sso_gpio_set(struct gpio_chip *chip, unsigned int offset, int value)
{
	struct sso_gpio_priv *priv = gpiochip_get_data(chip);

	sso_gpio_set_value(priv, offset, value);
}

static int sso_gpio_gc_init(struct sso_gpio_priv *priv,
			    struct device *dev, const char *name)
{
	struct gpio_chip *gc = &priv->chip;

	gc->request		= sso_gpio_request;
	gc->free		= sso_gpio_free;
	gc->get_direction	= sso_gpio_get_dir;
	gc->direction_output	= sso_gpio_dir_out;
	gc->get			= sso_gpio_get;
	gc->set			= sso_gpio_set;

	gc->label		= name;
	gc->base		= priv->gpio_base;
	gc->ngpio		= priv->pins;
	gc->parent		= dev;
	gc->owner		= THIS_MODULE;
	gc->of_node		= dev->of_node;

	return devm_gpiochip_add_data(dev, gc, priv);
}

static int sso_gpio_hw_init(struct sso_gpio_priv *priv)
{
	int i;
	int err;
	u32 activate;

	/* Clear all duty cycles */
	for (i = 0; i < priv->pins; i++) {
		err = regmap_write(priv->mmap, DUTY_CYCLE(i), 0);
		if (err)
			return err;
	}

	/* 4 groups for total 32 pins */
	for (i = 1; i <= MAX_GROUP_NUM; i++) {
		activate = !!(i * PINS_PER_GROUP <= priv->pins ||
			      priv->pins > (i - 1) * PINS_PER_GROUP);
		err = sso_gpio_update_bit(priv->mmap, SSO_CON1, i - 1,
					  activate);
		if (err)
			return err;
	}

	/* NO HW directly controlled pin by default */
	if (regmap_write(priv->mmap, SSO_CON3, 0))
		return -ENOTSUPP;

	/* NO BLINK for all pins */
	if (regmap_write(priv->mmap, SSO_CON2, 0))
		return -ENOTSUPP;

	/* OUTPUT 0 by default */
	if (regmap_write(priv->mmap, SSO_CPU, 0))
		return -ENOTSUPP;

	/* update edge */
	if (sso_gpio_update_bit(priv->mmap, SSO_CON0, RZFL, priv->edge))
		return -ENOTSUPP;

	/* SW mode by default */
	if (sso_gpio_update_bit(priv->mmap, SSO_CON0, BLINK_R, 0))
		return -ENOTSUPP;
	if (sso_gpio_write_mask(priv->mmap, SSO_CON1, US, US_MASK, 0))
		return -ENOTSUPP;

	return 0;
}

static int intel_sso_gpio_probe(struct platform_device *pdev)
{
	struct sso_gpio_priv *priv;
	struct device *dev = &pdev->dev;
	const char *drv_name;
	u32 prop;
	int ret;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	/* pin base */
	if (device_property_read_u32(dev, "intel,sso-gpio-base", &prop))
		priv->gpio_base = -1;
	else
		priv->gpio_base = prop;

	/* pin number */
	if (!device_property_read_u32(dev, "ngpios", &prop)) {
		priv->pins = prop;
		if (priv->pins > MAX_PIN_NUM_PER_BANK)
			return -EINVAL;
	} else {
		priv->pins = MAX_PIN_NUM_PER_BANK;
	}

	if (!device_property_read_string(dev, "intel,sso-gpio-name", &drv_name))
		drv_name = dev->of_node->full_name;
	else
		drv_name = sso_gpio_drv_name;

	/* gate clock */
	priv->gclk = devm_clk_get(dev, "sso");
	if (IS_ERR(priv->gclk)) {
		dev_err(dev, "get sso gate clock failed!\n");
		return -EINVAL;
	}

	if (clk_prepare_enable(priv->gclk)) {
		dev_err(dev, "Failed to enable sso gate clock!\n");
		return -EINVAL;
	}

	priv->rst = devm_reset_control_get_optional(dev, NULL);
	if (IS_ERR(priv->rst)) {
		dev_err(dev, "get sso reset failed!\n");
		return PTR_ERR(priv->rst);
	}
	if (priv->rst) {
		reset_control_assert(priv->rst);
		usleep_range(10, 20);
		reset_control_deassert(priv->rst);
	}

	/* reset pin must be set output 1 before HW init */
	priv->rst_gpio = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(priv->rst_gpio)) {
		dev_err(dev, "reset gpio get error!\n");
		return PTR_ERR(priv->rst_gpio);
	}

	if (device_property_present(dev, "intel,sso-rising"))
		priv->edge = 0;
	else
		priv->edge = 1;

	/* gpio mem */
	priv->mmap = syscon_node_to_regmap(dev->of_node);
	if (IS_ERR(priv->mmap)) {
		dev_err(dev, "Failed to map iomem!\n");
		return PTR_ERR(priv->mmap);
	}

	ret = sso_gpio_hw_init(priv);
	if (ret)
		goto err;

	ret = sso_gpio_gc_init(priv, dev, drv_name);
	if (ret)
		goto err;

	priv->dev = dev;
	priv->pdev = pdev;
	platform_set_drvdata(pdev, priv);

	dev_dbg(dev, "sso gpio init success\n");
	return 0;

err:
	regmap_exit(priv->mmap);
	return ret;
}

static const struct of_device_id of_sso_gpio_match[] = {
	{ .compatible = "intel,sso-gpio" },
	{},
};

MODULE_DEVICE_TABLE(of, of_sso_gpio_match);

static struct platform_driver intel_sso_gpio_driver = {
	.probe		= intel_sso_gpio_probe,
	.driver		= {
			.name = "intel-sso-gpio",
			.of_match_table = of_match_ptr(of_sso_gpio_match),
	},
};

module_platform_driver(intel_sso_gpio_driver);

MODULE_AUTHOR("Zhu Yixin <Yixin.zhu@intel.com>");
MODULE_DESCRIPTION("Intel SSO GPIO driver");
MODULE_LICENSE("GPL v2");
