/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>

#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <netlink/attr.h>
#include <arpa/inet.h>

#include <pon/pon_mcc.h>
#include <pon/pon_mcc_ikm.h>

struct cb_data {
	/** Callback status
	 *  - 1: The callback handler was not called yet.
	 *  - 0: The callback handler was called and this structure was filled,
	 *  and the copy or error_cb function was executed.
	 */
	int running;
	/** A netlink error code (NLE_*) indicating the result of the callback
	 *  If callback was successful, then it is set to NLE_SUCCESS.
	 */
	int errorcode;
};

static struct nla_policy pon_mcc_genl_policy[PON_MCC_A_MAX + 1] = {
	[PON_MCC_A_IFINDEX] = { .type = NLA_U32 },
	[PON_MCC_A_PROTO] = { .type = NLA_U8 },
	[PON_MCC_A_GRP_ADDR] = { .type = NLA_BINARY },
	[PON_MCC_A_SRC_ADDR] = { .type = NLA_BINARY },
	[PON_MCC_A_FLT_MODE] = { .type = NLA_U8 },
	[PON_MCC_A_VALID] = { .type = NLA_U8 },
	[PON_MCC_A_INITIAL] = { .type = NLA_U8 },
	[PON_MCC_A_IGMP_VERSION] = { .type = NLA_U8 },
	[PON_MCC_A_MC_GEM_IFINDEX] = { .type = NLA_U32 },
	[PON_MCC_A_BRIDGE_PORT_ID] = { .type = NLA_U8 },
};

/* Converts netlink errorcode (NLE_*) to errno errorcode */
static int nl_nlerr2syserr(int error)
{
	error = abs(error);
	switch (error) {
	case NLE_AF_NOSUPPORT: return -EAFNOSUPPORT;
	case NLE_AGAIN: return -EAGAIN;
	case NLE_BAD_SOCK: return -EBADF;
	case NLE_BUSY: return -EBUSY;
	case NLE_EXIST: return -EEXIST;
	case NLE_INTR: return -EINTR;
	case NLE_INVAL: return -EINVAL;
	case NLE_NOACCESS: return -EACCES;
	case NLE_NOADDR: return -EADDRNOTAVAIL;
	case NLE_NODEV: return -ENODEV;
	case NLE_NOMEM: return -ENOMEM;
	case NLE_OBJ_NOTFOUND: return -ENOENT;
	case NLE_OPNOTSUPP: return -EOPNOTSUPP;
	case NLE_PERM: return -EPERM;
	case NLE_PROTO_MISMATCH: return -EPROTONOSUPPORT;
	case NLE_RANGE: return -ERANGE;
	case NLE_SUCCESS: return 0;
	default: return -EINVAL;
	}
}

/* Generic functions */

static int prepare(struct nl_sock **nls, struct nl_msg **msg, uint8_t cmd,
		   int flags)
{
	int family;
	int ret;
	void *nl_hdr;

	*nls = nl_socket_alloc();
	if (!(*nls)) {
		fprintf(stderr, "can not alloc netlink socket\n");
		return -NLE_NOMEM;
	}

	ret = genl_connect(*nls);
	if (ret) {
		fprintf(stderr, "can not connect to netlink socket\n");
		goto out_nl_socket_free;
	}

	family = genl_ctrl_resolve(*nls, PON_MCC_FAMILY);
	if (family < 0) {
		fprintf(stderr, "no ponmcc netlink interface found: %i\n",
			family);
		goto out_nl_socket_free;
	}

	*msg = nlmsg_alloc();
	if (!(*msg)) {
		fprintf(stderr, "can not alloc netlink msg\n");
		ret = -NLE_NOMEM;
		goto out_nl_socket_free;
	}

	nl_hdr = genlmsg_put(*msg, 0, 0, family, 0, flags, cmd, 0);
	if (!nl_hdr) {
		fprintf(stderr, "can not generate message\n");
		goto out_nl_socket_msg_free;
	}

	return NLE_SUCCESS;

out_nl_socket_msg_free:
	nlmsg_free(*msg);
out_nl_socket_free:
	nl_socket_free(*nls);
	return ret;
}

static int nl_prepare(struct nl_sock **nls, struct nl_msg **msg, uint8_t cmd)
{
	return prepare(nls, msg, cmd, 0);
}

static int nl_prepare_dump(struct nl_sock **nls, struct nl_msg **msg,
			   uint8_t cmd)
{
	return prepare(nls, msg, cmd, NLM_F_DUMP);
}

/* End of generic functions */

/* Callback functions */

static int cb_nl_valid(struct nl_msg *msg, void *arg)
{
	struct cb_data *cb_data = arg;
	struct nlattr *attrs[PON_MCC_A_MAX + 1];
	int ret;

	ret = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MCC_A_MAX,
			    pon_mcc_genl_policy);
	if (ret < 0) {
		fprintf(stderr, "can not parse netlink message\n");
		cb_data->running = 0;
		cb_data->errorcode = -NLE_INVAL;
		return NL_STOP;
	}

	cb_data->errorcode = NLE_SUCCESS;
	cb_data->running = 0;

	return NL_OK;
}

static int cb_table_entry_read(struct nl_msg *msg, void *arg)
{
	struct cb_data *cb_data = arg;
	struct nlattr *attrs[PON_MCC_A_MAX + 1];
	union mcc_l3addr *grp_addr = NULL;
	union mcc_l3addr *src_addr = NULL;
	uint32_t ifindex;
	enum mcc_l3proto proto;
	enum src_flt_mode flt_mode;
	uint8_t valid;
	char src_buf[INET_ADDRSTRLEN];
	char grp_buf[INET_ADDRSTRLEN];
	char src_buf6[INET6_ADDRSTRLEN];
	char grp_buf6[INET6_ADDRSTRLEN];
	int ret;

	ret = genlmsg_parse(nlmsg_hdr(msg), 0, attrs, PON_MCC_A_MAX,
			    pon_mcc_genl_policy);
	if (ret < 0) {
		fprintf(stderr, "can not parse netlink message\n");
		cb_data->running = 0;
		cb_data->errorcode = -NLE_INVAL;
		return NL_STOP;
	}

	if (attrs[PON_MCC_A_VALID]) {
		valid = nla_get_u8(attrs[PON_MCC_A_VALID]);
	} else {
		fprintf(stdout,
			"cannot get data from the driver - valid not present\n");
		cb_data->running = 0;
		cb_data->errorcode = -NLE_INVAL;
		return NL_STOP;
	}

	if (!valid) {
		fprintf(stdout, "not valid entry\n");
		cb_data->running = 0;
		cb_data->errorcode = -NLE_INVAL;
		return NL_STOP;
	}

	if (!attrs[PON_MCC_A_IFINDEX] || !attrs[PON_MCC_A_PROTO] ||
	    !attrs[PON_MCC_A_FLT_MODE] || !attrs[PON_MCC_A_GRP_ADDR] ||
	    !attrs[PON_MCC_A_SRC_ADDR]) {
		fprintf(stdout, "cannot get data from the driver\n");
		cb_data->running = 0;
		cb_data->errorcode = -NLE_INVAL;
		return NL_STOP;
	}

	ifindex = nla_get_u32(attrs[PON_MCC_A_IFINDEX]);
	proto = nla_get_u8(attrs[PON_MCC_A_PROTO]);
	flt_mode = nla_get_u8(attrs[PON_MCC_A_FLT_MODE]);
	grp_addr = nla_data(attrs[PON_MCC_A_GRP_ADDR]);
	src_addr = nla_data(attrs[PON_MCC_A_SRC_ADDR]);

	fprintf(stdout, "ifindex=%u ", ifindex);
	fprintf(stdout, "ip_version=IPv%u ", proto * 2 + 4);

	if (proto == MCC_L3_PROTO_IPV4) {
		inet_ntop(AF_INET, &src_addr->addr, src_buf,
			  INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &grp_addr->addr, grp_buf,
			  INET_ADDRSTRLEN);
		fprintf(stdout, "grp_addr=%s src_addr=%s ",
			grp_buf, src_buf);
	} else {
		inet_ntop(AF_INET6, &src_addr->addr6, src_buf6,
			  INET6_ADDRSTRLEN);
		inet_ntop(AF_INET6, &grp_addr->addr6, grp_buf6,
			  INET6_ADDRSTRLEN);
		fprintf(stdout, "grp_addr=%s src_addr=%s ",
			grp_buf6, src_buf6);
	}

	fprintf(stdout, "flt_mode=%u valid=%u\n", flt_mode, valid);

	cb_data->errorcode = NLE_SUCCESS;
	cb_data->running = 0;

	return NL_OK;
}

/* End of callback functions */

/* PON MCC API functions */

static int nl_table_entry_modify(uint32_t ifindex, uint8_t proto,
				 const uint8_t *grp_addr,
				 const uint8_t *src_addr, uint8_t flt_mode,
				 uint8_t cmd)
{
	struct nl_msg *msg;
	struct nl_sock *nls;
	struct nl_cb *cb;
	union mcc_l3addr grp;
	union mcc_l3addr src;
	int grp_addr_len = sizeof(union mcc_l3addr);
	int src_addr_len = sizeof(union mcc_l3addr);
	int ret;
	struct cb_data cb_data = {
		.running = 1,
		.errorcode = NLE_SUCCESS,
	};

	ret = nl_prepare(&nls, &msg, cmd);
	if (ret)
		return ret;

	if (proto != MCC_L3_PROTO_IPV4 && proto != MCC_L3_PROTO_IPV6) {
		fprintf(stderr, "wrong input parameter\n");
		ret = -NLE_INVAL;
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	if (flt_mode != SRC_FLT_MODE_NA &&
	    flt_mode != SRC_FLT_MODE_INCLUDE &&
	    flt_mode != SRC_FLT_MODE_EXCLUDE) {
		fprintf(stderr, "wrong input parameter\n");
		ret = -NLE_INVAL;
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	if (proto == MCC_L3_PROTO_IPV4) {
		if (grp_addr != NULL) {
			memcpy(&grp.addr, grp_addr, PON_MCC_ADDRSZ);

			ret = nla_put(msg, PON_MCC_A_GRP_ADDR, grp_addr_len,
				      &grp);
			if (ret) {
				fprintf(stderr, "can not add nl attribute\n");
				nlmsg_free(msg);
				goto out_nl_socket_free;
			}
		}

		if (src_addr != NULL) {
			memcpy(&src.addr, src_addr, PON_MCC_ADDRSZ);

			ret = nla_put(msg, PON_MCC_A_SRC_ADDR, src_addr_len,
					&src);
			if (ret) {
				fprintf(stderr, "can not add nl attribute\n");
				nlmsg_free(msg);
				goto out_nl_socket_free;
			}
		}
	} else {
		if (grp_addr != NULL) {
			memcpy(&grp.addr6, grp_addr, PON_MCC_ADDR6SZ);

			ret = nla_put(msg, PON_MCC_A_GRP_ADDR, grp_addr_len,
				      &grp);
			if (ret) {
				fprintf(stderr, "can not add nl attribute\n");
				nlmsg_free(msg);
				goto out_nl_socket_free;
			}
		}

		if (src_addr != NULL) {
			memcpy(&src.addr6, src_addr, PON_MCC_ADDR6SZ);

			ret = nla_put(msg, PON_MCC_A_SRC_ADDR, src_addr_len,
					&src);
			if (ret) {
				fprintf(stderr, "can not add nl attribute\n");
				nlmsg_free(msg);
				goto out_nl_socket_free;
			}
		}
	}

	ret = nla_put_u32(msg, PON_MCC_A_IFINDEX, ifindex);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nla_put_u8(msg, PON_MCC_A_PROTO, proto);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nla_put_u8(msg, PON_MCC_A_FLT_MODE, flt_mode);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0) {
		fprintf(stderr, "can not send netlink msg: %i\n", ret);
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	nlmsg_free(msg);
	cb = nl_cb_alloc(NL_CB_DEFAULT);
	if (!cb) {
		ret = 1;
		goto out_nl_socket_free;
	}
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_nl_valid, &cb_data);

	ret = nl_recvmsgs(nls, cb);
	if (!ret)
		ret = cb_data.errorcode;

	nl_cb_put(cb);

out_nl_socket_free:
	nl_socket_free(nls);
	return ret;
}

int mcl_table_entry_add(uint32_t ifindex, uint8_t proto,
			const uint8_t *grp_addr, const uint8_t *src_addr,
			uint8_t flt_mode)
{
	int ret = nl_table_entry_modify(ifindex, proto, grp_addr, src_addr,
					flt_mode, PON_MCC_C_TABLE_ENTRY_ADD);

	return nl_nlerr2syserr(ret);
}

int mcl_table_entry_del(uint32_t ifindex, uint8_t proto,
			const uint8_t *grp_addr, const uint8_t *src_addr,
			uint8_t flt_mode)
{
	int ret = nl_table_entry_modify(ifindex, proto, grp_addr, src_addr,
					flt_mode, PON_MCC_C_TABLE_ENTRY_DELETE);

	/* Do not report an error in case the entry was not found */
	ret = (ret == -NLE_OBJ_NOTFOUND) ? 0 : ret;

	return nl_nlerr2syserr(ret);
}

static int nl_table_entry_read(uint8_t initial)
{
	struct nl_msg *msg;
	struct nl_sock *nls;
	struct nl_cb *cb;
	int ret;
	struct cb_data cb_data = {
		.running = 1,
		.errorcode = NLE_SUCCESS,
	};

	if (initial != 0 && initial != 1) {
		fprintf(stderr, "wrong input parameter\n");
		return -NLE_INVAL;
	}

	ret = nl_prepare_dump(&nls, &msg, PON_MCC_C_TABLE_ENTRY_READ);
	if (ret)
		return ret;

	ret = nla_put_u8(msg, PON_MCC_A_INITIAL, initial);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0) {
		fprintf(stderr, "can not send netlink msg: %i\n", ret);
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	nlmsg_free(msg);
	cb = nl_cb_alloc(NL_CB_DEFAULT);
	if (!cb) {
		ret = 1;
		goto out_nl_socket_free;
	}
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_table_entry_read, &cb_data);

	while (cb_data.running == 1) {
		ret = nl_recvmsgs(nls, cb);
		if (ret)
			break;
	}

	if (!ret)
		ret = cb_data.errorcode;

	nl_cb_put(cb);

out_nl_socket_free:
	nl_socket_free(nls);
	return ret;
}

int mcl_table_entry_read(uint8_t initial)
{
	return nl_nlerr2syserr(nl_table_entry_read(initial));
}

static int nl_multicast_enable(uint8_t igmp_version, uint32_t mc_gem_ifindex)
{
	struct nl_msg *msg;
	struct nl_sock *nls;
	struct nl_cb *cb;
	int ret;
	struct cb_data cb_data = {
		.running = 1,
		.errorcode = NLE_SUCCESS,
	};

	ret = nl_prepare(&nls, &msg, PON_MCC_C_MULTICAST_ENABLE);
	if (ret)
		return ret;

	if (igmp_version != IGMP_V2 && igmp_version != IGMP_V3 &&
	    igmp_version != MLD_V1 && igmp_version != MLD_V2) {
		fprintf(stderr, "wrong input parameter: igmp_version = %u\n",
			igmp_version);
		ret = -NLE_INVAL;
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nla_put_u8(msg, PON_MCC_A_IGMP_VERSION, igmp_version);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nla_put_u32(msg, PON_MCC_A_MC_GEM_IFINDEX, mc_gem_ifindex);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0) {
		fprintf(stderr, "can not send netlink msg: %i\n", ret);
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	nlmsg_free(msg);
	cb = nl_cb_alloc(NL_CB_DEFAULT);
	if (!cb) {
		ret = 1;
		goto out_nl_socket_free;
	}
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_nl_valid, &cb_data);

	ret = nl_recvmsgs(nls, cb);
	if (!ret)
		ret = cb_data.errorcode;

	nl_cb_put(cb);

out_nl_socket_free:
	nl_socket_free(nls);
	return ret;
}

int mcl_multicast_enable(uint8_t igmp_version, uint32_t mc_gem_ifindex)
{
	return nl_nlerr2syserr(
	    nl_multicast_enable(igmp_version, mc_gem_ifindex));
}

static int nl_multicast_disable(void)
{
	struct nl_msg *msg;
	struct nl_sock *nls;
	struct nl_cb *cb;
	int ret;
	struct cb_data cb_data = {
		.running = 1,
		.errorcode = NLE_SUCCESS,
	};

	ret = nl_prepare(&nls, &msg, PON_MCC_C_MULTICAST_DISABLE);
	if (ret)
		return ret;

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0) {
		fprintf(stderr, "can not send netlink msg: %i\n", ret);
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	nlmsg_free(msg);
	cb = nl_cb_alloc(NL_CB_DEFAULT);
	if (!cb) {
		ret = 1;
		goto out_nl_socket_free;
	}
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_nl_valid, &cb_data);

	ret = nl_recvmsgs(nls, cb);
	if (!ret)
		ret = cb_data.errorcode;

	nl_cb_put(cb);

out_nl_socket_free:
	nl_socket_free(nls);
	return ret;
}

int mcl_multicast_disable(void)
{
	return nl_nlerr2syserr(nl_multicast_disable());
}

/* End of PON MCC API functions */

/* PON MCC API debug functions - currently only dummy implementation prepared
 * for future usage if needed.
 */
int mcl_snoop_cfg_set(uint8_t igmp_mode, uint8_t is_igmpv3, uint8_t cross_vlan,
		      uint8_t fwd_port, uint8_t fwd_port_id, uint8_t cos,
		      uint8_t robust, uint8_t query_ival,
		      uint8_t suppression_aggregation, uint8_t fast_leave,
		      uint8_t learning_router, uint8_t mc_unk_drop,
		      uint8_t mc_fid_mode)
{
	UNUSED(igmp_mode);
	UNUSED(is_igmpv3);
	UNUSED(cross_vlan);
	UNUSED(fwd_port);
	UNUSED(fwd_port_id);
	UNUSED(cos);
	UNUSED(robust);
	UNUSED(query_ival);
	UNUSED(suppression_aggregation);
	UNUSED(fast_leave);
	UNUSED(learning_router);
	UNUSED(mc_unk_drop);
	UNUSED(mc_fid_mode);

	fprintf(stdout, "%s: not implemented yet\n", __func__);

	return 0;
}

int mcl_snoop_cfg_get(void)
{
	fprintf(stdout, "%s: not implemented yet\n", __func__);

	return 0;
}

int mcl_bridge_port_cfg_set(uint32_t bridge_port_id,
			    uint8_t mc_dest_ip_lookup_dis,
			    uint8_t mc_src_ip_lookup_en)
{
	UNUSED(bridge_port_id);
	UNUSED(mc_dest_ip_lookup_dis);
	UNUSED(mc_src_ip_lookup_en);

	fprintf(stdout, "%s: not implemented yet\n", __func__);

	return 0;
}

int mcl_bridge_port_cfg_get(uint32_t bridge_port_id)
{
	UNUSED(bridge_port_id);

	fprintf(stdout, "%s: not implemented yet\n", __func__);

	return 0;
}

int mcl_bridge_cfg_set(uint32_t bridge_id, uint8_t fwd_unk_mc_non_ip,
		       uint8_t fwd_unk_mc_ip)
{
	UNUSED(bridge_id);
	UNUSED(fwd_unk_mc_non_ip);
	UNUSED(fwd_unk_mc_ip);

	fprintf(stdout, "%s: not implemented yet\n", __func__);

	return 0;
}

int mcl_bridge_cfg_get(uint32_t bridge_id)
{
	UNUSED(bridge_id);

	fprintf(stdout, "%s: not implemented yet\n", __func__);

	return 0;
}

static int nl_cpu_fwd_set(uint8_t bridge_port_id, uint8_t cmd)
{
	struct nl_msg *msg;
	struct nl_sock *nls;
	struct nl_cb *cb;
	int ret;
	struct cb_data cb_data = {
		.running = 1,
		.errorcode = NLE_SUCCESS,
	};

	ret = nl_prepare(&nls, &msg, cmd);
	if (ret)
		return ret;

	ret = nla_put_u8(msg, PON_MCC_A_BRIDGE_PORT_ID, bridge_port_id);
	if (ret) {
		fprintf(stderr, "can not add nl attribute\n");
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	ret = nl_send_auto_complete(nls, msg);
	if (ret < 0) {
		fprintf(stderr, "can not send netlink msg: %i\n", ret);
		nlmsg_free(msg);
		goto out_nl_socket_free;
	}

	nlmsg_free(msg);
	cb = nl_cb_alloc(NL_CB_DEFAULT);
	if (!cb) {
		ret = 1;
		goto out_nl_socket_free;
	}
	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, cb_nl_valid, &cb_data);

	ret = nl_recvmsgs(nls, cb);
	if (!ret)
		ret = cb_data.errorcode;

	nl_cb_put(cb);

out_nl_socket_free:
	nl_socket_free(nls);
	return ret;
}

int mcl_cpu_fwd_enable(uint8_t bridge_port_id)
{
	return nl_nlerr2syserr(
	    nl_cpu_fwd_set(bridge_port_id, PON_MCC_C_CPU_FWD_ON));
}

int mcl_cpu_fwd_disable(uint8_t bridge_port_id)
{
	return nl_nlerr2syserr(
	    nl_cpu_fwd_set(bridge_port_id, PON_MCC_C_CPU_FWD_OFF));
}

/* End of PON MCC API debug functions */
