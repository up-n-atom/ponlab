/*****************************************************************************
 *
 * Copyright (c) 2025 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdlib.h>

#include <pon_adapter.h>
#include <pon_adapter_system.h>
#include <pon_adapter_config.h>
#include <omci/pon_adapter_omci.h>
#include <omci/me/pon_adapter_dot1x_port_ext_pkg.h>

#define DEBUG_MODULE libponpatemplate
#include <pon_adapter_debug_common.h>

#include "pon_pa_template.h"

#define FN_ERR_RET(ERR_FN, FN, ERR_OUT) \
	do { \
		dbg_err_fn_ret(FN, ERR_FN); \
		dbg_out_ret("%d", ERR_OUT); \
	} while (0)

static struct pon_pa_template_context g_pa_template_context;

/* Debug level for the PON Adapter template library */
uint8_t libponpatemplate_dbg_lvl = DBG_ERR;

static void set_dbglvl(const uint8_t level)
{
	libponpatemplate_dbg_lvl = level;
}

static uint8_t get_dbglvl(void)
{
	return libponpatemplate_dbg_lvl;
}

struct pa_ll_dbg_lvl_ops dbg_lvl_ops = {
	.set = set_dbglvl,
	.get = get_dbglvl
};

/* Implement the necessary functions for dot1x_port_ext_pkg_ops
 *
 * For a module with support for multiple Managed Entities,
 * you would typically move this to a separate file
 * and implement the operations for each Managed Entity.
 * Here we keep it simple and inline for demonstration purposes.
 * This is a placeholder for the actual implementation.
 *
 * You would need to implement the logic to handle the
 * Dot1x Port Extension Package Managed Entity.
 * This could involve interacting with the underlying system,
 * such as configuring network interfaces or updating
 * security settings based on the provided update data.
 */

static enum pon_adapter_errno pa_dot1x_port_ext_pkg_update(
	void *ll_handle, uint16_t me_id,
	const struct pa_dot1x_port_ext_pkg *update_data)
{
	struct pon_pa_template_context *ctx = ll_handle;
	const char *method;
	char *path;
	int error;

	dbg_in_args("%p, %d, %p", ll_handle, me_id, update_data);

	/* Here you would implement the logic to update the Dot1x Port Extension
	 * Package Managed Entity with the provided update data.
	 *
	 * Here an example with calling a UBUS interface.
	 */

	if (update_data->dot1x_enable)
		method = "enable";
	else
		method = "disable";

	/* Assume the lower part of me_id is the port number */
	error = asprintf(&path, "dot1x.port.%d", me_id & 0xFF);
	if (error < 0) {
		FN_ERR_RET(error, asprintf, error);
		return PON_ADAPTER_ERROR;
	}

	dbg_wrn("Calling ubus '%s' with '%s'\n", path, method);

	error = ctx->pa_config->ubus_call(ctx->hl_handle, path, method, NULL,
					  NULL, NULL, PON_UBUS_TIMEOUT);
	free(path);
	if (error) {
		FN_ERR_RET(error, ubus_call, error);
		//return PON_ADAPTER_ERROR;
		/* For demonstration, we assume success */
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const struct pa_dot1x_port_ext_pkg_ops dot1x_port_ext_pkg_ops = {
	.update = pa_dot1x_port_ext_pkg_update,
};

static const struct pa_omci_me_ops omci_me_ops = {
	.dot1x_port_ext_pkg = &dot1x_port_ext_pkg_ops,
};

static enum pon_adapter_errno pon_pa_template_init(
	char const * const *init_data, const struct pa_config *pa_config,
	const struct pa_eh_ops *event_handler, void *ll_handle)
{
	struct pon_pa_template_context *ctx = ll_handle;

	dbg_in_args("%p, %p, %p, %p", init_data, pa_config, event_handler,
		    ll_handle);

	ctx->pa_config = pa_config;

	/* Check if the ubus_call callback is provided for later.
	 * From this function the ubus_call can not be invoked yet.
	 */
	if (!ctx->pa_config->ubus_call) {
		dbg_err("No ubus_call callback provided, pon-pa-template-lib needs it\n");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_pa_template_start(void *ll_handle)
{
	dbg_in_args("%p", ll_handle);

	/* Here code can be included to be executed
	 * during the start process.
	 * For example, calling UBUS paths, which may involve
	 * setting up device drivers or configuring registers.
	 * All that is not possible in the init function
	 * because the UBUS system is not yet fully operational.
	 */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const struct pa_system_ops system_ops = {
	.init = pon_pa_template_init,
	.start = pon_pa_template_start,
	/* no shutdown, as we do not have anything to cleanup/free */
	.shutdown = NULL,
};

static const struct pa_ops pa_template_pa_ops = {
	.system_ops = &system_ops,
	.omci_me_ops = &omci_me_ops,
	.dbg_lvl_ops = &dbg_lvl_ops
};

enum pon_adapter_errno libpatemplate_ll_register_ops(
	void *hl_handle_legacy, const struct pa_ops **pa_ops, void **ll_handle,
	void *hl_handle, uint32_t if_version)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;

	dbg_in_args("%p, %p, %p, %p, %d", hl_handle_legacy, pa_ops, ll_handle,
		    hl_handle, if_version);

	/* Legacy mode not supported by this module. */
	if (hl_handle_legacy)
		return PON_ADAPTER_ERROR;

	if (PA_IF_VERSION_CHECK_COMPATIBLE(if_version)) {
		g_pa_template_context.hl_handle = hl_handle;
		*pa_ops = &pa_template_pa_ops;
		*ll_handle = &g_pa_template_context;
		ret = PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", ret);
	return ret;
}
