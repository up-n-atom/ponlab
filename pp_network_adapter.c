/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PPv4 Network Adapter
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "pp_network_adapter_internal.h"

#define MOD_NAME "PPv4 Network Adapter"
#define MOD_VERSION "0.0.1"

/**
 * @brief Add function name and line number for all pr_* prints
 */
#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_NAD]:%s:%d: " fmt, __func__, __LINE__
#endif

extern s32 pp_dev_late_init(void);

/**
 * @brief Module exit function
 * @return int 0 on success
 */
static void __net_adapter_exit(void)
{
	net_adapter_dbg_clean();
	pr_debug("end\n");
}

/**
 * @brief Module init function
 * @return int 0 on success
 */
static int __net_adapter_init(void)
{
	s32 ret = 0;

	pr_debug("start\n");

	ret = pp_dev_late_init();
	if (unlikely(ret)) {
		pr_err("pp late init failed, ret = %d\n", ret);
		return ret;
	}

	/* Init debug */
	ret = net_adapter_dbg_init();
	if (unlikely(ret)) {
		pr_err("Failed to initialized module debugfs, ret %d\n", ret);
		return ret;
	}

	return ret;
}

/**
 * @brief Modules attributes
 */
MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION(MOD_NAME);
MODULE_LICENSE("GPL");
MODULE_VERSION(MOD_VERSION);
module_init(__net_adapter_init);
module_exit(__net_adapter_exit);
