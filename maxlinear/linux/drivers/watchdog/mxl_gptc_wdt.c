// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
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

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <clocksource/mxl-gptc-timer.h>
#include <linux/sched/debug.h>
#include <linux/nmi.h>

#include "watchdog_pretimeout.h"

#define DEFAULT_TIMEOUT		30
#define DEFAULT_PRETIMEOUT	5

struct mxl_match_data {
	u32 reg_type;
};

static unsigned int timeout = DEFAULT_TIMEOUT;
module_param(timeout, uint, 0644);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds\n"
	" (default=" __MODULE_STRING(DEFAULT_TIMEOUT) ")");

static unsigned int pretimeout = DEFAULT_PRETIMEOUT;
module_param(pretimeout, uint, 0644);
MODULE_PARM_DESC(pretimeout, "Watchdog pretimeout in seconds\n"
	" (default=" __MODULE_STRING(DEFAULT_PRETIMEOUT) ")");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0644);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started\n"
	" (default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

struct mxl_wdt_drvdata {
	struct watchdog_device wdd;
	struct regmap *wdt_rst_base;
	int bootstatus;
	const struct mxl_match_data *soc_data;
};


static int reboot_reason = 0;

static int __init parse_rst_reason(char *rst_reason)
{
	if (strcmp(rst_reason, "GLOBAL_SW_RESET") == 0) {
		reboot_reason = 0;
	} else if (strcmp(rst_reason, "POR_RESET") == 0) {
		reboot_reason = WDIOF_POWEROVER;
	} else if (strcmp(rst_reason, "ATOM_WDT_RESET") == 0 ||
	       strcmp(rst_reason, "ARC_WDT_RESET") == 0) {
		reboot_reason = WDIOF_CARDRESET;
	} else {
		pr_err("Invalid rst_reason value: %s\n", rst_reason);
		return 0;
	}

	pr_debug("Parsed rst_reason: %s, reboot_reason: %d\n", rst_reason, reboot_reason);
	return 1;
}

__setup("rst_reason=", parse_rst_reason);

static inline void gptc_wdt_rst_en_write(struct mxl_wdt_drvdata *drvdata, u32 val)
{
	if (drvdata->soc_data->reg_type)
		regmap_write(drvdata->wdt_rst_base, BASEIA_WDT_RST_EN_B0, val);
	else
		regmap_write(drvdata->wdt_rst_base, BASEIA_WDT_RST_EN, val);
}

static inline void gptc_wdt_rst_en_read(struct mxl_wdt_drvdata *drvdata, u32 *p_val)
{
	if (drvdata->soc_data->reg_type)
		regmap_read(drvdata->wdt_rst_base, BASEIA_WDT_RST_EN_B0, p_val);
	else
		regmap_read(drvdata->wdt_rst_base, BASEIA_WDT_RST_EN, p_val);
}

static int mxl_gptc_wdt_start(struct watchdog_device *wdd)
{
	struct mxl_wdt_drvdata *drvdata = wdd->driver_data;
	int ret;
	u32 val;
	int cpu;

	gptc_wdt_rst_en_read(drvdata, &val);
	for_each_present_cpu(cpu) {
		ret = gptc_wdt_start(cpu);
		if (!ret) {
			val |= BIT(cpu);
		}
	}

	val |= BIA_WDT;

	/* Enable WDT reset to RCU */
	gptc_wdt_rst_en_write(drvdata, val);

	return 0;
}

static int mxl_gptc_wdt_stop(struct watchdog_device *wdd)
{
	struct mxl_wdt_drvdata *drvdata = wdd->driver_data;
	u32 val;
	int cpu;

	/* Disable WDT reset to RCU */
	gptc_wdt_rst_en_read(drvdata, &val);
	val &= ~(WDT0 | WDT1 | WDT2 | WDT3 | BIA_WDT);
	gptc_wdt_rst_en_write(drvdata, val);

	for_each_online_cpu(cpu) {
		gptc_wdt_stop(cpu);
	}

	return 0;
}

static int mxl_gptc_wdt_set_pretimeout(struct watchdog_device *wdd,
				unsigned int pretimeout)
{
	int cpu;

	wdd->pretimeout = pretimeout;

	for_each_online_cpu(cpu)
		gptc_wdt_set_timeout(cpu, wdd->timeout, wdd->pretimeout);

	return 0;
}

static int mxl_gptc_wdt_set_timeout(struct watchdog_device *wdd,
				unsigned int timeout)
{
	int cpu;

	wdd->timeout = timeout;

	for_each_online_cpu(cpu)
		gptc_wdt_set_timeout(cpu, wdd->timeout, wdd->pretimeout);

	return 0;
}

static uint32_t mxl_gptc_wdt_get_timeleft(struct watchdog_device *wdd)
{
	u32 timeleft = U32_MAX;
	int cpu;

	for_each_online_cpu(cpu)
		timeleft = min(timeleft, gptc_wdt_get_timeleft(cpu));

	return timeleft;
}

static void mxl_gptc_wdt_irq(void *data)
{
	struct platform_device *pdev = data;
	struct mxl_wdt_drvdata *drvdata = platform_get_drvdata(pdev);
	struct watchdog_device *wdd = &drvdata->wdd;

	if (!wdd) {
		panic("no watchdog device in pretimeout");
		return;
	}
	dump_stack();
	trigger_allbutself_cpu_backtrace();
	watchdog_notify_pretimeout(wdd);
}

static const struct watchdog_info mxl_gptc_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING |
		   WDIOF_MAGICCLOSE | WDIOF_PRETIMEOUT |
		   WDIOF_CARDRESET | WDIOF_POWEROVER,
	.identity = "Hardware Watchdog for Intel LGM",
};

static const struct watchdog_ops mxl_gptc_wdt_ops = {
	.owner = THIS_MODULE,
	.start = mxl_gptc_wdt_start,
	.stop = mxl_gptc_wdt_stop,
	.set_timeout = mxl_gptc_wdt_set_timeout,
	.set_pretimeout = mxl_gptc_wdt_set_pretimeout,
	.get_timeleft = mxl_gptc_wdt_get_timeleft,
};

static int mxl_gptc_wdt_probe(struct platform_device *pdev)
{
	int ret, cpu;

	struct device *dev = &pdev->dev;
	const struct mxl_match_data *match_data;
	struct watchdog_device *wdd;
	struct mxl_wdt_drvdata *drvdata;
	struct regmap *wdt_rst_base = syscon_regmap_lookup_by_phandle(
			dev->of_node,
			"mxl,wdt-rst-base");
	if (IS_ERR(wdt_rst_base)) {
		dev_err(dev, "failed to get syscon\n");
		return PTR_ERR(wdt_rst_base);
	}

	drvdata = devm_kzalloc(dev, sizeof(*drvdata), GFP_KERNEL);
	if (IS_ERR(drvdata))
		return PTR_ERR(drvdata);

	match_data = of_device_get_match_data(dev);

	for_each_online_cpu(cpu) {
		ret = gptc_wdt_request(cpu, true, &mxl_gptc_wdt_irq, pdev);
		if (ret < 0) {
			if (ret == -EPROBE_DEFER) {
				return ret;
			}
			dev_dbg(dev, "no watchdog configured for cpu-%d (%d)\n", cpu, ret);
			continue;
		}

		dev_info(dev, "Watchdog timer configured for cpu-%d (nowayout=%d)\n", cpu, nowayout);
	}

	drvdata->wdt_rst_base = wdt_rst_base;
	drvdata->soc_data = match_data;
	drvdata->bootstatus = reboot_reason;

	wdd = &drvdata->wdd;
	wdd->info = &mxl_gptc_wdt_info;
	wdd->ops = &mxl_gptc_wdt_ops;
	wdd->min_timeout = 1;
	wdd->max_timeout = 200;
	wdd->min_hw_heartbeat_ms = 1 * 1000;
	wdd->max_hw_heartbeat_ms = 200 * 1000; // FIXME
	wdd->parent = dev;
	wdd->bootstatus = drvdata->bootstatus;
	watchdog_set_drvdata(wdd, drvdata);
	watchdog_init_timeout(wdd, timeout, dev);
	watchdog_set_nowayout(wdd, nowayout);
	watchdog_stop_on_reboot(wdd);
	watchdog_stop_on_unregister(wdd);

	wdd->pretimeout = (wdd->timeout > pretimeout) ? pretimeout : 0;

	for_each_online_cpu(cpu) {
		gptc_wdt_set_timeout(cpu, wdd->timeout, wdd->pretimeout);
	}

	platform_set_drvdata(pdev, drvdata);

	return devm_watchdog_register_device(dev, wdd);
}

static int __exit mxl_gptc_wdt_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mxl_wdt_drvdata *drvdata = platform_get_drvdata(pdev);
	struct watchdog_device *wdd = &drvdata->wdd;
	int cpu;

	for_each_online_cpu(cpu) {

		gptc_wdt_release(cpu);
		dev_info(dev, "cpu = %d\n", cpu);
	}

	watchdog_unregister_pretimeout(wdd);

	return 0;
}

static void mxl_gptc_wdt_shutdown(struct platform_device *pdev)
{
	struct mxl_wdt_drvdata *drvdata = platform_get_drvdata(pdev);
	struct watchdog_device *wdd = &drvdata->wdd;

	mxl_gptc_wdt_stop(wdd);
}

static struct mxl_match_data lgm_b = {
	.reg_type = 1,
};

static struct mxl_match_data lgm = {
	.reg_type = 0,
};

static const struct of_device_id mxl_gptc_wdt_match[] = {
	{ .compatible = "mxl,lgm-a-gptc-wdt", .data = &lgm },
	{ .compatible = "mxl,lgm-b-gptc-wdt", .data = &lgm_b },
	{ .compatible = "mxl,gptc-wdt", .data = &lgm_b },
	{},
};

static struct platform_driver mxl_gptc_wdt_driver = {
	.probe = mxl_gptc_wdt_probe,
	.remove = __exit_p(mxl_gptc_wdt_remove),
	.shutdown = mxl_gptc_wdt_shutdown,
	.driver = {
		.name = "lgm,gptc-wdt",
		.owner = THIS_MODULE,
		.of_match_table = mxl_gptc_wdt_match,
	},
};

module_platform_driver(mxl_gptc_wdt_driver);

MODULE_DESCRIPTION("Intel GPTC Watchdog Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mxl_gptc_wdt");
