/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_enhanced_security_control.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

/*
 * PON Adapter wrappers and structures
 */

static enum pon_adapter_errno
encryption_get(void *ll_handle,
	       uint16_t me_id,
	       enum pon_adapter_enh_sec_ctrl_enc *enc)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_cap pon_cap;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_cap_get(pon_ctx, &pon_cap);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(pon_ret);

	if (pon_cap.features & PON_FEATURE_CRY3)
		*enc = ENC_HMAC_SHA_512;
	else if (pon_cap.features & PON_FEATURE_CRY2)
		*enc = ENC_HMAC_SHA_256;
	else if (pon_cap.features & PON_FEATURE_CRY1)
		*enc = ENC_AES_CMAC_128;
	else
		return PON_ADAPTER_ERR_DRV;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
encryption_set(void *ll_handle,
	       uint16_t me_id,
	       const enum pon_adapter_enh_sec_ctrl_enc enc)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct pon_enc_cfg enc_cfg = {0};
	uint32_t key_len;
	enum fapi_pon_errorcode pon_ret;

	UNUSED(me_id);

	/* Calculate supported key length
	 * 128 represent one data segment needed for encryption
	 */
	key_len = 128 << enc;

	/* Check if configured PSK is big enough to use */
	if ((key_len / 8) != sizeof(enc_cfg.psk))
		return PON_ADAPTER_ERR_DRV;

	if (memcpy_s(enc_cfg.psk, sizeof(enc_cfg.psk),
		     cfg->psk, sizeof(cfg->psk))) {
		dbg_err_fn(memcpy_s);
		return PON_ADAPTER_ERROR;
	}

	enc_cfg.key_size = key_len;
	/* Firmware is using X+1 value compared to OMCI */
	enc_cfg.enc_mode = enc + 1;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_auth_enc_cfg_set(pon_ctx, &enc_cfg);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(pon_ret);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
olt_rand_chl_set(void *ll_handle,
		 uint16_t me_id,
		 uint8_t *olt_rand_chl,
		 size_t len)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_generic_auth_table pon_olt_challenge_table;

	UNUSED(me_id);

	pon_olt_challenge_table.table = olt_rand_chl;
	pon_olt_challenge_table.size = len;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_auth_olt_challenge_set(pon_ctx,
						  &pon_olt_challenge_table);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(pon_ret);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
olt_auth_result_set(void *ll_handle,
		    uint16_t me_id,
		    uint8_t *olt_auth_result,
		    size_t len)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_generic_auth_table pon_olt_auth_result;

	UNUSED(me_id);

	pon_olt_auth_result.table = olt_auth_result;
	pon_olt_auth_result.size = len;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_auth_olt_result_set(pon_ctx,
					       &pon_olt_auth_result);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(pon_ret);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
msk_get(void *ll_handle,
	uint16_t me_id,
	uint8_t *msk,
	size_t *len)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_onu_msk_hash pon_onu_msk_hash;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_auth_onu_msk_hash_get(pon_ctx, &pon_onu_msk_hash);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(pon_ret);

	if (memcpy_s(msk, *len, &pon_onu_msk_hash.hash,
		     sizeof(pon_onu_msk_hash.hash))) {
		dbg_err_fn(memcpy_s);
		return PON_ADAPTER_ERROR;
	}
	*len = sizeof(pon_onu_msk_hash.hash);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
bc_key_set(void *ll_handle,
	   uint16_t me_id,
	   uint8_t *bc_key,
	   size_t len,
	   unsigned int index)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	struct pon_onu_bc_key onu_bc_key = {0};
	enum fapi_pon_errorcode pon_ret;

	UNUSED(me_id);

	onu_bc_key.table = bc_key;
	onu_bc_key.size = len;

	/* Index from adapter is zero based and fapi values starts with 1 */
	if (index != 0 && index != 1)
		return PON_ADAPTER_ERR_DRV;
	onu_bc_key.index = index + 1;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_auth_onu_bc_key_set(pon_ctx, &onu_bc_key);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(pon_ret);

	return PON_ADAPTER_SUCCESS;
}

const struct pon_adapter_enh_sec_ctrl_ops pon_pa_enh_sec_ctrl_ops = {
	.encryption_get = encryption_get,
	.encryption_set = encryption_set,
	.olt_rand_chl_set = olt_rand_chl_set,
	.olt_auth_result_set = olt_auth_result_set,
	.msk_get = msk_get,
	.bc_key_set = bc_key_set,
};

/** @} */
