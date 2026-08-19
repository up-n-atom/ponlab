// SPDX-License-Identifier: GPL-2.0
/* This provides IOCTL handlers and API to xt_GROUP to get acct/meter info from
 * the name of the acct/meter.
 *
 * Copyright (C) 2022-2024 MaxLinear, Inc.
 */
#define pr_fmt(fmt) "[sgam:%s:%d] " fmt, __func__, __LINE__

#include <linux/ioctl.h>
#include <linux/spinlock_types.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <net/sgam/sgam_api.h>

#include "sgam_db.h"
#include "sgam_core.h"

/* This is used by xt_GROUP.ko, hence exported */
int sgam_xt_get_id_from_name(struct xt_group_info *info)
{
	int ret = SGAM_SUCCESS;

	if (IS_ACCT_DB(info->flags)) {
		ret = sgam_xt_get_sgc_info(info->name, &info->id, &info->grp);
	} else if (IS_METER_DB(info->flags)) {
		ret = sgam_xt_get_tbm_info(info->name, info);
	} else {
		pr_err("Invalid DB!!\n");
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(sgam_xt_get_id_from_name);

int sgam_xt_update_db(struct xt_group_info *info)
{
	int ret = SGAM_SUCCESS;

	if (IS_ACCT_DB(info->flags)) {
		ret = sgam_xt_update_sgc_info(info);
	} else if (IS_METER_DB(info->flags)) {
		ret = sgam_xt_update_tbm_info(info);
	} else {
		pr_err("Invalid DB!!\n");
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(sgam_xt_update_db);

static int is_read_ok(unsigned int cmd, unsigned long arg)
{
	if ((_IOC_DIR(cmd) & _IOC_READ) && !access_ok(arg, _IOC_SIZE(cmd))) {
		pr_err("read access check: (%08X && %d)\n",
		       (_IOC_DIR(cmd) & _IOC_READ),
		       (int)!access_ok(arg, _IOC_SIZE(cmd)));
		return -EACCES;
	}

	return SGAM_SUCCESS;
}

static int is_write_ok(unsigned int cmd, unsigned long arg)
{
	if ((_IOC_DIR(cmd) & _IOC_WRITE) && !access_ok(arg, _IOC_SIZE(cmd))) {
		pr_err("write access check: (%08X && %d)\n",
		       (_IOC_DIR(cmd) & _IOC_WRITE),
		       (int)!access_ok(arg, _IOC_SIZE(cmd)));
		return -EACCES;
	}

	return SGAM_SUCCESS;
}

static int is_rw_access_ok(unsigned int cmd, unsigned long arg)
{
	if ((is_read_ok(cmd, arg) != SGAM_SUCCESS) ||
	    (is_write_ok(cmd, arg) != SGAM_SUCCESS))
		return -EACCES; /* Access is not ok */
	else
		return SGAM_SUCCESS; /* Access is ok */
}

static int list_cmd_handler(unsigned long arg)
{
	int list_cnt, size, ret = SGAM_SUCCESS;
	struct sgam_list_info *list_info = NULL;
	void *bptr = (void *)arg;

	list_cnt = ((struct sgam_list_info *)arg)->count_info.count;

	/* Allocate memory for list_info */
	size = sizeof(struct sgam_list_info) +
			(list_cnt + 1) * sizeof(struct sgam_name_list);
	list_info = (struct sgam_list_info *)sgam_malloc(size);
	if (!list_info) {
		pr_err("sgam_malloc error!\n");
		return -ENOMEM;
	}

	if (copy_from_user(&list_info->count_info, (void *)arg,
			   sizeof(struct sgam_count_info)) != 0) {
		pr_err("Failed to copy from user!\n");
		sgam_free((void *)&list_info);
		return -EFAULT;
	}

	/* Get the name list based on the database flag */
	if (IS_ACCT_DB(list_info->count_info.db_flag)) {
		ret = sgam_get_acct_name_list(list_info);
	} else if (IS_METER_DB(list_info->count_info.db_flag)) {
		ret = sgam_get_meter_name_list(list_info);
	} else {
		pr_err("Wrong db flag!!\n");
		ret = -EINVAL;
	}

	if (ret == SGAM_SUCCESS) {
		/* Copy the entire list_info structure back to user space */
		ret = copy_to_user(bptr, list_info, size);
		if (ret != 0) {
			pr_err("Failed to copy list_info to user!\n");
			ret = -EFAULT;
		}
	} else {
		pr_err("Failed to get the name list!\n");
		ret = -EFAULT;
	}

	sgam_free((void *)&list_info);

	return ret;
}

static int get_count_cmd_handler(unsigned long arg)
{
	int ret = SGAM_SUCCESS;
	struct sgam_count_info count_info = { 0 };

	if (copy_from_user(&count_info, (void *)arg,
			   sizeof(struct sgam_count_info)) != 0) {
		pr_err("copy from user failed!!\n");
		return -EFAULT;
	}

	if (IS_ACCT_DB(count_info.db_flag)) {
		sgam_get_acct_count(&count_info.count);
	} else if (IS_METER_DB(count_info.db_flag)) {
		sgam_get_meter_count(&count_info.count);
	} else { /* Unlikely */
		pr_err("Wrong DB flag!\n");
		return -EINVAL;
	}

	if (copy_to_user((struct sgam_count_info *)arg, &count_info,
			 sizeof(struct sgam_count_info))) {
		pr_err("Failed to copy count info to user!\n");
		ret = -EFAULT;
	}

	return ret;
}

static int del_cmd_handler(unsigned long arg)
{
	int ret = SGAM_SUCCESS;
	struct sgam_del_info del_info = { 0 };

	if (copy_from_user(&del_info, (void *)arg,
			   sizeof(struct sgam_del_info)) != 0) {
		pr_err("copy from user failed!!\n");
		return -EFAULT;
	}
	/* IOCTL uses only the account name, set sgc_id and sgc_grp to invalid */
	del_info.sgc_id = PP_SGC_INVALID;
	del_info.sgc_grp = -1;

	if (IS_ACCT_DB(del_info.db_flag)) {
		ret = sgam_del_account_entry(&del_info);
	} else if (IS_METER_DB(del_info.db_flag)) {
		ret = sgam_del_meter_entry(&del_info);
	} else {
		pr_err("Invalid db flag!\n");
		ret = -EINVAL;
	}

	if (copy_to_user((struct sgam_del_info *)arg, &del_info,
			 sizeof(struct sgam_del_info))) {
		pr_err("Failed to copy meter info to user!\n");
		ret = -EFAULT;
	}

	return ret;
}

static int add_account_handler(unsigned long arg)
{
	int ret = SGAM_SUCCESS;
	struct sgam_acct_info acct_info = { 0 };

	if (copy_from_user(&acct_info, (void *)arg,
			   sizeof(struct sgam_acct_info)) != 0) {
		pr_err("copy from user failed!!\n");
		return -EFAULT;
	}

	ret = sgam_add_new_account(&acct_info);

	return ret;
}

static int add_meter_handler(unsigned long arg)
{
	int ret = SGAM_SUCCESS;
	struct sgam_meter_info meter_info = { 0 };

	if (copy_from_user(&meter_info, (void *)arg,
			   sizeof(struct sgam_meter_info)) != 0) {
		pr_err("copy from user failed!!\n");
		return -EFAULT;
	}

	ret = sgam_add_new_meter(&meter_info);

	return ret;
}

static int show_meter_handler(unsigned long arg)
{
	int ret = -EINVAL;
	struct sgam_meter_info meter_info = { 0 };

	if (copy_from_user(&meter_info, (void *)arg,
			   sizeof(struct sgam_meter_info)) != 0) {
		pr_err("copy from user failed!!\n");
		return -EFAULT;
	}

	ret = sgam_get_meter_info(&meter_info);
	if (ret == SGAM_SUCCESS) {
		if (copy_to_user((struct sgam_meter_info *)arg, &meter_info,
				 sizeof(struct sgam_meter_info))) {
			pr_err("Failed to copy meter info to user!\n");
			ret = -EFAULT;
		}
	}

	return ret;
}

static int show_account_handler(unsigned long arg)
{
	int ret = -EINVAL;
	struct sgam_cmd_acct_stats stats_info = { 0 };

	if (copy_from_user(&stats_info, (void *)arg,
			   sizeof(struct sgam_cmd_acct_stats)) != 0) {
		pr_err("copy from user failed!!\n");
		return -EFAULT;
	}
	/* IOCTL uses only the account name, set sgc_id and sgc_grp to invalid */
	stats_info.sgc_id = PP_SGC_INVALID;
	stats_info.sgc_grp = -1;

	ret = sgam_get_account_stats(&stats_info);
	if (ret == SGAM_SUCCESS) {
		if (copy_to_user((struct sgam_cmd_acct_stats *)arg, &stats_info,
				 sizeof(struct sgam_cmd_acct_stats))) {
			pr_err("Failed to copy account info to user!!\n");
			ret = -EFAULT;
		}
	} else {
		ret = -EINVAL;
	}

	return ret;
}

long sgam_ioctl_handler(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = SGAM_SUCCESS;

	if (_IOC_TYPE(cmd) != SGAM_IOC_MAGIC) {
		pr_err("_IOC_TYPE(%08X - %d) != SGAM_IOC_MAGIC(%d)\n", cmd,
		       _IOC_TYPE(cmd), SGAM_IOC_MAGIC);
		return -EIO;
	} else if (_IOC_NR(cmd) >= SGAM_IOC_MAX_NR) {
		pr_err("Current cmd is %02x wrong, it should be < %02x\n",
		       _IOC_NR(cmd), SGAM_IOC_MAX_NR);
		return -EIO;
	}

	if (is_rw_access_ok(cmd, arg) != SGAM_SUCCESS) {
		pr_err("Access error!!\n");
		return -EACCES;
	}

	if (!capable(CAP_NET_ADMIN)) {
		pr_err("!CAP_NET_ADMIN\n");
		return -EACCES;
	}

	switch (cmd) {
	case SGAM_LIST_CMD:
		ret = list_cmd_handler(arg);
		break;
	case SGAM_GET_COUNT_CMD:
		ret = get_count_cmd_handler(arg);
		break;
	case SGAM_DEL_CMD:
		ret = del_cmd_handler(arg);
		break;
	case SGAM_ADD_ACCOUNT:
		ret = add_account_handler(arg);
		break;
	case SGAM_ADD_METER:
		ret = add_meter_handler(arg);
		break;
	case SGAM_SHOW_ACCOUNT:
		ret = show_account_handler(arg);
		break;
	case SGAM_SHOW_METER:
		ret = show_meter_handler(arg);
		break;
	default:
		pr_err("Invalid IOCTL command!!\n");
		ret = -ENOIOCTLCMD;
	}

	return ret;
}

int sgam_core_init(void)
{
	return sgam_init_db();
}

int sgam_core_exit(void)
{
	return sgam_exit_db();
}
