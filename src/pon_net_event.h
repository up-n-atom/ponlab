/*****************************************************************************
 *
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _PON_NET_EVENT_H
#define _PON_NET_EVENT_H

#include <pon_adapter_errno.h>

#include "pon_net_common.h"

#ifndef WIN32
enum pon_adapter_errno pon_net_if_event_init(struct pon_net_context *ctx);
enum pon_adapter_errno pon_net_if_event_stop(struct pon_net_context *ctx);
#else
static inline enum pon_adapter_errno
pon_net_if_event_init(struct pon_net_context *ctx)
{
	return PON_ADAPTER_SUCCESS;
}
static inline enum pon_adapter_errno
pon_net_if_event_stop(struct pon_net_context *ctx)
{
	return PON_ADAPTER_SUCCESS;
}
#endif

#endif /* _PON_NET_EVENT_H */

