/*****************************************************************************
 *
 * Copyright (c) 2021 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>
#include <pon_adapter_system.h>
#include <pon_adapter_config.h>
#include <omci/pon_adapter_omci.h>

#include <unistd.h>       /* for getpid */
#include <ifxos_thread.h>
#include <ifxos_time.h>   /* for IFXOS_MSecSleep */
#include <libubus.h> /* for UBUS enum errors */

#include "pon_img_register.h"
#include "pon_img_common.h"
#include "pon_img_debug.h"

#define IFXOS_THREAD_PRIO_LOWEST	5

/** \addtogroup PON_IMG_LIB
 *  @{
 */

/** ONU reboot thread control structure */
static IFXOS_ThreadCtrl_t pon_img_reboot_thread_control;

static struct pon_img_context g_pon_img_context;

/** List which holds supported UBUS interfaces */
static const char * const pon_img_list_of_path[] = {"fwupgrade", UBUS_SYSTEM_PATH};

static enum pon_adapter_errno
pon_img_init(char const * const *init_data,
	     const struct pa_config *pa_config,
	     const struct pa_eh_ops *event_handler,
	     void *ll_handle)
{
	struct pon_img_context *ctx = ll_handle;

	dbg_in_args("%p, %p, %p, %p", init_data, pa_config, event_handler,
		    ll_handle);

	ctx->pa_config = pa_config;

	if (!ctx->pa_config->ubus_call) {
		dbg_err("No ubus_call callback provided, pon-img-lib needs it\n");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_img_start(void *ll_handle)
{
	struct pon_img_context *ctx = ll_handle;
	int err, i;

	dbg_in_args("%p", ll_handle);
	for (i = 0; i < ARRAY_SIZE(pon_img_list_of_path); i++) {
		err = ctx->pa_config->ubus_call(ctx->hl_handle,
				pon_img_list_of_path[i],
				UBUS_METHOD_GET_UBOOTVARS,
				NULL, NULL, NULL, PON_UBUS_TIMEOUT);
		if (err == PON_ADAPTER_SUCCESS) {
			ctx->ubus_path = pon_img_list_of_path[i];
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	if (err == UBUS_STATUS_METHOD_NOT_FOUND) {
		/* reboot is expected to work in any case */
		dbg_prn("Only system reboot supported.\n");
		ctx->ubus_reboot_only = true;
		ctx->ubus_path = UBUS_SYSTEM_PATH;
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_err_fn_ret(ubus_call, err);
	dbg_out_ret("%d", PON_ADAPTER_ERROR);
	return PON_ADAPTER_ERROR;
}

/** ONU reboot thread
 *  \param[in] thr_params IFXOS_ThreadParams_t structure
 */
static int32_t pon_img_reboot_thread(struct IFXOS_ThreadParams_s *thr_params)
{
	struct pon_img_context *ctx;
	int err;

#ifdef LINUX
	dbg_prn("Reboot Thread (tid %d)\n", (int)getpid());
#endif
	IFXOS_MSecSleep((unsigned int)thr_params->nArg1);
	ctx = (struct pon_img_context *)thr_params->nArg2;

	err = ctx->pa_config->ubus_call(ctx->hl_handle,
					ctx->ubus_path, UBUS_METHOD_REBOOT,
					NULL, NULL, NULL, PON_UBUS_TIMEOUT);
	if (err) {
		dbg_err_fn_ret(ubus_call, err);
		return PON_ADAPTER_ERROR;
	}

	return 0;
}

static enum pon_adapter_errno pon_img_reboot(void *llhandle,
					     unsigned long timeout_ms)
{
	IFXOS_ThreadCtrl_t *p_thread = &pon_img_reboot_thread_control;

	if (IFXOS_THREAD_INIT_VALID(p_thread))
		(void)IFXOS_ThreadDelete(p_thread, 0);

	if (IFXOS_ThreadInit((IFXOS_ThreadCtrl_t *)p_thread,
			     "reboot",
			     pon_img_reboot_thread,
			     IFXOS_DEFAULT_STACK_SIZE,
			     IFXOS_THREAD_PRIO_LOWEST,
			     (IFX_ulong_t)timeout_ms, (IFX_ulong_t)llhandle)) {
		dbg_err("Can't reboot ONU\n");

		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static const struct pa_system_ops system_ops = {
	.init = pon_img_init,
	.start = pon_img_start,
	.reboot = pon_img_reboot,
	.shutdown = NULL,
};

static const struct pa_omci_me_ops omci_me_ops = {
	.sw_image = &sw_image_ops,
};

static const struct pa_ops pon_img_pa_ops = {
	.system_ops = &system_ops,
	.omci_me_ops = &omci_me_ops,
	.dbg_lvl_ops = &libponimg_dbg_lvl_ops
};

enum pon_adapter_errno
libponimg_ll_register_ops(void *hl_handle_legacy,
			  const struct pa_ops **pa_ops,
			  void **ll_handle,
			  void *hl_handle,
			  uint32_t if_version)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;

	dbg_in_args("%p, %p, %p, %p, %d",
		    hl_handle_legacy, pa_ops, ll_handle, hl_handle, if_version);

	/* In legacy mode, set the new arguments to compatible values. */
	if (hl_handle_legacy) {
		hl_handle = hl_handle_legacy;
		if_version = PA_IF_1ST_VER_NUMBER;
	}

	if (PA_IF_VERSION_CHECK_COMPATIBLE(if_version)) {
		g_pon_img_context.hl_handle = hl_handle;
		*pa_ops = &pon_img_pa_ops;
		*ll_handle = &g_pon_img_context;
		ret = PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/** @} */
