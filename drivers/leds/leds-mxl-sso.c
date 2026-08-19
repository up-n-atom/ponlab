// SPDX-License-Identifier: GPL-2.0
/*
 * MaxLinear LED SSO driver
 *
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
 * Zhu YiXin <yzhu@maxlinear.com>
 * Wu ZhiXian <wzhixian@maxlinear.com>
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

#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/gpio/consumer.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/uaccess.h>

/* reg definition */
#define DUTY_CYCLE(x)	(0x8 + ((x) * 4))
#define LED_BLINK_H8_0	0x0
#define LED_BLINK_H8_1	0x4
#define SSO_CON0	0x2B0
#define SSO_CON1	0x2B4
#define SSO_CPU0	0x2B8
#define SSO_CON2	0x2C4
#define SSO_CON3	0x2C8

/* CON0 */
#define RZFL		26
#define BLINK_R		30
#define SWU		31

/* BLINK H8 */
#define BLINK_FREQ_OFF(pin, src)	(((pin) * 6) + ((src) * 2))
#define BLINK_FREQ_MASK	0x3
#define BLINK_SRC_OFF(pin, src)		(((pin) * 6) + 4)
#define BLINK_SRC_MASK	0x3
/* represent for LED 0-23 */
#define BLINK_GEN_PIN   3
#define LED_BLINK_REG_OFF(pin)		((pin) == BLINK_GEN_PIN || ((pin) >= 29) ?  LED_BLINK_H8_1 : LED_BLINK_H8_0)

/* CON1 */
#define US		30
#define US_MASK		0x3
#define FCDSC		20
#define FCDSC_MASK	0x3
#define BLINK_FPID	23
#define BLINK_FPID_MASK 0x3
#define BLINK_GPTC	25
#define BLINK_GPTC_MASK 0x3

/* Driver MACRO */
#define SSO_LED_MAX_NUM		32
#define BRIGHTNESS_MAX		255

/**
 * If clock source is FPID, HW blink rate can be 2HZ/4HZ/8HZ/10HZ
 * If clock source is GPTC, the base frequency will be configured in
 * mxl,gptc-clkrate, it's blink rate is
 * mxl,gptc-clkrate / 200/ (4 | 5 | 10 | 20)
 * if clock source is GPTC HS, the blink rate can be 2HZ/4HZ/8HZ/10HZ or
 * mxl,gptc-clkrate / 200/ (1 | 4 | 5 | 10 | 20)
 * default clock source is GPTC HS, blink rate is 2HZ by default.
 * see below illustration
 *
 * FPID---|--2HZ---|
 *        |--4HZ---|--FPID based blink rate(mxl,sso-def-blinkrate)
 *        |--8HZ---|
 *        |--10HZ--|
 *
 * gptc-clk---/200 ---|---/20--|
 *                    |---/10--|--->gptc based blink rate(mxl,sso-def-blinkrate)
 *                    |---/5---|
 *                    |---/4---|
 *
 * LED has update rate and blink rate.
 * in FPID and GPTC mode, update rate is same as blink rate.
 * in GPTC HS mode, they can be set separately.
 */
enum {
	MAX_FPID_FREQ_RANK = 3, /* 0 - 3 */
	MAX_GPTC_FREQ_RANK = 7, /* 4 - 7 */
	MAX_GPTC_HS_FREQ_RANK = 8,
	MAX_FREQ_RANK = MAX_GPTC_HS_FREQ_RANK + 1,
};

enum {
	LED_GRP0_PIN_MAX = 24,
	LED_GRP1_PIN_MAX = 29,
	LED_GRP2_PIN_MAX = 32,
};

enum {
	LED_GRP0_0_23,
	LED_GRP1_24_28,
	LED_GRP2_29_31,
	LED_GROUP_MAX,
};

enum {
	CLK_SRC_FPID = 0,
	CLK_SRC_GPTC = 1,
	CLK_SRC_GPTC_HS = 2,
};

enum {
	SSO_BRIGHTNESS_OFF = 0,
	SSO_DEF_BRIGHTNESS = 0x80,
	SSO_MAX_BRIGHTNESS = 0xFF
};

enum {
	SSO_SW_UPDATE = 0,
	SSO_GPTC_CLK,
	SSO_FPID_CLK,
	SSO_GPTC_HS_CLK,
	SSO_UPDATE_MAX
};

enum {
	FCDSC_25MHZ,
	FCDSC_12_5MHZ, /* 12.5MHZ */
	FCDSC_6_25MHZ, /* 6.25MHZ */
	FCDSC_3_125MHZ, /* 3.125MHZ */
};

struct sso_led_priv;
/**
 * struct sso_led_desc
 *
 * @np: device tree node for this LED
 * @name: LED name
 * @default_trigger: LED trigger
 * @pin: gpio pin id/offset
 * @brighness: LED brightness/color
 * @blink_rate: LED HW blink frequency
 * @freq_idx: LED freqency idx of freq array of sso_led_priv
 * @hw_blink: HW blink
 * @hw_trig: HW control LED
 * @blinking: LED blink status
 * @default_on: LED on by default
 * @led_test: LED created by debugfs. This is for test only.
 */
struct sso_led_desc {
	struct device_node *np;
	const char *name;
	const char *default_trigger;
	u32 pin;
	unsigned int brightness;
	unsigned int blink_rate;
	u32 freq_idx;

	unsigned int hw_blink:1;
	unsigned int hw_trig:1;
	unsigned int blinking:1;
	unsigned int default_on:1;
	unsigned int led_test:1;
	unsigned int is_low_active:1;
};

/**
 * struct sso_led
 *
 * @list: LED struct list
 * @cdev: LED class device for this LED
 * @gpiod: gpio descriptor
 * @led: led settings for this LED
 * @priv: pointer to sso driver private data
 */
struct sso_led {
	struct list_head list;
	struct led_classdev cdev;
	struct gpio_desc *gpiod;
	struct sso_led_desc desc;
	struct sso_led_priv *priv;
};

/**
 * struct sso_cfg
 *
 * @gptc_clkrate: sso fast clock rate
 * @brightness: default brightness for all leds
 * @blink_rate_idx: default brink rate idx of freq
 * @update_src: sso blink source
 */
struct sso_cfg {
	u32 gptc_clkrate;
	u32 brightness;
	int blink_rate_idx;
	u32 update_src;
};

/**
 * struct sso_led_priv
 *
 * @membase: SSO mapped memory resource
 * @dev: sso basic device
 * @pdev: sso platform device
 * @freq: sso frequency array
 * @cfg: sso DT configuration
 * @led_list: link list of leds of SSO
 * @debugfs: debugfs proc
 */
struct sso_led_priv {
	struct regmap *mmap;
	struct device *dev;
	struct platform_device *pdev;
	u32 freq[MAX_FREQ_RANK];
	struct sso_cfg cfg;
	struct list_head led_list;
	struct dentry *debugfs;
};

static void sso_led_set_blink(struct sso_led_priv *priv, u32 pin, bool set);
static void led_output(struct sso_led *led, int val);

static struct sso_led
*cdev_to_sso_led_data(struct led_classdev *led_cdev)
{
	return container_of(led_cdev, struct sso_led, cdev);
}

static ssize_t
hw_blink_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sso_led *led = cdev_to_sso_led_data(led_cdev);

	return sprintf(buf, "%u\n", led->desc.blinking);
}

static ssize_t
hw_blink_store(struct device *dev, struct device_attribute *attr,
	       const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sso_led *led = cdev_to_sso_led_data(led_cdev);
	ssize_t ret;
	unsigned long state, org_state;

	ret = kstrtoul(buf, 10, &state);
	if (ret)
		return ret;

	state = !!state;
	org_state = (int)led->desc.blinking;
	led->desc.blinking = (u32)state;

	if (org_state == state)
		return size;

	if (led->desc.brightness) {
		led_output(led, 1);
		sso_led_set_blink(led->priv, led->desc.pin, state);
	}

	return size;
}
static DEVICE_ATTR_RW(hw_blink);

static void sso_led_shutdown(struct sso_led *led);

static int sso_led_writel(struct regmap *map, u32 reg, u32 val)
{
	int ret;

	ret = regmap_write(map, reg, val);
	return ret;
}

static int sso_led_update_bit(struct regmap *map, u32 reg, u32 off, u32 val)
{
	val = !!val;

	return regmap_update_bits(map, reg, BIT(off), (val << off));
}

static int sso_led_write_mask(struct regmap *map, u32 reg, u32 off,
			      u32 mask, u32 val)
{
	u32 reg_val;

	if (regmap_read(map, reg, &reg_val))
		return -EINVAL;

	reg_val = (reg_val & ~(mask << off)) | ((val & mask) << off);

	return sso_led_writel(map, reg, reg_val);
}

/**
 * set blink rate for LED 24 - 31
 * set blink rate for LED 0-32 in GPTC high speed mode
 */
static void sso_led_set_blink_rate(struct sso_led_priv *priv, u32 led_id,
				   u32 blink_rate_idx)
{
	u32 reg;
	u32 freq_idx;
	u32 blink_src;

	reg = LED_BLINK_REG_OFF(led_id);

	if (blink_rate_idx <= MAX_FPID_FREQ_RANK) {
		blink_src = CLK_SRC_FPID;
		freq_idx = blink_rate_idx;
	} else if (blink_rate_idx <= MAX_GPTC_FREQ_RANK) {
		blink_src = CLK_SRC_GPTC;
		freq_idx = blink_rate_idx - MAX_FPID_FREQ_RANK - 1;
	} else {
		blink_src = CLK_SRC_GPTC_HS;
		freq_idx = 0;
	}

	/* freq setting */
	if (blink_src != CLK_SRC_GPTC_HS) {
		sso_led_write_mask(priv->mmap, reg,
				   BLINK_FREQ_OFF(led_id, blink_src),
				   BLINK_FREQ_MASK, freq_idx);
	}

	/* select clock source */
	sso_led_write_mask(priv->mmap, reg,
			   BLINK_SRC_OFF(led_id, blink_src),
			   BLINK_SRC_MASK, blink_src);
}

static void sso_led_set_def_blinkrate(struct sso_led_priv *priv)
{
	struct sso_cfg *cfg = &priv->cfg;
	u32 freq_idx = 0;

	switch (cfg->update_src) {
	case SSO_FPID_CLK:
		sso_led_write_mask(priv->mmap, SSO_CON1, BLINK_FPID,
				   BLINK_FPID_MASK, cfg->blink_rate_idx);
		break;

	case SSO_GPTC_CLK:
		freq_idx = cfg->blink_rate_idx - MAX_FPID_FREQ_RANK - 1;
		sso_led_write_mask(priv->mmap, SSO_CON1, BLINK_GPTC,
				   BLINK_GPTC_MASK, freq_idx);
		break;

	case SSO_GPTC_HS_CLK:
		sso_led_set_blink_rate(priv, BLINK_GEN_PIN, cfg->blink_rate_idx);
		break;

	default:
		dev_err(priv->dev, "SSO update src is wrong!\n");
		return;
	}
}

static int sso_led_hw_init(struct sso_led_priv *priv)
{
	struct sso_cfg *cfg = &priv->cfg;
	u32 fcdsc; /* fixed divider shift clock, set to less than 25Mhz in leagcy mode */
	u32 fast_update;
	u32 update_src;

	if (cfg->update_src != SSO_GPTC_HS_CLK) {
		fcdsc = FCDSC_12_5MHZ;
		fast_update = 0;
		update_src = cfg->update_src;

	} else { /* GPTC HS mode */
		fcdsc = FCDSC_25MHZ;
		fast_update = 1;
		update_src = SSO_GPTC_CLK;
	}

	sso_led_write_mask(priv->mmap, SSO_CON1, US, US_MASK, update_src);
	sso_led_update_bit(priv->mmap, SSO_CON0, BLINK_R, fast_update);
	sso_led_write_mask(priv->mmap, SSO_CON1, FCDSC, FCDSC_MASK, fcdsc);

	sso_led_set_def_blinkrate(priv);

	return 0;
}

static u32 sso_rectify_brightness(u32 brightness)
{
	if (brightness > BRIGHTNESS_MAX)
		return BRIGHTNESS_MAX;
	else
		return brightness;
}

static int sso_rectify_blink_rate(struct sso_led_priv *priv, u32 rate)
{
	int i;
	struct sso_cfg *cfg = &priv->cfg;

	for (i = 0; i < MAX_FREQ_RANK; i++) {
		if (rate <= priv->freq[i])
			break;
	}

	if (cfg->update_src == SSO_FPID_CLK && i > MAX_FPID_FREQ_RANK)
		return MAX_FPID_FREQ_RANK;

	if (cfg->update_src == SSO_GPTC_CLK && i > MAX_GPTC_FREQ_RANK)
		return MAX_GPTC_FREQ_RANK;

	if (i >= MAX_FREQ_RANK)
		return MAX_GPTC_HS_FREQ_RANK;

	return i;
}

static unsigned int sso_led_pin_to_group(u32 pin)
{
	if (pin < LED_GRP0_PIN_MAX)
		return LED_GRP0_0_23;
	else if (pin < LED_GRP1_PIN_MAX)
		return LED_GRP1_24_28;
	else
		return LED_GRP2_29_31;
}

static void
sso_led_set_blink(struct sso_led_priv *priv, u32 pin, bool set)
{
	sso_led_update_bit(priv->mmap, SSO_CON2, pin, set);
}

static void
sso_led_set_hw_trigger(struct sso_led_priv *priv, u32 pin, bool set)
{
	sso_led_update_bit(priv->mmap, SSO_CON3, pin, set);
}

static void sso_led_freq_set(struct sso_led_priv *priv,
			     struct sso_led_desc *desc, int freq_idx)
{
	unsigned int group;
	u32 pin;

	pin = desc->pin;
	group = sso_led_pin_to_group(pin);

	/**
	 * LED group 0-23 use default frequency
	 * it only need be configured in test mode
	 */
	if (group == LED_GRP0_0_23 && !desc->led_test)
		return;

	if (group == LED_GRP0_0_23)
		sso_led_set_def_blinkrate(priv);
	else
		sso_led_set_blink_rate(priv, pin, freq_idx);
}

/**
 * This function is used by LEDs that without GPIO function.
 * These LEDS could be the test LEDS that created by debugfs,
 * or LEDs uing LED DD pins.
 */
static void led_pad_output(struct sso_led *led, int val)
{
	struct sso_led_priv *priv = led->priv;
	struct sso_led_desc *desc = &led->desc;

	/* set on/off */
	sso_led_update_bit(priv->mmap, SSO_CPU0, desc->pin, val);

	/* trigger output */
	if (priv->cfg.update_src == SSO_SW_UPDATE)
		sso_led_update_bit(priv->mmap, SSO_CON0, SWU, 1);
}

static void led_output(struct sso_led *led, int val)
{
	struct sso_cfg *cfg = &led->priv->cfg;
	struct sso_led_desc *desc = &led->desc;

	if (led->desc.hw_trig) /* HW controlled LED */
		return;

	if (desc->brightness == LED_OFF) {
		val = desc->is_low_active;
	} else {
		// brightness: 1~255
		if (cfg->update_src == SSO_GPTC_HS_CLK) {
			// In order to make dimming function, we need to enable LED in the LED controller. Brightess is controlled by DUTY_CYCLE.
			val = 1;
		} else {
			// Birghtness is always 255 in both FPID and SW mode.
			if (desc->hw_blink && desc->blinking) {
				// To enable the hw blink function, we need to activate the LED in the SSOLED controller.
				val = 1;
			} else {
				val = !desc->is_low_active;
			}
		}
	}

	led_pad_output(led, val);
}

static void sso_led_brightness_set(struct led_classdev *led_cdev,
				   enum led_brightness brightness)
{
	struct sso_led *led;
	struct sso_led_priv *priv;
	struct sso_led_desc *desc;
	int val;

	led = cdev_to_sso_led_data(led_cdev);
	priv = led->priv;
	desc = &led->desc;

	desc->brightness = brightness;
	/**
	 * Only GPTC Fast mode allow brightness setting
	 * Other mode use default 0xFF as brightness
	 * To make it compatible to Linux sys interface, user still
	 * can use brightness to control LED on/off
	 */
	if (priv->cfg.update_src == SSO_GPTC_HS_CLK) {
		sso_led_writel(priv->mmap, DUTY_CYCLE(desc->pin),
				desc->is_low_active ? (BRIGHTNESS_MAX - brightness) : brightness);
	}

	if (brightness == LED_OFF)
		val = 0;
	else
		val = 1;

	/* HW blink off/on */
	if (desc->hw_blink && desc->blinking)
		sso_led_set_blink(priv, desc->pin, val);

	led_output(led, val);
}

static enum led_brightness sso_led_brightness_get(struct led_classdev *led_cdev)
{
	struct sso_led *led;

	led = cdev_to_sso_led_data(led_cdev);
	return (enum led_brightness)led->desc.brightness;
}

static void sso_led_hw_cfg(struct sso_led_priv *priv, struct sso_led *led)
{
	struct sso_led_desc *desc = &led->desc;

	/* set freq */
	if (desc->hw_blink)
		sso_led_freq_set(priv, desc, desc->freq_idx);

	if (desc->hw_trig)
		sso_led_set_hw_trigger(priv, desc->pin, 1);

	/* set brightness */
	if (priv->cfg.update_src == SSO_GPTC_HS_CLK) {
		sso_led_writel(priv->mmap, DUTY_CYCLE(desc->pin),
				desc->is_low_active ? (BRIGHTNESS_MAX - desc->brightness) : desc->brightness);
	} else {  /* only support 255 */
		sso_led_writel(priv->mmap, DUTY_CYCLE(desc->pin),
			       BRIGHTNESS_MAX);
	}

	/* enable output */
	if (desc->default_on)
		led_pad_output(led, 1);
}

static int led_add_hw_blink(struct sso_led *led)
{
	struct device *dev = led->cdev.dev;
	int ret;

	if (!led->desc.hw_blink)
		return 0;

	ret = device_create_file(dev, &dev_attr_hw_blink);
	if (ret) {
		dev_err(dev, "Error creating hw_blink\n");
		return ret;
	}

	return 0;
}

static void led_remove_hw_blink(struct sso_led *led)
{
	struct device *dev = led->cdev.dev;

	if (!led->desc.hw_blink)
		return;

	device_remove_file(dev, &dev_attr_hw_blink);
}

static int sso_create_led(struct sso_led_priv *priv, struct sso_led *led)
{
	struct sso_led_desc *desc = &led->desc;

	led->cdev.name = desc->name;
	led->cdev.default_trigger = desc->default_trigger;
	led->cdev.brightness_set = sso_led_brightness_set;
	led->cdev.brightness_get = sso_led_brightness_get;
	led->cdev.brightness = desc->brightness;
	led->cdev.max_brightness = BRIGHTNESS_MAX;

	sso_led_hw_cfg(priv, led);

	if (devm_led_classdev_register(priv->dev, &led->cdev)) {
		dev_err(priv->dev, "register led class %s failed!\n",
			desc->name);
		return -EINVAL;
	}

	led_add_hw_blink(led);
	list_add(&led->list, &priv->led_list);

	return 0;
}

static int sso_led_dt_parse(struct sso_led_priv *priv)
{
	struct sso_led_desc *desc;
	struct sso_led *led;
	struct fwnode_handle *child;
	struct device *dev = priv->dev;
	struct list_head *p;
	int count, index;
	u32 prop;

	count = device_get_child_node_count(dev);
	if (!count)
		return 0;

	device_for_each_child_node(dev, child) {
		led = devm_kzalloc(dev, sizeof(*led), GFP_KERNEL);
		if (!led)
			return -ENOMEM;

		INIT_LIST_HEAD(&led->list);
		led->priv = priv;
		desc = &led->desc;

		desc->np = to_of_node(child);
		if (fwnode_property_read_string(child, "label", &desc->name)) {
			dev_err(dev, "LED no label name!\n");
			goto __dt_err;
		}

		if (fwnode_property_read_u32(child, "mxl,led-pin", &prop)) {
			dev_err(dev, "Failed to find led pin id!\n");
			goto __dt_err;
		} else {
			desc->pin = prop;
			if (desc->pin >= SSO_LED_MAX_NUM) {
				dev_err(dev, "invalid LED pin: %u\n",
					desc->pin);
				goto __dt_err;
			}
		}

		index = 0;
		led->gpiod = devm_fwnode_get_index_gpiod_from_child(dev, "led",
					index, child, GPIOD_ASIS, desc->name);
		if (IS_ERR(led->gpiod)) {
			dev_dbg(dev, "NO GPIO for LED: %s!\n", desc->name);
			led->gpiod = NULL;
		}

		fwnode_property_read_string(child, "linux,default-trigger",
					    &desc->default_trigger);

		if (fwnode_property_present(child, "mxl,sso-hw-blink"))
			desc->hw_blink = 1;

		if (fwnode_property_present(child, "mxl,sso-default-on"))
			desc->default_on = 1;

		if (fwnode_property_present(child, "mxl,sso-hw-trigger"))
			desc->hw_trig = 1;

		if (fwnode_property_present(child, "mxl,sso-low_active"))
			desc->is_low_active = 1;

		if (desc->hw_trig) {
			desc->default_trigger = NULL;
			desc->hw_blink = 0;
			desc->default_on = 0;
		}

		/**
		 * Note: only LED24-LED31 have their own blink rate
		 */
		if (desc->pin >= LED_GRP0_PIN_MAX &&
		    !fwnode_property_read_u32(child,
					      "mxl,sso-blink-rate", &prop)) {
			desc->freq_idx = sso_rectify_blink_rate(priv, prop);
			desc->blink_rate = priv->freq[desc->freq_idx];
		} else {
			/* use default freq rate */
			desc->freq_idx = priv->cfg.blink_rate_idx;
			desc->blink_rate = priv->freq[desc->freq_idx];
		}

		if (fwnode_property_read_u32(child,
					     "mxl,sso-brightness", &prop))
			desc->brightness = priv->cfg.brightness;
		else
			desc->brightness = sso_rectify_brightness(prop);

		if (sso_create_led(priv, led))
			goto __dt_err;
	}

	return 0;

__dt_err:
	fwnode_handle_put(child);
	/* unregister leds */
	list_for_each(p, &priv->led_list) {
		led = list_entry(p, struct sso_led, list);
		sso_led_shutdown(led);
	}
	return -EINVAL;
}

static int sso_dt_parse(struct sso_led_priv *priv)
{
	struct sso_cfg *cfg;
	struct device *dev = priv->dev;
	u32 prop;

	cfg = &priv->cfg;

	if (device_property_read_u32(dev, "mxl,sso-def-brightness", &prop)) {
		cfg->brightness = SSO_DEF_BRIGHTNESS;
	} else {
		cfg->brightness = prop;
		if (cfg->brightness > SSO_MAX_BRIGHTNESS)
			cfg->brightness = SSO_MAX_BRIGHTNESS;
	}

	if (device_property_read_u32(dev, "mxl,sso-def-updatesrc", &prop))
		cfg->update_src = SSO_FPID_CLK;
	else
		cfg->update_src = prop;

	if (cfg->update_src >= SSO_UPDATE_MAX) {
		dev_err(dev, "led update src error!\n");
		return -EINVAL;
	}

	if (device_property_read_u32(dev, "mxl,sso-def-blinkrate", &prop))
		prop = 2;

	cfg->blink_rate_idx = sso_rectify_blink_rate(priv, prop);

	if (sso_led_hw_init(priv)) {
		dev_err(dev, "led HW init fail!\n");
		return -EINVAL;
	}

	if (sso_led_dt_parse(priv))
		return -EINVAL;

	dev_dbg(priv->dev, "sso LED init success!\n");
	return 0;
}

/**
 * FPID blink rate: 2HZ, 4HZ, 8HZ, 10HZ
 * GPTC blink rate: gptc_clkrate / 200 / (4/5/10/20)
 * GPTC High speed blink rate is 2HZ fixed, but
 * update rate will be gptc_clkrate / 200
 */
static void sso_init_freq(struct sso_led_priv *priv)
{
	int i;
	struct sso_cfg *cfg = &priv->cfg;
	u32 blink_rate = cfg->gptc_clkrate;
	u32 fpid_freq[] = {2, 4, 8, 10};
	u32 gptc_freq_div_tbl[] = {20, 10, 5, 4};
	u32 gptc_freq;

	do_div(blink_rate, 200);
	for (i = 0; i < MAX_FREQ_RANK; i++) {
		if (i <= MAX_FPID_FREQ_RANK) {
			priv->freq[i] = fpid_freq[i];
		} else if (i <= MAX_GPTC_FREQ_RANK) {
			gptc_freq = blink_rate;
			do_div(gptc_freq,
			       gptc_freq_div_tbl[i - MAX_FPID_FREQ_RANK - 1]);
			priv->freq[i] = gptc_freq;
		}
	}

	/* debug dump */
	for (i = 0; i < MAX_FREQ_RANK; i++)
		dev_dbg(priv->dev, "SSO freq[%d]: %u\n", i, priv->freq[i]);
}

static void sso_led_shutdown(struct sso_led *led)
{
	struct sso_led_priv *priv;

	priv = led->priv;

	/* remove my sys file */
	led_remove_hw_blink(led);

	/* unregister led */
	devm_led_classdev_unregister(priv->dev, &led->cdev);

	/* clear HW control bit */
	if (led->desc.hw_trig)
		sso_led_set_hw_trigger(priv, led->desc.pin, 0);

	if (led->gpiod)
		devm_gpiod_put(priv->dev, led->gpiod);

	led->priv = NULL;
}

#if defined(CONFIG_DEBUG_FS)

static ssize_t
sso_led_create_write(struct file *s, const char __user *buffer,
		     size_t count, loff_t *pos)
{
	char buf[128] = {0};
	char *start, *led_name;
	u32 pin, blink_rate;
	struct sso_led_priv *priv;
	struct sso_led *led;
	struct sso_led_desc *desc;
	struct device *dev;
	int i;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	priv = file_inode(s)->i_private;
	dev = priv->dev;
	if (copy_from_user(buf, buffer, sizeof(buf) - 1))
		return -EFAULT;

	if (strcmp(buf, "help") == 0)
		goto __create_help;

	for (i = 0; buf[i] != ' ' && buf[i] != 0; i++)
		;
	if (buf[i] == 0)
		goto __create_help;
	buf[i++] = 0;
	start = &buf[i];
	for (; buf[i] >= '0' && buf[i] <= '9'; i++)
		;
	buf[i] = 0;

	if (kstrtou32(buf, 0, &pin)) {
		dev_err(dev, "Failed to parse led pin!\n");
		goto __create_help;
	}

	if (kstrtou32(start, 0, &blink_rate)) {
		dev_err(dev, "Failed to parse led blink rate!\n");
		goto __create_help;
	}

	if (pin >= SSO_LED_MAX_NUM)
		return -EINVAL;

	led = devm_kzalloc(dev, sizeof(*led), GFP_KERNEL);
	if (!led)
		return -ENOMEM;

	INIT_LIST_HEAD(&led->list);
	led->priv = priv;
	desc = &led->desc;

	led_name = devm_kmalloc(dev, 32, GFP_KERNEL);
	if (!led_name)
		return -ENOMEM;

	sprintf(led_name, "sso-led-%u", pin);
	desc->name = (const char *)led_name;

	desc->freq_idx = sso_rectify_blink_rate(priv, blink_rate);
	desc->blink_rate = priv->freq[desc->freq_idx];
	desc->brightness = 0;
	desc->pin = pin;
	desc->hw_trig = 0;
	desc->hw_blink = 1;
	desc->led_test = 1;
	desc->is_low_active = 0;

	if (sso_create_led(priv, led))
		return -EINVAL;

	return count;

__create_help:
	dev_info(priv->dev, "echo <pin> <blink rate> > create\n");
	return count;
}

static const struct file_operations sso_led_create_fops = {
	.write = sso_led_create_write,
	.llseek = noop_llseek,
};

static ssize_t
sso_led_delete_write(struct file *s, const char __user *buffer,
		     size_t count, loff_t *pos)
{
	char buf[32] = {0};
	u32 pin;
	struct sso_led_priv *priv;
	struct list_head *p, *n;
	struct sso_led *led;
	int i;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	priv = file_inode(s)->i_private;
	if (copy_from_user(buf, buffer, sizeof(buf) - 1))
		return -EFAULT;

	if (strcmp(buf, "help") == 0)
		goto __delete_err;

	for (i = 0; buf[i] >= '0' && buf[i] <= '9'; i++)
		;
	buf[i] = 0;

	if (kstrtou32(buf, 0, &pin))
		goto __delete_err;

	list_for_each_safe(p, n, &priv->led_list) {
		led = list_entry(p, struct sso_led, list);
		if (led->desc.pin == pin && led->desc.led_test) {
			list_del(p);
			sso_led_shutdown(led);
			devm_kfree(priv->dev, (void *)led->desc.name);
			devm_kfree(priv->dev, led);
			break;
		}
	}

	return count;

__delete_err:
	dev_info(priv->dev, "echo <pin> > delete\n");
	return count;
}

static const struct file_operations sso_led_delete_fops = {
	.write = sso_led_delete_write,
	.llseek = noop_llseek,
};

static void *sso_led_show_seq_start(struct seq_file *s, loff_t *pos)
{
	struct sso_led_priv *priv = s->private;

	seq_printf(s, "-------------Default-----------------------\n");
	seq_printf(s, "CLK SRC: %s\n", priv->cfg.update_src == SSO_SW_UPDATE ? "SW" : priv->cfg.update_src == SSO_FPID_CLK ? "FPID" : "GPTC");
	seq_printf(s, "GPTC CLK rate: %u\n", priv->cfg.gptc_clkrate);
	seq_printf(s, "Blink rate:%u\n", priv->freq[priv->cfg.blink_rate_idx]);
	seq_printf(s, "Brightness:%u\n", priv->cfg.brightness);

	return seq_list_start(&priv->led_list, *pos);
}

static void *sso_led_show_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct sso_led_priv *priv = s->private;

	return seq_list_next(v, &priv->led_list, pos);
}

static void sso_led_show_seq_stop(struct seq_file *s, void *v)
{
}

static int sso_led_show_seq_show(struct seq_file *s, void *v)
{
	struct list_head *p = v;
	struct sso_led *led;
	struct sso_led_desc *desc;

	led = list_entry(p, struct sso_led, list);
	desc = &led->desc;

	seq_printf(s, "-------------%s-----------------------\n", desc->name);
	seq_printf(s, "pin: %u\n", desc->pin);
	seq_printf(s, "brightness : %u\n", desc->brightness);
	seq_printf(s, "blink rate: %u\n", desc->blink_rate);
	if (desc->default_trigger)
		seq_printf(s, "default trigger: %s\n", desc->default_trigger);
	seq_printf(s, "%s driven LED\n", (!desc->hw_trig) ? "SW" : "HW");
	seq_printf(s, "%s blinking\n", (!desc->hw_blink) ? "SW" : "HW");
	seq_printf(s, "blinking status: %s\n", (desc->blinking) ? "Yes" : "No");
	seq_printf(s, "low active: %s\n", (desc->is_low_active) ? "Yes" : "No");

	return 0;
}

static const struct seq_operations sso_led_show_seq_ops = {
	.start = sso_led_show_seq_start,
	.next = sso_led_show_seq_next,
	.stop = sso_led_show_seq_stop,
	.show = sso_led_show_seq_show,
};

static int sso_led_show_seq_open(struct inode *inode, struct file *file)
{
	struct seq_file *s;
	int ret;

	ret = seq_open(file, &sso_led_show_seq_ops);
	if (!ret) {
		s = file->private_data;
		s->private = inode->i_private;
	}

	return ret;
}

static const struct file_operations sso_led_show_fops = {
	.open = sso_led_show_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

static int sso_led_proc_init(struct sso_led_priv *priv)
{
	char sso_led_dir[64] = {0};
	struct dentry *file;

	strscpy(sso_led_dir, priv->dev->of_node->name, sizeof(sso_led_dir));
	priv->debugfs = debugfs_create_dir(sso_led_dir, NULL);

	if (!priv->debugfs)
		return -ENOMEM;

	file = debugfs_create_file("create", 0200, priv->debugfs,
				   priv, &sso_led_create_fops);
	if (!file)
		goto __proc_err;

	file = debugfs_create_file("delete", 0200, priv->debugfs,
				   priv, &sso_led_delete_fops);
	if (!file)
		goto __proc_err;

	file = debugfs_create_file("show", 0400, priv->debugfs,
				   priv, &sso_led_show_fops);
	if (!file)
		goto __proc_err;

	return 0;

__proc_err:
	debugfs_remove_recursive(priv->debugfs);
	return -ENOMEM;
}

static void sso_led_proc_exit(struct sso_led_priv *priv)
{
	debugfs_remove_recursive(priv->debugfs);
	priv->debugfs = NULL;
}
#else
static int sso_led_proc_init(struct sso_led_priv *priv)
{
	return 0;
}

static void sso_led_proc_exit(struct sso_led_priv *priv)
{
}

#endif /* CONFIG_DEBUG_FS */

static int mxl_sso_led_probe(struct platform_device *pdev)
{
	struct sso_led_priv *priv;
	struct device *dev = &pdev->dev;
	struct sso_cfg *cfg;
	u32 prop;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	INIT_LIST_HEAD(&priv->led_list);

	/* led mem */
	priv->mmap = syscon_node_to_regmap(dev->parent->of_node);
	if (IS_ERR_OR_NULL(priv->mmap)) {
		dev_err(dev, "Failed to get sso led iomem\n");
		return PTR_ERR(priv->mmap);
	}

	cfg = &priv->cfg;
	if (device_property_read_u32(dev, "mxl,gptc-clkrate", &prop)) {
		dev_err(dev, "Failed to get gptc clock rate\n");
		goto __hw_err;
	} else {
		cfg->gptc_clkrate = prop;
	}

	priv->dev = dev;
	priv->pdev = pdev;
	platform_set_drvdata(pdev, priv);

	sso_init_freq(priv);
	if (sso_dt_parse(priv))
		goto __hw_err;

	if (sso_led_proc_init(priv))
		goto __hw_err;

	dev_dbg(dev, "[%s] SSO LED init success.\n", __func__);
	return 0;

__hw_err:
	return -EINVAL;
}

static int mxl_sso_led_remove(struct platform_device *pdev)
{
	struct sso_led_priv *priv;
	struct sso_led *led;
	struct list_head *pos, *n;

	priv = platform_get_drvdata(pdev);

	list_for_each_safe(pos, n, &priv->led_list) {
		list_del(pos);
		led = list_entry(pos, struct sso_led, list);
		sso_led_shutdown(led);
	}

	sso_led_proc_exit(priv);

	return 0;
}

static const struct of_device_id of_sso_led_match[] = {
	{ .compatible = "mxl,sso-led" },
	{},
};

MODULE_DEVICE_TABLE(of, of_sso_led_match);

static struct platform_driver mxl_sso_led_driver = {
	.probe		= mxl_sso_led_probe,
	.remove		= mxl_sso_led_remove,
	.driver		= {
			.name = "mxl-sso-led",
			.of_match_table = of_match_ptr(of_sso_led_match),
	},
};

module_platform_driver(mxl_sso_led_driver);

MODULE_AUTHOR("Zhu Yixin <yzhu@maxlinear.com>");
MODULE_DESCRIPTION("MaxLinear SSO LED driver");
MODULE_LICENSE("GPL v2");
