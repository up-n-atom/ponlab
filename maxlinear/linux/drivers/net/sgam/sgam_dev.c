// SPDX-License-Identifier: GPL-2.0
/* Registering of sgam_drv char dev and initialisation of SGAM module
 * is done here.
 *
 * Copyright (C) 2022-2024 MaxLinear, Inc.
 */
#define pr_fmt(fmt) "[sgam:%s:%d] " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/err.h>

#include <net/sgam/sgam_api.h>
#include "sgam_core.h"

static dev_t sgam_dev;
static struct class *sgam_class;
static struct cdev sgam_cdev;

static int sgam_dev_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int sgam_dev_release(struct inode *inode, struct file *file)
{
	return 0;
}

static long sgam_dev_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	if (_IOC_TYPE(cmd) != SGAM_IOC_MAGIC) {
		pr_err("Magic number not maching!!\n");
		return -EIO;
	} else if (_IOC_NR(cmd) > SGAM_IOC_MAX_NR) {
		pr_err("IOCTL command number is > SGAM_IOC_MAX_NR !!\n");
		return -EIO;
	}

	return sgam_ioctl_handler(file, cmd, arg);
}

static const struct file_operations g_sgam_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = sgam_dev_ioctl,
	.open = sgam_dev_open,
	.release = sgam_dev_release
};

static int sgam_reg_char_dev(void)
{
	if ((alloc_chrdev_region(&sgam_dev, 0, 1, "sgam_device")) < 0) {
		pr_err("Cannot allocate major number\n");
		return -1;
	}
	pr_info("Major = %d Minor = %d\n", MAJOR(sgam_dev), MINOR(sgam_dev));

	/* init the sgam_cdev struct */
	cdev_init(&sgam_cdev, &g_sgam_dev_fops);

	/* Adding sgam char device to the system */
	if ((cdev_add(&sgam_cdev, sgam_dev, 1)) < 0) {
		pr_err("Cannot add sgam_cdev to the system\n");
		goto err_class;
	}

	/* Creating sgam class */
	sgam_class = class_create(THIS_MODULE, "sgam_class");
	if (IS_ERR(sgam_class)) {
		pr_err("Cannot create the sgam_class\n");
		goto err_class;
	}

	/* Creating device */
	if (IS_ERR(device_create(sgam_class, NULL, sgam_dev, NULL,
				 SGAM_DEV_NAME))) {
		pr_err("Cannot create the Device 1\n");
		goto err_dev;
	}
	pr_info("sgam_dev creation successful!\n");
	return 0;

err_dev:
	class_destroy(sgam_class);
err_class:
	unregister_chrdev_region(sgam_dev, 1);
	return -1;
}

int __init sgam_dev_init(void)
{
	if (unlikely(sgam_core_init() != SGAM_SUCCESS))
		pr_err("Core init failed!!\n");

	/* Register char device */
	if (sgam_reg_char_dev() != SGAM_SUCCESS) {
		pr_err("%s init failed!!\n", SGAM_DEV_NAME);
		return SGAM_FAILURE;
	}

	pr_info("%s init successful\n", SGAM_DEV_NAME);
	return SGAM_SUCCESS;
}

void __exit sgam_dev_exit(void)
{
	device_destroy(sgam_class, sgam_dev);
	class_destroy(sgam_class);
	cdev_del(&sgam_cdev);
	unregister_chrdev_region(sgam_dev, 1);
	if (sgam_core_exit() == SGAM_SUCCESS)
		pr_info("sgam_dev exited gracefully\n");
	else
		pr_err("sgam_dev did not exit gracefully!!\n");
}

module_init(sgam_dev_init);
module_exit(sgam_dev_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Main module of session based group accounting & metering");
MODULE_AUTHOR("Shashidhar Kattemane Venkatesh <skvenkatesh@maxlinear.com>");
