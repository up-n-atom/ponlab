/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 *
 * firewall.h  - Definition and headers related to firewall domains.
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


#ifndef _LINUX_FIREWALL_H
#define _LINUX_FIREWALL_H

#include <linux/device.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/of.h>

typedef struct generic_fw_domain *(*genfw_xlate_t)(struct of_phandle_args *args,
						   void *data);

enum genfd_status {
	GENFD_STATE_ON,		/* firewall domain is on */
	GENFD_STATE_OFF,	/* firewall domain is off */
};

/* domain operations */
struct genfd_domain_ops {
	int (*firewall_on)(struct generic_fw_domain *domain);
	int (*firewall_off)(struct generic_fw_domain *domain);
	int (*attach_dev)(struct generic_fw_domain *domain, struct device *dev);
	int (*detach_dev)(struct generic_fw_domain *domain, struct device *dev);
};

struct generic_fw_domain {
	struct device dev;
	struct genfd_domain_ops ops;
	struct list_head gfd_list_node;
	struct list_head dev_list;
	struct fwnode_handle *provider;
	int has_provider;
	const char *name;
	struct mutex mlock; /* lock to protect domain structure member */
	enum genfd_status status;
	unsigned int device_count;
};

struct generic_fw_domain_data {
	struct device *dev;
	struct list_head list_node;
	void *data;
};

#if IS_ENABLED(CONFIG_P2P_FIREWALL)
int genfw_domain_init(struct generic_fw_domain *genfd, bool is_off);
void genfw_domain_remove(struct generic_fw_domain *genfd);
void of_genfd_del_provider(struct device_node *np);
int of_genfd_add_provider_simple(struct device_node *np,
				 struct generic_fw_domain *gfd);
void genfw_dev_dm_detach(struct device *dev);
int genfw_dev_dm_attach(struct device *dev);
#else
static inline int genfw_domain_init(struct generic_fw_domain *genfd,
				    bool is_off)
{
	return -EPERM;
}

static inline void genfw_domain_remove(struct generic_fw_domain *genfd)
{
}

static inline void of_genfd_del_provider(struct device_node *np)
{
}

static inline int of_genfd_add_provider_simple(struct device_node *np,
					       struct generic_fw_domain *gfd)
{
	return -EPERM;
}

static inline void genfw_dev_dm_detach(struct device *dev)
{
}

static inline int genfw_dev_dm_attach(struct device *dev)
{
	return 0;
}
#endif /* CONFIG_P2P_FIREWALL */

#endif /*_LINUX_FIREWALL_H */

