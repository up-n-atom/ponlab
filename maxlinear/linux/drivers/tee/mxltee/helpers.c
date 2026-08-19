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

#include <linux/tee_drv.h>
#include <linux/errno.h>

#include <linux/icc/drv_icc.h>
#include <private.h>

struct mxltee_session *get_session_from_session_id(struct mxltee_context *ctx,
		uint32_t session_id)
{
	struct mxltee_session *session = NULL;

	list_for_each_entry(session, &ctx->sess_list, list_node) {
		if (session->session_id == session_id)
			return session;
	}
	return NULL;
}

// debug functions will be removed after feature completion
void print_uuid(const uint8_t *uuid)
{
	unsigned char buffer[37] = {0};

	snprintf(buffer, sizeof(buffer), "%x%x%x%x-%x%x%x%x-%x%x%x%x-%x%x%x%x", uuid[0],
			uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7], uuid[8],
			uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
	pr_debug("%s\n", buffer);
}

// debug functions will be removed after feature completion
void print_icc_msg(const icc_msg_t *msg)
{
	int idx;

	pr_debug("clnt id	= 0x%04x\n", msg->src_client_id);
	pr_debug("dst id	= 0x%04x\n", msg->dst_client_id);
	pr_debug("msg id	= 0x%04x\n", msg->msg_id);
	pr_debug("msg attr	= 0x%04x\n", msg->param_attr);
	for (idx = 0; idx < MAX_UPSTRM_DATAWORDS; idx++)
		pr_debug("param[%d]	= 0x%04x\n", idx, msg->param[idx]);
}

uint32_t linux_error_to_tee(int linux_error)
{
	uint32_t tee_error = TEEC_SUCCESS;

	if (linux_error > 0)
		return tee_error;

	switch (linux_error) {
	case 0:
		break;
	case -EINVAL:
		tee_error = TEEC_ERROR_BAD_PARAMETERS;
		break;
	case -ENOMEM:
		tee_error = TEEC_ERROR_OUT_OF_MEMORY;
		break;
	case -ECOMM:
		tee_error = TEEC_ERROR_COMMUNICATION;
		break;
	case -ENOTSUPP:
		tee_error = TEEC_ERROR_BAD_PARAMETERS;
		break;
	default:
		tee_error = TEEC_ERROR_GENERIC;
	}
	return tee_error;
}

int tee_error_to_linux(uint32_t tee_error)
{
	int linux_error = 0;

	switch (tee_error) {
	case TEEC_SUCCESS:
		linux_error = 0;
		break;
	case TEEC_ERROR_BAD_PARAMETERS:
		linux_error = -EINVAL;
		break;
	case TEEC_ERROR_OUT_OF_MEMORY:
		linux_error = -ENOMEM;
		break;
	case TEEC_ERROR_COMMUNICATION:
		linux_error = -ECOMM;
		break;
	case TEEC_ERROR_GENERIC:
		linux_error = -EAGAIN;
		break;
	default:
		linux_error = -tee_error;
	}
	return linux_error;
}
