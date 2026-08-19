/*****************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_net_init.h"
#include "pon_net_debug.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"
#include "pon_net_ethtool.h"

#define CPU_QDISC_TBF_RATE 1000000
#define CPU_QDISC_TBF_PEAK_RATE 1250000
#define CPU_QDISC_TBF_BRUST_SIZE 10240
#define CPU_QDISC_TBF_PEAK_BURST_SIZE 20480
#define CPU_QDISC_TBF_LIMIT 100

#define CPU_QDISC_RED_PROBABILITY 1.0
#define CPU_QDISC_RED_LIMIT 117760
#define CPU_QDISC_RED_PARENT_QUEUE 1
#define CPU_QDISC_YELLOW_THRESHOLD 16384
#define CPU_QDISC_GREEN_THRESHOLD 8192

/* Table of CPU filters initial parameters */
static const struct {
	/* Queue number */
	uint16_t queue_number;
	/* Traffic class */
	int tc;
	/*
	 * Number in handle and prio lists:
	 * "pon_queue_handles", "pon_queue_prios"
	 */
	uint16_t list_num;
} pon_filters_params[] = {
	{1, 8, 8},
	{2, 9, 9},
	{3, 10, 10},
	{4, 11, 11},
	{5, 12, 12},
	{6, 13, 13},
	{7, 14, 14},
	{8, 15, 15},
	{8, 0, 0},
	{8, 1, 1},
	{8, 2, 2},
	{8, 3, 3},
	{8, 4, 4},
	{8, 5, 5},
	{8, 6, 6},
	{8, 7, 7}
};

/* Table of CPU tbf qdisc initial parameters */
static const struct {
	/* Qdisc handle major number */
	uint16_t id;
	/* Parent Queue. Unused if parent_id is 0 */
	uint16_t parent_queue;
} pon_qdisc_tbf_params[] = {
	{PON_TBF_QDISC_HANDLE(0), 2},
	{PON_TBF_QDISC_HANDLE(1), 3},
	{PON_TBF_QDISC_HANDLE(2), 4},
	{PON_TBF_QDISC_HANDLE(3), 5},
	{PON_TBF_QDISC_HANDLE(4), 6},
	{PON_TBF_QDISC_HANDLE(5), 7},
	{PON_TBF_QDISC_HANDLE(6), 8}
};

#define CPU_GREEN_QDISC_PARAMS(id_num) PON_WRED_QDISC_0(id_num), \
				       PON_TBF_QDISC_HANDLE(id_num)

/* Table of CPU red-green qdisc initial parameters */
static const struct {
	/* Qdisc handle major number */
	uint16_t id;
	/* Parent Qdisc. 0 if no parent */
	uint16_t parent_id;
	/* Parent Queue. Unused if parent_id is 0 */
} pon_qdisc_green_params[] = {
	{CPU_GREEN_QDISC_PARAMS(0)},
	{CPU_GREEN_QDISC_PARAMS(1)},
	{CPU_GREEN_QDISC_PARAMS(2)},
	{CPU_GREEN_QDISC_PARAMS(3)},
	{CPU_GREEN_QDISC_PARAMS(4)},
	{CPU_GREEN_QDISC_PARAMS(5)},
	{CPU_GREEN_QDISC_PARAMS(6)}
};

#define CPU_YELLOW_QDISC_PARAMS(id_num) PON_WRED_QDISC_1(id_num), \
					PON_WRED_QDISC_0(id_num)

/* Table of CPU red-yellow qdisc initial parameters */
static const struct {
	/* Qdisc handle major number */
	uint16_t id;
	/* Parent Qdisc. 0 if no parent */
	uint16_t parent_id;
	/* Parent Queue. Unused if parent_id is 0 */
} pon_qdisc_yellow_params[] = {
	{CPU_YELLOW_QDISC_PARAMS(0)},
	{CPU_YELLOW_QDISC_PARAMS(1)},
	{CPU_YELLOW_QDISC_PARAMS(2)},
	{CPU_YELLOW_QDISC_PARAMS(3)},
	{CPU_YELLOW_QDISC_PARAMS(4)},
	{CPU_YELLOW_QDISC_PARAMS(5)},
	{CPU_YELLOW_QDISC_PARAMS(6)}
};

static enum pon_adapter_errno
trap_handles_alloc(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ret = pon_net_dev_db_gen(ctx->db, "gem-omci",
				 PON_NET_HANDLE_FILTER_INGRESS,
				 &ctx->omci_trap_handle, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	ret = pon_net_dev_db_gen(ctx->db, "gem-omci",
				 PON_NET_PRIO_OMCI_TRAP_INGRESS,
				 &ctx->omci_trap_prio, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		ret2 = pon_net_dev_db_put(ctx->db, "gem-omci",
					  PON_NET_HANDLE_FILTER_INGRESS,
					  &ctx->omci_trap_handle, 1);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void trap_handles_free(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ret = pon_net_dev_db_put(ctx->db, "gem-omci",
				 PON_NET_HANDLE_FILTER_INGRESS,
				 &ctx->omci_trap_handle, 1);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, "gem-omci",
				 PON_NET_PRIO_OMCI_TRAP_INGRESS,
				 &ctx->omci_trap_prio, 1);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	dbg_out();
}

/* Allocate CPU handles and prios from pool */
enum pon_adapter_errno
pon_net_pon_handles_alloc(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", ctx);

	/* Generate unique handles */
	ret = pon_net_dev_db_gen(ctx->db, PON_MASTER_DEVICE,
				 PON_NET_HANDLE_FILTER_INGRESS,
				 ctx->pon_queue_handles,
				 ARRAY_SIZE(ctx->pon_queue_handles));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	/* Generate unique prios */
	ret = pon_net_dev_db_gen(ctx->db, PON_MASTER_DEVICE,
				 PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
				 ctx->pon_queue_prios,
				 ARRAY_SIZE(ctx->pon_queue_prios));
	if (ret != PON_ADAPTER_SUCCESS) {
		enum pon_adapter_errno ret2;

		ret2 = pon_net_dev_db_put(ctx->db, PON_MASTER_DEVICE,
					  PON_NET_HANDLE_FILTER_INGRESS,
					  ctx->pon_queue_handles,
					  ARRAY_SIZE(ctx->pon_queue_handles));
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_net_dev_db_put, ret2);
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	ctx->pon_net_queue_handles_valid = true;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return ret;
}

/* Return handles and prios to the pool */
void
pon_net_pon_handles_free(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	if (!ctx->pon_net_queue_handles_valid)
		goto out;

	ret = pon_net_dev_db_put(ctx->db, PON_MASTER_DEVICE,
				 PON_NET_HANDLE_FILTER_INGRESS,
				 ctx->pon_queue_handles,
				 ARRAY_SIZE(ctx->pon_queue_handles));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, PON_MASTER_DEVICE,
				 PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
				 ctx->pon_queue_prios,
				 ARRAY_SIZE(ctx->pon_queue_prios));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ctx->pon_net_queue_handles_valid = false;
out:
	dbg_out();
}

/* Create CPU filters */
static enum pon_adapter_errno
pon_net_tc_cpu_filters_add(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	size_t  i;
	uint32_t list_num;

	dbg_in_args("%p", ctx);

	for (i = 0; i < ARRAY_SIZE(pon_filters_params); i++) {
		list_num = pon_filters_params[i].list_num;
		ret =
		netlink_queue_assign(ctx->netlink, PON_MASTER_DEVICE,
				     PON_ROOT_QDISC,
				     pon_filters_params[i].queue_number, "",
				     ctx->pon_queue_handles[list_num],
				     NETLINK_FILTER_UNUSED,
				     pon_filters_params[i].tc,
				     (uint16_t)ctx->pon_queue_prios[list_num]);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_queue_assign, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return ret;
}

/* Create qdiscs and filters. */
enum pon_adapter_errno
pon_net_tc_cpu_queues_add(struct pon_net_context *ctx)
{
	struct netlink_qdisc_tbf params_tbf;
	struct netlink_qdisc_red params_green;
	struct netlink_qdisc_red params_yellow;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	size_t i;

	dbg_in_args("%p", ctx);

	netlink_qdisc_tbf_defaults(&params_tbf);
	netlink_qdisc_red_defaults(&params_green);
	netlink_qdisc_red_defaults(&params_yellow);

	/* Create root prio qdisc */
	ret = netlink_qdisc_prio_create(ctx->netlink, PON_MASTER_DEVICE,
					PON_ROOT_QDISC, 0, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_prio_create, ret);
		return ret;
	}

	netlink_qdisc_tbf_params_set(&params_tbf,
				     CPU_QDISC_TBF_RATE,
				     CPU_QDISC_TBF_PEAK_RATE,
				     CPU_QDISC_TBF_BRUST_SIZE,
				     CPU_QDISC_TBF_PEAK_BURST_SIZE,
				     CPU_QDISC_TBF_LIMIT);

	/* Create tbf qdisc */
	for (i = 0; i < ARRAY_SIZE(pon_qdisc_tbf_params); i++) {
		ret =
		netlink_qdisc_tbf_create(ctx->netlink,
					 PON_MASTER_DEVICE,
					 pon_qdisc_tbf_params[i].id,
					 PON_ROOT_QDISC,
					 pon_qdisc_tbf_params[i].parent_queue,
					 &params_tbf);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_qdisc_tbf_create, ret);
			return ret;
		}
	}

	netlink_qdisc_red_p_and_thr_set(&params_green,
				CPU_QDISC_GREEN_THRESHOLD,
				CPU_QDISC_GREEN_THRESHOLD,
				CPU_QDISC_RED_PROBABILITY,
				CPU_QDISC_RED_LIMIT,
				0, 0, 0);

	netlink_qdisc_red_p_and_thr_set(&params_yellow,
				CPU_QDISC_YELLOW_THRESHOLD,
				CPU_QDISC_YELLOW_THRESHOLD,
				CPU_QDISC_RED_PROBABILITY,
				CPU_QDISC_RED_LIMIT,
				0, 0, 0);

	for (i = 0; i < ARRAY_SIZE(pon_qdisc_green_params); i++) {
		ret =
		netlink_qdisc_red_create(ctx->netlink,
					 PON_MASTER_DEVICE,
					 pon_qdisc_green_params[i].id,
					 pon_qdisc_green_params[i].parent_id,
					 CPU_QDISC_RED_PARENT_QUEUE,
					 &params_green);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_qdisc_red_create, ret);
			return ret;
		}

		netlink_qdisc_red_create(ctx->netlink,
					 PON_MASTER_DEVICE,
					 pon_qdisc_yellow_params[i].id,
					 pon_qdisc_yellow_params[i].parent_id,
					 CPU_QDISC_RED_PARENT_QUEUE,
					 &params_yellow);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_qdisc_red_create, ret);
			return ret;
		}
	}

	/* Create filters */
	ret = pon_net_tc_cpu_filters_add(ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tc_filters_config, ret);
		return ret;
	}
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return ret;
}

/* Cleanup CPU qdiscs */
enum pon_adapter_errno
pon_net_tc_queues_exit(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", ctx);

	ret = netlink_qdisc_destroy(ctx->netlink, PON_MASTER_DEVICE,
				    PON_ROOT_QDISC, 0, 0);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(netlink_qdisc_destroy, ret);

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_master_dev_init(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ret = pon_net_dev_db_add(ctx->db, PON_MASTER_DEVICE, &pon_net_pon_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, PON_MASTER_DEVICE);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_clsact_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

}

enum pon_adapter_errno
pon_net_master_dev_exit(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ret = netlink_qdisc_clsact_destroy(ctx->netlink, PON_MASTER_DEVICE);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(netlink_qdisc_clsact_destroy, ret);

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_iphost_init(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	/* if 'ip0' was not created do not configure it */
	if (!netlink_netdevice_exists(ctx->netlink, IPH_MASTER_DEVICE)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_dev_db_add(ctx->db, IPH_MASTER_DEVICE, &pon_net_pon_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, IPH_MASTER_DEVICE);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_clsact_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void
pon_net_iphost_exit(struct pon_net_context *ctx)
{
	dbg_in_args("%p", ctx);

	/* if 'ip0' was not created do not deconfigure it */
	if (!netlink_netdevice_exists(ctx->netlink, IPH_MASTER_DEVICE)) {
		dbg_out();
		return;
	}

	netlink_qdisc_clsact_destroy(ctx->netlink, IPH_MASTER_DEVICE);
	pon_net_dev_db_del(ctx->db, IPH_MASTER_DEVICE);

	dbg_out();
}

#define TRAP_FILTER_HANDLE 1
/* Create OMCI channel,
 * i.e. tcont-omci and gem-omci for OMCI communication
 */
enum pon_adapter_errno
pon_net_omci_channel_init(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ret = pon_net_dev_db_add(ctx->db, "tcont-omci", &pon_net_tcont_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = pon_net_dev_db_add(ctx->db, "gem-omci", &pon_net_gem_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_add, ret);
		goto err_db_add_gem_omci;
	}

	ret = netlink_pon_net_omci_tcont_init(ctx->netlink);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_pon_net_omci_tcont_init, ret);
		goto err_omci_tcont_init;
	}

	ret = netlink_pon_net_omci_gem_init(ctx->netlink,
					    ctx->cfg.omcc_if_mac);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_pon_net_omci_gem_init, ret);
		goto err_omci_gem_init;
	}

	ret = netlink_netdevice_state_set(ctx->netlink, "gem-omci", true);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto err_omci_state_set;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, "gem-omci");
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto err_qdisc_clsact_create;
	}

	ret = trap_handles_alloc(ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(trap_handles_alloc, ret);
		goto err_trap_handles_alloc;
	}

	/*
	 * OMCI packets must go the high priority CPU ingress queue,
	 * therefore we need to setup tc-flower with trap action
	 */
	ret = netlink_omci_trap_set(ctx->netlink, "gem-omci",
				    NETLINK_FILTER_DIR_INGRESS,
				    ctx->omci_trap_handle,
				    (uint16_t)ctx->omci_trap_prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_omci_trap_set, ret);
		goto err_omci_trap_set;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

	/* error handling */
err_omci_trap_set:
	trap_handles_free(ctx);

err_trap_handles_alloc:
	netlink_qdisc_clsact_destroy(ctx->netlink, "gem-omci");

err_qdisc_clsact_create:
err_omci_state_set:
	netlink_pon_net_omci_gem_destroy(ctx->netlink);

err_omci_gem_init:
	netlink_pon_net_omci_tcont_destroy(ctx->netlink);

err_omci_tcont_init:
	pon_net_dev_db_del(ctx->db, "gem-omci");

err_db_add_gem_omci:
	pon_net_dev_db_del(ctx->db, "tcont-omci");

	dbg_out_ret("%d", ret);
	return ret;
}

/* Cleanup the previously create OMCI channel */
void
pon_net_omci_channel_exit(struct pon_net_context *ctx)
{
	dbg_in_args("%p", ctx);

	if (netlink_netdevice_exists(ctx->netlink, "gem-omci")) {
		trap_handles_free(ctx);
		netlink_qdisc_clsact_destroy(ctx->netlink, "gem-omci");
		netlink_pon_net_omci_gem_destroy(ctx->netlink);
		netlink_pon_net_omci_tcont_destroy(ctx->netlink);
		pon_net_dev_db_del(ctx->db, "gem-omci");
		pon_net_dev_db_del(ctx->db, "tcont-omci");
	}

	dbg_out();
}

static bool ext_switch_exists(struct pon_net_context *ctx)
{
	if (strnlen_s(ctx->cfg.uni_ext_switch_name, IF_NAMESIZE) > 0) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return true;
	}

	return false;
}

enum pon_adapter_errno
pon_net_ext_switch_init(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	if (!ext_switch_exists(ctx)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_dev_db_add(ctx->db, ctx->cfg.uni_ext_switch_name,
				 &pon_net_uni_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	/* Remove CPU port from external switch map */
	ret = pon_net_ethtool_priv_flag_set(ctx, ctx->cfg.uni_ext_switch_name,
					    "bp_to_cpu_enable", false);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ethtool_priv_flag_set, ret);
		goto cleanup_dev_db;
	}

	ret = netlink_netdevice_state_set(ctx->netlink,
					  ctx->cfg.uni_ext_switch_name,
					  true);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto cleanup_bp_cpu;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink,
					  ctx->cfg.uni_ext_switch_name);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto cleanup_bp_cpu;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

cleanup_bp_cpu:
	pon_net_ethtool_priv_flag_set(ctx, ctx->cfg.uni_ext_switch_name,
				      "bp_to_cpu_enable", true);
cleanup_dev_db:
	pon_net_dev_db_del(ctx->db, ctx->cfg.uni_ext_switch_name);

	dbg_out_ret("%d", ret);
	return ret;
}

void pon_net_ext_switch_exit(struct pon_net_context *ctx)
{
	dbg_in_args("%p", ctx);

	if (!ext_switch_exists(ctx)) {
		dbg_out();
		return;
	}

	if (!netlink_netdevice_exists(ctx->netlink,
				      ctx->cfg.uni_ext_switch_name)) {
		dbg_out();
		return;
	}

	netlink_qdisc_clsact_destroy(ctx->netlink,
				     ctx->cfg.uni_ext_switch_name);
	pon_net_ethtool_priv_flag_set(ctx, ctx->cfg.uni_ext_switch_name,
				      "bp_to_cpu_enable", true);
	pon_net_dev_db_del(ctx->db, ctx->cfg.uni_ext_switch_name);

	dbg_out();
}
