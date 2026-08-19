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
#include <linux/tee_drv.h>
#include <linux/genalloc.h>
#include <linux/dma-direct.h>
#include <linux/uaccess.h>

#include <linux/icc/drv_icc.h>
#include <tep_interface.h>

#define RSA_SIGN_2048_LENGTH		256
#define RSA_SIGN_3072_LENGTH		384
#define RSA_SIGN_4096_LENGTH		512
#define ECDSA_SIGN_P256_LENGTH		32
#define ECDSA_SIGN_P384_LENGTH		48
#define MAX_INPUT_SIZE	4095U

static inline u32 signature_len(enum sec_alg algo)
{
	u32 sign_len = 0;

	switch (algo) {
	case SEC_ALG_RSA_2048:
		sign_len = RSA_SIGN_2048_LENGTH;
		break;
	case SEC_ALG_RSA_3072:
		sign_len = RSA_SIGN_3072_LENGTH;
		break;
	case SEC_ALG_RSA_4096:
		sign_len = RSA_SIGN_4096_LENGTH;
		break;
	case SEC_ALG_ECDSA_P256:
		sign_len = ECDSA_SIGN_P256_LENGTH;
		break;
	case SEC_ALG_ECDSA_P384:
		sign_len = ECDSA_SIGN_P384_LENGTH;
		break;
	case SEC_ALG_AES_WRAP_UNWRAP:
	case SEC_ALG_MAX:
		break;
	}
	return sign_len;
}

static s32 atom_sign_ctx_init(enum sec_alg algo, u8 *signature, u32 *ctx_len)
{
	struct ecdsa_signature *ecdsa_sign = NULL;
	struct rsa_signature *rsa_sign = NULL;
	s32 ret = -1;

	switch (algo) {
	case SEC_ALG_RSA_2048:
	case SEC_ALG_RSA_3072:
	case SEC_ALG_RSA_4096:
		rsa_sign = (struct rsa_signature *)signature;
		rsa_sign->rsa_signature.num_len = signature_len(algo);
		rsa_sign->rsa_signature.num_ptr = ((u8 *)rsa_sign) + sizeof(*rsa_sign);
		*ctx_len = sizeof(*rsa_sign);
		ret = (s32)(*ctx_len + rsa_sign->rsa_signature.num_len);
		break;
	case SEC_ALG_ECDSA_P256:
	case SEC_ALG_ECDSA_P384:
		ecdsa_sign = (struct ecdsa_signature *)signature;
		ecdsa_sign->r.num_len = signature_len(algo);
		ecdsa_sign->s.num_len = signature_len(algo);
		ecdsa_sign->r.num_ptr = (u8 *)ecdsa_sign + sizeof(*ecdsa_sign);
		ecdsa_sign->s.num_ptr = (u8 *)ecdsa_sign->r.num_ptr + ecdsa_sign->r.num_len;
		*ctx_len = sizeof(*ecdsa_sign);
		ret = (s32)(*ctx_len + ecdsa_sign->r.num_len + ecdsa_sign->s.num_len);
		break;
	case SEC_ALG_AES_WRAP_UNWRAP:
	case SEC_ALG_MAX:
		break;
	}
	return ret;
}

static s32 get_sign_ctx_and_len(enum sec_alg algo, void *signature)
{
	struct ecdsa_signature *ecdsa_sign = NULL;
	struct rsa_signature *rsa_sign = NULL;
	s32 ret = -1;

	switch (algo) {
	case SEC_ALG_RSA_2048:
	case SEC_ALG_RSA_3072:
	case SEC_ALG_RSA_4096:
		rsa_sign = (struct rsa_signature *)signature;
		rsa_sign->rsa_signature.num_len = signature_len(algo);
		ret = (s32)(sizeof(*rsa_sign) + rsa_sign->rsa_signature.num_len);
		break;
	case SEC_ALG_ECDSA_P256:
	case SEC_ALG_ECDSA_P384:
		ecdsa_sign = (struct ecdsa_signature *)signature;
		ecdsa_sign->r.num_len = signature_len(algo);
		ecdsa_sign->s.num_len = signature_len(algo);
		ret = (s32)(sizeof(*ecdsa_sign) + ecdsa_sign->r.num_len + ecdsa_sign->s.num_len);
		break;
	case SEC_ALG_AES_WRAP_UNWRAP:
	case SEC_ALG_MAX:
		break;
	}
	return ret;
}

static void tep_sign_ctx_init(enum sec_alg algo, void *buffer, dma_addr_t dma, u32 *ctx_len)
{
	struct ecdsa_signature_tep *ecdsa_sign = NULL;
	struct rsa_signature_tep *rsa_sign = NULL;

	switch (algo) {
	case SEC_ALG_RSA_2048:
	case SEC_ALG_RSA_3072:
	case SEC_ALG_RSA_4096:
		rsa_sign = (struct rsa_signature_tep *)buffer;
		rsa_sign->rsa_signature.num_len = signature_len(algo);
		rsa_sign->rsa_signature.num_ptr = dma + sizeof(*rsa_sign);
		*ctx_len = sizeof(*rsa_sign);
		break;
	case SEC_ALG_ECDSA_P256:
	case SEC_ALG_ECDSA_P384:
		ecdsa_sign = (struct ecdsa_signature_tep *)buffer;
		ecdsa_sign->r.num_len = signature_len(algo);
		ecdsa_sign->r.num_ptr = dma + sizeof(*ecdsa_sign);
		ecdsa_sign->s.num_len = signature_len(algo);
		ecdsa_sign->s.num_ptr = ecdsa_sign->r.num_ptr + ecdsa_sign->r.num_len;
		*ctx_len = sizeof(*ecdsa_sign);
		break;
	case SEC_ALG_AES_WRAP_UNWRAP:
	case SEC_ALG_MAX:
		break;
	}
}

static void copy_signature_gen2user(enum sec_alg algo, void *sign_buffer, void *signature)
{
	struct ecdsa_signature_tep *tep_ecdsa_sign = NULL;
	struct rsa_signature_tep *tep_rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	struct rsa_signature *rsa_sign = NULL;

	if ((algo == SEC_ALG_RSA_2048) || (algo == SEC_ALG_RSA_4096) || (algo == SEC_ALG_RSA_3072)) {
		rsa_sign = signature;
		tep_rsa_sign = sign_buffer;
		memcpy(rsa_sign->rsa_signature.num_ptr, bus_to_virt(tep_rsa_sign->rsa_signature.num_ptr),
				rsa_sign->rsa_signature.num_len);
	} else if ((algo == SEC_ALG_ECDSA_P256) || (algo == SEC_ALG_ECDSA_P384)) {
		ecdsa_sign = signature;
		tep_ecdsa_sign = sign_buffer;
		memcpy(ecdsa_sign->r.num_ptr, bus_to_virt(tep_ecdsa_sign->r.num_ptr),
				ecdsa_sign->r.num_len);
		memcpy(ecdsa_sign->s.num_ptr, bus_to_virt(tep_ecdsa_sign->s.num_ptr),
				ecdsa_sign->s.num_len);
	}
}

static void copy_signature_user2gen(enum sec_alg algo, void *signature, void *sign_buffer)
{
	struct ecdsa_signature_tep *tep_ecdsa_sign = NULL;
	struct rsa_signature_tep *tep_rsa_sign = NULL;
	struct ecdsa_signature *ecdsa_sign = NULL;
	struct rsa_signature *rsa_sign = NULL;

	if ((algo == SEC_ALG_RSA_2048) || (algo == SEC_ALG_RSA_4096) || (algo == SEC_ALG_RSA_3072)) {
		rsa_sign = signature;
		tep_rsa_sign = sign_buffer;
		tep_rsa_sign->rsa_signature.num_len = rsa_sign->rsa_signature.num_len;
		memcpy(bus_to_virt(tep_rsa_sign->rsa_signature.num_ptr), rsa_sign->rsa_signature.num_ptr,
				rsa_sign->rsa_signature.num_len);
	} else if ((algo == SEC_ALG_ECDSA_P256) || (algo == SEC_ALG_ECDSA_P384)) {
		ecdsa_sign = signature;
		tep_ecdsa_sign = sign_buffer;
		tep_ecdsa_sign->r.num_len = ecdsa_sign->r.num_len;
		memcpy(bus_to_virt(tep_ecdsa_sign->r.num_ptr), ecdsa_sign->r.num_ptr,
				ecdsa_sign->r.num_len);
		tep_ecdsa_sign->s.num_len = ecdsa_sign->s.num_len;
		memcpy(bus_to_virt(tep_ecdsa_sign->s.num_ptr), ecdsa_sign->s.num_ptr,
				ecdsa_sign->s.num_len);
	}
}

static void print_signature(enum sec_alg algo, u32 signature)
{
	struct ecdsa_signature_tep *ecdsa_sign = NULL;
	struct rsa_signature_tep *rsa_sign = NULL;

	if ((algo == SEC_ALG_RSA_2048) || (algo == SEC_ALG_RSA_4096) || (algo == SEC_ALG_RSA_3072)) {
		rsa_sign = bus_to_virt(signature);
		pr_debug("struct rsa_signature_tep =>\n");
		pr_debug("rsa_sign->rsa_signature.num_len: 0x%x\n", rsa_sign->rsa_signature.num_len);
		pr_debug("rsa_sign->rsa_signature.num_ptr: 0x%x\n", rsa_sign->rsa_signature.num_ptr);
	} else if ((algo == SEC_ALG_ECDSA_P256) || (algo == SEC_ALG_ECDSA_P384)) {
		ecdsa_sign = bus_to_virt(signature);
		pr_debug("struct ecdsa_signature_tep =>\n");
		pr_debug("ecdsa_sign->r.num_len:	0x%x\n", ecdsa_sign->r.num_len);
		pr_debug("ecdsa_sign->r.num_ptr:	0x%x\n", ecdsa_sign->r.num_ptr);
		pr_debug("ecdsa_sign->s.num_len:	0x%x\n", ecdsa_sign->s.num_len);
		pr_debug("ecdsa_sign->s.num_ptr:	0x%x\n", ecdsa_sign->s.num_ptr);
	}
}

static void print_sign_struct(struct seccrypto_sign_param_tep *sign)
{
	pr_debug("struct seccrypto_sign_param_tep =>\n");
	pr_debug("sign->data_ptr	: 0x%x\n", sign->data_ptr);
	pr_debug("sign->data_len	: 0x%x\n", sign->data_len);
	pr_debug("sign->sign_ptr	: 0x%x\n", sign->sign_ptr);
	pr_debug("sign->sign_len	: 0x%x\n", sign->sign_len);
	pr_debug("sign->sign_algo	: 0x%x\n", sign->sign_algo);
	pr_debug("sign->hash_flags	: 0x%x\n", sign->hash_flags);
	pr_debug("sign->hash_algo	: 0x%x\n", sign->hash_algo);
	print_signature(sign->sign_algo, sign->sign_ptr);
}

static void print_verify_struct(struct seccrypto_verify_param_tep *verify)
{
	pr_debug("struct seccrypto_verify_param_tep =>\n");
	pr_debug("verify->data_ptr	: 0x%x\n", verify->data_ptr);
	pr_debug("verify->data_len	: 0x%x\n", verify->data_len);
	pr_debug("verify->sign_ptr	: 0x%x\n", verify->sign_ptr);
	pr_debug("verify->sign_len	: 0x%x\n", verify->sign_len);
	pr_debug("verify->sign_algo	: 0x%x\n", verify->sign_algo);
	pr_debug("verify->hash_flags	: 0x%x\n", verify->hash_flags);
	pr_debug("verify->hash_algo	: 0x%x\n", verify->hash_algo);
	print_signature(verify->sign_algo, verify->sign_ptr);
}

static int sign_params_validate(struct seccrypto_sign_param *usign,
		struct seccrypto_sign_param_tep *sign)
{
	if (sizeof(*usign) < sizeof(*sign)) {
		pr_debug("sign: signing parameter is invalid\n");
		return -EINVAL;
	}
	if (!usign->data || !usign->data_len) {
		pr_debug("sign: data parameters are invalid\n");
		return -EINVAL;
	}
	if (!usign->signature || !usign->sign_len) {
		pr_debug("sign: signature parameters are invalid\n");
		return -EINVAL;
	}
	if ((usign->hash_flags < SC_FLAG_PARTIAL) || (usign->hash_flags > SC_FLAG_FINAL)) {
		pr_debug("sign: signature generation flags are invalid\n");
		return -EINVAL;
	}
	if ((usign->sign_algo < SEC_ALG_RSA_2048) || (usign->sign_algo > SEC_ALG_ECDSA_P384)) {
		pr_debug("sign: algorithm %d is not supported\n", usign->sign_algo);
		return -ENOTSUPP;
	}
	if ((usign->hash_algo < RSA_PKCS1_5_SHA1) || (usign->hash_algo > ECDSA_ASN1_SHA384)) {
		pr_debug("sign: hash algorithm %d is not supported\n", usign->hash_algo);
		return -ENOTSUPP;
	}
	if (usign->data_len > MAX_INPUT_SIZE) {
		pr_debug("sign: maximum input size for signature is 4095 Bytes\n");
		return -EINVAL;
	}

	sign->sign_algo = usign->sign_algo;
	sign->hash_algo = usign->hash_algo;
	sign->hash_flags = usign->hash_flags;
	sign->data_len = usign->data_len;
	sign->sign_len = usign->sign_len;
	return 0;
}

static s32 verify_params_validate(struct seccrypto_verify_param *uverify,
		struct seccrypto_verify_param_tep *verify)
{
	if (sizeof(*uverify) < sizeof(*verify)) {
		pr_debug("verify: verify signature parameter is invalid\n");
		return -EINVAL;
	}
	if (!uverify->data || !uverify->data_len) {
		pr_debug("verify: verify signature data parameters are invalid\n");
		return -EINVAL;
	}
	if (!uverify->signature || !uverify->sign_len) {
		pr_debug("verify: verify signature parameters are invalid\n");
		return -EINVAL;
	}
	if ((uverify->hash_flags < SC_FLAG_PARTIAL) || (uverify->hash_flags > SC_FLAG_FINAL)) {
		pr_debug("verify: signature verification flags are invalid\n");
		return -EINVAL;
	}
	if ((uverify->sign_algo < SEC_ALG_RSA_2048) || (uverify->sign_algo > SEC_ALG_ECDSA_P384)) {
		pr_debug("verify: algorithm %d is not supported\n", uverify->sign_algo);
		return -ENOTSUPP;
	}
	if ((uverify->hash_algo < RSA_PKCS1_5_SHA1) || (uverify->hash_algo > ECDSA_ASN1_SHA384)) {
		pr_debug("verify: hash algorithm %d is not supported\n", uverify->hash_algo);
		return -ENOTSUPP;
	}
	if (uverify->data_len > MAX_INPUT_SIZE) {
		pr_debug("verify: maximum input size for sign verify is 4095 Bytes\n");
		return -EINVAL;
	}

	verify->sign_algo = uverify->sign_algo;
	verify->hash_algo = uverify->hash_algo;
	verify->hash_flags = uverify->hash_flags;
	verify->data_len = uverify->data_len;
	verify->sign_len = uverify->sign_len;
	return 0;
}

s32 handle_sign_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param)
{
	struct active_session_param *active_session = NULL;
	struct seccrypto_sign_param_tep *sign = NULL;
	struct seccrypto_sign_param *usign = NULL;
	dma_addr_t dma_active_session = 0x0;
	dma_addr_t dma_sign_buffer = 0x0;
	dma_addr_t dma_sign_data = 0x0;
	dma_addr_t dma_sign = 0x0;
	void *sign_buffer = NULL;
	void *sign_data = NULL;
	icc_msg_t icc_msg = {0};
	u32 sign_buf_len = 0;
	s32 ret = 0;

	active_session = get_active_scsa_session(drv, session, TA_SECURE_CRYPTO_SIGN,
			&dma_active_session);
	if (IS_ERR_OR_NULL(active_session)) {
		pr_debug("sign: memory allocation failed for session\n");
		return PTR_ERR(active_session);
	}

	sign = (void *)gen_pool_alloc(drv->iccpool, sizeof(*sign));
	if (!sign) {
		pr_debug("sign: memory allocation failed for signature generation\n");
		return -ENOMEM;
	}
	memset(sign, 0x0, sizeof(*sign));

	usign = param[0].u.memref.shm->kaddr;
	ret =  sign_params_validate(usign, sign);
	if (ret < 0)
		goto gen_free;

	ret = atom_sign_ctx_init(usign->sign_algo, usign->signature, &usign->sign_len);
	if (ret < 0) {
		pr_debug("sign: atom context init failed\n");
		goto gen_free;
	}
	sign_buf_len = ret;

	/* move user data to genpool buffer */
	sign_data = (void *)gen_pool_alloc(drv->iccpool, usign->data_len);
	if (!sign_data) {
		pr_debug("sign: memory allocation failed for storing data data_len:%u\n", usign->data_len);
		ret = -ENOMEM;
		goto gen_free;
	}
	memcpy(sign_data, usign->data, usign->data_len);
	dma_sign_data = dma_map_single_attrs(drv->iccdev, sign_data, usign->data_len,
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_sign_data)) {
		pr_debug("sign: DMA mapping failed for signature data\n");
		goto gen_free;
	}
	sign->data_ptr = dma_sign_data;

	/* create signature context buffer for tep */
	sign_buffer = (void *)gen_pool_alloc(drv->iccpool, sign_buf_len);
	if (!sign_buffer) {
		pr_debug("sign: memory allocation failed for signature generation sign_buf_len:%d\n", sign_buf_len);
		ret = -ENOMEM;
		goto dma_unmap;
	}
	memset(sign_buffer, 0x0, sign_buf_len);

	dma_sign_buffer = dma_map_single_attrs(drv->iccdev, sign_buffer, sign_buf_len,
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_sign_buffer)) {
		pr_debug("sign: DMA mapping failed for signature generation\n");
		goto dma_unmap;
	}
	sign->sign_ptr = dma_sign_buffer;
	tep_sign_ctx_init(sign->sign_algo, sign_buffer, sign->sign_ptr, &sign->sign_len);

	dma_sign = dma_map_single_attrs(drv->iccdev, sign, sizeof(*sign),
			DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_sign)) {
		pr_debug("sign: DMA mapping failed for signature generation\n");
		ret = -ENOMEM;
		goto dma_unmap;
	}
	dma_sync_single_for_device(drv->iccdev, sign->data_ptr, sign->data_len, DMA_TO_DEVICE);
	dma_sync_single_for_device(drv->iccdev, sign->sign_ptr, sign_buf_len, DMA_TO_DEVICE);
	dma_sync_single_for_device(drv->iccdev, dma_sign, sizeof(*sign), DMA_TO_DEVICE);

	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_INVOKE_CMD;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);
	icc_msg.param[0] = dma_active_session;
	icc_msg.param[1] = dma_sign;

	ret = icc_write_and_read(&icc_msg);
	dma_sync_single_for_cpu(drv->iccdev, dma_sign_buffer, sign->sign_len, DMA_FROM_DEVICE);
	if (ret < 0)
		goto dma_unmap;

	ret = validate_icc_reply(&icc_msg, session->session_id);
	if (ret < 0)
		goto dma_unmap;

	if (icc_msg.param[1] != sign->sign_ptr) {
		ret = -EINVAL;
		goto dma_unmap;
	}
	if (icc_msg.param[2] == sign->sign_len) {
		copy_signature_gen2user(sign->sign_algo, sign_buffer, usign->signature);
	} else {
		ret = -EINVAL;
		pr_debug("sign: invalid signature length found\n");
	}
	print_sign_struct(sign);
dma_unmap:
	if (dma_sign) {
		dma_sync_single_for_cpu(drv->iccdev, dma_sign, sizeof(*sign), DMA_TO_DEVICE);
		dma_unmap_single_attrs(drv->iccdev, dma_sign, sizeof(*sign), DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	}
	if (dma_sign_buffer)
		dma_unmap_single_attrs(drv->iccdev, dma_sign_buffer, sign_buf_len, DMA_FROM_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_sign_data)
		dma_unmap_single_attrs(drv->iccdev, dma_sign_data, sign->data_len, DMA_FROM_DEVICE, DMA_ATTR_NON_CONSISTENT);

gen_free:
	if (sign_data)
		gen_pool_free(drv->iccpool, (unsigned long)sign_data, sign->data_len);
	if (sign_buffer)
		gen_pool_free(drv->iccpool, (unsigned long)sign_buffer, sign_buf_len);
	if (sign)
		gen_pool_free(drv->iccpool, (unsigned long)sign, sizeof(*sign));

	if (active_session) {
		dma_sync_single_for_cpu(drv->iccdev, dma_active_session, sizeof(*active_session),
				DMA_TO_DEVICE);
		dma_unmap_single_attrs(drv->iccdev, dma_active_session, sizeof(*active_session),
				DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
		gen_pool_free(drv->iccpool, (unsigned long)active_session, sizeof(*active_session));
	}
	return ret;
}

s32 handle_verify_command(struct mxltee_driver *drv, struct mxltee_session *session,
		u32 num_params, struct tee_param *param)
{
	struct active_session_param *active_session = NULL;
	struct seccrypto_verify_param_tep *verify = NULL;
	struct seccrypto_verify_param *uverify = NULL;
	dma_addr_t dma_active_session = 0x0;
	dma_addr_t dma_sign_buffer = 0x0;
	dma_addr_t dma_sign_data = 0x0;
	dma_addr_t dma_verify = 0x0;
	void *sign_buffer = NULL;
	void *sign_data = NULL;
	icc_msg_t icc_msg = {0};
	u32 sign_buf_len = 0;
	s32 ret = 0;

	active_session = get_active_scsa_session(drv, session, TA_SECURE_CRYPTO_VERIFY,
			&dma_active_session);
	if (IS_ERR_OR_NULL(active_session)) {
		pr_debug("verify: memory allocation failed for session\n");
		return PTR_ERR(active_session);
	}

	verify = (void *)gen_pool_alloc(drv->iccpool, sizeof(*verify));
	if (!verify) {
		pr_debug("verify: memory allocation failed for signature verification\n");
		return -ENOMEM;
	}
	memset(verify, 0x0, sizeof(*verify));

	uverify = param[0].u.memref.shm->kaddr;
	ret =  verify_params_validate(uverify, verify);
	if (ret < 0)
		goto gen_free;

	ret = get_sign_ctx_and_len(uverify->sign_algo, uverify->signature);
	if (ret < 0) {
		pr_debug("verify: get signature context failed\n");
		goto gen_free;
	}
	sign_buf_len = ret;

	/* move user data to genpool buffer */
	sign_data = (void *)gen_pool_alloc(drv->iccpool, uverify->data_len);
	if (!sign_data) {
		pr_debug("verify: memory allocation failed for storing data data_len:%u\n", uverify->data_len);
		ret = -ENOMEM;
		goto gen_free;
	}
	memcpy(sign_data, uverify->data, uverify->data_len);
	dma_sign_data = dma_map_single_attrs(drv->iccdev, sign_data, uverify->data_len,
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_sign_data)) {
		pr_debug("verify: DMA mapping failed for signature data\n");
		goto gen_free;
	}
	verify->data_ptr = dma_sign_data;

	/* create signature context buffer for tep */
	sign_buffer = (void *)gen_pool_alloc(drv->iccpool, sign_buf_len);
	if (!sign_buffer) {
		pr_debug("verify: memory allocation failed for signature verification sign_buf_len:%d\n", sign_buf_len);
		ret = -ENOMEM;
		goto dma_unmap;
	}
	memset(sign_buffer, 0x0, sign_buf_len);

	dma_sign_buffer = dma_map_single_attrs(drv->iccdev, sign_buffer, sign_buf_len,
			DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_sign_buffer)) {
		pr_debug("verify: DMA mapping failed for signature verification\n");
		goto dma_unmap;
	}
	verify->sign_ptr = dma_sign_buffer;
	tep_sign_ctx_init(verify->sign_algo, sign_buffer, verify->sign_ptr, &verify->sign_len);
	copy_signature_user2gen(verify->sign_algo, uverify->signature, sign_buffer);

	dma_verify = dma_map_single_attrs(drv->iccdev, verify, sizeof(*verify),
			DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_verify)) {
		pr_debug("verify: DMA mapping failed for signature verification\n");
		ret = -ENOMEM;
		goto dma_unmap;
	}
	dma_sync_single_for_device(drv->iccdev, verify->data_ptr, verify->data_len, DMA_TO_DEVICE);
	dma_sync_single_for_device(drv->iccdev, verify->sign_ptr, sign_buf_len, DMA_TO_DEVICE);
	dma_sync_single_for_device(drv->iccdev, dma_verify, sizeof(*verify), DMA_TO_DEVICE);

	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_INVOKE_CMD;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);
	icc_msg.param[0] = dma_active_session;
	icc_msg.param[1] = dma_verify;

	print_verify_struct(verify);

	ret = icc_write_and_read(&icc_msg);
	if (ret < 0)
		goto dma_unmap;

	ret = validate_icc_reply(&icc_msg, session->session_id);
	if (ret < 0)
		goto dma_unmap;

dma_unmap:
	if (dma_verify) {
		dma_sync_single_for_cpu(drv->iccdev, dma_verify, sizeof(*verify), DMA_TO_DEVICE);
		dma_unmap_single_attrs(drv->iccdev, dma_verify, sizeof(*verify), DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	}
	if (dma_sign_buffer)
		dma_unmap_single_attrs(drv->iccdev, dma_sign_buffer, sign_buf_len, DMA_FROM_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_sign_data)
		dma_unmap_single_attrs(drv->iccdev, dma_sign_data, verify->data_len, DMA_FROM_DEVICE, DMA_ATTR_NON_CONSISTENT);

gen_free:
	if (sign_data)
		gen_pool_free(drv->iccpool, (unsigned long)sign_data, verify->data_len);
	if (sign_buffer)
		gen_pool_free(drv->iccpool, (unsigned long)sign_buffer, sign_buf_len);
	if (verify)
		gen_pool_free(drv->iccpool, (unsigned long)verify, sizeof(*verify));

	if (active_session) {
		dma_sync_single_for_cpu(drv->iccdev, dma_active_session, sizeof(*active_session),
				DMA_TO_DEVICE);
		dma_unmap_single_attrs(drv->iccdev, dma_active_session, sizeof(*active_session),
				DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
		gen_pool_free(drv->iccpool, (unsigned long)active_session, sizeof(*active_session));
	}
	return ret;
}
