// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright © 2022-2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * This file contains implementation of charector device file registration
 * and handling associated IOCTL commands
 */

#include <linux/fs.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include <linux/version.h>

#include "speedtest_driver.h"
#include "tr143diagtool.h"
#include "speedtest_helpers.h"

#define FIRST_MINOR_NUMBER	0
#define MAX_DEVICES			1
#define MIN_DATA_LEN		(1 * MB)

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 15, 0)
typedef unsigned int __poll_t;
#endif

#ifdef SPEEDTEST_DEBUG
void ioctl_print_config(struct tr143diag_config *config )
{
	LOG_INFO("IOCTL prints from application =>");
	LOG_INFO("test type   : %d", config->ttype);
	LOG_INFO("connections : %d", config->nconn);
	LOG_INFO("DSCP value  : %d", config->dscp);
	LOG_INFO("test mode   : %d", config->tmode);
	LOG_INFO("mode value  : %llu", config->mode.filesize);
	if (config->serverip.ip_type == IPV6)
		LOG_INFO("serverip6   : %pI6c", &config->serverip.ip.ip6);
	else if (config->serverip.ip_type == IPV4)
		LOG_INFO("serverip4   : %pI4", &config->serverip.ip.ip4);
	LOG_INFO("server_port : %d", config->serverport);
	LOG_INFO("server_name : %s", config->servername);
}

static void sptest_print_task(struct sptest_task *sp_task)
{
	struct sptest_task_config *config = NULL;
	struct http_user_config *user_cfg = NULL;
	char serverip[MAX_IP_LEN] = {0};

	config = sp_task->config;
	user_cfg = config->user_cfg;

	snprintf(serverip, sizeof(serverip), (user_cfg->server_ip.ip_type == IPV6) ? "[%pI6c]" : "%pI4",
			&user_cfg->server_ip.ip);
	LOG_INFO("configured sptest_task parameters =>");
	LOG_INFO("buffer_size: %llu", config->buffer_size);
	LOG_INFO("page_order : %d", config->page_order);
	LOG_INFO("serverip   : %s", serverip);
	LOG_INFO("serverport : %d", user_cfg->server_port);
	LOG_INFO("servername : %s", user_cfg->server_name);
	LOG_INFO("dscp  : %d", user_cfg->dscp);
	LOG_INFO("ttype : %d", user_cfg->ttype);
	LOG_INFO("tmode : %d", user_cfg->tmode);
	LOG_INFO("url   : %s", user_cfg->url);
	if (user_cfg->tmode)
		LOG_INFO("duration  : %llu", user_cfg->duration);
	else
		LOG_INFO("filesize  : %llu", user_cfg->data_len);
	if (user_cfg->wanip.ip.ip4) {
		LOG_INFO("wanip     : %pI4", serverip);
	}
	if (user_cfg->wanif[0] != '\0')
		LOG_INFO("wanif     : %s", user_cfg->wanif);
}

void print_perconn_result(struct PerConnectionResult *result)
{
	LOG_INFO("TCPOpenRequestTime  : %llu", result->TCPOpenRequestTime);
	LOG_INFO("TCPOpenResponseTime : %llu", result->TCPOpenResponseTime);
	LOG_INFO("ROMTime\t\t: %llu", result->ROMTime);
	LOG_INFO("BOMTime\t\t: %llu", result->BOMTime);
	LOG_INFO("EOMTime\t\t: %llu", result->EOMTime);
	LOG_INFO("TestBytesSent       : %llu", result->TestBytesSent);
	LOG_INFO("TestBytesReceived   : %llu", result->TestBytesReceived);
	LOG_INFO("TotalBytesSent      : %llu", result->TotalBytesSent);
	LOG_INFO("TotalBytesReceived  : %llu", result->TotalBytesReceived);
}

static void display_result(struct tr143diag_result *result)
{
	int index = 0;
	LOG_INFO("ROMTime : %llu", result->ROMTime);
	LOG_INFO("BOMTime : %llu", result->BOMTime);
	LOG_INFO("EOMTime : %llu", result->EOMTime);
	LOG_INFO("TestBytesSent       : %llu", result->TestBytesSent);
	LOG_INFO("TestBytesReceived   : %llu", result->TestBytesReceived);
	LOG_INFO("TotalBytesReceived(IP Layer)   : %llu", result->TotalBytesReceived);
	LOG_INFO("TotalBytesSent(IP Layer)       : %llu", result->TotalBytesSent);
	LOG_INFO("TestBytesSentUnderFullLoading  : %llu", result->TestBytesSentUnderFullLoading);
	LOG_INFO("TestBytesReceivedUnderFullLoading           : %llu", result->TestBytesReceivedUnderFullLoading);
	LOG_INFO("TotalBytesReceivedUnderFullLoading(IP Layer): %llu", result->TotalBytesReceivedUnderFullLoading);
	LOG_INFO("TotalBytesSentUnderFullLoading(IP Layer)    : %llu", result->TotalBytesSentUnderFullLoading);
	LOG_INFO("PeriodOfFullLoading     : %llu", result->periodOfFullLoading);
	LOG_INFO("Number of connections   : %u", result->nconn);
	for (index = 0; index < result->nconn; ++ index) {
		LOG_INFO("PerConnectionResult[%d]====>>>>>", index);
		print_perconn_result(&result->PerConnectionResult[index]);
	}
}

static void display_inc_result(struct tr143inc_result *inc_result)
{
	struct IncrementalResult *incresult = NULL;
	int i = 0;

	for (; i < inc_result->num_entries; i++) {
		incresult = inc_result->inc_result + i;
		LOG_DEBUG("IncrementalResult[%d]----", i);
		LOG_DEBUG("StartTime         :%llu", incresult->StartTime);
		LOG_DEBUG("EndTime           :%llu", incresult->EndTime);
		LOG_DEBUG("TestBytesSent     :%llu", incresult->TestBytesSent);
		LOG_DEBUG("TestBytesReceived :%llu", incresult->TestBytesReceived);
		LOG_DEBUG("TotalBytesSent     :%llu", incresult->TotalBytesSent);
		LOG_DEBUG("TotalBytesReceived :%llu", incresult->TotalBytesReceived);
	}
}
#endif

static int sptest_get_http_client_threads_state(int info_type, int state,
		struct tr143diag_notify *notify)
{
	struct sptest_task *sp_task = NULL;
	struct sptest_task_config *config = NULL;
	struct http_session_info *cl_info = NULL;
	int connections = 0;

	notify->time = 0;
	list_for_each_entry(sp_task, sptest_get_task_list(), list_node) {
		config = sp_task->config;
		wait_for_completion(&config->notify_state);
		cl_info = ((struct http_session_info*)config->data) + info_type;
		notify->http_code = cl_info->http_status_code;
		if (cl_info->state != state) {
			LOG_ERROR("sptest_task[%d] looking for state:%s, got state:%s", sp_task->id,
					sptest_task_state_to_str(state),
					sptest_task_state_to_str(cl_info->state));
			notify->err_code = cl_info->err_code;
			notify->ntype = TR143_DIAG_NTFY_ERR;
			continue;
		}
		if (state == SPTEST_TASK_STATE_TEST_INPROGRESS) {
			if (notify->time == 0)
				notify->time = cl_info->BOMTime;
			else
				notify->time = GET_LOWER_TIME(notify->time, cl_info->BOMTime);
		} else if (state == SPTEST_TASK_STATE_TEST_COMPLETED) {
			if (notify->time == 0)
				notify->time = cl_info->EOMTime;
			else
				notify->time = GET_GREATER_TIME(notify->time, cl_info->BOMTime);
		}
		++ connections;
	}
	return connections;
}

static int sptest_get_http_client_threads_start_state(int nconns,
		struct tr143diag_notify *notify)
{
	struct sptest_task *sp_task = NULL;
	struct sptest_task_config *config = NULL;
	int connections = 0;

	list_for_each_entry(sp_task, sptest_get_task_list(), list_node) {
		config = sp_task->config;
		if (!wait_for_completion_timeout(&config->notify_state,
				msecs_to_jiffies(2 * MSEC_PER_SEC)))
			LOG_ERROR("sptest_task[%d] init failed", sp_task->id);
		++ connections;
	}
	if (connections != nconns)
		return FAILURE;

	connections = sptest_get_http_client_threads_state(HTTP_SESSION_INFO_START_STATE,
			SPTEST_TASK_STATE_RUNNING, notify);
	if (connections != nconns)
		return FAILURE;
	return SUCCESS;
}

static int sptest_get_http_client_threads_bom_time(int nconns,
		struct tr143diag_notify *notify)
{
	int connections = 0;

	connections = sptest_get_http_client_threads_state(HTTP_SESSION_INFO_BOMTIME,
			SPTEST_TASK_STATE_TEST_INPROGRESS, notify);
	if (connections != nconns)
		return FAILURE;
	return SUCCESS;
}

static int sptest_get_http_client_threads_eom_time(int nconns,
		struct tr143diag_notify *notify)
{
	int connections = 0;

	if (sptest_get_http_client_threads_bom_time(nconns, notify))
		return FAILURE;

	connections = sptest_get_http_client_threads_state(HTTP_SESSION_INFO_EOMTIME,
			SPTEST_TASK_STATE_TEST_COMPLETED, notify);
	if (connections != nconns)
		return FAILURE;
	return SUCCESS;
}

static int sptest_get_http_client_threads_completed(int nconns)
{
	struct sptest_task *sp_task = NULL;
	struct sptest_task_config *config = NULL;
	struct http_session_info *info = NULL;
	int connections = 0;

	list_for_each_entry(sp_task, sptest_get_task_list(), list_node) {
		config = sp_task->config;
		info = (((struct http_session_info*)config->data) +
				HTTP_SESSION_INFO_RESULTS);
		if (completion_done(&config->notify_state) &&
				(info->state == SPTEST_TASK_STATE_CLOSING))
			++ connections;
	}
	if (connections != nconns)
		return FAILURE;
	return SUCCESS;
}

static int sptest_calc_aggr_result(struct tr143diag_result *result, int nconns)
{
	struct sptest_task *sp_task = NULL;
	struct sptest_task_config *config = NULL;
	struct http_session_info *session_info = NULL;
	struct PerConnectionResult *perconn_result = NULL;
	uint64_t NonFullLoadingTestBytes = 0;
	uint64_t TotalBytesBeforeFullLoading = 0;
	uint64_t TotalBytesAfterFullLoading = 0;
	uint64_t EOMTime_early = 0;
	uint64_t BOMTime_latest = 0;
	int connections = 0;

	result->http_status_code = 200;
	list_for_each_entry(sp_task, sptest_get_task_list(), list_node) {
		config = sp_task->config;
		if (!config->data)
			continue;
		result->ttype = config->user_cfg->ttype;
		wait_for_completion(&config->notify_state);
		session_info = ((struct http_session_info*)config->data) +
			HTTP_SESSION_INFO_RESULTS;
		if (session_info->state != SPTEST_TASK_STATE_CLOSING)
			continue;
		if ((result->http_status_code == 200) && (session_info->http_status_code != 200))
			result->http_status_code = session_info->http_status_code;

		perconn_result = session_info->perconn_result;
		if (session_info->err_code != TR143_COMPLETED) {
			if (session_info->err_code == TR143_NONE) {
				// Fetch error code from EOMTime information
				if ((session_info-1)->err_code != TR143_NONE) {
					result->err = (session_info-1)->err_code;
					continue;
				}
				// Fetch error code from BOMTime information
				if ((session_info-2)->err_code != TR143_NONE) {
					result->err = (session_info-2)->err_code;
					continue;
				}
				// Fetch error code from start state
				if ((session_info-3)->err_code != TR143_NONE) {
					result->err = (session_info-3)->err_code;
					continue;
				}
			} else {
				result->err = session_info->err_code;
				continue;
			}
		}

		if (result->ROMTime == 0)
			result->ROMTime = perconn_result->ROMTime;
		else
			result->ROMTime = GET_LOWER_TIME(result->ROMTime, perconn_result->ROMTime);

		if (result->BOMTime == 0)
			result->BOMTime = perconn_result->BOMTime;
		else
			result->BOMTime = GET_LOWER_TIME(result->BOMTime, perconn_result->BOMTime);

		result->EOMTime = GET_GREATER_TIME(result->EOMTime, perconn_result->EOMTime);
		BOMTime_latest = GET_GREATER_TIME(BOMTime_latest, result->BOMTime);
		if (EOMTime_early == 0)
			EOMTime_early = perconn_result->EOMTime;
		else
			EOMTime_early = GET_LOWER_TIME(EOMTime_early, perconn_result->EOMTime);

		result->TestBytesSent += perconn_result->TestBytesSent;
		result->TestBytesReceived += perconn_result->TestBytesReceived;

		result->TotalBytesSent = GET_GREATER_U64(result->TotalBytesSent,
				perconn_result->TotalBytesSent);
		result->TotalBytesReceived = GET_GREATER_U64(result->TotalBytesReceived,
				perconn_result->TotalBytesReceived);

		TotalBytesBeforeFullLoading = GET_GREATER_U64(TotalBytesBeforeFullLoading,
				session_info->interface_bytes_before_full_load);
		TotalBytesAfterFullLoading= GET_GREATER_U64(TotalBytesAfterFullLoading,
				session_info->interface_bytes_after_full_load);
		NonFullLoadingTestBytes += session_info->non_full_loading_test_bytes;

		memcpy(&result->PerConnectionResult[sp_task->id],
				session_info->perconn_result, sizeof(struct PerConnectionResult));
		++ connections;
	}
	if (result->ttype == TR143_DIAG_TEST_UPLOAD) {
		result->TestBytesSentUnderFullLoading =
			(result->TestBytesSent - NonFullLoadingTestBytes);
		result->TotalBytesSentUnderFullLoading = (TotalBytesAfterFullLoading -
			TotalBytesBeforeFullLoading);
	} else {
		result->TestBytesReceivedUnderFullLoading =
			(result->TestBytesReceived - NonFullLoadingTestBytes);
		result->TotalBytesReceivedUnderFullLoading = (TotalBytesAfterFullLoading
				- TotalBytesBeforeFullLoading);
	}
	result->periodOfFullLoading = EOMTime_early - BOMTime_latest;

	if (connections != nconns)
		return FAILURE;
	result->nconn = connections;
	return SUCCESS;
}

static bool sptest_prepare_sptest_task_struct(struct tr143diag_config *config,
		struct sptest_task *sp_task, const struct sptest_proc_limits *limits)
{
	struct sptest_task_config *task_config = NULL;
	struct http_user_config *user_cfg = NULL;
	char server_ip[MAX_IP_LEN] = {0};
	int num_entries = 0;
	int remainder = 0;

	if (!config || !sp_task || IS_ERR(limits)) {
		return false;
	}

	sp_task->priority = (MAX_RT_PRIO - 1);
	sp_task->sched_policy = SCHED_RR;

	task_config = sp_task->config;
	user_cfg = task_config->user_cfg;

	if (config->nconn <= 0) {
		LOG_INFO("invalid connections parameter");
		return false;
	}
	if (config->nconn > limits->max_conn) {
		LOG_INFO("maximum connection allowed is %d", limits->max_conn);
		return false;
	}

	user_cfg->nconns = config->nconn;
	user_cfg->server_ip.ip_type = config->serverip.ip_type;
	/*! updating sptest_task_config */
	if (config->serverport != 0)
		user_cfg->server_port = config->serverport;
	else
		user_cfg->server_port = 80;
#ifdef CONFIG_DNS_RESOLVER
	if (config->url[0] != '\0') {
		if (sptest_dns_resolve_hostname(user_cfg, (const char*)config->url) < 0)
			return false;
	} else if (!config->serverip.ip.ip4) {
		LOG_ERROR("URL or serverip:port is required");
		return false;
	} else {
		memcpy(&user_cfg->server_ip, &config->serverip, sizeof(struct ipaddr));
	}
#else
	if (config->serverip.ip.ip4) {
		memcpy(&user_cfg->server_ip, &config->serverip, sizeof(struct ipaddr));
	}
	if (config->url[0] != '\0') {
		strncpy(user_cfg->url, config->url, TR143_DIAG_URL_LEN);
	}
#endif
	if ((config->tmode != 0) && (config->tmode != 1))
		return false;
	user_cfg->tmode = config->tmode;
	if (config->tmode) {
		if (config->mode.duration <= 0) {
			LOG_INFO("duration should be grater than 0");
			return false;
		}
		if (config->mode.duration > limits->max_duration) {
			LOG_INFO("maximum allowed duration is %llu, given %llu",
					limits->max_duration, config->mode.duration);
			return false;
		}
		user_cfg->duration = config->mode.duration;
		user_cfg->data_len = limits->max_size;
	} else {
		if (config->mode.filesize < MIN_DATA_LEN) {
			LOG_INFO("minimum filesize %ld bytes is required", MIN_DATA_LEN);
			return false;
		}
		if (config->mode.filesize > limits->max_size) {
			LOG_INFO("maximum allowed filesize is %llu, given %llu",
					limits->max_size, user_cfg->data_len);
			return false;
		}
		user_cfg->data_len = config->mode.filesize;
	}
	task_config->buffer_size = limits->buffer_size;
	task_config->page_order = limits->page_order;

	/*! updating http_user_config */
	if ((config->ttype != TR143_DIAG_TEST_UPLOAD) &&
			(config->ttype != TR143_DIAG_TEST_DOWNLOAD))
		return false;
	user_cfg->ttype = config->ttype;
	if ((config->dscp != 0) && (config->dscp != 1))
		return false;
	user_cfg->dscp = config->dscp;

	if (user_cfg->server_ip.ip_type == IPV6) {
		snprintf(server_ip, sizeof(server_ip), "[%pI6c]", &user_cfg->server_ip.ip);
	} else {
		snprintf(server_ip, sizeof(server_ip), "%pI4", &user_cfg->server_ip.ip);
	}
	if (snprintf(user_cfg->server_name, TR143_HOSTNAME_LEN, "%s:%d", server_ip,
				user_cfg->server_port) < 0)
		return false;

	if (strnlen(config->wanif, IFNAMSIZ) > 0) {
		memset(user_cfg->wanif, 0, IFNAMSIZ);
		strncpy(user_cfg->wanif, config->wanif, IFNAMSIZ - 1);
	}
	if (config->wanip.ip.ip4)
		memcpy(&user_cfg->wanip, &config->wanip, sizeof(struct ipaddr));


	num_entries = config->inc_interval ? div_u64_rem(user_cfg->duration,
			config->inc_interval, &remainder) - 1 : 0;
	if (remainder)
		num_entries ++;

	if ((!config->tmode && config->inc_interval) ||
			(config->inc_interval && num_entries <= 0)) {
		LOG_ERROR("TR-143 Incremental interval parameter is invalid");
		return false;
	}
	if (num_entries > TR143_MAX_INC_RESULTS) {
		LOG_ERROR("Max TR-143 Incremental results supported : %d",
				TR143_MAX_INC_RESULTS);
		return false;
	}
	user_cfg->inc_interval = config->inc_interval;
	return true;
}

static long sptest_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct sptest_driver *driver = NULL;
	struct tr143diag_result *result = NULL;
	struct tr143diag_config config = {0};
	struct tr143diag_notify notify = {0};
	struct sptest_task sp_task = {0};
	struct sptest_task_config task_config = {0};
	struct http_user_config *usercfg = NULL;
	int max_entries = 0;
	int remainder = 0;
	long ret = 0;

	driver = (struct sptest_driver*) filp->private_data;
	if (IS_ERR(driver)) {
		LOG_ERROR("sptest_driver instance is null");
		return PTR_ERR(driver);
	}
	switch (cmd) {
		case TR143_DIAG_IOCTL_CONFIG:
			if (!arg) {
				LOG_ERROR("argument can't be null");
				ret = -EINVAL;
				goto end;
			}
			if ((driver->mgr_state != SPMANAGER_STATE_NONE) &&
					(driver->mgr_state != SPMANAGER_STATE_CONFIGURED) &&
					(driver->mgr_state != SPMANAGER_STATE_ABORTED)) {
				LOG_ERROR("can't be configure, waiting results to be fetched");
				ret = -EPERM;
				goto end;
			}
			ret = copy_from_user(&config, (struct tr143diag_config *) arg,
					sizeof(struct tr143diag_config));
			if(unlikely(ret != 0)) {
				LOG_ERROR("copy_from_user failed!");
				goto end;
			}
#ifdef SPEEDTEST_DEBUG
			ioctl_print_config(&config);
#endif
			memset(driver->user_data, 0x0, sizeof(struct http_user_config));
			memset(driver->common_data, 0x0, sizeof(struct sptest_task_common_data));
			task_config.user_cfg = driver->user_data;
			sp_task.config = &task_config;
			task_config.common = driver->common_data;

			if (!sptest_prepare_sptest_task_struct(&config, &sp_task,
						&driver->proc_limits)) {
				LOG_ERROR("invalid parameters found");
				ret = -EINVAL;
				goto end;
			}
#ifdef SPEEDTEST_DEBUG
			sptest_print_task(&sp_task);
#endif
			ret = sptest_update_task_list_by_defcfg(sptest_get_task_list(),
					&sp_task, config.nconn);
			if (unlikely(ret == 0)) {
				LOG_INFO("configured %d tasks for speed test",
						config.nconn);
#ifdef SPEEDTEST_DEBUG
				sptest_display_task_list(sptest_get_task_list());
#endif
				driver->nconns = config.nconn;
				driver->mgr_state = SPMANAGER_STATE_CONFIGURED;
			} else
				LOG_ERROR("failed to configure tasks!");
			break;
		case TR143_DIAG_IOCTL_START:
			if ((driver->mgr_state != SPMANAGER_STATE_CONFIGURED) &&
					(driver->mgr_state != SPMANAGER_STATE_ABORTED)) {
				LOG_ERROR("unable to start tasks, please re-configure or abort test");
				ret = -EPERM;
				goto end;
			}
			if (arg) {
				ret = copy_from_user(&notify, (struct tr143diag_notify*)arg,
						sizeof(struct tr143diag_notify));
				if(unlikely(ret != 0)) {
					LOG_ERROR("copy_from_user failed!");
					goto end;
				}
			}
			usercfg = (struct http_user_config*)driver->user_data;
			if (!usercfg) {
				LOG_ERROR("Doesn't have user configuration");
				ret = -EFAULT;
				goto end;
			}
			memset(&driver->common_data->shared_bits, 0x0,
					sizeof(driver->common_data->shared_bits));
			if (usercfg->inc_interval && !driver->common_data->inc_result) {
				max_entries = div_u64_rem(usercfg->duration, usercfg->inc_interval, &remainder) - 1;
				if (remainder)
					max_entries ++;

				LOG_DEBUG("IncrementalResult max_entries:%d size:%lu",
						max_entries, sizeof(struct tr143inc_result) +
						(sizeof(struct IncrementalResult) * max_entries));
				driver->common_data->inc_result = kmalloc(sizeof(struct tr143inc_result) +
						(sizeof(struct IncrementalResult) * max_entries), GFP_KERNEL);
				if (!driver->common_data->inc_result) {
					LOG_ERROR("IncrementalResult memory allocation failed");
					ret = -ENOMEM;
					goto end;
				}
				memset(driver->common_data->inc_result, 0x0,
						sizeof(struct tr143inc_result) +
						(sizeof(struct IncrementalResult) * max_entries));
				driver->common_data->inc_result->num_entries = 0;
				driver->common_data->inc_result->max_entries = max_entries;
				driver->common_data->tr143hrtimer.remaining_time = remainder;
				driver->common_data->tr143hrtimer.inc_interval = usercfg->inc_interval;
				driver->common_data->tr143hrtimer.result = driver->common_data->inc_result;
				driver->common_data->tr143hrtimer.shared_bits = &driver->common_data->shared_bits;
			}

			ret = sptest_start_task_list(sptest_get_task_list());
			if (ret != SUCCESS) {
				LOG_ERROR("failed to start task list");
				sptest_stop_task_list(sptest_get_task_list());
				ret = -EFAULT;
				goto end;
			}

			ret = sptest_get_http_client_threads_start_state(driver->nconns, &notify);
			if (ret == SUCCESS) {
				if (arg) {
					if (notify.ntype == TR143_DIAG_NTFY_BOM) {
						ret = sptest_get_http_client_threads_bom_time(
								driver->nconns, &notify);
					} else if (notify.ntype == TR143_DIAG_NTFY_EOM) {
						ret = sptest_get_http_client_threads_eom_time(
								driver->nconns, &notify);
					}
				}
			}

			if (ret == SUCCESS)
				driver->mgr_state = SPMANAGER_STATE_RUNNING;
			else
				sptest_stop_task_list(sptest_get_task_list());

			if (arg) {
				ret = copy_to_user((struct tr143diag_notify*) arg, &notify,
						sizeof(struct tr143diag_notify));
				if(unlikely(ret != 0)) {
					LOG_ERROR("copy_to_user failed!");
					goto end;
				}
			}
			break;
		case TR143_DIAG_IOCTL_ABORT:
			if (driver->mgr_state != SPMANAGER_STATE_RUNNING) {
				LOG_ERROR("speed test is not started");
				ret = -EPERM;
				goto end;
			}
			usercfg = (struct http_user_config*)driver->user_data;
			if (!usercfg) {
				LOG_ERROR("Doesn't have user configuration");
				ret = -EFAULT;
				goto end;
			}
			if (usercfg->inc_interval && driver->common_data->inc_result) {
				kfree(driver->common_data->inc_result);
				driver->common_data->inc_result = NULL;
			}

			ret = sptest_stop_task_list(sptest_get_task_list());
			if (ret != SUCCESS) {
				LOG_ERROR("failed to abort tasks");
				ret = -EFAULT;
				sptest_clear_task_list(sptest_get_task_list());
				driver->mgr_state = SPMANAGER_STATE_NONE;
				goto end;
			}
			driver->mgr_state = SPMANAGER_STATE_ABORTED;
			break;
		case TR143_DIAG_IOCTL_GETRESULT:
			if (!arg) {
				LOG_ERROR("argument can't be null");
				ret = -EINVAL;
				goto end;
			}
			if (driver->mgr_state != SPMANAGER_STATE_RUNNING) {
				LOG_ERROR("speed test is not started");
				ret = -EPERM;
				goto end;
			}
			if (sptest_get_http_client_threads_completed(driver->nconns)) {
				LOG_INFO("speed test is in progress");
				ret = -EBUSY;
				goto end;
			}
			usercfg = (struct http_user_config*)driver->user_data;
			if (!usercfg) {
				LOG_ERROR("Doesn't have user configuration");
				ret = -EFAULT;
				goto end;
			}
			if (usercfg->inc_interval && driver->common_data->inc_result) {
				kfree(driver->common_data->inc_result);
				driver->common_data->inc_result = NULL;
			}

			result = driver->priv_buf;
			if (!result) {
				LOG_ERROR("private buffer for result is not initialized");
				sptest_stop_task_list(sptest_get_task_list());
				ret = -ENOMEM;
				goto end;
			}
			memset(result, 0x0, sizeof(struct tr143diag_result));
			ret = sptest_calc_aggr_result(result, driver->nconns);
			if (ret < 0) {
				sptest_stop_task_list(sptest_get_task_list());
				LOG_ERROR("failed to get aggregate results err_code:%d", result->err);
				driver->mgr_state = SPMANAGER_STATE_CONFIGURED;
				goto end;
			}
			LOG_INFO("aggregate results done");
#ifdef SPEEDTEST_DEBUG
			display_result(result);
#endif

			if (sptest_stop_task_list(sptest_get_task_list())) {
				LOG_ERROR("failed to stop tasks");
				sptest_clear_task_list(sptest_get_task_list());
				driver->mgr_state = SPMANAGER_STATE_NONE;
				goto end;
			}
			ret = copy_to_user((struct tr143diag_result*) arg, result,
					sizeof(struct tr143diag_result));
			if(unlikely(ret != 0)) {
				LOG_ERROR("copy_to_user failed!");
				goto end;
			}
			driver->mgr_state = SPMANAGER_STATE_CONFIGURED;
			break;
		case TR143_DIAG_IOCTL_NOTIFY:
			if (driver->mgr_state != SPMANAGER_STATE_RUNNING) {
				LOG_ERROR("speed test is not started");
				ret = -EPERM;
				goto end;
			}
			ret = copy_from_user(&notify, (struct tr143diag_notify*)arg,
					sizeof(struct tr143diag_notify));
			if(unlikely(ret != 0)) {
				LOG_ERROR("copy_from_user failed!");
				goto end;
			}

			if (notify.ntype == TR143_DIAG_NTFY_BOM)
				sptest_get_http_client_threads_bom_time(driver->nconns, &notify);
			else if (notify.ntype == TR143_DIAG_NTFY_EOM)
				sptest_get_http_client_threads_eom_time(driver->nconns, &notify);

			ret = copy_to_user((struct tr143diag_notify*) arg, &notify,
					sizeof(struct tr143diag_notify));
			if(unlikely(ret != 0)) {
				LOG_ERROR("copy_to_user failed!");
				goto end;
			}
			break;
		case TR143_DIAG_IOCTL_GET_INCRESULT:
			if (!arg) {
				LOG_ERROR("argument can't be null");
				ret = -EINVAL;
				goto end;
			}
			if (driver->mgr_state != SPMANAGER_STATE_RUNNING) {
				LOG_ERROR("speed test is not started");
				ret = -EPERM;
				goto end;
			}
			usercfg = (struct http_user_config*)driver->user_data;
			if (!usercfg) {
				LOG_ERROR("Doesn't have user configuration");
				ret = -EFAULT;
				goto end;
			}
			if (!usercfg->inc_interval && !driver->common_data->inc_result) {
				LOG_ERROR("IncrementalResult is disabled, as part of configuration");
				ret = -EFAULT;
				goto end;
			}
#ifdef SPEEDTEST_DEBUG
			display_inc_result(driver->common_data->inc_result);
#endif
			ret = copy_to_user((struct tr143inc_result*)arg, driver->common_data->inc_result,
					sizeof(struct tr143inc_result) + (sizeof(struct IncrementalResult) * driver->common_data->inc_result->num_entries));
			if(unlikely(ret != 0)) {
				LOG_ERROR("copy_to_user failed!");
				goto end;
			}
			break;
		default:
			LOG_ERROR("invalid IOCTL command invoked");
	}
	return SUCCESS;

end:
	if ((cmd == TR143_DIAG_IOCTL_START) && driver->common_data->inc_result) {
		kfree(driver->common_data->inc_result);
		driver->common_data->inc_result = NULL;
	}
	return ret;
}

/*! currently sptest_poll is not supported
 *  contains only skeleton part of it.
 */
static __poll_t sptest_poll(struct file *filp, struct poll_table_struct *poll_table)
{
	/*! under development */
	/*
	struct sptest_driver *driver = NULL;
	__poll_t poll_mask = 0;

	driver = (struct sptest_driver*) filp->private_data;

	poll_wait(filp, &driver->waitq, poll_table);
	poll_mask |= (POLLIN | POLLRDNORM);
	*/
	return -EPERM;
}

static ssize_t sptest_read(struct file *filp, char __user *user_buf,
		size_t size, loff_t *offset)
{
	struct sptest_driver *driver = NULL;

	driver = (struct sptest_driver*) filp->private_data;
	/* write data to buffer */
	//wake_up_interruptible(&driver->waitq);

	return 0;
}

static ssize_t sptest_write(struct file *filp, const char __user *user_buf,
		size_t size, loff_t *offset)
{
	return -EPERM;
}

static int sptest_close(struct inode *inode, struct file *filp)
{
	struct sptest_driver *driver = NULL;

	driver = container_of(inode->i_cdev, struct sptest_driver, cdev);
	if (IS_ERR(driver)) {
		LOG_ERROR("sptest_driver instance is null");
		return PTR_ERR(driver);
	}
	atomic_dec(&driver->open_count);
	filp->private_data = NULL;
	return 0;
}

static int sptest_open(struct inode *inode, struct file *filp)
{
	struct sptest_driver *driver = NULL;

	driver = container_of(inode->i_cdev, struct sptest_driver, cdev);
	if (IS_ERR(driver)) {
		LOG_ERROR("sptest_driver instance is null");
		return PTR_ERR(driver);
	}
	if (atomic_read(&driver->open_count)) {
		LOG_INFO("device file busy with other process");
		return -EPERM;
	}
	filp->private_data = driver;

	atomic_inc(&driver->open_count);
	return 0;
}

static const struct file_operations sptest_driver_ops = {
	.owner = THIS_MODULE,
	.open = sptest_open,
	.release = sptest_close,
	.write = sptest_write,
	.read = sptest_read,
	.poll = sptest_poll,
	.unlocked_ioctl = sptest_ioctl
};

int sptest_register_chardev_ops(struct sptest_driver *driver)
{
	int ret = 0;
	static struct lock_class_key __key;

	/* setting no opened files */
	atomic_set(&driver->open_count, 0);

	driver->common_data = kmalloc(sizeof(struct sptest_task_common_data), GFP_KERNEL);
	if (!driver->common_data) {
		LOG_ERROR("memory not available");
		return -ENOMEM;
	}

	memset(driver->common_data, 0x0, sizeof(struct sptest_task_common_data));

	driver->priv_buf = kmalloc(sizeof(struct tr143diag_result), GFP_KERNEL);
	if (IS_ERR(driver->priv_buf)) {
		ret = PTR_ERR(driver->priv_buf);
		LOG_ERROR("memory not available");
		goto dealloc_common_data;
	}

	driver->user_data = kmalloc(sizeof(struct http_user_config), GFP_KERNEL);
	if (IS_ERR(driver->user_data)) {
		ret = PTR_ERR(driver->user_data);
		LOG_ERROR("memory not available");
		goto dealloc_privbuf;
	}

	/* initialize waitqueue for poll */
	init_waitqueue_head(&driver->waitq);

	ret = alloc_chrdev_region(&driver->dev_num, FIRST_MINOR_NUMBER, MAX_DEVICES,
			driver->name);
	if (ret < 0) {
		LOG_ERROR("alloc_chrdev_region() failed");
		driver->dev_num = 0;
		ret = -ENOMEM;
		goto dealloc_userdata;
	}

	cdev_init(&driver->cdev, &sptest_driver_ops);

	driver->dev_class = __class_create(THIS_MODULE, driver->name, &__key);
	if (IS_ERR(driver->dev_class)) {
		LOG_ERROR("device class registration failed");
		ret = PTR_ERR(driver->dev_class);
		goto unregister_chrdev;
	}

	driver->device = device_create(driver->dev_class, NULL, driver->dev_num,
			(void*)driver, driver->name);
	if (IS_ERR(driver->device)) {
		LOG_ERROR("device creattion failed");
		ret = PTR_ERR(driver->device);
		goto class_destroy;
	}

	ret = cdev_add(&driver->cdev, driver->dev_num, MAX_DEVICES);
	if (ret < 0) {
		LOG_ERROR("cdev_add() failed");
		goto device_destroy;
	}

	LOG_INFO("speedtest char device registered i.e (/dev/%s %d:%d)", driver->name,
			MAJOR(driver->dev_num), MINOR(driver->dev_num));

	return SUCCESS;

device_destroy:
	device_destroy(driver->dev_class, driver->dev_num);
class_destroy:
	class_destroy(driver->dev_class);
	driver->dev_class = NULL;
unregister_chrdev:
	unregister_chrdev_region(FIRST_MINOR_NUMBER, MAX_DEVICES);
	driver->dev_num = 0;
dealloc_userdata:
	if (driver->user_data) {
		kfree(driver->user_data);
		driver->user_data = NULL;
	}
dealloc_privbuf:
	if (driver->priv_buf) {
		kfree(driver->priv_buf);
		driver->priv_buf = NULL;
	}
dealloc_common_data:
	if (driver->common_data) {
		kfree(driver->common_data);
		driver->common_data = NULL;
	}
	LOG_ERROR("speedtest char device file registration failed");
	return ret;
}

void sptest_unregister_chardev_ops(struct sptest_driver *driver)
{
	cdev_del(&driver->cdev);

	if (driver->device) {
		device_destroy(driver->dev_class, driver->dev_num);
	}
	if (driver->dev_class) {
		class_destroy(driver->dev_class);
		driver->dev_class = NULL;
	}

	unregister_chrdev_region(driver->dev_num, MAX_DEVICES);
	driver->dev_num = 0;

	if (driver->priv_buf) {
		kfree(driver->priv_buf);
		driver->priv_buf = NULL;
	}
	if (driver->user_data) {
		kfree(driver->user_data);
		driver->user_data = NULL;
	}
	if (driver->common_data) {
		kfree(driver->common_data->inc_result);
		driver->common_data->inc_result = NULL;
		kfree(driver->common_data);
		driver->common_data = NULL;
	}
	LOG_INFO("speedtest char device unregistered");
	return;
}
