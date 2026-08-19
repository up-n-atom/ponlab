/******************************************************************************
 *
 *  Copyright (c) 2020 - 2022 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#include <stdio.h>
#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "pon_adapter_event_handlers.h"
#include "omci/pon_adapter_standard_me.h"
#include "omci/pon_adapter_meter.h"
#include "omci/pon_adapter_mcc.h"
#include "omci/pon_adapter_mib.h"
#include "omci/pon_adapter_msg.h"

/* Lower Layer */
static enum pon_adapter_errno omci_ll_msg_send(void *ll_handle,
	const uint8_t *msg,
	const uint16_t len,
	const uint32_t *crc)
{
	printf("LL Message sending.\n");

	return 1;
}

static enum pon_adapter_errno omci_ll_register_msg_rcv_cb(
	void *ll_handle,
	enum pon_adapter_errno (*receive_callback)(
		void *hl_handle,
		const uint8_t *msg,
		const uint16_t len,
		const uint32_t *crc),
	void *handle)
{
	printf("LL Register message receive cb.\n");

	return 1;
}

static const struct pa_msg_ops ll_msg_ops = {
	.msg_send = omci_ll_msg_send,
	.msg_rx_cb_register = omci_ll_register_msg_rcv_cb,
};

/* Higher Layer */
static struct pa_msg_ops hl_msg_ops;

static enum pon_adapter_errno omci_hl_register_msg_ops(void *hl_handle,
	const struct pa_msg_ops *ops, void *ll_handle)
{
	hl_msg_ops.msg_send = ops->msg_send;
	hl_msg_ops.msg_rx_cb_register = ops->msg_rx_cb_register;

	return PON_ADAPTER_SUCCESS;
}

int main(int argc, char **argv)
{
	printf("Hello world\n");

	(void)omci_hl_register_msg_ops(NULL, &ll_msg_ops, NULL);
	(void)hl_msg_ops.msg_send(NULL, NULL, 10, NULL);
	(void)hl_msg_ops.msg_rx_cb_register(NULL, NULL, NULL);

	return 0;
}
