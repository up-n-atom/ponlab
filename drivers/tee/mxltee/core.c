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

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/tee_drv.h>
#include <linux/mutex.h>
#include <linux/errno.h>

#include <linux/icc/drv_icc.h>

#include <private.h>
#include "../tee_private.h"

#define DRIVER_NAME		"mxltee"
#define DRIVER_AUTHOR	"MaxLinear, Inc."
#define DRIVER_DESC		"MaxLinear's Trusted Execution Environment driver"
#define DRIVER_VERSION	"v1.0"

static struct mxltee_driver *mxltee_drv;
/**
 * mxltee_get_version() - to get MXL TEE version
 */
static void mxltee_get_version(struct tee_device *teedev, struct tee_ioctl_version_data *version)
{
	version->impl_id = TEE_IMPL_ID_MXLTEE;
	version->impl_caps = 0;
	version->gen_caps = TEE_GEN_CAP_GP;
}

/**
 * mxltee_open_context() - creates context of specific application in MXL TEE driver
 * @ctx		:	TEE context of Global Client Application
 */
static int mxltee_open_context(struct tee_context *ctx)
{
	struct mxltee_context *context = NULL;
	int ret = 0;

	context = kzalloc(sizeof(struct mxltee_context), GFP_KERNEL);
	if (IS_ERR_OR_NULL(context)) {
		ret = PTR_ERR(context);
		pr_err("context init failed err:%d\n", ret);
		return ret;
	}
	INIT_LIST_HEAD(&context->sess_list);
	mutex_init(&context->sess_mutex);
	ctx->data = context;
	return 0;
}

/**
 * mxltee_release_context() - releases the application context
 * @ctx		:	TEE context of Global Client Application
 */
static void mxltee_release_context(struct tee_context *ctx)
{
	struct mxltee_context *context = ctx->data;
	struct mxltee_session *session = NULL;
	struct mxltee_session *session_tmp = NULL;
	struct mxltee_driver *drv_data = NULL;

	drv_data = dev_get_drvdata(&ctx->teedev->dev);
	if (IS_ERR_OR_NULL(drv_data)) {
		pr_debug("private data structure cannot be null\n");
		goto error;
	}

	list_for_each_entry_safe(session, session_tmp, &context->sess_list, list_node) {
		mutex_lock(&drv_data->invoke_mutex);
		scs_close_session(drv_data, session);
		mutex_unlock(&drv_data->invoke_mutex);
		mutex_lock(&context->sess_mutex);
		list_del(&session->list_node);
		mutex_unlock(&context->sess_mutex);
		kfree(session);
	}

error:
	kfree(ctx->data);
	ctx->data = NULL;
}

/**
 * mxltee_open_session() - opens session with SCSA Trusted application, returns session handle
 * @ctx		:	TEE context of Global Client Application
 * @sess_arg	:	standard session arguments required to establish session
 * @tee_param	:	parameters required for a session
 */
static int mxltee_open_session(struct tee_context *ctx,
		struct tee_ioctl_open_session_arg *sess_arg, struct tee_param *tee_param)
{
	struct mxltee_context *context = ctx->data;
	struct mxltee_session *session = NULL;
	struct mxltee_driver *drv_data = NULL;
	uuid_t clnt_uuid;
	int ret = 0;


	session = kzalloc(sizeof(struct mxltee_session), GFP_KERNEL);
	if (IS_ERR_OR_NULL(session)) {
		ret = PTR_ERR(session);
		pr_err("open session failed ret:%d\n", ret);
		return ret;
	}

	if (sess_arg->clnt_login == TEE_IOCTL_LOGIN_PUBLIC) {
		pr_debug("public login type not supported\n");
		ret = -EINVAL;
		goto error;
	}

	ret = tee_session_calc_client_uuid(&clnt_uuid, sess_arg->clnt_login,
			sess_arg->clnt_uuid);
	if (ret)
		goto error;

	export_uuid(&session->clnt_uuid[0], &clnt_uuid);
	export_uuid(&session->ta_uuid[0], (const uuid_t *)&sess_arg->uuid[0]);

	drv_data = dev_get_drvdata(&ctx->teedev->dev);
	if (IS_ERR_OR_NULL(drv_data)) {
		ret = PTR_ERR(drv_data);
		pr_debug("private data structure cannot be null\n");
		goto error;
	}

	mutex_lock(&drv_data->invoke_mutex);
	ret = scs_create_session(drv_data, session, tee_param);
	mutex_unlock(&drv_data->invoke_mutex);
	if (ret < 0) {
		goto error;
	} else {
		sess_arg->session = session->session_id;
		sess_arg->ret_origin = TEEC_ORIGIN_COMMS;
		sess_arg->ret = TEEC_SUCCESS;
		mutex_lock(&context->sess_mutex);
		list_add(&session->list_node, &context->sess_list);
		mutex_unlock(&context->sess_mutex);
	}
	return TEEC_SUCCESS;

error:
	kfree(session);
	return ret;
}

/**
 * mxltee_close_session() - closes the active session with Trusted Application
 * @ctx		:	TEE context of Global Client Application
 * @session_id	:	session identifier
 */
static int mxltee_close_session(struct tee_context *ctx, uint32_t session_id)
{
	struct mxltee_context *context = ctx->data;
	struct mxltee_session *session = NULL;
	struct mxltee_driver *drv_data = NULL;
	int ret = 0;


	session = get_session_from_session_id(context, session_id);
	if (!session) {
		pr_err("session id:%u not found\n", session_id);
		return -EINVAL;
	}

	drv_data = dev_get_drvdata(&ctx->teedev->dev);
	if (IS_ERR_OR_NULL(drv_data)) {
		ret = PTR_ERR(drv_data);
		pr_debug("private data structure cannot be null\n");
		return -EINVAL;
	}

	mutex_lock(&drv_data->invoke_mutex);
	ret = scs_close_session(drv_data, session);
	mutex_unlock(&drv_data->invoke_mutex);
	if (ret < 0)
		return ret;

	mutex_lock(&context->sess_mutex);
	list_del(&session->list_node);
	mutex_unlock(&context->sess_mutex);
	kfree(session);

	return TEEC_SUCCESS;
}

/**
 * mxltee_invoke_function() - helps to invoke specific functions in Trusted Application
 * @ctx		:	TEE context of Global Client Application
 * @sess_arg	:	session arguements
 * @tee_param	:	parameters required for invoking function in TA
 */
static int mxltee_invoke_function(struct tee_context *ctx,
		struct tee_ioctl_invoke_arg *sess_arg, struct tee_param *tee_param)
{
	struct mxltee_context *context = ctx->data;
	struct mxltee_session *session = NULL;
	struct mxltee_driver *drv_data = NULL;
	int ret = 0;

	pr_debug("%s() session_id:%u\n", __func__, sess_arg->session);

	session = get_session_from_session_id(context, sess_arg->session);
	if (!session) {
		pr_err("session id:%u not found\n", sess_arg->session);
		return -EINVAL;
	}

	drv_data = dev_get_drvdata(&ctx->teedev->dev);
	if (IS_ERR_OR_NULL(drv_data)) {
		ret = PTR_ERR(drv_data);
		pr_debug("private data structure cannot be null\n");
		return -EINVAL;
	}

	mutex_lock(&drv_data->invoke_mutex);
	ret = scs_invoke(drv_data, context, sess_arg, tee_param);
	mutex_unlock(&drv_data->invoke_mutex);
	sess_arg->ret_origin = TEEC_ORIGIN_COMMS;
	sess_arg->ret = linux_error_to_tee(ret);
	return 0;
}

/**
 * mxltee_cancel_request() - currently not supported
 */
static int mxltee_cancel_request(struct tee_context *ctx, uint32_t cancel_id,
		uint32_t session)
{
	return -EINVAL;
}

/**
 * MXL-TEE driver supported operations.
 */
static const struct tee_driver_ops mxltee_driver_ops = {
	.get_version = mxltee_get_version,
	.open = mxltee_open_context,
	.release = mxltee_release_context,
	.open_session = mxltee_open_session,
	.close_session = mxltee_close_session,
	.invoke_func = mxltee_invoke_function,
	.cancel_req = mxltee_cancel_request,
};

/**
 * MXL-TEE device description
 */
static const struct tee_desc mxltee_desc = {
	.name = DRIVER_NAME,
	.ops = &mxltee_driver_ops,
	.owner = THIS_MODULE,
};


static int __init mxltee_driver_init(void)
{
	struct mxltee_driver *drv_data = NULL;
	int ret = -1;

	drv_data = kzalloc(sizeof(struct mxltee_driver), GFP_KERNEL);
	if (IS_ERR_OR_NULL(drv_data)) {
		pr_err("insufficient memory\n");
		ret = PTR_ERR(drv_data);
		goto error;
	}

	ret = icc_open((struct inode *)SECURE_SIGN_SERVICE, NULL);
	if (ret < 0) {
		pr_err("icc open failed for secure sign service\n");
		goto error_drv_kfree;
	}

	ret = icc_register_callback(SECURE_SIGN_SERVICE, &secure_sign_icc_callback);
	if (ret < 0) {
		pr_err("icc callback registration failed\n");
		goto error_icc_close;
	}

	drv_data->iccdev = icc_get_genpool_dev();
	if (IS_ERR_OR_NULL(drv_data->iccdev)) {
		pr_err("icc pool device get failed\n");
		ret = PTR_ERR(drv_data->iccdev);
		goto error_icc_unregister;
	}

	drv_data->iccpool = icc_get_genpool();
	if (IS_ERR_OR_NULL(drv_data->iccpool)) {
		pr_err("icc genpool get failed\n");
		ret = PTR_ERR(drv_data->iccpool);
		goto error_icc_unregister;
	}

	drv_data->shmpool = mxltee_register_shm_pool(drv_data);
	if (IS_ERR_OR_NULL(drv_data->shmpool)) {
		pr_err("shm pool registration failed\n");
		ret = PTR_ERR(drv_data->shmpool);
		goto error_icc_unregister;
	}

	drv_data->teedev = tee_device_alloc(&mxltee_desc, NULL, drv_data->shmpool, drv_data);
	if (IS_ERR_OR_NULL(drv_data->teedev)) {
		pr_err("MXL-TEE device allocation failed\n");
		ret = PTR_ERR(drv_data->teedev);
		goto error_shmpool_unregister;
	}

	ret = tee_device_register(drv_data->teedev);
	if (ret) {
		pr_err("MXL-TEE device registration failed\n");
		ret = -EFAULT;
		goto error_tdev_unregister;
	}

	mutex_init(&drv_data->invoke_mutex);
	mxltee_drv = drv_data;
	pr_info("MXL-TEE driver initialized\n");
	return 0;

error_tdev_unregister:
	tee_device_unregister(drv_data->teedev);
error_shmpool_unregister:
	tee_shm_pool_free(drv_data->shmpool);
error_icc_unregister:
	icc_unregister_callback(SECURE_SIGN_SERVICE);
error_icc_close:
	icc_close((struct inode *)SECURE_SIGN_SERVICE, NULL);
error_drv_kfree:
	kfree(drv_data);
error:
	pr_err("MXL-TEE driver init failed ret:%d\n", ret);
	return ret;
}
module_init(mxltee_driver_init);


static void __exit mxltee_driver_exit(void)
{
	if (IS_ERR_OR_NULL(mxltee_drv)) {
		if (mxltee_drv->teedev)
			tee_device_unregister(mxltee_drv->teedev);
		icc_unregister_callback(SECURE_SIGN_SERVICE);
		icc_close((struct inode *)SECURE_SIGN_SERVICE, NULL);
		if (mxltee_drv->shmpool)
			tee_shm_pool_free(mxltee_drv->shmpool);
		kfree(mxltee_drv);
	}

	pr_info("MXL-TEE driver uninitialized\n");
}
module_exit(mxltee_driver_exit);


MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL v2");
