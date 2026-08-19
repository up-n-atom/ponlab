// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2022, MaxLinear, Inc. */
/* Copyright (C) 2020 Intel Corporation. */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/version.h>

#include <net/datapath_api.h>
#if IS_ENABLED(CONFIG_QOS_TC)
#include <net/qos_tc.h>
#endif

static struct net_device *rndev;
static struct platform_device *rpdev;

struct dp_eth_reinsert_priv {
	struct net_device_stats stats;
	int status;
	struct net_device *dev;
};

int dp_eth_reinsert_open(struct net_device *dev)
{
	netif_start_queue(dev);
	return 0;
}

int dp_eth_reinsert_release(struct net_device *dev)
{
	netif_stop_queue(dev);
	return 0;
}

int dp_eth_reinsert_config(struct net_device *dev, struct ifmap *map)
{
	return 0;
}

static bool dp_eth_reinsert_skb_mark_ok(struct sk_buff *skb)
{
	if (!skb || !skb->sk || !skb->sk->sk_socket)
		return false;

	if (skb->sk->sk_socket->type == SOCK_RAW && skb->mark > 0)
		return true;

	return false;
}

int dp_eth_reinsert_tx(struct sk_buff *skb, struct net_device *dev)
{
	struct dma_tx_desc_0 *dw0 = (struct dma_tx_desc_0 *)&skb->DW0;
	struct dma_tx_desc_1 *dw1 = (struct dma_tx_desc_1 *)&skb->DW1;
	struct net_device *rdev = NULL;
	dp_subif_t subif = {0};
	int ret = -EINVAL;

	do {
		/* ifindex for reinsertion passed using the SO_MARK */
		if (!dp_eth_reinsert_skb_mark_ok(skb))
			break;

		rdev = dev_get_by_index(&init_net, skb->mark);
		if (!rdev)
			break;

		ret = dp_get_netif_subifid(rdev, NULL, NULL, NULL, &subif, 0);
		if (ret != DP_SUCCESS) {
			netdev_err(skb->dev, "%s: err %d for %s\n",
				   __func__, ret, rdev->name);
			break;
		}

		dw1->field.ep = subif.port_id;
		dw0->field.dest_sub_if_id = subif.subif;
		netdev_dbg(skb->dev, "%s: ifi: %u if: %s EP: %u subif: %u\n",
			   __func__, rdev->ifindex, rdev->name,
			   subif.port_id, subif.subif);
		ret = dp_xmit(rdev, &subif, skb, skb->len, DP_TX_INSERT);
		if (ret)
			netdev_err(skb->dev, "%s: insert fail\n", __func__);

		/* dp_xmit always frees the skb */
		ret = 0;
	} while (0);

	if (rdev)
		dev_put(rdev);

	if (ret)
		dev_kfree_skb(skb);

	return NETDEV_TX_OK;
}

static int dp_eth_reinsert_subif_register(struct net_device *dev)
{
	struct dp_subif_data subif_data = {0};
	dp_subif_t *subif;
	int ret;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -1;
	}

	subif->subif = -1;
	subif_data.flag_ops |= DP_SUBIF_REINSERT;
	ret = dp_register_subif_spl_dev(0, dev, dev->name,
					subif, &subif_data, 0);
	if (ret != DP_SUCCESS) {
		netdev_err(dev, "%s: failed for device: %s ret %d\n",
			   __func__, dev->name, ret);
		ret = -1;
	} else {
		ret = 0;
	}
	kfree(subif);
	return ret;
}

static void dp_eth_reinsert_subif_unregister(struct net_device *dev)
{
	struct dp_subif_data data = {0};
	dp_subif_t *subif;
	int ret;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return;
	}

	data.flag_ops |= DP_SUBIF_REINSERT;
	ret = dp_register_subif_spl_dev(0, dev, dev->name,
					subif, &data,
					DP_F_DEREGISTER);

	if (ret != DP_SUCCESS) {
		netdev_err(dev, "%s: failed for device: %s ret %d\n",
			   __func__, dev->name, ret);
	}
	kfree(subif);
}

#if IS_ENABLED(CONFIG_QOS_TC)
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
static int dp_eth_reinsert_setup_tc(struct net_device *dev, u32 handle,
				    __be16 protocol, struct tc_to_netdev *tc)
{
	return qos_tc_setup(dev, handle, protocol, tc, -1, -1);
}
#else
static int dp_eth_reinsert_setup_tc(struct net_device *dev,
				    enum tc_setup_type type,
				    void *type_data)
{
	return qos_tc_setup(dev, type, type_data, -1, -1);
}
#endif
#endif
static const struct net_device_ops dp_eth_reinsert_netdev_ops = {
	.ndo_open            = dp_eth_reinsert_open,
	.ndo_stop            = dp_eth_reinsert_release,
	.ndo_start_xmit      = dp_eth_reinsert_tx,
	.ndo_set_config      = dp_eth_reinsert_config,
#if IS_ENABLED(CONFIG_QOS_TC)
	.ndo_setup_tc        = dp_eth_reinsert_setup_tc,
#endif
};

void dp_eth_reinsert_init(struct net_device *dev)
{
	struct dp_eth_reinsert_priv *priv;

	ether_setup(dev); /* assign some of the fields */
	dev->netdev_ops = &dp_eth_reinsert_netdev_ops;
	dev->flags |= IFF_NOARP;
	dev->features |= NETIF_F_HW_CSUM;
#if IS_ENABLED(CONFIG_QOS_TC)
	dev->features |= NETIF_F_HW_TC;
#endif
	dev->type = ARPHRD_VOID;

	priv = netdev_priv(dev);

	memset(priv, 0, sizeof(struct dp_eth_reinsert_priv));
}

static int dp_eth_reinsert_probe(struct platform_device *pdev)
{
	int ret = -ENOMEM;

	if (!dp_is_ready())
		return -EPROBE_DEFER;

	rndev = alloc_netdev(sizeof(struct dp_eth_reinsert_priv),
			     "ins%d", NET_NAME_UNKNOWN,
			     dp_eth_reinsert_init);
	if (!rndev)
		return ret;

	ret = register_netdev(rndev);
	if (ret)
		goto free;

	ret = dp_eth_reinsert_subif_register(rndev);
	if (ret)
		goto unregister;

	return 0;

unregister:
	unregister_netdev(rndev);
free:
	free_netdev(rndev);

	return ret;
}

static int dp_eth_reinsert_remove(struct platform_device *pdev)
{
	if (rndev) {
		dp_eth_reinsert_subif_unregister(rndev);
		unregister_netdev(rndev);
		free_netdev(rndev);
	}
	return 0;
}

static struct platform_driver dp_eth_reinsert_drv = {
	.probe = dp_eth_reinsert_probe,
	.remove = dp_eth_reinsert_remove,
	.driver = {
		.name = "dp_eth_reinsert",
		.owner = THIS_MODULE,
	},
};

static int __init dp_eth_reinsert_drv_init(void)
{
	int ret;

	ret = platform_driver_register(&dp_eth_reinsert_drv);
	if (ret)
		return ret;

	rpdev = platform_device_register_simple("dp_eth_reinsert", -1, NULL, 0);
	if (!rpdev)
		return -ENODEV;
	return 0;
}

static void __exit dp_eth_reinsert_drv_exit(void)
{
	if (rpdev)
		platform_device_del(rpdev);
	platform_driver_unregister(&dp_eth_reinsert_drv);
}

module_init(dp_eth_reinsert_drv_init);
module_exit(dp_eth_reinsert_drv_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("PON Ethernet reinsertion support");
