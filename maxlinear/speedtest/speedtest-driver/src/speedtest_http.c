// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022 - 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * This file contains http kernel thread routine and http helper functions
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/tcp.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <net/sock.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/timer.h>
#include <asm/bitops.h>
#include <linux/version.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/wait.h>
#include <linux/dns_resolver.h>

#include "speedtest_driver.h"
#include "speedtest_http.h"
#include "speedtest_data_init.h"
#include "speedtest_helpers.h"

#define THREAD_TAG				"http_client_kthread"
#define HTTP_CONTENT_TYPE_STR	"application/octet-stream"

#define RESPONSE_MSG_LEN 120
#define METHOD_NAME_LEN_MAX	10
/*! dns resolution will be carried out upto below number of times */
#define MAX_DNS_RESOLVE_TRIALS	2

#define TIMER_BIT_POS				BIT(0)
#define FULL_LOADING_BIT			BIT(1)
#define FIRST_THREAD_STARTED_BIT	BIT(2)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
#define setup_timer(t, cb, d)	timer_setup(t, cb, 0)
#endif

/*! Temporary variables used while calculating incremental results */
static struct IncrementalResult cur_incresult = {0};
static struct IncrementalResult prev_incresult = {0};

/*! to enable/disable full loading on simultaneous connections */
DEFINE_SPINLOCK(full_load_spinlock);
static struct full_loading {
	int bit_pos;				/*! Indicates bit pos of full load, currently 0 */
	volatile unsigned long flag; /*! Holds full load state via bit_pos */
	atomic_t http_connected; /*! This will be updated as HTTP sessions connected */
} full_load = {0};

static char methods [][METHOD_NAME_LEN_MAX] = {
	"POST",		/*! For test type upload */
	"GET",		/*! For test type download */
	"PUT"		/*! For test type upload */
};

int sptest_dns_resolve_hostname(struct http_user_config *user_cfg, const char *url)
{
	int ret = SUCCESS;
#ifdef CONFIG_DNS_RESOLVER
	char options[13] = "Hello World!";	// This enables upcall for dns_query()
	char domain[TR143_HOSTNAME_LEN] = {0};
	uint16_t port = 80;
	char *server_ip = NULL;
	time64_t expiry = 0;
	ssize_t len = 0;

	if (!user_cfg || !url) {
		LOG_DEBUG("invalid parameters");
		return FAILURE;
	}

	len = strnlen(url, TR143_DIAG_URL_LEN);
	if (len < 0) {
		LOG_DEBUG("URL is invalid, len:%ld", len);
		ret = len;
		goto end;
	}

	ret = sscanf(url, "http://%128[^:]:%4hu", domain, &port);
	if (ret != 2)
		ret = sscanf(url, "http://%128[^/]", domain);
	if (ret <= 0) {
		LOG_DEBUG("URL format is invalid, match:%d", ret);
		ret = FAILURE;
		goto end;
	}

	len = strnlen(domain, TR143_HOSTNAME_LEN);
	if (len < 0) {
		LOG_DEBUG("domain name is invalid");
		ret = len;
		goto end;
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
	ret = dns_query(NULL, domain, len, &options[0], &server_ip, &expiry);
#else
	ret = dns_query(&init_net, NULL, domain, len, &options[0], &server_ip, &expiry, true);
#endif
	if (ret <= 0) {
		LOG_DEBUG("dns_query() failed, ret:%d", ret);
		ret = FAILURE;
		goto end;
	}
	if (server_ip) {
		LOG_DEBUG("URL:%s IP:%s PORT:%d TTL:%lld", url, server_ip, port, expiry);
	} else {
		LOG_DEBUG("server_ip is NUll!!!");
		ret = FAILURE;
		goto end;
	}

	memcpy(user_cfg->url, url, TR143_DIAG_URL_LEN);
	if (user_cfg->server_ip.ip_type == IPV6) {
		ret = in6_pton(server_ip, strlen(server_ip), user_cfg->server_ip.ip.ip6.s6_addr, -1, NULL);
		if (!ret) {
			LOG_ERROR(THREAD_TAG"Invalid IPv6 address: %s\n", server_ip);
			ret = FAILURE;
			goto end;
		}
	} else if (user_cfg->server_ip.ip_type == IPV4) {
		user_cfg->server_ip.ip.ip4 = in_aton(server_ip);
	}
	user_cfg->server_port = port;
	snprintf(user_cfg->server_name, TR143_HOSTNAME_LEN, "%s:%d", server_ip,
			user_cfg->server_port);
	kfree(server_ip);
end:
#else
	ret = FAILURE;
#endif
	return ret;
}

static bool inline check_for_full_loading(struct full_loading *full_load,
		int conns, bool beginning)
{
	bool ret = false;

	if (conns == 1) {
		clear_bit(full_load->bit_pos, &full_load->flag);
		atomic_set(&full_load->http_connected, 0);
		return ret;
	}

	spin_lock(&full_load_spinlock);
	if (beginning) {
		if (!atomic_read(&full_load->http_connected))
			set_bit(full_load->bit_pos, &full_load->flag);
		atomic_inc(&full_load->http_connected);
		if (atomic_read(&full_load->http_connected) == conns) {
			clear_bit(full_load->bit_pos, &full_load->flag);
			ret = true;
		}
	} else {
		if (atomic_read(&full_load->http_connected) == conns) {
			set_bit(full_load->bit_pos, &full_load->flag);
			ret = true;
		}
		atomic_dec(&full_load->http_connected);
	}
	spin_unlock(&full_load_spinlock);

	return ret;
}

static int parse_http_response(int task_id, struct http_response *hdr_res,
		char *response_msg, int response_msg_len)
{
	int k = 0;
	int j = 0;
	int index = 0;
	char *token = NULL;
	char *v_name = NULL;
	char *content_len = NULL;
	char *line[RESPONSE_MSG_LEN];
	char *line_token = NULL;

	if (!response_msg) {
		LOG_ERROR(THREAD_TAG"[%d] response_msg is empty ", task_id);
		return 0;
	}
	/*! Split the header with delimeter '\r\n' */
	while ((token = strsep(&response_msg, "\r\n")) != 0)
		line[index++] = token;

	/*! Split the header with delimeter ':' and ' ' */
	for (j = 0; j < index; ++j) {
		if (j == 0) {
			line_token = strsep(&line[j], " ");
			if (line_token != NULL) {
				strncpy(hdr_res->version, line_token, HTTP_VERSION_LEN - 1);
				hdr_res->version[HTTP_VERSION_LEN - 1] = '\0';
			}
			line_token = strsep(&line[j], " ");
			if (line_token != NULL)
				hdr_res->status_code = simple_strtoul(line_token, NULL, 10);
			line_token = strsep(&line[j], "\r\n");
			if (line_token != NULL) {
				strncpy(hdr_res->text_phrase, line_token,
					HTTP_REASON_PHRASE_LEN_MAX - 1);
				hdr_res->text_phrase[HTTP_REASON_PHRASE_LEN_MAX - 1] = '\0';
			}
		} else if ((v_name = strsep(&line[j], ":")) != 0) {
			/*! Response header parameters */
			if (strcasecmp(v_name, "Content-Type") == 0) {
				hdr_res->hdr[HTTP_CONTENT_TYPE].valid = true;
				line_token = strsep(&line[j], ":");
				if (line_token != NULL) {
					strncpy(hdr_res->hdr[HTTP_CONTENT_TYPE].value.str, line_token,
						HTTP_HEADER_LEN_MAX - 1);
					hdr_res->hdr[HTTP_CONTENT_TYPE].value.str[HTTP_HEADER_LEN_MAX - 1] = '\0';
					k++;
				}
			} else if (strcasecmp(v_name, "Content-Length") == 0) {
				content_len = strsep(&line[j], ":");
				hdr_res->hdr[HTTP_CONTENT_LEN].valid = true;
				if (content_len != NULL) {
					if (content_len[0] == ' ')
						memmove(content_len, content_len+1, strlen(content_len));
					if (!kstrtoull(content_len, 10, &hdr_res->hdr[HTTP_CONTENT_LEN].value.num))
						k++;
				}
			} else if (strcasecmp(v_name, "ETag") == 0) {
				hdr_res->hdr[HTTP_ETAG].valid = true;
				line_token = strsep(&line[j], ":");
				if (line_token != NULL) {
					strncpy(hdr_res->hdr[HTTP_ETAG].value.str, line_token,
						HTTP_HEADER_LEN_MAX - 1);
					hdr_res->hdr[HTTP_ETAG].value.str[HTTP_HEADER_LEN_MAX - 1] = '\0';
					k++;
				}
			} else if (strcasecmp(v_name, "Alt_svc") == 0) {
				hdr_res->hdr[HTTP_ALT_SVC].valid = true;
				line_token = strsep(&line[j], ":");
				if (line_token != NULL) {
					strncpy(hdr_res->hdr[HTTP_ALT_SVC].value.str, line_token,
						HTTP_HEADER_LEN_MAX - 1);
					hdr_res->hdr[HTTP_ALT_SVC].value.str[HTTP_HEADER_LEN_MAX - 1] = '\0';
					k++;
				}
			} else if (strcasecmp(v_name, "Header_Type_Max") == 0) {
				hdr_res->hdr[HTTP_HEADER_TYPE_MAX].valid = true;
				line_token = strsep(&line[j], ":");
				if (line_token != NULL) {
					strncpy(hdr_res->hdr[HTTP_HEADER_TYPE_MAX].value.str, line_token,
						HTTP_HEADER_LEN_MAX - 1);
					hdr_res->hdr[HTTP_HEADER_TYPE_MAX].value.str[HTTP_HEADER_LEN_MAX - 1] = '\0';
					k++;
				}
			} else if (strcasecmp(v_name, "Location") == 0) {
				hdr_res->hdr[HTTP_LOCATION].valid = true;
				line_token = strsep(&line[j], "\r\n");
				if (line_token != NULL) {
					strncpy(hdr_res->hdr[HTTP_LOCATION].value.str, line_token,
						HTTP_HEADER_LEN_MAX - 1);
					hdr_res->hdr[HTTP_LOCATION].value.str[HTTP_HEADER_LEN_MAX - 1] = '\0';
					if (hdr_res->hdr[HTTP_LOCATION].value.str[0] == ' ') {
						memmove(hdr_res->hdr[HTTP_LOCATION].value.str,
								hdr_res->hdr[HTTP_LOCATION].value.str + 1,
								strlen(hdr_res->hdr[HTTP_LOCATION].value.str));
					}
					k++;
				}
			} else if (strcasecmp(v_name, "Connection") == 0) {
				hdr_res->hdr[HTTP_CONNECTION].valid =true;
				line_token = strsep(&line[j], ":");
				if (line_token != NULL) {
					strncpy(hdr_res->hdr[HTTP_CONNECTION].value.str, line_token,
						HTTP_HEADER_LEN_MAX - 1);
					hdr_res->hdr[HTTP_CONNECTION].value.str[HTTP_HEADER_LEN_MAX - 1] = '\0';
					k++;
				}
			}
		}
	}
	return k;
}

static enum hrtimer_restart tr143_interval_timer_task(struct hrtimer *hrtimer)
{
	struct tr143_hrtimer *timer = container_of(hrtimer, struct tr143_hrtimer, hrtimer);
	struct sptest_task *sp_task = NULL;
	struct http_session_info *session_info = NULL;
	struct tr143inc_result *inc_result = NULL;
	struct IncrementalResult *incresult = NULL;
	int ret = 0;

	if (!timer->inc_interval)
		goto timer_expired;

	inc_result = timer->result;
	inc_result->num_entries ++;

	if (inc_result->max_entries < inc_result->num_entries) {
		inc_result->num_entries --;
		goto timer_expired;
	}

	if ((inc_result->max_entries == inc_result->num_entries) && timer->remaining_time) {
		ret = hrtimer_forward_now(hrtimer, ktime_set(timer->remaining_time, 0));
	} else {
		ret = hrtimer_forward_now(hrtimer, ktime_set(timer->inc_interval, 0));
	}
	incresult = inc_result->inc_result + (inc_result->num_entries - 1);
	NOTEDOWN_TIME(incresult->EndTime);
	if (inc_result->num_entries < inc_result->max_entries)
		(incresult + 1)->StartTime = incresult->EndTime;

	list_for_each_entry(sp_task, sptest_get_task_list(), list_node) {
		session_info = ((struct http_session_info*)sp_task->config->data) +
				HTTP_SESSION_INFO_RESULTS;
		incresult->TestBytesSent += session_info->perconn_result->TestBytesSent;
		incresult->TestBytesReceived += session_info->perconn_result->TestBytesReceived;
	}

	if (timer->inc_interval) {
		timer->result->num_entries = inc_result->num_entries;
		timer->tr143_incresult = incresult;
		timer->tr143_thread->tr143_t = true;
		wake_up_interruptible(&timer->tr143_thread->tr143_wait_queue);
		incresult = timer->tr143_incresult;
	}
#ifdef SPEEDTEST_DEBUG
	LOG_DEBUG("%s() timer_overrun:%d cur_entries:%d max_entries:%d", __func__,
			ret, inc_result->num_entries, inc_result->max_entries);
#endif
	return HRTIMER_RESTART;

timer_expired:
	clear_bit(TIMER_BIT_POS, timer->shared_bits);
	LOG_DEBUG("TR-143 timer expired Time:%llu usec", sptest_get_time_now_us());
	return HRTIMER_NORESTART;
}

static inline void tr143_timer_init(struct tr143_hrtimer *tr143timer,
		struct sptest_task_config *config)
{
	tr143timer->tr143_thread = kmalloc(sizeof(struct tr143_stats_collector_thread) , GFP_KERNEL);
	if (tr143timer->tr143_thread) {
		tr143timer->tr143_thread->tr143_prev = 1;
		memset(&prev_incresult, 0x0, sizeof(prev_incresult));

		if (tr143timer->inc_interval) {
			init_waitqueue_head(&tr143timer->tr143_thread->tr143_wait_queue);
			tr143timer->tr143_thread->task_stats_collect = kthread_create(tr143_stats_collect_kthread,
					(void *)tr143timer, "speedtest_stats_collector");
			if (tr143timer->tr143_thread->task_stats_collect) {
				get_task_struct(tr143timer->tr143_thread->task_stats_collect);
				wake_up_process(tr143timer->tr143_thread->task_stats_collect);
			}
		}
	}
	hrtimer_init(&tr143timer->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	tr143timer->hrtimer.function = tr143_interval_timer_task;
	tr143timer->shared_bits = &config->common->shared_bits;
	if (tr143timer->inc_interval) {
		hrtimer_start(&tr143timer->hrtimer,
				ktime_set(tr143timer->inc_interval, 0), HRTIMER_MODE_REL);
		NOTEDOWN_TIME(tr143timer->result->inc_result[0].StartTime);
	} else {
		hrtimer_start(&tr143timer->hrtimer,
				ktime_set(config->user_cfg->duration, 0), HRTIMER_MODE_REL);
	}

	LOG_DEBUG("TR-143 timer started Time:%llu usec", sptest_get_time_now_us());
}

static inline uint64_t sptest_get_interface_stats(struct net_device *net_dev, bool recvd)
{
	struct rtnl_link_stats64 storage64 = {0};
	struct net_device_stats *stats = NULL;
	uint64_t stat = 0;

	rtnl_lock();
	if (!net_dev)
		goto end;
	if (net_dev->netdev_ops->ndo_get_stats64) {
		net_dev->netdev_ops->ndo_get_stats64(net_dev, &storage64);
		stat = (recvd ? storage64.rx_bytes : storage64.tx_bytes);
	} else if (net_dev->netdev_ops->ndo_get_stats) {
		stats = net_dev->netdev_ops->ndo_get_stats(net_dev);
		if (stats)
			stat = (recvd ? stats->rx_bytes : stats->tx_bytes);
	} else {
		stats = &net_dev->stats;
		stat = (recvd ? stats->rx_bytes : stats->tx_bytes);
	}
	LOG_DEBUG("Interface:%s bytes_%s:%llu", net_dev->name, recvd ? "rcvd":"sent", stat);
end:
	rtnl_unlock();
	return stat;
}

static inline int send_single_page(struct socket *sk, struct page *pg, int size, int8_t nb)
{
	int offset = 0;
	int ret = 0;
	volatile int sent_bytes = 0;
	int flag = nb ? MSG_DONTWAIT : 0;

	while (size > 0) {
		ret = kernel_sendpage(sk, pg, offset, size, flag);
		if (nb && (ret == -EWOULDBLOCK))
			continue;
		if (unlikely(ret < 0))
			return ret;
		sent_bytes += ret;
		offset = sent_bytes + 1;
		size -= ret;
	}
	return sent_bytes;
}

static inline int send_pages(struct socket *sk, struct list_head *head, int data_len, int8_t nb)
{
	struct page_node *page_node = NULL;
	volatile int sent_bytes = 0;
	int data_tobe_send = 0;
	int ret = 0;

	list_for_each_entry(page_node, head, list_node) {
		data_tobe_send = ((page_node->dsize < data_len) ? page_node->dsize : data_len);
		ret = send_single_page(sk, page_node->page, data_tobe_send, nb);
		if (unlikely(ret < 0))
			return ret;
		data_len -= ret;
		sent_bytes += ret;
	}

	return sent_bytes;
}

static int http_client_read_response_header(int task_id, struct socket *sk,
		char *http_resp_header, int max_len, int *actual_hdr_len)
{
	struct msghdr msg = {0};
	struct kvec vector = {0};
	int recv_bytes = 0;
	int remain_bytes = 0;
	int prev_offset = 0;
	int ret = 0;
	int i = 0;

	if (!sk) {
		LOG_DEBUG(THREAD_TAG"[%d] invalid socket parameter", task_id);
		return -EINVAL;
	}
	if ((!http_resp_header) || (max_len < 0) ||
			(max_len < HTTP_RESPONSE_HEADER_SIZE_MAX)) {
		LOG_DEBUG(THREAD_TAG"[%d] invalid buffer parameters", task_id);
		return -EINVAL;
	}

	*actual_hdr_len = 0;
	vector.iov_base = http_resp_header;
	vector.iov_len = max_len;
	remain_bytes = max_len;

	while (remain_bytes > 0) {
		ret = kernel_recvmsg(sk, &msg, &vector, 1, remain_bytes, 0);
		if (unlikely((ret == -EWOULDBLOCK) || (ret == -EAGAIN)))
			continue;
		if (unlikely(ret < 0)) {
			LOG_ERROR(THREAD_TAG"[%d] failed to read http header response", task_id);
			return ret;
		}
		prev_offset = recv_bytes;
		remain_bytes -= ret;
		recv_bytes += ret;
		vector.iov_base = http_resp_header + recv_bytes;
		vector.iov_len = remain_bytes;

		for (i = recv_bytes; i > 3; i--) {
			if ((http_resp_header[i] == '\n') & (http_resp_header[i - 1] == '\r') &
					(http_resp_header[i - 2] == '\n') & (http_resp_header[i - 3] == '\r')) {
				(*actual_hdr_len) = i;
				return recv_bytes;
			}
			continue;
		}
	}
	return FAILURE;
}

/*! bind to wan interface */
static int bindto_wanif(struct socket *sock, struct http_user_config *cfg)
{
	struct net_device *net_dev = NULL;
	int ret = FAILURE;
	char __user wanif[IFNAMSIZ] = { 0 };
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 7, 19)
	sockptr_t optval;
#endif

	if (cfg->wanif[0] != '\0') {
		strncpy(wanif, cfg->wanif, IFNAMSIZ - 1);
	} else if (cfg->wanip.ip.ip4) {
		net_dev = ip_dev_find(sock_net(sock->sk), cfg->wanip.ip.ip4);
		if (!net_dev)
			return ret;
		rcu_read_lock();
		strncpy(wanif, net_dev->name, IFNAMSIZ - 1);
		dev_put(net_dev);
		rcu_read_unlock();
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 7, 19)
	optval = KERNEL_SOCKPTR(wanif);
#endif

	ret = sock_setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE,
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 7, 19)
				    optval,
#else
					wanif,
#endif
					strnlen(wanif, IFNAMSIZ));

	dev_put(net_dev);
	LOG_INFO("connecting via %s WAN interface", wanif);
	if (ret < 0)
		return ret;
	else
		return SUCCESS;
}

static int http_client_prepare_request(int task_id, char *req_buf, int max_buf,
		char *method, char *url, char *server_name, char *cont_type, uint64_t cont_len,
		int8_t keep_alive)
{
	int ret = -1;
	int header_len = 0;

	if ((!req_buf) || (max_buf < 0)) {
		LOG_DEBUG(THREAD_TAG"[%d] http request buffer parameter is invalid", task_id);
		return ret;
	}

	if ((!method) || (!server_name) || (!cont_type)) {
		LOG_DEBUG(THREAD_TAG"[%d] http string parameters are invalid", task_id);
		return ret;
	}

	if ((cont_len < 0) || (keep_alive < 0) || (keep_alive > 1)) {
		LOG_DEBUG(THREAD_TAG"[%d] http interger parameters are invalid", task_id);
		return ret;
	}
	if (!strcmp(method, "GET") && !url) {
		LOG_DEBUG(THREAD_TAG"[%d] URL is missing", task_id);
		return ret;
	}

	if (url) {
		if (strnchr(url, 1, '/'))
			ret = snprintf(req_buf, max_buf, "%s %s HTTP/1.1\r\n", method, url);
		else if (strnstr(url, "http://", sizeof("http://")))
			ret = snprintf(req_buf, max_buf, "%s %s HTTP/1.1\r\n", method, url);
		else
			ret = snprintf(req_buf, max_buf, "%s /%s HTTP/1.1\r\n", method, url);
	} else
		ret = snprintf(req_buf, max_buf, "%s / HTTP/1.1\r\n", method);
	if (ret < 0) {
		goto end;
	}
	header_len = ret;

	ret = snprintf(req_buf + header_len, max_buf,
			"Host: %s\r\n"
			"Accept: */*\r\n"
			"Content-Type: %s\r\n", server_name, cont_type);
	if (ret < 0)
		goto end;

	header_len += ret;

	if (strcmp(method, "GET")) {
		ret = snprintf(req_buf + header_len, max_buf, "Content-Length: %llu\r\n",
				cont_len);
		if (ret < 0)
			goto end;
		header_len += ret;
	}

	if (keep_alive)
		ret = snprintf(req_buf + header_len, max_buf,
				"Connection: Keep-Alive\r\n\r\n");
	else
		ret = snprintf(req_buf + header_len, max_buf,
				"Connection: close\r\n\r\n");
	if (ret < 0)
		goto end;

	header_len += ret;
	return header_len;
end:
	LOG_ERROR(THREAD_TAG"[%d] http request header preparation failed", task_id);
	return ret;
}

static int http_client_upload(struct sptest_task_config *config,
		char *http_header, int header_len, struct http_session_info *session_info,
		struct http_response **resp_struct)
{
	struct socket *sock = NULL;
	struct list_head page_list = {NULL, NULL};
	struct sockaddr_in addr = {0};
	struct sockaddr_in6 addr6 = {0};
	struct http_response *http_resp_struct = *resp_struct;
	struct http_user_config *user_cfg = NULL;
	struct PerConnectionResult *perconn_result = NULL;
	struct tr143_hrtimer *tr143timer = NULL;
	struct net_device *net_dev = NULL;
	char serverip[MAX_IP_LEN] = {0};
	char *http_resp_str = NULL;
	int http_resp_hdr_len = 0;
	int hdr_data_bytes = 0;
	uint64_t *sent_data_bytes = NULL;
	uint64_t *non_full_load_test_bytes = NULL;
	uint64_t *interface_bytes_before_full_loading = NULL;
	uint64_t *interface_bytes_after_full_loading = NULL;
	struct msghdr msg = {0};
	struct kvec vector = {0};
	int8_t page_order = 0;
	int data_tobe_send = 0;
	volatile int64_t data_len = 0;
	int task_id = 0;
	int ret = 0;

	if (!config || !http_header || (header_len < 0) || !session_info) {
		LOG_DEBUG(THREAD_TAG" invalid parameters");
		return -EINVAL;
	}
	task_id = config->id;
	page_order = config->page_order;
	user_cfg = config->user_cfg;
	tr143timer = &config->common->tr143hrtimer;

	http_resp_str = kmalloc(HTTP_RESPONSE_HEADER_SIZE_MAX, GFP_KERNEL);
	if (IS_ERR(http_resp_str)) {
		LOG_DEBUG(THREAD_TAG"[%d] memory not available", task_id);
		ret = PTR_ERR(http_resp_str);
		goto end;
	}

	perconn_result = (session_info + HTTP_SESSION_INFO_RESULTS)->perconn_result;
	non_full_load_test_bytes =
		&(session_info + HTTP_SESSION_INFO_RESULTS)->non_full_loading_test_bytes;
	interface_bytes_before_full_loading =
		&(session_info + HTTP_SESSION_INFO_RESULTS)->interface_bytes_before_full_load;
	interface_bytes_after_full_loading =
		&(session_info + HTTP_SESSION_INFO_RESULTS)->interface_bytes_after_full_load;
	sent_data_bytes = &perconn_result->TestBytesSent;

	if (user_cfg->server_ip.ip_type == IPV6)
		ret = sock_create_kern(&init_net, AF_INET6, SOCK_STREAM, IPPROTO_TCP, &sock);
	else if (user_cfg->server_ip.ip_type == IPV4)
		ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] socket creation failed ret:%d", task_id, -ret);
		goto end;
	}

	if (user_cfg->server_ip.ip_type == IPV4) {
		addr.sin_family = AF_INET;
		addr.sin_port = htons(user_cfg->server_port);
		addr.sin_addr.s_addr = user_cfg->server_ip.ip.ip4;
	} else if (user_cfg->server_ip.ip_type == IPV6) {
		addr6.sin6_family = AF_INET6;
		addr6.sin6_port = htons(user_cfg->server_port);
		addr6.sin6_addr = user_cfg->server_ip.ip.ip6;
	}

	INIT_LIST_HEAD(&page_list);

	if (user_cfg->wanif[0] != '\0' || user_cfg->wanip.ip.ip4) {
		ret = bindto_wanif(sock, user_cfg);
		if (ret != SUCCESS) {
			LOG_ERROR(THREAD_TAG"[%d] unable to bind WAN interface", task_id);
			goto close;
		}
	}

	if (config->buffer_size > user_cfg->data_len)
		ret = allocate_pages(&page_list, user_cfg->data_len, &page_order);
	else
		ret = allocate_pages(&page_list, config->buffer_size, &page_order);
	if(ret < 0) {
		LOG_DEBUG(THREAD_TAG"[%d] allocate_pages() failed", task_id);
		goto close;
	}
	LOG_DEBUG(THREAD_TAG"[%d] %d number of pages allocated", task_id, ret);
	if (page_order < config->page_order) {
		LOG_DEBUG(THREAD_TAG"[%d] page order reduced from %d to %d", task_id, config->page_order,
				page_order);
	}

	init_pages(&page_list);

	if (user_cfg->server_ip.ip_type == IPV6)
		snprintf(serverip, sizeof(serverip), "[%pI6c]", &user_cfg->server_ip.ip);
	else if (user_cfg->server_ip.ip_type == IPV4)
		snprintf(serverip, sizeof(serverip), "%pI4", &user_cfg->server_ip.ip);

	NOTEDOWN_TIME(perconn_result->TCPOpenRequestTime);
	if (user_cfg->server_ip.ip_type == IPV6)
		ret = kernel_connect(sock, (struct sockaddr*)&addr6, sizeof(addr6), 0);
	else if (user_cfg->server_ip.ip_type == IPV4)
		ret = kernel_connect(sock, (struct sockaddr*)&addr, sizeof(addr), 0);
	NOTEDOWN_TIME(perconn_result->TCPOpenResponseTime);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] TCP connect failed ret:%d", task_id, -ret);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_InitConnectionFailed;
		goto close;
	}
	LOG_INFO(THREAD_TAG"[%d] TCP connection established to '%s' http server", task_id,
			serverip);

	net_dev = ip_dev_find(sock_net(sock->sk), sock->sk->sk_rcv_saddr);
	if (!net_dev) {
		LOG_ERROR(THREAD_TAG"[%d] couldn't find interface", task_id);
		goto close;
	}
	set_bit(TIMER_BIT_POS, &config->common->shared_bits);
	if (user_cfg->tmode && !test_and_set_bit(FIRST_THREAD_STARTED_BIT,
				&config->common->shared_bits)) {
		tr143timer->net_dev = net_dev;
		tr143_timer_init(tr143timer, config);
	}

	vector.iov_base = http_header;
	vector.iov_len = header_len;
	data_len = user_cfg->data_len;

	NOTEDOWN_TIME(perconn_result->ROMTime);
	ret = kernel_sendmsg(sock, &msg, &vector, 1, header_len);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] HTTP header sending failed ret:%d", task_id, ret);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_NoTransferComplete;
		goto close;
	}
	LOG_DEBUG(THREAD_TAG"[%d] HTTP header sent:%d bytes", task_id, ret);

	*sent_data_bytes = ret;
	perconn_result->TotalBytesSent = sptest_get_interface_stats(net_dev, false);
	if (check_for_full_loading(&full_load, user_cfg->nconns, true))
		*interface_bytes_before_full_loading = perconn_result->TotalBytesSent;
	NOTEDOWN_TIME(perconn_result->BOMTime);

	session_info[HTTP_SESSION_INFO_BOMTIME].state = SPTEST_TASK_STATE_TEST_INPROGRESS;
	session_info[HTTP_SESSION_INFO_BOMTIME].BOMTime = perconn_result->BOMTime;
	complete(&config->notify_state);

	while ((!kthread_should_stop()) & (data_len > 0) & test_bit(TIMER_BIT_POS, &config->common->shared_bits)) {
		data_tobe_send = ((data_len > config->buffer_size) ? config->buffer_size : data_len);
		ret = send_pages(sock, &page_list, data_tobe_send, config->non_block);
		if (ret < 0) {
			LOG_ERROR(THREAD_TAG"[%d] failed to send data - ret:%d", task_id, -ret);
			session_info[HTTP_SESSION_INFO_EOMTIME].err_code = TR143_Error_NoTransferComplete;
			goto shutdown;
		}
		if (test_bit(full_load.bit_pos, &full_load.flag))
			*non_full_load_test_bytes += ret;

		data_len -= ret;
		*sent_data_bytes += ret;
	}
	if (*sent_data_bytes == user_cfg->data_len) {
		LOG_DEBUG(THREAD_TAG"[%d] HTTP data sent:%llu bytes", task_id, *sent_data_bytes);
	} else {
		LOG_DEBUG(THREAD_TAG"[%d] HTTP data sent:%llu bytes and remaining:%llu bytes",
				task_id, *sent_data_bytes, user_cfg->data_len - *sent_data_bytes);
		if (kthread_should_stop() || !test_bit(TIMER_BIT_POS, &config->common->shared_bits))
			kernel_sock_shutdown(sock, SHUT_WR);
	}

	ret = http_client_read_response_header(task_id, sock, http_resp_str,
			HTTP_RESPONSE_HEADER_SIZE_MAX, &http_resp_hdr_len);
	NOTEDOWN_TIME(perconn_result->EOMTime);

	perconn_result->TotalBytesSent = SAFE_SUB(sptest_get_interface_stats(net_dev, false),
			perconn_result->TotalBytesSent);
	if (check_for_full_loading(&full_load, user_cfg->nconns, false))
		*interface_bytes_after_full_loading = sptest_get_interface_stats(net_dev, false);
	session_info[HTTP_SESSION_INFO_EOMTIME].state = SPTEST_TASK_STATE_TEST_COMPLETED;
	session_info[HTTP_SESSION_INFO_EOMTIME].EOMTime = perconn_result->EOMTime;
	complete(&config->notify_state);

	if (ret < 0)
		goto shutdown;

	hdr_data_bytes = ret - http_resp_hdr_len;
	LOG_DEBUG(THREAD_TAG"[%d] HTTP Response(hlen:%d, tlen:%d)=\n%s", task_id, http_resp_hdr_len,
			ret, http_resp_str);
	ret = parse_http_response(task_id, http_resp_struct, http_resp_str, http_resp_hdr_len);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] http header not found", task_id);
		session_info[HTTP_SESSION_INFO_RESULTS].err_code = TR143_Error_NoResponse;
		goto shutdown;
	}
	session_info[HTTP_SESSION_INFO_RESULTS].http_status_code = http_resp_struct->status_code;
	session_info[HTTP_SESSION_INFO_EOMTIME].http_status_code = http_resp_struct->status_code;
	session_info[HTTP_SESSION_INFO_BOMTIME].http_status_code = http_resp_struct->status_code;
	if (http_resp_struct->status_code == 200) {
		LOG_INFO(THREAD_TAG"[%d] http upload request success -> %d  %s", task_id,
				http_resp_struct->status_code, http_resp_struct->text_phrase);
	} else {
		if (!test_bit(TIMER_BIT_POS, &config->common->shared_bits)) {
			if ( http_resp_struct->status_code == 400) {
				LOG_INFO(THREAD_TAG"[%d] http upload request success, timer expired -> %d %s",
						task_id, http_resp_struct->status_code, http_resp_struct->text_phrase);
				goto shutdown;
			}
		} else {
			LOG_ERROR(THREAD_TAG"[%d] http upload request failed -> %d %s", task_id,
					http_resp_struct->status_code, http_resp_struct->text_phrase);
			session_info[HTTP_SESSION_INFO_RESULTS].err_code = TR143_Error_NoTransferComplete;
			ret = FAILURE;
			if (http_resp_struct->status_code == 302) {
				LOG_INFO(THREAD_TAG"[%d] URL moved to %s", task_id,
						http_resp_struct->hdr[HTTP_LOCATION].value.str);
				ret = -EAGAIN;
			}
			goto shutdown;
		}
	}

#ifdef SPEEDTEST_DEBUG
	print_perconn_result(perconn_result);
#endif
shutdown:
	kernel_sock_shutdown(sock, SHUT_RDWR);
	LOG_INFO(THREAD_TAG"[%d] disconnected from '%s' server", task_id, serverip);
close:
	sock_release(sock);
	deallocate_pages(&page_list);
	if (user_cfg->tmode && test_and_clear_bit(FIRST_THREAD_STARTED_BIT,
				&config->common->shared_bits)) {
		if (tr143timer->tr143_thread) {
			tr143timer->tr143_thread->tr143_t = true;
			if (tr143timer->inc_interval) {
				kthread_stop(tr143timer->tr143_thread->task_stats_collect);
				put_task_struct(tr143timer->tr143_thread->task_stats_collect);
			}
			kfree(tr143timer->tr143_thread);
			tr143timer->tr143_thread = NULL;
		}
		hrtimer_cancel(&tr143timer->hrtimer);
	}
	if (net_dev)
		dev_put(net_dev);
	if (user_cfg->tmode)
		clear_bit(TIMER_BIT_POS, &config->common->shared_bits);
end:
	if (http_resp_str) {
		kfree(http_resp_str);
		http_resp_str = NULL;
	}
	return ret;
}

static int http_client_download(struct sptest_task_config *config,
		char *http_header, int header_len, struct http_session_info *session_info,
		struct http_response **resp_struct)
{
	struct socket *sock = NULL;
	struct sockaddr_in addr = {0};
	struct sockaddr_in6 addr6 = {0};
	struct http_user_config *user_cfg = NULL;
	struct http_response *http_resp_struct = *resp_struct;
	struct PerConnectionResult *perconn_result = NULL;
	char serverip[MAX_IP_LEN] = {0};
	char *http_resp_str = NULL;
	struct tr143_hrtimer *tr143timer = NULL;
	struct net_device *net_dev = NULL;
	int http_resp_hdr_len = 0;
	uint64_t *recv_data_bytes = NULL;
	volatile int64_t data_tobe_recvd = 0;
	uint64_t *non_full_load_test_bytes = NULL;
	uint64_t *interface_bytes_before_full_loading = NULL;
	uint64_t *interface_bytes_after_full_loading = NULL;
	char *data_buffer = NULL;
	int buffer_len = 0;
	struct msghdr msg = {0};
	struct kvec vector = {0};
	uint64_t cont_len = 0;
	int task_id = 0;
	int ret = 0;

	if (!config || !http_header || (header_len < 0) || !session_info) {
		LOG_DEBUG(THREAD_TAG" invalid parameters");
		return -EINVAL;
	}
	task_id = config->id;
	user_cfg = config->user_cfg;
	tr143timer = &config->common->tr143hrtimer;

	http_resp_str = kmalloc(HTTP_RESPONSE_HEADER_SIZE_MAX, GFP_KERNEL);
	if (IS_ERR(http_resp_str)) {
		LOG_DEBUG(THREAD_TAG"[%d] memory not available", task_id);
		ret = PTR_ERR(http_resp_str);
		goto end;
	}

	if (config->buffer_size < user_cfg->data_len) {
		data_buffer = kmalloc(config->buffer_size, GFP_KERNEL);
		buffer_len = config->buffer_size;
	} else {
		data_buffer = kmalloc(user_cfg->data_len, GFP_KERNEL);
		buffer_len = user_cfg->data_len;
	}

	perconn_result = (session_info + HTTP_SESSION_INFO_RESULTS)->perconn_result;
	non_full_load_test_bytes =
		&(session_info + HTTP_SESSION_INFO_RESULTS)->non_full_loading_test_bytes;
	interface_bytes_before_full_loading =
		&(session_info + HTTP_SESSION_INFO_RESULTS)->interface_bytes_before_full_load;
	interface_bytes_after_full_loading =
		&(session_info + HTTP_SESSION_INFO_RESULTS)->interface_bytes_after_full_load;
	recv_data_bytes = &perconn_result->TestBytesReceived;

	if (user_cfg->server_ip.ip_type == IPV4) {
		ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	} else if (user_cfg->server_ip.ip_type == IPV6) {
		ret = sock_create_kern(&init_net, AF_INET6, SOCK_STREAM, IPPROTO_TCP, &sock);
	}
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] socket creation failed ret:%d", task_id, -ret);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_Internal;
		goto end;
	}

	if (user_cfg->server_ip.ip_type == IPV4) {
		snprintf(serverip, sizeof(serverip), "%pI4", &user_cfg->server_ip.ip);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(user_cfg->server_port);
		addr.sin_addr.s_addr = user_cfg->server_ip.ip.ip4;
	} else if (user_cfg->server_ip.ip_type == IPV6) {
		snprintf(serverip, sizeof(serverip), "[%pI6c]", &user_cfg->server_ip.ip);
		addr6.sin6_family = AF_INET6;
		addr6.sin6_port = htons(user_cfg->server_port);
		addr6.sin6_addr = user_cfg->server_ip.ip.ip6;
	}

	if (user_cfg->wanif[0] != '\0' || user_cfg->wanip.ip.ip4) {
		ret = bindto_wanif(sock, user_cfg);
		if (ret != SUCCESS) {
			LOG_ERROR(THREAD_TAG"[%d] unable to bind WAN interface", task_id);
			goto close;
		}
	}

	NOTEDOWN_TIME(perconn_result->TCPOpenRequestTime);
	if (user_cfg->server_ip.ip_type == IPV6)
		ret = kernel_connect(sock, (struct sockaddr*)&addr6, sizeof(addr6), 0);
	else if (user_cfg->server_ip.ip_type == IPV4)
		ret = kernel_connect(sock, (struct sockaddr*)&addr, sizeof(addr), 0);
	NOTEDOWN_TIME(perconn_result->TCPOpenResponseTime);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] TCP connect failed ret:%d", task_id, -ret);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_InitConnectionFailed;
		goto close;
	}
	LOG_INFO(THREAD_TAG"[%d] TCP connection established to '%s' http server", task_id,
			serverip);

	net_dev = ip_dev_find(sock_net(sock->sk), sock->sk->sk_rcv_saddr);
	if (!net_dev) {
		LOG_ERROR(THREAD_TAG"[%d] couldn't find interface", task_id);
		goto close;
	}
	set_bit(TIMER_BIT_POS, &config->common->shared_bits);
	if (user_cfg->tmode && !test_and_set_bit(FIRST_THREAD_STARTED_BIT,
				&config->common->shared_bits)) {
		tr143timer->net_dev = net_dev;
		tr143_timer_init(tr143timer, config);
	}

	vector.iov_base = http_header;
	vector.iov_len = header_len;

	NOTEDOWN_TIME(perconn_result->ROMTime);
	ret = kernel_sendmsg(sock, &msg, &vector, 1, header_len);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] HTTP header sending failed ret:%d", task_id, ret);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_NoTransferComplete;
		goto close;
	}
	LOG_DEBUG(THREAD_TAG"[%d] HTTP header sent:%d bytes", task_id, ret);

	ret = http_client_read_response_header(task_id, sock, http_resp_str,
			HTTP_RESPONSE_HEADER_SIZE_MAX, &http_resp_hdr_len);
	if (ret < 0)
		goto shutdown;

	*recv_data_bytes = ret - http_resp_hdr_len;
	LOG_DEBUG(THREAD_TAG"[%d] HTTP Response(hlen:%d, tlen:%d)=\n%s", task_id, http_resp_hdr_len,
			ret, http_resp_str);
	ret = parse_http_response(task_id, http_resp_struct, http_resp_str, http_resp_hdr_len);
	if (ret < 0) {
		LOG_ERROR(THREAD_TAG"[%d] http header not found", task_id);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_NoResponse;
		goto shutdown;
	}

	session_info[HTTP_SESSION_INFO_RESULTS].http_status_code = http_resp_struct->status_code;
	session_info[HTTP_SESSION_INFO_BOMTIME].http_status_code = http_resp_struct->status_code;
	session_info[HTTP_SESSION_INFO_EOMTIME].http_status_code = http_resp_struct->status_code;
	if (http_resp_struct->status_code == 200) {
		LOG_INFO(THREAD_TAG"[%d] http download request success -> %d  %s", task_id,
				http_resp_struct->status_code, http_resp_struct->text_phrase);
	} else {
		LOG_ERROR(THREAD_TAG"[%d] http download request failed -> %d %s", task_id,
				http_resp_struct->status_code, http_resp_struct->text_phrase);
		session_info[HTTP_SESSION_INFO_BOMTIME].err_code = TR143_Error_NoTransferComplete;
		complete(&config->notify_state);
		ret = FAILURE;
		if (http_resp_struct->status_code == 302) {
			LOG_INFO(THREAD_TAG"[%d] URL moved to %s", task_id,
					http_resp_struct->hdr[HTTP_LOCATION].value.str);
			ret = -EAGAIN;
		}
		goto shutdown;
	}

	cont_len = http_resp_struct->hdr[HTTP_CONTENT_LEN].value.num;
	if (user_cfg->data_len < cont_len)
		cont_len = user_cfg->data_len;
	data_tobe_recvd = cont_len - *recv_data_bytes;
	vector.iov_base = data_buffer;
	vector.iov_len = buffer_len;

	perconn_result->TotalBytesReceived = sptest_get_interface_stats(net_dev, true);
	if (check_for_full_loading(&full_load, user_cfg->nconns, true))
		*interface_bytes_before_full_loading = perconn_result->TotalBytesReceived;
	NOTEDOWN_TIME(perconn_result->BOMTime);

	session_info[HTTP_SESSION_INFO_BOMTIME].BOMTime = perconn_result->BOMTime;
	session_info[HTTP_SESSION_INFO_BOMTIME].state = SPTEST_TASK_STATE_TEST_INPROGRESS;
	complete(&config->notify_state);

	while ((!kthread_should_stop()) & (*recv_data_bytes < cont_len) 
			& test_bit(TIMER_BIT_POS, &config->common->shared_bits)) {
		vector.iov_len = ((buffer_len < data_tobe_recvd) ? buffer_len : data_tobe_recvd);
		ret = kernel_recvmsg(sock, &msg, &vector, 1, vector.iov_len, MSG_TRUNC);
		if (unlikely(ret == -EWOULDBLOCK) || unlikely(ret == -EAGAIN)) {
			continue;
		}
		if (unlikely(ret < 0)) {
			LOG_ERROR(THREAD_TAG"[%d] failed to read http data - ret:%d", task_id, -ret);
			session_info[HTTP_SESSION_INFO_EOMTIME].err_code = TR143_Error_NoTransferComplete;
			goto shutdown;
		}
		if (test_bit(full_load.bit_pos, &full_load.flag))
			*non_full_load_test_bytes += ret;

		data_tobe_recvd -= ret;
		*recv_data_bytes += ret;
	}
	NOTEDOWN_TIME(perconn_result->EOMTime);

	perconn_result->TotalBytesReceived = SAFE_SUB(sptest_get_interface_stats(net_dev, true),
			perconn_result->TotalBytesReceived);
	if (check_for_full_loading(&full_load, user_cfg->nconns, false))
		*interface_bytes_after_full_loading = sptest_get_interface_stats(net_dev, true);

	session_info[HTTP_SESSION_INFO_EOMTIME].EOMTime = perconn_result->EOMTime;
	session_info[HTTP_SESSION_INFO_EOMTIME].state = SPTEST_TASK_STATE_TEST_COMPLETED;
	complete(&config->notify_state);

	kernel_sock_shutdown(sock, SHUT_RD);
	if (kthread_should_stop() || !test_bit(TIMER_BIT_POS, &config->common->shared_bits))
		ret = SUCCESS;
	if (*recv_data_bytes == user_cfg->data_len) {
		LOG_DEBUG(THREAD_TAG"[%d] http downloaded -> %llu bytes", task_id, *recv_data_bytes);
	} else {
		LOG_DEBUG(THREAD_TAG"[%d] http downloaded -> %llu bytes, remaining:%llu bytes", task_id,
				*recv_data_bytes, user_cfg->data_len - *recv_data_bytes);
	}
#ifdef SPEEDTEST_DEBUG
	print_perconn_result(perconn_result);
#endif
shutdown:
	kernel_sock_shutdown(sock, SHUT_RDWR);
	LOG_INFO(THREAD_TAG"[%d] disconnected from '%s' server", task_id, serverip);
close:
	sock_release(sock);
	if (user_cfg->tmode && test_and_clear_bit(FIRST_THREAD_STARTED_BIT,
				&config->common->shared_bits)) {
		if (tr143timer->tr143_thread) {
			tr143timer->tr143_thread->tr143_t = true;
			if (tr143timer->inc_interval) {
				kthread_stop(tr143timer->tr143_thread->task_stats_collect);
				put_task_struct(tr143timer->tr143_thread->task_stats_collect);
			}
			kfree(tr143timer->tr143_thread);
		}
		hrtimer_cancel(&tr143timer->hrtimer);
	}
	if (net_dev)
		dev_put(net_dev);
	if (user_cfg->tmode)
		clear_bit(TIMER_BIT_POS, &config->common->shared_bits);
end:
	if (data_buffer) {
		kfree(data_buffer);
		data_buffer = NULL;
	}
	if (http_resp_str) {
		kfree(http_resp_str);
		http_resp_str = NULL;
	}
	return ret;
}

int tr143_stats_collect_kthread(void *data)
{
	struct rtnl_link_stats64 storage64 = {0};
	const struct rtnl_link_stats64 *storage;
	struct tr143_hrtimer *tr143timer = (struct tr143_hrtimer*)data;
	while ((!kthread_should_stop())) {
		storage = dev_get_stats(tr143timer->net_dev, &storage64);
		if (tr143timer->tr143_thread->tr143_prev == 1) {
			tr143timer->tr143_thread->tr143_prev++;
			prev_incresult.TotalBytesSent = storage->tx_bytes;
			prev_incresult.TotalBytesReceived = storage->rx_bytes;
		} else {
			tr143timer->tr143_incresult->TotalBytesReceived = storage->rx_bytes;
			tr143timer->tr143_incresult->TotalBytesSent = storage->tx_bytes;
			if (tr143timer->result->num_entries - 1) {
				memcpy(&cur_incresult, tr143timer->tr143_incresult, sizeof(struct IncrementalResult));
				tr143timer->tr143_incresult->TestBytesSent -= prev_incresult.TestBytesSent;
				tr143timer->tr143_incresult->TotalBytesSent =
					SAFE_SUB(tr143timer->tr143_incresult->TotalBytesSent, prev_incresult.TotalBytesSent);
				tr143timer->tr143_incresult->TestBytesReceived -= prev_incresult.TestBytesReceived;
				tr143timer->tr143_incresult->TotalBytesReceived =
					SAFE_SUB(tr143timer->tr143_incresult->TotalBytesReceived,
							prev_incresult.TotalBytesReceived);
				memcpy(&prev_incresult, &cur_incresult, sizeof(struct IncrementalResult));
			} else {
				memcpy(&prev_incresult, tr143timer->tr143_incresult, sizeof(struct IncrementalResult));
				tr143timer->tr143_incresult->TotalBytesSent
					= SAFE_SUB(tr143timer->tr143_incresult->TotalBytesSent, prev_incresult.TotalBytesSent);
				tr143timer->tr143_incresult->TotalBytesReceived
					= SAFE_SUB(tr143timer->tr143_incresult->TotalBytesReceived,
							prev_incresult.TotalBytesReceived);
			}
		}
		tr143timer->tr143_thread->tr143_t = false;
		wait_event_interruptible(tr143timer->tr143_thread->tr143_wait_queue, tr143timer->tr143_thread->tr143_t);
	}
	return SUCCESS;
}

int sptest_http_client_kthread(void *data)
{
	struct sptest_task_config *config = NULL;
	struct http_session_info *session_info = NULL;
	struct http_response *http_resp_struct = NULL;
	char http_header[HTTP_REQUEST_HEADER_SIZE_MAX] = {0};
	struct http_user_config user_cfg = {0};
	int dns_resolve_trial = 0;
	size_t len = 0;
	int ret = 0;
	int id = -1;

	if (IS_ERR(data)) {
		LOG_ERROR(THREAD_TAG" parameters are invalid and thread start failed");
		ret = -EINVAL;
		goto end;
	}

	config = (struct sptest_task_config*)data;
	id = config->id;
	if (!config->user_cfg) {
		LOG_ERROR(THREAD_TAG"[%d] user configuration parameter is missing", id);
		ret = -EINVAL;
		goto end;
	}
	memcpy(&user_cfg, config->user_cfg, sizeof(struct http_user_config));

	len = (sizeof(struct http_session_info) * HTTP_SESSION_INFO_MAX) +
		sizeof(struct PerConnectionResult);

	session_info = kmalloc(len, GFP_KERNEL);
	if (IS_ERR_OR_NULL(session_info)) {
		LOG_ERROR(THREAD_TAG"[%d] memory not available and thread start failed", id);
		ret = PTR_ERR(session_info);
		goto end;
	}

	http_resp_struct = kmalloc(sizeof(struct http_response), GFP_KERNEL);
	if (IS_ERR_OR_NULL(http_resp_struct)) {
		LOG_DEBUG(THREAD_TAG"[%d] memory not available", id);
		ret = PTR_ERR(http_resp_struct);
		goto end;
	}

start:
	memset(session_info, 0x0, len);
	memset(http_resp_struct, 0x0, sizeof(struct http_response));

	(session_info + HTTP_SESSION_INFO_RESULTS)->perconn_result =
		(struct PerConnectionResult*)(session_info + HTTP_SESSION_INFO_MAX);
	config->data = session_info;
	complete(&config->notify_state);

	LOG_DEBUG(THREAD_TAG"[%d] configuration => affinity:%d priority:%d rt_priority:%d sched_policy:%d",
			id, get_cpu(), current->prio, current->rt_priority, current->policy);
	if (user_cfg.ttype == TR143_DIAG_TEST_DOWNLOAD) {
		ret = http_client_prepare_request(id, http_header, sizeof(http_header),
				methods[user_cfg.ttype], &user_cfg.url[0],
				user_cfg.server_name, HTTP_CONTENT_TYPE_STR, user_cfg.data_len, 0);
	} else if (user_cfg.ttype == TR143_DIAG_TEST_UPLOAD) {
		ret = http_client_prepare_request(id, http_header, sizeof(http_header),
				methods[user_cfg.ttype], &user_cfg.url[0],
				user_cfg.server_name, HTTP_CONTENT_TYPE_STR, user_cfg.data_len, 0);
	} else {
		LOG_DEBUG(THREAD_TAG"[%d] test type(%d) is invalid", id, user_cfg.ttype);
		ret = -EINVAL;
		goto start_failed;
	}
#ifdef SPEEDTEST_DEBUG
	LOG_INFO("\n%s", http_header);
#endif
	if ((ret < 0) || (ret > HTTP_REQUEST_HEADER_SIZE_MAX)) {
		LOG_DEBUG(THREAD_TAG"[%d] http header length is invalid header_len:%d", id, ret);
		ret = -EINVAL;
		goto start_failed;
	}
	session_info[HTTP_SESSION_INFO_START_STATE].state = SPTEST_TASK_STATE_RUNNING;
	complete(&config->notify_state);

	if (user_cfg.ttype == TR143_DIAG_TEST_UPLOAD) {
		ret = http_client_upload(config, http_header, ret, session_info, &http_resp_struct);
		if (ret == -EAGAIN) {
			goto dns_resolution;
		} else if (ret < 0) {
			LOG_DEBUG(THREAD_TAG"[%d] http upload failed", id);
			goto run_failed;
		}
	} else if (user_cfg.ttype == TR143_DIAG_TEST_DOWNLOAD) {
		ret = http_client_download(config, http_header, ret, session_info,
				&http_resp_struct);
		if (ret == -EAGAIN) {
			goto dns_resolution;
		} else if (ret < 0) {
			LOG_DEBUG(THREAD_TAG"[%d] http download failed", id);
			goto run_failed;
		}
	} else {
		LOG_ERROR("invalid test type requested");
		goto run_failed;
	}

dns_resolution:
	if (ret == -EAGAIN) {
		if (sptest_dns_resolve_hostname(&user_cfg,
					(const char*)http_resp_struct->hdr[HTTP_LOCATION].value.str) < 0) {
			LOG_ERROR(THREAD_TAG"[%d] DNS resolution failed for URL:%s",
					id, http_resp_struct->hdr[HTTP_LOCATION].value.str);
			goto run_failed;
		}
		init_completion(&config->notify_state);
		if (dns_resolve_trial < MAX_DNS_RESOLVE_TRIALS) {
			dns_resolve_trial ++;
			goto start;
		}
	}

	session_info[HTTP_SESSION_INFO_RESULTS].state = SPTEST_TASK_STATE_CLOSING;
	complete_all(&config->notify_state);
	LOG_DEBUG(THREAD_TAG"[%d] is stopping", id);
	return SUCCESS;

start_failed:
	session_info[HTTP_SESSION_INFO_START_STATE].err_code = TR143_Error_Internal;
	session_info[HTTP_SESSION_INFO_START_STATE].state = SPTEST_TASK_STATE_START_FAILED;
run_failed:
	if (session_info[HTTP_SESSION_INFO_START_STATE].err_code == TR143_NONE)
		session_info[HTTP_SESSION_INFO_START_STATE].err_code = TR143_Error_Internal;
	session_info[HTTP_SESSION_INFO_BOMTIME].state = SPTEST_TASK_STATE_RUN_FAILED;
end:
	if (session_info != NULL)
		session_info[HTTP_SESSION_INFO_RESULTS].state = SPTEST_TASK_STATE_CLOSING;
	complete_all(&config->notify_state);
	LOG_DEBUG(THREAD_TAG"[%d] is stopping", id);
	if (http_resp_struct) {
		kfree(http_resp_struct);
		http_resp_struct = NULL;
	}
	/* Addition of extra parameter to full_loading struture needs to be handled carefully */
	if ((ret < 0) || (atomic_read(&full_load.http_connected) <= 0)) {
		memset(&full_load, 0x0, sizeof(struct full_loading));
	}
	return SUCCESS;
}
