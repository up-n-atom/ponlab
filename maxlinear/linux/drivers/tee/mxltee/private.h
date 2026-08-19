/* SPDX-License-Identifier: GPL-2.0 */
/*****************************************************************************
 * Copyright © 2024 MaxLinear, Inc.
 *
 *****************************************************************************/

/**
 * File contains definitions of private structures like for maintainig
 * application contexts etc...
 */

#ifndef _PRIVATE__H_
#define _PRIVATE_H_

#define MAX_TEE_SESSIONS	(30)

/* Global platform error codes */
#define TEEC_SUCCESS			0x00000000
#define TEEC_ERROR_GENERIC		0xFFFF0000
#define TEEC_ERROR_BAD_PARAMETERS	0xFFFF0006
#define TEEC_ERROR_OUT_OF_MEMORY	0xFFFF000C
#define TEEC_ERROR_COMMUNICATION	0xFFFF000E
#define TEEC_ORIGIN_COMMS			0x00000002

#undef pr_fmt
#define pr_fmt(fmt)	KBUILD_MODNAME "-%s: " fmt, __func__

/*!
 * struct mxltee_driver - private data structure for MXL TEE driver
 */
struct mxltee_driver {
	struct tee_device *teedev;
	struct device *iccdev;
	struct gen_pool *iccpool;
	struct tee_shm_pool *shmpool;
	struct mutex invoke_mutex;
	uint8_t num_sessions;
};

/*!
 * struct mxltee_context - represents context for user space application
 */
struct mxltee_context {
	struct list_head sess_list;
	struct mutex sess_mutex;
};

/*!
 * struct mxltee_session - represents active session with TEP SCS trusted agent
 */
struct mxltee_session {
	struct list_head list_node;
	uint32_t session_id;
	uint8_t ta_uuid[TEE_IOCTL_UUID_LEN];
	uint8_t clnt_uuid[TEE_IOCTL_UUID_LEN];
	uint32_t clnt_login;
};

/*!
 * mxltee_register_shm_pool() - registers the shared memory pool
 */
struct tee_shm_pool *mxltee_register_shm_pool(struct mxltee_driver *driver);

/*!
 * get_session_from_session_id() - function to get session structure from session identifier
 */
struct mxltee_session *get_session_from_session_id(struct mxltee_context *ctx, uint32_t session_id);

/*!
 * scs_create_session() - function to open session with Trusted Agent
 */
int scs_create_session(struct mxltee_driver *drv, struct mxltee_session *session,
		struct tee_param *param);

/*!
 * scs_close_session() - function to close session with Trusted Agent
 */
int scs_close_session(struct mxltee_driver *drv, struct mxltee_session *session);

/*!
 * scs_invoke() - function to invoke secure crypto service commands
 */
int scs_invoke(struct mxltee_driver *drv, struct mxltee_context *ctx,
		struct tee_ioctl_invoke_arg *sess_arg, struct tee_param *param);

/*!
 * secure_sign_icc_callback() - icc callback for secure signing service
 */
void secure_sign_icc_callback(icc_wake_type wake_type);

/*!
 * print_uuid() - prints the ta and client uuid
 */
void print_uuid(const uint8_t *uuid);

/*!
 * print_icc_msg() - prints icc message
 */
void print_icc_msg(const icc_msg_t *msg);

/*!
 * tee_error_to_linux() - converts tee error to linux error code
 */
int tee_error_to_linux(uint32_t tee_error);

/*!
 * linux_error_to_tee() - converts linux error code to tee error code
 */
uint32_t linux_error_to_tee(int linux_error);
#endif //_PRIVATE_H_
