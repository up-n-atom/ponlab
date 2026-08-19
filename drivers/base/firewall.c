// SPDX-License-Identifier: GPL-2.0
/*
 * Common code related to device firewall domains.
 *
 * Zhu YiXin <yzhu@maxlinear.com>
 * Copyright (C) 2021-2022 MaxLinear, Inc.
 *
 * Firewall domain support one domain can have multiple devices
 * And one device can have multiple domains (due to concept DT design)
 *
 * With current use case in maxlinear noc_firewall.dtsi, it use
 * One device to multiple domains, and one domain only have one device
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

#define pr_fmt(fmt) "FWLL: " fmt

#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/err.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/firewall.h>

/**
 * struct of_genfd_provider - firewall domain provider registration structure
 * @link: Entry in global list of FW domain providers.
 * @node: Pointer to device tree node of FW domain provider.
 * @xlate: Provider-specific xlate callback mapping a set of specifier cells
 *         into a FW domain.
 * @data: context pointer to be passed into @xlate callback.
 */
struct of_genfd_provider {
	struct list_head link;
	struct device_node *node;
	genfw_xlate_t xlate;
	void *data;
};

static LIST_HEAD(gfd_list);
static LIST_HEAD(of_genfd_providers);
static DEFINE_MUTEX(gfd_list_lock);
static DEFINE_MUTEX(of_genfw_lock);

#define genfd_status_on(genfd)		((genfd)->status == GENFD_STATE_ON)

/**
 * genfd_xlate_simple = Xlate function for direct node-domain mapping
 * @genfdspec: OF phandle args to map into a FW domain
 * @data: xlate function private data - pointer to struct generic_fw_domain
 *
 * This is a generic xlate function that can be used to model FW domains that
 * have their own defcie tree nodes. The private data of xlate function needs
 * to be a valid pointer to struct generic_fw_domain.
 */
static struct generic_fw_domain
*genfd_xlate_simple(struct of_phandle_args *genfdspec, void *data)
{
	return data;
}

static void genfd_lock(struct generic_fw_domain *gfd)
{
	mutex_lock(&gfd->mlock);
}

static void genfd_unlock(struct generic_fw_domain *gfd)
{
	mutex_unlock(&gfd->mlock);
}

static void genfd_free_dev_data(struct device *dev,
				struct generic_fw_domain_data *gfd_data)
{
	kfree(gfd_data);
}

static struct generic_fw_domain_data
*data_from_genfw_domain(struct generic_fw_domain *genfd, struct device *dev)
{
	struct generic_fw_domain_data *gfd_data;

	list_for_each_entry(gfd_data, &genfd->dev_list, list_node) {
		if (gfd_data->dev == dev)
			return gfd_data;
	}

	return NULL;
}

/**
 * genfd_turn_on - turn firewall on to a given PM domain
 * @genfd: FW domain to turn on.
 *
 * turn on firewall to @genfd
 */
static int genfd_turn_on(struct generic_fw_domain *genfd)
{
	int ret = 0;

	if (genfd_status_on(genfd))
		return 0;

	if (genfd->ops.firewall_on)
		ret = genfd->ops.firewall_on(genfd);
	if (ret)
		return ret;

	genfd->status = GENFD_STATE_ON;

	return 0;
}

/**
 * genfd_turn_off - Turn off firewall from a given FW domain.
 * @genfd: FW domain to turn off
 *
 * if all devices have been detached from @genfd, then turn off
 * from it.
 */
static int genfd_turn_off(struct generic_fw_domain *genfd)
{
	int ret = 0;

	if (!genfd_status_on(genfd))
		return 0;

	if (genfd->device_count)
		return -EBUSY;

	if (genfd->ops.firewall_off)
		ret = genfd->ops.firewall_off(genfd);

	if (ret)
		return ret;

	genfd->status = GENFD_STATE_OFF;

	return 0;
}

static int genfd_remove_device(struct generic_fw_domain *genfd,
			       struct device *dev)
{
	struct generic_fw_domain_data *gfd_data;

	dev_dbg(dev, "%s()\n", __func__);

	genfd_lock(genfd);

	genfd->device_count--;
	gfd_data = data_from_genfw_domain(genfd, dev);
	if (!gfd_data) {
		dev_err(dev, "%s: Couldn't find dev in fw domain: %s!\n",
			__func__, genfd->name);
		goto _nodata;
	}
	list_del_init(&gfd_data->list_node);

	genfd_unlock(genfd);

	dev_err(dev, "Detach fw domain:%s\n", genfd->name);
	if (genfd->ops.detach_dev)
		genfd->ops.detach_dev(genfd, dev);

	genfd_free_dev_data(dev, gfd_data);

	return 0;

_nodata:
	genfd_unlock(genfd);
	return -ENODEV;
}

static struct generic_fw_domain_data*
genfd_alloc_dev_data(struct device *dev, struct device_node *np)
{
	struct generic_fw_domain_data *gfd_data;

	gfd_data = kzalloc(sizeof(*gfd_data), GFP_KERNEL);
	if (!gfd_data)
		return ERR_PTR(-ENOMEM);

	gfd_data->dev = dev;
	gfd_data->data = np;

	return gfd_data;
}

static int genfd_add_device(struct generic_fw_domain *genfd, struct device *dev,
			    struct device_node *np)
{
	struct generic_fw_domain_data *gfd_data;
	int ret;

	dev_dbg(dev, "%s()\n", __func__);

	if (IS_ERR_OR_NULL(genfd) || IS_ERR_OR_NULL(dev))
		return -EINVAL;

	gfd_data = genfd_alloc_dev_data(dev, np);
	if (IS_ERR(gfd_data))
		return PTR_ERR(gfd_data);

	ret = genfd->ops.attach_dev ? genfd->ops.attach_dev(genfd, dev) : 0;
	if (ret)
		goto out;

	genfd_lock(genfd);
	genfd->device_count++;

	list_add_tail(&gfd_data->list_node, &genfd->dev_list);

	genfd_unlock(genfd);

out:
	if (ret)
		genfd_free_dev_data(dev, gfd_data);

	return ret;
}

/**
 * genfw_domain_init - Initialize a generic firewall domain
 * @genfd: FW domain object to initialize
 * @is_off: Initial value of the firewall is on or off
 *
 * Return 0 on successful initialization, eles negative error code
 */
int genfw_domain_init(struct generic_fw_domain *genfd, bool is_off)
{
	if (IS_ERR_OR_NULL(genfd))
		return -EINVAL;

	INIT_LIST_HEAD(&genfd->dev_list);
	mutex_init(&genfd->mlock);
	genfd->status = is_off ? GENFD_STATE_OFF : GENFD_STATE_ON;
	genfd->device_count = 0;
	genfd->provider = NULL;
	genfd->has_provider = false;

	device_initialize(&genfd->dev);
	dev_set_name(&genfd->dev, "%s", genfd->name);

	mutex_lock(&gfd_list_lock);
	list_add(&genfd->gfd_list_node, &gfd_list);
	mutex_unlock(&gfd_list_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(genfw_domain_init);

static void genfd_remove(struct generic_fw_domain *genfd)
{
	genfd_lock(genfd);
	if (genfd->provider) {
		pr_err("provider present, unable to remove %s\n",
		       genfd->name);
		goto err;
	}

	if (genfd->device_count) {
		pr_err("device present, unable to remove %s\n",
		       genfd->name);
		goto err;
	}

	list_del(&genfd->gfd_list_node);
	genfd_unlock(genfd);

	pr_debug("%s: remove %s\n", __func__, genfd->name);
	return;

err:
	genfd_unlock(genfd);
}

/**
 * genfw_domain_remove - Remove a generic firewall domain
 * @genfd: pointer to the firewall domain to be removed.
 *
 * The firewall domain will only be removed if the associated
 * provider has been removed and it has no device associated to it.
 */
void genfw_domain_remove(struct generic_fw_domain *genfd)
{
	if (IS_ERR_OR_NULL(genfd))
		return;

	mutex_lock(&gfd_list_lock);
	genfd_remove(genfd);
	mutex_unlock(&gfd_list_lock);
}
EXPORT_SYMBOL_GPL(genfw_domain_remove);

/**
 * of_genfd_del_provider - Remove a previously registered fW domain provider
 * @np: Device node pointer associated with the FW domain provider
 */
void of_genfd_del_provider(struct device_node *np)
{
	struct of_genfd_provider *cp, *tmp;
	struct generic_fw_domain *gfd;

	mutex_lock(&gfd_list_lock);
	mutex_lock(&of_genfw_lock);

	list_for_each_entry_safe(cp, tmp, &of_genfd_providers, link) {
		if (cp->node != np)
			continue;

		list_for_each_entry(gfd, &gfd_list, gfd_list_node) {
			if (gfd->provider != &np->fwnode)
				continue;

			gfd->has_provider = false;
		}

		list_del(&cp->link);
		of_node_put(cp->node);
		kfree(np);
		break;
	}
	mutex_unlock(&of_genfw_lock);
	mutex_unlock(&gfd_list_lock);
}
EXPORT_SYMBOL_GPL(of_genfd_del_provider);

/**
 * of_genfd_add_provider - Register a Firewall domain provider for a node
 * @np: device node pointer associated with the FW domain provider.
 * @xlate: Callback function for decodeing FW domain from phandle arguments.
 * @data: Context pointer for @xlate callback
 */
static int of_genfd_add_provider(struct device_node *np,
				 genfw_xlate_t xlate, void *data)
{
	struct of_genfd_provider *cp;

	cp = kzalloc(sizeof(*cp), GFP_KERNEL);
	if (!cp)
		return -ENOMEM;

	cp->node = of_node_get(np);
	cp->data = data;
	cp->xlate = xlate;

	mutex_lock(&of_genfw_lock);
	list_add(&cp->link, &of_genfd_providers);
	mutex_unlock(&of_genfw_lock);
	pr_debug("%s added domain provider from %s\n", __func__, np->name);

	return 0;
}

static bool genfd_present(const struct generic_fw_domain *genfd)
{
	const struct generic_fw_domain *gfd;

	list_for_each_entry(gfd, &gfd_list, gfd_list_node)
		if (gfd == genfd)
			return true;

	return false;
}

/**
 * of_genfd_add_provider_simple - Register a simple FW domain provider
 * @np: Device node pointer associated with the PM domain provider.
 * @gfd: Pointer to FW domain associated with the FW domain provider.
 */
int of_genfd_add_provider_simple(struct device_node *np,
				 struct generic_fw_domain *gfd)
{
	int ret = -EINVAL;

	if (!np || !gfd)
		return ret;

	mutex_lock(&gfd_list_lock);

	if (!genfd_present(gfd))
		goto unlock;

	gfd->dev.of_node = np;
	ret = of_genfd_add_provider(np, genfd_xlate_simple, gfd);
	if (ret)
		goto unlock;

	gfd->provider = &np->fwnode;
	gfd->has_provider = true;

unlock:
	mutex_unlock(&gfd_list_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(of_genfd_add_provider_simple);

/**
 * genfd_get_from_provider - look up firewall domain
 * @genfwspec: OF phandler args to use for look up
 *
 * Looks for a firewall domain provider under the node specified
 * by @genfwspec and if found, uses xlate function of the provider
 * to map phandler arguments to a firewall domain.
 *
 * Return a valid pointer to struct generic_fw_domain on success
 * or ERR_PTR() on failure.
 */
static struct generic_fw_domain
*genfd_get_from_provider(struct of_phandle_args *genfwspec)
{
	struct generic_fw_domain *gfd = ERR_PTR(-ENOENT);
	struct of_genfd_provider *provider;

	if (!genfwspec)
		return ERR_PTR(-EINVAL);

	mutex_lock(&of_genfw_lock);

	list_for_each_entry(provider, &of_genfd_providers, link) {
		if (provider->node == genfwspec->np)
			gfd = provider->xlate(genfwspec, provider->data);
		if (!IS_ERR(gfd))
			break;
	}

	mutex_unlock(&of_genfw_lock);

	return gfd;
}

static void
__genfw_dev_dm_detach(struct generic_fw_domain *gfd, struct device *dev)
{
	int ret;
	dev_dbg(dev, "removing device from FW domain %s\n", gfd->name);

	ret = genfd_remove_device(gfd, dev);
	if (ret < 0)
		return;

	genfd_lock(gfd);
	genfd_turn_off(gfd);
	genfd_unlock(gfd);
}

/**
 * genfw_dev_dm_detach - Detach a device from its firewall domain
 * @dev: Device to detach.
 *
 * Try to locate a corresponding generic firewall domain, which the device
 * was attached to previously. If such is found, the device is detached from it.
 */
void genfw_dev_dm_detach(struct device *dev)
{
	struct generic_fw_domain *genfd;
	struct generic_fw_domain_data *gfd_data;

	mutex_lock(&gfd_list_lock);
	list_for_each_entry(genfd, &gfd_list, gfd_list_node) {
		list_for_each_entry(gfd_data, &genfd->dev_list, list_node) {
			if (gfd_data->dev == dev) {
				__genfw_dev_dm_detach(genfd, dev);
				break;
			}
		}
	}
	mutex_unlock(&gfd_list_lock);
	pr_debug("====device(%s) detach firewall success===\n", dev_name(dev));
}
EXPORT_SYMBOL_GPL(genfw_dev_dm_detach);

static int __genfw_dev_dm_attach(struct device *dev, unsigned int index)
{
	struct of_phandle_args args;
	struct generic_fw_domain *gfd;
	int ret;

	ret = of_parse_phandle_with_args(dev->of_node, "firewall-domains",
					 "#firewall-domain-cells",
					 index, &args);
	if (ret < 0)
		return ret;

	mutex_lock(&gfd_list_lock);
	gfd = genfd_get_from_provider(&args);
	mutex_unlock(&gfd_list_lock);
	of_node_put(args.np);

	if (IS_ERR(gfd)) {
		dev_dbg(dev, "%s: Failed to find firewall domain: %ld\n",
			__func__, PTR_ERR(gfd));
		return PTR_ERR(gfd);
	}

	dev_dbg(dev, "adding to FW domain: %s\n", gfd->name);
	ret = genfd_add_device(gfd, dev, args.np);

	if (ret < 0)
		return ret;

	genfd_lock(gfd);
	ret = genfd_turn_on(gfd);
	genfd_unlock(gfd);

	if (ret)
		genfd_remove_device(gfd, dev);

	return ret;
}

/**
 * genfw_dev_dm_attach - Attach a device to its firewall domain using DT
 * @dev: device to attach
 *
 * return 0 when device doesn't need firewall or firewall rules apply okay
 * return negative error code in error condition including -EPROBE_DEFER
 */
int genfw_dev_dm_attach(struct device *dev)
{
	int i, domain_num, ret;

	if (!dev->of_node)
		return 0;

	/* Only allow one domain per device */
	domain_num = of_count_phandle_with_args(dev->of_node, "firewall-domains",
						"#firewall-domain-cells");
	if (domain_num < 0)
		return 0;

	for (i = 0; i < domain_num; ++i) {
		ret = __genfw_dev_dm_attach(dev, i);
		if (ret)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(genfw_dev_dm_attach);

static struct bus_type genfw_bus_type = {
	.name		= "genfirewall",
};

static int __init genfw_bus_init(void)
{
	return bus_register(&genfw_bus_type);
}
core_initcall(genfw_bus_init);
