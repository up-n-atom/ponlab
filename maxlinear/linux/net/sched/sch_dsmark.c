// SPDX-License-Identifier: GPL-2.0-only
/* net/sched/sch_dsmark.c - Differentiated Services field marker
 * Dummy module to support backward compatibility for base openwrt
 */

#include <linux/module.h>
#include <linux/init.h>

static int __init dsmark_module_init(void)
{
	return 0;
}

static void __exit dsmark_module_exit(void)
{
	return;
}

module_init(dsmark_module_init)
module_exit(dsmark_module_exit)

MODULE_LICENSE("GPL");
