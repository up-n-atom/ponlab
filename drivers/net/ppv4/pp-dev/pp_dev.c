/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP Device Driver
 */

#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/uaccess.h>
#include <linux/pp_qos_api.h>
#include "pp_dev.h"
#include "pp_dma.h"
#include "pp_common.h"
#include "pp_port_mgr.h"
#include "pp_qos_fw.h"
#include "pput.h"
#include "pp_si.h"
#include "pp_desc.h"

#define DRV_NAME "PPv4_Platform_Driver"

#define CONST2STR(c) #c
#define DRV_VERSION \
	CONST2STR(PP_VER_MAJOR)"."	\
	CONST2STR(PP_VER_MAJOR_MID)"."	\
	CONST2STR(PP_VER_MID)"."	\
	CONST2STR(PP_VER_MINOR_MID)"."	\
	CONST2STR(PP_VER_MINOR)

static int __pp_dev_probe(struct platform_device *pdev);
static int __pp_dev_remove(struct platform_device *pdev);

static struct device *dev;
static u32 hw_clk;

static const struct of_device_id pp_dev_match[] = {
	{ .compatible = "intel,ppv4" },
	{},
};
MODULE_DEVICE_TABLE(of, pp_dev_match);

/* Platform driver */
static struct platform_driver pp_plat_drv = {
	.probe  = __pp_dev_probe,
	.remove = __pp_dev_remove,
	.driver = {
		.name  = DRV_NAME,
		.owner = THIS_MODULE,
#if IS_ENABLED(CONFIG_OF)
		.of_match_table = pp_dev_match,
#endif
	},
};

/**
 * @brief Add function name and line number for all pr_* prints
 */
#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_DEV]:%s:%d: " fmt, __func__, __LINE__
#endif

bool __pp_is_ready(const char *caller)
{
	struct pp_dev_priv *priv;

	if (unlikely(!dev))
		goto not_ready;

	priv = dev_get_drvdata(dev);
	if (unlikely(!priv))
		goto not_ready;

	if (likely(priv->ready))
		return true;

not_ready:
	pr_err("%s: PP driver isn't ready!\n", caller);
	return false;
}

/**
 * @brief get the PP device
 * @return struct device* pointer to device
 */
struct device *pp_dev_get(void)
{
	return dev;
}

u32 pp_dev_hw_clk_get(void)
{
	u32 dfs = pp_qos_dfs_level_get();

	return hw_clk >> dfs;
}

struct pp_dev_priv *pp_priv_get(void)
{
	struct device *dev;

	dev = pp_dev_get();
	if (unlikely(ptr_is_null(dev)))
		return NULL;

	return dev_get_drvdata(dev);
}

/**
 * @brief Init pp dev driver configuration from device tree
 * @param pdev platform device
 */
static s32 __pp_dts_cfg_init(struct platform_device *pdev)
{
	s32 ret = 0;
	struct pp_dev_priv *priv;

	if (unlikely(ptr_is_null(pdev)))
		return -EPERM;

	priv = dev_get_drvdata(dev);
	if (unlikely(ptr_is_null(priv)))
		return -EPERM;

	if (IS_ENABLED(CONFIG_OF)) {
		ret = pp_dts_cfg_get(pdev, &priv->dts_cfg);
		if (unlikely(ret)) {
			pr_err("Failed to fetch dts config, ret %d\n", ret);
			return ret;
		}
		pp_dts_cfg_dump(&priv->dts_cfg);
	}

	return ret;
}

/**
 * @brief Set the parent debug directory for all modules.
 * @param priv device's private data
 * @return s32
 */
static void __pp_dbg_init(struct pp_dev_priv *priv)
{
	/* main pp sysfs folder */
	priv->dts_cfg.pmgr_params.sysfs = priv->pp_sysfs_dir;
	priv->dts_cfg.chk_param.sysfs   = priv->pp_sysfs_dir;
	priv->dts_cfg.misc_params.sysfs = priv->pp_sysfs_dir;
	priv->dts_cfg.qos_params.sysfs  = priv->pp_sysfs_dir;

	if (IS_ENABLED(CONFIG_DEBUG_FS)) {
		/* main pp debugfs folder */
		priv->dts_cfg.smgr_params.dbgfs      = priv->pp_dbgfs_dir;
		priv->dts_cfg.bmgr_params.dbgfs      = priv->pp_dbgfs_dir;
		priv->dts_cfg.pmgr_params.dbgfs      = priv->pp_dbgfs_dir;
		priv->dts_cfg.misc_params.root_dbgfs = priv->pp_dbgfs_dir;
		priv->dts_cfg.qos_params.dbgfs       = priv->pp_dbgfs_dir;
		priv->dts_cfg.chk_param.root_dbgfs   = priv->pp_dbgfs_dir;

		/* pp hal debugfs folder */
		priv->dts_cfg.misc_params.hal_dbgfs  = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.infra_params.dbgfs     = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.parser_params.dbgfs    = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.rpb_params.dbgfs       = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.port_dist_params.dbgfs = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.cls_param.dbgfs        = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.chk_param.dbgfs        = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.mod_param.dbgfs        = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.rx_dma_params.dbgfs    = priv->pp_hal_dbgfs_dir;
		priv->dts_cfg.uc_params.dbgfs        = priv->pp_hal_dbgfs_dir;
	}
}

static int init_clocks(struct pp_dev_priv *priv)
{
	const char *clock_name;
	struct clk *clk;
	u32 i, clk_rate;

	for (i = 0; i < ARRAY_SIZE(priv->dts_cfg.pp_clocks); i++) {
		if (!priv->dts_cfg.pp_clocks[i])
			return 0;

		clock_name = priv->dts_cfg.pp_clocks[i];
		clk = devm_clk_get(dev, clock_name);
		if (IS_ERR(clk)) {
			pr_err("failed to get clock '%s', error %ld\n",
			       clock_name, PTR_ERR(clk));
			return -ENODEV;
		}

		clk_prepare_enable(clk);
		clk_rate = clk_get_rate(clk);
		/* Hz -> Mhz*/
		clk_rate /= 1000000;
		pr_debug("enabling pp clock '%s' obtained rate %u Mhz\n", clock_name, clk_rate);
		if (!strcmp(priv->dts_cfg.pp_clocks[i], "ppv4")) {
			priv->dts_cfg.hw_clk = clk_rate;
			priv->dts_cfg.chk_param.hw_clk = clk_rate;
		}
	}

	hw_clk = priv->dts_cfg.hw_clk;
	priv->is_clk_enable = true;
	return 0;
}

static void disable_clocks(struct pp_dev_priv *priv)
{
	const char *clock_name;
	struct clk *clk;
	u32 i;

	if (!priv->is_clk_enable)
		return;

	for (i = 0; i < ARRAY_SIZE(priv->dts_cfg.pp_clocks); i++) {
		if (!priv->dts_cfg.pp_clocks[i])
			return;

		clock_name = priv->dts_cfg.pp_clocks[i];
		clk = devm_clk_get(dev, clock_name);
		if (IS_ERR(clk)) {
			pr_err("failed to get clock '%s', error %ld\n",
			       clock_name, PTR_ERR(clk));
			continue;
		}

		pr_debug("disabling pp clock '%s'\n", clock_name);
		clk_disable_unprepare(clk);
	}

	priv->is_clk_enable = false;
}

/**
 * @brief Exit pp hal modules
 */
static void __pp_hal_exit(void)
{
	port_dist_exit();
	rpb_exit();
	prsr_exit();
	cls_exit();
	chk_exit();
	mod_exit();
	uc_exit();
	rx_dma_exit();
	bm_exit();
	infra_exit();
}

#if IS_ENABLED(PP_POWER_ON_REGS_TEST)
#include "pp_dev_test.h"
extern s32 uc_dbg_clk_set(struct uc_init_params *init_param, bool en);
extern s32 infra_clck_ctrl_set(struct infra_init_param *init_param, bool en);

static s32 __reg_access_test(struct test_reg *reg, bool r_only)
{
	u32 val;
	void *virt;

	virt = ioremap(reg->addr, 4);
	if (unlikely(!virt)) {
		pr_err("Failed to map reg %#llx\n", reg->addr);
		return -ENOMEM;
	}

	if (!r_only)
		PP_REG_WR32(virt, reg->val);

	val = PP_REG_RD32(virt);
	/* unmap the memory */
	iounmap(virt);

	if (reg->val == val)
		return 0;

	pr_err("PP reg %#llx value is %#x, expected is %#x\n",
	       reg->addr, val, reg->val);
	return -EIO;
}

/**
 * @brief Test all PP register and memories
 */
static void __pp_initial_hw_test(void)
{
	struct test_reg *reg;
	s32 test_ret = 0;

	/* enable necessary clocks */
	if (unlikely(infra_clck_ctrl_set(&priv->dts_cfg.infra_params, true))) {
		pr_err("Failed to enable infra clocks\n");
		return;
	}

	if (unlikely(uc_dbg_clk_set(&priv->dts_cfg.uc_params, true))) {
		pr_err("Failed to enable uc clocks\n");
		goto disable_infra;
	}

	/* Test the registers default values */
	for_each_arr_entry(reg, reg_dflt, ARRAY_SIZE(reg_dflt))
		test_ret |= __reg_access_test(reg, true);

	/* Test the registers random values */
	for_each_arr_entry(reg, reg_rand, ARRAY_SIZE(reg_rand))
		test_ret |= __reg_access_test(reg, false);

	/* Test the registers all '1' values */
	for_each_arr_entry(reg, reg_mask, ARRAY_SIZE(reg_mask))
		test_ret |= __reg_access_test(reg, false);

	/* Test the memories all '0' values */
	for_each_arr_entry(reg, mem_zero, ARRAY_SIZE(mem_zero))
		test_ret |= __reg_access_test(reg, false);

	/* Test the memories all '1' values */
	for_each_arr_entry(reg, mem_mask, ARRAY_SIZE(mem_mask))
		test_ret |= __reg_access_test(reg, false);

	/* Test the memories random values */
	for_each_arr_entry(reg, mem_rand, ARRAY_SIZE(mem_rand))
		test_ret |= __reg_access_test(reg, false);

	/* Test the memories random (reversed) values */
	for_each_arr_entry(reg, mem_rand_rev, ARRAY_SIZE(mem_rand_rev))
		test_ret |= __reg_access_test(reg, false);

	pr_err("+======================================+\n");
	pr_err("| PP HW registers test %-4s            |\n",
	       test_ret ? "fail" : "pass");
	pr_err("+======================================+\n");

	/* disable back the clocks */
	if (unlikely(uc_dbg_clk_set(&priv->dts_cfg.uc_params, false)))
		pr_err("Failed to disable infra and uc clocks\n");
disable_infra:
	if (unlikely(infra_clck_ctrl_set(&priv->dts_cfg.infra_params, false)))
		pr_err("Failed to disable infra and uc clocks\n");
}
#endif

/**
 * @brief Init pp hal modules
 * @return s32 0 on success
 */
static s32 __pp_hal_init(struct pp_dev_priv *priv)
{
	s32 ret = 0;

#if IS_ENABLED(PP_POWER_ON_REGS_TEST)
	__pp_initial_hw_test();
#endif

	ret = pp_regs_init();
	if (unlikely(ret)) {
		pr_err("Failed to init infra module\n");
		goto out;
	}

	ret = infra_init(&priv->dts_cfg.infra_params);
	if (unlikely(ret)) {
		pr_err("Failed to init infra module\n");
		goto out;
	}

	/* init bm */
	ret = bm_init(&priv->dts_cfg.bm_params);
	if (unlikely(ret)) {
		pr_err("Failed to init bm module\n");
		goto out;
	}
	/* init rxdma */
	ret = rx_dma_init(&priv->dts_cfg.rx_dma_params);
	if (unlikely(ret)) {
		pr_err("Failed to init rxdma module\n");
		goto out;
	}
	/* init modifier */
	ret = mod_init(&priv->dts_cfg.mod_param);
	if (unlikely(ret)) {
		pr_err("Failed to init modifier module\n");
		goto out;
	}
	/* init checker */
	ret = chk_init(&priv->dts_cfg.chk_param);
	if (unlikely(ret)) {
		pr_err("Failed to init checker module\n");
		goto out;
	}
	/* init classifier */
	ret = cls_init(&priv->dts_cfg.cls_param);
	if (unlikely(ret)) {
		pr_err("Failed to init classifier module\n");
		goto out;
	}
	/* init parser */
	ret = prsr_init(&priv->dts_cfg.parser_params);
	if (unlikely(ret)) {
		pr_err("Failed to init parser module\n");
		goto out;
	}
	/* init rpb */
	ret = rpb_init(&priv->dts_cfg.rpb_params);
	if (unlikely(ret)) {
		pr_err("Failed to init rpb module\n");
		goto out;
	}
	/* init port dist */
	ret = port_dist_init(&priv->dts_cfg.port_dist_params);
	if (unlikely(ret)) {
		pr_err("Failed to init port dist module\n");
		goto out;
	}

	/* init HW dynamic clock gating */
	if (priv->dts_cfg.infra_params.valid)
		infra_dynamic_clk_init();

out:
	return ret;
}

/**
 * @brief Exit pp api modules
 */
static void __pp_api_exit(void)
{
	pp_misc_exit();
	pp_smgr_exit();
	pmgr_exit();
	pp_bmgr_exit();
	pp_qos_mgr_exit();
}

/**
 * @brief Init pp api modules
 * @param dev device
 * @return s32 0 on success
 */
static s32 __pp_api_init(struct device *dev)
{
	s32 ret = 0;
	struct pp_qos_dev *qdev;
	struct pp_dev_priv *priv;

	priv = dev_get_drvdata(dev);
	if (unlikely(ptr_is_null(priv)))
		return -EINVAL;

	/* init qos */
	if (priv->dts_cfg.qos_params.valid) {
		pp_qos_mgr_init();
		qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
		if (unlikely(ptr_is_null(qdev)))
			return -EINVAL;

		ret = pp_qos_dev_init(qdev, &priv->dts_cfg.qos_params);
		if (unlikely(ret)) {
			pr_err("pp_qos_dev_init failed\n");
			goto out;
		}
	}

	/* init bm */
	ret = pp_bmgr_init(&priv->dts_cfg.bmgr_params);
	if (unlikely(ret)) {
		pr_err("Failed to init buffer mgr module\n");
		goto out;
	}

	/* init port mgr */
	if (priv->dts_cfg.port_dist_params.valid) {
		ret = pmgr_init(&priv->dts_cfg.pmgr_params);
		if (unlikely(ret)) {
			pr_err("Failed to init port mgr module\n");
			goto out;
		}
	}

	/* init session mgr */
	ret = pp_smgr_init(dev);
	if (unlikely(ret)) {
		pr_err("Failed to init session mgr module\n");
		goto out;
	}

	/* init misc */
	ret = pp_misc_init(dev);
	if (unlikely(ret)) {
		pr_err("Failed to init misc module\n");
		goto out;
	}

out:
	return ret;
}

/**
 * @brief Module exit function
 * @return int 0 on success
 */
static void __pp_dev_exit(void)
{
	pr_debug("start\n");
	platform_driver_unregister(&pp_plat_drv);
	pr_info("__pp_dev_exit done\n");
}

/**
 * @brief Platform driver probe function, do as follow:<br>
 *         1. Initialized driver database <br>
 *         2. Initialized PP drivers <br>
 *         3. Create dbgfs stuff <br>
 * @param pdev platform device
 * @return int 0 on success, non-zero value otherwise
 */
static int __pp_dev_probe(struct platform_device *pdev)
{
	struct pp_dev_priv *priv;
	s32 ret;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		pr_err("Failed to allocate pp_dev_priv memory of size %zu\n",
		       sizeof(*priv));
		return -ENOMEM;
	}
	pr_debug("start (pdev: %s)\n", pdev->name);

	platform_set_drvdata(pdev, priv);
	dev = &pdev->dev;
	priv->is_clk_enable = false;

	/* Create PP sysfs directory */
	priv->pp_sysfs_dir = kobject_create_and_add("pp", kernel_kobj);
	if (unlikely(!priv->pp_sysfs_dir)) {
		pr_err("Failed to create PP sysfs dir\n");
		ret = -ENOMEM;
		goto out;
	}
	pr_debug("pp sysfs directory created\n");
	/* Create PP debugfs directory */
	if (IS_ENABLED(CONFIG_DEBUG_FS)) {
		/* debug - init pp base debugfs dir */
		priv->pp_dbgfs_dir = debugfs_create_dir("pp", NULL);
		if (unlikely(!priv->pp_dbgfs_dir)) {
			pr_err("Failed to create PP debugfs dir\n");
			ret = -ENOMEM;
			goto out;
		}
		pr_debug("pp debugfs directory created\n");

		priv->pp_hal_dbgfs_dir =
				debugfs_create_dir("hal", priv->pp_dbgfs_dir);
		if (unlikely(!priv->pp_hal_dbgfs_dir)) {
			pr_err("Failed to create PP hal debugfs dir\n");
			ret = -ENOMEM;
			goto out;
		}
		pr_debug("pp/hal debugfs directory created\n");
	}

	ret = __pp_dts_cfg_init(pdev);
	if (unlikely(ret)) {
		pr_err("Failed to fetch dts config, ret %d\n", ret);
		goto out;
	}

	__pp_dbg_init(priv);

	ret = init_clocks(priv);
	if (unlikely(ret)) {
		pr_err("Failed to init the clocks\n");
		goto out;
	}

	ret = pp_dma_init(pdev);
	if (unlikely(ret)) {
		pr_err("Failed to init pp_dma, ret %d\n", ret);
		goto out;
	}

	ret = __pp_hal_init(priv);
	if (unlikely(ret)) {
		pr_err("Failed to init pp hal drivers\n");
		goto out;
	}

	ret = __pp_api_init(&pdev->dev);
	if (unlikely(ret)) {
		pr_err("Failed to init pp api drivers\n");
		goto out;
	}

	/* init unit tests */
	if (IS_ENABLED(CONFIG_PPV4_UT) && IS_ENABLED(CONFIG_DEBUG_FS)) {
		ret = pput_init(priv->pp_dbgfs_dir);
		if (unlikely(ret)) {
			pr_err("Failed to init unit tests, ret %d\n", ret);
			goto out;
		}
	}

	if (likely(!ret))
		priv->init_done = true;

	return ret;

out:
	__pp_dev_remove(pdev);
	return ret;
}

/**
 * @brief Platform driver remove function, free all resources
 *        that was used by the driver
 * @note Doesn't change any HW configuration or registers
 *
 * @return int 0 on success
 */
static int __pp_dev_remove(struct platform_device *pdev)
{
	struct pp_dev_priv *priv = platform_get_drvdata(pdev);

	if (unlikely(ptr_is_null(priv)))
		return 0;

	priv->ready = false;

	/* clean other modules first */
	if (IS_ENABLED(CONFIG_PPV4_UT) && IS_ENABLED(CONFIG_DEBUG_FS))
		pput_exit();

	__pp_api_exit();
	__pp_hal_exit();
	pp_dma_exit(pdev);
	disable_clocks(priv);

	if (IS_ENABLED(CONFIG_DEBUG_FS))
		debugfs_remove_recursive(priv->pp_dbgfs_dir);

	kobject_put(priv->pp_sysfs_dir);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

/**
 * @brief PP driver late init call, mainly for initializing
 *        the UCs FWs
 * @note this is a late init routine of the PP driver and should
 *       be called by any driver!!!
 * @return s32 0 on success, error code otherwise
 */
s32 pp_dev_late_init(void)
{
	void *qdev;
	s32 ret;
	struct pp_dev_priv *priv;
#if IS_ENABLED(CONFIG_SOC_LGM)
	ulong en_pools[BITS_TO_LONGS(PP_BM_MAX_POOLS)] = { 0 };
	struct uc_init_params *uc_params;
	struct pool_info *pool;
	u8 id;
#endif

	if (unlikely(ptr_is_null(dev)))
		return -EINVAL;

	priv = dev_get_drvdata(dev);
	if (unlikely(ptr_is_null(priv)))
		return -EPERM;

	if (unlikely(!priv->init_done)) {
		pr_err("PP init is not done\n");
		return -EPERM;
	}

	if (unlikely(priv->ready)) {
		pr_err("PP init was already completed\n");
		return -EPERM;
	}

#if IS_ENABLED(CONFIG_SOC_LGM)
	/** load all pools addresses to map in UCs FAT table so UCs will be
	 *  able to access them via the right path (IOC/NIOC).
	 *  The UCs needs to access all possible buffers in the BMGR
	 *  due to the different network functions
	 */
	uc_params = &priv->dts_cfg.uc_params;
	bmgr_get_active_pools(en_pools, PP_BM_MAX_POOLS);

	for_each_set_bit(id, en_pools, PP_BM_MAX_POOLS) {
		pool = &uc_params->pools[id];
		ret = pp_bmgr_pool_base_get(id, NULL, &pool->addr, &pool->sz);
		if (ret)
			return ret;
	}
#endif

	/* init pp uc */
	ret = uc_init(&priv->dts_cfg.uc_params);
	if (unlikely(ret)) {
		pr_err("Failed to init uc module\n");
		return ret;
	}

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(!qdev)) {
		pr_err("failed to open qos dev %u\n", PP_QOS_INSTANCE_ID);
		return -EINVAL;
	}

	ret = qos_fw_init(qdev);
	if (unlikely(ret)) {
		pr_err("failed to init qos fw, ret %d\n", ret);
		return ret;
	}

	/* open the datapath */
	ret = rpb_start(&priv->dts_cfg.rpb_params);
	if (unlikely(ret)) {
		pr_err("Failed to start rpb\n");
		return ret;
	}

	/* Init synch queues */
	ret = smgr_sq_init(dev);
	if (unlikely(ret)) {
		pr_err("Failed to start synchronization queues\n");
		return ret;
	}

	/* finally, pp driver is ready */
	priv->ready = true;
	pr_info("PP is ready!\n");
	return 0;
}
EXPORT_SYMBOL(pp_dev_late_init);

/**
 * @brief Module init function
 * @return int 0 on success
 */
static int __pp_dev_init(void)
{
	int ret = 0;

	pr_debug("start\n");

	ret = platform_driver_register(&pp_plat_drv);
	if (unlikely(ret < 0)) {
		pr_err("Failed to register platform driver, ret = %u\n", ret);
		return ret;
	}

	pr_debug("done\n");
	return 0;
}

/**
 * @brief Modules attributes
 */
MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION(DRV_NAME);
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
#ifdef CONFIG_SOC_LGM
device_initcall(__pp_dev_init);
#else
arch_initcall(__pp_dev_init);
#endif
module_exit(__pp_dev_exit);
