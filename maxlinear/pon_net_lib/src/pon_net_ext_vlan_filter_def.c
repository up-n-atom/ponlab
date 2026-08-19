/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * Overview:
 *
 * This file contains code responsible for translating OMCI rules into
 * multiple TC filters. This usually involves matching provided OMCI rule
 * against internal table and using corresponding filter generation template
 * to produce multiple TC filters needed to realize given OMCI rule
 */

#include <stdlib.h>
#ifdef LINUX
#include <arpa/inet.h>
#endif

#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink.h"
#include "pon_net_ext_vlan_filter_def.h"
#include "pon_net_ext_vlan_table.h"

/* This is a special ethertype value that means:
   Copy ethertype from input TPID */
#define ETH_INPUT_TPID -2

/* This is a special ethertype value that means:
   Copy ethertype from output TPID */
#define ETH_OUTPUT_TPID -3

/* This is a special ethertype value that means:
   Copy ethertype from inner tag of received frame */
#define ETH_COPY_INNER -4

/* This is a special ethertype value that means:
   Copy ethertype from outer tag of received frame */
#define ETH_COPY_OUTER -5

#define EVAL(x) x
#define DBG(level, arg) EVAL(dbg_prn arg)
#define OMCI_API_ERR

/* One filter_ethertype value can map up to 2 ETH_P_* values
   (e.g. 2 maps to ETH_P_PPP_DISC and ETH_P_PPP_SES) */
struct ethertype {
	/* To how many ethertypes filter_ethertype is mapped */
	unsigned int count;
	/* List of ethertypes */
	int ethertypes[2];
};

/* 0 Do not filter on Ethertype. (NETLINK_FILTER_UNUSED)
   1 Ethertype = 0x0800 (filter IPoE frames) (ETH_P_IP)
   2 Ethertype = 0x8863 or 0x8864 (filter PPPoE frames)
   (ETH_P_PPP_DISC or ETH_P_PPP_SES)
   3 Ethertype = 0x0806 (filter ARP frames) (ETH_P_ARP)
   4 Ethertype = 0x86DD (filter IPv6 IpoE frames) (ETH_P_IPV6)
   5 Ethertype = 0x888E (filter EAPOL frames) (ETH_P_PAE) */
static const struct ethertype ethertype_list[] = {
	[0] = { .count = 1, .ethertypes = {NETLINK_FILTER_UNUSED, 0} },
	[1] = { .count = 1, .ethertypes = {ETH_P_IP, 0} },
	[2] = { .count = 2, .ethertypes = {ETH_P_PPP_DISC, ETH_P_PPP_SES} },
	[3] = { .count = 1, .ethertypes = {ETH_P_ARP, 0} },
	[4] = { .count = 1, .ethertypes = {ETH_P_IPV6, 0} },
	[5] = { .count = 1, .ethertypes = {ETH_P_PAE, 0} },
};

/* Filter inner/outer TPID/DEI: (3 bits)
   - 000 Do not filter on inner TPID field.
   - 100 Inner TPID = 0x8100
   - 101 Inner TPID = input TPID attribute value, "don't care" about DEI bit
   - 110 Inner TPID = input TPID, DEI = 0
   - 111 Inner TPID = input TPID, DEI = 1 */
static const struct ethertype tpid_ethertypes[] = {
	[0] = { .count = 2, .ethertypes = {ETH_P_8021Q, ETH_P_8021AD} },
	[1] = { .count = 1, .ethertypes = {NETLINK_FILTER_UNUSED, 0} },
	[2] = { .count = 1, .ethertypes = {NETLINK_FILTER_UNUSED, 0} },
	[3] = { .count = 1, .ethertypes = {NETLINK_FILTER_UNUSED, 0} },
	[4] = { .count = 1, .ethertypes = {ETH_P_8021Q, 0} },
	[5] = { .count = 1, .ethertypes = {ETH_INPUT_TPID, 0} },
	[6] = { .count = 1, .ethertypes = {ETH_INPUT_TPID, 0} },
	[7] = { .count = 1, .ethertypes = {ETH_INPUT_TPID, 0} },
};

/* Treatment outer TPID/DEI: (3 bits)
   - 000 Copy TPID (and DEI, if present)
	 from the inner tag of the received frame
   - 001 Copy TPID (and DEI, if present)
	 from the outer tag of the received frame
   - 010 Set TPID = output TPID attribute value, copy DEI bit
	 from the inner tag of the received frame
   - 011 Set TPID = output TPID, copy DEI
	 from the outer tag of the received frame
   - 100 Set TPID = 0x8100
   - 101 Reserved
   - 110 Set TPID = output TPID, DEI = 0
   - 111 Set TPID = output TPID, DEI = 1 */
static const struct ethertype treatment_tpid_ethertypes[] = {
	[0] = { .count = 1, .ethertypes = {ETH_COPY_INNER, 0} },
	[1] = { .count = 1, .ethertypes = {ETH_COPY_OUTER, 0} },
	[2] = { .count = 1, .ethertypes = {ETH_OUTPUT_TPID, 0} },
	[3] = { .count = 1, .ethertypes = {ETH_OUTPUT_TPID, 0} },
	[4] = { .count = 1, .ethertypes = {ETH_P_8021Q, 0} },
	/* 5 is reserved - but we are using this table also with
	   filter tpid values (in case of downstream rules) */
	[5] = { .count = 1, .ethertypes = {ETH_OUTPUT_TPID, 0} },
	[6] = { .count = 1, .ethertypes = {ETH_OUTPUT_TPID, 0} },
	[7] = { .count = 1, .ethertypes = {ETH_OUTPUT_TPID, 0} },
};

static const struct ethertype no_ethertype = {
	.count = 1,
	.ethertypes = {NETLINK_FILTER_UNUSED, 0},
};

/* Translate OMCI TPID value into ethertype value that
   can be used with TC flower filters */
static const struct ethertype
*find_ethertype(int omci_tpid,
		const struct ethertype *ethertypes,
		int size)
{
	dbg_in_args("%u", omci_tpid);

	if (omci_tpid < size) {
		dbg_out_ret("%p", &ethertypes[omci_tpid]);
		return &ethertypes[omci_tpid];
	}

	dbg_out_ret("%p", &ethertypes[0]);
	return &ethertypes[0];
}

/*
 * Get a field from struct pon_adapter_ext_vlan_filter specified by constant.
 * It is needed for \ref ext_vlan_rules table, where these constants define
 * which field from struct pon_adapter_ext_vlan_filter to get from rule template
 */
static int map_val(const struct pon_adapter_ext_vlan_filter *filter, int var)
{
	switch (var) {
	case fop:
		return filter->filter_outer_priority;
	case fov:
		return filter->filter_outer_vid;
	case fot:
		return filter->filter_outer_tpid_de;
	case fip:
		return filter->filter_inner_priority;
	case fiv:
		return filter->filter_inner_vid;
	case fit:
		return filter->filter_inner_tpid_de;
	case top:
		return filter->treatment_outer_priority;
	case tov:
		return filter->treatment_outer_vid;
	case tot:
		return filter->treatment_outer_tpid_de;
	case tip:
		return filter->treatment_inner_priority;
	case tiv:
		return filter->treatment_inner_vid;
	case tit:
		return filter->treatment_inner_tpid_de;
	case fe:
		return filter->filter_ethertype;
	case tr:
		return filter->treatment_tags_to_remove;
	}

	return var;
}

static const struct ethertype
*find_ethertype_by_special_value(int value,
				 const struct pon_adapter_ext_vlan_filter *f)
{
	switch (value) {
	case tit:
	case tot:
		return find_ethertype(map_val(f, value),
				      treatment_tpid_ethertypes,
				      ARRAY_SIZE(treatment_tpid_ethertypes));
	case fit:
	case fot:
	default:
		return find_ethertype(map_val(f, value),
				      tpid_ethertypes,
				      ARRAY_SIZE(tpid_ethertypes));
	}

	return &no_ethertype;
}

/*
 * Find ethertype value for field from filtering part of struct filter_template
 * The "value" should be struct filter_template->inner_vlan_proto or
 * struct filter_template->outer_vlan_proto.
 */
static const struct ethertype
*find_ethertype_for_filter_field(int value,
				 const struct pon_adapter_ext_vlan_filter *f)
{
	const struct ethertype *ethertype = &no_ethertype;

	ethertype = find_ethertype_by_special_value(value, f);

	if (ethertype->ethertypes[0] == ETH_COPY_INNER) {
		value = f->filter_inner_tpid_de;
		ethertype = find_ethertype(value,
					  tpid_ethertypes,
					  ARRAY_SIZE(tpid_ethertypes));
	} else if (ethertype->ethertypes[0] == ETH_COPY_OUTER) {
		value = f->filter_outer_tpid_de;
		ethertype = find_ethertype(value,
					  tpid_ethertypes,
					  ARRAY_SIZE(tpid_ethertypes));
	}

	return ethertype;
}

/*
 * Find ethertype value for field from treatment part of struct filter_template
 * The "value" should be struct filter_template->act_vlan_proto
 */
static const struct ethertype
*find_ethertype_for_treatment_field(int value,
				 const struct pon_adapter_ext_vlan_filter *f)
{
	const struct ethertype *ethertype;

	ethertype = find_ethertype_by_special_value(value, f);
	if (ethertype->count <= 1)
		return ethertype;

	return find_ethertype(0, treatment_tpid_ethertypes,
			      ARRAY_SIZE(treatment_tpid_ethertypes));
}

/* Return number of items in the struct filter_template list */
static unsigned int filter_template_count(const struct filter_template *def)
{
	unsigned int count = 0;
	const struct filter_template *p;

	dbg_in_args("%p", def);

	for (p = def; p != NULL; p = p->next)
		count++;

	dbg_out_ret("%u", count);
	return count;
}

/*
 * Convert a list of struct filter_template filter definitions into an array
 * of struct netlink_filter_definition. Netlink filter definition contains
 * struct netlink_filter, which can be later added to the system using
 * netlink_filter_add()
 */
static enum pon_adapter_errno
filter_template_to_netlink_filter_defs(const struct filter_template *def,
				       struct netlink_filter_definition **out,
				       unsigned int *num)
{
	struct netlink_filter *filter;
	struct netlink_vlan_data *vlan_data;
	const struct filter_template *p;
	int j = 0;
	unsigned int count = 0;

	dbg_in_args("%p", def);

	count = filter_template_count(def);

	*out = calloc(count, sizeof(**out));
	if (!*out) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}

	for (p = def; p != NULL; p = p->next, j++) {
		filter = &(*out)[j].filter;

		netlink_filter_defaults(filter);

		if (p->outer_vlan_enable && p->inner_vlan_enable) {
			filter->proto = p->outer_vlan_proto;
			filter->vlan_id = p->outer_vlan_id;
			filter->vlan_prio = p->outer_vlan_prio;

			filter->vlan_proto = p->inner_vlan_proto;
			filter->cvlan_id = p->inner_vlan_id;
			filter->cvlan_prio = p->inner_vlan_prio;

			filter->cvlan_proto = p->proto;
		} else if (p->inner_vlan_enable) {
			filter->proto = p->inner_vlan_proto;
			filter->vlan_id = p->inner_vlan_id;
			filter->vlan_prio = p->inner_vlan_prio;

			filter->vlan_proto = p->proto;
		} else {
			filter->proto = p->proto;
		}

		filter->act = p->act;
		filter->ip_tos = p->ip_tos;

		vlan_data = &(*out)[j].vlan_data;

		netlink_vlan_defaults(vlan_data);

		vlan_data->vlan_id = p->act_vlan_id;
		vlan_data->vlan_prio = p->act_vlan_prio;
		vlan_data->vlan_proto = p->act_vlan_proto;
		vlan_data->act_vlan = p->act_vlan;
		if (p->act_vlan == NETLINK_FILTER_ACT_VLAN_MODIFY_AND_PUSH) {
			vlan_data->svlan_id = p->act_svlan_id;
			vlan_data->svlan_prio = p->act_svlan_prio;
			vlan_data->svlan_eth_type = p->act_svlan_proto;
		}
		filter->act_data = vlan_data;
	}

	*num = count;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

}

/* Sets act data pointer on filter definition to correct act data */
static void
filter_definition_act_data_pointer_set(struct netlink_filter_definition *def)
{
	dbg_in_args("%p", def);

	switch (def->filter.act) {
	case NETLINK_FILTER_ACT_VLAN:
		def->filter.act_data = &def->vlan_data;
		break;
	case NETLINK_FILTER_ACT_OK:
	case NETLINK_FILTER_ACT_DROP:
		def->filter.act_data = &def->vlan_data.cookie;
		break;
	default:
		def->filter.act_data = NULL;
		break;
	}

	dbg_out();
}

/*
 * Given a "def" pointer to struct filter_template list, the function
 * will duplicate it and insert a copy as the next item
 */
static enum pon_adapter_errno
filter_template_duplicate(struct filter_template *def)
{
	struct filter_template *copy;

	dbg_in_args("%p", def);

	copy = malloc(sizeof(*def));
	if (!copy) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}
	if (memcpy_s(copy, sizeof(*copy), def, sizeof(*def))) {
		dbg_err_fn(memcpy_s);
		free(copy);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	def->next = copy;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Given a "def" pointer to struct filter_template list, the function
 * will make n - 1 copies inserting them a as next items.
 *
 * filter_template_multiplicate(def, 1) will not produce any new filters
 * filter_template_multiplicate(def, 2) will not produce one additional filter
 * filter_template_multiplicate(def, 3) will not produce two additional filters
 */
static enum pon_adapter_errno
filter_template_multiplicate(struct filter_template *def, unsigned int n)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i;
	unsigned int j;

	struct filter_template *tmp;

	dbg_in_args("%p, %u", def, n);

	if (!n)
		goto exit;

	for (i = 0; i < n - 1; ++i) {
		ret = filter_template_duplicate(def);
		if (ret != PON_ADAPTER_SUCCESS) {
			/* clean up already created duplicates */
			for (j = 0; j < i; ++j) {
				if (!def->next)
					break;
				tmp = def->next;
				def->next = def->next->next;
				free(tmp);
			}
			goto exit;
		}
	}

exit:
	dbg_out_ret("%d", ret);
	return ret;
}

static void filter_template_destroy(struct filter_template *def)
{
	struct filter_template *next;
	struct filter_template *p;

	dbg_in_args("%p", def);

	for (p = def; p != NULL; p = next) {
		next = p->next;
		free(p);
	}

	dbg_out();
}

struct expand_args {
	/* Ext. Vlan ME Input TPID */
	int input_tpid;
	/* Ext. Vlan ME Output TPID */
	int output_tpid;
	/* 1 if downstream rule, 0 if upstream */
	int ds;
	/* Downstream mode */
	uint16_t ds_mode;
	const struct pon_adapter_ext_vlan_filter *filter;
	uint8_t *dscp;
	bool ignore_ds_rules_prio;
};

static int
default_act_tpid_get(const struct expand_args *args)
{
	int default_act_tpid;

	dbg_in_args("%p", args);

	if (args->ds) {
		default_act_tpid = args->input_tpid;
		if (!args->input_tpid)
			dbg_wrn("Input TPID is 0\n");
	} else {
		default_act_tpid = args->output_tpid;
		if (!args->output_tpid)
			dbg_wrn("Output TPID is 0\n");
	}

	if (!default_act_tpid) {
		dbg_wrn("Assuming 802.1q\n");
		default_act_tpid = ETH_P_8021Q;
	}

	dbg_out_ret("%u", default_act_tpid);
	return default_act_tpid;
}

typedef enum pon_adapter_errno (*expand_fn)(struct filter_template *def,
					    void *arg);

/*
 * Expand one struct filter_template into several filters. OMCI rules often
 * can't be represented with a single TC filter. For example: an OMCI rule
 * to match all double tagged packets requires at least 4 filters to match
 * all combinations of VLAN ethertypes:
 *
 *     802.1q 802.1ad
 *     802.1ad 802.1q
 *     802.1ad 802.1ad
 *     802.1q 802.1q
 *
 * Expand uses an array of function pointers, each of which performs some
 * form of expansion or processing on struct filter_template list. The
 * struct filter_template will be multiplicated using
 * filter_template_multiplicate() in the process
 */
static enum pon_adapter_errno
filter_template_expand(struct filter_template *def,
		       const expand_fn *expanders,
		       unsigned int num_expanders,
		       void *arg)
{
	enum pon_adapter_errno ret;
	unsigned int i = 0;
	struct filter_template *j;
	struct filter_template *next;
	expand_fn expander;

	dbg_in_args("%p, %p, %u, %p", def, expanders, num_expanders, arg);

	for (i = 0; i < num_expanders; ++i) {
		expander = expanders[i];
		for (j = def; j != NULL; j = next) {
			next = j->next;

			ret = expander(j, arg);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, expander, ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tpid_expand(struct filter_template *def,
	    void *arg,
	    unsigned int offset)
{
	unsigned int i = 0;
	const struct expand_args *expand_args = arg;
	const struct ethertype *ethtype;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int *field;

	dbg_in_args("%p, %p, %u", def, arg, offset);

	field = (int *)(((unsigned char *)def) + offset);

	if (*field == NETLINK_FILTER_UNUSED) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ethtype = find_ethertype_for_filter_field(*field, expand_args->filter);

	ret = filter_template_multiplicate(def, ethtype->count);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, filter_template_multiplicate, ret);
		return ret;
	}

	for (i = 0; i < ethtype->count; ++i) {
		field = (int *)(((unsigned char *)def) + offset);
		switch (ethtype->ethertypes[i]) {
		case ETH_INPUT_TPID:
			*field = expand_args->input_tpid;
			break;
		case ETH_OUTPUT_TPID:
			*field = expand_args->output_tpid;
			break;
		default:
			*field = ethtype->ethertypes[i];
			break;
		}
		def = def->next;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
inner_vlan_proto_expand(struct filter_template *def, void *arg)
{
	if (!def->inner_vlan_enable)
		return PON_ADAPTER_SUCCESS;

	return tpid_expand(def, arg,
			   offsetof(struct filter_template, inner_vlan_proto));
}

static enum pon_adapter_errno
outer_vlan_proto_expand(struct filter_template *def, void *arg)
{
	if (!def->outer_vlan_enable)
		return PON_ADAPTER_SUCCESS;

	return tpid_expand(def, arg,
			   offsetof(struct filter_template, outer_vlan_proto));
}

static enum pon_adapter_errno
proto_expand(struct filter_template *def, void *arg)
{
	unsigned int i = 0;
	const struct expand_args *expand_args = arg;
	const struct ethertype *ethtype;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", def, arg);

	if (def->proto == NETLINK_FILTER_UNUSED) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ethtype = find_ethertype(map_val(expand_args->filter, def->proto),
				 ethertype_list,
				 ARRAY_SIZE(ethertype_list));

	ret = filter_template_multiplicate(def, ethtype->count);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, filter_template_multiplicate, ret);
		return ret;
	}

	for (i = 0; i < ethtype->count; ++i) {
		switch (ethtype->ethertypes[i]) {
		case ETH_INPUT_TPID:
			def->proto = expand_args->input_tpid;
			break;
		case ETH_OUTPUT_TPID:
			def->proto = expand_args->output_tpid;
			break;
		default:
			def->proto = ethtype->ethertypes[i];
			break;
		}
		def = def->next;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* This function must be executed after proto_expand(), because
 * it expects outer_vlan_proto and inner_vlan_proto to be already expanded */
static enum pon_adapter_errno
act_vlan_proto_expand(struct filter_template *def, void *arg)
{
	const struct expand_args *expand_args = arg;
	const struct ethertype *ethtype;

	dbg_in_args("%p, %p", def, arg);

	if (def->act_vlan_proto == NETLINK_FILTER_UNUSED) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ethtype = find_ethertype_for_treatment_field(def->act_vlan_proto,
						     expand_args->filter);
	switch (ethtype->ethertypes[0]) {
	case ETH_INPUT_TPID:
		def->act_vlan_proto = expand_args->input_tpid;
		break;
	case ETH_OUTPUT_TPID:
		def->act_vlan_proto = expand_args->output_tpid;
		break;
	case ETH_COPY_INNER:
		if (def->act_vlan_proto == fot) {
			def->act_vlan_proto = def->outer_vlan_proto;
			break;
		}
		def->act_vlan_proto = def->inner_vlan_proto;
		break;
	case ETH_COPY_OUTER:
		if (def->act_vlan_proto == fit) {
			def->act_vlan_proto = def->inner_vlan_proto;
			break;
		}

		def->act_vlan_proto = def->outer_vlan_proto;
		break;
	default:
		def->act_vlan_proto = ethtype->ethertypes[0];
		break;
	}

	ethtype = find_ethertype_for_treatment_field(def->act_svlan_proto,
						     expand_args->filter);
	switch (ethtype->ethertypes[0]) {
	case ETH_INPUT_TPID:
		def->act_svlan_proto = expand_args->input_tpid;
		break;
	case ETH_OUTPUT_TPID:
		def->act_svlan_proto = expand_args->output_tpid;
		break;
	case ETH_COPY_INNER:
		if (def->act_svlan_proto == fot) {
			def->act_svlan_proto = def->outer_vlan_proto;
			break;
		}
		def->act_svlan_proto = def->inner_vlan_proto;
		break;
	case ETH_COPY_OUTER:
		if (def->act_svlan_proto == fit) {
			def->act_svlan_proto = def->inner_vlan_proto;
			break;
		}

		def->act_svlan_proto = def->outer_vlan_proto;
		break;
	default:
		def->act_svlan_proto = ethtype->ethertypes[0];
		break;
	}

	/* Sometimes def->act_vlan_proto can be 0.
	 * For example if it resolved to ETH_COPY_INNER or
	 * ETH_COPY_OUTER and def->inner_vlan_proto or
	 * def->outer_vlan_proto was 0, then we will get
	 * invalid value. We use expand_args->default_act_tpid to
	 * fix this case */
	if (!def->act_vlan_proto)
		def->act_vlan_proto = default_act_tpid_get(expand_args);
	if (!def->act_svlan_proto)
		def->act_svlan_proto = default_act_tpid_get(expand_args);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
priority_expand(struct filter_template *def, void *arg)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct expand_args *expand_args = arg;
	int i = 0;

	dbg_in_args("%p, %p", def, arg);

	def->inner_vlan_prio = map_val(expand_args->filter,
				       def->inner_vlan_prio);
	def->outer_vlan_prio = map_val(expand_args->filter,
				       def->outer_vlan_prio);
	def->act_vlan_prio = map_val(expand_args->filter, def->act_vlan_prio);
	def->act_svlan_prio = map_val(expand_args->filter, def->act_svlan_prio);

	if (def->inner_vlan_prio == PRIO_ANY)
		def->inner_vlan_prio = NETLINK_FILTER_UNUSED;

	if (def->outer_vlan_prio == PRIO_ANY)
		def->outer_vlan_prio = NETLINK_FILTER_UNUSED;

	if (def->act_vlan_prio == PRIO_DSCP) {
		ret = filter_template_multiplicate(def, DSCP_MAX);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, filter_template_multiplicate, ret);
			return ret;
		}

		for (i = 0; i < DSCP_MAX; ++i) {
			/* Last 2 bits of ip_tos are reserved for ECN */
			def->ip_tos = i << 2;
			def->act_vlan_prio = expand_args->dscp[i];
			def = def->next;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if ((def->act_vlan_prio == PRIO_COPY_INNER ||
	     def->act_svlan_prio == PRIO_COPY_INNER) &&
	     def->inner_vlan_prio == NETLINK_FILTER_UNUSED) {

		ret = filter_template_multiplicate(def, PRIO_COUNT);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, filter_template_multiplicate, ret);
			return ret;
		}

		for (i = 0; i < PRIO_COUNT; ++i) {
			def->inner_vlan_prio = i;
			if (def->act_vlan_prio == PRIO_COPY_INNER)
				def->act_vlan_prio = i;
			if (def->act_svlan_prio == PRIO_COPY_INNER)
				def->act_svlan_prio = i;
			def->act_vlan_prio = i;
			def = def->next;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (def->act_vlan_prio == PRIO_COPY_OUTER &&
	    def->outer_vlan_prio == NETLINK_FILTER_UNUSED
	    && (def->act_vlan == NETLINK_FILTER_ACT_VLAN_PUSH ||
		def->act_vlan == NETLINK_FILTER_ACT_VLAN_POP_AND_MODIFY)) {

		ret = filter_template_multiplicate(def, PRIO_COUNT);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, filter_template_multiplicate, ret);
			return ret;
		}

		for (i = 0; i < PRIO_COUNT; ++i) {
			def->outer_vlan_prio = i;
			def->act_vlan_prio = i;
			def = def->next;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (def->act_vlan_prio == PRIO_COPY_INNER &&
	    def->inner_vlan_prio == NETLINK_FILTER_UNUSED
	    && (def->act_vlan == NETLINK_FILTER_ACT_VLAN_PUSH ||
		def->act_vlan == NETLINK_FILTER_ACT_VLAN_POP_AND_MODIFY)) {
		ret = filter_template_multiplicate(def, PRIO_COUNT);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, filter_template_multiplicate, ret);
			return ret;
		}

		for (i = 0; i < PRIO_COUNT; ++i) {
			def->inner_vlan_prio = i;
			def->act_vlan_prio = i;
			def = def->next;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (def->act_vlan_prio == PRIO_COPY_OUTER &&
	    def->act_vlan == NETLINK_FILTER_ACT_VLAN_MODIFY) {
		def->act_vlan_prio = NETLINK_FILTER_UNUSED;

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (def->act_vlan_prio == PRIO_COPY_INNER) {
		def->act_vlan_prio = def->inner_vlan_prio;

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (def->act_vlan_prio == PRIO_COPY_OUTER) {
		def->act_vlan_prio = def->outer_vlan_prio;

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
generic_expand(struct filter_template *def, void *arg)
{
	const struct expand_args *expand_args = arg;
	const struct pon_adapter_ext_vlan_filter *filter = expand_args->filter;

	dbg_in_args("%p, %p", def, arg);

	def->outer_vlan_id = map_val(filter, def->outer_vlan_id);
	def->outer_vlan_prio = map_val(filter, def->outer_vlan_prio);
	def->outer_vlan_proto = map_val(filter, def->outer_vlan_proto);
	def->inner_vlan_id = map_val(filter, def->inner_vlan_id);
	def->inner_vlan_prio = map_val(filter, def->inner_vlan_prio);
	def->inner_vlan_proto = map_val(filter, def->inner_vlan_proto);
	def->proto = map_val(filter, def->proto);
	def->act = map_val(filter, def->act);
	def->act_vlan_id = map_val(filter, def->act_vlan_id);
	def->act_vlan_prio = map_val(filter, def->act_vlan_prio);
	def->act_vlan_proto = map_val(filter, def->act_vlan_proto);
	def->act_vlan = map_val(filter, def->act_vlan);
	def->act_svlan_id = map_val(filter, def->act_svlan_id);
	def->act_svlan_prio = map_val(filter, def->act_svlan_prio);
	def->act_svlan_proto = map_val(filter, def->act_svlan_proto);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
dscp_protocol_all_expand_to_ip_and_ipv6(struct filter_template *def, void *arg)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", def, arg);

	if (def->ip_tos != NETLINK_FILTER_UNUSED &&
	    (def->proto == ETH_P_ALL ||
	    def->proto == NETLINK_FILTER_UNUSED)) {
		/* create 2 more tc filter templates based on the current one */
		ret = filter_template_multiplicate(def, 2);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, filter_template_multiplicate, ret);
			return ret;
		}
		def->proto = ETH_P_IP;
		def = def->next;
		def->proto = ETH_P_IPV6;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ds_mode_expand(struct filter_template *def, void *arg)
{
	const struct expand_args *expand_args = arg;

	dbg_in_args("%p, %p", def, arg);

	if (!expand_args->ds ||
	    def->act_vlan != NETLINK_FILTER_ACT_VLAN_MODIFY) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	switch (expand_args->ds_mode) {
	case DS_MODE_REVERSE:
	case DS_MODE_VID_PBIT_OR_FORWARD:
	case DS_MODE_VID_PBIT_OR_DISCARD:
		break;
	case DS_MODE_VID_OR_FORWARD:
	case DS_MODE_VID_OR_DISCARD:
		def->outer_vlan_prio = NETLINK_FILTER_UNUSED;
		def->inner_vlan_prio = NETLINK_FILTER_UNUSED;
		def->act_vlan_prio = NETLINK_FILTER_UNUSED;
		break;
	case DS_MODE_PBIT_OR_FORWARD:
	case DS_MODE_PBIT_OR_DISCARD:
		def->outer_vlan_id = NETLINK_FILTER_UNUSED;
		def->inner_vlan_id = NETLINK_FILTER_UNUSED;
		def->act_vlan_id = NETLINK_FILTER_UNUSED;
		break;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ignore_ds_prio_expand(struct filter_template *def, void *arg)
{
	const struct expand_args *expand_args = arg;

	dbg_in_args("%p, %p", def, arg);

	if (expand_args->ds && expand_args->ignore_ds_rules_prio) {
		def->outer_vlan_prio = NETLINK_FILTER_UNUSED;
		def->inner_vlan_prio = NETLINK_FILTER_UNUSED;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const expand_fn expanders[] = {
	ds_mode_expand,
	ignore_ds_prio_expand,
	inner_vlan_proto_expand,
	outer_vlan_proto_expand,
	proto_expand,
	act_vlan_proto_expand,
	priority_expand,
	generic_expand,
	dscp_protocol_all_expand_to_ip_and_ipv6
};

/* Find rule by major and minor number */
static struct rule
*rule_find(int major, int minor, struct rule *rules, unsigned int num_rules)
{
	unsigned int i = 0;

	dbg_in_args("%d, %d, %p, %u", major, minor, rules, num_rules);

	for (i = 0; i < num_rules; ++i) {
		if (rules[i].rule_number.major == major &&
		    rules[i].rule_number.minor == minor) {
			dbg_out_ret("%p", &rules[i]);
			return &rules[i];
		}
	}

	dbg_out_ret("%p", NULL);
	return NULL;
}

unsigned int
pon_net_ext_vlan_filters_get(struct pon_net_ext_vlan *ext_vlan,
			     const struct pon_adapter_ext_vlan_filter *filter,
			     int ds,
			     int rule_major,
			     int rule_minor,
			     struct netlink_filter_definition **out)
{
	struct expand_args args = {0};
	unsigned int num_filter_defs = 0;
	const struct filter_template *tpl;
	struct filter_template *def;
	struct rule *rule;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i = 0;

	dbg_in_args("%p, %p, %d, %d, %d, %p",
		    ext_vlan, filter, ds, rule_major, rule_minor, out);

	args.filter = filter;
	args.input_tpid = ext_vlan->input_tpid;
	args.output_tpid = ext_vlan->output_tpid;
	args.dscp = ext_vlan->dscp;
	args.ds = ds;
	args.ds_mode = ext_vlan->ds_mode;
	args.ignore_ds_rules_prio = ext_vlan->ctx->ignore_ds_rules_prio;

	/* Skip creation of default forwarding rules in downstream direction */
	if (ds && (rule_minor == 1)) {
		dbg_out_ret("%d", 0);
		return 0;
	}

	rule = rule_find(rule_major,
			 rule_minor,
			 ext_vlan_rules,
			 ARRAY_SIZE(ext_vlan_rules));
	if (!rule) {
		dbg_out_ret("%d", 0);
		return 0;
	}

	tpl = ds ? &rule->downstream : &rule->upstream;
	if (tpl->unused) {
		dbg_out_ret("%d", 0);
		return 0;
	}

	def = malloc(sizeof(*tpl));
	if (!def) {
		dbg_err_fn(malloc);
		dbg_out_ret("%d", 0);
		return 0;
	}
	if (memcpy_s(def, sizeof(*def), tpl, sizeof(*tpl))) {
		dbg_err_fn(memcpy_s);
		free(def);
		dbg_out_ret("%d", 0);
		return 0;
	}

	/* ip_tos will be set by priority_expand function
	   it should not be set by directly template */
	def->ip_tos = NETLINK_FILTER_UNUSED;
	ret = filter_template_expand(def,
				     expanders,
				     ARRAY_SIZE(expanders),
				     &args);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, filter_template_expand, 0);
		filter_template_destroy(def);
		return 0;
	}

	ret = filter_template_to_netlink_filter_defs(def,
						     out,
						     &num_filter_defs);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, filter_template_to_netlink_filter_defs, 0);
		filter_template_destroy(def);
		return 0;
	}

	for (i = 0; i < num_filter_defs; ++i) {
		const struct filter_template *ds_template = NULL;

		if (ds)
			ds_template = tpl;

		pon_net_ext_vlan_make_cookie(&(*out)[i].vlan_data.cookie,
					     filter, ds_template);

		filter_definition_act_data_pointer_set(&(*out)[i]);
	}

	filter_template_destroy(def);
	dbg_out_ret("%d", num_filter_defs);
	return num_filter_defs;
}

/*
 * Classify struct pon_adapter_ext_vlan_fiter provided from higher layer as
 * one of our internal Extended VLAN configuration cases defined in
 * \ref omci_rules
 */
static int
pon_net_find_ext_vlan_rule(const struct pon_adapter_ext_vlan_filter *f,
			   int ds, const bool dump)
{
	int i, m = 0, best_match = 0;
	const struct omci_rules *ptr = &omci_rules[0];
	const struct omci_rule *rule;
	bool drop;

	for (i = 0; i < (int)ARRAY_SIZE(omci_rules); i++, ptr++) {
		rule = ds ? &ptr->down : &ptr->up;

		if (dump) {
			DBG(OMCI_API_ERR,
			    ("match: test rule: %d.%d: %s (%u)\n",
			     rule->major, rule->minor,
			     rule->description,  i));

			DBG(OMCI_API_ERR,
			    ("match: treatment_tags_to_remove %d %d\n",
			     rule->f.treatment_tags_to_remove,
			     f->treatment_tags_to_remove));
		}

		if (rule->f.treatment_tags_to_remove !=
						f->treatment_tags_to_remove)
			continue;

		if (f->treatment_tags_to_remove == 3)
			drop = true;
		else
			drop = false;

		if (dump) {
			DBG(OMCI_API_ERR,  ("match: filter_ethertype %d %d\n",
					    rule->f.filter_ethertype,
					    f->filter_ethertype));
		}

		if (rule->f.filter_ethertype == m_0_4) {
			if (f->filter_ethertype > 4 || f->filter_ethertype < 0)
				continue;
		} else if (rule->f.filter_ethertype == m_1_4) {
			if (f->filter_ethertype > 4 || f->filter_ethertype < 1)
				continue;
		} else {
			if (rule->f.filter_ethertype != f->filter_ethertype)
				continue;
		}

		if (dump) {
			DBG(OMCI_API_ERR,
			    ("match: filter_outer_priority %d %d\n",
			     rule->f.filter_outer_priority,
			     f->filter_outer_priority));
		}

		if (rule->f.filter_outer_priority == Ps ||
		    rule->f.filter_outer_priority == m_0_7) {
			if (f->filter_outer_priority > 7)
				continue;
		} else {
			if (rule->f.filter_outer_priority !=
						f->filter_outer_priority)
				continue;
		}

		if (dump) {
			DBG(OMCI_API_ERR,
			    ("match: filter_inner_priority %d %d\n",
			     rule->f.filter_inner_priority,
			      f->filter_inner_priority));
		}

		if (rule->f.filter_inner_priority == Pc) {
			if (f->filter_inner_priority > 7)
				continue;
		} else {
			if (rule->f.filter_inner_priority !=
						f->filter_inner_priority)
				continue;
		}

		if (rule->f.filter_outer_priority < 15) {
			if (dump) {
				DBG(OMCI_API_ERR,
				    ("match: filter_outer_vid %d %d\n",
				     rule->f.filter_outer_vid,
				     f->filter_outer_vid));
			}
			if (rule->f.filter_outer_vid == VIDs) {
				if (f->filter_outer_vid > 4095)
					continue;
			} else {
				if (rule->f.filter_outer_vid !=
							f->filter_outer_vid)
					continue;
			}
			if (rule->f.filter_outer_tpid_de == m_0_4_5_6_7) {
				switch (f->filter_outer_tpid_de) {
				case 0:
				case 4:
				case 5:
				case 6:
				case 7:
					break;
				default:
					continue;
				}
			} else {
				if (rule->f.filter_outer_tpid_de !=
					f->filter_outer_tpid_de)
					continue;
			}
		}

		if (rule->f.filter_inner_priority < 15) {
			if (dump) {
				DBG(OMCI_API_ERR,
				    ("match: filter_inner_vid %d %d\n",
				     rule->f.filter_inner_vid,
				     f->filter_inner_vid));
			}
			if (rule->f.filter_inner_vid == VIDc) {
				if (f->filter_inner_vid > 4095)
					continue;
			} else {
				if (rule->f.filter_inner_vid !=
					f->filter_inner_vid)
					continue;
			}
			if (rule->f.filter_inner_tpid_de == m_0_4_5_6_7) {
				switch (f->filter_inner_tpid_de) {
				case 0:
				case 4:
				case 5:
				case 6:
				case 7:
					break;
				default:
					continue;
				}
			} else {
				if (rule->f.filter_inner_tpid_de !=
					f->filter_inner_tpid_de)
					continue;
			}
		}

		if (!drop) {
			if (dump) {
				DBG(OMCI_API_ERR,
				    ("match: treatment_outer_priority %d %d\n",
				     rule->f.treatment_outer_priority,
				     f->treatment_outer_priority));
			}

			if (rule->f.treatment_outer_priority == m_0_7) {
				if (f->treatment_outer_priority > 7)
					continue;
			} else {
				if (rule->f.treatment_outer_priority !=
						f->treatment_outer_priority)
					continue;
			}

			if (dump) {
				DBG(OMCI_API_ERR,
				    ("match: treatment_inner_priority %d %d\n",
				     rule->f.treatment_inner_priority,
				     f->treatment_inner_priority));
			}

			if (rule->f.treatment_inner_priority == Px ||
				rule->f.treatment_inner_priority == m_0_7) {
				if (f->treatment_inner_priority > 7)
					continue;
			} else {
				if ((rule->f.treatment_inner_priority == 8 &&
				     f->treatment_inner_priority == 9) ||
				    (rule->f.treatment_inner_priority == 9 &&
				     f->treatment_inner_priority == 8)) {
					/* allow 8 - 9 mismatch */
				} else {
					if (rule->f.treatment_inner_priority
						!= f->treatment_inner_priority)
						continue;
				}
			}

			if (dump) {
				DBG(OMCI_API_ERR,
				    ("match: treatment_outer_priority %d treatment_outer_vid %d\n",
				     rule->f.treatment_outer_priority,
				     f->treatment_outer_vid));
			}

			if (f->treatment_outer_priority != 15) {
			    if (rule->f.treatment_outer_priority == m_0_7 &&
				f->treatment_outer_priority > 7)
				continue;

			    if (rule->f.treatment_outer_priority > 7 &&
				rule->f.treatment_outer_priority
				!= f->treatment_outer_priority)
				continue;

			    if (rule->f.treatment_outer_vid == VIDy) {
				if (f->treatment_outer_vid == 4096 ||
				    f->treatment_outer_vid == 4097)
				    continue;
			    } else {
				if (f->treatment_outer_vid <= 4095)
				    continue;
			    }
			}

			if (dump) {
				DBG(OMCI_API_ERR,
				    ("match: treatment_inner_priority %d treatment_inner_vid %d\n",
				     rule->f.treatment_inner_priority,
				     f->treatment_inner_vid));
			}

			if (f->treatment_inner_priority != 15) {
				/* if we add a tag,  match only valid points*/
				if (f->treatment_inner_vid > 4097)
					continue;

				if (rule->f.treatment_inner_vid == VIDx) {
					if (f->treatment_inner_vid == 4096 ||
					    f->treatment_inner_vid == 4097)
						continue;
				} else {
					if (f->treatment_inner_vid <= 4095)
						continue;
				}
			}
		}

		if (dump) {
			DBG(OMCI_API_ERR,
			     ("match: filter_inner_tpid_de %d filter_outer_tpid_de %d\n",
			      rule->f.filter_inner_tpid_de,
			      rule->f.filter_outer_tpid_de));
		}

		best_match = i;
		m++;

		DBG(OMCI_API_MSG,
		    ("found rule: %d.%d: %s; best_match=%u,  match_count=%u\n",
		     rule->major,  rule->minor,  rule->description,
		     best_match,  m));
	}

	if (m == 0) {
		DBG(OMCI_API_ERR,  ("no match\n"));
		return -1;
	}

	if (m > 1) {
		DBG(OMCI_API_ERR,  ("multi match\n"));
		return -2;
	}

	return best_match;
}

static int get_rule_by_filter(const struct pon_adapter_ext_vlan_filter *filter,
			      int ds,
			      int *major,
			      int *minor,
			      uint8_t *def)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int omci_idx;

	dbg_in_args("%p, %d, %p, %p, %p", filter, ds, major, minor, def);

	omci_idx = pon_net_find_ext_vlan_rule(filter, ds, false);
	if (omci_idx < 0) {
		dbg_err("can't find %s rule:", ds ? "ds" : "us");
		dbg_printf(DBG_ERR, " filter_outer_priority=%u",
			   filter->filter_outer_priority);
		dbg_printf(DBG_ERR, " filter_outer_vid=%u",
			   filter->filter_outer_vid);
		dbg_printf(DBG_ERR, " filter_outer_tpid_de=%u",
			   filter->filter_outer_tpid_de);
		dbg_printf(DBG_ERR, " filter_inner_priority=%u",
			   filter->filter_inner_priority);
		dbg_printf(DBG_ERR, " filter_inner_vid=%u",
			   filter->filter_inner_vid);
		dbg_printf(DBG_ERR, " filter_inner_tpid_de=%u",
			   filter->filter_inner_tpid_de);
		dbg_printf(DBG_ERR, " filter_ethertype=%u",
			   filter->filter_ethertype);
		dbg_printf(DBG_ERR, " treatment_tags_to_remove=%u",
			   filter->treatment_tags_to_remove);
		dbg_printf(DBG_ERR, " treatment_outer_priority=%u",
			   filter->treatment_outer_priority);
		dbg_printf(DBG_ERR, " treatment_outer_vid=%u",
			   filter->treatment_outer_vid);
		dbg_printf(DBG_ERR, " treatment_outer_tpid_de=%u",
			   filter->treatment_outer_tpid_de);
		dbg_printf(DBG_ERR, " treatment_inner_priority=%u",
			   filter->treatment_inner_priority);
		dbg_printf(DBG_ERR, " treatment_inner_vid=%u",
			   filter->treatment_inner_vid);
		dbg_printf(DBG_ERR, " treatment_inner_tpid_de=%u\n",
			   filter->treatment_inner_tpid_de);
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* rule number and def are same for upstream and downstream */
	*major = omci_rules[omci_idx].up.major;
	*minor = omci_rules[omci_idx].up.minor;
	*def = omci_rules[omci_idx].up.def;

	dbg_out_ret("%d", ret);
	return ret;
}

int
pon_net_get_rule_by_filter_us(const struct pon_adapter_ext_vlan_filter *filter,
			      int *major,
			      int *minor,
			      uint8_t *def)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %p, %p", filter, major, minor, def);
	ret = get_rule_by_filter(filter, 0, major, minor, def);
	dbg_out_ret("%d", ret);
	return ret;
}

int
pon_net_get_rule_by_filter_ds(const struct pon_adapter_ext_vlan_filter *filter,
			      int *major,
			      int *minor,
			      uint8_t *def)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %p, %p", filter, major, minor, def);
	ret = get_rule_by_filter(filter, 1, major, minor, def);
	dbg_out_ret("%d", ret);
	return ret;
}

static int cookie_map_val(const struct pon_adapter_ext_vlan_filter *filter,
			  int var, int def)
{
	int val = map_val(filter, var);

	return val == NETLINK_FILTER_UNUSED ? def : val;
}

#define LEN(x) ((0x1 << (x)) - 1)
#define PUSH(word, x, len) \
	do { \
		word <<= (len); \
		word |= (x) & LEN(len); \
	} while (0)

enum pon_adapter_errno
pon_net_ext_vlan_make_cookie(struct netlink_cookie *cookie,
			     const struct pon_adapter_ext_vlan_filter *f,
			     const struct filter_template *t)
{
	uint32_t words[4] = { 0 };
	int ret = 0;

	dbg_in_args("%p, %p", f, cookie);

	PUSH(words[0], f->filter_outer_priority, 4);
	PUSH(words[0], f->filter_outer_vid, 13);
	PUSH(words[0], f->filter_outer_tpid_de, 3);
	if (t) {
		PUSH(words[0], cookie_map_val(f, t->outer_vlan_prio, 0xF), 4);
		PUSH(words[0], cookie_map_val(f, t->outer_vlan_proto, 0), 3);
		PUSH(words[0], 0, 5);
	} else {
		PUSH(words[0], 0, 12);
	}
	words[0] = htonl(words[0]);

	PUSH(words[1], f->filter_inner_priority, 4);
	PUSH(words[1], f->filter_inner_vid, 13);
	PUSH(words[1], f->filter_inner_tpid_de, 3);
	if (t) {
		PUSH(words[1], cookie_map_val(f, t->inner_vlan_prio, 0xF), 4);
		PUSH(words[1], cookie_map_val(f, t->inner_vlan_proto, 0), 3);
		PUSH(words[1], 0, 1);
	} else {
		PUSH(words[1], 0, 8);
	}
	PUSH(words[1], f->filter_ethertype, 4);
	words[1] = htonl(words[1]);

	PUSH(words[2], f->treatment_tags_to_remove, 2);
	PUSH(words[2], 0, 10);
	PUSH(words[2], f->treatment_outer_priority, 4);
	PUSH(words[2], f->treatment_outer_vid, 13);
	PUSH(words[2], f->treatment_outer_tpid_de, 3);
	words[2] = htonl(words[2]);

	if (t) {
		PUSH(words[3], cookie_map_val(f, t->act_vlan_prio, 0xF), 4);
		PUSH(words[3], cookie_map_val(f, t->act_vlan_proto, 0), 3);
		PUSH(words[3], 0, 4);
		PUSH(words[3], 1, 1);
	} else {
		PUSH(words[3], 0, 12);
	}
	PUSH(words[3], f->treatment_inner_priority, 4);
	PUSH(words[3], f->treatment_inner_vid, 13);
	PUSH(words[3], f->treatment_inner_tpid_de, 3);
	words[3] = htonl(words[3]);

	ret = memcpy_s(cookie->cookie, sizeof(cookie->cookie), words,
		       sizeof(words));
	if (ret) {
		dbg_err_fn_ret(memcpy_s, ret);
		dbg_out_ret("%d", PON_ADAPTER_ERR_MEM_ACCESS);
		return PON_ADAPTER_ERR_MEM_ACCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define VLAN_ANY 4096
#define VLAN_COPY_FROM_INNER 4096
#define VLAN_COPY_FROM_OUTER 4097

bool
pon_net_ext_vlan_bridge_port_vlan(const struct pon_adapter_ext_vlan_filter *f,
				  uint16_t *vlan_id)
{
	/*
	 * You can think that this variable is a VLAN of a frame that enters
	 * the ONU
	 */
	int vlans[4];
	int vlans_len = 0;

	dbg_in_args("%p, %p", f, vlan_id);

	/* If we filter on inner tag, we have one tag */
	if (f->filter_inner_priority != 15)
		vlans[vlans_len++] = f->filter_inner_vid;
	/* If we filter on outer tag, we have another tag */
	if (f->filter_outer_priority != 15)
		vlans[vlans_len++] = f->filter_outer_vid;

	if (f->treatment_tags_to_remove > 2) {
		/* This is a drop rule - no frame would enter the bridge */
		dbg_out_ret("%d", false);
		return false;
	}

	/*
	 * At this moment we have 'vlans_len' tagged frame.
	 * Let's remove the number of VLANs that would be removed if we applied
	 * the rule
	 */
	vlans_len -= f->treatment_tags_to_remove;
	if (vlans_len < 0)
		vlans_len = 0;

	/* If we have a tag to push, push it */
	if (f->treatment_inner_priority != 15) {
		if (f->treatment_inner_vid == VLAN_COPY_FROM_INNER)
			vlans[vlans_len++] = f->filter_inner_vid;
		else if (f->treatment_inner_vid == VLAN_COPY_FROM_OUTER)
			vlans[vlans_len++] = f->filter_outer_vid;
		else
			vlans[vlans_len++] = f->treatment_inner_vid;
	}

	/* If we have a tag to push, push it */
	if (f->treatment_outer_priority != 15) {
		if (f->treatment_outer_vid == VLAN_COPY_FROM_INNER)
			vlans[vlans_len++] = f->filter_inner_vid;
		else if (f->treatment_outer_vid == VLAN_COPY_FROM_OUTER)
			vlans[vlans_len++] = f->filter_outer_vid;
		else
			vlans[vlans_len++] = f->treatment_outer_vid;
	}

	/* If frame is untagged */
	if (!vlans_len) {
		dbg_out_ret("%d", false);
		return false;
	}

	/* Return the outer tag if available */
	if (vlans[vlans_len - 1] != VLAN_ANY) {
		*vlan_id = (uint16_t)vlans[vlans_len - 1];
		dbg_out_ret("%d", true);
		return true;
	}

	dbg_out_ret("%d", false);
	return false;
}
