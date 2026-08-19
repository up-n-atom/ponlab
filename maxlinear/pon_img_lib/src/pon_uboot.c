/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#include <safe_lib.h>
#include <safe_str_lib.h>
#pragma GCC diagnostic pop
#include <libubox/blobmsg.h>
/* libubus include needed for struct ubus_request */
#include <libubus.h>
#include <pon_adapter_config.h>

#include "pon_uboot.h"
#include "pon_img_common.h"
#include "pon_img_debug.h"

static const struct blobmsg_policy uboot_get_policy[] = {
	{ .name = "message", .type = BLOBMSG_TYPE_STRING },
	{ .name = "active_bank", .type = BLOBMSG_TYPE_STRING },
	{ .name = "img_validA", .type = BLOBMSG_TYPE_STRING },
	{ .name = "img_validB", .type = BLOBMSG_TYPE_STRING },
	{ .name = "img_validA", .type = BLOBMSG_TYPE_INT32 },
	{ .name = "img_validB", .type = BLOBMSG_TYPE_INT32 },
	{ .name = "img_versionA", .type = BLOBMSG_TYPE_STRING },
	{ .name = "img_versionB", .type = BLOBMSG_TYPE_STRING },
	{ .name = "commit_bank", .type = BLOBMSG_TYPE_STRING },
	{ .name = "img_activate", .type = BLOBMSG_TYPE_STRING },
};

struct uboot_get_cache_entry {
	const char *name;
	char value[UBOOT_VAL_LEN_MAX + 1];
	unsigned int value_size;
};

static struct uboot_get_cache_entry uboot_cache[ARRAY_SIZE(uboot_get_policy)];

static void uboot_get_cb(struct ubus_request *req,
			 int type, struct blob_attr *msg)
{
	struct blob_attr *tb[ARRAY_SIZE(uboot_get_policy)];
	struct uboot_get_cache_entry *entry;
	int i, len;

	(void)req; /* unused */
	(void)type; /* unused */
	blobmsg_parse(uboot_get_policy, ARRAY_SIZE(uboot_get_policy), tb,
		      blob_data(msg), blob_len(msg));

	/* skip first entry, it is for "message" */
	for (i = 1; i < ARRAY_SIZE(uboot_get_policy); i++) {
		entry = &uboot_cache[i];
		entry->value_size = 0;
		entry->name = uboot_get_policy[i].name;
		if (!tb[i])
			continue;

		if (uboot_get_policy[i].type == BLOBMSG_TYPE_INT32) {
			len = snprintf(entry->value, sizeof(entry->value), "%s",
				blobmsg_get_u32(tb[i]) ? "true" : "false");
			if (len > UBOOT_VAL_LEN_MAX)
				len = UBOOT_VAL_LEN_MAX;
			entry->value_size = len;
			continue;
		}
		/* if the type is not "INT32", it must be a string */
		len = strnlen_s(blobmsg_get_string(tb[i]),
				blobmsg_data_len(tb[i]));
		if (len > UBOOT_VAL_LEN_MAX)
			len = UBOOT_VAL_LEN_MAX;
		if (strncpy_s(entry->value, sizeof(entry->value),
			      blobmsg_get_string(tb[i]), len)) {
			dbg_err_fn(strncpy_s);
			/* don't break when cannot copy the value */
			continue;
		}
		entry->value_size = len;
	}
}

static enum pon_adapter_errno
uboot_get_cache_update(struct pon_img_context *ctx)
{
	int err;
	time_t current_time = time(NULL);

	dbg_in_args("%p", ctx);

	if (ctx->ubus_reboot_only)
		return PON_ADAPTER_ERR_NOT_SUPPORTED;

	/* if last update was less then two seconds ago, return */
	if (current_time - ctx->last_ubus_ubootvars < 2) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	ctx->last_ubus_ubootvars = current_time;

	err = ctx->pa_config->ubus_call(ctx->hl_handle,
					ctx->ubus_path,
					UBUS_METHOD_GET_UBOOTVARS,
					NULL, uboot_get_cb, NULL,
					PON_UBUS_TIMEOUT);
	if (err == UBUS_STATUS_METHOD_NOT_FOUND)
		return PON_ADAPTER_ERR_NOT_SUPPORTED;
	if (err) {
		dbg_err_fn_ret(ubus_call, err);
		return PON_ADAPTER_ERROR;
	}
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_uboot_get(struct pon_img_context *ctx,
				     const char *name, char *value,
				     const unsigned int value_size)
{
	enum pon_adapter_errno err;
	int i, len;

	dbg_in_args("%p, %s, %p, %u", ctx, name, value, value_size);

	err = uboot_get_cache_update(ctx);
	if (err != PON_ADAPTER_SUCCESS &&
	    err != PON_ADAPTER_ERR_NOT_SUPPORTED) {
		dbg_err_fn_ret(uboot_get_cache_update, err);
		return err;
	}

	for (i = 0; i < ARRAY_SIZE(uboot_cache); i++) {
		if (!uboot_cache[i].value_size)
			continue;
		if (strcmp(name, uboot_cache[i].name) != 0)
			continue;

		len = uboot_cache[i].value_size;
		dbg_prn("get %s: len %d, val %s\n",
			name, len, uboot_cache[i].value);
		if (strncpy_s(value, value_size, uboot_cache[i].value, len)) {
			dbg_err_fn(strncpy_s);
			return PON_ADAPTER_ERROR;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_err("U-Boot variable '%s' not found\n", name);
	return PON_ADAPTER_ERR_RESOURCE_NOT_FOUND;
}

static enum pon_adapter_errno _pon_uboot_set(struct pon_img_context *ctx,
					     struct blob_buf *req)
{
	int err;
	uint32_t retval = 0;

	if (ctx->ubus_reboot_only)
		return PON_ADAPTER_ERROR;

	err = ctx->pa_config->ubus_call(ctx->hl_handle,
					ctx->ubus_path,
					UBUS_METHOD_SET_UBOOTVAR,
					req->head, retval_get, &retval,
					PON_UBUS_TIMEOUT);
	if (err) {
		dbg_err_fn_ret(ubus_call, err);
		return PON_ADAPTER_ERROR;
	}
	if (retval) {
		dbg_err("ubus %s %s() failed with %d\n",
			ctx->ubus_path, UBUS_METHOD_SET_UBOOTVAR, retval);
		return PON_ADAPTER_ERROR;
	}
	/* just invalidate the cached values */
	ctx->last_ubus_ubootvars = 0;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_uboot_set_str(struct pon_img_context *ctx,
					 const char *name, const char *value)
{
	struct blob_buf req = {0, };
	enum pon_adapter_errno ret;

	dbg_prn("U-Boot variable set: '%s' to '%s'\n", name, value);

	blob_buf_init(&req, 0);
	/* we could check the "name" here, but as it is only called inside this
	 * library and only with fixed names, we trust that wrong names are
	 * ignored by the ubus method in procd.
	 * And for now the names of U-Boot variables are matching the supported
	 * parameter names in ubus.
	 */
	blobmsg_add_string(&req, name, value);

	ret = _pon_uboot_set(ctx, &req);
	blob_buf_free(&req);
	return ret;
}

enum pon_adapter_errno pon_uboot_set_bool(struct pon_img_context *ctx,
					  const char *name, bool value)
{
	struct blob_buf req = {0, };
	enum pon_adapter_errno ret;

	dbg_prn("U-Boot variable set: '%s' to '%s'\n",
		name, value ? "true" : "false");

	blob_buf_init(&req, 0);
	/* we could check the "name" here, but as it is only called inside this
	 * library and only with fixed names, we trust that wrong names are
	 * ignored by the ubus method in procd.
	 * And for now the names of U-Boot variables are matching the supported
	 * parameter names in ubus.
	 */
	blobmsg_add_u8(&req, name, value);

	ret = _pon_uboot_set(ctx, &req);
	blob_buf_free(&req);
	return ret;
}
