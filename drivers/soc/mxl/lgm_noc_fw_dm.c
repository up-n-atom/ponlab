// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2021-2022 MaxLinear, Inc.
 *
 * lgm_noc_fw_dm - Maxlinear firewall default domain driver
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
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

static int lgm_fw_dm_probe(struct platform_device *pdev)
{
	pr_info("lgm noc firewall default domain driver loaded!\n");

	return 0;
}

static const struct of_device_id lgm_fw_dm_dt_ids[] = {
	{.compatible = "mxl,lgm-noc-fw-default"},
	{ },
};

static struct platform_driver lgm_fw_dm_driver = {
	.probe = lgm_fw_dm_probe,
	.driver = {
		.name = "mxl-noc-fw-domain",
		.of_match_table = of_match_ptr(lgm_fw_dm_dt_ids),
		/* Firewall default domain driver cannot be removed. */
		.suppress_bind_attrs = true,
	},
};

static int __init lgm_fw_dm_drv_register(void)
{
	return platform_driver_register(&lgm_fw_dm_driver);
}
arch_initcall_sync(lgm_fw_dm_drv_register);
