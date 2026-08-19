/*****************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#include <safe_lib.h>
#include <safe_str_lib.h>
#pragma GCC diagnostic pop

#include <pon_adapter.h>
#include <pon_adapter_crc.h>
#include <omci/me/pon_adapter_sw_image.h>

#include "../pon_img_common.h"
#include "../pon_img_debug.h"
#include "pon_img.h"

/** \addtogroup PON_IMG_LIB
 *  @{
 */

/** Image default version (if no version info is located in the u-boot) */
#define SWIMAGE_DEFAULT_VERSION		"00000000000000"

/** SW Image Version length as defined by G.988 */
#define SWIMAGE_VERSION_LEN		14

/* Create a directory with the full path, like "mkdir -p" from a shell */
static void mkdir_parents(char *path)
{
	char *sep = strrchr(path, '/');

	if (sep && sep != path) {
		*sep = 0;
		mkdir_parents(path);
		*sep = '/';
	}
	if (mkdir(path, 0777) && errno != EEXIST)
		dbg_err("error while trying to create '%s': %s\n",
			path, strerror(errno));
}

static int open_mkdir(const char *path, int flags, mode_t mode)
{
	char *sep = strrchr(path, '/');

	dbg_in_args("%s, %x, %0o", path, flags, mode);

	if (sep) {
		char *path0 = strdup(path);

		if (!path0) {
			dbg_err("strdup failed\n");
			return -1;
		}
		path0[sep - path] = 0;
		mkdir_parents(path0);
		free(path0);
	}
	return open(path, flags, mode);
}

/** Preparation of image download
 *
 *  \param[in] ll_handle        Lower layer context pointer
 *  \param[in] id               SW image id
 *  \param[in] size             Size of image to download
 */
static enum pon_adapter_errno download_start(void *ll_handle,
					     const uint8_t id,
					     const uint32_t size)
{
	enum pon_adapter_errno error;
	struct pon_img_context *ctx = ll_handle;
	struct pon_image_info *image;
	const char *path = SWIMAGE_PATH;

	dbg_in_args("%p, %d, %d", ll_handle, id, size);

	if (!ctx) {
		error = PON_ADAPTER_ERR_PTR_INVALID;
		goto exit;
	}

	image = &ctx->image;

	/* prepare internal image data */

	image->fd = open_mkdir(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (image->fd < 0) {
		dbg_err("%s can not be opened\n", path);
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	image->size = size;
	image->offset = 0;
	image->next_window = 0;
	image->crc = 0xffffffff;

	error = PON_ADAPTER_SUCCESS;

exit:
	dbg_out_ret("%d", error);
	return error;
}

/** Stop the image download, for example due to error or timeout.
 *
 *  \param[in] ll_handle        Lower layer context pointer
 *  \param[in] id               SW image id
 */
static enum pon_adapter_errno download_stop(void *ll_handle,
					    const uint8_t id)
{
	enum pon_adapter_errno error;
	struct pon_img_context *ctx = ll_handle;
	struct pon_image_info *image;

	dbg_in_args("%p, %d", ll_handle, id);

	if (!ctx) {
		error = PON_ADAPTER_ERR_PTR_INVALID;
		goto exit;
	}

	image = &ctx->image;

	if (image->fd >= 0) {
		close(image->fd);
		image->fd = -1;
	}
	image->size = 0;

	error = PON_ADAPTER_SUCCESS;

exit:
	dbg_out_ret("%d", error);
	return error;
}

/** Check CRC and size of the image. If the image is ready to be stored,
 *  the file name of the temp image file is returned.
 *
 *  \param[in] ll_handle        Lower layer context pointer
 *  \param[in] id               SW image id
 *  \param[in] size             Size of downloaded image to check
 *  \param[in] crc              Image CRC to check
 *  \param[in] filepath_size    Supported temp file path size
 *  \param[out] filepath        Temp file path
 */
static enum pon_adapter_errno download_end(void *ll_handle,
					   const uint8_t id,
					   const uint32_t size,
					   const uint32_t crc,
					   const uint8_t filepath_size,
					   char *filepath)
{
	enum pon_adapter_errno error;
	struct pon_img_context *ctx = ll_handle;
	struct pon_image_info *image;
	uint8_t path_length;
	const char *path = SWIMAGE_PATH;

	dbg_in_args("%p, %d, 0x%08X, %d, %d, %p",
		    ll_handle, id, crc, size, filepath_size, filepath);

	if (!ctx) {
		error = PON_ADAPTER_ERR_PTR_INVALID;
		goto exit;
	}

	image = &ctx->image;
	path_length = strnlen_s(path, filepath_size);

	/* check defined image size */
	if (image->size != size) {
		dbg_err("Incorrect image size definition:\n");
		dbg_err("Checksize = %d, Expected = %d\n",
			size, image->size);

		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	dbg_msg("defined image size checked successfully\n");

	/* check received image size */
	if (image->size != image->offset) {
		dbg_err("Incorrect image size collected:\n");
		dbg_err("Received = %d, Expected = %d\n",
			image->offset, image->size);

		error = PON_ADAPTER_ERR_SIZE;
		goto exit;
	}

	dbg_msg("received image size checked successfully\n");

	/* check CRC */
	if ((image->crc ^ 0xffffffff) != crc) {
		dbg_err("Incorrect CRC:\n");
		dbg_err("Received = 0x%08x, Calculated = 0x%08X\n",
			crc, image->crc ^ 0xffffffff);

		error = PON_ADAPTER_ERR_CRC;
		goto exit;
	}

	dbg_msg("CRC checked successfully\n");

	/* download is finalized - ready to store */
	download_stop(ll_handle, id);

	if (filepath) {
		strncpy_s(filepath, filepath_size - 1, path, path_length);
		filepath[filepath_size - 1] = '\0';
	}

	error = PON_ADAPTER_SUCCESS;

exit:
	dbg_out_ret("%d", error);
	return error;
}

/** Concatenate a Window to the image under download.
 *
 *  \param[in] ll_handle        Lower layer context pointer
 *  \param[in] id               SW image id
 *  \param[in] window_nr        Number of Window to handle
 *  \param[in] window           Pointer to Window byte array
 *  \param[in] length           Length of Window byte array
 */
static enum pon_adapter_errno handle_window(void *ll_handle,
					    const uint8_t id,
					    const uint32_t window_nr,
					    const uint8_t *window,
					    const uint16_t length)
{
	enum pon_adapter_errno error;
	struct pon_img_context *ctx = ll_handle;
	struct pon_image_info *image;

	dbg_in_args("%p, %d, %d, %p, %d",
		    ll_handle, id, window_nr, window, length);

	if (!ctx) {
		error = PON_ADAPTER_ERR_PTR_INVALID;
		goto exit;
	}

	image = &ctx->image;

	if (image->offset >= image->size) {
		dbg_err("image size overflow: %d of %d bytes\n",
			image->offset, image->size);
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	if ((image->size - image->offset) < length) {
		dbg_err("window size failure: %d but %d bytes left\n",
			length, image->size - image->offset);
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	if (window_nr != image->next_window) {
		dbg_err("wrong window number: %d (expected: %d)\n",
			window_nr, image->next_window);
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	if (write(image->fd, window, length) < length) {
		error = PON_ADAPTER_ERROR;
		goto exit;
	}

	image->offset += length;
	image->next_window++;
	image->crc = pa_omci_crc32(image->crc, window, length);

	if ((image->offset >> 20) > ((image->offset - length) >> 20))
		dbg_msg("Image download from OLT: %d/%d MB received\n",
			image->offset >> 20, image->size >> 20);

	error = PON_ADAPTER_SUCCESS;

exit:
	dbg_out_ret("%d", error);
	return error;
}

static char part_get(const uint8_t id)
{
	switch (id) {
	case 0:
		return 'A';
	case 1:
		return 'B';
	default:
		dbg_err("OMCI specified wrong partition number! Using default.\n");
		return 'A';
	}
}

static enum pon_adapter_errno store(void *ll_handle,
				    const uint8_t id,
				    const uint8_t filepath_size,
				    const char *filepath)
{
	struct pon_img_context *ctx = ll_handle;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u, %p", ll_handle, id, filepath_size, filepath);

	ret = pon_img_upgrade(ctx, part_get(id), filepath);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno valid_get(void *ll_handle,
					const uint8_t id,
					uint8_t *valid)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERR_PTR_INVALID;
	struct pon_img_context *ctx = ll_handle;
	bool state;

	dbg_in_args("%p, %u, %p", ll_handle, id, valid);

	if (!valid)
		goto exit;

	ret = pon_img_valid_get(ctx, part_get(id), &state);
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	*valid = (state) ? 1 : 0;

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno version_get(void *ll_handle,
					  const uint8_t id,
					  const uint8_t version_size,
					  char *version)
{
	struct pon_img_context *ctx = ll_handle;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u, %p", ll_handle, id, version_size, version);

	if (version)
		ret = pon_img_version_get(ctx, part_get(id), version,
					  version_size);
	else
		ret = PON_ADAPTER_ERR_PTR_INVALID;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno commit(void *ll_handle,
				     const uint8_t id)
{
	struct pon_img_context *ctx = ll_handle;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ll_handle, id);

	ret = pon_img_commit_set(ctx, part_get(id));

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno commit_get(void *ll_handle,
					 const uint8_t id,
					 uint8_t *committed)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERR_PTR_INVALID;
	struct pon_img_context *ctx = ll_handle;
	bool state;

	dbg_in_args("%p, %u, %p", ll_handle, id, committed);

	if (!committed)
		goto exit;

	ret = pon_img_commit_get(ctx, part_get(id), &state);
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	*committed = (state) ? 1 : 0;

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno activate(void *ll_handle,
				       const uint8_t id,
				       const uint32_t timeout)
{
	struct pon_img_context *ctx = ll_handle;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u", ll_handle, id, timeout);

	ret = pon_img_active_set(ctx, part_get(id));

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno active_get(void *ll_handle,
					 const uint8_t id,
					 uint8_t *active)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERR_PTR_INVALID;
	struct pon_img_context *ctx = ll_handle;
	bool state;

	dbg_in_args("%p, %u, %p", ll_handle, id, active);

	if (!active)
		goto exit;

	ret = pon_img_active_get(ctx, part_get(id), &state);
	if (ret != PON_ADAPTER_SUCCESS)
		goto exit;

	*active = (state) ? 1 : 0;

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_sw_image_ops sw_image_ops = {
	.download_start = download_start,
	.download_stop = download_stop,
	.download_end = download_end,
	.handle_window = handle_window,

	.store = store,
	.valid_get = valid_get,
	.version_get = version_get,
	.commit = commit,
	.commit_get = commit_get,
	.activate = activate,
	.active_get = active_get,
};

/** @} */
