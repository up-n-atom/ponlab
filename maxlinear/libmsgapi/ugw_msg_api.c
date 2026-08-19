/********************************************************************************
 
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : ugw_msg_api.c                                         *
 *         Description  : UGW Msg API layer which exposes APIs that can be used *
 *			  to communicate with UGW framework from non UGW 	*
 *			  component 						*
 *  *****************************************************************************/


#include "ugw_msg_api.h"
#include <pthread.h>
#include <ulogging.h>

#define MSG_API_UBUS_TIMEOUT (302*1000)

/* UBUS is NOT thread safe, so to guard for improper usage we use a lock for each operation */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#ifndef LOG_LEVEL
uint16_t LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
uint16_t LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
uint16_t LOGTYPE = SYS_LOG_TYPE_FILE;
#else
uint16_t LOGTYPE = LOG_TYPE;
#endif

#define ERROR(fmt, args...) LOGF_LOG_ERROR(fmt, ##args)
#define INFO(fmt, args...) LOGF_LOG_INFO(fmt, ##args)
#define DEBUG(fmt, args...) LOGF_LOG_DEBUG(fmt, ##args)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

enum {
	MSG_API_MID, /* message id */
	MSG_API_SN,
	MSG_API_NID,
	MSG_API_OPERTYPE,
	MSG_API_OBJTYPE,
	MSG_API_MSG,
	MSG_API_RMSG
};

static const struct blobmsg_policy pol[] = {
	[MSG_API_MID] = {
		.name = "msgid",
		.type = BLOBMSG_TYPE_INT32,
	},
	[MSG_API_SN] = {
		.name = "sn",
		.type = BLOBMSG_TYPE_STRING,
	},
	[MSG_API_NID] = {
		.name = "nid",
		.type = BLOBMSG_TYPE_STRING,
	},
	[MSG_API_OPERTYPE] = {
		.name = "opertype",
		.type = BLOBMSG_TYPE_INT32,
	},
	[MSG_API_OBJTYPE] = {
		.name = "objtype",
		.type = BLOBMSG_TYPE_INT32,
	},
	[MSG_API_MSG] = {
		.name = "msg",
		.type = BLOBMSG_TYPE_UNSPEC,
	},
	[MSG_API_RMSG] = {
		.name = "rmsg",
		.type = BLOBMSG_TYPE_UNSPEC,
	},
};

static int msg2blob(IN Msg_t *msg, OUT struct blob_buf *buf, IN unsigned int msg_id)
{
	int ret;

	DEBUG("ENTER\n");

	if (!buf || !msg) {
		ERROR("buf[=%p] or msg[=%p] is NULL\n", buf, msg);
		return UGW_FAILURE;
	}

	memset(buf, 0 , sizeof(*buf));
	if ((ret = blobmsg_buf_init(buf)) < 0)
		goto out_err;
	if ((ret = blobmsg_add_u32(buf, "msgid", msg_id)) < 0)
		goto out_err;
	if ((ret = blobmsg_add_string(buf, "sn", msg->sSerName)) < 0)
		goto out_err;
	if ((ret = blobmsg_add_string(buf, "nid",msg->sSLId)) < 0)
		goto out_err;
	if ((ret = blobmsg_add_u32(buf, "opertype",msg->nOperType)) < 0)
		goto out_err;
	if ((ret = blobmsg_add_u32(buf, "objtype",msg->nMsgType)) < 0)
		goto out_err;
	if (msg->nMsgSize) {
		if (!msg->pMsg) {
			ERROR("msg->pMsg is NULL although msg->MsgSize is not zero\n");
			ret = UGW_FAILURE;
			goto out_err;
		}
		if ((ret = blobmsg_add_field(buf, BLOBMSG_TYPE_UNSPEC, "msg", msg->pMsg, msg->nMsgSize)) < 0)
			goto out_err;
	}
	if (msg->nRMsgSize) {
		if (!msg->pRMsg) {
			ERROR("msg->pRMsg is NULL although msg->RMsgSize is not zero\n");
			ret = UGW_FAILURE;
			goto out_err;
		}
		if ((ret = blobmsg_add_field(buf, BLOBMSG_TYPE_UNSPEC, "rmsg", msg->pRMsg, msg->nRMsgSize)) < 0)
			goto out_err;
	}

	DEBUG("EXIT SUCCESS\n");
	return UGW_SUCCESS;

out_err:
	if(buf->buf != NULL)
		blob_buf_free(buf);
	DEBUG("EXIT FAILURE (ret=%d)\n", ret);
	return ret;
}

static int blob2msg(IN struct blob_attr *blob, OUT Msg_t *msg)
{
	struct blob_attr *tb[ARRAY_SIZE(pol)];

	if (!blob || !msg) {
		ERROR("blob[=%p] or msg[=%p] is NULL\n", blob, msg);
		return UGW_FAILURE;
	}
	
	if (blobmsg_parse(pol, ARRAY_SIZE(pol), tb, blob_data(blob), blob_len(blob))) {
		ERROR("Parse blobmsg failed\n");
		return UGW_FAILURE;
	}

	if (!tb[MSG_API_MID]) {
		ERROR("Parse failed - 'msgid' field is missing in the blob\n");
		return UGW_FAILURE;
	}
	msg->id = blobmsg_get_u32(tb[MSG_API_MID]);
	DEBUG("message %p id = %d\n", msg, msg->id);

	if (!tb[MSG_API_SN]) {
		ERROR("Parse failed - 'sn' field is missing in the blob\n");
		return UGW_FAILURE;
	}
	sprintf_s(msg->sSerName, MAX_SERVER_NAME, "%s", (char *)blobmsg_data(tb[MSG_API_SN]));

	if (!tb[MSG_API_NID]) {
		ERROR("Parse failed - 'nid' field is missing in the blob\n");
		return UGW_FAILURE;
	}
	sprintf_s(msg->sSLId, MAX_SL_NAME, "%s", (char *)blobmsg_data(tb[MSG_API_NID]));

	if (!tb[MSG_API_OPERTYPE]) {
		ERROR("Parse failed - 'opertype' field is missing in the blob\n");
		return UGW_FAILURE;
	}
	msg->nOperType = blobmsg_get_u32(tb[MSG_API_OPERTYPE]);

	if (!tb[MSG_API_OBJTYPE]) {
		ERROR("Parse failed - 'objtype' field is missing in the blob\n");
		return UGW_FAILURE;
	}
	msg->nMsgType = blobmsg_get_u32(tb[MSG_API_OBJTYPE]);

	if (tb[MSG_API_MSG]) {
		msg->nMsgSize = blobmsg_data_len(tb[MSG_API_MSG]);
		INFO("message data recieved (len=%d)\n", msg->nMsgSize);
		msg->pMsg = calloc(1, msg->nMsgSize);
		if(!msg->pMsg) {
			ERROR("Memory allocation failed \n");
			return UGW_FAILURE;
		}
		memcpy_s(msg->pMsg, msg->nMsgSize, blobmsg_data(tb[MSG_API_MSG]), msg->nMsgSize);
	}

	if (tb[MSG_API_RMSG]) {
		msg->nRMsgSize = blobmsg_data_len(tb[MSG_API_RMSG]);
		INFO("reply message data recieved (len=%d)\n", msg->nRMsgSize);
		msg->pRMsg = calloc(1, msg->nRMsgSize);
		if(!msg->pRMsg) {
			ERROR("Memory allocation failed \n");
			if (msg->pMsg)
				free(msg->pMsg);
			return UGW_FAILURE;
		}
		memcpy_s(msg->pRMsg, msg->nMsgSize, blobmsg_data(tb[MSG_API_RMSG]), msg->nRMsgSize);
	}

	return UGW_SUCCESS;
}

int msg_send(IN Msg_t *msg)
{
	struct blob_buf buf;
	struct ubus_context *ctx = NULL;
	unsigned int id;
	static unsigned int counter = 0;
	int ret;

	if (!msg) {
		ERROR("msg is NULL\n");
		return UGW_FAILURE;
	}

	pthread_mutex_lock(&lock);
	INFO("send message #%d (%p) to %s (%s) START\n", counter, msg, msg->sSerName, msg->sSLId);
	DEBUG("send message #%d (%p) params: nOperType=%d, nObjType=%d, nMsgSize=%d, nRMsgSize=%d\n",
	      counter, msg, msg->nOperType, msg->nMsgType, msg->nMsgSize, msg->nRMsgSize);

	if ((ret = msg2blob(msg, &buf, counter)) != UGW_SUCCESS) {
		ERROR("msg2blob failed, ret=%d\n", ret);
		goto out;
	}

	ctx = ubus_connect(NULL);
	if (!ctx) {
		ERROR("ubus_connect failed\n");
		ret = UGW_FAILURE;
		goto out;
	}

	/* TODO - understand why this is needed */
	ctx->sock.registered = IS_THRED_ENV_SET(msg->nOperType) ? true : false;

	if ((ret = ubus_lookup_id(ctx, msg->sSerName, &id)) < 0) {
		ERROR("UBUS lookup id (%s) failed (ret=%d)\n", msg->sSerName, ret);
		ret = UGW_FAILURE;
		goto out;
	}

	ret = ubus_invoke(ctx, id, "notify.msg" , buf.head, NULL, NULL, MSG_API_UBUS_TIMEOUT);
	if (ret != UBUS_STATUS_OK) {
		ERROR("send message #%d (%p) to %s (%s) [nOperType=%d, nObjType=%d, nMsgSize=%d, nRMsgSize=%d] failed with ret=%d\n",
		      counter, msg, msg->sSerName, msg->sSLId, msg->nOperType, msg->nMsgType, msg->nMsgSize, msg->nRMsgSize, ret);
		ret = UGW_FAILURE;
		goto out;
	}
	
	INFO("send message #%d (%p) to %s (%s) SUCCESS\n", counter, msg, msg->sSerName, msg->sSLId);
out:
	counter++;
	if(buf.buf != NULL)
		blob_buf_free(&buf);
	if (ctx)
		ubus_free(ctx);
	pthread_mutex_unlock(&lock);
	return ret;
}

int msg_recv(IN struct blob_attr *blob, OUT Msg_t *msg)
{
	int ret = UGW_SUCCESS;

	if (!blob || !msg) {
		ERROR("blob[=%p] or msg[=%p] is NULL\n", blob, msg);
		return UGW_FAILURE;
	}

	memset(msg, 0 , sizeof(*msg));
	if ((ret = blob2msg(blob, msg)) != UGW_SUCCESS) {
		ERROR("blob2msg failed with ret=%d\n", ret);
		goto out;
	}

	INFO("received message #%d (%p) at %s (%s)\n", msg->id, msg, msg->sSerName, msg->sSLId);
	DEBUG("received message #%d (%p) params: nOperType=%d, nObjType=%d, nMsgSize=%d, nRMsgSize=%d\n",
	      msg->id, msg, msg->nOperType, msg->nMsgType, msg->nMsgSize, msg->nRMsgSize);

out:
	return ret;
}	
