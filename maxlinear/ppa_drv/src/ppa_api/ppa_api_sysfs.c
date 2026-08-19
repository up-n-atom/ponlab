// SPDX-License-Identifier: GPL-2.0
/* Allocating ppa in sysfs. Also, writable for configuration attached
 * to the system.
 *
 * Copyright (C) 2023-2024 MaxLinear, Inc.
 */

#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <net/ppa/ppa_api.h>
#include "ppa_api_sysfs.h"
#include "ppa_api_qos_tc_logical.h"
#include "ppa_api_sess_helper.h"
#include "ppa_api_session.h"

#define PPA_BASE_SCH_ATTR 4

/* Keep destination mac learning disabled by default */
uint8_t g_dst_mac_learning;
struct kobject *kobj_ref;

struct ppa_attribute {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);
	ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count);
};

#define PPA_ATTR(_name, _mode, _show, _store)			\
const struct ppa_attribute ppa_attr_##_name = {			\
	.attr = {.name = __stringify(_name),			\
		.mode = _mode },				\
	.show   = _show,					\
	.store  = _store,					\
}

/*
 * This function will be called when we read the sysfs file
 */
static ssize_t min_hit_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", g_ppa_min_hits);
}

/*
 * This function will be called when we write the sysfsfs file
 */
static ssize_t min_hit_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	uint32_t min_hit;

	if (kstrtouint(buf, 0, &min_hit))
		return -EINVAL;
	ppa_update_min_hit(min_hit);
	return count;
}

static ssize_t dst_mac_learn_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u (%s)\n", g_dst_mac_learning,
		       g_dst_mac_learning ? "Enabled" : "Disabled");
}

static ssize_t dst_mac_learn_store(struct kobject *kobj,
				   struct kobj_attribute *attr, const char *buf,
				   size_t len)
{
	if (*buf == '1') {
		g_dst_mac_learning = 1;
		pr_warn("This should be enabled only for lab testing. Do not enable it unless you know what it does!!\n");
	} else if (*buf == '0') {
		g_dst_mac_learning = 0;
		pr_info("Disabled destination mac learning in PPA macdb\n");
	} else {
		pr_warn("This is an experimental feature. You might encouter unexpected result if it is enabled!!\n");
		pr_err("echo 0/1 > /sys/kernel/ppa/core/dst_mac_learn\n");
		return -EINVAL;
	}

	return len;
}

#if IS_ENABLED(CONFIG_QOS_TC)
static ssize_t ppa_qos_base_sched_type_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "Base scheduler type: %s\n",
			ppa_qos_base_sch_type == PPA_QOS_BASE_SCH_DRR ?
			"DRR" : "WSP");
}

static ssize_t ppa_qos_base_sched_type_store(struct kobject *kobj,
		struct kobj_attribute *attr,
		const char *buf,
		size_t len)
{
	int32_t base_sch_id;

	if (len != PPA_BASE_SCH_ATTR) {
		pr_warn("The selected base scheduler type is not supported."
				" The supported types are DRR/WSP\n");
		return -EINVAL;
	}

	base_sch_id = ppa_qos_get_base_sch_id();
	if (base_sch_id != 0) {
		pr_info("WARNING: Remove all tc-qos configurations for new base"
				" scheduler to take effect\n");
		return -EINVAL;
	}
	if (strncasecmp(buf, "DRR", strlen("DRR")) == 0) {
		ppa_qos_base_sch_type = PPA_QOS_BASE_SCH_DRR;
		pr_info("The base scheduler type has been set to DRR.\n");
	} else if (strncasecmp(buf, "WSP", strlen("WSP")) == 0) {
		pr_info("The base scheduler type has been set to WSP.\n");
		ppa_qos_base_sch_type = PPA_QOS_BASE_SCH_WSP;
	} else {
		pr_warn("The selected base scheduler type is not supported."
				" The supported types are DRR/WSP\n");
		return -EINVAL;
	}

	return len;
}

static PPA_ATTR(ppa_qos_base_sch_type, 0660, ppa_qos_base_sched_type_show,
		ppa_qos_base_sched_type_store);
#endif /* CONFIG_QOS_TC */

static ssize_t uc_session_max_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	if (!capable(CAP_NET_ADMIN)) {
		pr_err("Read Permission denied\n");
		return -EPERM;
	}
	return sprintf(buf, "%uK\n", (g_session_item_max / SZ_1K));
}

static ssize_t uc_session_max_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	uint32_t sess_max;
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	int32_t ret;
	uint32_t pp_max_sess;
#endif

	if (!capable(CAP_NET_ADMIN)) {
		pr_err("Write Permission denied\n");
		return -EPERM;
	}

	if (kstrtouint(buf, 0, &sess_max))
		return -EINVAL;

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	ret = pp_max_sessions_get(&pp_max_sess);
	if (ret) {
		pr_err("Fetch PP sess failure\n");
		return -EINVAL;
	}
	
	if ((sess_max * SZ_1K) > (pp_max_sess * SZ_2))
		return -EINVAL;
#endif
	g_session_item_max = (sess_max * SZ_1K);
	return count;
}
static PPA_ATTR(uc_session_max, 0660, uc_session_max_show, uc_session_max_store);

static ssize_t non_accel_session_timeout_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	if (!capable(CAP_NET_ADMIN)) {
		pr_err("Read Permission denied\n");
		return -EPERM;
	}
	return sprintf(buf, "%u\n", g_non_accel_session_timeout);
}

static ssize_t non_accel_session_timeout_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	uint32_t timeout;

	if (!capable(CAP_NET_ADMIN)) {
		pr_err("Write Permission denied\n");
		return -EPERM;
	}

	if (kstrtouint(buf, 0, &timeout))
		return -EINVAL;

	g_non_accel_session_timeout = timeout;
	return count;
}
static PPA_ATTR(non_accel_session_timeout, 0660, non_accel_session_timeout_show,
		non_accel_session_timeout_store);

static PPA_ATTR(min_hit, 0660, min_hit_show, min_hit_store);
static PPA_ATTR(dst_mac_learn, 0660, dst_mac_learn_show, dst_mac_learn_store);

#define to_ppa_attr(_attr) container_of(_attr, struct ppa_attribute, attr)

static const struct ppa_attribute *ppa_attrs[] = {
	&ppa_attr_min_hit,
	&ppa_attr_dst_mac_learn,
#if IS_ENABLED(CONFIG_QOS_TC)
	&ppa_attr_ppa_qos_base_sch_type,
#endif
	&ppa_attr_uc_session_max,
	&ppa_attr_non_accel_session_timeout,
	NULL
};

/*
 * ppa_sysfs Init function
 */
int32_t ppa_api_sysfs_init(void)
{
	const struct ppa_attribute **a;

	/* Creating a directory in /sys/kernel/ */
	kobj_ref = kobject_create_and_add("ppa", kernel_kobj);
	/* Creating sysfs files */
	for (a = ppa_attrs; *a; ++a) {
		if (sysfs_create_file(kobj_ref, &(*a)->attr)) {
			pr_err("Cannot create sysfs file......\n");
			goto r_sysfs;
		}
	}
	pr_debug("PPA added in sysfs...\n");
	return 0;

r_sysfs:
	kobject_put(kobj_ref);
	for (a = ppa_attrs; *a; ++a)
		sysfs_remove_file(kernel_kobj, &((*a)->attr));
	return -1;
}

/*
 * ppa_sysfs exit function
 */
void ppa_api_sysfs_exit(void)
{
	const struct ppa_attribute **a;

	kobject_put(kobj_ref);
	for (a = ppa_attrs; *a; ++a)
		sysfs_remove_file(kernel_kobj, &((*a)->attr));
	pr_debug("PPA removed from sysfs...\n");
}
