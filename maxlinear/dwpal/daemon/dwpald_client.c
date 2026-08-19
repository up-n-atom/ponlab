/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "dwpald_client.h"
#include "dwpal_daemon.h"
#include "dwpal_os.h"
#include "wave_ipc_client.h"
#include "linked_list.h"
#include "logs.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#if defined YOCTO
#include <slibc/string.h>
#include <slibc/stdio.h>
#else
#include <stddef.h>
#include "libsafec/safe_str_lib.h"
#include "libsafec/safe_mem_lib.h"
#endif

static char *server_name = DWPALD_SERVER_NAME;

#define OPCODE_SIZE 64
#define MAX_NUM_CB_EVENTS 64

typedef struct _dwpald_hostap_attachment {
	char ifname[IFNAMSIZ + 1];
	hostap_event_clb common_event;
	/* list of dwpald_hostap_event_with_id */
	l_list *hostap_events;
	uint8_t state;
} dwpald_hostap_attachment;

typedef struct _dwpald_hostap_clb_id {
	hostap_event_clb hapd_clb;
	unsigned int id;
} dwpald_hostap_clb_id;

typedef struct _dwpald_hostap_event_with_id {
	char *op_code;
	uint8_t op_code_len;
	/* list of dwpald_hostap_clb_id */
	l_list *dwpald_hostap_clb_id_list;
} dwpald_hostap_event_with_id;

typedef struct _dwpald_nl_event_clb_id {
	nl80211_event_clb nl_event_id;
	unsigned int id;
} dwpald_nl_event_clb_id;

typedef struct _dwpald_drv_clb_id {
	driver_nl_event_clb drv_clb;
	unsigned int id;
} dwpald_drv_clb_id;

typedef struct _dwpald_driver_event_with_id {
	uint32_t nl_id;
	/* list of dwpald_drv_clb_id */
	l_list *drv_clb_id_list;
} dwpald_driver_nl_event_with_id;

typedef struct _dwpald_drv_nl_attachment {
	l_list *drv_events;    /* list of dwpald_driver_nl_event_with_id */
	l_list *nl_event_cb;   /* list of dwpald_nl_event_clb_id */
} dwpald_drv_nl_attachment;

typedef struct _dwpald_connection {
	wv_ipclient *client_handle;
	int listener_running;

	/* list of dwpald_hostap_attachment */
	l_list *hostap_attachments;
	pthread_mutex_t hap_attach_lock;

	dwpald_drv_nl_attachment *drv_nl_attch;
	pthread_mutex_t drv_nl_attach_lock;

	int nl80211_id;

	termination_cond term_cond;
} dwpald_connection;

static dwpald_connection *dwpald_conn = NULL;

/* "free" API */

static inline void free_hostap_clb_id(dwpald_hostap_clb_id *obj)
{
	free(obj);
}

static void free_hostap_event_with_id(dwpald_hostap_event_with_id *obj)
{
	if (obj) {
		free(obj->op_code);
		list_delete_all(obj->dwpald_hostap_clb_id_list, free_hostap_clb_id, dwpald_hostap_clb_id);
		list_free(obj->dwpald_hostap_clb_id_list);
		free(obj);
	}
}

static inline void free_drv_clb_with_id (dwpald_drv_clb_id *obj)
{
	free(obj);
}

static void free_driver_nl_event_with_id(dwpald_driver_nl_event_with_id *obj)
{
	if (obj) {
		list_delete_all(obj->drv_clb_id_list, free_drv_clb_with_id, dwpald_drv_clb_id);
		list_free(obj->drv_clb_id_list);
		free(obj);
	}
}

static inline void free_nl_event_clb_id(dwpald_nl_event_clb_id *obj)
{
	free(obj);
}

static void free_hostap_attachment(dwpald_hostap_attachment *obj)
{
	if (obj) {
		list_delete_all(obj->hostap_events, free_hostap_event_with_id, dwpald_hostap_event_with_id);
		list_free(obj->hostap_events);
		free(obj);
	}
}

static void free_drv_nl_attachment(dwpald_drv_nl_attachment *obj)
{
	if (obj) {
		list_delete_all(obj->drv_events, free_driver_nl_event_with_id, dwpald_driver_nl_event_with_id);
		list_free(obj->drv_events);
		list_delete_all(obj->nl_event_cb, free_nl_event_clb_id, dwpald_nl_event_clb_id);
		list_free(obj->nl_event_cb);
		free(obj);
	}
}

inline static dwpald_ret dwpald_ret_from_dwpal_ret(DWPAL_Ret dpal_ret) {
	if (dpal_ret == DWPAL_FAILURE) return DWPALD_DWPAL_FAILURE;
	else if (dpal_ret == DWPAL_INTERFACE_IS_DOWN) return DPWALD_DWPAL_IFACE_IS_DOWN;
	else return DWPALD_ERROR;
}

/* Send 'DWPALD_ATTACH' request to the server. Not locked with mutex */
static dwpald_ret dwpald_send_attach_cmd(const char *reg_request, size_t req_len,
					 uint8_t if_type, wv_ipc_msg **resp,
					 dwpald_header *resp_hdr)
{
	dwpald_header hdr = { 0 };
	wv_ipc_msg *reg_cmd;
	wv_ipc_ret ret;
	dwpald_ret res = DWPALD_ERROR;

	/* Cleanup response */
	*resp = NULL;
	memset(resp_hdr, 0, sizeof(*resp_hdr));

	/* Fill message header */
	hdr.header[0] = DWPALD_ATTACH_REQ;
	hdr.header[1] = if_type;

	/* Create message */
	if ((reg_cmd = wave_ipc_msg_alloc()) == NULL)
		return DWPALD_ERROR;

	/* Push header and data; send message */
	dwpald_header_push(reg_cmd, &hdr);
	wave_ipc_msg_fill_data(reg_cmd, reg_request, req_len);
	ret = wave_ipcc_send_cmd(dwpald_conn->client_handle, reg_cmd, resp);
	wave_ipc_msg_put(reg_cmd);

	switch (ret) {
		case WAVE_IPC_ERROR:
			ELOG("wave_ipcc_send_cmd() returned error");
			break;

		case WAVE_IPC_SUCCESS:
			if (dwpald_header_pop(*resp, resp_hdr) ||
				resp_hdr->header[0] != DWPALD_ATTACH_RESP ||
				resp_hdr->header[1] != if_type) {
				BUG("header error");
				break;
			}
			/* The ONLY case when result is SUCCESS and *resp is not NULL */
			LOG(2, "succesfully attached");
			return DWPALD_SUCCESS;

		case WAVE_IPC_CMD_FAILED:
			ELOG("dwpald_daemon could not process our command");
			break;

		default:
			ELOG("disconnected");
			res = DWPALD_DISCONNECTED;
			break;
	}

	if (*resp) {
		wave_ipc_msg_put(*resp);
		*resp = NULL;
	}
	return res;
}

/* Send 'DWPALD_UPDATE' request to the server. Not locked with mutex */
static dwpald_ret dwpald_send_update_event_cmd(char *reg_request, size_t req_len,
					 uint8_t if_type, wv_ipc_msg **resp,
					 dwpald_header *resp_hdr)
{
	dwpald_header hdr = { 0 };
	wv_ipc_msg *reg_cmd;
	wv_ipc_ret ret;
	dwpald_ret res = DWPALD_ERROR;

	/* Cleanup response */
	*resp = NULL;
	memset(resp_hdr, 0, sizeof(*resp_hdr));

	/* Fill message header */
	hdr.header[0] = DWPALD_UPDATE_EVENT_REQ;
	hdr.header[1] = if_type;

	/* Create message */
	if ((reg_cmd = wave_ipc_msg_alloc()) == NULL)
		return DWPALD_ERROR;

	/* Push header and data; send message */
	dwpald_header_push(reg_cmd, &hdr);
	wave_ipc_msg_fill_data(reg_cmd, reg_request, req_len);
	ret = wave_ipcc_send_cmd(dwpald_conn->client_handle, reg_cmd, resp);
	wave_ipc_msg_put(reg_cmd);

	switch (ret) {
		case WAVE_IPC_ERROR:
			ELOG("wave_ipcc_send_cmd() returned error");
			break;

		case WAVE_IPC_SUCCESS:
			if (dwpald_header_pop(*resp, resp_hdr) ||
				resp_hdr->header[0] != DWPALD_UPDATE_EVENT_RESP ||
				resp_hdr->header[1] != if_type) {
				BUG("header error");
				break;
			}
			/* The ONLY case when result is SUCCESS and *resp is not NULL */
			LOG(2, "succesfully updated");
			return DWPALD_SUCCESS;

		case WAVE_IPC_CMD_FAILED:
			ELOG("dwpald_daemon could not process our command");
			break;

		default:
			ELOG("disconnected");
			res = DWPALD_DISCONNECTED;
			break;
	}

	if (*resp) {
		wave_ipc_msg_put(*resp);
		*resp = NULL;
	}
	return res;
}

static dwpald_ret dwpald_send_hostap_attach(dwpald_hostap_attachment* hap_attach)
{
	char *reg_request;
	size_t req_len = 0, i = 0, written = 0;
	wv_ipc_msg *reply = NULL;
	dwpald_ret ret;
	dwpald_header resp_hdr;

	req_len += IFNAMSIZ + 1;

	/* calculate request length */
	list_foreach_start(hap_attach->hostap_events, hap_event, dwpald_hostap_event_with_id)
		req_len += hap_event->op_code_len + 1;
		i++;
	list_foreach_end
	req_len += 1;

	if (i != list_get_size(hap_attach->hostap_events)) {
		BUG("i=%zu, list_size=%zu", i, list_get_size(hap_attach->hostap_events));
		return DWPALD_ERROR;
	}

	reg_request = calloc(req_len, sizeof(char));
	if (!reg_request)
		return DWPALD_ERROR;

	// hostap_register_sta_to_events expect last byte to be zero to terminate registration,
	// therefore need to zero array (special case WPS_PBC, WPS_CANCEL, WPS_PIN)

	strncpy_s(reg_request, req_len, hap_attach->ifname, IFNAMSIZ);
	written = IFNAMSIZ + 1;
	list_foreach_start(hap_attach->hostap_events, hap_event, dwpald_hostap_event_with_id)
		reg_request[written++] = hap_event->op_code_len;
		written += sprintf_s(reg_request + written, req_len - written,
				    "%s", hap_event->op_code);
	list_foreach_end

	ret = dwpald_send_attach_cmd(reg_request, req_len,
				     DWPALD_IF_TYPE_HOSTAP, &reply, &resp_hdr);
	free(reg_request);

	if (ret == DWPALD_SUCCESS) {
		hap_attach->state = resp_hdr.header[2];
		wave_ipc_msg_put(reply);

		LOG(2, "hap state of %s is %hhu", hap_attach->ifname, hap_attach->state);
	}

	return ret;
}

static dwpald_ret dwpald_send_drv_nl_update_event(l_list *drv_events)
{
	uint32_t *reg_request;
	char *update_event_request;
	size_t update_event_req_len, req_len, i = 0;
	wv_ipc_msg *reply = NULL;
	dwpald_ret ret;
	dwpald_header resp_hdr;

	req_len = list_get_size(drv_events) * sizeof(uint32_t);
	reg_request = calloc(req_len, sizeof(char));
	if (!reg_request)
		return DWPALD_ERROR;

	update_event_req_len = IFNAMSIZ + 1 + req_len;
	update_event_request = calloc(update_event_req_len, sizeof(char));
	if (!update_event_request) {
		free(reg_request);
		return DWPALD_ERROR;
	}

	strcpy_s(update_event_request, IFNAMSIZ + 1,
		  DWPALD_NL_DRV_IFNAME);
	list_foreach_start(drv_events, drv_event, dwpald_driver_nl_event_with_id)
		reg_request[i] = drv_event->nl_id;
		i++;
	list_foreach_end

	if (i != list_get_size(drv_events)) {
		BUG("i=%zu, list_size=%zu", i, list_get_size(drv_events));
		free(update_event_request);
		free(reg_request);
		return DWPALD_ERROR;
	}

	memcpy_s(&update_event_request[IFNAMSIZ + 1], req_len, reg_request, req_len);
	free(reg_request);

	ret = dwpald_send_update_event_cmd(update_event_request, update_event_req_len,
				     DWPALD_IF_TYPE_KERNEL, &reply, &resp_hdr);
	free(update_event_request);
	if (ret == DWPALD_SUCCESS) {
		dwpald_conn->nl80211_id = resp_hdr.header[2];
		LOG(2, "nl80211_id = %d", dwpald_conn->nl80211_id);
		wave_ipc_msg_put(reply);
	}

	return ret;
}


static dwpald_ret dwpald_send_hostap_update_event(const char *ifname,
					l_list *hap_events)
{
	char *reg_request;
	size_t req_len = 0, i = 0, written = 0;
	wv_ipc_msg *reply = NULL;
	dwpald_ret ret;
	dwpald_header resp_hdr;

	req_len += IFNAMSIZ + 1;

	list_foreach_start(hap_events, hap_event, dwpald_hostap_event_with_id)
		req_len += hap_event->op_code_len + 1;
		i++;
	list_foreach_end
	req_len += 1;

	if (i != list_get_size(hap_events)) {
		BUG("i=%zu, list_size=%zu", i, list_get_size(hap_events));
		return DWPALD_ERROR;
	}

	reg_request = calloc(req_len, sizeof(char));
	if (!reg_request)
		return DWPALD_ERROR;

	// hostap_register_sta_to_events expect last byte to be zero to terminate registration,
	// therefore need to zero array (special case WPS_PBC, WPS_CANCEL, WPS_PIN)

	strncpy_s(reg_request, req_len, ifname, IFNAMSIZ);
	written = IFNAMSIZ + 1;
	list_foreach_start(hap_events, hap_event, dwpald_hostap_event_with_id)
		reg_request[written++] = hap_event->op_code_len;
		written += sprintf_s(reg_request + written, req_len - written,
				    "%s", hap_event->op_code);
	list_foreach_end

	ret = dwpald_send_update_event_cmd(reg_request, req_len,
				     DWPALD_IF_TYPE_HOSTAP, &reply, &resp_hdr);
	free(reg_request);
	if (ret == DWPALD_SUCCESS) {
		wave_ipc_msg_put(reply);
	}

	return ret;
}

/* Send 'HOSTAP DETACH' request to the server. Not locked with mutex */
static dwpald_ret dwpald_send_hostap_detach(const char *ifname)
{
	dwpald_header hdr = { 0 };
	dwpald_header resp_hdr = { 0 };
	wv_ipc_msg *cmd, *resp = NULL;
	wv_ipc_ret ret;
	dwpald_ret res = DWPALD_ERROR;
	char *data;

	/* Fill message header */
	hdr.header[0] = DWPALD_DETACH_REQ;
	hdr.header[1] = DWPALD_IF_TYPE_HOSTAP;

	/* Create message */
	if ((cmd = wave_ipc_msg_alloc()) == NULL)
		return DWPALD_ERROR;

	/* Put ifname to 'attach' message */
	wave_ipc_msg_reserve_data(cmd, IF_NAMESIZE + 1);
	data = wave_ipc_msg_get_data(cmd);
	strncpy_s(data, IF_NAMESIZE + 1, ifname, IF_NAMESIZE);

	/* Push header and send message */
	dwpald_header_push(cmd, &hdr);
	ret = wave_ipcc_send_cmd(dwpald_conn->client_handle, cmd, &resp);
	wave_ipc_msg_put(cmd);

	switch (ret) {
		case WAVE_IPC_ERROR:
			ELOG("wave_ipcc_send_cmd() returned error");
			break;

		case WAVE_IPC_SUCCESS:
			/* resp != NULL */
			if (dwpald_header_pop(resp, &resp_hdr) ||
				resp_hdr.header[0] != DWPALD_DETACH_RESP ||
				resp_hdr.header[1] != DWPALD_IF_TYPE_HOSTAP) {
				BUG("header error");
				break;
			}
			LOG(2, "succesfully detached");
			res = DWPALD_SUCCESS;
			break;

		case WAVE_IPC_CMD_FAILED:
			ELOG("dwpald_daemon could not process our command");
			break;

		default:
			ELOG("disconnected");
			res = DWPALD_DISCONNECTED;
			break;
	}
	/* Cleanup response */
	if (resp) wave_ipc_msg_put(resp);
	return res;
}

static dwpald_ret dwpald_send_drv_nl_attach(l_list *drv_events)
{
	uint32_t *reg_request;
	char *attach_request;
	size_t attach_req_len, req_len, i = 0;
	wv_ipc_msg *reply = NULL;
	dwpald_ret ret;
	dwpald_header resp_hdr;

	req_len = list_get_size(drv_events) * sizeof(uint32_t);
	reg_request = calloc(req_len, sizeof(char));
	if (!reg_request)
		return DWPALD_ERROR;

	attach_req_len = IFNAMSIZ + 1 + req_len;
	attach_request = calloc(attach_req_len, sizeof(char));
	if (!attach_request) {
		free(reg_request);
		return DWPALD_ERROR;
	}

	strncpy_s(attach_request, IFNAMSIZ + 1,
		  DWPALD_NL_DRV_IFNAME, sizeof(DWPALD_NL_DRV_IFNAME) - 1);
	list_foreach_start(drv_events, drv_event, dwpald_driver_nl_event_with_id)
		reg_request[i] = drv_event->nl_id;
		i++;
	list_foreach_end

	if (i != list_get_size(drv_events)) {
		BUG("i=%zu, list_size=%zu", i, list_get_size(drv_events));
		free(attach_request);
		free(reg_request);
		return DWPALD_ERROR;
	}

	memcpy_s(&attach_request[IFNAMSIZ + 1], req_len, reg_request, req_len);
	free(reg_request);
	ret = dwpald_send_attach_cmd(attach_request, attach_req_len,
				     DWPALD_IF_TYPE_KERNEL, &reply, &resp_hdr);
	free(attach_request);
	if (ret == DWPALD_SUCCESS) {
		dwpald_conn->nl80211_id = resp_hdr.header[2];
		LOG(2, "nl80211_id = %d", dwpald_conn->nl80211_id);
		wave_ipc_msg_put(reply);
	}

	return ret;
}

static void dwpald_free_hostap_events_with_id(l_list * hap_events, unsigned int id)
{
	list_foreach_start(hap_events, events, dwpald_hostap_event_with_id) // loop through event
		list_foreach_start(events->dwpald_hostap_clb_id_list, clb_id, dwpald_hostap_clb_id) // loop through clb registered
			if (clb_id->id == id) {
				/* found id, remove clb */
				// DLOG("Removing callback of event %s for id %u",events->op_code, id);
				free_hostap_clb_id(clb_id);
				list_foreach_remove_current_entry()
			}
		list_foreach_end

		if (0 == list_get_size(events->dwpald_hostap_clb_id_list)) {
			/* if no clb associated with event, then remove it */
			// DLOG("Removing event %s for id %u",events->op_code, id);
			free_hostap_event_with_id(events);
			list_foreach_remove_current_entry()
		}
	list_foreach_end
}

static int dwpald_push_drv_callback(l_list *l, driver_nl_event_clb clb, unsigned int id)
{
	dwpald_drv_clb_id *new_cb = NULL;
	if (!clb)
		return 0;
	new_cb = calloc(1, sizeof(dwpald_drv_clb_id));
	if (new_cb == NULL)
		return -1;
	new_cb->id = id;
	new_cb->drv_clb = clb;
	if (list_push_back(l, new_cb)) {
		BUG("list push error");
		free(new_cb);
		return -1;
	}
	return 0;
}

static int dwpald_push_kernel_callback(l_list *l, nl80211_event_clb clb, unsigned int id)
{
	dwpald_nl_event_clb_id *new_cb = NULL;
	if (!clb)
		return 0;
	new_cb = calloc(1, sizeof(dwpald_nl_event_clb_id));
	if (new_cb == NULL)
		return -1;
	new_cb->id = id;
	new_cb->nl_event_id = clb;
	if (list_push_back(l, new_cb)) {
		BUG("list push error");
		free(new_cb);
		return -1;
	}
	return 0;
}

static int dwpald_push_hostap_callback(l_list *l, hostap_event_clb clb, unsigned int id)
{
	dwpald_hostap_clb_id *new_cb = NULL;
	new_cb = calloc(1, sizeof(dwpald_hostap_clb_id));
	if (new_cb == NULL)
		return -1;
	new_cb->id = id;
	new_cb->hapd_clb = clb;
	if (list_push_back(l, new_cb)) {
		BUG("list push error");
		free(new_cb);
		return -1;
	}
	return 0;
}

static int dwpald_copy_hostap_events(l_list * hap_events_dst, size_t num_hap_events,
				     const dwpald_hostap_event hostap_events[], unsigned int id)
{
	size_t i, ret;
	dwpald_hostap_event_with_id *cpy;

	for (i = 0; i < num_hap_events; i++) {
		if (!hostap_events[i].op_code_len ||
		    !hostap_events[i].op_code ||
		    !hostap_events[i].hapd_clb) {
			ELOG("bad arguments at idx=%zu", i);
			goto err;
		}

		cpy = (dwpald_hostap_event_with_id*)calloc(1, sizeof(dwpald_hostap_event_with_id));
		if (!cpy)
			goto err;

		cpy->op_code = (char*)calloc(hostap_events[i].op_code_len + 1, sizeof(char));
		if (!cpy->op_code) {
			free_hostap_event_with_id(cpy);
			goto err;
		}

		cpy->op_code_len = hostap_events[i].op_code_len;
		strncpy_s(cpy->op_code, hostap_events[i].op_code_len + 1,
			  hostap_events[i].op_code, cpy->op_code_len);

		cpy->dwpald_hostap_clb_id_list = list_init();
		if (!cpy->dwpald_hostap_clb_id_list) {
			BUG("list init error");
			free_hostap_event_with_id(cpy);
			goto err;
		}

		ret = dwpald_push_hostap_callback(cpy->dwpald_hostap_clb_id_list, hostap_events[i].hapd_clb, id);
		if (!ret) {
			ret = list_push_front(hap_events_dst, cpy);
		}
		if (ret) {
			BUG("list push error");
			free_hostap_event_with_id(cpy);
			goto err;
		}
	}

	return 0;

err:
	dwpald_free_hostap_events_with_id(hap_events_dst, id);
	return 1;
}

static void dwpald_free_driver_events_with_id(l_list *drv_events, unsigned int id)
{
	list_foreach_start(drv_events, drv_event, dwpald_driver_nl_event_with_id) // loop through event
		list_foreach_start(drv_event->drv_clb_id_list, clb_id, dwpald_drv_clb_id) // loop through clb registered
			if (clb_id->id == id) {
				/* found id, remove clb */
				// DLOG("Removing callback of event %s for id %u",events->op_code, id);
				free_drv_clb_with_id(clb_id);
				list_foreach_remove_current_entry()
			}
		list_foreach_end

		if (0 == list_get_size(drv_event->drv_clb_id_list)) {
			/* if no clb associated with event, then remove it */
			// DLOG("Removing event %s for id %u",events->op_code, id);
			free_driver_nl_event_with_id(drv_event);
			list_foreach_remove_current_entry()
		}
	list_foreach_end
}

static int dwpald_copy_driver_events(l_list *drv_events_dst, size_t num_drv_events,
				     const dwpald_driver_nl_event *driver_events, unsigned int id)
{
	size_t i;
	dwpald_driver_nl_event_with_id *cpy;

	for (i = 0; i < num_drv_events; i++) {
		if (!driver_events[i].drv_clb) {
			ELOG("bad arguments at idx=%zu", i);
			goto err;
		}

		cpy = (dwpald_driver_nl_event_with_id *)calloc(1, sizeof(dwpald_driver_nl_event_with_id));
		if (cpy == NULL)
			goto err;

		cpy->nl_id = driver_events[i].nl_id;
		cpy->drv_clb_id_list = list_init();
		if (!cpy->drv_clb_id_list) {
			BUG("list init error");
			free_driver_nl_event_with_id(cpy);
			goto err;
		}

		if (dwpald_push_drv_callback(cpy->drv_clb_id_list, driver_events[i].drv_clb, id)) {
			free_driver_nl_event_with_id(cpy);
			goto err;
		}

		if (list_push_front(drv_events_dst, cpy)) {
			BUG("list push error");
			free_driver_nl_event_with_id(cpy);
			goto err;
		}
	}

	return 0;
err:
	dwpald_free_driver_events_with_id(drv_events_dst, id);
	return 1;
}

static inline size_t copy_hostap_event(dwpald_hostap_attachment* hap_attach, dwpald_hostap_event_with_id* event,
				    hostap_event_clb *callback, size_t max_num_events)
{
	size_t num_events = 0;

	if (!event || !max_num_events)
		return 0;

	/* This event can be used as a common handler for all registered events to execute a common code.
		NOTE: must be called BEFORE event */
	if (hap_attach->common_event)
		callback[num_events++] = hap_attach->common_event;

	list_foreach_start(event->dwpald_hostap_clb_id_list, clb, dwpald_hostap_clb_id)
		if (!clb->hapd_clb)
			continue;

		if (num_events >= max_num_events) {
			BUG("Reached limit for number of supported hostap event callbacks");
			break;
		}

		callback[num_events++] = clb->hapd_clb;
	list_foreach_end

	return num_events;
}

static int dwpald_receive_hostap_event(wv_ipc_msg *event, dwpald_header *hdr)
{
	char ifname[IFNAMSIZ + 1] = { 0 };
	char op_code[OPCODE_SIZE] = { 0 };
	char *event_msg = NULL;
	uint16_t event_msg_len = wv_aligned_16_bit_fetch(&hdr->header[4]);
	char *event_data = wave_ipc_msg_get_data(event);
	size_t event_data_size = wave_ipc_msg_get_size(event);
	int found = 0, intf_event = 0;
	size_t i, num_events = 0;
	hostap_event_clb event_callback[MAX_NUM_CB_EVENTS];

	if (event_data_size != (size_t)(hdr->header[2] + hdr->header[3] + event_msg_len)) {
		BUG("event_data_size=%zu, hdr[2]=%hhu hdr[3]=%hhu event_msg_len=%hu",
		    event_data_size, hdr->header[2], hdr->header[3], event_msg_len);
		return 1;
	}

	if (hdr->header[2] > IFNAMSIZ) {
		BUG("hdr[2]=(%hhu) > %d", hdr->header[2], IFNAMSIZ);
		return 1;
	}

	if (hdr->header[3] == 0 || hdr->header[3] > sizeof(op_code) - 1) {
		BUG("hdr[3]=(%hhu)", hdr->header[3]);
		return 1;
	}

	if (hdr->header[2]) {
		memcpy_s(ifname, sizeof(ifname), event_data, hdr->header[2]);
		ifname[hdr->header[2]] = '\0';
	}
	event_data += hdr->header[2];

	memcpy_s(op_code, sizeof(op_code), event_data, hdr->header[3]);
	op_code[hdr->header[3]] = '\0';

	if (event_msg_len) {
		event_msg = event_data + hdr->header[3];
		event_msg_len--; /* len should not include '\n' at the end */
	}

	MUTEX_LOCK(&dwpald_conn->hap_attach_lock);
	list_foreach_start(dwpald_conn->hostap_attachments, hap_attch, dwpald_hostap_attachment)
		if (strncmp(hap_attch->ifname, ifname, sizeof(ifname)))
			continue;

		if (!strncmp(op_code, "INTERFACE_CONNECTED_OK", sizeof("INTERFACE_CONNECTED_OK") - 1)) {
			hap_attch->state = INTERFACE_DWPAL_STATE_CONNECTED;
			intf_event = 1;
		}
		else if (!strncmp(op_code, "INTERFACE_RECONNECTED_OK", sizeof("INTERFACE_RECONNECTED_OK") - 1)) {
			hap_attch->state = INTERFACE_DWPAL_STATE_CONNECTED;
			intf_event = 1;
		}
		else if (!strncmp(op_code, "INTERFACE_DISCONNECTED", sizeof("INTERFACE_DISCONNECTED") - 1)) {
			hap_attch->state = INTERFACE_DWPAL_STATE_DISCONNECTED;
			intf_event = 1;
		}

		list_foreach_start(hap_attch->hostap_events, hap_event, dwpald_hostap_event_with_id)
			if (strncmp(hap_event->op_code, op_code, sizeof(op_code)))
				continue;

			num_events = copy_hostap_event(hap_attch, hap_event, event_callback, MAX_NUM_CB_EVENTS);
			found = 1;
			break;
		list_foreach_end

		break;
	list_foreach_end
	MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);

	/* Call events, should not be called under mutex to avoid deadlocks */
	for (i = 0; i < num_events; i++)
		event_callback[i](ifname, op_code, event_msg, event_msg_len);

	/* INTERFACE_* events can be generated by dwpal/dwpald regardless of the client's subscription state,
	   so don't report an error if no handler is registered. */
	if (!found && !intf_event) {
		LOG(2, "Received event '%s' not registered to", op_code);
		return 1;
	}

	return 0;
}

static int dwpald_receive_drv_event(wv_ipc_msg *event, dwpald_header *hdr)
{
	char ifname[IFNAMSIZ + 1] = { 0 };
	char *event_data = wave_ipc_msg_get_data(event);
	size_t event_data_size = wave_ipc_msg_get_size(event);
	uint16_t data_size = wv_aligned_16_bit_fetch(&hdr->header[3]);
	int event_id;
	int found = 0;
	int i, num_events = 0;
	driver_nl_event_clb event_callback[MAX_NUM_CB_EVENTS];

	if (event_data == NULL) {
		BUG("event_data is NULL");
		return 1;
	}

	if (event_data_size != sizeof(int) + hdr->header[2] + data_size) {
		BUG("event_data_size=%zu, hdr[2]=%hhu data_size=%hu",
		    event_data_size, hdr->header[2], data_size);
		return 1;
	}

	if (hdr->header[2] > IFNAMSIZ) {
		BUG("hdr[2]=(%hhu) > %d", hdr->header[2], IFNAMSIZ);
		return 1;
	}

	event_id = *((int*)event_data);
	event_data += sizeof(int);

	if (hdr->header[2])
		memcpy_s(ifname, sizeof(ifname), event_data, hdr->header[2]);

	event_data += hdr->header[2];

	MUTEX_LOCK(&dwpald_conn->drv_nl_attach_lock);
	if (!dwpald_conn->drv_nl_attch) {
		BUG("received drv event wile not attached");
		MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
		return 1;
	}

	list_foreach_start(dwpald_conn->drv_nl_attch->drv_events, drv_event, dwpald_driver_nl_event_with_id)
		if (drv_event->nl_id != (uint32_t)event_id)
			continue;

		list_foreach_start(drv_event->drv_clb_id_list, drv_clb, dwpald_drv_clb_id)
			if (num_events >= MAX_NUM_CB_EVENTS) {
				BUG("Reached limit for number of supported drv event callbacks");
				break;
			}
			event_callback[num_events++] = drv_clb->drv_clb;
		list_foreach_end
		found = 1;
		break;
	list_foreach_end
	MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);

	/* Call events, should not be called under mutex to avoid deadlocks */
	for (i = 0; i < num_events; i++)
		event_callback[i](ifname, event_id, event_data, data_size);

	if (!found) {
		LOG(2, "Received event '%d' not registered to", event_id);
		return 1;
	}

	return 0;
}

static int dwpald_receive_kernel_event(wv_ipc_msg *event)
{
	struct nl_msg *msg = dwpald_nl_msg_from_ipc_msg(event);
	int i, num_events = 0;
	nl80211_event_clb event_callback[MAX_NUM_CB_EVENTS];

	if (msg == NULL) {
		ELOG("got NULL nl_msg");
		return 1;
	}

	MUTEX_LOCK(&dwpald_conn->drv_nl_attach_lock);
	if (!dwpald_conn->drv_nl_attch) {
		BUG("received nl event while not attached");
		MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
		nlmsg_free(msg);
		return 1;
	}

	if (dwpald_conn->drv_nl_attch->nl_event_cb)
	{
		list_foreach_start(dwpald_conn->drv_nl_attch->nl_event_cb, clb, dwpald_nl_event_clb_id)
			if (num_events >= MAX_NUM_CB_EVENTS) {
				BUG("Reached limit for number of supported kernel event callbacks");
				break;
			}
			event_callback[num_events++] = clb->nl_event_id;
		list_foreach_end
	}
	MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);

	/* Call events, should not be called under mutex to avoid deadlocks */
	for (i = 0; i < num_events; i++)
		event_callback[i](msg);

	nlmsg_free(msg);
	return 0;
}

static int dwpald_event(void *arg, wv_ipc_msg *event)
{
	dwpald_header event_hdr;

	if ((dwpald_connection*)arg != dwpald_conn) {
		BUG("arg != dwpald_conn");
		return 1;
	}

	if (dwpald_header_pop(event, &event_hdr) ||
	    event_hdr.header[0] != DWPALD_EVENT) {
		BUG("ipc msg pop header failed");
		return 1;
	}

	switch (event_hdr.header[1]) {
	case DWPALD_IF_TYPE_HOSTAP:
		return dwpald_receive_hostap_event(event, &event_hdr);
		break;
	case DWPALD_IF_TYPE_DRIVER:
		return dwpald_receive_drv_event(event, &event_hdr);
		break;
	case DWPALD_IF_TYPE_KERNEL:
		return dwpald_receive_kernel_event(event);
		break;
	default:
		BUG("unkown iface type: %hhu", event_hdr.header[1]);
		return 1;
		break;
	}
}

/* Put emulated event to serializer */
static int dwpald_emulate_event(wv_ipc_sync_mode mode, const char* ifname, const char* op_code, const char *msg, size_t msg_len)
{
	wv_ipc_msg *e_msg;
	dwpald_header hdr = { 0 };

	char *event_data;
	size_t total_msg_size = 0, reserve_size;
	uint16_t msg_len_16bit;

	if (ifname == NULL || op_code == NULL || (msg == NULL && msg_len))
		return 1;

	LOG(2, "push hostap event '%s' to iface '%s' (len=%zu)", op_code, ifname, msg_len);

	e_msg = wave_ipc_msg_alloc();
	if (e_msg == NULL)
		return 1;

	if (msg && msg_len)
		msg_len++; /* for '\0' */
	msg_len_16bit = (uint16_t)msg_len;
	if ((size_t)msg_len_16bit != msg_len)
		return DWPAL_FAILURE;

	hdr.header[0] = DWPALD_EVENT;
	hdr.header[1] = DWPALD_IF_TYPE_HOSTAP;
	hdr.header[2] = strnlen_s(ifname, IFNAMSIZ);
	hdr.header[3] = strnlen_s(op_code, OPCODE_SIZE);
	wv_aligned_16_bit_assign(&hdr.header[4], msg_len_16bit);
	dwpald_header_push(e_msg, &hdr);

	reserve_size = hdr.header[2] + hdr.header[3] + msg_len_16bit;
	if (wave_ipc_msg_reserve_data(e_msg, reserve_size) != WAVE_IPC_SUCCESS) {
		BUG("could not reserve %zu data in ipc msg", reserve_size);
		wave_ipc_msg_put(e_msg);
		return 1;
	}

	if ((event_data = wave_ipc_msg_get_data(e_msg)) == NULL) {
		wave_ipc_msg_put(e_msg);
		return 1;
	}

	memcpy_s(event_data, reserve_size, ifname, hdr.header[2]);
	total_msg_size += hdr.header[2];

	memcpy_s(event_data + total_msg_size, reserve_size - total_msg_size,
			op_code, hdr.header[3]);
	total_msg_size += hdr.header[3];

	if (msg && msg_len_16bit) {
		memcpy_s(event_data + total_msg_size, reserve_size - total_msg_size,
			msg, msg_len_16bit - 1);
		event_data[total_msg_size + (msg_len_16bit - 1)] = '\0';
	}

	if (reserve_size != total_msg_size + msg_len_16bit) {
		BUG("resrv = %zu, placed 0 at %zu", reserve_size, (total_msg_size + msg_len_16bit));
	}

	return (WAVE_IPC_SUCCESS == wave_ipcc_emulate_event(dwpald_conn->client_handle, mode, e_msg) ? 0 : 1);
}

static int dwpald_reconnected(void *arg)
{
	if ((dwpald_connection*)arg != dwpald_conn) {
		BUG("arg != dwpald_conn");
		return 1;
	}

	MUTEX_LOCK(&dwpald_conn->hap_attach_lock);
	list_foreach_start(dwpald_conn->hostap_attachments, hap_attch, dwpald_hostap_attachment)
		if (dwpald_send_hostap_attach(hap_attch) != DWPALD_SUCCESS) {
			ELOG("attach cmd for hostap iface %s returned error", hap_attch->ifname);
			MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);
			return 1;
		}

		if (hap_attch->state == INTERFACE_DWPAL_STATE_CONNECTED) {
			/* push "INTERFACE_RECONNECTED_OK" into serializer */
			dwpald_emulate_event(WV_IPC_ASYNC, hap_attch->ifname, "INTERFACE_RECONNECTED_OK", NULL, 0);
		}
	list_foreach_end
	MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);

	MUTEX_LOCK(&dwpald_conn->drv_nl_attach_lock);
	if (!dwpald_conn->drv_nl_attch) {
		MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
		return 0;
	}

	if (dwpald_send_drv_nl_attach(dwpald_conn->drv_nl_attch->drv_events) != DWPALD_SUCCESS) {
		MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
		ELOG("attach cmd for drv nl returned error");
		return 1;
	}
	MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);

	return 0;
}

static int dwpald_disconnected(void *arg)
{
	if ((dwpald_connection*)arg != dwpald_conn) {
		BUG("arg != dwpald_conn");
		return 1;
	}

	MUTEX_LOCK(&dwpald_conn->hap_attach_lock);
	list_foreach_start(dwpald_conn->hostap_attachments, hap_attch, dwpald_hostap_attachment)
		if (hap_attch->state == INTERFACE_DWPAL_STATE_CONNECTED) {
			hap_attch->state = INTERFACE_DWPAL_STATE_DISCONNECTED;
			dwpald_emulate_event(WV_IPC_ASYNC, hap_attch->ifname, "INTERFACE_DISCONNECTED", NULL, 0);
		}
	list_foreach_end
	MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);

	return 0;
}

static int dwpald_termination_cond(void *arg)
{
	if ((dwpald_connection*)arg != dwpald_conn) {
		BUG("arg != dwpald_conn");
		return 1;
	}

	return dwpald_conn->term_cond();
}

#ifdef CONFIG_DWPALD_DEBUG_TOOLS
void dwpald_unit_test_mode(void)
{
	server_name = DWPALD_SERVER_UTEST_NAME;
}

dwpald_ret dwpald_term_daemon(void)
{
	wv_ipc_msg *msg, *response = NULL;
	dwpald_header cmd_hdr = { 0 }, resp_hdr;
	wv_ipc_ret ipc_ret;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if ((msg = wave_ipc_msg_alloc()) == NULL)
		return DWPALD_ERROR;

	cmd_hdr.header[0] = DWPALD_TERMINATE_REQ;
	dwpald_header_push(msg, &cmd_hdr);

	ipc_ret = wave_ipcc_send_cmd(dwpald_conn->client_handle,
				     msg, &response);
	wave_ipc_msg_put(msg);
	if (ipc_ret != WAVE_IPC_SUCCESS) {
		ELOG("ipcc_send_cmd returned err (ret=%d)", ipc_ret);
		goto err;
	}

	if (dwpald_header_pop(response, &resp_hdr) ||
	    resp_hdr.header[0] != DWPALD_TERMINATE_RESP) {
		BUG("response header is corrupted");
		goto err;
	}
	wave_ipc_msg_put(response);
	return DWPALD_SUCCESS;

err:
	if (response)
		wave_ipc_msg_put(response);
	return DWPALD_ERROR;
}

dwpald_ret dwpald_get_connected_clients(char *reply, size_t *reply_len)
{
	wv_ipc_msg *msg, *response = NULL;
	dwpald_header cmd_hdr = { 0 }, resp_hdr;
	wv_ipc_ret ipc_ret;
	char *resp_data;
	size_t resp_data_size;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if ((msg = wave_ipc_msg_alloc()) == NULL)
		return DWPALD_ERROR;

	cmd_hdr.header[0] = DWPALD_CONNECTED_CLIENTS_REQ;
	dwpald_header_push(msg, &cmd_hdr);

	ipc_ret = wave_ipcc_send_cmd(dwpald_conn->client_handle,
				     msg, &response);
	wave_ipc_msg_put(msg);
	if (ipc_ret != WAVE_IPC_SUCCESS) {
		ELOG("ipcc_send_cmd returned err (ret=%d)", ipc_ret);
		goto err;
	}

	if (dwpald_header_pop(response, &resp_hdr) ||
	    resp_hdr.header[0] != DWPALD_CONNECTED_CLIENTS_RESP) {
		BUG("response header is corrupted");
		goto err;
	}
	resp_data = wave_ipc_msg_get_data(response);
	resp_data_size = wave_ipc_msg_get_size(response);
	if (*reply_len < resp_data_size) {
		ELOG("reply_len(%zu) < resp_data_size (%zu)", *reply_len, resp_data_size);
		goto err;
	}

	if (resp_data_size) {
		memcpy_s(reply, *reply_len, resp_data, resp_data_size);
		*reply_len = resp_data_size;
	} else {
		reply[0] = '\0';
		*reply_len = 0;
	}

	wave_ipc_msg_put(response);
	return DWPALD_SUCCESS;

err:
	if (response)
		wave_ipc_msg_put(response);
	return DWPALD_ERROR;
}
#endif

bool dwpald_connected(void)
{
	return (dwpald_conn != NULL);
}

/* Connect to dwpal daemon */
dwpald_ret dwpald_connect(const char *prog_name)
{
	dwpald_connection *conn;
	wv_ipc_ret conn_ret;

	if (dwpald_conn != NULL) {
		ELOG("dwpald client is already connected");
		return DWPALD_ERROR;
	}

	conn = (dwpald_connection*)calloc(1, sizeof(dwpald_connection));
	if (conn == NULL)
		return DWPALD_ERROR;

	conn->nl80211_id = -1;

	conn->hostap_attachments = list_init();
	if (!conn->hostap_attachments)
		goto err;

	conn_ret = wave_ipcc_connect(&conn->client_handle, prog_name, server_name);
	if (conn_ret == WAVE_IPC_ERROR) {
		ELOG("ipcc_connect() returned error");
		goto err;
	}

	DWPAL_CHECK_RET(pthread_mutex_init(&conn->hap_attach_lock, NULL));
	DWPAL_CHECK_RET(pthread_mutex_init(&conn->drv_nl_attach_lock, NULL));

	dwpald_conn = conn;

	return (conn_ret == WAVE_IPC_SUCCESS) ? DWPALD_SUCCESS : DWPALD_DISCONNECTED;

err:
	if (conn->hostap_attachments) {
		list_delete_all(conn->hostap_attachments, free_hostap_attachment, dwpald_hostap_attachment);
		list_free(conn->hostap_attachments);
	}
	free(conn);
	return DWPALD_ERROR;
}

dwpald_ret dwpald_disconnect(void)
{
	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}
	// DLOG("dwpald_disconnect");

	if (dwpald_conn->listener_running) {
		wave_ipcc_stop_listener(dwpald_conn->client_handle);
	}

	DWPAL_CHECK_RET(pthread_mutex_destroy(&dwpald_conn->drv_nl_attach_lock));
	DWPAL_CHECK_RET(pthread_mutex_destroy(&dwpald_conn->hap_attach_lock));

	wave_ipcc_disconnect(&dwpald_conn->client_handle);

	list_delete_all(dwpald_conn->hostap_attachments, free_hostap_attachment, dwpald_hostap_attachment);
	list_free(dwpald_conn->hostap_attachments);

	free_drv_nl_attachment(dwpald_conn->drv_nl_attch);

	free(dwpald_conn);
	dwpald_conn = NULL;

	return DWPALD_SUCCESS;
}

dwpald_ret dwpald_start_listener(void)
{
	wv_ipc_ret ret;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (dwpald_conn->listener_running)
		return DWPALD_SUCCESS;

	ret = wave_ipcc_start_listener(dwpald_conn->client_handle,
				       dwpald_event, NULL, dwpald_disconnected,
				       dwpald_reconnected, (void*)dwpald_conn, 1);

	if (ret == WAVE_IPC_ERROR) {
		ELOG("start_listener returned error");
		return DWPALD_ERROR;
	}

	dwpald_conn->listener_running = 1;

	return DWPALD_SUCCESS;
}

dwpald_ret dwpald_stop_listener(void)
{
    wv_ipc_ret ret;

    if (dwpald_conn == NULL) {
        ELOG("dwpald client is not connected");
        return DWPALD_ERROR;
    }

    if (dwpald_conn->listener_running == 0)
        return DWPALD_SUCCESS;

    ret = wave_ipcc_stop_listener(dwpald_conn->client_handle);

    if (ret == WAVE_IPC_ERROR) {
        ELOG("stop_listener returned error");
        return DWPALD_ERROR;
    }

    dwpald_conn->listener_running = 0;

    return DWPALD_SUCCESS;
}

dwpald_ret dwpald_start_blocked_listen(termination_cond term_cond)
{
	wv_ipc_ret ret;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	dwpald_conn->term_cond = term_cond;
	ret = wave_ipcc_blocked_event_listener(dwpald_conn->client_handle,
					       dwpald_event,
					       NULL,
					       dwpald_disconnected,
					       dwpald_reconnected,
					       dwpald_termination_cond,
					       (void*)dwpald_conn);

	return (ret == WAVE_IPC_SUCCESS) ? DWPALD_SUCCESS : DWPALD_ERROR;
}

static int dwpald_add_driver_events(l_list *events, const dwpald_driver_nl_event *drv_events,
					size_t num_drv_events, unsigned int id)
{
	size_t i = 0;
	bool evt_found = false, clb_found = false;

	for (i = 0; i < num_drv_events; i++) { // loop through all events
		evt_found = false;
		list_foreach_start(events, drv_event, dwpald_driver_nl_event_with_id) // search if event already exists
			if (drv_events[i].nl_id == drv_event->nl_id) {
				evt_found = true;
				list_foreach_start(drv_event->drv_clb_id_list, clb, dwpald_drv_clb_id)  // search if clb already exists
					if (clb->drv_clb == drv_events[i].drv_clb) {
						clb_found = true;
						// DLOG("Already registed callback for %s", hap_event->op_code);
						break; // clb already exist no need to append
					}
				list_foreach_end

				if (!clb_found) {
					/* new callback registration for existing event */
					// DLOG("Adding callback for %s", hap_event->op_code);
					if (dwpald_push_drv_callback(drv_event->drv_clb_id_list, drv_events[i].drv_clb, id))
						return -1;
				}
				break;
			}
		list_foreach_end

		if (!evt_found) {
			// DLOG("Adding new event for %s", hostap_events[i].op_code);
			if (dwpald_copy_driver_events(events, 1, &drv_events[i], id))
				return -1;
		}
	}

	return 0;
}


/* Check if current code is executed in events thread context */
bool dwpald_is_events_thread_context(void)
{
	if (dwpald_conn == NULL) {
		return false;
	}

	return wv_ipcc_is_event_thread(dwpald_conn->client_handle);
}


static int dwpald_add_kernel_events(l_list *kernel_clb, nl80211_event_clb kernel_event,
					unsigned int id)
{
	bool clb_found = false;
	list_foreach_start(kernel_clb, clb, dwpald_nl_event_clb_id) // search if clb already exists
		if (clb->nl_event_id == kernel_event) {
			clb_found = true;
			// DLOG("Already registed callback");
			break; // clb already exist no need to append
		}
	list_foreach_end

	if (!clb_found) {
		/* new callback registration for existing event */
		// DLOG("Adding callback");
		if (dwpald_push_kernel_callback(kernel_clb, kernel_event, id))
			return -1;
	}
	return 0;
}

static int dwpald_add_hostap_events(l_list *events, const dwpald_hostap_event *hostap_events,
					size_t num_hap_events, unsigned int id)
{
	unsigned int i = 0;
	bool evt_found = false, clb_found = false;

	for (i = 0; i < num_hap_events; i++) { // loop through all events
		evt_found = false;
		list_foreach_start(events, hap_event, dwpald_hostap_event_with_id) // search if event already exists
			if (!strncmp(hap_event->op_code, hostap_events[i].op_code, hostap_events[i].op_code_len)) {
				evt_found = true;
				list_foreach_start(hap_event->dwpald_hostap_clb_id_list, clb, dwpald_hostap_clb_id) // search if clb already exists
					if (clb->hapd_clb == hostap_events[i].hapd_clb) {
						clb_found = true;
						// DLOG("Already registed callback for %s", hap_event->op_code);
						break; // clb already exist no need to append
					}
				list_foreach_end

				if (!clb_found) {
					/* new callback registration for existing event */
					// DLOG("Adding callback for %s", hap_event->op_code);
					if (dwpald_push_hostap_callback(hap_event->dwpald_hostap_clb_id_list, hostap_events[i].hapd_clb, id))
						return -1;
				}
				break;
			}
		list_foreach_end

		if (!evt_found) {
			// DLOG("Adding new event for %s", hostap_events[i].op_code);
			if (dwpald_copy_hostap_events(events, 1, &hostap_events[i], id))
				return -1;
		}
	}

	return 0;
}

/*! \fn dwpald_ret dwpald_hostap_attach_ex_with_id(const char *ifname, size_t num_hap_events,
				const dwpald_hostap_event hostap_events[],
				hostap_event_clb common_event,
				int fail_on_iface_down, unsigned int id)
 **************************************************************************
 *  \brief Attaches to hostapd interface
 *  \param[in] char *ifname                        - WLAN interface name (like "wlan0", "wlan2", etc);
 *  \param[in] size_t num_hap_events               - The number of entries in hostap_events[] array;
 *  \param[in] dwpald_hostap_event hostap_events[] - The array of event's handlers to be registered;
 *  \param[in] hostap_event_clb common_event       - The common handler.
 *           This handler is called before any registered event's handler when hostapd event comes;
 *  \param[in] int fail_on_iface_down              - When this parameter '1' function returns DWPALD_ERROR
 *           if hostapd interface is in down state; otherwise function returns DPWALD_DWPAL_IFACE_IS_DOWN;
 *  \param[in] unsigned id                         - attach id
 *
 *  \return DWPALD_SUCCESS             - 'attach' was successfull;
 *  \return DPWALD_DWPAL_IFACE_IS_DOWN - 'attach' was successfull, but interface is in 'down' state;
 *  \return DWPALD_DISCONNECTED        - 'attach' has failed because dwpal daemon is not in running state;
 *  \return DWPALD_FAILED              - 'attach' has failed because it was called from the event's handler context;
 *  \return DWPALD_ERROR               - 'attach' has failed (all others errors);
 ***************************************************************************/
dwpald_ret dwpald_hostap_attach_ex_with_id(const char *ifname, size_t num_hap_events,
				const dwpald_hostap_event hostap_events[],
				hostap_event_clb common_event,
				int fail_on_iface_down, unsigned int id)
{
	dwpald_ret ret = DWPALD_ERROR;
	dwpald_hostap_attachment *hap_attachment = NULL;
	bool bConnected = false;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (!ifname || (num_hap_events && !hostap_events)) {
		ELOG("bad arguments");
		return DWPALD_ERROR;
	}

	if (wv_ipcc_is_event_thread(dwpald_conn->client_handle)) {
		/* Cannot attach from the serializer */
		// BUG("can't attach from the serializer context");
		return DWPALD_FAILED;
	}

	MUTEX_LOCK(&dwpald_conn->hap_attach_lock);

	/* Check if interface was already attached */
	list_foreach_start(dwpald_conn->hostap_attachments, hap_attch, dwpald_hostap_attachment)
		if (!strncmp(ifname, hap_attch->ifname, sizeof(hap_attch->ifname))) {
			LOG(2, "iface %s was already attached", ifname);
			if (hap_attch->state == INTERFACE_DWPAL_STATE_CONNECTED)
				ret = DWPALD_SUCCESS;
			else
				ret = DPWALD_DWPAL_IFACE_IS_DOWN;

			if (id != DEFAULT_ATTACH_ID)
			{
				size_t event_size_before_push = list_get_size(hap_attch->hostap_events);
				if (dwpald_add_hostap_events(hap_attch->hostap_events, hostap_events, num_hap_events, id))
				{
					MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);
					return DWPALD_ERROR;
				}
				if (event_size_before_push != list_get_size(hap_attch->hostap_events))
				{
					if (DWPALD_SUCCESS != dwpald_send_hostap_update_event(ifname, hap_attch->hostap_events)) {
						MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);
						ELOG("attach cmd for iface %s returned error", ifname);
						return DWPALD_ERROR;
					}
				}
			}
			MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);
			return ret;
		}
	list_foreach_end

	/* Create new events attachement */
	hap_attachment = (dwpald_hostap_attachment*)calloc(1, sizeof(dwpald_hostap_attachment));
	if (!hap_attachment)
		goto err;

	/* Copy events list */
	hap_attachment->hostap_events = list_init();
	if (!hap_attachment->hostap_events)
		goto err;

	if (dwpald_copy_hostap_events(hap_attachment->hostap_events, num_hap_events, hostap_events, id)) {
		ELOG("copy_hostap_events for iface %s returned error", ifname);
		goto err;
	}

	strncpy_s(hap_attachment->ifname, sizeof(hap_attachment->ifname),
		  ifname, sizeof(hap_attachment->ifname) - 1);

	hap_attachment->common_event = common_event;
	hap_attachment->state = INTERFACE_DWPAL_STATE_DISCONNECTED;

	/* Send attach request */
	ret = dwpald_send_hostap_attach(hap_attachment);
	if (ret != DWPALD_SUCCESS) {
		ELOG("attach cmd for iface %s returned error", ifname);
		goto err;
	}

	/* Check connection status */
	if (hap_attachment->state == INTERFACE_DWPAL_STATE_DISCONNECTED) {
		ret = DPWALD_DWPAL_IFACE_IS_DOWN;
		if (fail_on_iface_down) {
			dwpald_send_hostap_detach(ifname);
			goto err;
		}
	}

	/* update events handlers */
	list_push_back(dwpald_conn->hostap_attachments, hap_attachment);

	/* finalize */
	bConnected = (hap_attachment->state == INTERFACE_DWPAL_STATE_CONNECTED);
	MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);

	if (bConnected) {
		/* push "INTERFACE_CONNECTED_OK" into serializer */
		dwpald_emulate_event(WV_IPC_SYNC, ifname, "INTERFACE_CONNECTED_OK", NULL, 0);
		// DLOG("INTERFACE_CONNECTED_OK scheduled <------------------\n");
	}

	return ret;

err:
	MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);

	/* release events */
	free_hostap_attachment(hap_attachment);
	return ret;
}


dwpald_ret dwpald_hostap_attach_ex(const char *ifname, size_t num_hap_events,
				const dwpald_hostap_event hostap_events[],
				hostap_event_clb common_event,
				int fail_on_iface_down)
{
	return dwpald_hostap_attach_ex_with_id(ifname, num_hap_events, hostap_events, common_event, fail_on_iface_down, DEFAULT_ATTACH_ID);
}

dwpald_ret dwpald_hostap_attach(const char *ifname, size_t num_hap_events,
				const dwpald_hostap_event hostap_events[],
				int fail_on_iface_down)
{
	return dwpald_hostap_attach_ex_with_id(ifname, num_hap_events, hostap_events, NULL, fail_on_iface_down, DEFAULT_ATTACH_ID);
}

dwpald_ret dwpald_hostap_attach_with_id(const char *ifname, size_t num_hap_events,
				const dwpald_hostap_event hostap_events[],
				int fail_on_iface_down, unsigned int id)
{
	if (id == DEFAULT_ATTACH_ID)
	{
		ELOG("id must be > %u", DEFAULT_ATTACH_ID);
		return DWPALD_ERROR;
	}
	return dwpald_hostap_attach_ex_with_id(ifname, num_hap_events, hostap_events, NULL, fail_on_iface_down, id);
}

dwpald_ret dwpald_hostap_detach_with_id(const char *ifname, unsigned int id)
{
	dwpald_ret ret = DWPALD_SUCCESS;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (!ifname) {
		ELOG("bad arguments");
		return DWPALD_ERROR;
	}

	if (wv_ipcc_is_event_thread(dwpald_conn->client_handle)) {
		/* Can't detach from the serializer */
		BUG("can't detach from the serializer context");
		return DWPALD_FAILED;
	}

	MUTEX_LOCK(&dwpald_conn->hap_attach_lock);

	/* Now it is safe to change the list of events */
	list_foreach_start(dwpald_conn->hostap_attachments, hap_attch, dwpald_hostap_attachment)
		if (!strncmp(ifname, hap_attch->ifname, sizeof(hap_attch->ifname))) {
			size_t event_list_size_before_free = list_get_size(hap_attch->hostap_events);
			size_t event_list_size_after_free = 0;
			dwpald_free_hostap_events_with_id(hap_attch->hostap_events, id);

			event_list_size_after_free = list_get_size(hap_attch->hostap_events);
			if (event_list_size_after_free == event_list_size_before_free)
				break;
			if (0 == event_list_size_after_free) {
				// DLOG("All events removed");
				free_hostap_attachment(hap_attch);
				list_foreach_remove_current_entry()
				ret = dwpald_send_hostap_detach(ifname);
			}
			else
				ret = dwpald_send_hostap_update_event(ifname,
						hap_attch->hostap_events);
			break;
		}
	list_foreach_end

	MUTEX_UNLOCK(&dwpald_conn->hap_attach_lock);

	if (ret == DWPALD_DISCONNECTED)
		ret = DWPALD_SUCCESS;

	return ret;
}

dwpald_ret dwpald_hostap_detach(const char *ifname)
{
	return dwpald_hostap_detach_with_id(ifname, DEFAULT_ATTACH_ID);
}

dwpald_ret dwpald_nl_drv_attach_with_id(size_t num_drv_events,
				const dwpald_driver_nl_event driver_events[],
				nl80211_event_clb nl_event_cb, unsigned int id)
{
	dwpald_ret ret = DWPALD_ERROR;
	dwpald_drv_nl_attachment *drv_nl_attachment = NULL;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (num_drv_events && !driver_events) {
		ELOG("bad arguments");
		return DWPALD_ERROR;
	}

	MUTEX_LOCK(&dwpald_conn->drv_nl_attach_lock);
	if (dwpald_conn->drv_nl_attch) {
		LOG(2, "iface was already attached");
		if (id != DEFAULT_ATTACH_ID)
		{
				if (num_drv_events) {
					size_t event_size_before_push = list_get_size(dwpald_conn->drv_nl_attch->drv_events);
					if (dwpald_add_driver_events(dwpald_conn->drv_nl_attch->drv_events, driver_events, num_drv_events, id))
					{
						MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
						return DWPALD_ERROR;
					}
					if (event_size_before_push != list_get_size(dwpald_conn->drv_nl_attch->drv_events))
					{
						ret = dwpald_send_drv_nl_update_event(dwpald_conn->drv_nl_attch->drv_events);
						if (ret != DWPALD_SUCCESS) {
							MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
							ELOG("update event cmd for iface returned error, ret=%d (%s)", ret, dwpald_ret_to_string(ret));
							return ret;
						}
					}
				}

				if (nl_event_cb) {
					// DLOG("Add new kernel event callback");
					if (dwpald_add_kernel_events(dwpald_conn->drv_nl_attch->nl_event_cb, nl_event_cb, id)) {
						MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
						ELOG("Failed to update nl_event_clb");
						return DWPALD_ERROR;
					}
				}
		}
		MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
		return DWPALD_SUCCESS;
	}

	drv_nl_attachment = (dwpald_drv_nl_attachment*)calloc(1, sizeof(dwpald_drv_nl_attachment));
	if (!drv_nl_attachment)
		goto err;

	drv_nl_attachment->drv_events = list_init();
	if (!drv_nl_attachment->drv_events)
		goto err;

	drv_nl_attachment->nl_event_cb = list_init();
	if (!drv_nl_attachment->nl_event_cb)
		goto err;

	if (dwpald_copy_driver_events(drv_nl_attachment->drv_events, num_drv_events, driver_events, id)) {
		ELOG("copy_driver_events for iface returned error");
		goto err;
	}

	ret = dwpald_send_drv_nl_attach(drv_nl_attachment->drv_events);
	if (ret != DWPALD_SUCCESS) {
		ELOG("attach cmd for iface returned error, ret=%d (%s)", ret, dwpald_ret_to_string(ret));
		goto err;
	}

	if (dwpald_push_kernel_callback(drv_nl_attachment->nl_event_cb, nl_event_cb, id)) {
		ELOG("NL kernel callback adding error");
		goto err;
	}

	dwpald_conn->drv_nl_attch = drv_nl_attachment;
	MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);

	return DWPALD_SUCCESS;

err:
	MUTEX_UNLOCK(&dwpald_conn->drv_nl_attach_lock);
	free_drv_nl_attachment(drv_nl_attachment);
	return ret;
}

dwpald_ret dwpald_nl_drv_attach(size_t num_drv_events,
				const dwpald_driver_nl_event *driver_events,
				nl80211_event_clb nl_event_cb)
{
	return dwpald_nl_drv_attach_with_id(num_drv_events, driver_events, nl_event_cb, DEFAULT_ATTACH_ID);
}

static const char* _dwpal_ret_to_string(DWPAL_Ret val)
{
	switch (val) {
		case DWPAL_FAILURE:
			return "DWPAL_FAILURE";
		case DWPAL_SUCCESS:
			return "DWPAL_SUCCESS";
		case DWPAL_INTERFACE_IS_DOWN:
			return "DWPAL_INTERFACE_IS_DOWN";
		default:
			return "(DWPAL internal)";
	}
}

dwpald_ret dwpald_hostap_cmd(const char *ifname, const char *cmd, size_t len,
			     char *reply, size_t *reply_len)
{
	wv_ipc_msg *msg, *response = NULL;
	dwpald_header cmd_hdr = { 0 }, resp_hdr;
	dwpald_ret ret = DWPALD_ERROR;
	DWPAL_Ret dpal_ret;
	wv_ipc_ret ipc_ret;
	void *resp_data;

	if (!ifname || !cmd  || !reply || !reply_len) {
		ELOG("bad arguments");
		return DWPALD_ERROR;
	}

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if ((msg = wave_ipc_msg_alloc()) == NULL)
		return DWPALD_ERROR;

	cmd_hdr.header[0] = DWPALD_CMD;
	cmd_hdr.header[1] = DWPALD_IF_TYPE_HOSTAP;
	cmd_hdr.header[2] = strnlen_s(ifname, IFNAMSIZ);
	dwpald_header_push(msg, &cmd_hdr);
	wave_ipc_msg_fill_data(msg, ifname, cmd_hdr.header[2]);
	wave_ipc_msg_append_data(msg, cmd, len);
	if (cmd[len - 1] != '\0')
		wave_ipc_msg_append_data(msg, "\0", 1);

	ipc_ret = wave_ipcc_send_cmd(dwpald_conn->client_handle,
				     msg, &response);
	wave_ipc_msg_put(msg);
	if (ipc_ret != WAVE_IPC_SUCCESS) {
		ELOG("'%s': ipcc_send_cmd() returned err (ret=%d)", ifname, ipc_ret);
		if (ipc_ret == WAVE_IPC_DISCONNECTED)
			ret = DWPALD_DISCONNECTED;
		goto err;
	}

	if (dwpald_header_pop(response, &resp_hdr) ||
	    resp_hdr.header[0] != DWPALD_CMD_RESP ||
	    resp_hdr.header[1] != DWPALD_IF_TYPE_HOSTAP) {
		BUG("'%s': response header is corrupted", ifname);
		goto err;
	}

	dpal_ret = (int8_t)resp_hdr.header[2];
	if (dpal_ret != DWPAL_SUCCESS) {
		ELOG("'%s': dwpal failed to handle our cmd (dpal_ret=%d, %s)", ifname, dpal_ret, _dwpal_ret_to_string(dpal_ret));
		ret = dwpald_ret_from_dwpal_ret(dpal_ret);
		goto err;
	} else
		ret = DWPALD_SUCCESS;

	if (wave_ipc_msg_get_size(response) > *reply_len) {
		ELOG("'%s': reply len from dwpald (%zu) is greater than reply_len(%zu)", ifname,
		     wave_ipc_msg_get_size(response), *reply_len);
		ret = DWPALD_ERROR;
		goto err;
	}

	resp_data = wave_ipc_msg_get_data(response);
	*reply_len = wave_ipc_msg_get_size(response);
	if (!*reply_len || resp_data == NULL) {
		BUG("'%s': get_data returned NULL", ifname);
		ret = DWPALD_ERROR;
		goto err;
	}

	memcpy_s(reply, *reply_len, resp_data, *reply_len);
	(*reply_len)--;

	wave_ipc_msg_put(response);
	return ret;

err:
	if (response)
		wave_ipc_msg_put(response);
	*reply_len = 0;
	return ret;
}

static struct nl_msg * dwpald_nl_msg_get(char *ifname, int flags, uint8_t command)
{
	struct nl_msg *msg;
	uint32_t devidx;

	devidx = if_nametoindex(ifname);
	if (!devidx) {
		ELOG("err getting devidx for %s", ifname);
		return NULL;
	}

	msg = nlmsg_alloc();
	if (msg == NULL)
		return NULL;

	if (!genlmsg_put(msg, 0, 0, dwpald_conn->nl80211_id, 0, flags, command, 0))
		goto err;

	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, devidx) < 0)
		goto err;

	return msg;
err:
	nlmsg_free(msg);
	return NULL;
}

static dwpald_ret dwpald_send_nl_command_to_daemon(struct nl_msg *msg,
						   wv_ipc_msg **reply)
{
	wv_ipc_msg *ipc_cmd = dwpald_ipc_msg_from_nl_msg(msg);
	dwpald_header hdr = { 0 };
	wv_ipc_msg *local_reply = NULL;
	wv_ipc_ret ipc_ret;

	if (ipc_cmd == NULL)
		return DWPALD_ERROR;

	hdr.header[0] = DWPALD_CMD;
	hdr.header[1] = DWPALD_IF_TYPE_KERNEL;
	if (dwpald_header_push(ipc_cmd, &hdr))
		goto err;

	ipc_ret = wave_ipcc_send_cmd(dwpald_conn->client_handle, ipc_cmd, &local_reply);
	wave_ipc_msg_put(ipc_cmd);
	if (ipc_ret != WAVE_IPC_SUCCESS) {
		ELOG("ipcc_send_cmd() returned err (ret=%d)", ipc_ret);
		if (local_reply)
			wave_ipc_msg_put(local_reply);
		if (ipc_ret == WAVE_IPC_DISCONNECTED)
			return DWPALD_DISCONNECTED;
		return DWPALD_ERROR;
	}

	*reply = local_reply;
	return DWPALD_SUCCESS;
err:
	wave_ipc_msg_put(ipc_cmd);
	return DWPALD_ERROR;
}

dwpald_ret dwpald_drv_get(char *ifname, unsigned int command_id, int *cmd_res,
			  void *in_data, size_t in_data_size,
			  void *out_data, size_t *out_data_size)
{
	struct nl_msg *msg;
	struct nl_msg *reply = NULL;
	wv_ipc_msg *ipc_reply = NULL;
	dwpald_header resp_hdr;
	DWPAL_Ret dpal_ret;
	dwpald_ret ret = DWPALD_ERROR;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (ifname == NULL || cmd_res == NULL || out_data == NULL ||
	    out_data_size == NULL || *out_data_size == 0)
		return DWPALD_ERROR;

	msg = dwpald_nl_msg_get(ifname, 0, NL80211_CMD_VENDOR);
	if (msg == NULL)
		return DWPALD_ERROR;

	if (nla_put_u32(msg, NL80211_ATTR_VENDOR_ID, OUI_LTQ) < 0 ||
	    nla_put_u32(msg, NL80211_ATTR_VENDOR_SUBCMD, command_id) < 0)
		goto err;

	if (in_data && in_data_size &&
	    nla_put(msg, NL80211_ATTR_VENDOR_DATA, in_data_size, in_data) < 0)
		goto err;

	ret = dwpald_send_nl_command_to_daemon(msg, &ipc_reply);
	if (ret != DWPALD_SUCCESS) {
		ELOG("sending nl command to daemon failed, ret=%d (%s)", ret, dwpald_ret_to_string(ret));
		goto err;
	}
	ret = DWPALD_ERROR;

	dwpald_header_pop(ipc_reply, &resp_hdr);

	if (resp_hdr.header[2] == 0) {
		dpal_ret = resp_hdr.header[4];
		if (dpal_ret != DWPAL_SUCCESS) {
			ELOG("daemon returned dwpal err (%d)", dpal_ret);
			ret = dwpald_ret_from_dwpal_ret(dpal_ret);
			goto finish;
		}

		*cmd_res = (int8_t)resp_hdr.header[3];
		ret = DWPALD_SUCCESS;
		goto finish;
	}
	*cmd_res = 0;

	reply = dwpald_nl_msg_from_ipc_msg(ipc_reply);
	if (reply == NULL) {
		ELOG("reply is NULL");
		goto err;
	}

	{
		struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(reply));
		struct nlattr *tb[NL80211_ATTR_MAX + 1];
		struct nlattr *attr;

		nla_parse(tb, NL80211_ATTR_MAX,
			  genlmsg_attrdata(gnlh, 0),
			  genlmsg_attrlen(gnlh, 0),
			  NULL);

		attr = nla_find(genlmsg_attrdata(gnlh, 0),
				genlmsg_attrlen(gnlh, 0),
				NL80211_ATTR_VENDOR_DATA);

		if (attr == NULL) {
			ELOG("attr is NULL");
			goto err;
		}

		if ((size_t)(nla_len(attr)) > *out_data_size) {
			ELOG("size mis-match nla_len(%d), out_data_size(%zu)",
			     nla_len(attr), *out_data_size);
			goto err;
		}

		*out_data_size = nla_len(attr);
		memcpy_s(out_data, *out_data_size, (char*)nla_data(attr), *out_data_size);
	}

	ret = DWPALD_SUCCESS;
finish:
	nlmsg_free(msg);
	if (reply)
		nlmsg_free(reply);
	wave_ipc_msg_put(ipc_reply);
	return ret;
err:
	if (ipc_reply)
		wave_ipc_msg_put(ipc_reply);
	if (reply)
		nlmsg_free(reply);
	nlmsg_free(msg);
	return ret;
}

dwpald_ret dwpald_drv_set(char *ifname, unsigned int command_id, int *cmd_res,
			  const void *vendor_data, size_t vendor_data_size)
{
	struct nl_msg *msg;
	wv_ipc_msg *ipc_reply = NULL;
	dwpald_header resp_hdr;
	DWPAL_Ret dpal_ret;
	dwpald_ret ret = DWPALD_ERROR;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (ifname == NULL || cmd_res == NULL || vendor_data == NULL || !vendor_data_size)
		return DWPALD_ERROR;

	msg = dwpald_nl_msg_get(ifname, 0, NL80211_CMD_VENDOR);
	if (msg == NULL)
		return DWPALD_ERROR;

	if (nla_put_u32(msg, NL80211_ATTR_VENDOR_ID, OUI_LTQ) < 0 ||
	    nla_put_u32(msg, NL80211_ATTR_VENDOR_SUBCMD, command_id) < 0 ||
	    nla_put(msg, NL80211_ATTR_VENDOR_DATA, vendor_data_size, vendor_data) < 0)
		goto err;

	ret = dwpald_send_nl_command_to_daemon(msg, &ipc_reply);
	if (ret != DWPALD_SUCCESS) {
		ELOG("sending nl command to daemon failed, ret=%d (%s)", ret, dwpald_ret_to_string(ret));
		goto err;
	}
	ret = DWPALD_ERROR;

	dwpald_header_pop(ipc_reply, &resp_hdr);

	dpal_ret = resp_hdr.header[4];
	if (dpal_ret != DWPAL_SUCCESS) {
		ELOG("daemon returned dwpal err (%d)", dpal_ret);
		ret = dwpald_ret_from_dwpal_ret(dpal_ret);
		goto finish;
	}
	*cmd_res = (int8_t)resp_hdr.header[3];

	ret = DWPALD_SUCCESS;
finish:
	nlmsg_free(msg);
	wave_ipc_msg_put(ipc_reply);
	return ret;

err:
	if (ipc_reply)
		wave_ipc_msg_put(ipc_reply);
	nlmsg_free(msg);
	return ret;
}

static int parse_hex(char *hex_str, unsigned char **out_result, size_t *out_len)
{
	size_t len;
	unsigned char *result = NULL;
	char *hex_str_copy = NULL, *hex_str_copy_tmp;

	len = strnlen_s(hex_str, SCAN_PARAM_STRING_LENGTH);
	hex_str_copy = calloc(len + 1, sizeof(char));
	if (hex_str_copy == NULL)
		goto err;

	strncpy_s(hex_str_copy, len + 1, hex_str, len);
	result = calloc((len / 2) + 2, sizeof(char));
	if (result == NULL)
		goto err;

	len = 0;
	hex_str_copy_tmp = hex_str_copy;
	while (1) {
		int temp;
		char *end, *cp;

		cp = strchr(hex_str_copy_tmp, ':');
		if (cp) {
			*cp = 0;
			cp++;
		}

		temp = strtoul(hex_str_copy_tmp, &end, 16);
		if (*end || temp < 0 || temp > 255)
			goto err;

		result[len] = temp;
		len++;

		if (!cp)
			break;

		hex_str_copy_tmp = cp;
	}

	*out_len = len;
	*out_result = result;
	free(hex_str_copy);
	return 0;

err:
	if (result) free(result);
	if (hex_str_copy) free(hex_str_copy);
	return 1;
}

dwpald_ret dwpald_nl80211_id_get(int *nl80211_id)
{
	if (!nl80211_id)
		return DWPALD_ERROR;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (dwpald_conn->nl80211_id == -1) {
		ELOG("nl80211_id is still invalid, nl_drv_attach needs to be called");
		return DWPALD_ERROR;
	}

	*nl80211_id = dwpald_conn->nl80211_id;
	return DWPALD_SUCCESS;
}

dwpald_ret dwpald_nl80211_cmd_send(struct nl_msg *msg, nl80211_cmd_clb cmd_cb,
				   int *cmd_res, void *cb_arg)
{
	wv_ipc_msg *ipc_reply = NULL;
	dwpald_header resp_hdr = { 0 };
	DWPAL_Ret dpal_ret;
	dwpald_ret ret = DWPALD_ERROR;

	if (msg == NULL)
		return DWPALD_ERROR;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		goto err;
	}

	if (cmd_res == NULL)
		goto err;

	ret = dwpald_send_nl_command_to_daemon(msg, &ipc_reply);
	if (ret != DWPALD_SUCCESS) {
		ELOG("sending nl command to daemon failed, ret=%d (%s)", ret, dwpald_ret_to_string(ret));
		goto err;
	}
	ret = DWPALD_ERROR;

	if (wave_ipc_msg_is_multi_msg(ipc_reply)) {
		wv_ipc_msg *next = ipc_reply;
		int filled_res = 0;

		LOG(2, "reply is multi msg");

		if (!cmd_cb) {
			ELOG("nl cmd has reply, but cmd_cb is NULL");
			goto err;
		}

		do {
			if (dwpald_header_pop(next, &resp_hdr)) {
				BUG("header is corrupt");
				goto err;
			}

			if (resp_hdr.header[2]) {
				struct nl_msg *res_msg = dwpald_nl_msg_from_ipc_msg(next);
				if (res_msg) {
					cmd_cb(res_msg, cb_arg);
					nlmsg_free(res_msg);
				} else {
					BUG("nl msg is NULL");
					goto err;
				}
			} else {
				dpal_ret = resp_hdr.header[4];
				if (dpal_ret != DWPAL_SUCCESS) {
					ELOG("daemon returned dwpal err (%d)", dpal_ret);
					ret = dwpald_ret_from_dwpal_ret(dpal_ret);
					goto finish;
				}
				*cmd_res = (int8_t)resp_hdr.header[3];
				LOG(2, "final message in a series, res=%d", *cmd_res);

				filled_res = 1;
			}

			next = wave_ipc_multi_msg_get_next(next);
		} while (next);

		if (!filled_res) {
			BUG("did not get cmd_res from multi msg response");
			goto err;
		}
	} else {
		LOG(2, "reply is single");
		dwpald_header_pop(ipc_reply, &resp_hdr);
		if (resp_hdr.header[2] == 0) {
			dpal_ret = resp_hdr.header[4];
			if (dpal_ret != DWPAL_SUCCESS) {
				ELOG("daemon returned dwpal err (%d)", dpal_ret);
				ret = dwpald_ret_from_dwpal_ret(dpal_ret);
				goto finish;
			}

			*cmd_res = (int8_t)resp_hdr.header[3];
		} else {
			struct nl_msg *res_msg;

			if (!cmd_cb) {
				ELOG("nl cmd has reply, but cmd_cb is NULL");
				goto err;
			}

			res_msg = dwpald_nl_msg_from_ipc_msg(ipc_reply);
			if (res_msg) {
				cmd_cb(res_msg, cb_arg);
				nlmsg_free(res_msg);
			} else {
				BUG("nl msg is NULL");
				goto err;
			}

			*cmd_res = 0;
		}
	}

	ret = DWPALD_SUCCESS;
finish:
	nlmsg_free(msg);
	wave_ipc_msg_put(ipc_reply);
	return ret;
err:
	if (ipc_reply)
		wave_ipc_msg_put(ipc_reply);
	nlmsg_free(msg);
	return ret;
}

dwpald_ret dwpald_ieee80211_scan_trigger(char *ifname, scan_params *params, int *cmd_res)
{
	struct nl_msg *msg = NULL;
	dwpald_ret ret = DWPALD_ERROR;

	int i;
	uint32_t scan_flags = 0;
	size_t ies_len = 0, meshid_len = 0;
	unsigned char *ies = NULL, *meshid = NULL, *tmpies = NULL;
	struct nl_msg *freqs = NULL, *ssids = NULL;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (ifname == NULL || params == NULL || cmd_res == NULL)
		return DWPALD_ERROR;

	msg = dwpald_nl_msg_get(ifname, 0, NL80211_CMD_TRIGGER_SCAN);
	if (msg == NULL)
		return DWPALD_ERROR;

	if (params->lowpri) scan_flags |= NL80211_SCAN_FLAG_LOW_PRIORITY;
	if (params->flush) scan_flags |= NL80211_SCAN_FLAG_FLUSH;
	if (params->ap_force) scan_flags |= NL80211_SCAN_FLAG_AP;

	if (scan_flags &&
	    nla_put_u32(msg, NL80211_ATTR_SCAN_FLAGS, scan_flags) < 0)
		goto err;

	if (params->ies[0] != '\0') {
		if (parse_hex(params->ies, &ies, &ies_len)) {
			ELOG("failed to parse 'ies' field");
			goto err;
		}
	}

	if (params->meshid[0] != '\0') {
		meshid_len = strnlen_s(params->meshid, SCAN_PARAM_STRING_LENGTH);
		meshid = (unsigned char *)calloc(meshid_len + 2, sizeof(char));
		if (meshid == NULL)
			goto err;

		meshid[0] = 114; /* mesh element id */
		meshid[1] = meshid_len;
		memcpy_s(&meshid[2], meshid_len - 2,
			 params->meshid, meshid_len);
	}

	if (ies || meshid) {
		tmpies = (unsigned char *)calloc(ies_len + meshid_len, sizeof(unsigned char));
		if (tmpies == NULL)
			goto err;

		if (ies != NULL)
			memcpy_s(tmpies, ies_len + meshid_len, ies, ies_len);

		if (meshid != NULL)
			memcpy_s(&tmpies[ies_len], meshid_len, meshid, meshid_len);

		if (nla_put(msg, NL80211_ATTR_IE, ies_len + meshid_len, tmpies) < 0)
			goto err;
	}

	for (i = 0; i < NUM_OF_SSIDS && params->ssid[i][0] != '\0'; i++) {
		size_t ssid_len;

		if (ssids == NULL) {
			ssids = nlmsg_alloc();
			if (ssids == NULL)
				goto err;
		}

		ssid_len = strnlen_s(params->ssid[i], SSID_STRING_LENGTH);
		if (nla_put(ssids, i, ssid_len, params->ssid[i]) < 0)
			goto err;
	}

	if (params->passive == false) {
		if (ssids == NULL) {
			ssids = nlmsg_alloc();
			if (ssids == NULL)
				goto err;

			if (nla_put(ssids, 1, 0, "") < 0)
				goto err;
		}

		nla_put_nested(msg, NL80211_ATTR_SCAN_SSIDS, ssids);
	}

	for (i=0; i < NUM_OF_FREQUENCIES && params->freq[i] > 0; i++) {
		if (freqs == NULL) {
			freqs = nlmsg_alloc();
			if (freqs == NULL)
				goto err;
		}

		if (nla_put_u32(freqs, i, params->freq[i]) < 0)
			goto err;
	}

	if (freqs)
		nla_put_nested(msg, NL80211_ATTR_SCAN_FREQUENCIES, freqs);

	ret = dwpald_nl80211_cmd_send(msg, NULL, cmd_res, NULL);
	msg = NULL; //msg is freed inside dwpald_nl80211_cmd_send

err:
	if (ies) free(ies);
	if (meshid) free(meshid);
	if (tmpies) free(tmpies);
	if (ssids) nlmsg_free(ssids);
	if (freqs) nlmsg_free(freqs);
	if (msg) nlmsg_free(msg);
	return ret;
}

dwpald_ret dwpald_ieee80211_scan_dump(char *ifname, nl80211_cmd_clb dump_cb,
				      int *cmd_res, void *cb_arg)
{
	struct nl_msg *msg;

	if (dwpald_conn == NULL) {
		ELOG("dwpald client is not connected");
		return DWPALD_ERROR;
	}

	if (ifname == NULL || dump_cb == NULL || cmd_res == NULL)
		return DWPALD_ERROR;

	msg = dwpald_nl_msg_get(ifname, NLM_F_DUMP, NL80211_CMD_GET_SCAN);
	if (msg == NULL)
		return DWPALD_ERROR;

	return dwpald_nl80211_cmd_send(msg, dump_cb, cmd_res, cb_arg);
}

const char* dwpald_ret_to_string(dwpald_ret val)
{
	switch (val) {
		case DWPALD_SUCCESS:
			return "DWPALD_SUCCESS";
		case DWPALD_DISCONNECTED:
			return "DWPALD_DISCONNECTED";
		case DWPALD_DWPAL_FAILURE:
			return "DWPALD_DWPAL_FAILURE";
		case DPWALD_DWPAL_IFACE_IS_DOWN:
			return "DPWALD_DWPAL_IFACE_IS_DOWN";
		case DWPALD_FAILED:
			return "DWPALD_FAILED";
		case DWPALD_ERROR:
			return "DWPALD_ERROR";
		default:
			return "UNKNOWN";
	}
}
