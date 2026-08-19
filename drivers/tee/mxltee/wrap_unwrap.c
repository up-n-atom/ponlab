// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/bitops.h>
#include <linux/tee_drv.h>
#include <linux/genalloc.h>
#include <linux/dma-direct.h>
#include <linux/uaccess.h>

#include <linux/icc/drv_icc.h>
#include <tep_interface.h>

#define IV_SIZE				(64/8)

static void print_wrap_unwrap_struct(struct seccrypto_wrap_unwrap_tep *wrap_unwrap)
{
	pr_debug("struct seccrypto_wrap_unwrap_tep =>\n");
	pr_debug("wrap_unwrap->input	: 0x%x\n", wrap_unwrap->input);
	pr_debug("wrap_unwrap->output	: 0x%x\n", wrap_unwrap->output);
	pr_debug("wrap_unwrap->key	: 0x%x\n", wrap_unwrap->key);
	pr_debug("wrap_unwrap->input_size	: 0x%x\n", wrap_unwrap->input_size);
	pr_debug("wrap_unwrap->output_size	: 0x%x\n", wrap_unwrap->output_size);
	pr_debug("wrap_unwrap->key_size	: 0x%x\n", wrap_unwrap->key_size);
	pr_debug("wrap_unwrap->flags	: 0x%x\n", wrap_unwrap->flags);
}

static s32 wrap_params_validate(struct seccrypto_wrap_unwrap *uwrap, struct seccrypto_wrap_unwrap_tep *wrap)
{
	if (!uwrap || !wrap) {
		pr_debug("wrap: wrap parameter are null\n");
		return -EINVAL;
	}
	if (__sw_hweight32(uwrap->flags) != 2) {
		pr_debug("wrap: wrap flags are invalid - 0x%x\n", uwrap->flags);
		return -EINVAL;
	}
	if (!uwrap->key || !uwrap->key_size) {
		if (!((uwrap->flags >> WRAP_KEY_TEE_ASSET) & 0x1)) {
			pr_debug("wrap: wrap key cannot be null or flag should be WRAP_KEY_TEE_ASSET\n");
			return -EINVAL;
		}
	}
	if (!uwrap->input || !uwrap->input_size) {
		pr_debug("wrap: wrap input cannot be null or zero\n");
		return -EINVAL;
	}
	if (!uwrap->output || !uwrap->output_size) {
		pr_debug("wrap: wrap output cannot be null or zero\n");
		return -EINVAL;
	}
	if ((uwrap->input_size + IV_SIZE) > uwrap->output_size) {
		pr_debug("wrap: insufficient output buffer size, expected:%u actual:%u\n",
				(uwrap->input_size + IV_SIZE), uwrap->output_size);
		return -EINVAL;
	}

	wrap->key_size = uwrap->key_size;
	wrap->input_size = uwrap->input_size;
	wrap->output_size = uwrap->output_size;
	wrap->flags = uwrap->flags;
	return 0;
}

static s32 unwrap_params_validate(struct seccrypto_wrap_unwrap *uunwrap, struct seccrypto_wrap_unwrap_tep *unwrap)
{
	if (!uunwrap || !unwrap) {
		pr_debug("unwrap: unwrap parameter are null\n");
		return -EINVAL;
	}
	if (__sw_hweight32(uunwrap->flags) != 3) {
		pr_debug("unwrap: unwrap flags are invalid - 0x%x\n", uunwrap->flags);
		return -EINVAL;
	}
	if (!uunwrap->key || !uunwrap->key_size) {
		if (!((uunwrap->flags >> UNWRAP_KEY_OTP) & 0x1)) {
			pr_debug("unwrap: unwrap key cannot be null or flag should be UNWRAP_KEY_OTP\n");
			return -EINVAL;
		}
	}
	if (!uunwrap->input || !uunwrap->input_size) {
		pr_debug("unwrap: unwrap input cannot be null or zero\n");
		return -EINVAL;
	}
	if (!uunwrap->output || !uunwrap->output_size) {
		pr_debug("unwrap: unwrap output cannot be null or zero\n");
		return -EINVAL;
	}
	if ((uunwrap->input_size - IV_SIZE) < uunwrap->output_size) {
		pr_debug("wrap: insufficient output buffer size, expected:%u actual:%u\n",
				(uunwrap->input_size - IV_SIZE), uunwrap->output_size);
		return -EINVAL;
	}

	unwrap->key_size = uunwrap->key_size;
	unwrap->input_size = uunwrap->input_size;
	unwrap->output_size = uunwrap->output_size;
	unwrap->flags = uunwrap->flags;
	return 0;
}

static u8 *handle_buffer_request(struct mxltee_driver *drv, u32 size,
		dma_addr_t *dma_addr)
{
	u8 *buffer;
	s32 ret;

	buffer = (u8 *)gen_pool_alloc(drv->iccpool, size);
	if (!buffer) {
		pr_debug("memory allocation failed for buffer request\n");
		return ERR_PTR(-ENOMEM);
	}

	*dma_addr = dma_map_single_attrs(drv->iccdev, buffer, size, DMA_TO_DEVICE,
			DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, *dma_addr)) {
		pr_debug("DMA mapping failed for buffer request\n");
		ret = -ENOMEM;
		goto gen_free;
	}
	return buffer;

gen_free:
	gen_pool_free(drv->iccpool, (unsigned long)buffer, size);
	return ERR_PTR(ret);
}

static u8 *handle_sst_request(struct mxltee_driver *drv,
		struct secure_storage_params *usst_params, dma_addr_t *dma_addr)
{
	struct secure_storage_params_tep *sst_params;
	s32 ret;

	sst_params = (typeof(sst_params))gen_pool_alloc(drv->iccpool, sizeof(*sst_params));
	if (!sst_params) {
		pr_debug("memory allocation failed for secure storage details\n");
		return ERR_PTR(-ENOMEM);
	}
	sst_params->handle = usst_params->handle;
	sst_params->access_perm = usst_params->access_perm;
	sst_params->policy_attr = usst_params->policy_attr;
	sst_params->crypto_mode_flag = usst_params->crypto_mode_flag;
	fill_secure_storage_params(sst_params);

	*dma_addr = dma_map_single_attrs(drv->iccdev, sst_params, sizeof(*sst_params),
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, *dma_addr)) {
		pr_debug("DMA mapping failed for secure storage details\n");
		ret = -ENOMEM;
		goto gen_free;
	}
	return (u8 *)sst_params;

gen_free:
	gen_pool_free(drv->iccpool, (unsigned long)sst_params, sizeof(*sst_params));
	return ERR_PTR(ret);
}

s32 handle_wrap_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param)
{
	struct active_session_param *active_session = NULL;
	struct seccrypto_wrap_unwrap_tep *wrap = NULL;
	struct seccrypto_wrap_unwrap *uwrap = NULL;
	dma_addr_t dma_active_session = 0x0;
	dma_addr_t dma_wrap_output = 0x0;
	dma_addr_t dma_wrap_input = 0x0;
	dma_addr_t dma_wrap_key = 0x0;
	dma_addr_t dma_wrap = 0x0;
	icc_msg_t icc_msg = {0};
	u8 *wrap_output = NULL;
	u8 *wrap_input = NULL;
	u8 *wrap_key = NULL;
	s32 ret = 0;

	active_session = get_active_scsa_session(drv, session, TA_SECURE_CRYPTO_WRAP,
			&dma_active_session);
	if (IS_ERR_OR_NULL(active_session)) {
		pr_debug("wrap: memory allocation failed for session\n");
		return PTR_ERR(active_session);
	}

	wrap = (typeof(wrap))gen_pool_alloc(drv->iccpool, sizeof(*wrap));
	if (!wrap) {
		pr_debug("wrap: memory allocation failed for wrap struct\n");
		goto gen_free;
	}
	memset(wrap, 0x0, sizeof(*wrap));

	uwrap = param[0].u.memref.shm->kaddr;
	ret =  wrap_params_validate(uwrap, wrap);
	if (ret < 0)
		goto gen_free;

	wrap_output = handle_buffer_request(drv, wrap->output_size, &dma_wrap_output);
	if (IS_ERR(wrap_output)) {
		ret = PTR_ERR(wrap_output);
		goto gen_free;
	}
	memset(wrap_output, 0x0, wrap->output_size);
	dma_sync_single_for_device(drv->iccdev, dma_wrap_output, wrap->output_size, DMA_TO_DEVICE);

	if ((wrap->flags >> WRAP_INPUT_SS_HANDLE) & 0x1) {
		wrap_input = handle_sst_request(drv, (void *)uwrap->input, &dma_wrap_input);
		if (IS_ERR(wrap_input)) {
			ret = PTR_ERR(wrap_input);
			goto dma_unmap;
		}
		dma_sync_single_for_device(drv->iccdev, dma_wrap_input,
				sizeof(struct secure_storage_params_tep), DMA_TO_DEVICE);
	} else if ((wrap->flags >> WRAP_INPUT_BUFFER) & 0x1) {
		wrap_input = handle_buffer_request(drv, wrap->input_size, &dma_wrap_input);
		if (IS_ERR(wrap_input)) {
			ret = PTR_ERR(wrap_input);
			goto dma_unmap;
		}
		memcpy(wrap_input, uwrap->input, wrap->input_size);
		dma_sync_single_for_device(drv->iccdev, dma_wrap_input, wrap->input_size,
				DMA_TO_DEVICE);
	}

	if ((wrap->flags >> WRAP_KEY_SS_HANDLE) & 0x1) {
		wrap_key = handle_sst_request(drv, (void *)uwrap->key, &dma_wrap_key);
		if (IS_ERR(wrap_key)) {
			ret = PTR_ERR(wrap_key);
			goto dma_unmap;
		}
		dma_sync_single_for_device(drv->iccdev, dma_wrap_key,
				sizeof(struct secure_storage_params_tep), DMA_TO_DEVICE);
	} else if ((wrap->flags >> WRAP_KEY_BUFFER) & 0x1) {
		wrap_key = handle_buffer_request(drv, wrap->key_size, &dma_wrap_key);
		if (IS_ERR(wrap_key)) {
			ret = PTR_ERR(wrap_key);
			goto dma_unmap;
		}
		memcpy(wrap_key, uwrap->key, wrap->key_size);
		dma_sync_single_for_device(drv->iccdev, dma_wrap_key,
				wrap->key_size, DMA_TO_DEVICE);
	} else if ((wrap->flags >> WRAP_KEY_TEE_ASSET) & 0x1) {
		wrap_key = handle_buffer_request(drv, wrap->key_size, &dma_wrap_key);
		if (IS_ERR(wrap_key)) {
			ret = PTR_ERR(wrap_key);
			goto dma_unmap;
		}
		memcpy(wrap_key, uwrap->key, wrap->key_size);
		dma_sync_single_for_device(drv->iccdev, dma_wrap_key,
				wrap->key_size, DMA_TO_DEVICE);
		pr_debug("unwrap: OTP dynamic asset number :0x%x\n", *(u32 *)wrap_key);
	}

	wrap->output = dma_wrap_output;
	wrap->input = dma_wrap_input;
	wrap->key = dma_wrap_key;

	dma_wrap = dma_map_single_attrs(drv->iccdev, wrap, sizeof(*wrap),
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_wrap)) {
		pr_debug("wrap: DMA mapping failed for wrap struct\n");
		ret = -ENOMEM;
		goto dma_unmap;
	}

	dma_sync_single_for_device(drv->iccdev, dma_wrap, sizeof(*wrap), DMA_TO_DEVICE);

	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_INVOKE_CMD;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);
	icc_msg.param[0] = dma_active_session;
	icc_msg.param[1] = dma_wrap;

	print_wrap_unwrap_struct(wrap);

	ret = icc_write_and_read(&icc_msg);
	if (ret < 0)
		goto dma_unmap;
	dma_sync_single_for_cpu(drv->iccdev, dma_wrap_output, wrap->output_size,
			DMA_TO_DEVICE);
	ret = validate_icc_reply(&icc_msg, session->session_id);
	if (ret < 0)
		goto dma_unmap;

	if (icc_msg.param[1] != wrap->output) {
		ret = -EAGAIN;
		goto dma_unmap;
	}
	wrap->output_size = icc_msg.param[2];
	uwrap->output_size = wrap->output_size;
	memcpy(uwrap->output, wrap_output, wrap->output_size);

dma_unmap:
	if (dma_wrap) {
		dma_unmap_single_attrs(drv->iccdev, dma_wrap, sizeof(*wrap), DMA_TO_DEVICE,
				DMA_ATTR_NON_CONSISTENT);
	}
	if (wrap->output) {
		dma_unmap_single_attrs(drv->iccdev, wrap->output, wrap->output_size,
				DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
		gen_pool_free(drv->iccpool, (unsigned long)wrap_output, wrap->output_size);
	}
	if (wrap->input) {
		if ((wrap->flags >> WRAP_INPUT_SS_HANDLE) & 0x1) {
			dma_unmap_single_attrs(drv->iccdev, wrap->input,
					sizeof(struct secure_storage_params_tep),
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)wrap_input,
					sizeof(struct secure_storage_params_tep));
		} else {
			dma_unmap_single_attrs(drv->iccdev, wrap->input, wrap->input_size,
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)wrap_input, wrap->input_size);
		}
	}
	if (wrap->key) {
		if ((wrap->flags >> WRAP_KEY_SS_HANDLE) & 0x1) {
			dma_unmap_single_attrs(drv->iccdev, wrap->key,
					sizeof(struct secure_storage_params_tep),
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)wrap_key,
					sizeof(struct secure_storage_params_tep));
		} else {
			dma_unmap_single_attrs(drv->iccdev, wrap->key, wrap->key_size,
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)wrap_key, wrap->key_size);
		}
	}

gen_free:
	if (wrap)
		gen_pool_free(drv->iccpool, (unsigned long)wrap, sizeof(*wrap));
	if (active_session) {
		dma_unmap_single_attrs(drv->iccdev, dma_active_session, sizeof(*active_session),
				DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
		gen_pool_free(drv->iccpool, (unsigned long)active_session, sizeof(*active_session));
	}
	return ret;
}

s32 handle_unwrap_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param)
{
	struct active_session_param *active_session = NULL;
	struct seccrypto_wrap_unwrap_tep *unwrap = NULL;
	struct seccrypto_wrap_unwrap *uunwrap = NULL;
	dma_addr_t dma_active_session = 0x0;
	dma_addr_t dma_unwrap_output = 0x0;
	dma_addr_t dma_unwrap_input = 0x0;
	dma_addr_t dma_unwrap_key = 0x0;
	dma_addr_t dma_unwrap = 0x0;
	icc_msg_t icc_msg = {0};
	u8 *unwrap_output = NULL;
	u8 *unwrap_input = NULL;
	u8 *unwrap_key = NULL;
	s32 ret = 0;

	active_session = get_active_scsa_session(drv, session, TA_SECURE_CRYPTO_UNWRAP,
			&dma_active_session);
	if (IS_ERR_OR_NULL(active_session)) {
		pr_debug("unwrap: memory allocation failed for session\n");
		return PTR_ERR(active_session);
	}

	unwrap = (typeof(unwrap))gen_pool_alloc(drv->iccpool, sizeof(*unwrap));
	if (!unwrap) {
		pr_debug("unwrap: memory allocation failed for unwrap struct\n");
		goto gen_free;
	}
	memset(unwrap, 0x0, sizeof(*unwrap));

	uunwrap = param[0].u.memref.shm->kaddr;
	ret =  unwrap_params_validate(uunwrap, unwrap);
	if (ret < 0)
		goto gen_free;
	if ((unwrap->flags >> UNWRAP_OUTPUT_BUFFER) & 0x1) {
		unwrap_output = handle_buffer_request(drv, unwrap->output_size, &dma_unwrap_output);
		if (IS_ERR(unwrap_output)) {
			ret = PTR_ERR(unwrap_output);
			goto gen_free;
		}
		memset(unwrap_output, 0x0, unwrap->output_size);
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_output, unwrap->output_size, DMA_TO_DEVICE);
	} else if ((unwrap->flags >> UNWRAP_OUTPUT_SS_HANDLE) & 0x1) {
		unwrap_output = handle_sst_request(drv, (void *)uunwrap->output, &dma_unwrap_output);
		if (IS_ERR(unwrap_output)) {
			ret = PTR_ERR(unwrap_output);
			goto dma_unmap;
		}
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_output,
				sizeof(struct secure_storage_params_tep), DMA_TO_DEVICE);
	} else if ((unwrap->flags >> UNWRAP_OUTPUT_TEE_ASSET) & 0x1) {
		unwrap_output = handle_buffer_request(drv, unwrap->output_size, &dma_unwrap_output);
		if (IS_ERR(unwrap_output)) {
			ret = PTR_ERR(unwrap_output);
			goto gen_free;
		}
		memset(unwrap_output, 0x0, unwrap->output_size);
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_output, unwrap->output_size, DMA_TO_DEVICE);
		pr_debug("unwrap: output will be stored in asset store, asset number is returned\n");
	}

	if ((unwrap->flags >> UNWRAP_INPUT_SS_HANDLE) & 0x1) {
		unwrap_input = handle_sst_request(drv, (void *)uunwrap->input, &dma_unwrap_input);
		if (IS_ERR(unwrap_input)) {
			ret = PTR_ERR(unwrap_input);
			goto dma_unmap;
		}
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_input,
				sizeof(struct secure_storage_params_tep), DMA_TO_DEVICE);
	} else if ((unwrap->flags >> UNWRAP_INPUT_BUFFER) & 0x1) {
		unwrap_input = handle_buffer_request(drv, unwrap->input_size, &dma_unwrap_input);
		if (IS_ERR(unwrap_input)) {
			ret = PTR_ERR(unwrap_input);
			goto dma_unmap;
		}
		memcpy(unwrap_input, uunwrap->input, unwrap->input_size);
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_input, unwrap->input_size,
				DMA_TO_DEVICE);
	}

	if ((unwrap->flags >> UNWRAP_KEY_SS_HANDLE) & 0x1) {
		unwrap_key = handle_sst_request(drv, (void *)uunwrap->key, &dma_unwrap_key);
		if (IS_ERR(unwrap_key)) {
			ret = PTR_ERR(unwrap_key);
			goto dma_unmap;
		}
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_key,
				sizeof(struct secure_storage_params_tep), DMA_TO_DEVICE);
	} else if ((unwrap->flags >> UNWRAP_KEY_BUFFER) & 0x1) {
		unwrap_key = handle_buffer_request(drv, unwrap->key_size, &dma_unwrap_key);
		if (IS_ERR(unwrap_key)) {
			ret = PTR_ERR(unwrap_key);
			goto dma_unmap;
		}
		memcpy(unwrap_key, uunwrap->key, unwrap->key_size);
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_key,
				unwrap->key_size, DMA_TO_DEVICE);
	} else if ((unwrap->flags >> UNWRAP_KEY_OTP) & 0x1) {
		unwrap_key = handle_buffer_request(drv, unwrap->key_size, &dma_unwrap_key);
		if (IS_ERR(unwrap_key)) {
			ret = PTR_ERR(unwrap_key);
			goto dma_unmap;
		}
		memcpy(unwrap_key, uunwrap->key, unwrap->key_size);
		dma_sync_single_for_device(drv->iccdev, dma_unwrap_key,
				unwrap->key_size, DMA_TO_DEVICE);
		pr_debug("unwrap: OTP asset number :0x%x\n", *(u32 *)unwrap_key);
	}

	unwrap->output = dma_unwrap_output;
	unwrap->input = dma_unwrap_input;
	unwrap->key = dma_unwrap_key;

	dma_unwrap = dma_map_single_attrs(drv->iccdev, unwrap, sizeof(*unwrap),
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_unwrap)) {
		pr_debug("unwrap: DMA mapping failed for unwrap struct\n");
		ret = -ENOMEM;
		goto dma_unmap;
	}

	dma_sync_single_for_device(drv->iccdev, dma_unwrap, sizeof(*unwrap), DMA_TO_DEVICE);

	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_INVOKE_CMD;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);
	icc_msg.param[0] = dma_active_session;
	icc_msg.param[1] = dma_unwrap;

	print_wrap_unwrap_struct(unwrap);

	ret = icc_write_and_read(&icc_msg);
	if (ret < 0)
		goto dma_unmap;
	dma_sync_single_for_cpu(drv->iccdev, dma_unwrap_output, unwrap->output_size,
			DMA_TO_DEVICE);
	ret = validate_icc_reply(&icc_msg, session->session_id);
	if (ret < 0)
		goto dma_unmap;

	if ((icc_msg.param[1] != unwrap->output) || (icc_msg.param[2] != unwrap->output_size)) {
		ret = -EAGAIN;
		goto dma_unmap;
	}
	memcpy(uunwrap->output, unwrap_output, unwrap->output_size);

dma_unmap:
	if (dma_unwrap) {
		dma_unmap_single_attrs(drv->iccdev, dma_unwrap, sizeof(*unwrap), DMA_TO_DEVICE,
				DMA_ATTR_NON_CONSISTENT);
	}
	if (unwrap->output) {
		if ((unwrap->flags >> UNWRAP_OUTPUT_SS_HANDLE) & 0x1) {
			dma_unmap_single_attrs(drv->iccdev, unwrap->output,
					sizeof(struct secure_storage_params_tep),
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)unwrap_output,
					sizeof(struct secure_storage_params_tep));
		} else {
			dma_unmap_single_attrs(drv->iccdev, unwrap->output, unwrap->output_size,
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)unwrap_output, unwrap->output_size);
		}
	}
	if (unwrap->input) {
		if ((unwrap->flags >> UNWRAP_INPUT_SS_HANDLE) & 0x1) {
			dma_unmap_single_attrs(drv->iccdev, unwrap->input,
					sizeof(struct secure_storage_params_tep),
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)unwrap_input,
					sizeof(struct secure_storage_params_tep));
		} else {
			dma_unmap_single_attrs(drv->iccdev, unwrap->input, unwrap->input_size,
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)unwrap_input, unwrap->input_size);
		}
	}
	if (unwrap->key) {
		if ((unwrap->flags >> UNWRAP_KEY_SS_HANDLE) & 0x1) {
			dma_unmap_single_attrs(drv->iccdev, unwrap->key,
					sizeof(struct secure_storage_params_tep),
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)unwrap_key,
					sizeof(struct secure_storage_params_tep));
		} else {
			dma_unmap_single_attrs(drv->iccdev, unwrap->key, unwrap->key_size,
					DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
			gen_pool_free(drv->iccpool, (unsigned long)unwrap_key, unwrap->key_size);
		}
	}

gen_free:
	if (unwrap)
		gen_pool_free(drv->iccpool, (unsigned long)unwrap, sizeof(*unwrap));
	if (active_session) {
		dma_unmap_single_attrs(drv->iccdev, dma_active_session, sizeof(*active_session),
				DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
		gen_pool_free(drv->iccpool, (unsigned long)active_session, sizeof(*active_session));
	}
	return ret;
}
