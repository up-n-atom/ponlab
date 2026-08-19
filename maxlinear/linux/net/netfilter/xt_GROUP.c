// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2022-2024 MaxLinear, Inc.
 *
 * Summary :
 * This iptables target extension provides new target "GROUP" which adds the
 * sessions to the account or the meter passed as parameter.
 */
#define pr_fmt(fmt) "[xt_GROUP:%s:%d] " fmt, __func__, __LINE__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter/xt_GROUP.h>
#if IS_ENABLED(CONFIG_SGAM)
#include <net/sgam/sgam_api.h>
#endif /* IS_ENABLED(CONFIG_SGAM) */

#if IS_ENABLED(CONFIG_SGAM)
int xt_group_attach_to_skb(struct sk_buff *skb,
		const struct xt_group_info *info)
{
	int ret = XT_CONTINUE;

	if (!info) {
		pr_warn_once("%s: INVALID group info\n", __func__);
		return XT_RETURN;
	}

	if (IS_METER_DB(info->flags)) {
		struct sgam_meter_info meter_info = {0};

		strncpy(meter_info.name, info->name, MAX_LABEL_LEN);
		meter_info.tbm_id = info->id;
		meter_info.remark_dscp = info->remark_dscp;
		if (sgam_meter_attach_to_skb(skb, &meter_info) != SGAM_SUCCESS)
			return XT_RETURN;
	} else if (IS_ACCT_DB(info->flags)) {
		struct sgam_acct_info acct_info = {0};

		strncpy(acct_info.name, info->name, MAX_LABEL_LEN);
		acct_info.sgc_id = info->id;
		acct_info.sgc_grp = sgam_convert_pp_to_sgam_sgc_grp(info->grp);
		if (sgam_account_attach_to_skb(skb, &acct_info) != SGAM_SUCCESS)
			return XT_RETURN;
	} else { /* Unlikely */
		pr_warn_once("SKB belongs to neither GROUP nor METER !!\n");
		return XT_RETURN;
	}

	return ret;
}
#endif /* IS_ENABLED(CONFIG_SGAM) */

static unsigned int xt_group_target(struct sk_buff *skb,
				    const struct xt_action_param *par)
{
#if IS_ENABLED(CONFIG_SGAM)
	return xt_group_attach_to_skb(skb,
				     (struct xt_group_info *)par->targinfo);
#else
	return XT_CONTINUE;
#endif /* IS_ENABLED(CONFIG_SGAM) */
}

#if IS_ENABLED(CONFIG_SGAM)
static int xt_group_convert_to_id(const struct xt_tgchk_param *par)
{
	struct xt_group_info *info = par->targinfo;
	int ret = SGAM_SUCCESS;

	ret = sgam_xt_get_id_from_name(info);
	if (ret != SGAM_SUCCESS) {
		pr_err("No %s exists with the name %s\n",
		       IS_METER_DB(info->flags) ? "meter" : "group",
		       info->name);
	}

	return ret;
}
#endif /* IS_ENABLED(CONFIG_SGAM) */

static int xt_group_checkentry_v4(const struct xt_tgchk_param *par)
{
#if IS_ENABLED(CONFIG_SGAM)
	struct xt_group_info *info = par->targinfo;
	const struct ipt_ip *ip = par->entryinfo;

	if ((info->flags & ~SGAM_DB_MASK) ||
	    (ip->proto != IPPROTO_TCP && ip->proto != IPPROTO_UDP)) {
		pr_err("Only TCP and UDP protocols are supported!!\n");
		return -EINVAL;
	}

	return xt_group_convert_to_id(par);
#else
	pr_err("The target 'GROUP' won't work without sgam-drv and skb extensions!\n");

	return -ENOENT;
#endif /* IS_ENABLED(CONFIG_SGAM) */
}

#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
static int xt_group_checkentry_v6(const struct xt_tgchk_param *par)
{
#if IS_ENABLED(CONFIG_SGAM)
	struct xt_group_info *info = par->targinfo;
	const struct ip6t_ip6 *ip6 = par->entryinfo;

	if ((info->flags & ~SGAM_DB_MASK) ||
	    (ip6->proto != IPPROTO_TCP && ip6->proto != IPPROTO_UDP)) {
		pr_err("Only TCP and UDP protocols are supported!!\n");
		return -EINVAL;
	}

	return xt_group_convert_to_id(par);
#else
	pr_err("The target 'GROUP' won't work without sgam-drv and skb extensions!\n");

	return -ENOENT;
#endif /* IS_ENABLED(CONFIG_SGAM) */
}
#endif /* IS_ENABLED(CONFIG_IP6_NF_IPTABLES) */

static void xt_group_destroy(const struct xt_tgdtor_param *par)
{
#if IS_ENABLED(CONFIG_SGAM)
	int ret = SGAM_SUCCESS;
	struct xt_group_info *info = par->targinfo;

	ret = sgam_xt_update_db(info);
	if (ret != SGAM_SUCCESS)
		pr_notice("Failed to update SGAM db!!\n");
#else
	pr_err("The target 'GROUP' won't work without sgam-drv and skb extensions!\n");
#endif /* IS_ENABLED(CONFIG_SGAM) */
}

static struct xt_target xt_group[] __read_mostly = {
	{
		.name           = "GROUP",
		.revision       = 0,
		.family         = NFPROTO_IPV4,
		.target         = xt_group_target,
		.targetsize     = sizeof(struct xt_group_info),
		.checkentry     = xt_group_checkentry_v4,
		.destroy        = xt_group_destroy,
		.me             = THIS_MODULE,
	},
#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
	{
		.name           = "GROUP",
		.revision       = 0,
		.family         = NFPROTO_IPV6,
		.target         = xt_group_target,
		.targetsize     = sizeof(struct xt_group_info),
		.checkentry     = xt_group_checkentry_v6,
		.destroy        = xt_group_destroy,
		.me             = THIS_MODULE,
	},
#endif /* IS_ENABLED(CONFIG_IP6_NF_IPTABLES) */
};

static int __init xt_group_init(void)
{
	return xt_register_targets(xt_group, ARRAY_SIZE(xt_group));
}

static void __exit xt_group_exit(void)
{
	xt_unregister_targets(xt_group, ARRAY_SIZE(xt_group));
}

module_init(xt_group_init);
module_exit(xt_group_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Module for adding sessions to group acocunts or meters");
MODULE_AUTHOR("Shashidhar Kattemane Venkatesh <skvenkatesh@maxlinear.com>");
MODULE_ALIAS("ipt_GROUP");
