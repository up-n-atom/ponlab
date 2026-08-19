// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
 */

#include <asm/types.h>
#include <linux/adp_nl.h>
#include <linux/debugfs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/net.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/thermal.h>
#include <linux/version.h>
#include <net/genetlink.h>
#include <net/netlink.h>
#include <net/sock.h>

/* #################### function prototypes */
/* receive functions */
static int adp_genl_cmd_dvfs(struct sk_buff *skb, struct genl_info *info);
static int adp_genl_cmd_cooling(struct sk_buff *skb, struct genl_info *info);
static int adp_genl_cmd_status(struct sk_buff *skb, struct genl_info *info);
/* ####################                     */


/* #################### static variable definition */
static struct nla_policy adp_genl_policy[ADP_GENL_ATTR_MAX+1] = {
[ADP_GENL_ATTR_SRC]          = { .type = NLA_STRING },
[ADP_GENL_ATTR_COMMAND]      = { .type = NLA_U32 },
[ADP_GENL_ATTR_LINK]         = { .type = NLA_STRING },
[ADP_GENL_ATTR_LSPEED]       = { .type = NLA_U32 },
[ADP_GENL_ATTR_STATUS_COOL]  = { .type = NLA_U32 },
[ADP_GENL_ATTR_STATUS_DVFS]  = { .type = NLA_U32 },
};

/** List of the supported NetLink messages. */
static struct genl_ops adp_genl_ops[] = {
	{
		.cmd = ADP_GENL_CMD_DVFS,
		.policy = adp_genl_policy,
		.doit = adp_genl_cmd_dvfs,
	},
	{
		.cmd = ADP_GENL_CMD_COOLING,
		.policy = adp_genl_policy,
		.doit = adp_genl_cmd_cooling,
	},
	{
		.cmd = ADP_GENL_CMD_STATUS,
		.policy = adp_genl_policy,
		.doit = adp_genl_cmd_status,
	},
};

struct sock *nl_sk;
static const struct genl_multicast_group adp_event_mcgrps[] = {
	{ .name = ADP_GENL_MCAST_GROUP_NAME, },
};

static struct genl_family adp_event_genl_family __ro_after_init = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0)
	.id = GENL_ID_GENERATE,
#endif
	.name = ADP_GENL_FAMILY_NAME,
	.version = ADP_GENL_VERSION,
	.maxattr = ADP_GENL_ATTR_MAX,
	.ops = adp_genl_ops,
	.n_ops = ARRAY_SIZE(adp_genl_ops),
	.mcgrps = adp_event_mcgrps,
	.n_mcgrps = ARRAY_SIZE(adp_event_mcgrps),
};

static char *adp_cmd_str[] = {
	"ADP_UNKNOWN",
	"ADP_COOLING_DIS",
	"ADP_COOLING_ENA",
	"ADP_DVFS_LOW",
	"ADP_DVFS_MED",
	"ADP_DVFS_HIGH",
	"ADP_COOLING_ND4", /* not defined */
	"ADP_COOLING_ND5", /* not defined */
};

static int adp_dvfs_status = ADP_DVFS_HIGH;
static int adp_cooling_status = ADP_COOLING_DIS;
/* ####################                     */


/* #################### Debugfs definitions - for test only*/
static struct dentry *adp_debug;

static ssize_t write_nl_msg(struct file *file, const char __user *ubuf,
					  size_t len, loff_t *offp)
{
	char *buf;
	int ret;

	buf = memdup_user(ubuf, len);
	if (IS_ERR(buf))
		return PTR_ERR(buf);

	ret = kstrtoint(buf, 0, &adp_cooling_status);
	if (ret != 0) {
		kfree(buf);
		return ret;
	}
	kfree(buf);

	if ((adp_cooling_status == ADP_COOLING_ENA) ||
		(adp_cooling_status == ADP_COOLING_DIS))
		adp_send_nlmc(ADP_GENL_CMD_COOLING, adp_cooling_status);

	return len;
}

static int show_nl_msg(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", adp_cooling_status);
	return 0;
}
static int open_nl_msg(struct inode *inode, struct file *file)
{
	return single_open(file, show_nl_msg, NULL);
}

static const struct file_operations nl_msg_fops = {
	.open		= open_nl_msg,
	.write		= write_nl_msg,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
/* ####################                     */

static int adp_send_nl_reply(int nl_cmd, int cmd, struct genl_info *info)
{
	int ret;
	struct sk_buff *skb;
	void *hdr;

	if (!info)
		return -EINVAL;

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		pr_err("can't allocate memory\n");
		return -ENOMEM;
	}

	hdr = genlmsg_put_reply(skb, info, &adp_event_genl_family, 0 ,nl_cmd);
	if (!hdr) {
		nlmsg_free(skb);
		return -EMSGSIZE;
	}

	ret = nla_put_string(skb, ADP_GENL_ATTR_SRC, "ADP_DEVICE");
	if (ret) {
		pr_err("can't add ADP SRC to netlink msg: %i\n", ret);
		return -EINVAL;
	}

	ret = nla_put_u32(skb, ADP_GENL_ATTR_COMMAND, cmd);
	if (ret) {
		pr_err("can't add ADP CMD to netlink msg: %i\n", ret);
		return -EINVAL;
	}

	switch (nl_cmd) {
	case ADP_GENL_CMD_COOLING:
		adp_cooling_status = cmd;
		break;
	case ADP_GENL_CMD_STATUS:
		ret = nla_put_u32(skb, ADP_GENL_ATTR_STATUS_COOL,
							adp_cooling_status);
		if (ret)
			pr_err("can't add data to netlink msg: %i\n", ret);

		ret = nla_put_u32(skb, ADP_GENL_ATTR_STATUS_DVFS,
							adp_dvfs_status);
		if (ret)
			pr_err("can't add data to netlink msg: %i\n", ret);
		break;
	default:
		pr_err("ADP netlink cmd not supported (%d) \n", nl_cmd);
		nlmsg_free(skb);
		return -EINVAL;
	}

	genlmsg_end(skb, hdr);

	return genlmsg_reply(skb, info);
}

/* adp dvfs receive callback */
static int adp_genl_cmd_dvfs(struct sk_buff *skb, struct genl_info *info)
{
	int cmd, ret = 0;
	char src[ADP_STRING_MAX];

	if (info->attrs) {
		nla_strscpy(src, info->attrs[ADP_GENL_ATTR_SRC],
						ADP_STRING_MAX);
		cmd = nla_get_u32(info->attrs[ADP_GENL_ATTR_COMMAND]);
		switch (cmd & ADP_CMD_MASK) {
		case ADP_DVFS_HIGH:
			pr_debug("DVFS Level HIGH is requested from %s\n",
				 src);
			adp_dvfs_status = ADP_DVFS_HIGH;
			ret = epu_notifier_blocking_chain(ADP_EVENT_HIGH, NULL);
			break;
		case ADP_DVFS_MED:
			pr_debug("DVFS Level MED is requested from %s\n", src);
			adp_dvfs_status = ADP_DVFS_MED;
			ret = epu_notifier_blocking_chain(ADP_EVENT_MED, NULL);
			break;
		case ADP_DVFS_LOW:
			pr_debug("DVFS Level LOW is requested from %s\n", src);
			adp_dvfs_status = ADP_DVFS_LOW;
			ret = epu_notifier_blocking_chain(ADP_EVENT_LOW, NULL);
			break;

		default:
			pr_err("unknown ADP DVFS command received\n");
			break;
		}
	}
	return 0;
}

/* adp cooling receive callback */
static int adp_genl_cmd_cooling(struct sk_buff *skb, struct genl_info *info)
{
	int cmd;
	char src[ADP_STRING_MAX];

	if (info->attrs) {
		nla_strscpy(src, info->attrs[ADP_GENL_ATTR_SRC],
			    ADP_STRING_MAX);
		cmd = nla_get_u32(info->attrs[ADP_GENL_ATTR_COMMAND]);
		if (cmd & ADP_CMD_MASK) {
			if (cmd & ADP_CMD_ACK) {
				pr_debug("%s send ACK for cmd: %s\n", src,
					adp_cmd_str[cmd & ADP_CMD_MASK]);
			} else {
				pr_debug("%s send cmd: %s\n", src,
					adp_cmd_str[cmd & ADP_CMD_MASK]);
			}
		} else {
			pr_debug("unknown cooling cmd received from %s\n",
				 src);
		}
	}
	return 0;
}

/* adp status receive callback */
static int adp_genl_cmd_status(struct sk_buff *skb, struct genl_info *info)
{
	int cmd;
	char src[ADP_STRING_MAX];

	if (info->attrs) {
		nla_strscpy(src, info->attrs[ADP_GENL_ATTR_SRC],
			    ADP_STRING_MAX);
		cmd = nla_get_u32(info->attrs[ADP_GENL_ATTR_COMMAND]);
		/* ADP command received */
		if (cmd & ADP_CMD_ACK) {
			pr_debug("%s send ACK for get status answer\n", src);
		} else {
			pr_debug("%s sending get status\n", src);
			adp_send_nl_reply(ADP_GENL_CMD_STATUS, ADP_CMD_ACK,
									 info);
		}
	}
	return 0;
}

/* send multicast message */
int adp_send_nlmc(int nl_cmd, int cmd)
{
	struct sk_buff *skb;
	void *msg_header;
	int ret;
	unsigned int adp_event_seqnum = 1;

	/* allocate memory */
	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		pr_err("can't allocate memory\n");
		return -ENOMEM;
	}

	/* add the genetlink message header */
	msg_header = genlmsg_put(skb, 0, adp_event_seqnum,
				 &adp_event_genl_family, 0,
				 nl_cmd);
	if (!msg_header) {
		nlmsg_free(skb);
		return -ENOMEM;
	}

	/* add attributes */
	ret = nla_put_string(skb, ADP_GENL_ATTR_SRC, "ADP_DEVICE");
	if (ret)
		pr_err("can't add data to netlink msg: %i\n", ret);

	ret = nla_put_u32(skb, ADP_GENL_ATTR_COMMAND, cmd);
	if (ret)
		pr_err("can't add data to netlink msg: %i\n", ret);

	if (nl_cmd == ADP_GENL_CMD_COOLING)
		adp_cooling_status = cmd;

	if (nl_cmd == ADP_GENL_CMD_STATUS) {
		ret = nla_put_u32(skb, ADP_GENL_ATTR_STATUS_COOL,
							adp_cooling_status);
		if (ret)
			pr_err("can't add data to netlink msg: %i\n", ret);

		ret = nla_put_u32(skb, ADP_GENL_ATTR_STATUS_DVFS,
							adp_dvfs_status);
		if (ret)
			pr_err("can't add data to netlink msg: %i\n", ret);
	}

	/* send multicast genetlink message */
	genlmsg_end(skb, msg_header);
	genlmsg_multicast(&adp_event_genl_family, skb, 0, 0, GFP_KERNEL);

	return ret;
}
EXPORT_SYMBOL_GPL(adp_send_nlmc);

static int adp_epu_dvfs_lvl_notifier(struct notifier_block *nb,
						   unsigned long e, void *data)
{
	int ret;

	if (e == adp_dvfs_status)
		return 0; /* No DVFS level change. Skip it. */

	switch(e) {
	case 0: /* There is no enum for dvfs lvl in EPU drv */
		adp_dvfs_status = ADP_DVFS_HIGH;
		break;
	case 1:
		adp_dvfs_status = ADP_DVFS_MED;
		break;
	case 2:
		adp_dvfs_status = ADP_DVFS_LOW;
		break;
	default:
		pr_err("Received EPU DVFS lvl %lu  which is not convertible",
									    e);
		return -EINVAL ;
	}

	/* Multicast the notification to the subscribers */
	ret = adp_send_nlmc(ADP_GENL_CMD_STATUS, ADP_CMD_ACK);
	if (ret) {
		pr_err("Cannot propagate DVFS level change notification!\n");
		return ret;
	}

	return 0;
}

static struct notifier_block nb = {.notifier_call = adp_epu_dvfs_lvl_notifier};

static int genetlink_init(void)
{
	return genl_register_family(&adp_event_genl_family);
}

static void genetlink_exit(void)
{
	genl_unregister_family(&adp_event_genl_family);
}

static int __init adp_nl_init(void)
{
	struct dentry *d;
	int ret;

	ret = genetlink_init();
	if (ret) {
		pr_err("can't register generic netlink");
		return ret;
	}

	/* Init debugfs - for test only */
	adp_debug = debugfs_create_dir("adp_nl_drv", NULL);
	d = debugfs_create_file("nl_msg", 0600, adp_debug, NULL, &nl_msg_fops);
	if (!d) {
		pr_err("%s: Cannot create debugfs file\n ", __func__);
		return -ENOMEM;
	}

	/* Sign up for EPU DVFS level change notification */
	ret = epu_adp_lvl_notify_register(&nb);
	if (unlikely(ret)) {
		pr_err("Failed to epu_adp_lvl_notify_register\n");
		return ret;
	}

	return 0;
}

static void __exit adp_nl_exit(void)
{
	pr_debug("exiting adp_nl_drv module\n");

	epu_adp_lvl_notify_unregister(&nb);
	genetlink_exit();
}

fs_initcall(adp_nl_init);
module_exit(adp_nl_exit);

struct adp_cooling_device {
	unsigned int		cur_state;
	unsigned int		max_state;
	unsigned int		cooling_levels[2];
	struct thermal_cooling_device *cdev;
};

/* thermal cooling device callbacks */
static int adp_get_max_state(struct thermal_cooling_device *cdev,
			     unsigned long *state)
{
	struct adp_cooling_device *adev = cdev->devdata;

	if (!adev)
		return -EINVAL;

	*state = adev->max_state;

	return 0;
}

static int adp_get_cur_state(struct thermal_cooling_device *cdev,
			     unsigned long *state)
{
	struct adp_cooling_device *adev = cdev->devdata;

	if (!adev)
		return -EINVAL;

	*state = adev->cur_state;

	return 0;
}

static int
adp_set_cur_state(struct thermal_cooling_device *cdev, unsigned long state)
{
	struct adp_cooling_device *adev = cdev->devdata;
	int ret;

	if (!adev || (state > adev->max_state))
		return -EINVAL;

	if (state == adev->cur_state)
		return 0;

	ret = adp_send_nlmc(ADP_GENL_CMD_COOLING,
			    adev->cooling_levels[state]);
	if (ret) {
		dev_err(&cdev->device, "Cannot set adp cooling device!\n");
		return ret;
	}

	adev->cur_state = state;

	return ret;
}

static const struct thermal_cooling_device_ops adp_cooling_ops = {
	.get_max_state = adp_get_max_state,
	.get_cur_state = adp_get_cur_state,
	.set_cur_state = adp_set_cur_state,
};

static int adp_cooling_probe(struct platform_device *pdev)
{
	struct thermal_cooling_device *cdev;
	struct adp_cooling_device *adev;

	adev = devm_kzalloc(&pdev->dev, sizeof(*adev), GFP_KERNEL);
	if (!adev)
		return -ENOMEM;

	platform_set_drvdata(pdev, adev);

	adev->max_state = 1;
	adev->cur_state = 0;
	adev->cooling_levels[0] = ADP_COOLING_DIS;
	adev->cooling_levels[1] = ADP_COOLING_ENA;

	cdev = devm_thermal_of_cooling_device_register(&pdev->dev,
						       pdev->dev.of_node,
						       "adp", adev,
						       &adp_cooling_ops);
	if (IS_ERR(cdev)) {
		dev_err(&pdev->dev,
			"Failed to register adp as cooling device");
		return PTR_ERR(cdev);
	}

	adev->cdev = cdev;

	return 0;
}

static const struct of_device_id of_adp_cooling_match[] = {
	{ .compatible = "adp-cooling", },
	{ },
};

static struct platform_driver adp_cooling_driver = {
	.probe	= adp_cooling_probe,
	.driver = {
		.name = "adp-cooling",
		.of_match_table = of_match_ptr(of_adp_cooling_match),
	},
};

module_platform_driver(adp_cooling_driver);
MODULE_LICENSE("GPL");
