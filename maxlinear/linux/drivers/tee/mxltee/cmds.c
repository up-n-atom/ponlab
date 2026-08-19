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
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/tee_drv.h>
#include <linux/genalloc.h>
#include <linux/dma-direct.h>

#include <linux/icc/drv_icc.h>
#include <tep_interface.h>

#define SCSA_ICC_MSG_READ		(1)
#define SCSA_ICC_MSG_NOT_READ	(0)
DECLARE_WAIT_QUEUE_HEAD(icc_msg_arrived);
static atomic_t icc_msg_read = ATOMIC_INIT(SCSA_ICC_MSG_READ);

/*
 * Function pointer for invoking TEE functions
 */
typedef s32 (*invoke_tee_function)(struct mxltee_driver *drv,
		struct mxltee_session *session, u32 num_params, struct tee_param *param);
static invoke_tee_function invoke_tee_func[] = {
	handle_sign_command,
	handle_verify_command,
	NULL,
	NULL,
	handle_keyload_command,
	handle_keygen_command,
	handle_wrap_command,
	handle_unwrap_command,
	handle_macgen_command,
};

int get_hash_algo(enum sec_alg algo)
{
	enum sign_flag hash_algo = SEC_ALG_MAX;

	switch (algo) {
	case SEC_ALG_RSA_2048:
	case SEC_ALG_RSA_3072:
	case SEC_ALG_RSA_4096:
		hash_algo = RSA_PSS_SHA384;
		break;
	case SEC_ALG_ECDSA_P256:
		hash_algo = ECDSA_ASN1_SHA256;
		break;
	case SEC_ALG_ECDSA_P384:
		hash_algo = ECDSA_ASN1_SHA384;
		break;
	case SEC_ALG_AES_WRAP_UNWRAP:
	case SEC_ALG_MAX:
	default:
		pr_debug("sign: algorithm %d is not supported\n", algo);
		break;
	}
	return hash_algo;
}

void fill_secure_storage_params(struct secure_storage_params_tep *params)
{
	const struct cred *cred = NULL;

	cred = current_cred();
	params->uid = cred->uid.val;
	params->gid = cred->gid.val;
	strncpy(params->pname, current->comm, TASK_COMM_LEN - 1);
}

int icc_write_and_read(icc_msg_t *icc_msg)
{
	int ret;

	pr_debug("ICC command request\n");
	print_icc_msg((const icc_msg_t *)icc_msg);

	ret = icc_write(SECURE_SIGN_SERVICE, icc_msg);
	if (ret < 0) {
		pr_debug("failed to write icc or open session ret:%d\n", ret);
		return -ECONNREFUSED;
	}

	ret = wait_event_interruptible_timeout(icc_msg_arrived, !atomic_read(&icc_msg_read), msecs_to_jiffies(500 * HZ));
	if (ret == 0) {
		pr_err("secure timeout ret:%d\n", ret);
		return -ECONNREFUSED;
	}

	memset(icc_msg, 0x0, sizeof(icc_msg_t));

	ret = icc_read(SECURE_SIGN_SERVICE, icc_msg);
	if (ret < 0) {
		pr_debug("failed to read icc message for open session ret:%d\n", ret);
		return -ETIMEDOUT;
	}
	atomic_set(&icc_msg_read, SCSA_ICC_MSG_READ);

	pr_debug("ICC command response\n");
	print_icc_msg((const icc_msg_t *)icc_msg);
	return ret;
}

int validate_icc_reply(icc_msg_t *icc_msg, uint32_t session_id)
{
	int ret = TEEC_SUCCESS;

	if (icc_msg->msg_id != ICC_CMD_ID_INVOKE_CMD_REPLY) {
		ret = -EINVAL;
	} else {
		ret = tee_error_to_linux(icc_msg->param_attr);
		if (ret < 0)
			return ret;
	}

	if (icc_msg->param[0] != session_id)
		ret = -EINVAL;
	return ret;
}

struct active_session_param *get_active_scsa_session(struct mxltee_driver *drv,
		struct mxltee_session *session, uint32_t sub_command, dma_addr_t *dma_active_session)
{
	struct active_session_param *active_session = NULL;
	int ret = 0;

	active_session = (void *)gen_pool_alloc(drv->iccpool, sizeof(*active_session));
	if (!active_session) {
		pr_debug("memory allocation failed for key generation\n");
		return ERR_PTR(-ENOMEM);
	}
	memset(active_session, 0x0, sizeof(*active_session));

	memcpy(&active_session->client_uuid[0], &session->clnt_uuid[0], TEE_IOCTL_UUID_LEN);
	memcpy(&active_session->scsa_uuid[0], &session->ta_uuid[0], TEE_IOCTL_UUID_LEN);
	active_session->session_id = session->session_id;
	active_session->sub_cmd = sub_command;

	*dma_active_session = dma_map_single_attrs(drv->iccdev, active_session, sizeof(*active_session),
			DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, *dma_active_session)) {
		pr_debug("DMA mapping failed for key generation\n");
		ret = -ENOMEM;
		goto gen_free;
	}
	dma_sync_single_for_device(drv->iccdev, *dma_active_session, sizeof(*active_session), DMA_TO_DEVICE);
	return active_session;

gen_free:
	gen_pool_free(drv->iccpool, (unsigned long)active_session, sizeof(struct active_session_param));
	return ERR_PTR(ret);
}

int scs_create_session(struct mxltee_driver *drv, struct mxltee_session *session,
		struct tee_param *param)
{
	struct open_session_param *sess_uuid = NULL;
	icc_msg_t icc_msg = {0};
	dma_addr_t dma_sess_uuid;
	int ret = 0;


	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_OPEN_SESSION;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);

	sess_uuid = (void *)gen_pool_alloc(drv->iccpool, sizeof(struct open_session_param));
	if (!sess_uuid) {
		pr_debug("memory allocation failed for open session\n");
		return -ENOMEM;
	}
	memcpy(&sess_uuid->client_uuid[0], &session->clnt_uuid[0], TEE_IOCTL_UUID_LEN);
	memcpy(&sess_uuid->scsa_uuid[0], &session->ta_uuid[0], TEE_IOCTL_UUID_LEN);

	pr_debug("ta-uuid: ");
	print_uuid((const uint8_t *)&session->ta_uuid[0]);
	pr_debug("client-uuid: ");
	print_uuid((const uint8_t *)&session->clnt_uuid[0]);

	dma_sess_uuid = dma_map_single_attrs(drv->iccdev, sess_uuid, sizeof(struct open_session_param),
			DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_sess_uuid)) {
		pr_debug("DMA mapping failed for open session\n");
		ret = -ENOMEM;
		goto gen_free;
	}
	dma_sync_single_for_device(drv->iccdev, dma_sess_uuid, sizeof(struct open_session_param), DMA_TO_DEVICE);

	icc_msg.param[0] = (unsigned long)dma_sess_uuid;

	ret = icc_write_and_read(&icc_msg);
	if (ret < 0)
		goto gen_free;
	if (icc_msg.msg_id != ICC_CMD_ID_OPEN_SESSION_REPLY) {
		ret = -EINVAL;
	} else {
		ret = (int)icc_msg.param_attr;
		ret = ret ? -ret : ret;
	}
	session->session_id = icc_msg.param[0];
gen_free:
	if (sess_uuid)
		gen_pool_free(drv->iccpool, (unsigned long)sess_uuid, sizeof(struct open_session_param));
	dma_sync_single_for_cpu(drv->iccdev, dma_sess_uuid, sizeof(struct open_session_param), DMA_TO_DEVICE);
	dma_unmap_single_attrs(drv->iccdev, dma_sess_uuid, sizeof(struct open_session_param), DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	return ret;
}

int scs_close_session(struct mxltee_driver *drv, struct mxltee_session *session)
{
	struct close_session_param *close_session = NULL;
	icc_msg_t icc_msg = {0};
	dma_addr_t dma_close_session;
	int ret = 0;


	icc_msg.src_client_id = SECURE_SIGN_SERVICE;
	icc_msg.dst_client_id = SECURE_SIGN_SERVICE;
	icc_msg.msg_id = ICC_CMD_ID_CLOSE_SESSION;
	icc_msg.param_attr = ICC_PARAM_PTR | (ICC_PARAM_PTR_NON_IOCU << 1);

	close_session = (void *)gen_pool_alloc(drv->iccpool, sizeof(struct close_session_param));
	if (!close_session) {
		pr_debug("memory allocation failed for close session\n");
		return -ENOMEM;
	}
	memcpy(&close_session->client_uuid[0], &session->clnt_uuid[0], TEE_IOCTL_UUID_LEN);
	memcpy(&close_session->scsa_uuid[0], &session->ta_uuid[0], TEE_IOCTL_UUID_LEN);
	close_session->session_id = session->session_id;

	pr_debug("ta-uuid: ");
	print_uuid((const uint8_t *)&session->ta_uuid[0]);
	pr_debug("client-uuid: ");
	print_uuid((const uint8_t *)&session->clnt_uuid[0]);

	dma_close_session = dma_map_single_attrs(drv->iccdev, close_session, sizeof(struct close_session_param),
			DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(drv->iccdev, dma_close_session)) {
		pr_debug("DMA mapping failed for open session\n");
		ret = -ENOMEM;
		goto gen_free;
	}
	dma_sync_single_for_device(drv->iccdev, dma_close_session, sizeof(struct close_session_param), DMA_TO_DEVICE);

	icc_msg.param[0] = (unsigned long)dma_close_session;

	ret = icc_write_and_read(&icc_msg);
	if (ret < 0)
		goto gen_free;
	if (icc_msg.msg_id != ICC_CMD_ID_CLOSE_SESSION_REPLY) {
		ret = -ECOMM;
	} else {
		ret = (int)icc_msg.param_attr;
		ret = ret ? -ret : ret;
	}
gen_free:
	if (close_session)
		gen_pool_free(drv->iccpool, (unsigned long)close_session, sizeof(struct close_session_param));
	dma_sync_single_for_cpu(drv->iccdev, dma_close_session, sizeof(struct close_session_param), DMA_TO_DEVICE);
	dma_unmap_single_attrs(drv->iccdev, dma_close_session, sizeof(struct close_session_param), DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	return ret;
}

int scs_invoke(struct mxltee_driver *drv, struct mxltee_context *ctx,
		struct tee_ioctl_invoke_arg *sess_arg, struct tee_param *param)
{
	struct mxltee_session *session = NULL;
	int ret = 0;

	if (!sess_arg || (!param && sess_arg->num_params))
		return -EINVAL;

	session = get_session_from_session_id(ctx, sess_arg->session);
	if (!session) {
		pr_debug("session_id:%d is invalid\n", sess_arg->session);
		return -EINVAL;
	}
	if (invoke_tee_func[sess_arg->func]) {
		pr_debug("TEE function:0x%x invoked\n", sess_arg->func);
		ret = invoke_tee_func[sess_arg->func](drv, session, sess_arg->num_params,
				param);
	} else {
		pr_debug("TEE function:0x%x is not supported\n", sess_arg->func);
		ret = -ENOTSUPP;
	}
	return ret;
}

void secure_sign_icc_callback(icc_wake_type wake_type)
{
	if (wake_type & ICC_READ) {
		atomic_set(&icc_msg_read, SCSA_ICC_MSG_NOT_READ);
		wake_up_interruptible(&icc_msg_arrived);
	}
}
