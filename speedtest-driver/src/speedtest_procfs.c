// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022 -2023 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * This file contains implementation procfs files registration
 * and de-registration
 */

#include <linux/string.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/version.h>

#include "speedtest_driver.h"
#include "speedtest_helpers.h"

#define PROC_ENTRY_MAX_SIZE			"max_size"
#define PROC_ENTRY_MAX_DURATION		"max_duration"
#define PROC_ENTRY_MAX_CONN			"max_conn"
#define PROC_ENTRY_BUFF_SIZE		"buffer_size"
#define PROC_ENTRY_PAGE_ORDER		"page_order"
#define PROC_MAX_BUF_SIZE			100
#define DEFAULT_PAGE_ORDER			2
#define MAX_PAGE_ORDER				10
#define DEFAULT_BUFFER_SIZE			(2 * MB)
#define MAX_BUFFER_SIZE				(50 * MB)

static int sptest_open(struct inode *inode, struct file *file)
{
	struct sptest_driver *driver = NULL;
	char *file_name = NULL;

	driver = PDE_DATA(inode);
	file_name = file->f_path.dentry->d_iname;
	if (!driver || !file_name) {
		LOG_ERROR("sptest_driver instance or file_name is null");
		return -EINVAL;
	}
	if (atomic_read(&driver->open_count)) {
		LOG_DEBUG("proc file is busy with other process");
		return -EPERM;
	}

	LOG_DEBUG("/proc/%s/%s file opening", (char*)driver->name, file_name);

	file->private_data = driver;
	atomic_inc(&driver->open_count);
	return SUCCESS;
}

static int sptest_release(struct inode *inode, struct file *file)
{
	struct sptest_driver *driver = NULL;
	char *file_name = NULL;

	driver = PDE_DATA(inode);
	file_name = file->f_path.dentry->d_iname;
	if (!driver || !file_name) {
		LOG_ERROR("sptest_driver instance or file_name is null");
		return -EINVAL;
	}

	LOG_DEBUG("/proc/%s/%s file is closing", (char*)driver->name, file_name);

	file->private_data = NULL;
	atomic_dec(&driver->open_count);
	return SUCCESS;
}

static ssize_t sptest_write(struct file *file, const char *ubuff, size_t count, loff_t *of)
{
	struct sptest_driver *driver = NULL;
	char buffer[PROC_MAX_BUF_SIZE] = {0};
	char *file_name = NULL;
	unsigned long value = 0;
	int ret = -1;

	driver = (struct sptest_driver*) file->private_data;
	file_name = file->f_path.dentry->d_iname;
	if (!driver || !file_name) {
		LOG_ERROR("sptest_driver instance or file_name is null");
		return -EINVAL;
	}

	LOG_DEBUG("/proc/%s/%s file opened for writing", (char*)driver->name, file_name);

	if ((*of > 0) || (count > PROC_MAX_BUF_SIZE))
		return 0;

	if (!capable(CAP_NET_ADMIN)){
		LOG_DEBUG("user do not have access to write");
		return -EACCES;
	}

	if (copy_from_user(buffer, ubuff, count)) {
		LOG_DEBUG("copy_from_user failed");
		return -EFAULT;
	}

	ret = kstrtoul(buffer, 10, &value);
	if (ret != 0) {
		LOG_ERROR("inavlid input, ret:%d", ret);
		return -EINVAL;
	}
	if (value <= 0) {
		LOG_ERROR("inavlid input, shouldn't be value < 1");
		return -EINVAL;
	}

	if (!strcmp(file_name, PROC_ENTRY_MAX_SIZE))
		driver->proc_limits.max_size = (uint64_t)value;
	else if (!strcmp(file_name, PROC_ENTRY_MAX_DURATION))
		driver->proc_limits.max_duration = (uint64_t)value;
	else if (!strcmp(file_name, PROC_ENTRY_MAX_CONN))
		driver->proc_limits.max_conn = (uint16_t)value;
	else if (!strcmp(file_name, PROC_ENTRY_BUFF_SIZE)) {
		if ((value > MAX_BUFFER_SIZE) || (value < (512 * KB))) {
			LOG_ERROR("buffer_size range => 512Kb < buffer_size < 50Mb");
			return -EINVAL;
		}
		driver->proc_limits.buffer_size = value;
	} else if (!strcmp(file_name, PROC_ENTRY_PAGE_ORDER)) {
		if((value > MAX_PAGE_ORDER) || (value < 0)) {
			LOG_ERROR("page order range => 0 < page_order < 10");
			return -EINVAL;
		}
		driver->proc_limits.page_order = (uint16_t)value;
	} else {
		LOG_DEBUG("%s file is not supported", file_name);
		return -EINVAL;
	}

	*of = count;
	return count;
}

static ssize_t sptest_read(struct file *file, char *ubuff, size_t count, loff_t *of)
{
	struct sptest_driver *driver = NULL;
	char *file_name = NULL;
	char buffer[PROC_MAX_BUF_SIZE] = {0};
	int len = 0;

	driver = (struct sptest_driver*) file->private_data;
	file_name = file->f_path.dentry->d_iname;

	if (!driver || !file_name) {
		LOG_ERROR("parameters invalid");
		return -EINVAL;
	}
	LOG_DEBUG("/proc/%s/%s opened for reading", (char*)driver->name, file_name);

	if ((*of > 0) || (count < PROC_MAX_BUF_SIZE))
		return len;

	if (!strcmp(file_name, PROC_ENTRY_MAX_SIZE))
		len += sprintf(buffer, " max_size = %llu \n ", driver->proc_limits.max_size);
	else if (!strcmp(file_name, PROC_ENTRY_MAX_DURATION))
		len += sprintf(buffer, " max_duration = %llu \n ", driver->proc_limits.max_duration);
	else if (!strcmp(file_name, PROC_ENTRY_MAX_CONN))
		len += sprintf(buffer, " max_conn = %d\n ", driver->proc_limits.max_conn);
	else if (!strcmp(file_name, PROC_ENTRY_BUFF_SIZE))
		len += sprintf(buffer, " buffer_size = %llu\n ", driver->proc_limits.buffer_size);
	else if (!strcmp(file_name, PROC_ENTRY_PAGE_ORDER))
		len += sprintf(buffer, " page_order = %d\n ", driver->proc_limits.page_order);
	else {
		LOG_DEBUG("%s file is not supported", file_name);
		return -EINVAL;
	}

	if (copy_to_user(ubuff, buffer, len)) {
		LOG_DEBUG("copy_to_user failed");
		return -EFAULT;
	}

	*of = len;
	return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops sptest_fops = {
	.proc_open = sptest_open,
	.proc_release = sptest_release,
	.proc_write = sptest_write,
	.proc_read = sptest_read,
};
#else
static const struct file_operations sptest_fops = {
	.open = sptest_open,
	.release = sptest_release,
	.write = sptest_write,
	.read = sptest_read,
};
#endif

static void sptest_init_proc_limits(struct sptest_proc_limits *limits)
{
	limits->max_duration = TR143_MAX_DEFAULT_DURATION;
	limits->max_conn = TR143_MAX_CONN;
	limits->max_size = TR143_MAX_DEFAULT_SIZE;
	limits->buffer_size = DEFAULT_BUFFER_SIZE;
	limits->page_order = DEFAULT_PAGE_ORDER;
}

int sptest_create_procfs(struct sptest_driver *driver)
{
	struct proc_dir_entry *dir_entry = NULL;
	int ret = SUCCESS;

	sptest_init_proc_limits(&driver->proc_limits);

	dir_entry = proc_mkdir(MODULE_NAME, NULL);
	if (!dir_entry) {
		LOG_ERROR("proc_mkdir() failed");
		return -ENOMEM;
	}
	driver->parent = dir_entry;

	if (!proc_create_data(PROC_ENTRY_MAX_SIZE, 0644, dir_entry, &sptest_fops, driver)) {
		LOG_ERROR("can't create max_size entry");
		ret = -ENOMEM;
		goto remove;
	}

	if (!proc_create_data(PROC_ENTRY_MAX_DURATION, 0644, dir_entry, &sptest_fops, driver)) {
		LOG_ERROR("can't create max_duration entry");
		ret = -ENOMEM;
		goto max_size_remove;
	}

	if (!proc_create_data(PROC_ENTRY_MAX_CONN, 0644, dir_entry, &sptest_fops, driver)) {
		LOG_ERROR("can't create max_conn entry");
		ret = -ENOMEM;
		goto max_duration_remove;
	}
	if (!proc_create_data(PROC_ENTRY_BUFF_SIZE, 0644, dir_entry, &sptest_fops, driver)) {
		LOG_ERROR("can't create buffer_size entry");
		ret = -ENOMEM;
		goto max_conn_remove;
	}
	if (!proc_create_data(PROC_ENTRY_PAGE_ORDER, 0644, dir_entry, &sptest_fops, driver)) {
		LOG_ERROR("can't create page_order entry");
		ret = -ENOMEM;
		goto buff_size_remove;
	}
	LOG_INFO("Created procfs files successfully");
	return ret;

buff_size_remove:
	remove_proc_entry(PROC_ENTRY_BUFF_SIZE, dir_entry);
max_conn_remove:
	remove_proc_entry(PROC_ENTRY_MAX_CONN, dir_entry);
max_duration_remove:
	remove_proc_entry(PROC_ENTRY_MAX_DURATION, dir_entry);
max_size_remove:
	remove_proc_entry(PROC_ENTRY_MAX_SIZE, dir_entry);
remove:
	remove_proc_entry(MODULE_NAME, NULL);
	return ret;
}

void sptest_remove_procfs(struct sptest_driver *driver)
{
	if (!driver)
		return;
	remove_proc_entry(PROC_ENTRY_MAX_SIZE, driver->parent);
	remove_proc_entry(PROC_ENTRY_MAX_DURATION, driver->parent);
	remove_proc_entry(PROC_ENTRY_MAX_CONN, driver->parent);
	remove_proc_entry(PROC_ENTRY_BUFF_SIZE, driver->parent);
	remove_proc_entry(PROC_ENTRY_PAGE_ORDER, driver->parent);
	driver->parent = NULL;

	remove_proc_entry(MODULE_NAME, NULL);
	LOG_INFO("Removed procfs files successfully");
	return;
}
