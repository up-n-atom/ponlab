/*****************************************************************************
 *
 * Copyright (c) 2022 - 2025 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file drv_pon_mbox_sfp.c
 *
 */

#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h> /* kmalloc, kfree */
#include <linux/version.h>

#include "drv_pon_mbox.h"

/** SFPs contain two logical EEPROM devices. */
#define MAX_SFP_EEPROM 2

struct eeprom {
	struct i2c_client *client;
	struct bin_attribute bin;
};

struct pon_sfp_priv {
	struct eeprom eep[MAX_SFP_EEPROM];
	/* kernel objects for hotplug events */
	struct kset *kset;
	struct kobject kobj;
	/* optional gpio for hotplug detection */
	struct gpio_desc *gpio_mod_def_0;
	/* irq number for selected gpio */
	int gpio_irq_mod_def_0;
};

#define to_sfp_obj(x) container_of(x, struct pon_sfp_priv, kobj)

static struct pon_sfp_priv *g_sfp_priv;

static int i2c_read(struct i2c_client *client, u8 dev_addr, void *buf,
		    size_t len)
{
	struct i2c_msg msgs[2] = { 0 };
	u8 bus_addr = client->addr;
	size_t this_len;
	int ret;

	msgs[0].addr = bus_addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &dev_addr;
	msgs[1].addr = bus_addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = len;
	msgs[1].buf = buf;

	while (len) {
		this_len = len;
		if (this_len > 16)
			this_len = 16;

		msgs[1].len = this_len;

		ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
		if (ret < 0)
			return ret;

		if (ret != ARRAY_SIZE(msgs))
			break;

		msgs[1].buf += this_len;
		dev_addr += this_len;
		len -= this_len;
	}

	return msgs[1].buf - (u8 *)buf;
}

static int i2c_write(struct i2c_client *client, u8 dev_addr, void *buf,
		     size_t len)
{
	struct i2c_msg msgs[1] = { 0 };
	u8 bus_addr = client->addr;
	int ret;

	msgs[0].addr = bus_addr;
	msgs[0].flags = 0;
	msgs[0].len = 1 + len;
	msgs[0].buf = kmalloc(1 + len, GFP_KERNEL);
	if (!msgs[0].buf)
		return -ENOMEM;

	msgs[0].buf[0] = dev_addr;
	memcpy(&msgs[0].buf[1], buf, len);

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));

	kfree(msgs[0].buf);

	if (ret < 0)
		return ret;

	return ret == ARRAY_SIZE(msgs) ? len : 0;
}

int pon_sfp_read(bool a2, u8 addr, void *buf, size_t len)
{
	struct pon_sfp_priv *sfp_priv = g_sfp_priv;
	struct i2c_client *client;

	if (!sfp_priv)
		return -ENODEV;

	client = sfp_priv->eep[a2].client;

	return i2c_read(client, addr, buf, len);
}
EXPORT_SYMBOL(pon_sfp_read);

int pon_sfp_write(bool a2, u8 addr, void *buf, size_t len)
{
	struct pon_sfp_priv *sfp_priv = g_sfp_priv;
	struct i2c_client *client;

	if (!sfp_priv)
		return -ENODEV;

	client = sfp_priv->eep[a2].client;

	return i2c_write(client, addr, buf, len);
}
EXPORT_SYMBOL(pon_sfp_write);

static ssize_t eeprom_bin_read(struct file *filp, struct kobject *kobj,
			       struct bin_attribute *attr, char *buf,
			       loff_t off, size_t count)
{
	struct eeprom *eeprom;
	int ret;

	eeprom = container_of(attr, struct eeprom, bin);
	if (count + off > attr->size)
		count = attr->size - off;

	ret = i2c_read(eeprom->client, off, buf, count);
	if (ret < 0)
		return ret;

	return count;
}

static ssize_t eeprom_bin_write(struct file *filp, struct kobject *kobj,
				struct bin_attribute *attr, char *buf,
				loff_t off, size_t count)
{
	struct eeprom *eeprom;
	int ret;

	eeprom = container_of(attr, struct eeprom, bin);

	if (count + off > attr->size)
		count = attr->size - off;

	ret = i2c_write(eeprom->client, off, buf, count);
	if (ret < 0)
		return ret;

	return count;
}

static int create_eep_file(struct pon_mbox *pon, struct eeprom *eeprom)
{
	sysfs_bin_attr_init(&eeprom->bin);
	eeprom->bin.attr.name = kasprintf(GFP_KERNEL, "eeprom%02x",
					  eeprom->client->addr);
	/* S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH */
	eeprom->bin.attr.mode = 0644;
	eeprom->bin.read = eeprom_bin_read;
	eeprom->bin.write = eeprom_bin_write;
	eeprom->bin.size = 256;

	return device_create_bin_file(pon->dev, &eeprom->bin);
}

static void remove_eep_file(struct pon_mbox *pon, struct eeprom *eeprom)
{
	device_remove_bin_file(pon->dev, &eeprom->bin);
	kfree(eeprom->bin.attr.name);
}

static int send_sfp_hotplug_event(struct pon_mbox *pon, int state)
{
	struct pon_sfp_priv *sfp_priv = pon->sfp;

	kobject_uevent(&sfp_priv->kobj, state ? KOBJ_ONLINE : KOBJ_OFFLINE);

	return 0;
}

static irqreturn_t moddef0_irq_handler(int irq, void *data)
{
	struct pon_mbox *pon = data;
	struct pon_sfp_priv *sfp_priv = pon->sfp;
	int v;

	v = gpiod_get_value_cansleep(sfp_priv->gpio_mod_def_0);

	dev_dbg(pon->dev, "irq %d: mod-def0 changed to %d\n", irq, v);
	send_sfp_hotplug_event(pon, v);

	return IRQ_HANDLED;
}

static int moddef0_register(struct pon_mbox *pon, struct device_node *np)
{
	struct pon_sfp_priv *sfp_priv = pon->sfp;
	struct gpio_desc *gpiod;
	int gpio_irq;
	int ret;

	gpiod = devm_fwnode_gpiod_get(pon->dev, of_fwnode_handle(np),
				      "mod-def0", GPIOD_IN, "pon-sfp-mod-def0");
	if (IS_ERR_OR_NULL(gpiod)) {
		ret = PTR_ERR_OR_ZERO(gpiod);
		if (ret == -ENOENT)
			return 0;
		dev_err(pon->dev, "Request failed for mod-def0 GPIO: %d\n",
			ret);
		return ret;
	}

	gpio_irq = gpiod_to_irq(gpiod);
	if (gpio_irq < 0) {
		dev_err(pon->dev, "no irq possible for mod-def0 (%d)\n",
			gpio_irq);
		devm_gpiod_put(pon->dev, gpiod);
		return gpio_irq;
	}

	ret = devm_request_threaded_irq(
		pon->dev, gpio_irq, NULL, moddef0_irq_handler,
		IRQF_ONESHOT | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
		"pon-sfp-mod-def0", pon);
	if (ret) {
		dev_warn(pon->dev,
			 "requesting irq for mod-def0 failed with %d\n", ret);
		devm_gpiod_put(pon->dev, gpiod);
		return ret;
	}

	sfp_priv->gpio_mod_def_0 = gpiod;
	sfp_priv->gpio_irq_mod_def_0 = gpio_irq;
	return 0;
}

static void sfp_release(struct kobject *kobj)
{
	pr_debug("sfp-release %p\n", kobj);
}

static struct kobj_type sfp_ktype = {
	.release = &sfp_release,
};

int pon_sfp_probe(struct pon_mbox *pon, struct device_node *np)
{
	struct device_node *sfp_node, *tmp_node, *moddef_node;
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	struct pon_sfp_priv *sfp_priv;
	int i;
	int ret;

	sfp_node = of_parse_phandle(np, "sfp", 0);
	if (!sfp_node) {
		dev_warn(pon->dev, "No sfp node found\n");
		return -ENODEV;
	}
	moddef_node = sfp_node;

	if (of_device_is_compatible(sfp_node, "sff,sff") ||
	    of_device_is_compatible(sfp_node, "sff,sfp")) {
		tmp_node = of_parse_phandle(sfp_node, "i2c-bus", 0);
		of_node_put(sfp_node);
		if (!tmp_node) {
			dev_warn(pon->dev,
				 "sfp node without 'i2c-bus' found\n");
			return -ENODEV;
		}
		/* TODO: For parsing more info from the "sff,sfp" node,
		 *       the relevant code should be added here.
		 */
		sfp_node = tmp_node;
	}

	adapter = of_find_i2c_adapter_by_node(sfp_node);
	of_node_put(sfp_node);
	if (!adapter)
		return -ENODEV;

	sfp_priv = devm_kzalloc(pon->dev, sizeof(*sfp_priv), GFP_KERNEL);
	if (!sfp_priv)
		return -ENOMEM;

	pon->sfp = sfp_priv;

	for (i = 0; i < MAX_SFP_EEPROM; i++) {
		client = i2c_new_dummy_device(adapter, 0x50 + i);
		if (IS_ERR(client)) {
			ret = PTR_ERR(client);
			goto err;
		}

		if (!client->dev.driver ||
		    !try_module_get(client->dev.driver->owner)) {
			i2c_unregister_device(client);
			ret = -ENOENT;
			goto err;
		}
		sfp_priv->eep[i].client = client;
		ret = create_eep_file(pon, &sfp_priv->eep[i]);
		if (ret) {
			module_put(client->dev.driver->owner);
			i2c_unregister_device(client);
			sfp_priv->eep[i].client = NULL;
			ret = -ENOENT;
			goto err;
		}
	}

	/* "kset" is used as SUBSYSTEM in hotplug calls */
	sfp_priv->kset = kset_create_and_add("pon-sfp", NULL, &pon->dev->kobj);
	if (sfp_priv->kset) {
		sfp_priv->kobj.kset = sfp_priv->kset;

		ret = kobject_init_and_add(&sfp_priv->kobj, &sfp_ktype,
					   &pon->dev->kobj, "sfp%d", 0);
	}

	moddef0_register(pon, moddef_node);

	if (!g_sfp_priv)
		g_sfp_priv = sfp_priv;
	return 0;

err:
	kobject_put(&sfp_priv->kobj);
	kset_unregister(sfp_priv->kset);

	for (i = 0; i < MAX_SFP_EEPROM; i++) {
		client = sfp_priv->eep[i].client;
		if (!client)
			continue;
		remove_eep_file(pon, &sfp_priv->eep[i]);
		module_put(client->dev.driver->owner);
		i2c_unregister_device(client);
		sfp_priv->eep[i].client = NULL;
	}
	/* No need to call devm_kfree for devm-managed memory */

	return ret;
}

void pon_sfp_remove(struct pon_mbox *pon)
{
	struct pon_sfp_priv *sfp_priv = pon->sfp;
	struct i2c_client *client;
	int i;

	if (!sfp_priv)
		return;

	kobject_put(&sfp_priv->kobj);
	kset_unregister(sfp_priv->kset);

	if (sfp_priv->gpio_irq_mod_def_0)
		devm_free_irq(pon->dev, sfp_priv->gpio_irq_mod_def_0, pon);

	for (i = 0; i < MAX_SFP_EEPROM; i++) {
		client = sfp_priv->eep[i].client;
		if (!client)
			continue;
		remove_eep_file(pon, &sfp_priv->eep[i]);
		module_put(client->dev.driver->owner);
		i2c_unregister_device(client);
	}
	devm_kfree(pon->dev, sfp_priv);
	if (g_sfp_priv == sfp_priv)
		g_sfp_priv = NULL;
}
