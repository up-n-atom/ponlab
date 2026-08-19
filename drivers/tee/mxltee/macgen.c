// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the
 * Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/tee_drv.h>
#include <linux/genalloc.h>
#include <linux/dma-direct.h>
#include <linux/uaccess.h>

#include <linux/icc/drv_icc.h>
#include <tep_interface.h>

#define	WORD_SIZE	4
#define MAX_INPUT_SIZE	(1024U * 6)	/* 6KB */

static void print_sst_details(struct secure_storage_params_tep *sst)
{
	pr_debug("sst_handle	: 0x%llx\n", sst->handle);
	pr_debug("sst_uid		: %u\n", sst->uid);
	pr_debug("sst_gid		: %u\n", sst->gid);
	pr_debug("sst_pname		: %s\n", sst->pname);
	pr_debug("sst_perm		: %u\n", sst->access_perm);
	pr_debug("sst_policy	: %u\n", sst->policy_attr);
	pr_debug("sst_crypto	: %u\n", sst->crypto_mode_flag);
}

static s32 macgen_params_validate(struct seccrypto_mac_gen *umac_gen,
		u32 umac_gen_size, struct seccrypto_mac_gen_tep *tep_mac_gen)
{
	if (!umac_gen_size || (umac_gen_size < sizeof(*tep_mac_gen))
			|| !umac_gen || !tep_mac_gen) {
		pr_debug("macgen: MAC generate parameter is invalid\n");
		return -EINVAL;
	}
	if ((__sw_hweight32(umac_gen->flags) != 1) ||
			(umac_gen->flags < (1 << MAC_KEY_SECURE_STORE)) ||
			(umac_gen->flags > (1 << MAC_KEY_OTP))) {
		pr_debug("macgen: MAC key flags is invalid\n");
		return -EINVAL;
	}
	if ((umac_gen->mac_algo < MAC_ALGO_HMAC_SHA1) ||
			(umac_gen->mac_algo > MAC_ALGO_POLY1305)) {
		pr_debug("macgen: MAC generation algorithm:%d is not supported\n",
			umac_gen->mac_algo);
		return -ENOTSUPP;
	}
	if (!umac_gen->input || !umac_gen->input_size) {
		pr_debug("macgen: MAC input cannot be null\n");
		return -EINVAL;
	}
	if (!umac_gen->output || !umac_gen->output_size) {
		pr_debug("macgen: MAC output cannot empty buffer\n");
		return -EINVAL;
	}
	if (!umac_gen->mac_key || !umac_gen->key_size) {
		pr_debug("macgen: MAC key cannot be null\n");
		return -EINVAL;
	}
	if (umac_gen->input_size > MAX_INPUT_SIZE) {
		pr_debug("macgen: MAC input size is more than 6KB\n");
		return -EINVAL;
	}

	tep_mac_gen->mac_algo = umac_gen->mac_algo;
	tep_mac_gen->flags = umac_gen->flags;
	tep_mac_gen->input_size = umac_gen->input_size;
	tep_mac_gen->output_size = umac_gen->output_size;
	tep_mac_gen->key_size = umac_gen->key_size;

	return 0;
}

static inline void print_seccrypto_mac_gen_tep_struct(
		struct seccrypto_mac_gen_tep *tep_mac_gen)
{
	pr_debug("truct seccrypto_mac_gen_tep =>\n");
	pr_debug("tep_mac_gen->mac_algo	: 0x%x\n", tep_mac_gen->mac_algo);
	pr_debug("tep_mac_gen->input	: 0x%x\n", tep_mac_gen->input);
	pr_debug("tep_mac_gen->output	: 0x%x\n", tep_mac_gen->output);
	pr_debug("tep_mac_gen->mac_key	: 0x%x\n", tep_mac_gen->mac_key);
	pr_debug("tep_mac_gen->flags	: 0x%x\n", tep_mac_gen->flags);
	pr_debug("tep_mac_gen->input_size	: 0x%x\n",
			tep_mac_gen->input_size);
	pr_debug("tep_mac_gen->output_size	: 0x%x\n",
			tep_mac_gen->output_size);
	pr_debug("tep_mac_gen->key_size		: 0x%x\n", tep_mac_gen->key_size);
	if ((tep_mac_gen->flags >> MAC_KEY_SECURE_STORE) & 0x1)
		print_sst_details((struct secure_storage_params_tep *)
				(u64)tep_mac_gen->mac_key);
}

static inline void seccrypto_mac_gen_tep_struct_init(
		struct seccrypto_mac_gen_tep *tep_mac_gen,
		dma_addr_t dma_io_key_buffer, struct seccrypto_mac_gen *umac_gen)
{
	struct secure_storage_params_tep *sst_params;
	struct secure_storage_params *usst_params;

	tep_mac_gen->input = dma_io_key_buffer;
	tep_mac_gen->output = ALIGN(tep_mac_gen->input +
			tep_mac_gen->input_size, WORD_SIZE);
	tep_mac_gen->mac_key = ALIGN(tep_mac_gen->output +
			tep_mac_gen->output_size, WORD_SIZE);
	memcpy(bus_to_virt(tep_mac_gen->input), umac_gen->input,
			tep_mac_gen->input_size);
	memcpy(bus_to_virt(tep_mac_gen->output), umac_gen->output,
			tep_mac_gen->output_size);
	switch ((u32)tep_mac_gen->flags) {
	case (1 << MAC_KEY_SECURE_STORE):
		usst_params = (typeof(usst_params))umac_gen->mac_key;
		sst_params = (typeof(sst_params))(u64)tep_mac_gen->mac_key;
		sst_params->handle = usst_params->handle;
		sst_params->access_perm = usst_params->access_perm;
		sst_params->policy_attr = usst_params->policy_attr;
		sst_params->crypto_mode_flag = usst_params->crypto_mode_flag;
		fill_secure_storage_params(sst_params);
		break;
	case (1 << MAC_KEY_BUFFER):
	case (1 << MAC_KEY_TEE_ASSET):
	case (1 << MAC_KEY_OTP):
		memcpy(bus_to_virt(tep_mac_gen->mac_key), umac_gen->mac_key,
				tep_mac_gen->key_size);
		break;
	}
}

s32 handle_macgen_command(struct mxltee_driver *drv,
		struct mxltee_session *session, u32 num_params,
		struct tee_param *param)
{
	struct active_session_param *active_session = NULL;
	struct seccrypto_mac_gen_tep *tep_mac_gen = NULL;
	struct seccrypto_mac_gen *umac_gen = NULL;
	dma_addr_t dma_active_session = 0x0;
	dma_addr_t dma_io_key_buffer = 0x0;
	dma_addr_t dma_tep_mac_gen = 0x0;
	void *io_key_buffer = NULL;
	u32 io_key_buffer_len;
	icc_msg_t icc_msg = {0};
	u32 umac_gen_size;
	s32 ret = 0;

	active_session = get_active_scsa_session(drv, session,
			TA_SECURE_CRYPTO_GEN_MAC_SINGLE, &dma_active_session);
	if (IS_ERR_OR_NULL(active_session)) {
		pr_debug("macgen: memory allocation failed for session\n");
		return PTR_ERR(active_session);
	}

	tep_mac_gen = (void *)gen_pool_alloc(drv->iccpool,
			sizeof(*tep_mac_gen));
	if (!tep_mac_gen) {
		pr_debug("macgen: memory allocation failed for MAC generation\n");
		return -ENOMEM;
	}
	memset(tep_mac_gen, 0x0, sizeof(*tep_mac_gen));

	umac_gen = param[0].u.memref.shm->kaddr;
	umac_gen_size = param[0].u.memref.shm->size;
	ret =  macgen_params_validate(umac_gen, umac_gen_size, tep_mac_gen);
	if (ret < 0)
		goto gen_free;

	io_key_buffer_len = ALIGN(tep_mac_gen->input_size, WORD_SIZE) +
			ALIGN(tep_mac_gen->output_size, WORD_SIZE) +
			tep_mac_gen->key_size;

	io_key_buffer = (void *)gen_pool_alloc(drv->iccpool,
			io_key_buffer_len);
	if (!io_key_buffer) {
		pr_debug("macgen: memory allocation failed for io_key_buffer\n");
		ret = -ENOMEM;
		goto gen_free;
	}
	memset(io_key_buffer, 0x0, io_key_buffer_len);

	dma_io_key_buffer = dma_map_single_attrs(drv->iccdev, io_key_buffer,
			io_key_buffer_len, DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_io_key_buffer)) {
		pr_debug("macgen: DMA mapping failed for io_key_buffer\n");
		ret = -ENOMEM;
		goto gen_free;
	}

	seccrypto_mac_gen_tep_struct_init(tep_mac_gen, dma_io_key_buffer,
			umac_gen);

	print_seccrypto_mac_gen_tep_struct(tep_mac_gen);

	dma_tep_mac_gen = dma_map_single_attrs(drv->iccdev, tep_mac_gen,
			sizeof(*tep_mac_gen), DMA_BIDIRECTIONAL,
			DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_tep_mac_gen)) {
		pr_debug("macgen: DMA mapping failed for MAC generation\n");
		ret = -ENOMEM;
		goto dma_unmap;
	}

	dma_sync_single_for_device(drv->iccdev, dma_tep_mac_gen,
			sizeof(*tep_mac_gen), DMA_TO_DEVICE);
	dma_sync_single_for_device(drv->iccdev, dma_io_key_buffer,
			io_key_buffer_len, DMA_TO_DEVICE);

	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_INVOKE_CMD;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);
	icc_msg.param[0] = dma_active_session;
	icc_msg.param[1] = dma_tep_mac_gen;

	ret = icc_write_and_read(&icc_msg);
	if (ret < 0)
		goto dma_unmap;

	dma_sync_single_for_cpu(drv->iccdev, dma_io_key_buffer,
			io_key_buffer_len, DMA_FROM_DEVICE);
	dma_sync_single_for_cpu(drv->iccdev, dma_tep_mac_gen,
			sizeof(*tep_mac_gen), DMA_FROM_DEVICE);

	ret = validate_icc_reply(&icc_msg, session->session_id);
	if (ret < 0) {
		pr_debug("macgen: ICC reply is not OK\n");
		goto dma_unmap;
	}

	if (icc_msg.param[1] != tep_mac_gen->output) {
		pr_debug("macgen: MAC output buffers not matched\n");
		ret = -EINVAL;
		goto dma_unmap;
	}

	tep_mac_gen->output_size = icc_msg.param[2];
	pr_debug("macgen: MAC length:%u\n", tep_mac_gen->output_size);
	if (tep_mac_gen->output_size) {
		memcpy(umac_gen->output, bus_to_virt(tep_mac_gen->output),
				tep_mac_gen->output_size);
		umac_gen->output_size = tep_mac_gen->output_size;
	} else {
		ret = -EINVAL;
	}

dma_unmap:
	if (dma_tep_mac_gen) {
		dma_unmap_single_attrs(drv->iccdev, dma_tep_mac_gen,
				sizeof(*tep_mac_gen), DMA_TO_DEVICE,
				DMA_ATTR_NON_CONSISTENT);
	}

	if (dma_io_key_buffer) {
		dma_unmap_single_attrs(drv->iccdev, dma_io_key_buffer,
				io_key_buffer_len, DMA_FROM_DEVICE,
				DMA_ATTR_NON_CONSISTENT);
	}

gen_free:
	if (io_key_buffer) {
		gen_pool_free(drv->iccpool, (unsigned long)io_key_buffer,
				io_key_buffer_len);
	}

	if (tep_mac_gen) {
		gen_pool_free(drv->iccpool, (unsigned long)tep_mac_gen,
				sizeof(*tep_mac_gen));
	}

	if (active_session) {
		dma_sync_single_for_cpu(drv->iccdev, dma_active_session,
				sizeof(*active_session), DMA_TO_DEVICE);
		dma_unmap_single_attrs(drv->iccdev, dma_active_session,
				sizeof(*active_session), DMA_TO_DEVICE,
				DMA_ATTR_NON_CONSISTENT);
		gen_pool_free(drv->iccpool, (unsigned long)active_session,
				sizeof(*active_session));
	}

	return ret;
}
