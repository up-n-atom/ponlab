// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022 - 2024 MaxLinear, Inc.
 *
 *****************************************************************************/

/*!
 * This file contains driver registration, initialization and maintains
 * list of kernel threads in terms of sptest_task
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/tcp.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <net/sock.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
	#include <uapi/linux/sched/types.h>
#endif

#include "speedtest_driver.h"
#include "speedtest_data_init.h"
#include "speedtest_http.h"
#include "speedtest_helpers.h"

/* Flag not defined for kernel version < 4.16 */
#ifndef SCHED_FLAG_ALL
#ifndef SCHED_FLAG_RECLAIM
#define SCHED_FLAG_RECLAIM	0x02
#endif
#ifndef SCHED_FLAG_DL_OVERRUN
#define SCHED_FLAG_DL_OVERRUN	0x04
#endif
#define SCHED_FLAG_ALL	(SCHED_FLAG_RESET_ON_FORK	| \
			 SCHED_FLAG_RECLAIM		| \
			 SCHED_FLAG_DL_OVERRUN)
#endif

static struct list_head g_task_list = {NULL, NULL};

static struct sptest_driver driver;

/*! function to get cpu from round-robin manner
 */
static int sptest_get_cpu(int prev_cpu)
{
	uint32_t active_cpus_bitmap = 0x0;
	int next_cpu = prev_cpu + 1;
	int cpu_index = 0;

	for (; cpu_index < MAX_CPUS; ++cpu_index) {
		if (cpu_online(cpu_index)) {
			active_cpus_bitmap |=  (1 << cpu_index);
		}
	}

	if (active_cpus_bitmap == 0) {
		return FAILURE;
	}
	if (next_cpu >= MAX_CPUS) {
		next_cpu = 0;
	}
	while (!((active_cpus_bitmap >> next_cpu) & 1)) {
		if (next_cpu >= (MAX_CPUS - 1)) {
			next_cpu = 0;
		}
		++ next_cpu;
	}
	return next_cpu;
}

struct list_head *sptest_get_task_list(void)
{
	if (g_task_list.next == NULL || g_task_list.prev == NULL) {
		return NULL;
	}
	return &g_task_list;
}

struct sptest_task *sptest_get_task_by_id(struct list_head *task_list, int task_id)
{
	struct sptest_task *sp_task = NULL;

	if ((task_id < 0) && (task_id > SPTEST_TASKS_MAX)) {
		LOG_DEBUG("invalid task id:%d", task_id);
		return NULL;
	}

	list_for_each_entry(sp_task, task_list, list_node) {
		if (sp_task->id == task_id) {
			return sp_task;
		}
	}
	return NULL;
}

void sptest_display_task(struct sptest_task *sp_task)
{
	struct sptest_task_config *config = sp_task->config;

	LOG_INFO("sptest_task[%d]-----------------", sp_task->id);
	LOG_INFO("id         : %d", sp_task->id);
	LOG_INFO("state      : %s", sptest_task_state_to_str(sp_task->state));
	LOG_INFO("cpu_affinity : %d", sp_task->cpu_affinity);
	LOG_INFO("priority  : %d", sp_task->priority);
	LOG_INFO("policy    : %s", (sp_task->sched_policy == SCHED_RR) ? "SCHED_RR" : "SCHED_FIFO");
	LOG_INFO("sptest_task_config ------>>>>>>>");
	LOG_INFO("  page_order        : %d", config->page_order);
	LOG_INFO("  buffer_size       : %llu", config->buffer_size);
	LOG_INFO("  sock_options      : %d", config->sock_options);
	LOG_INFO("  non-block         : %d", config->non_block);
	LOG_INFO("  protocol          : %s", config->protocol ? "not specified" : "HTTP");
	return;
}

int sptest_display_task_by_id(struct list_head *task_list, int task_id)
{
	struct sptest_task *sp_task = NULL;

	if (!task_list) {
		LOG_DEBUG("task_list parameter is NULL");
		return -EINVAL;
	}

	if ((task_id < 0) || (task_id > SPTEST_TASKS_MAX)) {
		LOG_DEBUG("invalid task id:%d", task_id);
		return -EINVAL;
	}

	sp_task = sptest_get_task_by_id(task_list, task_id);
	if (sp_task) {
		sptest_display_task(sp_task);
	} else {
		LOG_DEBUG("sptest_task[%d] doesn't exists", sp_task->id);
	}
	return SUCCESS;
}

void sptest_display_task_list(struct list_head *task_list)
{
	struct sptest_task *sp_task = NULL;

	if (!task_list) {
		return;
	}

	list_for_each_entry(sp_task, task_list, list_node) {
		sptest_display_task(sp_task);
	}
	return;
}

static int sptest_create_task_struct_and_run(struct sptest_task *sp_task)
{
	struct sptest_task_config *config = NULL;
	struct task_struct *task = NULL;
	struct sched_attr attr = {0};

	if (!sp_task) {
		LOG_DEBUG("sptest_task is null");
		return -EINVAL;
	}

	config = sp_task->config;
	if (!config) {
		LOG_DEBUG("sptest_task_config is null for sptest_task[%d] task", sp_task->id);
		return -EINVAL;
	}

	config->id = sp_task->id;
	init_completion(&config->notify_state);
	task = kthread_create(sptest_http_client_kthread, (void*)config,
						"sptest_task[%d]", sp_task->id);
	if (!task) {
		LOG_ERROR("sptest_task[%d] kthread creation failed", sp_task->id);
		return -ENOMEM;
	}

	get_task_struct(task);
	sp_task->task = task;

	/* Binding task to particular CPU */
	kthread_bind(task, sp_task->cpu_affinity);

	/* Setting RT priority */
	attr.size = sizeof(attr);
	attr.sched_flags = SCHED_FLAG_ALL;
	attr.sched_nice = task_nice(task);
	attr.sched_priority = sp_task->priority;
	attr.sched_policy = sp_task->sched_policy;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,14,0)
	sched_setattr_nocheck(task, &attr);
#else
	sched_setattr(task, &attr);
#endif

	if (wake_up_process(sp_task->task))
		sp_task->state = SPTEST_TASK_STATE_RUNNING;
	else
		return -EBUSY;

	return SUCCESS;
}

int sptest_start_task_by_id(struct list_head *task_list, int task_id)
{
	struct sptest_task *sp_task = NULL;
	int ret = 0;

	if ((task_id < 0) || (task_id > SPTEST_TASKS_MAX)) {
		LOG_ERROR("invalid task id:%d", task_id);
		return -EINVAL;
	}

	sp_task = sptest_get_task_by_id(task_list, task_id);
	if (sp_task) {
		if ((sp_task->state == SPTEST_TASK_STATE_CREATED) ||
				(sp_task->state == SPTEST_TASK_STATE_CLOSED)) {
			ret = sptest_create_task_struct_and_run(sp_task);
			if ((ret == SUCCESS) || (ret == -EBUSY)) {
				LOG_DEBUG("sptest_task[%d] is running", sp_task->id);
			}
		} else {
			LOG_DEBUG("sptest_task[%d] is already running", sp_task->id);
			ret = -EBUSY;
		}
	} else {
		LOG_DEBUG("sptest_task[%d] doesn't exists", task_id);
		ret = FAILURE;
	}
	return ret;
}

int sptest_start_task_list(struct list_head *task_list)
{
	struct sptest_task *sp_task = NULL;
	int ret = SUCCESS;

	if (!task_list) {
		LOG_ERROR("task_list parameter is NULL");
		return -EINVAL;
	}

	list_for_each_entry(sp_task, task_list, list_node) {
		if ((sp_task->state == SPTEST_TASK_STATE_CREATED) ||
				(sp_task->state == SPTEST_TASK_STATE_CLOSED)) {
			ret = sptest_create_task_struct_and_run(sp_task);
			if ((ret < 0) && (ret != -EBUSY)) {
				goto end;
			}
			LOG_DEBUG("sptest_task[%d] kthread is running", sp_task->id);
		} else {
			LOG_DEBUG("sptest_task[%d] is already running or state is not cleared", sp_task->id);
		}
	}
	return ret;

end:
	sptest_stop_task(sp_task);
	return ret;
}

int sptest_stop_task(struct sptest_task *sp_task)
{
	if (!sp_task) {
		return -EINVAL;
	}

	if (sp_task->state == SPTEST_TASK_STATE_RUNNING) {
		complete_all(&sp_task->config->notify_state);
		if (kthread_stop(sp_task->task) != SUCCESS) {
			LOG_DEBUG("failed to stop kthread id:%d", sp_task->id);
		}
		put_task_struct(sp_task->task);
		if (sp_task->config->data) {
			kfree(sp_task->config->data);
			sp_task->config->data = NULL;
		}
		sp_task->state = SPTEST_TASK_STATE_CLOSED;
		return SUCCESS;
	}
	return FAILURE;
}

int sptest_stop_task_list(struct list_head *task_list)
{
	struct sptest_task *sp_task = NULL;
	int ret = SUCCESS;

	if (!task_list) {
		LOG_ERROR("task_list parameter is NULL");
		return -EINVAL;
	}

	list_for_each_entry(sp_task, task_list, list_node) {
		if (sp_task->state == SPTEST_TASK_STATE_RUNNING) {
			ret = sptest_stop_task(sp_task);
			if (ret < 0) {
				goto end;
			}
			LOG_DEBUG("sptest_task[%d] kthread is stopped", sp_task->id);
		} else {
			LOG_DEBUG("sptest_task[%d] is already stopped", sp_task->id);
		}
	}
end:
	return ret;
}

int sptest_stop_task_by_id(struct list_head *task_list, int task_id)
{
	struct sptest_task *sp_task = NULL;

	if ((task_id < 0) || (task_id > SPTEST_TASKS_MAX)) {
		LOG_ERROR("invalid task id:%d", task_id);
		return FAILURE;
	}

	sp_task = sptest_get_task_by_id(task_list, task_id);
	if (sp_task) {
		if (sptest_stop_task(sp_task) == SUCCESS) {
			LOG_DEBUG("sptest_task[%d] stopped", sp_task->id);
		}
	} else {
		LOG_DEBUG("sptest_task[%d] doesn't exists", sp_task->id);
	}
	return SUCCESS;
}

void sptest_clear_task_list(struct list_head *task_list)
{
	struct sptest_task *sp_task = NULL;
	struct list_head *iter = NULL;
	struct list_head *safe_iter = NULL;

	if (!task_list) {
		return;
	}

	list_for_each_safe(iter, safe_iter, task_list) {
		sp_task = list_entry(iter, struct sptest_task, list_node);
		if (sp_task->state == SPTEST_TASK_STATE_RUNNING) {
			sptest_stop_task(sp_task);
		}
		list_del(iter);
		if (sp_task->config->data) {
			kfree(sp_task->config->data);
			sp_task->config->data = NULL;
		}
		kfree(sp_task->config);
		kfree(sp_task);
	}
	return;
}

static void sptest_update_task_config(struct sptest_task_config *prev,
		struct sptest_task_config *new)
{
	/*! update sptest_task_config structure
	 * Extra checks are for single parameter update, which is for future */
	prev->page_order = (new->page_order != -1) ? new->page_order : prev->page_order;
	prev->buffer_size = (new->buffer_size != -1) ? new->buffer_size : prev->buffer_size;
	prev->sock_options = (new->sock_options != 0xFF) ? new->sock_options : prev->sock_options;
	prev->non_block = (new->non_block != -1) ? new->non_block : prev->non_block;
	prev->protocol = (new->protocol != -1) ? new->protocol : prev->protocol;
	prev->user_cfg = IS_ERR(new->user_cfg) ? prev->user_cfg : new->user_cfg;
	prev->common = IS_ERR(new->common) ? prev->common : new->common;
	return;
}

static void sptest_update_task(struct sptest_task *prev, struct sptest_task *new)
{
	/* update sptest_task structure
	 * Extra checks are for single parameter update, which is for future*/
	prev->priority = (new->priority != -1) ? new->priority : prev->priority;
	if (cpu_online(new->cpu_affinity))
		prev->cpu_affinity = (new->cpu_affinity != -1) ? new->cpu_affinity : prev->cpu_affinity;
	prev->sched_policy = (new->sched_policy != 0xFF) ? new->sched_policy : prev->sched_policy;
	return;
}

int sptest_update_task_list(struct list_head *task_list, struct sptest_task *new_task)
{
	struct sptest_task *sp_task = NULL;

	if ((new_task->id < 0) || (new_task->id > SPTEST_TASKS_MAX)) {
		LOG_INFO("invalid task id:%d", new_task->id);
		return FAILURE;
	}

	if (new_task->id == SPTEST_TASKS_MAX) {
		list_for_each_entry(sp_task, task_list, list_node) {
			sptest_update_task(sp_task, new_task);
			sptest_update_task_config(sp_task->config, new_task->config);
		}
	} else {
		sp_task = sptest_get_task_by_id(task_list, new_task->id);
		if (sp_task) {
			sptest_update_task(sp_task, new_task);
			sptest_update_task_config(sp_task->config, new_task->config);
		} else {
			LOG_INFO("invalid task id:%d", new_task->id);
			return FAILURE;
		}
	}
	return SUCCESS;
}

int sptest_add_task_to_task_list(struct list_head *task_list, struct sptest_task *new_task)
{
	struct sptest_task *sp_task = NULL;
	struct sptest_task *tail_task = NULL;
	struct sptest_task_config *sp_config = NULL;
	int ret = SUCCESS;

	if ((!task_list) || !sptest_get_task_list()) {
		LOG_DEBUG("task_list parameter is invalid");
		return -EINVAL;
	}
	if (!new_task) {
		LOG_DEBUG("new_task parameter is invalid");
		return -EINVAL;
	}

	sp_task = kmalloc(sizeof(struct sptest_task), GFP_KERNEL);
	if (!sp_task) {
		LOG_ERROR("sptest_task memory allocation failed");
		ret = -ENOMEM;
		goto end;
	}
	memset(sp_task, 0x0, sizeof(struct sptest_task));
	sptest_update_task(sp_task, new_task);
	sp_task->state = SPTEST_TASK_STATE_CREATED;

	sp_config = kmalloc(sizeof(struct sptest_task_config), GFP_KERNEL);
	if (!sp_config) {
		LOG_ERROR("sptest_task_config memory allocation failed");
		ret = -ENOMEM;
		goto end;
	}
	sp_task->config = sp_config;
	memset(sp_config, 0x0, sizeof(struct sptest_task_config));
	sptest_update_task_config(sp_config, new_task->config);

	if (!list_empty(task_list)) {
		tail_task = list_last_entry(task_list, struct sptest_task, list_node);
		sp_task->id = tail_task->id + 1;
		sp_task->cpu_affinity = sptest_get_cpu(tail_task->cpu_affinity);
	} else {
		sp_task->id = 0;
		sp_task->cpu_affinity = 0;
	}
	list_add_tail(&sp_task->list_node, task_list);
	return ret;

end:
	if (sp_task) {
		kfree(sp_task);
		sp_task = NULL;
	}
	if (sp_config) {
		kfree(sp_config);
		sp_config = NULL;
	}
	return ret;
}

int sptest_update_task_list_by_defcfg(struct list_head *task_list,
		struct sptest_task *def_task, int ntasks)
{
	int i = 0;
	int ret = 0;

	if (!task_list) {
		LOG_DEBUG("task_list parameter is invalid");
		return -EINVAL;
	}
	if (!def_task) {
		LOG_DEBUG("def_task parameter is invalid");
		return -EINVAL;
	}
	if ((ntasks < 0) || (ntasks > SPTEST_TASKS_MAX)) {
		LOG_DEBUG("ntasks parameter is invalid");
		return -EINVAL;
	}

	sptest_clear_task_list(task_list);

	for (i = 0; i < ntasks; i++) {
		ret = sptest_add_task_to_task_list(task_list, def_task);
		if (ret != SUCCESS) {
			LOG_DEBUG("failed to add def_task to task_list");
			return ret;
		}
	}

	return ret;
}

static int sptest_init(void)
{
	int ret = SUCCESS;

	LOG_DEBUG("speedtest driver initializing...");

	INIT_LIST_HEAD(&g_task_list);

	if (!strncpy(driver.name, MODULE_NAME, sizeof(driver.name) - 1)) {
		LOG_ERROR("strncpy() failed to MODULE_NAME");
		return -EFAULT;
	}

	ret = sptest_create_procfs(&driver);
	if (ret < 0) {
		LOG_ERROR("failed to create procfs entry");
		return ret;
	}

	ret = sptest_register_chardev_ops(&driver);
	if (ret != SUCCESS) {
		LOG_ERROR("charector device registration failed");
		goto remove_procfs;
	}
	LOG_INFO("speedtest driver initialized");
	return SUCCESS;

remove_procfs:
	sptest_remove_procfs(&driver);
	return ret;
}

static void sptest_cleanup(void)
{
	LOG_DEBUG("speedtest driver cleaning up...");
	sptest_clear_task_list(&g_task_list);
	sptest_remove_procfs(&driver);
	sptest_unregister_chardev_ops(&driver);
	LOG_INFO("speedtest driver uninitialized!");
	return;
}

module_init(sptest_init);
module_exit(sptest_cleanup);
MODULE_LICENSE("GPL v2");
