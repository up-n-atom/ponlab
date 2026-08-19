/*****************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * We need IF_OPER_UP flag, which comes from linux/if.h, but
 * pon_net_common.h includes net/if.h which defines struct ifreq
 * and other ioctl related structs that are also defined in linux/if.h
 * and this causes conflicts.
 *
 * The net/if.h only redefines those structs if _BSD_SOURCE or _GNU_SOURCE
 * is defined. Unfortunately in musl libc net/if.h also includes features.h,
 * which will define _BSD_SOURCE by default if no other _*_SOURCE macro is
 * defined. This is why we need _POSIX_C_SOURCE.
 */
#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#ifdef LINUX
#include <netinet/in.h>
#include <net/if.h>
#include <linux/if.h>
#include <arpa/inet.h>
#endif

#include <linux/if_ether.h>
#include <linux/tc_act/tc_vlan.h>
#include <math.h>
#include <pthread.h>

#include <netlink/route/link.h>
#include <netlink/route/link/gem.h>
#include <netlink/route/link/pmapper.h>
#include <netlink/route/link/tcont.h>
#include <netlink/route/link/pon.h>
#include <netlink/route/link/gem.h>
#include <netlink/route/link/bridge_info.h>
#include <netlink/route/link/bridge.h>
#include <netlink/route/qdisc.h>
#include <netlink/route/qdisc/prio.h>
#include <netlink/route/qdisc/red.h>
#include <netlink/route/qdisc/tbf.h>
#include <netlink/route/qdisc/drr.h>
#include <netlink/route/action.h>
#include <netlink/route/act/colmark.h>
#include <netlink/route/act/police.h>
#include <netlink/route/act/gact.h>
#include <netlink/route/act/vlan.h>
#include <netlink/route/act/mirred.h>
#include <netlink/route/act/skbedit.h>
#include <netlink/route/class.h>
#include <netlink/route/classifier.h>
#include <netlink/route/cls/flower.h>
#include <string.h>
#include <limits.h>

#include "pon_net_config.h"
#include "pon_net_netlink.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink_debug.h"

#ifdef MCC_DRV_ENABLE
#include "pon_mcc.h"
#endif

#define TCONT_OMCI_DEVICE "tcont-omci"

#ifndef TC_H_CLSACT
#define TC_H_CLSACT	TC_H_INGRESS
#define TC_H_MIN_INGRESS	0xFFF2U
#define TC_H_MIN_EGRESS		0xFFF3U
#endif

#ifndef TC_H_MIN_PRIORITY
#define TC_H_MIN_PRIORITY	0xFFF0U
#endif

#ifndef TCA_CLS_FLAGS_SKIP_HW
#define TCA_CLS_FLAGS_SKIP_HW	(1 << 0)
#endif

#ifndef TCA_CLS_FLAGS_SKIP_SW
#define TCA_CLS_FLAGS_SKIP_SW	(1 << 1)
#endif

/* Calculate unique filter priority */
#define IPHOST_PRIO(me_id) \
	((me_id) + 1)
#define IPHOST_VLAN_PRIO(vlan_id) \
	((vlan_id) + 1)

/* Default number of traffic classes (tc) */
#define QDISC_PRIO_DEFAULT_NUM_TC 8

/** PON Network Library Netlink context
 *
 *  Used by all pon_net_netlink functions to assure data integrity
 */
struct pon_net_netlink {
	/** Netlink lock for the entire structure */
	pthread_mutex_t nl_lock;
	/** libnl-route socket for netlink communication */
	struct nl_sock *nl_sock;
	/** Cache which holds information about available network devices */
	struct nl_cache *rtnl_link_cache;
};

/* callers need to take lock */
static int link_change(struct nl_sock *sock, struct rtnl_link *link,
		       struct rtnl_link *update_data,
		       struct nl_cache *cache,
		       int flags)
{
	int err;

	dbg_in_args("%p, %p, %p, %p, %d", sock, link,
		    update_data, cache, flags);

	err = rtnl_link_change(sock, link, update_data, flags);
	if (err)
		goto err;

	err = nl_cache_refill(sock, cache);

err:
	dbg_out_ret("%d", err);
	return err;
}

/* Convenience function to change link just by giving its ifindex */
/* callers need to take lock */
static int link_change_by_ifindex(struct nl_sock *sock,
				  struct nl_cache *cache,
				  int ifindex,
				  struct rtnl_link *changes,
				  int flags)
{
	struct rtnl_link *link;
	int err;

	dbg_in_args("%p, %d, %p, %d", sock, ifindex, changes, flags);

	link = rtnl_link_alloc();
	if (!link) {
		dbg_out_ret("%d", -NLE_NOMEM);
		return -NLE_NOMEM;
	}

	rtnl_link_set_ifindex(link, ifindex);

	rtnl_link_set_family(link, rtnl_link_get_family(changes));

	err = link_change(sock, link, changes, cache, flags);
	rtnl_link_put(link);
	dbg_out_ret("%d", err);
	return err;
}

/* Convenience function to change link just by giving its name */
/* callers need to take lock */
static int link_change_by_name(struct nl_cache *cache,
			       struct nl_sock *sock,
			       const char *name,
			       struct rtnl_link *changes,
			       int flags)
{
	int err;
	int ifindex;

	dbg_in_args("%p, %p, \"%s\", %p, %d", cache, sock, name, changes,
		    flags);

	ifindex = rtnl_link_name2i(cache, name);

	if (ifindex <= 0) {
		dbg_out_ret("%d", -NLE_OBJ_NOTFOUND);
		return NLE_OBJ_NOTFOUND;
	}

	err = link_change_by_ifindex(sock, cache, ifindex, changes, flags);
	dbg_out_ret("%d", err);
	return err;
}

/* callers need to take lock */
static int
link_add(struct pon_net_netlink *nl_ctx, struct rtnl_link *link, int flags)
{
	int err;

	dbg_in_args("%p, %p, %d", nl_ctx, link, flags);

	err = rtnl_link_add(nl_ctx->nl_sock, link, flags);
	if (err)
		goto err;

	err = nl_cache_refill(nl_ctx->nl_sock, nl_ctx->rtnl_link_cache);

err:
	dbg_out_ret("%d", err);
	return err;
}

/* callers need to take lock */
static int
link_delete(struct pon_net_netlink *nl_ctx, struct rtnl_link *link)
{
	int err;
	const char *type;

	dbg_in_args("%p, %p", nl_ctx, link);

	err = rtnl_link_delete(nl_ctx->nl_sock, link);
	if (err)
		goto err;

	/*
	 * If the deleted link is a bridge, we refresh the cache to update the
	 * "master" on potentially child links. Otherwise we remove only the
	 * deleted link from the cache, without touching remaining links.
	 */
	type = rtnl_link_get_type(link);
	if (type && strcmp(type, "bridge") == 0)
		nl_cache_refill(nl_ctx->nl_sock, nl_ctx->rtnl_link_cache);
	else
		nl_cache_remove((struct nl_object *)link);

err:
	dbg_out_ret("%d", err);
	return err;
}

/* callers need to take lock */
static int
link_enslave_ifindex(struct pon_net_netlink *nl_ctx, int master, int slave)
{
	int err;

	dbg_in_args("%p, %d, %d", nl_ctx, master, slave);

	err = rtnl_link_enslave_ifindex(nl_ctx->nl_sock, master, slave);
	if (err)
		goto err;

	err = nl_cache_refill(nl_ctx->nl_sock, nl_ctx->rtnl_link_cache);

err:
	dbg_out_ret("%d", err);
	return err;
}

/* callers need to take lock */
static int link_bridge_add(struct pon_net_netlink *nl_ctx, const char *ifname,
			   const struct netlink_bridge *params)
{
	int err;
	struct rtnl_link *link = NULL;

	dbg_in_args("%p, \"%s\", %p", nl_ctx, ifname, params);

	link = rtnl_link_bridge_alloc();
	if (!link) {
		err = -NLE_NOMEM;
		goto err;
	}

	rtnl_link_set_name(link, ifname);

	if (params && params->mask & NETLINK_BRIDGE_vlan_filtering) {
		err = rtnl_link_bridge_set_vlan_filtering(
		    link, params->vlan_filtering);
		if (err)
			goto err;
	}

	if (params && params->mask & NETLINK_BRIDGE_vlan_protocol) {
		err = rtnl_link_bridge_set_vlan_protocol(link,
			htons(params->vlan_protocol));
		if (err)
			goto err;
	}

	if (params && params->mask & NETLINK_BRIDGE_vlan_stats_enabled) {
		err = rtnl_link_bridge_set_vlan_stats_enabled(
		    link, params->vlan_stats_enabled);
		if (err)
			goto err;
	}

	err = link_add(nl_ctx, link, NLM_F_CREATE);
	if (err)
		goto err;

err:
	if (link)
		rtnl_link_put(link);

	dbg_out_ret("%d", err);
	return err;
}

/* does locking */
enum pon_adapter_errno
netlink_qdisc_prio_create(struct pon_net_netlink *nl_ctx,
			  const char *ifname,
			  uint16_t id,
			  uint16_t parent_id,
			  uint16_t parent_queue)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
#ifndef NO_TC_OFFLOAD
	struct rtnl_qdisc *qdisc = NULL;
	struct rtnl_link *link = NULL;
	int err;
	/* TODO: Change it to 8 elements when bug will be fixed in libnl */
	uint8_t map[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

	dbg_in_args("%p, \"%s\", %u, %u, %u", nl_ctx, ifname, id, parent_id,
		    parent_queue);

#ifndef NO_TC_OFFLOAD
	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err;
	}

	qdisc = rtnl_qdisc_alloc();
	if (!qdisc) {
		dbg_err_fn(rtnl_qdisc_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_link(TC_CAST(qdisc), link);
	if (parent_id)
		rtnl_tc_set_parent(TC_CAST(qdisc),
				   TC_HANDLE((unsigned int)parent_id,
					     (unsigned int)parent_queue));
	else
		rtnl_tc_set_parent(TC_CAST(qdisc), TC_H_ROOT);

	rtnl_tc_set_handle(TC_CAST(qdisc), TC_HANDLE((unsigned int)id, 0));

	err = rtnl_tc_set_kind(TC_CAST(qdisc), "prio");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		goto err;
	}

	rtnl_qdisc_prio_set_bands(qdisc, QDISC_PRIO_DEFAULT_NUM_TC);

	err = rtnl_qdisc_prio_set_priomap(qdisc, map, ARRAY_SIZE(map));
	if (err) {
		dbg_err_fn_ret(rtnl_qdisc_prio_set_priomap, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	netlink_debug_tc_qdisc_add(nl_ctx->rtnl_link_cache, qdisc);
	err = rtnl_qdisc_add(nl_ctx->nl_sock, qdisc, NLM_F_CREATE);
	if (err == -NLE_OPNOTSUPP) {
		dbg_wrn("rtnl_qdisc_add - operation not supported\n");
	} else if (err) {
		ret = PON_ADAPTER_ERROR;
		FN_ERR_NL(err, rtnl_qdisc_add, ifname);
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	if (qdisc)
		rtnl_qdisc_put(qdisc);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
#endif /* NO_TC_OFFLOAD */
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno
netlink_queue_assign(struct pon_net_netlink *nl_ctx, const char *ifname,
		     uint32_t id, uint16_t queue_number, const char *indev,
		     uint32_t assignment_id, int vlan_prio, int tc,
		     uint16_t prio)
{
	struct netlink_filter filter = {0};
	enum pon_adapter_errno ret;
	struct netlink_cookie c = {0};

	dbg_in_args("%p, \"%s\", %u, %u, \"%s\", %u, %d, %d, %d", nl_ctx,
		    ifname, id, queue_number, indev, assignment_id, vlan_prio,
		    tc, prio);

	netlink_filter_defaults(&filter);

	if (strncpy_s(filter.device, sizeof(filter.device),
		      ifname, IF_NAMESIZE)) {
		dbg_err_fn(strncpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	if (strncpy_s(filter.indev, sizeof(filter.indev),
		      indev, IF_NAMESIZE)) {
		dbg_err_fn(strncpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	if (vlan_prio != NETLINK_FILTER_UNUSED) {
		filter.vlan_prio = vlan_prio;
		filter.proto = ETH_P_8021AD;
	}

	if (tc != NETLINK_FILTER_UNUSED) {
		c.cookie[ARRAY_SIZE(c.cookie) - 1] = (unsigned char)tc;
		filter.act_data = &c;
	}

	filter.act = NETLINK_FILTER_ACT_OK;
	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.classid_maj = (int)id < 0 ? NETLINK_FILTER_UNUSED : (int)id;
	filter.classid_min = queue_number;
	filter.handle = assignment_id;
	filter.prio = prio;

	dbg_msg("Adding: tc filter add dev %s ingress flower skip_sw indev %s classid %"
		PRIx16 ":%" PRIx16 " handle 0x%x action ok",
		ifname,
		indev,
		id,
		queue_number,
		assignment_id);

	ret = netlink_filter_add(nl_ctx, &filter);

	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
static enum pon_adapter_errno qdisc_destroy(struct pon_net_netlink *nl_ctx,
					    const char *ifname,
					    uint16_t id,
					    uint32_t parent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_qdisc *qdisc = NULL;
	int ifindex;
	int err;

	dbg_in_args("%p, %p", nl_ctx, ifname);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, ifname);
	if (!ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, ifname);
		ret = PON_ADAPTER_ERROR;
		goto out;
	}

	qdisc = rtnl_qdisc_alloc();
	if (!qdisc) {
		dbg_err_fn(rtnl_qdisc_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto out;
	}
	rtnl_tc_set_ifindex(TC_CAST(qdisc), ifindex);
	rtnl_tc_set_parent(TC_CAST(qdisc), parent);
	rtnl_tc_set_handle(TC_CAST(qdisc), TC_HANDLE((unsigned int)id, 0));

	netlink_debug_tc_qdisc_del(nl_ctx->rtnl_link_cache, qdisc);
	err = rtnl_qdisc_delete(nl_ctx->nl_sock, qdisc);
	if (err && err != -NLE_INVAL && err != -NLE_OBJ_NOTFOUND) {
		ret = PON_ADAPTER_ERROR;
		FN_ERR_NL(err, rtnl_qdisc_delete, ifname);
		goto out;
	}
out:
	if (qdisc)
		rtnl_qdisc_put(qdisc);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_qdisc_destroy(struct pon_net_netlink *nl_ctx,
					     const char *ifname, uint16_t id,
					     uint16_t parent_id,
					     uint16_t parent_queue)
{
	uint32_t parent;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, id);

	if (parent_id)
		parent = TC_HANDLE((unsigned int)parent_id,
				   (unsigned int)parent_queue);
	else
		parent = TC_H_ROOT;

	ret = qdisc_destroy(nl_ctx, ifname, id, parent);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(qdisc_destroy, ret);

	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
void netlink_qdisc_red_defaults(struct netlink_qdisc_red *red)
{
	const struct netlink_qdisc_red tmp = {
		.limit = NETLINK_QDISC_RED_UNUSED,
		.min = NETLINK_QDISC_RED_UNUSED,
		.max = NETLINK_QDISC_RED_UNUSED,
		.flags = NETLINK_QDISC_RED_UNUSED,
		.wlog = NETLINK_QDISC_RED_UNUSED,
		.plog = NETLINK_QDISC_RED_UNUSED,
		.scell_log = NETLINK_QDISC_RED_UNUSED,
		.max_p = NETLINK_QDISC_RED_UNUSED,
	};

	dbg_in_args("%p", red);

	*red = tmp;

	dbg_out();
}

/* no locking needed */
void netlink_qdisc_red_p_and_thr_set(struct netlink_qdisc_red *red,
				     int min,
				     int max,
				     double probability,
				     int limit,
				     int scell_log,
				     int wlog,
				     int flags)
{
	dbg_in_args("%p, %d, %d, %f", red, min, max, probability);

	red->min = min;
	red->max = (int)max;
	red->plog = (int)log2((double)(max - min) / probability);
	red->max_p = probability;
	red->limit = limit;
	red->scell_log = scell_log;
	red->wlog = wlog;
	red->flags = flags;

	dbg_out();
}

/* no locking needed */
static uint32_t scale_probability(double p)
{
	if (p >= 1.0)
		return UINT_MAX;

	return (uint32_t)(p * pow(2, 32));
}

/* does locking */
enum pon_adapter_errno
netlink_qdisc_red_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t parent_id,
			 uint16_t parent_queue,
			 const struct netlink_qdisc_red *params)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link = NULL;
	struct rtnl_qdisc *qdisc = NULL;
	unsigned char stab[256] = {0};
	int err;

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, id);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err;
	}

	qdisc = rtnl_qdisc_alloc();
	if (!qdisc) {
		dbg_err_fn(rtnl_qdisc_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_link(TC_CAST(qdisc), link);
	if (parent_id)
		rtnl_tc_set_parent(TC_CAST(qdisc),
				   TC_HANDLE((unsigned int)parent_id,
					     (unsigned int)parent_queue));
	else
		rtnl_tc_set_parent(TC_CAST(qdisc), TC_H_ROOT);
	rtnl_tc_set_handle(TC_CAST(qdisc), TC_HANDLE((unsigned int)id, 0));

	err = rtnl_tc_set_kind(TC_CAST(qdisc), "red");
	if (err) {
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		FN_ERR_NL(err, rtnl_tc_set_kind, ifname);
		goto err;
	}

	if (params->limit != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_limit(qdisc, params->limit);
	if (params->min != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_min(qdisc, params->min);
	if (params->max != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_max(qdisc, params->max);
	if (params->flags != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_flags(qdisc, params->flags);
	if (params->wlog != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_wlog(qdisc, params->wlog);
	if (params->plog != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_plog(qdisc, params->plog);
	if (params->scell_log != NETLINK_QDISC_RED_UNUSED)
		rtnl_qdisc_red_set_scell_log(qdisc, params->scell_log);
	if (params->max_p >= 0)
		rtnl_qdisc_red_set_max_p(qdisc,
					 scale_probability(params->max_p));

	rtnl_qdisc_red_set_stab(qdisc, stab, sizeof(stab));

	netlink_debug_tc_qdisc_add(nl_ctx->rtnl_link_cache, qdisc);
	err = rtnl_qdisc_add(nl_ctx->nl_sock, qdisc, NLM_F_CREATE);
	if (err == -NLE_OPNOTSUPP) {
		dbg_wrn("rtnl_qdisc_add - operation not supported\n");
	} else if (err) {
		ret = PON_ADAPTER_ERROR;
		FN_ERR_NL(err, rtnl_qdisc_add, ifname);
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	if (qdisc)
		rtnl_qdisc_put(qdisc);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
void netlink_qdisc_tbf_defaults(struct netlink_qdisc_tbf *tbf)
{
	const struct netlink_qdisc_tbf tmp = {
		.cir = NETLINK_QDISC_TBF_UNUSED,
		.pir = NETLINK_QDISC_TBF_UNUSED,
		.cbs = NETLINK_QDISC_TBF_UNUSED,
		.pbs = NETLINK_QDISC_TBF_UNUSED,
	};

	dbg_in_args("%p", tbf);

	*tbf = tmp;

	dbg_out();
}

/*
 * 80kbps is the QoS bandwidth granularity defined in PPv4
 *
 * Libnl accepts CIR bytes, so to set cir to 1kbps we need to set it to
 *
 *     1024 / 8 = 128Bps
 *
 * Since the granularity is 80kbps, then the minimal CIR becomes:
 *
 *     128Bps * 80
 *
 * This will result in setting bw = 80, which you can check here:
 *
 *     echo queue=QUEUE_NUMBER  > /sys/kernel/debug/pp/qos0/queue
 */
#define DEFAULT_COMMITTED_RATE (128 * 80)
#define DEFAULT_COMMITTED_BURST (256 * 1024)
#define DEFAULT_PEAK_BURST (512 * 1024)
#define DEFAULT_TBF_LIMIT 1024

/* no locking needed */
void netlink_qdisc_tbf_params_set(struct netlink_qdisc_tbf *tbf,
				  int cir, int pir,
				  int cbs, int pbs,
				  int lim)
{
	dbg_in_args("%p, %d, %d, %d, %d", tbf, cir, pir, cbs, pbs);

	tbf->cir = cir;
	tbf->pir = pir;

	if (tbf->cir < DEFAULT_COMMITTED_RATE && tbf->pir > 0) {
		tbf->cir = DEFAULT_COMMITTED_RATE;
		if (pir > DEFAULT_COMMITTED_RATE)
			tbf->pir = pir - DEFAULT_COMMITTED_RATE;
	}

	if (tbf->cir == 0)
		tbf->cir = NETLINK_QDISC_TBF_UNUSED;

	if (tbf->pir == 0)
		tbf->pir = NETLINK_QDISC_TBF_UNUSED;

	if (tbf->pir <= tbf->cir)
		tbf->pir = NETLINK_QDISC_TBF_UNUSED;

	if (cbs == 0)
		tbf->cbs = DEFAULT_COMMITTED_BURST;
	else
		tbf->cbs = cbs;

	if (pbs == 0)
		tbf->pbs = DEFAULT_PEAK_BURST;
	else
		tbf->pbs = pbs;

	if (lim == 0)
		tbf->lim = DEFAULT_TBF_LIMIT;
	else
		tbf->lim = lim;
	dbg_out();
}

/* does locking */
enum pon_adapter_errno
netlink_qdisc_tbf_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t parent_id,
			 uint16_t parent_queue,
			 const struct netlink_qdisc_tbf *params)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link = NULL;
	struct rtnl_qdisc *qdisc = NULL;
	int bucket = 0;
	int cell = 0;
	int err;

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, id);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err;
	}

	qdisc = rtnl_qdisc_alloc();
	if (!qdisc) {
		dbg_err_fn(rtnl_qdisc_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_link(TC_CAST(qdisc), link);
	if (parent_id)
		rtnl_tc_set_parent(TC_CAST(qdisc),
				   TC_HANDLE((unsigned int)parent_id,
					     (unsigned int)parent_queue));
	else
		rtnl_tc_set_parent(TC_CAST(qdisc), TC_H_ROOT);
	rtnl_tc_set_handle(TC_CAST(qdisc), TC_HANDLE((unsigned int)id, 0));

	err = rtnl_tc_set_kind(TC_CAST(qdisc), "tbf");
	if (err) {
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		FN_ERR_NL(err, rtnl_tc_set_kind, ifname);
		goto err;
	}

	/* TODO: get bucket and cell values (check relation to burst) */
	if (params->cir != NETLINK_QDISC_TBF_UNUSED)
		rtnl_qdisc_tbf_set_rate(qdisc, params->cir, bucket, cell);
	if (params->pir != NETLINK_QDISC_TBF_UNUSED)
		rtnl_qdisc_tbf_set_peakrate(qdisc, params->pir, bucket, cell);
	if (params->cbs != NETLINK_QDISC_TBF_UNUSED)
		rtnl_qdisc_tbf_set_burst(qdisc, params->cbs);
	if (params->pbs != NETLINK_QDISC_TBF_UNUSED)
		rtnl_qdisc_tbf_set_peakburst(qdisc, params->pbs);

	rtnl_qdisc_tbf_set_limit(qdisc, params->lim);

	netlink_debug_tc_qdisc_add(nl_ctx->rtnl_link_cache, qdisc);
	err = rtnl_qdisc_add(nl_ctx->nl_sock, qdisc, NLM_F_CREATE);
	if (err == -NLE_OPNOTSUPP) {
		dbg_wrn("rtnl_qdisc_add - operation not supported\n");
	} else if (err) {
		ret = PON_ADAPTER_ERROR;
		FN_ERR_NL(err, rtnl_tc_set_kind, ifname);
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	if (qdisc)
		rtnl_qdisc_put(qdisc);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_netdevice_destroy(struct pon_net_netlink *nl_ctx,
						 const char *ifname)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	struct rtnl_link *link;

	dbg_in_args("%p, %s", nl_ctx, ifname);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	/* Nothing to delete, so we act like we deleted the device.
	 * The end result is the same
	 */
	if (!link)
		goto err;

	netlink_debug_ip_link_del(ifname);
	err = link_delete(nl_ctx, link);
	/* If nothing to delete we act like we deleted the device.
	 * The end result is the same
	 */
	if (err && err != -NLE_NODEV) {
		FN_ERR_NL_RET(err, link_delete, ifname, PON_ADAPTER_ERROR);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
static enum pon_adapter_errno netlink_flag_set(struct pon_net_netlink *nl_ctx,
					       const char *ifname,
					       unsigned int flag,
					       int up)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link;
	int err;

	dbg_in_args("%p, \"%s\", %u, %d", nl_ctx, ifname, flag, up);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (up)
		rtnl_link_set_flags(link, flag);
	else
		rtnl_link_unset_flags(link, flag);

	netlink_debug_ip_link_set(ifname, up ? "up" : "down");
	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  ifname, link, 0);
	if (err) {
		FN_ERR_NL_RET(err, link_change_by_name, ifname,
			      PON_ADAPTER_ERROR);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
enum pon_adapter_errno
netlink_netdevice_state_set(struct pon_net_netlink *nl_ctx,
			    const char *ifname,
			    int up)
{
	return netlink_flag_set(nl_ctx, ifname, IFF_UP, up);
}

/* no locking needed */
enum pon_adapter_errno
netlink_multicast_state_set(struct pon_net_netlink *nl_ctx,
			    const char *ifname,
			    int enabled)
{
	return netlink_flag_set(nl_ctx, ifname, IFF_MULTICAST, enabled);
}

/* callers need to take lock */
static enum pon_adapter_errno netdevice_mtu_set(struct pon_net_netlink *nl_ctx,
						const char *ifname,
						unsigned int max_frames_size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link;
	unsigned int mtu;
	int err;

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, max_frames_size);

	/*
	 * Ignore the request for invalid range (upper limit depends on drivers)
	 * for Ethernet the minimal packet size is 64 (+ 4 for the FCS)
	 */
	if (max_frames_size < 68) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	link = rtnl_link_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	/*
	 * The Linux MTU value is only considering the payload of an Ethernet
	 * packet. The 18 bytes extra are:
	 * - Ethernet header (DA + SA + Ethertype) = 6 + 6 + 2 = 14 bytes
	 * - Ethernet FCS: 4 bytes
	 *
	 * OMCI defines the "Max Frame Size" for the complete Ethernet packet,
	 * including header and FCS.
	 */
	mtu = max_frames_size - 18;
	rtnl_link_set_mtu(link, mtu);

	netlink_debug_ip_link_set_mtu(ifname, mtu);
	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  ifname, link, 0);
	if (err) {
		FN_ERR_NL(err, link_change_by_name, ifname);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (link)
		rtnl_link_put(link);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_netdevice_mtu_set(struct pon_net_netlink *nl_ctx,
						 const char *ifname,
						 unsigned int max_frames_size)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, max_frames_size);

	pthread_mutex_lock(&nl_ctx->nl_lock);
	ret = netdevice_mtu_set(nl_ctx, ifname, max_frames_size);
	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_netdevice_mac_addr_set(struct pon_net_netlink *nl_ctx,
			       const char *ifname,
			       const uint8_t if_mac[ETH_ALEN])
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link;
	struct nl_addr *mac_addr;
	int err;

	dbg_in_args("%p, \"%s\", %p", nl_ctx, ifname, if_mac);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	/* Set MAC address for OMCC interface */
	mac_addr = nl_addr_build(AF_LLC, if_mac, ETH_ALEN);
	if (!mac_addr) {
		dbg_err_fn_ret(nl_addr_build, 0);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
	rtnl_link_set_addr(link, mac_addr);
	nl_addr_put(mac_addr);

	netlink_debug_ip_link_set_macaddr(ifname, if_mac);
	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  ifname, link, 0);
	if (err) {
		FN_ERR_NL(err, link_change_by_name, ifname);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_netdevice_operstate_get(struct pon_net_netlink *nl_ctx,
				const char *ifname,
				bool *up)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link = NULL;
	uint8_t state;
	int err;

	pthread_mutex_lock(&nl_ctx->nl_lock);

	dbg_in_args("%p, %s, %p", nl_ctx, ifname, up);

	err = rtnl_link_get_kernel(nl_ctx->nl_sock, 0, ifname, &link);
	if (err || !link) {
		dbg_err("rtnl_link_get_kernel() failed on [%s] with %d (%s)\n",
			ifname, err, nl_geterror(err));
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	state = rtnl_link_get_operstate(link);

	*up = (state == IF_OPER_UP);

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_netdevice_enslave(struct pon_net_netlink *nl_ctx,
			  const char *master,
			  const char *slave)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	int master_ifindex = 0;
	int slave_ifindex = 0;

	dbg_in_args("%p, \"%s\", \"%s\"", nl_ctx, master, slave);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	if (strnlen_s(master, IF_NAMESIZE) > 0) {
		master_ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache,
						  master);
		if (!master_ifindex) {
			FN_ERR_NL(0, rtnl_link_name2i, master);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	slave_ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, slave);
	if (!slave_ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, slave);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	netlink_debug_ip_link_set_master(master, slave);
	err = link_enslave_ifindex(nl_ctx, master_ifindex, slave_ifindex);
	if (err) {
		dbg_err("link_enslave_ifindex() failed on [%s:%s] with %d (%s)\n",
			master, slave, err, nl_geterror(err));
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
set_link_master_device(struct pon_net_netlink *nl_ctx, struct rtnl_link *link,
		       const char *master)
{
	int ifindex;

	dbg_in_args("%p, %p, %s", nl_ctx, link, master);

	ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, master);
	if (!ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, master);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	rtnl_link_set_link(link, ifindex);
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
set_link_pon_master_device(struct pon_net_netlink *nl_ctx,
			   struct rtnl_link *link)
{
	return set_link_master_device(nl_ctx, link, PON_MASTER_DEVICE);
}

/* does locking */
enum pon_adapter_errno
netlink_set_queue_lookup_mode(struct pon_net_netlink *nl_ctx,
			      enum netlink_queue_lookup mode)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	struct rtnl_link *link;

	dbg_in_args("%p, %u", nl_ctx, mode);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_pon_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_pon_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	err = rtnl_link_pon_set_queue_lookup_mode(link, (uint8_t)mode);
	if (err) {
		dbg_err_fn_ret(rtnl_link_pon_set_queue_lookup_mode, err);
		ret = PON_ADAPTER_ERR_INVALID_VAL;
		goto err;
	}

	dbg_prn("ip link set %s type pon queue_lookup_mode %u\n",
		PON_MASTER_DEVICE, mode);
	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  PON_MASTER_DEVICE, link, 0);
	if (err) {
		FN_ERR_NL(err, link_change_by_name, PON_MASTER_DEVICE);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_is_slave(struct pon_net_netlink *nl_ctx,
					const char *ifname,
					int *is_slave)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link;

	dbg_in_args("%p, \"%s\", %p", nl_ctx, ifname, is_slave);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	*is_slave = !!rtnl_link_get_master(link);

	rtnl_link_put(link);

err:
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_pmapper_create(struct pon_net_netlink *nl_ctx,
					      uint16_t pmapper_index)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	struct rtnl_link *link;
	char pmapper_name[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u", nl_ctx, pmapper_index);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_pmapper_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_pmapper_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ret = set_link_pon_master_device(nl_ctx, link);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(set_link_pon_master_device, ret);
		goto err;
	}

	snprintf(pmapper_name, sizeof(pmapper_name), PON_IFNAME_PMAPPER,
		 pmapper_index);

	rtnl_link_set_name(link, pmapper_name);
	rtnl_link_pmapper_set_default_pcp(link, 0);

	netlink_debug_ip_link_add_pmapper(PON_MASTER_DEVICE, pmapper_name, 0);
	err = link_add(nl_ctx, link, NLM_F_CREATE);
	if (err) {
		FN_ERR_NL(err, link_add, pmapper_name);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_pmapper_setup(struct pon_net_netlink *nl_ctx,
					     uint16_t pmapper_index,
					     uint8_t default_pcp,
					     uint16_t *gem_ctp_me_ids,
					     uint8_t *dscp_to_pbit)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	struct rtnl_link *link;
	char pmapper_name[IF_NAMESIZE] = "";
	uint8_t i = 0;
	int ifindex = 0;

	dbg_in_args("%p, %u, %u, %p, %p", nl_ctx, pmapper_index, default_pcp,
		    gem_ctp_me_ids, dscp_to_pbit);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	snprintf(pmapper_name, sizeof(pmapper_name), PON_IFNAME_PMAPPER,
		 pmapper_index);

	link = rtnl_link_pmapper_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_pmapper_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_link_pmapper_set_default_pcp(link, default_pcp);
	if (err) {
		FN_ERR_NL(err, rtnl_link_pmapper_set_default_pcp,
			      pmapper_name);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	for (i = 0; i < PON_8021P_PCP_COUNT; i++) {
		char ifname[IF_NAMESIZE];

		/* 0xFFFF is a NULL pointer, as specified in OMCI */
		if (gem_ctp_me_ids[i] != 0xFFFF) {
			snprintf(ifname, sizeof(ifname), PON_IFNAME_GEM,
				 gem_ctp_me_ids[i]);

			ifindex =
			    rtnl_link_name2i(nl_ctx->rtnl_link_cache,
					     ifname);
			if (!ifindex) {
				FN_ERR_NL(0, rtnl_link_name2i, ifname);
				ret = PON_ADAPTER_ERROR;
				goto err;
			}
		} else {
			ifindex = 0;
		}

		err = rtnl_link_pmapper_set_pcp_ifindex(link, i, ifindex);
		if (err) {
			FN_ERR_NL(err, rtnl_link_pmapper_set_pcp_ifindex,
				  ifname);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	if (dscp_to_pbit) {
		for (i = 0; i < DSCP_MAX; ++i) {
			err = rtnl_link_pmapper_set_dscp_to_pcp(
			    link, i, dscp_to_pbit[i]);
			if (err) {
				FN_ERR_NL(err,
					  rtnl_link_pmapper_set_dscp_to_pcp,
					  pmapper_name);
				ret = PON_ADAPTER_ERROR;
				goto err;
			}
		}

		/*
		 * The RTNL_PMAPPER_MODE_DSCP is not supported at the moment,
		 * but is needed in future, to make this feature fully working.
		 *
		 * TODO: Change this to RTNL_PMAPPER_MODE_DSCP when DSCP + PBit
		 * becomes supported
		 */
		err = rtnl_link_pmapper_set_mode(link,
						 RTNL_PMAPPER_MODE_DSCP_ONLY);
		if (err) {
			FN_ERR_NL(err, rtnl_link_pmapper_set_mode,
				  pmapper_name);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	} else {
		err = rtnl_link_pmapper_set_mode(link,
						 RTNL_PMAPPER_MODE_PCP);
		if (err) {
			FN_ERR_NL(err, rtnl_link_pmapper_set_mode,
				  pmapper_name);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	netlink_debug_ip_link_set_pmapper(pmapper_name, gem_ctp_me_ids,
					  PON_8021P_PCP_COUNT, dscp_to_pbit);
	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  pmapper_name,
				  link,
				  0);
	if (err) {
		FN_ERR_NL(err, link_change_by_name, pmapper_name);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_pmapper_state_set(struct pon_net_netlink *nl_ctx,
						 uint16_t pmapper_index, int up)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u, %d", nl_ctx, pmapper_index, up);

	snprintf(ifname, sizeof(ifname), PON_IFNAME_PMAPPER,
		 pmapper_index);
	ret = netlink_netdevice_state_set(nl_ctx, ifname, up);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);

	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_pmapper_destroy(struct pon_net_netlink *nl_ctx,
					       uint16_t pmapper_index)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char pmapper_name[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u", nl_ctx, pmapper_index);

	snprintf(pmapper_name, sizeof(pmapper_name), PON_IFNAME_PMAPPER,
		 pmapper_index);
	ret = netlink_netdevice_destroy(nl_ctx, pmapper_name);

	dbg_out_ret("%d", ret);
	return ret;
}

/* TODO: remove after DP Library fix is implemented */
/* does locking */
enum pon_adapter_errno
netlink_pmapper_has_gem_ports(struct pon_net_netlink *nl_ctx,
			      const char *pmapper_ifname, bool *result)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *pmapper;
	uint8_t pcp;

	dbg_in_args("%p, \"%s\", %p", nl_ctx, pmapper_ifname, result);

	*result = false;

	pthread_mutex_lock(&nl_ctx->nl_lock);

	pmapper = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache,
					pmapper_ifname);
	if (!pmapper) {
		dbg_err_fn(rtnl_link_get_by_name);
		dbg_err("No match was found for %s\n", pmapper_ifname);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (!rtnl_link_is_pmapper(pmapper)) {
		dbg_err_fn(rtnl_link_is_pmapper);
		dbg_err("Interface %s is not pmapper\n", pmapper_ifname);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	for (pcp = 0; pcp < PON_8021P_PCP_COUNT; pcp++) {
		if (rtnl_link_pmapper_get_pcp_ifindex(pmapper, pcp)
			!= -NLE_MISSING_ATTR) {
			*result = true;
			dbg_msg("Interface %s have gem\n", pmapper_ifname);
			goto err;
		}
	}

	dbg_msg("Interface %s doesn't have gem\n", pmapper_ifname);

err:
	if (pmapper)
		rtnl_link_put(pmapper);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_pon_net_omci_tcont_init(
	struct pon_net_netlink *nl_ctx)
{
	struct rtnl_link *tcont;
	int pon_ifindex;
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", nl_ctx);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	pon_ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache,
				       PON_MASTER_DEVICE);
	if (!pon_ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, PON_MASTER_DEVICE);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	tcont = rtnl_link_tcont_alloc();
	if (!tcont) {
		dbg_err_fn(rtnl_link_tcont_alloc);
		dbg_err("Allocation failed\n");
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	rtnl_link_set_name(tcont, TCONT_OMCI_DEVICE);

	err = rtnl_link_tcont_set_index(tcont, 0);
	if (err) {
		dbg_err_fn_ret(rtnl_link_tcont_set_index, err);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

	rtnl_link_set_link(tcont, pon_ifindex);

	err = link_add(nl_ctx, tcont, NLM_F_CREATE);
	if (err) {
		FN_ERR_NL(err, link_add, TCONT_OMCI_DEVICE);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

rtnl_err:
	rtnl_link_put(tcont);
err:
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
void netlink_pon_net_omci_tcont_destroy(struct pon_net_netlink *nl_ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", nl_ctx);

	ret = netlink_netdevice_destroy(nl_ctx, TCONT_OMCI_DEVICE);
	if (ret)
		dbg_err_fn_ret(netlink_netdevice_destroy, ret);

	dbg_out();
}

/* does locking */
enum pon_adapter_errno netlink_pon_net_omci_gem_init(
	struct pon_net_netlink *nl_ctx, const uint8_t if_mac[ETH_ALEN])
{
	struct rtnl_link *gem;
	int pon_ifindex, tcont_ifindex;
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct nl_addr *mac_addr;

	dbg_in_args("%p", nl_ctx);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	pon_ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache,
				       PON_MASTER_DEVICE);
	if (!pon_ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, PON_MASTER_DEVICE);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	tcont_ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache,
					 TCONT_OMCI_DEVICE);
	if (!tcont_ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, TCONT_OMCI_DEVICE);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	gem = rtnl_link_gem_alloc();
	if (!gem) {
		dbg_err_fn(rtnl_link_gem_alloc);
		dbg_err("Allocation failed\n");
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	rtnl_link_set_name(gem, GEM_OMCI_DEVICE);

	err = rtnl_link_gem_set_index(gem, 0);
	if (err) {
		dbg_err_fn_ret(rtnl_link_gem_set_index, err);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

	/* Set MAC address for OMCC interface */
	mac_addr = nl_addr_build(AF_LLC, if_mac, ETH_ALEN);
	if (!mac_addr) {
		dbg_err_fn_ret(nl_addr_build, 0);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}
	rtnl_link_set_addr(gem, mac_addr);
	nl_addr_put(mac_addr);

	/* Set traffic type to OMCI */
	err = rtnl_link_gem_set_traffic_type(gem, 1);
	if (err) {
		dbg_err_fn_ret(rtnl_link_gem_set_traffic_type, err);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

	/* Set direction to bidirectional */
	err = rtnl_link_gem_set_dir(gem, 3);
	if (err) {
		dbg_err_fn_ret(rtnl_link_gem_set_dir, err);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

	err = rtnl_link_gem_set_tcont(gem, (uint32_t)tcont_ifindex);
	if (err || tcont_ifindex < 0) {
		dbg_err_fn_ret(rtnl_link_gem_set_tcont, err);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

	rtnl_link_set_link(gem, pon_ifindex);

	err = link_add(nl_ctx, gem, NLM_F_CREATE);
	if (err) {
		FN_ERR_NL(err, link_add, GEM_OMCI_DEVICE);
		ret = PON_ADAPTER_ERROR;
		goto rtnl_err;
	}

	/* Update MTU - Max Frame Size - 14 - 4 (EHT Hdr + FCS) = MTU Size */
	err = netdevice_mtu_set(nl_ctx, GEM_OMCI_DEVICE, 2048);
	if (err) {
		ret = PON_ADAPTER_ERROR;
		dbg_err_fn_ret(netdevice_mtu_set, err);
		goto rtnl_err;
	}

rtnl_err:
	rtnl_link_put(gem);
err:
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
void netlink_pon_net_omci_gem_destroy(struct pon_net_netlink *nl_ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", nl_ctx);

	ret = netlink_netdevice_destroy(nl_ctx, GEM_OMCI_DEVICE);
	if (ret)
		dbg_err_fn_ret(netlink_netdevice_destroy, ret);

	dbg_out();
}

/* no locking needed */
void netlink_gem_defaults(struct netlink_gem *gem)
{
	gem->tcont_meid = NETLINK_GEM_UNUSED;
	gem->traffic_type = NETLINK_GEM_UNUSED;
	gem->dir = NETLINK_GEM_UNUSED;
	gem->enc = NETLINK_GEM_UNUSED;
	gem->max_size = NETLINK_GEM_UNUSED;
	gem->port_id = NETLINK_GEM_UNUSED;
	gem->mc = NETLINK_GEM_UNUSED;
}

/* callers need to take lock */
static enum pon_adapter_errno
set_gem_settings(struct pon_net_netlink *nl_ctx, struct rtnl_link *gem,
		 const struct netlink_gem *gem_data, int *changed)
{
	int do_update = 0;
	int err;
	int tcont_ifindex;
	char tcont_ifname[IF_NAMESIZE];

	dbg_in_args("%p, %p, %p, %p", nl_ctx, gem, gem_data, changed);

	if (gem_data->port_id != NETLINK_GEM_UNUSED) {
		err = rtnl_link_gem_set_id(gem, (uint16_t)gem_data->port_id);
		if (err || gem_data->port_id > 0xFFFF ||
		    gem_data->port_id < 0) {
			FN_ERR_RET(err, rtnl_link_gem_set_id,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (gem_data->tcont_meid != NETLINK_GEM_UNUSED) {
		if (gem_data->tcont_meid == 0xFFFF) {
			tcont_ifindex = 0;
		} else {
			snprintf(tcont_ifname, sizeof(tcont_ifname),
				 PON_IFNAME_TCONT, gem_data->tcont_meid);

			tcont_ifindex =
			    rtnl_link_name2i(nl_ctx->rtnl_link_cache,
					     tcont_ifname);
			if (!tcont_ifindex) {
				FN_ERR_NL(0, rtnl_link_name2i, tcont_ifname);
				dbg_out_ret("%d", PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
			}
		}
		err = rtnl_link_gem_set_tcont(gem, (uint32_t)tcont_ifindex);
		if (err || tcont_ifindex < 0) {
			FN_ERR_RET(err, rtnl_link_gem_set_tcont,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (gem_data->traffic_type != NETLINK_GEM_UNUSED) {
		err = rtnl_link_gem_set_traffic_type(
		    gem, (uint8_t)gem_data->traffic_type);
		if (err) {
			FN_ERR_RET(err, rtnl_link_gem_set_traffic_type,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (gem_data->dir != NETLINK_GEM_UNUSED) {
		err = rtnl_link_gem_set_dir(gem, (uint8_t)gem_data->dir);
		if (err) {
			FN_ERR_RET(err, rtnl_link_gem_set_dir,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (gem_data->enc != NETLINK_GEM_UNUSED) {
		err = rtnl_link_gem_set_enc(gem, (uint8_t)gem_data->enc);
		if (err) {
			FN_ERR_RET(err, rtnl_link_gem_set_enc,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (gem_data->max_size != NETLINK_GEM_UNUSED) {
		err = rtnl_link_gem_set_max_size(gem,
						 (uint16_t)gem_data->max_size);
		if (err) {
			FN_ERR_RET(err, rtnl_link_gem_set_max_size,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (gem_data->mc != NETLINK_GEM_UNUSED) {
		err = rtnl_link_gem_set_multicast(gem,
						 (uint8_t)gem_data->mc);
		if (err) {
			FN_ERR_RET(err, rtnl_link_gem_set_multicast,
				   PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		do_update = 1;
	}

	if (changed)
		*changed = do_update;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* does locking */
enum pon_adapter_errno
netlink_gem_create(struct pon_net_netlink *nl_ctx, uint32_t gem_meid,
		   const struct netlink_gem *gem_data)
{
	char gem_ifname[IF_NAMESIZE];
	struct rtnl_link *gem;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;

	dbg_in_args("%p, %u, %p", nl_ctx, gem_meid, gem_data);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	snprintf(gem_ifname, sizeof(gem_ifname), PON_IFNAME_GEM, gem_meid);

	gem = rtnl_link_gem_alloc();
	if (!gem) {
		dbg_err_fn(rtnl_link_gem_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ret = set_link_pon_master_device(nl_ctx, gem);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(set_link_pon_master_device, ret);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ret = set_gem_settings(nl_ctx, gem, gem_data, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(set_gem_settings, ret);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	rtnl_link_set_name(gem, gem_ifname);

	netlink_debug_ip_link_gem(gem_ifname, "add", gem_data);
	err = link_add(nl_ctx, gem, NLM_F_CREATE);
	if (err)
		ret = PON_ADAPTER_EAGAIN;

err:
	if (gem)
		rtnl_link_put(gem);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_gem_update(struct pon_net_netlink *nl_ctx,
					  uint16_t gem_meid,
					  const struct netlink_gem *gem_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *gem;
	char gem_ifname[IF_NAMESIZE];
	int err;
	int do_update = 0;

	dbg_in_args("%p, %u, %p", nl_ctx, gem_meid, gem_data);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	gem = rtnl_link_gem_alloc();
	if (!gem) {
		dbg_err_fn(rtnl_link_gem_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ret = set_gem_settings(nl_ctx, gem, gem_data, &do_update);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(set_gem_settings, ret);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (!do_update) {
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	snprintf(gem_ifname, sizeof(gem_ifname), PON_IFNAME_GEM, gem_meid);
	netlink_debug_ip_link_gem(gem_ifname, "set", gem_data);
	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  gem_ifname,
				  gem,
				  0);
	if (err) {
		FN_ERR_NL(err, link_change_by_name, gem_ifname);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (gem)
		rtnl_link_put(gem);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_gem_state_set(struct pon_net_netlink *nl_ctx,
					     uint16_t gem_meid, int up)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u, %d", nl_ctx, gem_meid, up);

	snprintf(ifname, sizeof(ifname), PON_IFNAME_GEM, gem_meid);
	ret = netlink_netdevice_state_set(nl_ctx, ifname, up);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_gem_destroy(struct pon_net_netlink *nl_ctx,
					   uint16_t gem_index)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char gem_name[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u", nl_ctx, gem_index);

	snprintf(gem_name, sizeof(gem_name), PON_IFNAME_GEM,
		 gem_index);
	ret = netlink_netdevice_destroy(nl_ctx, gem_name);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_tcont_create(struct pon_net_netlink *nl_ctx, uint16_t me_id,
		     uint16_t alloc_id)
{
	struct rtnl_link *tcont;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	char ifname[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u, %u", nl_ctx, me_id, alloc_id);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	snprintf(ifname, sizeof(ifname), PON_IFNAME_TCONT, me_id);

	tcont = rtnl_link_tcont_alloc();
	if (!tcont) {
		dbg_err_fn(rtnl_link_tcont_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	ret = set_link_pon_master_device(nl_ctx, tcont);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(set_link_pon_master_device, ret);

		goto err;
	}

	err = rtnl_link_tcont_set_id(tcont, alloc_id);
	if (err) {
		dbg_err_fn_ret(rtnl_link_tcont_set_id, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	rtnl_link_set_name(tcont, ifname);

	netlink_debug_ip_link_add_tcont(PON_MASTER_DEVICE, ifname, alloc_id);
	err = link_add(nl_ctx, tcont, NLM_F_CREATE);
	if (err)
		ret = PON_ADAPTER_EAGAIN;

err:
	if (tcont)
		rtnl_link_put(tcont);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno
netlink_tcont_destroy(struct pon_net_netlink *nl_ctx, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";

	dbg_in_args("%p, %u", nl_ctx, me_id);

	snprintf(ifname, sizeof(ifname), PON_IFNAME_TCONT, me_id);
	ret = netlink_netdevice_destroy(nl_ctx, ifname);

	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_bridge_create(struct pon_net_netlink *nl_ctx, const char *ifname,
		      const struct netlink_bridge *params)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;

	dbg_in_args("%p, \"%s\", %p", nl_ctx, ifname, params);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	netlink_debug_ip_link_bridge_add(ifname, params);
	err = link_bridge_add(nl_ctx, ifname, params);
	if (err) {
		dbg_err_fn_ret(link_bridge_add, err);
		ret = PON_ADAPTER_ERROR;
	}

	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_bridge_port_cfg(struct pon_net_netlink *nl_ctx,
					       int ifidx_bp, bool dest_lookup,
					       bool src_lookup, bool learning)
{
	struct rtnl_link *link = NULL;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;

	dbg_in_args("%p, %d, %i, %i",
		    nl_ctx, ifidx_bp, dest_lookup, src_lookup);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get(nl_ctx->rtnl_link_cache, ifidx_bp);
	if (!link) {
		dbg_err_fn(rtnl_link_get);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (rtnl_link_get_master(link) == 0) {
		ret = PON_ADAPTER_SUCCESS;
		goto err;
	}
	rtnl_link_put(link);

	link = rtnl_link_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	rtnl_link_set_family(link, AF_BRIDGE);
	if (dest_lookup)
		err = rtnl_link_bridge_set_flags(link,
						 RTNL_BRIDGE_DEST_LOOKUP);
	else
		err = rtnl_link_bridge_unset_flags(link,
						   RTNL_BRIDGE_DEST_LOOKUP);
	if (err) {
		dbg_err_fn_ret(rtnl_link_bridge_set_flags, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (src_lookup)
		err = rtnl_link_bridge_set_flags(link, RTNL_BRIDGE_SRC_LOOKUP);
	else
		err = rtnl_link_bridge_unset_flags(link,
						   RTNL_BRIDGE_SRC_LOOKUP);
	if (err) {
		dbg_err_fn_ret(rtnl_link_bridge_set_flags, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (learning)
		err = rtnl_link_bridge_set_flags(link, RTNL_BRIDGE_LEARNING);
	else
		err = rtnl_link_bridge_unset_flags(link, RTNL_BRIDGE_LEARNING);
	if (err) {
		dbg_err_fn_ret(rtnl_link_bridge_set_flags, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = link_change_by_ifindex(nl_ctx->nl_sock,
				     nl_ctx->rtnl_link_cache,
				     ifidx_bp, link, 0);
	if (err) {
		dbg_err_fn_ret(link_change_by_ifindex, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_bridge_mcast_flood(struct pon_net_netlink *nl_ctx,
			   int ifidx_bp, bool enable)
{
	struct rtnl_link *link = NULL;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;

	dbg_in_args("%p, %d, %i", nl_ctx, ifidx_bp, enable);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_bridge_alloc();
	if (!link) {
		dbg_err_fn(rtnl_link_bridge_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_link_bridge_set_mc_flood(link,
			enable ? RTNL_BRIDGE_MCAST_FLOOD_ALL :
				 RTNL_BRIDGE_MCAST_FLOOD_DISCARD);
	if (err) {
		dbg_err_fn_ret(rtnl_link_bridge_set_mc_flood, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = link_change_by_ifindex(nl_ctx->nl_sock,
				     nl_ctx->rtnl_link_cache,
				     ifidx_bp, link, 0);
	if (err) {
		dbg_err_fn_ret(link_change_by_ifindex, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
void netlink_filter_defaults(struct netlink_filter *filter)
{
	filter->classid_maj = NETLINK_FILTER_UNUSED;
	filter->classid_min = NETLINK_FILTER_UNUSED;
	filter->cvlan_id = NETLINK_FILTER_UNUSED;
	filter->cvlan_prio = NETLINK_FILTER_UNUSED;
	filter->cvlan_proto = NETLINK_FILTER_UNUSED;
	filter->vlan_id = NETLINK_FILTER_UNUSED;
	filter->vlan_prio = NETLINK_FILTER_UNUSED;
	filter->vlan_proto = NETLINK_FILTER_UNUSED;
	filter->dir = NETLINK_FILTER_DIR_INGRESS;
	filter->proto = ETH_P_ALL;
	filter->act = NETLINK_FILTER_ACT_OK;
	filter->act_data = NULL;
	memset(filter->indev, 0, sizeof(filter->indev));
	memset(filter->device, 0, sizeof(filter->device));
	filter->handle = (uint32_t)NETLINK_FILTER_UNUSED;
	filter->prio = 0;
	filter->ip_tos = NETLINK_FILTER_UNUSED;
	filter->flags = NETLINK_HW_ONLY;
	filter->eth_dst = false;
	memset(filter->eth_dst_addr, 0x00, sizeof(filter->eth_dst_addr));
	memset(filter->eth_dst_mask, 0x00, sizeof(filter->eth_dst_mask));
	filter->ip_proto = 0;
	filter->icmpv6_type = 0;
	filter->ipv4_dst_en = false;
	filter->ipv4_dst_addr = 0x0;
	filter->ipv4_dst_mask = 0x0;
	filter->ipv6_dst_en = false;
	memset(filter->ipv6_dst_addr, 0x00, sizeof(filter->ipv6_dst_addr));
	memset(filter->ipv6_dst_mask, 0x00, sizeof(filter->ipv6_dst_mask));
}

/* no locking needed */
void netlink_vlan_defaults(struct netlink_vlan_data *data)
{
	static const struct netlink_cookie zero = {0};

	data->cookie = zero;
	data->vlan_id = NETLINK_FILTER_UNUSED;
	data->vlan_prio = NETLINK_FILTER_UNUSED;
	data->vlan_proto = NETLINK_FILTER_UNUSED;
	data->svlan_id = NETLINK_FILTER_UNUSED;
	data->svlan_prio = NETLINK_FILTER_UNUSED;
	data->svlan_eth_type = NETLINK_FILTER_UNUSED;
	data->act_vlan = NETLINK_FILTER_ACT_VLAN_PUSH;
}

void netlink_skbedit_defaults(struct netlink_skbedit_data *data)
{
	data->action = TC_ACT_UNSPEC;
	data->prio = NETLINK_FILTER_UNUSED;
	data->vlan_act = NULL;
	memset(data->cookie, 0, sizeof(data->cookie));
}

/* Creates qdisc of type "clsact" for given link.
   "clsact" is the qdisc that supports adding classifiers to
   ingress and egress. */
/* callers need to take lock */
static enum pon_adapter_errno qdisc_add_clsact(struct pon_net_netlink *nl_ctx,
					       struct rtnl_link *link)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_qdisc *qdisc = NULL;
	int err;

	dbg_in_args("%p, %p", nl_ctx, link);

	qdisc = rtnl_qdisc_alloc();
	if (!qdisc) {
		dbg_err_fn(rtnl_qdisc_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_link(TC_CAST(qdisc), link);
	rtnl_tc_set_parent(TC_CAST(qdisc), TC_H_CLSACT);
	rtnl_tc_set_handle(TC_CAST(qdisc), TC_H_MAKE(TC_H_CLSACT, 0));

	err = rtnl_tc_set_kind(TC_CAST(qdisc), "clsact");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		goto err;
	}

	netlink_debug_tc_qdisc_add(nl_ctx->rtnl_link_cache, qdisc);
	err = rtnl_qdisc_add(nl_ctx->nl_sock, qdisc, NLM_F_CREATE);
	if (err && err != -NLE_EXIST) {
		dbg_err_fn_ret(rtnl_qdisc_add, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (qdisc)
		rtnl_qdisc_put(qdisc);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
static enum pon_adapter_errno
filter_clear_one(struct pon_net_netlink *nl_ctx, const char *ifname,
		 uint16_t protocol, uint16_t priority, uint32_t handle,
		 enum netlink_filter_dir dir)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_cls *cls = NULL;
	int ifindex;
	int err;

	dbg_in_args("%p, \"%s\", %u, %u, %u, %d", nl_ctx, ifname, protocol,
		    priority, handle, dir);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, ifname);
	if (!ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, ifname);
		ret = PON_ADAPTER_ERR_INVALID_VAL;
		goto err;
	}

	cls = rtnl_cls_alloc();
	if (!cls) {
		dbg_err_fn(rtnl_cls_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_cls_set_protocol(cls, protocol);
	rtnl_cls_set_prio(cls, priority);
	rtnl_tc_set_ifindex(TC_CAST(cls), ifindex);
	rtnl_tc_set_handle(TC_CAST(cls), handle);

	switch (dir) {
	case NETLINK_FILTER_DIR_INGRESS:
		rtnl_tc_set_parent(TC_CAST(cls),
				   TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_INGRESS));
		break;
	case NETLINK_FILTER_DIR_EGRESS:
		rtnl_tc_set_parent(TC_CAST(cls),
				   TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_EGRESS));
		break;
	default:
		ret = PON_ADAPTER_ERR_INVALID_VAL;
		goto err;
	}

	netlink_debug_tc_flower_filter_del(nl_ctx->rtnl_link_cache, cls);

	err = rtnl_cls_delete(nl_ctx->nl_sock, cls, 0);
	if (err && err != -NLE_INVAL && err != -NLE_OBJ_NOTFOUND)
		dbg_err_fn_ret(rtnl_cls_delete, err);
err:
	if (cls)
		rtnl_cls_put(cls);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_filter_clear_one(struct pon_net_netlink *nl_ctx,
						const char *ifname,
						uint16_t protocol,
						uint16_t priority,
						uint32_t handle,
						enum netlink_filter_dir dir)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %u, %u, %u, %d",
		    nl_ctx, ifname, protocol, priority, handle, dir);

	if (dir == NETLINK_FILTER_DIR_BOTH) {
		/*
		 * There are no bidirectional filters. NETLINK_FILTER_DIR_BOTH
		 * means that we will delete one filter from ingress and one
		 * from egress.
		 */
		ret = filter_clear_one(nl_ctx, ifname, protocol, priority,
				       handle, NETLINK_FILTER_DIR_INGRESS);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(filter_clear_one, ret);
			goto err;
		}
		ret = filter_clear_one(nl_ctx, ifname, protocol, priority,
				       handle, NETLINK_FILTER_DIR_EGRESS);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(filter_clear_one, ret);
			goto err;
		}
	} else {
		ret = filter_clear_one(nl_ctx, ifname, protocol, priority,
				       handle, dir);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(filter_clear_one, ret);
			goto err;
		}
	}

err:
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_qdisc_clsact_create(struct pon_net_netlink *nl_ctx,
			    const char *ifname)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link = NULL;

	dbg_in_args("%p, \"%s\"", nl_ctx, ifname);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err;
	}

	ret = qdisc_add_clsact(nl_ctx, link);
	if (ret) {
		dbg_err_fn_ret(qdisc_add_clsact, ret);
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno
netlink_qdisc_clsact_destroy(struct pon_net_netlink *nl_ctx,
			     const char *ifname)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p", nl_ctx, ifname);

	ret = qdisc_destroy(nl_ctx, ifname, TC_H_MAJ(TC_H_CLSACT) >> 16,
			    TC_H_CLSACT);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(qdisc_destroy, ret);

	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
qdisc_add_drr(struct pon_net_netlink *nl_ctx, struct rtnl_link *link,
	      uint16_t id, uint16_t parent_id, uint16_t parent_queue)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_qdisc *qdisc;
	int err;

	dbg_in_args("%p, %p, %u", nl_ctx, link, id);

	qdisc = rtnl_qdisc_alloc();
	if (!qdisc) {
		dbg_err_fn(rtnl_qdisc_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_link(TC_CAST(qdisc), link);
	if (parent_id)
		rtnl_tc_set_parent(TC_CAST(qdisc),
				   TC_HANDLE((unsigned int)parent_id,
					     (unsigned int)parent_queue));
	else
		rtnl_tc_set_parent(TC_CAST(qdisc), TC_H_ROOT);
	rtnl_tc_set_handle(TC_CAST(qdisc), TC_HANDLE((unsigned int)id, 0));

	err = rtnl_tc_set_kind(TC_CAST(qdisc), "drr");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		goto err;
	}

	netlink_debug_tc_qdisc_add(nl_ctx->rtnl_link_cache, qdisc);
	err = rtnl_qdisc_add(nl_ctx->nl_sock, qdisc, NLM_F_CREATE);
	if (err == -NLE_OPNOTSUPP) {
		dbg_wrn("rtnl_qdisc_add - operation not supported\n");
	} else if (err) {
		dbg_err_fn_ret(rtnl_qdisc_add, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (qdisc)
		rtnl_qdisc_put(qdisc);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
class_add_drr(struct pon_net_netlink *nl_ctx, struct rtnl_link *link,
	      uint16_t id,
	      uint16_t queue_number,
	      uint32_t quantum)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_class *class;
	int err;

	dbg_in_args("%p, %p, %u, %u, %u", nl_ctx, link, id, queue_number,
		    quantum);

	class = rtnl_class_alloc();
	if (!class) {
		dbg_err_fn(rtnl_class_alloc);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_link(TC_CAST(class), link);
	rtnl_tc_set_parent(TC_CAST(class), TC_HANDLE((unsigned int)id, 0));
	rtnl_tc_set_handle(TC_CAST(class),
			   TC_HANDLE((unsigned int)id,
				     (unsigned int)queue_number));

	err = rtnl_tc_set_kind(TC_CAST(class), "drr");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		goto err;
	}

	err = rtnl_drr_set_quantum(class, quantum);
	if (err) {
		dbg_err_fn_ret(rtnl_drr_set_quantum, err);
		ret = PON_ADAPTER_ERR_NOT_SUPPORTED;
		goto err;
	}

	netlink_debug_tc_class_add(nl_ctx->rtnl_link_cache, class);
	err = rtnl_class_add(nl_ctx->nl_sock, class, NLM_F_CREATE);
	if (err) {
		dbg_err_fn_ret(rtnl_class_add, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
err:
	if (class)
		rtnl_class_put(class);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_qdisc_drr_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t parent_id,
			 uint16_t parent_queue)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link;

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, id);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err;
	}

	ret = qdisc_add_drr(nl_ctx, link, id, parent_id, parent_queue);
	if (ret) {
		dbg_err_fn_ret(qdisc_add_drr, ret);
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_class_drr_create(struct pon_net_netlink *nl_ctx,
			 const char *ifname,
			 uint16_t id,
			 uint16_t queue_number,
			 uint32_t quantum)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *link;

	dbg_in_args("%p, \"%s\", %u, %u, %u", nl_ctx, ifname, id, queue_number,
		    quantum);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, ifname);
		ret = PON_ADAPTER_ERR_NOT_FOUND;
		goto err;
	}

	ret = class_add_drr(nl_ctx, link, id, queue_number, quantum);
	if (ret) {
		dbg_err_fn_ret(class_add_drr, ret);
		goto err;
	}

err:
	if (link)
		rtnl_link_put(link);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* Arguments passed to one_class_remove() as arg */
struct one_class_remove_args {
	/* PON Net Lib context */
	struct pon_net_netlink *nl_ctx;
	/* qdisc major number */
	uint16_t id;
	/* Error returned by rtnl_class_delete() after deleting class */
	int err;
};

/* Callback to nl_cache_foreach() used to delete classes for given qdisc */
/* callers need to take lock */
static void one_class_remove(struct nl_object *object, void *arg)
{
	struct rtnl_class *class = (struct rtnl_class *)(object);
	struct one_class_remove_args *args = arg;
	int err = 0;
	uint32_t handle;

	dbg_in_args("%p, %p", object, arg);

	handle = rtnl_tc_get_handle(TC_CAST(class));
	if (TC_H_MAJ(handle) == TC_HANDLE((unsigned int)args->id, 0)) {
		netlink_debug_tc_class_del(args->nl_ctx->rtnl_link_cache,
					   class);
		err = rtnl_class_delete(args->nl_ctx->nl_sock, class);
		if (err == -NLE_OBJ_NOTFOUND)
			/* Class does not exists - nothing to delete.
			 * This is necessary, because the kernel
			 * sometimes reports tc class twice. */
			err = 0;

		if (!args->err && err)
			args->err = err;
	}

	dbg_out();
}

/* does locking */
enum pon_adapter_errno
netlink_class_clear_all(struct pon_net_netlink *nl_ctx,
			const char *ifname,
			uint16_t id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct nl_cache *cache = NULL;
	int ifindex;
	int err;
	struct one_class_remove_args args = {
		.id = id,
		.nl_ctx = nl_ctx,
		.err = 0
	};

	dbg_in_args("%p, \"%s\", %u", nl_ctx, ifname, id);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, ifname);
	if (!ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, ifname);
		ret = PON_ADAPTER_ERROR;
		goto out;
	}

	err = rtnl_class_alloc_cache(nl_ctx->nl_sock, ifindex, &cache);
	if (err) {
		dbg_err_fn_ret(rtnl_class_alloc_cache, err);
		ret = PON_ADAPTER_ERROR;
		goto out;
	}

	nl_cache_foreach(cache, one_class_remove, &args);
	if (args.err) {
		dbg_err("At least one of the tc classes was not properly removed. Reason: %d\n",
			args.err);
		ret = PON_ADAPTER_ERROR;
		goto out;
	}
out:
	if (cache)
		nl_cache_free(cache);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_class_clear_one(struct pon_net_netlink *nl_ctx,
			const char *ifname,
			uint16_t parent,
			uint16_t traffic_class)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int ifindex;
	int err;
	struct rtnl_class *class = NULL;

	dbg_in_args("%p, \"%s\", %u, %u",
		    nl_ctx, ifname, parent, traffic_class);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, ifname);
	if (!ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, ifname);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	class = rtnl_class_alloc();
	if (!class) {
		dbg_err_fn_ret(rtnl_class_alloc, 0);
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		goto err;
	}

	rtnl_tc_set_ifindex(TC_CAST(class), ifindex);
	rtnl_tc_set_handle(TC_CAST(class),
			   TC_HANDLE((unsigned int)parent,
				     (unsigned int)traffic_class));

	netlink_debug_tc_class_del(nl_ctx->rtnl_link_cache, class);
	err = rtnl_class_delete(nl_ctx->nl_sock, class);
	if (err == -NLE_OBJ_NOTFOUND) {
		/* Class does not exists - nothing to delete. */
		err = 0;
	}
	if (err) {
		dbg_err_fn_ret(rtnl_class_delete, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (class)
		rtnl_class_put(class);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_colmark_add(struct rtnl_cls *cls,
		   const struct netlink_colmark_data *colmark)
{
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_act *act = NULL;

	dbg_in_args("%p, %p", cls, colmark);

	act = rtnl_act_alloc();
	if (!act) {
		dbg_err_fn(rtnl_act_alloc);
		goto err;
	}

	err = rtnl_tc_set_kind(TC_CAST(act), "colmark");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (colmark->mode != NETLINK_FILTER_UNUSED) {
		err = rtnl_colmark_set_mode(act, (uint32_t)colmark->mode);
		if (err || colmark->mode < 0) {
			dbg_err_fn_ret(rtnl_colmark_set_mode, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	if (colmark->drop_precedence != NETLINK_FILTER_UNUSED) {
		err = rtnl_colmark_set_drop_precedence(act,
			(uint32_t)colmark->drop_precedence);
		if (err || colmark->drop_precedence < 0) {
			dbg_err_fn_ret(rtnl_colmark_set_drop_precedence, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	if (colmark->meter_type != NETLINK_FILTER_UNUSED) {
		err = rtnl_colmark_set_meter_type(act,
						(uint32_t)colmark->meter_type);
		if (err || colmark->meter_type < 0) {
			dbg_err_fn_ret(rtnl_colmark_set_meter_type, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	err = rtnl_flower_add_action(cls, act);
	if (err) {
		dbg_err_fn_ret(rtnl_flower_add_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
err:
	if (act)
		rtnl_act_put(act);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_police_add(struct rtnl_cls *cls,
		 const struct netlink_police_data *police)
{
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_act *act = NULL;
	const struct netlink_colmark_data *colmark = &police->colmark;

	dbg_in_args("%p, %p", cls, police);

	act = rtnl_act_alloc();
	if (!act) {
		dbg_err_fn(rtnl_act_alloc);
		goto err;
	}

	err = rtnl_tc_set_kind(TC_CAST(act), "police");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_police_set_rate(act, police->cir);
	if (err) {
		dbg_err_fn_ret(rtnl_police_set_rate, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_police_set_peakrate(act, police->pir);
	if (err) {
		dbg_err_fn_ret(rtnl_police_set_peakrate, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_police_set_burst(act, police->cbs);
	if (err) {
		dbg_err_fn_ret(rtnl_police_set_peakrate, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_police_set_mtu(act, police->pbs);
	if (err) {
		dbg_err_fn_ret(rtnl_police_set_peakrate, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_flower_add_action(cls, act);
	if (err) {
		dbg_err_fn_ret(rtnl_flower_add_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_police_set_action(act, TC_POLICE_PIPE);
	if (err) {
		dbg_err_fn_ret(rtnl_police_set_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
err:
	if (act)
		rtnl_act_put(act);

	/* We always add a color marking action after the pipe in the
	 * police action.
	 */
	err = action_colmark_add(cls, colmark);
	if (err) {
		dbg_err_fn_ret(action_colmark_add, err);
		ret = PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_gact_add(struct rtnl_cls *cls,
		enum netlink_filter_act action,
		struct netlink_cookie *cookie)
{
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_act *act = NULL;

	dbg_in_args("%p, %d", cls, action);

	act = rtnl_act_alloc();
	if (!act) {
		dbg_err_fn(rtnl_act_alloc);
		goto err;
	}

	UNUSED(cls);

	err = rtnl_tc_set_kind(TC_CAST(act), "gact");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	switch (action) {
	case NETLINK_FILTER_ACT_OK:
		rtnl_gact_set_action(act, TC_ACT_OK);
		break;
	case NETLINK_FILTER_ACT_DROP:
		rtnl_gact_set_action(act, TC_ACT_SHOT);
		break;
	case NETLINK_FILTER_ACT_TRAP:
		rtnl_gact_set_action(act, TC_ACT_TRAP);
		break;
	default:
		dbg_err("Unsupported action %d\n", action);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (cookie) {
		err = rtnl_act_set_cookie(act, cookie->cookie,
					  sizeof(cookie->cookie));
		if (err) {
			dbg_err_fn_ret(rtnl_act_set_cookie, err);
			ret = PON_ADAPTER_ERR_INVALID_VAL;
			goto err;
		}
	}

	err = rtnl_flower_add_action(cls, act);
	if (err) {
		dbg_err_fn_ret(rtnl_flower_add_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
err:
	if (act)
		rtnl_act_put(act);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
static bool bytes_non_zero(const uint8_t *bytes, unsigned int len)
{
	unsigned int i = 0;

	dbg_in_args("%p, %u", bytes, len);

	for (i = 0; i < len; ++i) {
		if (bytes[i]) {
			dbg_out_ret("%d", true);
			return true;
		}
	}

	dbg_out_ret("%d", false);
	return false;
}

/* no locking needed */
static bool cookie_non_zero(const struct netlink_cookie *cookie)
{
	bool ret;

	dbg_in_args("%p", cookie);

	ret = bytes_non_zero(cookie->cookie, ARRAY_SIZE(cookie->cookie));

	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_vlan_add(struct rtnl_cls *cls, const struct netlink_vlan_data *vlan)
{
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_act *act = NULL;

	dbg_in_args("%p, %p", cls, vlan);

	act = rtnl_act_alloc();
	if (!act) {
		dbg_err_fn(rtnl_act_alloc);
		goto err;
	}

	UNUSED(cls);

	err = rtnl_tc_set_kind(TC_CAST(act), "vlan");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (vlan->vlan_id != NETLINK_FILTER_UNUSED) {
		err = rtnl_vlan_set_vlan_id(act, (uint16_t)vlan->vlan_id);
		if (err || vlan->vlan_id < 0) {
			dbg_err_fn_ret(rtnl_vlan_set_vlan_id, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	if (vlan->vlan_prio != NETLINK_FILTER_UNUSED) {
		err = rtnl_vlan_set_vlan_prio(act, (uint8_t)vlan->vlan_prio);
		if (err || vlan->vlan_prio < 0) {
			dbg_err_fn_ret(rtnl_vlan_set_vlan_prio, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	if (vlan->vlan_proto != NETLINK_FILTER_UNUSED) {
		err = rtnl_vlan_set_protocol(act,
					     htons((uint16_t)vlan->vlan_proto));
		if (err || vlan->vlan_proto < 0) {
			dbg_err_fn_ret(rtnl_vlan_set_protocol, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	switch (vlan->act_vlan) {
	case NETLINK_FILTER_ACT_VLAN_PUSH:
		err = rtnl_vlan_set_mode(act, TCA_VLAN_ACT_PUSH);
		break;
	case NETLINK_FILTER_ACT_VLAN_POP:
		err = rtnl_vlan_set_mode(act, TCA_VLAN_ACT_POP);
		break;
	case NETLINK_FILTER_ACT_VLAN_MODIFY:
		err = rtnl_vlan_set_mode(act, TCA_VLAN_ACT_MODIFY);
		break;
	default:
		dbg_err("Unsupported action %d\n", vlan->act_vlan);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
	if (err) {
		dbg_err_fn_ret(rtnl_vlan_set_mode, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_vlan_set_action(act, TC_ACT_PIPE);
	if (err) {
		dbg_err_fn_ret(rtnl_vlan_set_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (cookie_non_zero(&vlan->cookie)) {
		err = rtnl_act_set_cookie(act, vlan->cookie.cookie,
					  sizeof(vlan->cookie.cookie));
		if (err) {
			dbg_err_fn_ret(rtnl_act_set_cookie, err);
			ret = PON_ADAPTER_ERR_INVALID_VAL;
			goto err;
		}
	}

	err = rtnl_flower_add_action(cls, act);
	if (err) {
		dbg_err_fn_ret(rtnl_flower_add_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
err:
	if (act)
		rtnl_act_put(act);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_skbedit_add(struct rtnl_cls *cls,
		   const struct netlink_skbedit_data *skbedit)
{
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_act *act = NULL;

	dbg_in_args("%p, %p", cls, skbedit);

	act = rtnl_act_alloc();
	if (!act) {
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		dbg_err_fn(rtnl_act_alloc);
		goto err_put;
	}

	err = rtnl_tc_set_kind(TC_CAST(act), "skbedit");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERROR;
		goto err_put;
	}

	if (skbedit->prio != NETLINK_FILTER_UNUSED) {
		err = rtnl_skbedit_set_priority(act, (uint32_t)skbedit->prio);
		if (err) {
			dbg_err_fn_ret(rtnl_skbedit_set_priority, err);
			ret = PON_ADAPTER_ERROR;
			goto err_put;
		}
	}

	err = rtnl_act_set_cookie(act, (unsigned char *)skbedit->cookie,
				  sizeof(skbedit->cookie));
	if (err) {
		dbg_err_fn_ret(rtnl_act_set_cookie, err);
		ret = PON_ADAPTER_ERR_INVALID_VAL;
		goto err_put;
	}

	err = rtnl_skbedit_set_action(act, skbedit->action);
	if (err) {
		dbg_err_fn_ret(rtnl_skbedit_set_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err_put;
	}

	err = rtnl_flower_add_action(cls, act);
	if (err) {
		dbg_err_fn_ret(rtnl_flower_add_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err_put;
	}
err_put:
	if (act)
		rtnl_act_put(act);
	dbg_out_ret("%d", ret);
	return ret;
}

/* Some actions must be implemented as 2 piped actions */
/* callers need to take lock */
static enum pon_adapter_errno
action_vlan_add_complex(struct rtnl_cls *cls,
			const struct netlink_vlan_data *vlan)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", cls, vlan);

	if (vlan->act_vlan == NETLINK_FILTER_ACT_VLAN_POP_AND_MODIFY) {
		/* PON_AND_MODIFY - as the name suggests is implemented as
		   2 actions - POP followed by modify */
		struct netlink_vlan_data vlan_tmp = *vlan;

		vlan_tmp.act_vlan = NETLINK_FILTER_ACT_VLAN_POP;
		ret = action_vlan_add(cls, &vlan_tmp);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		vlan_tmp.act_vlan = NETLINK_FILTER_ACT_VLAN_MODIFY;
		ret = action_vlan_add_complex(cls, &vlan_tmp);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		dbg_out_ret("%d", ret);
		return ret;
	}

	if (vlan->act_vlan == NETLINK_FILTER_ACT_VLAN_MODIFY_AND_PUSH) {
		/* PON_AND_MODIFY - as the name suggests is implemented as
		   2 actions - POP followed by modify */
		struct netlink_vlan_data vlan_tmp = *vlan;

		vlan_tmp.act_vlan = NETLINK_FILTER_ACT_VLAN_MODIFY;
		ret = action_vlan_add(cls, &vlan_tmp);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		vlan_tmp.act_vlan = NETLINK_FILTER_ACT_VLAN_PUSH;
		vlan_tmp.vlan_id = vlan_tmp.svlan_id;
		vlan_tmp.vlan_prio = vlan_tmp.svlan_prio;
		vlan_tmp.vlan_proto = vlan_tmp.svlan_eth_type;

		ret = action_vlan_add(cls, &vlan_tmp);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = action_vlan_add(cls, vlan);

	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_mirred_add(struct pon_net_netlink *nl_ctx,
		  struct rtnl_cls *cls,
		  const struct netlink_mirred_data *data)
{
	int err;
	struct rtnl_act *act = NULL;
	struct rtnl_link *link = NULL;
	int ifindex;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %p", nl_ctx, cls, data);

	act = rtnl_act_alloc();
	if (!act) {
		dbg_err_fn(rtnl_act_alloc);
		goto err;
	}

	err = rtnl_tc_set_kind(TC_CAST(act), "mirred");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache,
				     data->dev);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, data->dev);
		goto err;
	}

	ifindex = rtnl_link_get_ifindex(link);
	if (ifindex < 0) {
		dbg_err("Invalid ifindex\n");
		goto err;
	}
	rtnl_mirred_set_ifindex(act, (uint32_t)ifindex);

	rtnl_mirred_set_action(act, data->action);

	rtnl_mirred_set_policy(act, data->policy);

	err = rtnl_flower_add_action(cls, act);
	if (err) {
		dbg_err_fn_ret(rtnl_flower_add_action, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}
err:
	if (act)
		rtnl_act_put(act);
	if (link)
		rtnl_link_put(link);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_mirred_add_complex(struct pon_net_netlink *nl_ctx,
			  struct rtnl_cls *cls,
			  const struct netlink_mirred_data *data)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p", nl_ctx, cls, data);

	if (data->policy == TC_ACT_PIPE && data->vlan_act) {
		ret = action_vlan_add(cls, data->vlan_act);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}
		ret = action_mirred_add(nl_ctx, cls, data);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = action_mirred_add(nl_ctx, cls, data);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static enum pon_adapter_errno
action_skbedit_add_complex(struct pon_net_netlink *nl_ctx,
			   struct rtnl_cls *cls,
			   const struct netlink_skbedit_data *data)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p", nl_ctx, cls, data);

	if (data->vlan_act) {
		ret = action_vlan_add(cls, data->vlan_act);
		if (ret) {
			dbg_out_ret("%d", ret);
			return ret;
		}
	}

	ret = action_skbedit_add(cls, data);
	dbg_out_ret("%d", ret);
	return ret;
}

/* Create rtnl_cls from filter structure */
/* callers need to take lock */
static struct rtnl_cls
*create_cls_from_filter(struct pon_net_netlink *nl_ctx,
			const struct netlink_filter *filter)
{
	struct rtnl_cls *ret = NULL;
	int err;
	struct rtnl_cls *cls = NULL;
	struct rtnl_link *link = NULL;
	int clean_cls = 0;
	struct netlink_vlan_data *vlan;

	dbg_in_args("%p, %p", nl_ctx, filter);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache,
				     filter->device);
	if (!link) {
		dbg_err_fn_ifname(rtnl_link_get_by_name, filter->device);
		goto err;
	}

	cls = rtnl_cls_alloc();
	if (!cls) {
		dbg_err_fn(rtnl_cls_alloc);
		goto err;
	}
	clean_cls = 1;

	err = rtnl_tc_set_kind(TC_CAST(cls), "flower");
	if (err) {
		dbg_err_fn_ret(rtnl_tc_set_kind, err);
		goto err;
	}

	if (filter->prio != NETLINK_FILTER_UNUSED)
		rtnl_cls_set_prio(cls, (uint16_t)filter->prio);

	if (filter->proto != NETLINK_FILTER_UNUSED)
		rtnl_cls_set_protocol(cls, (uint16_t)filter->proto);
	else
		rtnl_cls_set_protocol(cls, ETH_P_ALL);

	rtnl_tc_set_ifindex(TC_CAST(cls), rtnl_link_get_ifindex(link));

	if (filter->handle != (uint32_t)NETLINK_FILTER_UNUSED)
		rtnl_tc_set_handle(TC_CAST(cls), filter->handle);

	switch (filter->dir) {
	case NETLINK_FILTER_DIR_INGRESS:
		rtnl_tc_set_parent(TC_CAST(cls),
			TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_INGRESS));
		break;
	case NETLINK_FILTER_DIR_EGRESS:
		rtnl_tc_set_parent(TC_CAST(cls),
			TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_EGRESS));
		break;
	default:
		goto err;
	}

	if (filter->vlan_id != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_vlan_id(cls, (uint32_t)filter->vlan_id);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_vlan_id, err);
			goto err;
		}
	}

	if (filter->vlan_prio != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_vlan_prio(cls,
						(uint32_t)filter->vlan_prio);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_vlan_prio, err);
			goto err;
		}
	}

	if (filter->vlan_proto != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_vlan_eth_type(cls,
						  (uint16_t)filter->vlan_proto);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_vlan_eth_type, err);
			goto err;
		}
	}

	if (filter->cvlan_id != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_cvlan_id(cls,
					       (uint32_t)filter->cvlan_id);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_cvlan_id, err);
			goto err;
		}
	}

	if (filter->cvlan_prio != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_cvlan_prio(cls,
						 (uint32_t)filter->cvlan_prio);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_cvlan_prio, err);
			goto err;
		}
	}

	if (filter->cvlan_proto != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_cvlan_eth_type(cls,
						 (uint16_t)filter->cvlan_proto);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_cvlan_eth_type, err);
			goto err;
		}
	}

	if (filter->classid_maj != NETLINK_FILTER_UNUSED &&
	    filter->classid_min != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_classid(
		    cls, TC_HANDLE((unsigned int)filter->classid_maj,
				   (unsigned int)filter->classid_min));
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_classid, err);
			goto err;
		}
	}

	if (filter->indev[0]) {
		err = rtnl_flower_set_indev(cls, filter->indev);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_indev, err);
			goto err;
		}
	}

	if (filter->ip_tos != NETLINK_FILTER_UNUSED) {
		err = rtnl_flower_set_ip_tos(cls, (uint8_t)filter->ip_tos);
		if (err || filter->ip_tos < 0 || filter->ip_tos > 0xFF) {
			dbg_err_fn_ret(rtnl_flower_set_ip_tos, err);
			goto err;
		}
	}

	if (filter->act == NETLINK_FILTER_ACT_VLAN) {
		vlan = filter->act_data;
		err = action_vlan_add_complex(cls, vlan);
		if (err) {
			dbg_err_fn_ret(action_vlan_add_complex, err);
			goto err;
		}
	} else if (filter->act == NETLINK_FILTER_ACT_COLMARK) {
		err = action_colmark_add(cls, filter->act_data);
		if (err) {
			dbg_err_fn_ret(action_colmark_add, err);
			goto err;
		}
	} else if (filter->act == NETLINK_FILTER_ACT_POLICE) {
		err = action_police_add(cls, filter->act_data);
		if (err) {
			dbg_err_fn_ret(action_police_add, err);
			goto err;
		}
	} else if (filter->act == NETLINK_FILTER_ACT_MIRRED) {
		err = action_mirred_add_complex(nl_ctx, cls, filter->act_data);
		if (err) {
			dbg_err_fn_ret(action_mirred_add, err);
			goto err;
		}
	} else if (filter->act == NETLINK_FILTER_ACT_SKBEDIT) {
		err = action_skbedit_add_complex(nl_ctx, cls,
						 filter->act_data);
		if (err) {
			dbg_err_fn_ret(action_skbedit_add_complex, err);
			goto err;
		}
	} else {
		err = action_gact_add(cls, filter->act, filter->act_data);
		if (err) {
			dbg_err_fn_ret(action_gact_add, err);
			goto err;
		}
	}

	if (filter->eth_dst) {
		err = rtnl_flower_set_eth_dst(cls, filter->eth_dst_addr,
					      ETH_ALEN);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_eth_dst, err);
			goto err;
		}

		if (bytes_non_zero(filter->eth_dst_mask,
				   ARRAY_SIZE(filter->eth_dst_mask))) {
			err = rtnl_flower_set_eth_dst_mask(
			    cls, filter->eth_dst_mask, ETH_ALEN);
			if (err) {
				dbg_err_fn_ret(rtnl_flower_set_eth_dst_mask,
					       err);
				goto err;
			}
		}
	}

	if (filter->ip_proto) {
		err = rtnl_flower_set_ip_proto(cls, filter->ip_proto);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_ip_proto, err);
			goto err;
		}
	}

	if (filter->icmpv6_type) {
		err = rtnl_flower_set_icmpv6_type(cls, filter->icmpv6_type);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_icmpv6_type, err);
			goto err;
		}
	}

	if (filter->ipv4_dst_en) {
		err = rtnl_flower_set_ipv4_dst(cls, filter->ipv4_dst_addr);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_ipv4_dst, err);
			goto err;
		}
		err = rtnl_flower_set_ipv4_dst_mask(cls, filter->ipv4_dst_mask);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_ipv4_mask, err);
			goto err;
		}
	}

	if (filter->ipv6_dst_en) {
		err = rtnl_flower_set_ipv6_dst(cls, filter->ipv6_dst_addr,
					       IPV6_ALEN);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_ipv6_dst, err);
			goto err;
		}
		err = rtnl_flower_set_ipv6_dst_mask(cls, filter->ipv6_dst_mask,
						    IPV6_ALEN);
		if (err) {
			dbg_err_fn_ret(rtnl_flower_set_ipv6_mask, err);
			goto err;
		}
	}

	ret = cls;
	clean_cls = 0;

err:
	if (clean_cls && cls)
		rtnl_cls_put(cls);
	if (link)
		rtnl_link_put(link);
	dbg_out_ret("%p", ret);
	return ret;
}

/* callers need to take lock */
static int __pon_net_cls_add(struct pon_net_netlink *nl_ctx,
			     struct rtnl_cls *cls,
			     uint8_t tca_cls_flag)
{
	struct nl_sock *sock = nl_ctx->nl_sock;
	int ret;

	dbg_in_args("%p, %p", sock, cls);

	ret = rtnl_flower_set_flags(cls, tca_cls_flag);
	if (ret) {
		FN_ERR_RET(ret, rtnl_flower_set_flags, ret);
		return ret;
	}

	netlink_debug_tc_flower_filter_add(nl_ctx->rtnl_link_cache, cls);
	ret = rtnl_cls_add(sock, cls, NLM_F_CREATE);
	if (ret == 0) {
		dbg_out_ret("%d", 0);
		return 0;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static uint8_t pon_net_map_offload_flags(enum netlink_offload_flags flags)
{
	switch (flags) {
	case NETLINK_HW_ONLY:
		return TCA_CLS_FLAGS_SKIP_SW;
	case NETLINK_SW_ONLY:
		return TCA_CLS_FLAGS_SKIP_HW;
	case NETLINK_HW_SW:
		return 0;
	default:
		break;
	}

	return TCA_CLS_FLAGS_SKIP_SW;
}

/* A wrapper for rtnl_cls_add. It will try to add a filter with
 * TCA_CLS_FLAGS_SKIP_SW flags. If it fails then it means HW offloading
 * is probably not supported and a warning will be printed and classifier
 * will be added with no flags
 */
/* callers need to take lock */
static int pon_net_cls_add(struct pon_net_netlink *nl_ctx,
			   struct rtnl_cls *cls,
			   enum netlink_offload_flags flags,
			   const char *device)
{
	struct nl_sock *sock = nl_ctx->nl_sock;
	uint8_t tc_flags = pon_net_map_offload_flags(flags);
	int sw_ret;
	int ret = 0;

	dbg_in_args("%p, %p", sock, cls);

	ret = __pon_net_cls_add(nl_ctx, cls, tc_flags);
	if (ret == 0) {
		dbg_out_ret("%d", 0);
		return 0;
	}

	if (tc_flags == TCA_CLS_FLAGS_SKIP_SW) {
		sw_ret = __pon_net_cls_add(nl_ctx, cls, TCA_CLS_FLAGS_SKIP_HW);
		if (sw_ret) {
			FN_ERR_NL_RET(sw_ret, __pon_net_cls_add,
				      device, sw_ret);
			return sw_ret;
		}
		/* Classifier was added to SW.
		 * This means that HW offloading is not supported
		 */
		dbg_err("Flower classifier was added to SW on %s (hw offloading probably not supported - error returned by libnl: %d (%s))\n",
			device, ret, nl_geterror(ret));
		ret = 0;
	}

	dbg_out_ret("%d", 0);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_filter_add(struct pon_net_netlink *nl_ctx,
					  const struct netlink_filter *filter)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_cls *cls_ingress = NULL;
	struct rtnl_cls *cls_egress = NULL;
	struct netlink_filter filter_tmp = {0};
	int err;

	dbg_in_args("%p, %p", nl_ctx, filter);

	if (memcpy_s(&filter_tmp, sizeof(filter_tmp),
				filter, sizeof(*filter))) {
		dbg_err_fn(memcpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	pthread_mutex_lock(&nl_ctx->nl_lock);

	/* There are no bidirectional filters. If the filter is bidirectional
	   we should create 2 filters instead */

	if (filter->dir & NETLINK_FILTER_DIR_INGRESS) {
		filter_tmp.dir = NETLINK_FILTER_DIR_INGRESS;
		cls_ingress = create_cls_from_filter(nl_ctx, &filter_tmp);
		if (!cls_ingress) {
			dbg_err_fn_ret(create_cls_from_filter, 0);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}

		err = pon_net_cls_add(nl_ctx, cls_ingress, filter->flags,
				      filter->device);
		if (err) {
			dbg_err_fn_ret(pon_net_cls_add, err);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

	if (filter->dir & NETLINK_FILTER_DIR_EGRESS) {
		filter_tmp.dir = NETLINK_FILTER_DIR_EGRESS;
		cls_egress = create_cls_from_filter(nl_ctx, &filter_tmp);
		if (!cls_egress) {
			dbg_err_fn_ret(create_cls_from_filter, 0);
			/* If we fail to create egress classifier but we
			   succeeded to create ingress classifier, then we
			   should remove it */
			if (cls_ingress) {
				netlink_debug_tc_flower_filter_del(
				    nl_ctx->rtnl_link_cache, cls_ingress);
				rtnl_cls_delete(nl_ctx->nl_sock,
						cls_ingress, 0);
			}
			ret = PON_ADAPTER_ERROR;
			goto err;
		}

		err = pon_net_cls_add(nl_ctx, cls_egress, filter->flags,
				      filter->device);
		if (err) {
			dbg_err_fn_ret(hw_or_sw_cls_add, err);
			/* If we fail to create egress classifier but we
			   succeeded to create ingress classifier, then we
			   should remove it */
			if (cls_ingress) {
				netlink_debug_tc_flower_filter_del(
				    nl_ctx->rtnl_link_cache, cls_ingress);
				rtnl_cls_delete(nl_ctx->nl_sock,
						cls_ingress, 0);
			}
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

err:
	if (cls_ingress)
		rtnl_cls_put(cls_ingress);
	if (cls_egress)
		rtnl_cls_put(cls_egress);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* callers need to take lock */
static void destroy_pon_net_netdevice(struct rtnl_link *link, void *arg)
{
	/* TODO: check arg */
	struct pon_net_netlink *nl_ctx = arg;
	static const char *const valid_prefixes[] = PON_IFNAME_PREFIXES;
	unsigned int i;
	char *ifname = rtnl_link_get_name(link);

	dbg_in_args("%p, %p", link, arg);

	if (!ifname)
		return;

	for (i = 0; i < ARRAY_SIZE(valid_prefixes); ++i) {
		/* If link name starts with one of specified prefixes */
		if (strncmp(valid_prefixes[i], ifname,
			    strnlen_s(valid_prefixes[i],
			    IF_NAMESIZE)) == 0) {
			netlink_debug_ip_link_del(ifname);
			rtnl_link_delete(nl_ctx->nl_sock, link);
			break;
		}
	}
	dbg_out();
}

/* does locking */
enum pon_adapter_errno
netlink_netdevices_clear_all(struct pon_net_netlink *nl_ctx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;

	dbg_in_args("%p", nl_ctx);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	rtnl_link_foreach(nl_ctx->rtnl_link_cache,
			  destroy_pon_net_netdevice,
			  nl_ctx);

	err = nl_cache_refill(nl_ctx->nl_sock,
			      nl_ctx->rtnl_link_cache);
	if (err) {
		dbg_err_fn_ret(nl_cache_refill, err);
		ret = PON_ADAPTER_ERROR;
	}

	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
int netlink_netdevice_ifindex_get(struct pon_net_netlink *nl_ctx,
				  const char *ifname)
{
	int ret;

	dbg_in_args("%p, \"%s\"", nl_ctx, ifname);

	pthread_mutex_lock(&nl_ctx->nl_lock);
	ret = rtnl_link_name2i(nl_ctx->rtnl_link_cache, ifname);
	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno
netlink_ifindex_to_name(struct pon_net_netlink *nl_ctx, int ifindex, char *name,
			size_t len)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char *endptr;

	dbg_in_args("%p, %d, %p, %zd", nl_ctx, ifindex, name, len);

	memset(name, 0, len);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	endptr = rtnl_link_i2name(nl_ctx->rtnl_link_cache, ifindex, name,
				  len);
	if (!endptr) {
		dbg_err("Could not find interface index %i\n", ifindex);
		ret = PON_ADAPTER_ERROR;
	}

	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

int netlink_name_to_ifindex(struct pon_net_netlink *nl_ctx, char *name)
{
	int ifindex;

	dbg_in_args("%p, %s", nl_ctx, name);

	pthread_mutex_lock(&nl_ctx->nl_lock);
	ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache, name);
	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ifindex);
	return ifindex;
}

/* does locking */
enum pon_adapter_errno
netlink_bport_learning_limit_set(struct pon_net_netlink *nl_ctx,
				 const char *ifname,
				 uint8_t omci_depth_limit)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *changes;
	int is_slave = 0;
	int err;

	dbg_in_args("%p, \"%s\", %d", nl_ctx, ifname, omci_depth_limit);

	/* netlink_is_slave is doing locking on its own
	 * and must be called before getting the lock
	 */
	err = netlink_is_slave(nl_ctx, ifname, &is_slave);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, netlink_is_slave, err);
		return err;
	}

	if (is_slave == 0) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	pthread_mutex_lock(&nl_ctx->nl_lock);

	changes = rtnl_link_alloc();
	if (!changes) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	/* The max learning limit value accepted by kernel is 254 */
	omci_depth_limit = (omci_depth_limit == 255) ? 254 : omci_depth_limit;

	/* The value for unlimited learning defined by the caller (for example
	 * OMCI) is zero. The corresponding kernel-defined value
	 * for unlimited learning is -1.
	 * Note: The caller has no value for 'disable learning', thus
	 * the corresponding kernel-defined value for 'disable learning',
	 * which is zero, is never sent to kernel
	 */
	rtnl_link_set_family(changes, AF_BRIDGE);
	if (rtnl_link_bridge_set_learning_limit(changes, omci_depth_limit ?
						omci_depth_limit : -1)) {
		dbg_err_fn(rtnl_link_bridge_set_learning_limit);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = link_change_by_name(nl_ctx->rtnl_link_cache,
				  nl_ctx->nl_sock,
				  ifname, changes, 0);
	if (err) {
		FN_ERR_NL(err, link_change_by_name, ifname);
		ret = PON_ADAPTER_ERROR;
	}

err:
	if (changes)
		rtnl_link_put(changes);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
static enum pon_adapter_errno bport_vlan_add(struct pon_net_netlink *nl_ctx,
					     const char *ifname, uint16_t vlan,
					     bool self, bool delete)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *changes = NULL;
	int ifindex;
	int err = 0;

	dbg_in_args("%p, \"%s\", %u, %d", nl_ctx, ifname, vlan, self);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	changes = rtnl_link_alloc();
	if (!changes) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	rtnl_link_set_family(changes, AF_BRIDGE);

	if (self)
		rtnl_link_bridge_set_self(changes);
	else
		rtnl_link_bridge_set_master(changes);

	err = rtnl_link_bridge_set_vlan_range(changes, vlan, -1);
	if (err) {
		FN_ERR_NL(err, rtnl_link_set_family, ifname);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (delete) {
		ifindex = rtnl_link_name2i(nl_ctx->rtnl_link_cache,
					   ifname);
		if (ifindex <= 0) {
			FN_ERR_NL(ifindex, rtnl_link_name2i, ifname);
			ret = PON_ADAPTER_ERR_NOT_FOUND;
			goto err;
		}

		rtnl_link_set_ifindex(changes, ifindex);

		/*
		 * This will not delete the interface, but rather
		 * it will delete the only the specified VLAN.
		 */
		netlink_debug_bridge_vlan(ifname, vlan, self, delete);
		err = rtnl_link_bridge_delete(nl_ctx->nl_sock, changes);
		if (err) {
			FN_ERR_NL(err, rtnl_link_bridge_delete, ifname);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	} else {
		netlink_debug_bridge_vlan(ifname, vlan, self, delete);
		err = link_change_by_name(nl_ctx->rtnl_link_cache,
					  nl_ctx->nl_sock,
					  ifname, changes, 0);
		if (err) {
			FN_ERR_NL(err, link_change_by_name, ifname);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
	}

err:
	if (changes)
		rtnl_link_put(changes);

	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_bport_vlan_add(struct pon_net_netlink *nl_ctx,
					      const char *ifname, uint16_t vlan,
					      bool self)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %u, %d", nl_ctx, ifname, vlan, self);

	ret = bport_vlan_add(nl_ctx, ifname, vlan, self, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_vlan_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno
netlink_bport_vlan_del(struct pon_net_netlink *nl_ctx,
		       const char *ifname,
		       uint16_t vlan,
		       bool self)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %u, %d", nl_ctx, ifname, vlan, self);

	ret = bport_vlan_add(nl_ctx, ifname, vlan, self, true);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_vlan_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
void netlink_colmark_defaults(struct netlink_colmark_data *colmark)
{
	static const struct netlink_colmark_data defaults = {
		.mode = NETLINK_FILTER_UNUSED,
		.drop_precedence = NETLINK_FILTER_UNUSED,
		.meter_type = NETLINK_FILTER_UNUSED
	};

	dbg_in_args("%p", colmark);

	*colmark = defaults;

	dbg_out();
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno
netlink_color_marking_set(struct pon_net_netlink *nl_ctx, const char *ifname,
			  enum netlink_filter_dir dir,
			  uint8_t color_marking,
			  uint32_t handle, uint16_t priority)
{
	struct netlink_filter filter = {0};
	struct netlink_colmark_data colmark = {0};
	enum pon_adapter_errno ret;
	int res;

	dbg_in_args("%p, \"%s\", %d, %d, %u, %u", nl_ctx, ifname, dir,
		    color_marking, handle, priority);

	netlink_filter_defaults(&filter);
	netlink_colmark_defaults(&colmark);

	res = strncpy_s(filter.device, sizeof(filter.device), ifname,
			IF_NAMESIZE);
	if (res) {
		dbg_err_fn_ret(strncpy_s, res);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	filter.act = NETLINK_FILTER_ACT_COLMARK;
	colmark.drop_precedence = color_marking;
	filter.act_data = &colmark;
	filter.dir = dir;
	filter.handle = handle;
	filter.prio = (int)priority;
	filter.proto = ETH_P_ALL;

	netlink_filter_clear_one(nl_ctx, ifname, ETH_P_ALL, priority, handle,
				 dir);

	if (!color_marking) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = netlink_filter_add(nl_ctx, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno
netlink_police_set(struct pon_net_netlink *nl_ctx, const char *ifname,
		   enum netlink_filter_dir dir, uint32_t handle,
		   uint16_t priority, struct netlink_police_data *police)
{
	struct netlink_filter filter = {0};
	enum pon_adapter_errno ret;
	int res;

	dbg_in_args("%p, \"%s\", %d, %u, %u, %p", nl_ctx, ifname, dir,
		    handle, priority, police);

	netlink_filter_defaults(&filter);

	res = strncpy_s(filter.device, sizeof(filter.device), ifname,
			IF_NAMESIZE);
	if (res) {
		dbg_err_fn_ret(strncpy_s, res);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	filter.act = NETLINK_FILTER_ACT_POLICE;
	filter.act_data = police;
	filter.dir = dir;
	filter.handle = handle;
	filter.prio = (int)priority;
	filter.proto = ETH_P_ALL;

	netlink_filter_clear_one(nl_ctx, ifname, ETH_P_ALL, priority, handle,
				 dir);

	ret = netlink_filter_add(nl_ctx, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* TRAP */

/* no locking needed */
enum pon_adapter_errno netlink_filter_mac(struct netlink_filter *flt,
					  const char *ifname, uint8_t dir,
					  uint32_t hw_tc, int proto,
					  int vlan_id, const uint8_t *eth_dst,
					  enum netlink_filter_act act,
					  void *act_data)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, \"%s\", %u, %u, %d, %d, %p, %d, %p", flt, ifname,
		    dir, hw_tc, proto, vlan_id, eth_dst, act, act_data);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)sizeof(filter.device) - 1) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.dir = dir;
	filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
	filter.classid_min = (int)(TC_H_MIN_PRIORITY + hw_tc);
	filter.act = act;
	filter.act_data = act_data;
	filter.proto = proto;
	filter.vlan_id = vlan_id;
	if (eth_dst) {
		filter.eth_dst = true;
		res = memcpy_s(filter.eth_dst_addr, ETH_ALEN, eth_dst,
			       ETH_ALEN);
		if (res) {
			dbg_err_fn_ret(memcpy_s, res);
			return PON_ADAPTER_ERROR;
		}
	}

	*flt = filter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* no lock, as all called functions are doing it itself */
static enum pon_adapter_errno
trap_set(struct pon_net_netlink *nl_ctx, const char *ifname, uint8_t dir,
	 uint32_t hw_tc, int proto, uint32_t handle, int priority,
	 int vlan_id, const uint8_t *eth_dst)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, \"%s\", %u, %d, %d, %u, %d, %d, %p", nl_ctx, ifname,
		    dir, hw_tc, proto, handle, priority, vlan_id, eth_dst);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)sizeof(filter.device) - 1) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.prio = priority;
	filter.dir = dir;
	filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
	filter.classid_min = (int)(TC_H_MIN_PRIORITY + hw_tc);
	filter.act = NETLINK_FILTER_ACT_TRAP;
	filter.handle = handle;
	filter.proto = proto;
	filter.vlan_id = vlan_id;
	if (eth_dst) {
		filter.eth_dst = true;
		res = memcpy_s(filter.eth_dst_addr, ETH_ALEN, eth_dst,
			       ETH_ALEN);
		if (res) {
			dbg_err_fn_ret(memcpy_s, res);
			return PON_ADAPTER_ERROR;
		}
	}

	netlink_filter_clear_one(nl_ctx, ifname, (uint16_t)proto,
				 (uint16_t)priority, handle, dir);

	ret = netlink_filter_add(nl_ctx, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return PON_ADAPTER_SUCCESS;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_omci_trap_set(struct pon_net_netlink *nl_ctx,
					     const char *ifname,
					     enum netlink_filter_dir dir,
					     uint32_t handle, uint16_t prio)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %d, %u, %u",
		    nl_ctx, ifname, dir, handle, prio);

	ret = trap_set(nl_ctx, ifname, dir, OMCI_HW_TC, ETH_P_ALL, handle,
		       prio, NETLINK_FILTER_UNUSED, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, trap_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* does locking */
enum pon_adapter_errno
netlink_iphost_create(struct pon_net_netlink *nl_ctx,
		      const char *ifname,
		      const char *master)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct rtnl_link *iphost;
	int err;

	dbg_in_args("%p, %s, %s", nl_ctx, ifname, master);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	iphost = rtnl_link_alloc();
	if (!iphost) {
		dbg_err_fn(rtnl_link_alloc);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	err = rtnl_link_set_type(iphost, "iphost");
	if (err < 0) {
		dbg_err_fn_ret(rtnl_link_set_type, err);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ret = set_link_master_device(nl_ctx, iphost, master);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(set_link_pon_master_device, ret);
		goto err;
	}

	rtnl_link_set_name(iphost, ifname);

	dbg_prn("ip link add %s link %s type iphost\n", ifname,
		master);
	err = link_add(nl_ctx, iphost, NLM_F_CREATE);
	if (err) {
		FN_ERR_NL(err, link_add, ifname);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

err:
	if (iphost)
		rtnl_link_put(iphost);
	pthread_mutex_unlock(&nl_ctx->nl_lock);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_iphost_destroy(struct pon_net_netlink *nl_ctx,
					      const char *ifname)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %s", nl_ctx, ifname);

	ret = netlink_netdevice_destroy(nl_ctx, ifname);
	dbg_out_ret("%d", ret);
	return ret;
}

/* no locking needed */
enum pon_adapter_errno
netlink_filter_vlan(struct netlink_filter *flt,
		    const char *ifname,
		    uint32_t hw_tc,
		    int vlan_id,
		    enum netlink_filter_act act,
		    void *act_data)
{
	dbg_in_args("%p, \"%s\", %u, %d, %d, %p", flt, ifname, hw_tc, vlan_id,
		    act, act_data);

	netlink_filter_defaults(flt);
	flt->act_data = act_data;

	snprintf(flt->device, IF_NAMESIZE, "%s", ifname);
	flt->proto = ETH_P_8021Q;
	/* Use same indev as the filter device to make sure that the global
	 * rule matches only PON DS traffic.
	 */
	if (strncpy_s(flt->indev, sizeof(flt->indev),
		      PON_MASTER_DEVICE, sizeof(PON_MASTER_DEVICE))) {
		dbg_err_fn(strncpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}
	flt->vlan_id = vlan_id;
	flt->dir = NETLINK_FILTER_DIR_INGRESS;
	flt->act = act;
	flt->act_data = act_data;
	flt->classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT));
	flt->classid_min = (int)(TC_H_MIN_PRIORITY + hw_tc);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* no lock, as all called functions are doing it itself */
enum pon_adapter_errno netlink_filter_del(struct pon_net_netlink *nl_ctx,
					  struct netlink_filter *filter)
{
	enum pon_adapter_errno ret;
	uint16_t proto = filter->proto == NETLINK_FILTER_UNUSED
			     ? ETH_P_ALL
			     : (uint16_t)filter->proto;
	dbg_in_args("%p, %p", nl_ctx, filter);

	ret = netlink_filter_clear_one(nl_ctx, filter->device, proto,
				       (uint16_t)filter->prio, filter->handle,
				       filter->dir);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define IGMP_UNIQUE_ID 1
#define TAG_IGMP_UNIQUE_ID 2
/* no locking needed */
enum pon_adapter_errno
netlink_filter_igmp_trap(struct netlink_filter *flt, bool tagged)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, %d", flt, tagged);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), PON_MASTER_DEVICE);
	if (res > (int)(sizeof(filter.device) - 1)) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	if (!tagged) {
		/* untagged IGMP packets */
		filter.dir = NETLINK_FILTER_DIR_INGRESS;
		filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
		filter.classid_min = TC_H_MIN_PRIORITY + IGMP_HW_TC;
		filter.act = NETLINK_FILTER_ACT_TRAP;
		filter.proto = ETH_P_IP;
		filter.ip_proto = IPPROTO_IGMP;
	} else {
		/* tagged IGMP packets */
		filter.dir = NETLINK_FILTER_DIR_INGRESS;
		filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
		filter.classid_min = TC_H_MIN_PRIORITY + IGMP_HW_TC;
		filter.act = NETLINK_FILTER_ACT_TRAP;
		filter.proto = ETH_P_8021Q;
		filter.vlan_proto = ETH_P_IP;
		filter.ip_proto = IPPROTO_IGMP;
	}

	*flt = filter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define ICMPV6_UNIQUE_ID(val) \
	((val) << 4)
#define TAG_ICMPV6_UNIQUE_ID(val) \
	((val) << 4 | 0x08)

/* no locking needed */
enum pon_adapter_errno netlink_filter_icmpv6(struct netlink_filter *flt,
					     const char *ifname,
					     uint32_t hw_tc,
					     uint8_t type,
					     bool tagged,
					     enum netlink_filter_act act,
					     void *act_data)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, %s, %u, %u, %d, %p", flt, ifname, type, tagged, act,
		    act_data);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)(sizeof(filter.device) - 1)) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.act = act;
	filter.act_data = act_data;
	filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT));
	filter.classid_min = (int)(TC_H_MIN_PRIORITY + hw_tc);

	if (!tagged) {
		/* untagged ICMPv6 packets */
		filter.dir = NETLINK_FILTER_DIR_INGRESS;
		filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
		filter.classid_min = TC_H_MIN_PRIORITY + IGMP_HW_TC;
		filter.proto = ETH_P_IPV6;
		filter.ip_proto = IPPROTO_ICMPV6;
		filter.icmpv6_type = type;

		*flt = filter;
	} else {
		/* tagged ICMPv6 packets */
		filter.dir = NETLINK_FILTER_DIR_INGRESS;
		filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
		filter.classid_min = TC_H_MIN_PRIORITY + IGMP_HW_TC;
		filter.proto = ETH_P_8021Q;
		filter.vlan_proto = ETH_P_IPV6;
		filter.ip_proto = IPPROTO_ICMPV6;
		filter.icmpv6_type = type;

		*flt = filter;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* no locking needd */
enum pon_adapter_errno netlink_filter_arp(struct netlink_filter *flt,
					  const char *ifname,
					  uint32_t hw_tc,
					  bool tagged,
					  enum netlink_filter_act act,
					  void *act_data)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, %s, %u, %u, %d, %p", flt, ifname, hw_tc, tagged, act,
		    NULL);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)(sizeof(filter.device) - 1)) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
	filter.classid_min = (int)(TC_H_MIN_PRIORITY + hw_tc);
	filter.act = act;
	filter.act_data = act_data;

	if (!tagged) {
		/* untagged ARP packets */
		filter.proto = ETH_P_ARP;

	} else {
		/* tagged ARP packets */
		filter.proto = ETH_P_8021Q;
		filter.vlan_proto = ETH_P_ARP;
	}

	*flt = filter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* no locking needed */
enum pon_adapter_errno netlink_filter_lct_trap_ptp(struct netlink_filter *flt,
						   const char *ifname,
						   bool tagged)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, %s, %u", flt, ifname, tagged);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)(sizeof(filter.device) - 1)) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.classid_maj = (int)(TC_H_MAJ(TC_H_CLSACT) >> 16);
	filter.classid_min = TC_H_MIN_PRIORITY + LCT_PTP_HW_TC;
	filter.act = NETLINK_FILTER_ACT_TRAP;

	if (!tagged) {
		/* untagged PTP packets */
		filter.proto = ETH_P_1588;
	} else {
		/* tagged PTP packets */
		filter.proto = ETH_P_8021Q;
		filter.vlan_proto = ETH_P_1588;
	}

	*flt = filter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define MC_UNSPEC_GROUP_IP	0xe0000000U /* 224.0.0.0 */
#define MC_UNSPEC_MASK		0xf0000000U /* /4 */

/* no locking needed */
enum pon_adapter_errno
netlink_filter_mc_unspec_ipv4_drop(struct netlink_filter *flt,
				   const char *ifname, int proto)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, %s, %d", flt, ifname, proto);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)(sizeof(filter.device) - 1)) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.act = NETLINK_FILTER_ACT_DROP;
	filter.proto = proto;
	filter.ipv4_dst_en = true;
	filter.ipv4_dst_addr = htonl(MC_UNSPEC_GROUP_IP);
	filter.ipv4_dst_mask = htonl(MC_UNSPEC_MASK);

	if (proto == ETH_P_8021Q)
		filter.vlan_proto = ETH_P_IP;

	*flt = filter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* no locking needed */
enum pon_adapter_errno
netlink_filter_mc_unspec_ipv6_drop(struct netlink_filter *flt,
				   const char *ifname, int proto)
{
	struct netlink_filter filter;
	int res;

	dbg_in_args("%p, %s, %d", flt, ifname, proto);

	netlink_filter_defaults(&filter);

	res = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (res > (int)(sizeof(filter.device) - 1)) {
		FN_ERR_RET(res, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.act = NETLINK_FILTER_ACT_DROP;
	filter.proto = proto;
	filter.ipv6_dst_en = true;
	/* Address ff00:: */
	filter.ipv6_dst_addr[0] = 0xff;
	/* Mask /8 */
	filter.ipv6_dst_mask[0] = 0xff;

	if (proto == ETH_P_8021Q)
		filter.vlan_proto = ETH_P_IPV6;

	*flt = filter;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* does locking */
bool netlink_netdevice_exists(struct pon_net_netlink *nl_ctx,
			      const char *ifname)
{
	struct rtnl_link *link = NULL;
	bool ret = false;

	dbg_in_args("%p, \"%s\"", nl_ctx, ifname);

	pthread_mutex_lock(&nl_ctx->nl_lock);

	link = rtnl_link_get_by_name(nl_ctx->rtnl_link_cache, ifname);
	if (link) {
		ret = true;
		rtnl_link_put(link);
	}

	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ret);
	return ret;
}

/* does locking */
enum pon_adapter_errno netlink_cache_refill(struct pon_net_netlink *nl_ctx)
{
	int err;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	pthread_mutex_lock(&nl_ctx->nl_lock);

	err = nl_cache_refill(nl_ctx->nl_sock, nl_ctx->rtnl_link_cache);
	if (err)
		ret = PON_ADAPTER_ERROR;

	pthread_mutex_unlock(&nl_ctx->nl_lock);

	dbg_out_ret("%d", ret);
	return ret;
}

struct pon_net_netlink *netlink_create(void)
{
	struct pon_net_netlink *nl_ctx;
	int err;

	dbg_in();

	nl_ctx = calloc(1, sizeof(*nl_ctx));

	if (!nl_ctx)
		goto err;

	err = pthread_mutex_init(&nl_ctx->nl_lock, NULL);
	if (err) {
		dbg_err_fn(pthread_mutex_init);
		goto err_ctx;
	}

	nl_ctx->nl_sock = nl_socket_alloc();
	if (!nl_ctx->nl_sock) {
		dbg_err_fn(nl_socket_alloc);
		goto err_mutex;
	}

	err = nl_connect(nl_ctx->nl_sock, NETLINK_ROUTE);
	if (err) {
		dbg_err_fn(nl_connect);
		goto err_sock;
	}

	nl_ctx->rtnl_link_cache = NULL;
	err = rtnl_link_alloc_cache(nl_ctx->nl_sock,
				    AF_UNSPEC,
				    &nl_ctx->rtnl_link_cache);
	if (err) {
		dbg_err_fn_ret(rtnl_link_alloc_cache, err);
		goto err_sock;
	}

	dbg_out_ret("%p", nl_ctx);
	return nl_ctx;

err_sock:
	nl_close(nl_ctx->nl_sock);
	nl_socket_free(nl_ctx->nl_sock);
err_mutex:
	pthread_mutex_destroy(&nl_ctx->nl_lock);
err_ctx:
	free(nl_ctx);
err:
	dbg_out_ret("%p", NULL);
	return NULL;
}

void netlink_destroy(struct pon_net_netlink *nl_ctx)
{
	dbg_in_args("%p", nl_ctx);

	if (!nl_ctx)
		return;

	if (nl_ctx->nl_sock) {
		if (nl_ctx->rtnl_link_cache)
			nl_cache_free(nl_ctx->rtnl_link_cache);

		nl_close(nl_ctx->nl_sock);
		nl_socket_free(nl_ctx->nl_sock);
	}

	pthread_mutex_destroy(&nl_ctx->nl_lock);

	free(nl_ctx);

	dbg_out();
}
