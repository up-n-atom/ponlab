/*******************************************************************************

  Copyright (C) 2020-2022  MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/
/*  Update Header */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/random.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <linux/stat.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include "drv_icc.h"
#include "drv_mps.h"
#include "secure_services.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MaxLinear Inc");
MODULE_DESCRIPTION("Secure Service Driver");

extern struct ssd_client ssd_clients[];
static short ss_major_id = 0;
static struct cdev *ss_cdev = NULL;
static dev_t dev_no;
static struct class *ss_class = NULL;

static long sse_ioctl(struct file *file_p, uint32_t nCmd, unsigned long arg);
static unsigned int sse_poll(struct file *fp, poll_table * wait);
static int sse_open(struct inode *inode, struct file *filp);
static ssize_t sse_read_d(struct file *fp, char *buffer, size_t count,
				loff_t * position);
static long (*sse_client_ioctl_handler[ICC_MAX_MINOR_NR])(struct file *, unsigned int, unsigned long);
static void destroyDevices(void);

static const struct file_operations ss_fops = {
	.open = sse_open,
	.owner = THIS_MODULE,
	.unlocked_ioctl = sse_ioctl,
	.poll = sse_poll,
	.read = sse_read_d
};

#ifdef SSE_DEBUG
/**
  ====================================================================================================
 * @brief Hexdump function
 ====================================================================================================
 */
void hexdump(unsigned char *buf, unsigned int len) {
	while (len--)
		printk(KERN_CONT "%02x", *buf++);
}
#endif

void dump_ss_icc_msg(icc_msg_t *dump_msg)
{
	int i = 0;

	if (!dump_msg)
		return;

	pr_info("Source Client ID %d\n", dump_msg->src_client_id);
	pr_info("Dest CLient ID %d\n", dump_msg->dst_client_id);
	pr_info("Message ID %x\n", dump_msg->msg_id);
	pr_info("Attributes %d\n", dump_msg->param_attr);
	for (i = 0; i < MAX_UPSTRM_DATAWORDS; i++)
		pr_info("params @%x value %x\n", i, dump_msg->param[i]);

	return;
}

static int sse_dev_init(int minor_id)
{
	int ret = 0;

	switch (minor_id) {
	case ICC_SEC_STG_ADMIN_NR:
		ret = sse_sec_storage_client_init();
		if (!ret)
			sse_client_ioctl_handler[ICC_SEC_STG_ADMIN_NR] = sse_sec_storage_client_ioctl;
		break;
	case ICC_SEC_STG_NORMAL_NR:
		sse_client_ioctl_handler[ICC_SEC_STG_NORMAL_NR] = sse_sec_storage_client_ioctl;
		break;
	case ICC_SEC_DBG_ADMIN_NR:
		ret = secure_debug_client_init();
		if (!ret)
			sse_client_ioctl_handler[ICC_SEC_DBG_ADMIN_NR] = sse_sec_dbg_client_ioctl;
		break;
	case ICC_SEC_UPGRADE_NR:
		ret = sse_img_auth_init();
		if (!ret)
			sse_client_ioctl_handler[ICC_SEC_UPGRADE_NR] = sse_img_auth_ioctl;
		break;
	default:
		pr_err("device with %d minor number is not registered\n", minor_id);
		ret = -ENODEV;
	}

	return ret;
}
/**
  ====================================================================================================
 * @brief open function registered with the driver
 ====================================================================================================
 */
static int sse_open(struct inode *inode, struct file *filp)
{
	int devMinor = MINOR(inode->i_rdev);
	if(devMinor > ICC_MAX_MINOR_NR) {
		return -ENODEV;
	}
	filp->private_data = &ssd_clients[devMinor];
	return 0;
	
}

static ssize_t sse_read_d(struct file *file_p, char *buf, size_t count, loff_t *ppos){
	return -1;
}

/**
  ====================================================================================================
 * @brief poll function registered with the driver
 ====================================================================================================
 */
static unsigned int sse_poll(struct file *fp, poll_table *wait){
	int ret = -1;
	/* Get the minor number form the inode info and direct the request to the approppriate service */
	int devMinor = MINOR(fp->f_inode->i_rdev);
	if (devMinor > ICC_MAX_MINOR_NR) {
		return -ENODEV;
	}

	if (devMinor == ICC_SEC_UPGRADE_NR)
		return sse_img_auth_poll(fp, wait);

	return ret;
}


/**=================================================================================================
 * @brief  Ioctls functions supported by Secure Service Driver
 *
 * @param fd    
 * file descriptor passed by the application.
 *
 * @param cmd
 * control commands or switch statements
 * 
 * @param arg
 * data passed by the application
 ====================================================================================================
 */
static long sse_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	long ret = -1;

	int devMinor = MINOR(fp->f_inode->i_rdev);

	if (devMinor > ICC_MAX_MINOR_NR)
		return -ENODEV;

	/* Route the ioctl commands to respective modules */
	if (sse_client_ioctl_handler[devMinor])
		ret = sse_client_ioctl_handler[devMinor](fp, cmd, arg);

	return ret;
}


static int __init ss_init_module(void)
{
	/*register the char region with the linux*/
	/* cat /proc/devices */
	int ret=0, nCnt=0;
	int dev_cnt = 0;
	struct device *dev = NULL;

	ss_class = class_create(THIS_MODULE, SS_DEV_NAME);
	if (IS_ERR(ss_class)) {
		pr_err("Unable to create class");
		return PTR_ERR(ss_class);
	}

	ret = alloc_chrdev_region(&dev_no, 0, ICC_MAX_MINOR_NR, SS_DEV_NAME);
	if (ret < 0) {
		pr_err("Major number allocation failed\n");
		goto out_ss_class;
	}

	ss_major_id = MAJOR(dev_no);
	ss_cdev = cdev_alloc();
	if (ss_cdev == NULL) {
		ret= -ENOMEM;
		goto unregister_ss_dev;
	}

	ss_cdev->ops = &ss_fops;
	ss_cdev->owner = THIS_MODULE;

	ret = cdev_add(ss_cdev, dev_no, ICC_MAX_MINOR_NR);
	if (ret < 0)
		goto free_ss_dev;


	for (nCnt = 1; nCnt < ICC_MAX_MINOR_NR; nCnt++) {
		if (ssd_clients[nCnt].name != NULL && ssd_clients[nCnt].ssd == 1) {
			dev = device_create(ss_class, NULL, MKDEV(ss_major_id, nCnt), NULL, ssd_clients[nCnt].name);
			if (IS_ERR(dev)) {
				pr_err("Unable to create device node for %s\n",ssd_clients[nCnt].name);
			} else {
				ret = sse_dev_init(nCnt);
				if (ret)
					device_destroy(ss_class,  MKDEV(ss_major_id, nCnt));
				else
					dev_cnt++;
			}
		}
	}

	/*If not a  device is registered */
	if (!dev_cnt)
		goto fail;

	return 0;

fail:
	destroyDevices();
free_ss_dev:
	cdev_del(ss_cdev);
unregister_ss_dev:
	unregister_chrdev_region(dev_no, ICC_MAX_MINOR_NR);
out_ss_class:
	class_destroy(ss_class);

	return -ENODEV;
}

static void destroyDevices(void) {
	int nCnt;

	for (nCnt = 1; nCnt < ICC_MAX_MINOR_NR; nCnt++) {
		if (ssd_clients[nCnt].name != NULL && ssd_clients[nCnt].ssd == 1) {
			 pr_debug("destroying device: %d\r\n", nCnt);
			device_destroy(ss_class,  MKDEV(ss_major_id, nCnt));
			

		}
	}
}

static void __exit ss_cleanup_module (void){
	sse_img_auth_cleanup();
	sse_sec_storage_client_cleanup();
	secure_debug_client_cleanup();
	destroyDevices();
	cdev_del(ss_cdev);
	class_destroy(ss_class);

	if (ss_major_id != 0)
		unregister_chrdev_region(dev_no, 1);
}

module_init(ss_init_module);
module_exit(ss_cleanup_module);
