/******************************************************************************
 *
 * Copyright (c) 2022 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <libubox/blobmsg.h>
#include <pon_adapter_config.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#include <safe_lib.h>
#include <safe_mem_lib.h>
#include <safe_str_lib.h>
#pragma GCC diagnostic pop

/* libubus include needed for struct ubus_request */
#include <libubus.h>

#include "pon_img.h"
#include "pon_img_common.h"
#include "pon_uboot.h"
#include "pon_img_debug.h"

#define DEFAULT_VERSION "0.0"

#ifdef EXTRA_VERSION
#define pon_extra_ver_str "." EXTRA_VERSION
#else
#define pon_extra_ver_str ""
#endif

/* what string support, version string */
const char pon_img_lib_whatversion[] =
	"@(#)MaxLinear PON Image Library, Version "
	PACKAGE_VERSION pon_extra_ver_str;

/** \addtogroup PON_IMG_LIB
 *  @{
 */

/* convert from a character id to a boolean */
static bool get_id_bool(char id)
{
	switch (id) {
	case 0:
	case 'A':
		return false;
	case 1:
	case 'B':
		return true;
	}
	return false;
}

/* convert from a character id to a string with A or B */
static const char *get_id_str(char id)
{
	return get_id_bool(id) ? "B" : "A";
}

static int copy_file(const char *dest_file, const char *src_file)
{
	int in_fd = -1, out_fd = -1;
	char buf[8192];
	int ret = -1;

	in_fd = open(src_file, O_RDONLY);
	if (in_fd < 0)
		goto exit;
	out_fd = open(dest_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (out_fd < 0)
		goto exit;

	while (1) {
		ret = read(in_fd, &buf[0], sizeof(buf));
		if (ret <= 0)
			break;
		ret = write(out_fd, &buf[0], ret);
		if (ret < 0)
			break;
	}

exit:
	if (in_fd >= 0)
		close(in_fd);
	if (out_fd >= 0)
		close(out_fd);
	return ret;
}

static const struct blobmsg_policy retval_get_policy[] = {
	{ .name = "retval", .type = BLOBMSG_TYPE_INT32 },
	{ .name = "write_retval", .type = BLOBMSG_TYPE_INT32 },
	{ .name = "img_write_retval", .type = BLOBMSG_TYPE_INT32 },
};

void retval_get(struct ubus_request *req, int type, struct blob_attr *msg)
{
	uint32_t *retval = req->priv;
	struct blob_attr *tb[ARRAY_SIZE(retval_get_policy)];
	int i;

	(void)type; /* unused */

	/* default return value */
	*retval = 0;

	blobmsg_parse(retval_get_policy, ARRAY_SIZE(retval_get_policy), tb,
		      blob_data(msg), blob_len(msg));

	for (i = 0; i < ARRAY_SIZE(retval_get_policy); i++) {
		if (!tb[i])
			continue;

		/** return when first expected attribute is found */
		*retval = blobmsg_get_u32(tb[i]);
		return;
	}
}

enum pon_adapter_errno pon_img_upgrade(struct pon_img_context *ctx,
				       const char id, const char *filename)
{
	int err;
	struct blob_buf req = {0, };
	uint32_t retval = 0;

	dbg_in_args("%c, %p", id, filename);

	if (ctx->ubus_reboot_only)
		return PON_ADAPTER_ERROR;

	/* is the file in the expected location? */
	if (strcmp(SWIMAGE_PATH, filename) != 0) {
		err = copy_file(SWIMAGE_PATH, filename);
		if (err < 0) {
			dbg_err_fn_ret(copy_file, err);
			return PON_ADAPTER_ERROR;
		}
	}

	blob_buf_init(&req, 0);
	blobmsg_add_u8(&req, "noreboot", 1);
	blobmsg_add_string(&req, "bank", get_id_str(id));
	blobmsg_add_string(&req, "image_name", SWIMAGE_NAME);

	err = ctx->pa_config->ubus_call(ctx->hl_handle,
					ctx->ubus_path, UBUS_METHOD_UPGRADE,
					req.head, retval_get, &retval,
					UBUS_TIMEOUT_UPGRADE);
	blob_buf_free(&req);
	if (err) {
		dbg_err_fn_ret(ubus_call, err);
		return PON_ADAPTER_ERROR;
	}
	if (retval) {
		dbg_err("ubus %s %s() failed with %d\n",
			ctx->ubus_path, UBUS_METHOD_UPGRADE, retval);
		return PON_ADAPTER_ERROR;
	}
	/* The "upgrade" call will also change U-Boot variables,
	 * so drop current values from cache.
	 */
	ctx->last_ubus_ubootvars = 0;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_img_active_set(struct pon_img_context *ctx,
					  const char id)
{
	enum pon_adapter_errno ret;
	struct blob_buf req = {0, };
	int err;
	uint32_t retval = 0;

	if (ctx->ubus_reboot_only)
		return PON_ADAPTER_ERROR;

	ret = PON_ADAPTER_SUCCESS;

	blob_buf_init(&req, 0);
	blobmsg_add_string(&req, "bank", get_id_str(id));

	err = ctx->pa_config->ubus_call(ctx->hl_handle,
					ctx->ubus_path, "img_activate",
					req.head, retval_get, &retval,
					PON_UBUS_TIMEOUT);
	blob_buf_free(&req);
	if (err) {
		dbg_err_fn_ret(ubus_call, err);
		ret = PON_ADAPTER_ERROR;
		goto exit;
	}
	if (retval) {
		dbg_err("ubus %s img_activate() failed with %d\n",
			ctx->ubus_path, retval);
		return PON_ADAPTER_ERROR;
	}

	/* This call will change an U-Boot variable,
	 * so drop current values from cache.
	 */
	ctx->last_ubus_ubootvars = 0;

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_img_active_get(struct pon_img_context *ctx,
					  const char id, bool *active)
{
	char uboot_val[UBOOT_VAL_LEN_MAX];
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;

	dbg_in_args("%c, %p", id, active);

	if (!active)
		goto exit;

	ret = pon_uboot_get(ctx, UBOOT_VAR_IMG_ACTIVE, uboot_val,
			    sizeof(uboot_val));
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	/*
	 * If the variable active_bank is not set, assume that the first
	 * image (A) is active. This is a workaround for some systems which
	 * do not store an image version like URX. Some OLTs need a version.
	 */
	if (strnlen_s(uboot_val, sizeof(uboot_val)) == 0) {
		if (strcmp(get_id_str(id), "A"))
			*active = true;
		else
			*active = false;
	} else {
		if (strcmp(get_id_str(id), uboot_val) == 0)
			*active = true;
		else
			*active = false;
	}

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_img_commit_set(struct pon_img_context *ctx,
					  const char id)
{
	enum pon_adapter_errno ret;
	char var[UBOOT_VAL_LEN_MAX];

	dbg_in_args("%c", id);

	snprintf(var, UBOOT_VAL_LEN_MAX, "%c", id);

	ret = pon_uboot_set_str(ctx, UBOOT_VAR_IMG_COMMIT, var);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_uboot_set_str, ret);
		goto exit;
	}

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_img_commit_get(struct pon_img_context *ctx,
					  const char id, bool *committed)
{
	char uboot_val[UBOOT_VAL_LEN_MAX];
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;

	dbg_in_args("%c, %p", id, committed);

	if (!committed)
		goto exit;

	ret = pon_uboot_get(ctx, UBOOT_VAR_IMG_COMMIT, uboot_val,
			    sizeof(uboot_val));
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	if (strcmp(get_id_str(id), uboot_val) == 0)
		*committed = true;
	else
		*committed = false;

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_img_version_set(struct pon_img_context *ctx,
					   const char id, const char *buff)
{
	char var[UBOOT_VAL_LEN_MAX];
	enum pon_adapter_errno ret;

	dbg_in_args("%c %s", id, buff);

	snprintf(var, UBOOT_VAL_LEN_MAX, "%s%c", UBOOT_VAR_IMG_VERSION, id);

	ret = pon_uboot_set_str(ctx, var, buff);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_uboot_set_str, ret);
		goto exit;
	}

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

#define min(a, b) ((a) < (b) ? (a) : (b))

enum pon_adapter_errno pon_img_version_get(struct pon_img_context *ctx,
					   const char id, char *buff,
					   const uint8_t len)
{
	char var[UBOOT_VAL_LEN_MAX];
	char uboot_val[UBOOT_VAL_LEN_MAX] = { 0 };
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;
	size_t count;

	dbg_in_args("%c %s %u", id, buff, len);

	if (!buff)
		goto exit;

	memset(buff, 0, len);

	snprintf(var, UBOOT_VAL_LEN_MAX, "%s%c", UBOOT_VAR_IMG_VERSION, id);

	ret = pon_uboot_get(ctx, var, uboot_val, sizeof(uboot_val));
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	/*
	 * Set version to 0.0 if version field is empty. This is a workaround
	 * for some systems which do not store an image version like URX.
	 * Some OLTs need a version.
	 */
	if (strnlen_s(uboot_val, sizeof(uboot_val)) == 0) {
		count = min(sizeof(DEFAULT_VERSION), len);
		if (memcpy_s(buff, len, DEFAULT_VERSION, count))
			ret = PON_ADAPTER_ERR_MEM_ACCESS;
	} else {
		count = min(sizeof(uboot_val), len);
		if (memcpy_s(buff, len, uboot_val, count))
			ret = PON_ADAPTER_ERR_MEM_ACCESS;
	}
	if (count < len)
		buff[count] = '\x00';

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_img_valid_set(struct pon_img_context *ctx,
					 const char id, const bool valid)
{
	char var[UBOOT_VAL_LEN_MAX];
	enum pon_adapter_errno ret;

	dbg_in_args("%c %d", id, valid);

	snprintf(var, UBOOT_VAL_LEN_MAX, "%s%c", UBOOT_VAR_IMG_VALID, id);

	ret = pon_uboot_set_bool(ctx, var, valid);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_uboot_set_bool, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_img_valid_get(struct pon_img_context *ctx,
					 const char id, bool *valid)
{
	char var[UBOOT_VAL_LEN_MAX];
	char uboot_val[UBOOT_VAL_LEN_MAX];
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;

	dbg_in_args("%c", id);

	if (!valid)
		goto exit;

	snprintf(var, UBOOT_VAL_LEN_MAX, "%s%c", UBOOT_VAR_IMG_VALID, id);

	ret = pon_uboot_get(ctx, var, uboot_val, sizeof(uboot_val));
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	if (strcmp("true", uboot_val) == 0)
		*valid = true;
	else
		*valid = false;

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

/** @} */
