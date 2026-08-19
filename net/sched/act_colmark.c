// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 *****************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/version.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>

#include <linux/tc_act/tc_colmark.h>
#include <net/tc_act/tc_colmark.h>

#define COLMARK_TAB_MASK	256

static unsigned int colmark_net_id;
static struct tc_action_ops act_colmark_ops;

static int tcf_colmark(struct sk_buff *skb, const struct tc_action *a,
		       struct tcf_result *res)
{
	struct tcf_colmark *d = to_colmark(a);

	/* TODO: add real implementation in the future. Currently only offload
	 * path is supported
	 */

	return d->tcf_action;
}

#ifndef NLA_POLICY_EXACT_LEN
#define NLA_POLICY_EXACT_LEN(__len) { .len = __len }
#endif

static const struct nla_policy colmark_policy[TCA_COLMARK_MAX + 1] = {
	[TCA_COLMARK_PARMS]		= NLA_POLICY_EXACT_LEN(sizeof(struct tc_colmark)),
	[TCA_COLMARK_MODE]		= { .type = NLA_U8 },
	[TCA_COLMARK_DROP_PRECEDENCE]	= { .type = NLA_U8 },
	[TCA_COLMARK_METER_TYPE]	= { .type = NLA_U8 },
};

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
static int tcf_colmark_init(struct net *net, struct nlattr *nla,
			    struct nlattr *est, struct tc_action **a,
			    int ovr, int bind)
#elif (KERNEL_VERSION(5, 15, 0) > LINUX_VERSION_CODE)
static int tcf_colmark_init(struct net *net, struct nlattr *nla,
			    struct nlattr *est, struct tc_action **a,
			    int ovr, int bind, bool rtnl_held,
			    struct netlink_ext_ack *extack)
#else
static int tcf_colmark_init(struct net *net, struct nlattr *nla,
			    struct nlattr *est, struct tc_action **a,
			    struct tcf_proto *tp,
			    u32 flags, struct netlink_ext_ack *extack)
#endif
{
	struct tc_action_net *tn = net_generic(net, colmark_net_id);
#if (KERNEL_VERSION(5, 15, 0) <= LINUX_VERSION_CODE)
	bool bind = flags & TCA_ACT_FLAGS_BIND;
#endif
	struct nlattr *tb[TCA_COLMARK_MAX + 1];
	struct tc_colmark *parm;
	struct tcf_colmark *d;
	u32 lflags = 0;
	u8 *mode = NULL, *precedence = NULL, *type = NULL;
	bool exists = false;
	int ret = 0, err;

	if (!nla)
		return -EINVAL;

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	err = nla_parse_nested(tb, TCA_COLMARK_MAX, nla, colmark_policy);
#else
	err = nla_parse_nested(tb, TCA_COLMARK_MAX, nla, colmark_policy,
			       extack);
#endif
	if (err < 0)
		return err;

	if (!tb[TCA_COLMARK_PARMS])
		return -EINVAL;

	if (tb[TCA_COLMARK_MODE]) {
		lflags |= COLMARK_F_MODE;
		mode = nla_data(tb[TCA_COLMARK_MODE]);
	}

	if (tb[TCA_COLMARK_DROP_PRECEDENCE]) {
		lflags |= COLMARK_F_DROP_PRECEDENCE;
		precedence = nla_data(tb[TCA_COLMARK_DROP_PRECEDENCE]);
	}

	if (tb[TCA_COLMARK_METER_TYPE]) {
		lflags |= COLMARK_F_METER_TYPE;
		type = nla_data(tb[TCA_COLMARK_METER_TYPE]);
	}

	parm = nla_data(tb[TCA_COLMARK_PARMS]);

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	exists = tcf_hash_check(tn, parm->index, a, bind);
#else
	exists = tcf_idr_check_alloc(tn, &parm->index, a, bind);
#endif
	if (exists && bind)
		return 0;

	if (!lflags) {
		if (exists)
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
			tcf_hash_release(*a, bind);
#else
			tcf_idr_release(*a, bind);
#endif
		return -EINVAL;
	}

	if (!exists) {
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
		ret = tcf_hash_create(tn, parm->index, est, a,
				      &act_colmark_ops, bind, false);
#elif  (KERNEL_VERSION(5, 5, 0) > LINUX_VERSION_CODE)
		ret = tcf_idr_create(tn, parm->index, est, a,
				     &act_colmark_ops, bind, false);
#else
		ret = tcf_idr_create(tn, parm->index, est, a,
				     &act_colmark_ops, bind, false, 0);
#endif
		if (ret)
			return ret;

		d = to_colmark(*a);
		ret = ACT_P_CREATED;
	} else {
		d = to_colmark(*a);
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
		tcf_hash_release(*a, bind);
#else
		tcf_idr_release(*a, bind);
#endif
#if (KERNEL_VERSION(5, 15, 0) > LINUX_VERSION_CODE)
		if (!ovr)
#else
		if (!(flags & TCA_ACT_FLAGS_REPLACE))
#endif
			return -EEXIST;
	}

	d->flags = lflags;
	if (lflags & COLMARK_F_MODE)
		d->blind = *mode  ? true : false;
	if (lflags & COLMARK_F_DROP_PRECEDENCE)
		d->precedence = *precedence;
	if (lflags & COLMARK_F_METER_TYPE)
		d->type = *type;

	d->tcf_action = parm->action;

#if (KERNEL_VERSION(5, 9, 0) > LINUX_VERSION_CODE)
	if (ret == ACT_P_CREATED)
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
		tcf_hash_insert(tn, *a);
#else
		 tcf_idr_insert(tn, *a);
#endif
#endif
	return ret;
}

static int tcf_colmark_dump(struct sk_buff *skb, struct tc_action *a,
			    int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_colmark *d = to_colmark(a);
	struct tc_colmark opt = {
		.index   = d->tcf_index,
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
		.refcnt  = d->tcf_refcnt - ref,
#else
		.refcnt  = d->tcf_refcnt.refs.counter - ref,
#endif
		.bindcnt = d->tcf_bindcnt.counter - bind,
		.action  = d->tcf_action,
	};
	struct tcf_t t;

	if (nla_put(skb, TCA_COLMARK_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;
	if ((d->flags & COLMARK_F_MODE) &&
	    nla_put_u8(skb, TCA_COLMARK_MODE, d->blind ? 1 : 0))
		goto nla_put_failure;
	if ((d->flags & COLMARK_F_DROP_PRECEDENCE) &&
	    nla_put_u8(skb, TCA_COLMARK_DROP_PRECEDENCE, d->precedence))
		goto nla_put_failure;
	if ((d->flags & COLMARK_F_METER_TYPE) &&
	    nla_put_u8(skb, TCA_COLMARK_METER_TYPE, d->type))
		goto nla_put_failure;

	tcf_tm_dump(&t, &d->tcf_tm);
	if (nla_put_64bit(skb, TCA_COLMARK_TM, sizeof(t), &t, TCA_COLMARK_PAD))
		goto nla_put_failure;
	return skb->len;

nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
static int tcf_colmark_walker(struct net *net, struct sk_buff *skb,
			      struct netlink_callback *cb, int type,
			      const struct tc_action_ops *ops)
#else
static int tcf_colmark_walker(struct net *net, struct sk_buff *skb,
			      struct netlink_callback *cb, int type,
			      const struct tc_action_ops *ops,
			      struct netlink_ext_ack *extack)
#endif
{
	struct tc_action_net *tn = net_generic(net, colmark_net_id);

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	return tcf_generic_walker(tn, skb, cb, type, ops);
#else
	return tcf_generic_walker(tn, skb, cb, type, ops, extack);
#endif
}

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
static int tcf_colmark_search(struct net *net, struct tc_action **a, u32 index)
#elif (KERNEL_VERSION(4, 20, 0) > LINUX_VERSION_CODE)
static int tcf_colmark_search(struct net *net, struct tc_action **a, u32 index,
			      struct netlink_ext_ack *extack)
#else
static int tcf_colmark_search(struct net *net, struct tc_action **a, u32 index)
#endif
{
	struct tc_action_net *tn = net_generic(net, colmark_net_id);

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	return tcf_hash_search(tn, a, index);
#else
	return tcf_idr_search(tn, a, index);
#endif
}

static struct tc_action_ops act_colmark_ops = {
	.kind		=	"colmark",
	.id		=	TCA_ID_COLMARK,
	.owner		=	THIS_MODULE,
	.act		=	tcf_colmark,
	.dump		=	tcf_colmark_dump,
	.init		=	tcf_colmark_init,
	.walk		=	tcf_colmark_walker,
	.lookup		=	tcf_colmark_search,
	.size		=	sizeof(struct tcf_colmark),
};

static __net_init int colmark_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, colmark_net_id);

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	return tc_action_net_init(tn, &act_colmark_ops, COLMARK_TAB_MASK);
#elif (KERNEL_VERSION(4, 14, 143) > LINUX_VERSION_CODE)
	return tc_action_net_init(tn, &act_colmark_ops);
#elif (KERNEL_VERSION(4, 15, 0) > LINUX_VERSION_CODE)
	return tc_action_net_init(net, tn, &act_colmark_ops);
#elif (KERNEL_VERSION(4, 19, 72) > LINUX_VERSION_CODE)
	return tc_action_net_init(tn, &act_colmark_ops);
#else
	return tc_action_net_init(net, tn, &act_colmark_ops);
#endif
}

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
static void __net_exit colmark_exit_net(struct net *net)
#else
static void __net_exit colmark_exit_net(struct list_head *net_list)
#endif
{
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	struct tc_action_net *tn = net_generic(net, colmark_net_id);

	tc_action_net_exit(tn);
#else
	tc_action_net_exit(net_list, colmark_net_id);
#endif
}

static struct pernet_operations colmark_net_ops = {
	.init = colmark_init_net,
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	.exit = colmark_exit_net,
#else
	.exit_batch = colmark_exit_net,
#endif
	.id   = &colmark_net_id,
	.size = sizeof(struct tc_action_net),
};

static int __init colmark_init_module(void)
{
	return tcf_register_action(&act_colmark_ops, &colmark_net_ops);
}

static void __exit colmark_cleanup_module(void)
{
	tcf_unregister_action(&act_colmark_ops, &colmark_net_ops);
}

module_init(colmark_init_module);
module_exit(colmark_cleanup_module);

MODULE_DESCRIPTION("Packet color marking");
MODULE_LICENSE("GPL");
