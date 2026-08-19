// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/ip.h>

#include <net/datapath_api_gswip32.h>
#include <net/datapath_api_tx.h>
#include <net/datapath_api.h>

static int udp_cksum_wa_handler(struct sk_buff *skb, struct dp_tx_common *cmn,
		void *p)
{
	struct pmac_tx_hdr *pmac;
	struct udphdr *udph;
	u16 udp_plen;

	if (unlikely(!skb || !cmn || !cmn->pmac_len))
		return DP_TX_FN_CONTINUE;

	pmac = (struct pmac_tx_hdr *)cmn->pmac;

	if (unlikely(!pmac || !pmac->tcp_chksum))
		return DP_TX_FN_CONTINUE;

	if ((skb->ip_summed == CHECKSUM_PARTIAL) &&
	    (pmac->tcp_type == UDP_OVER_IPV4 ||
	     pmac->tcp_type == UDP_OVER_IPV6)) {
		udph = skb->encapsulation ? inner_udp_hdr(skb) : udp_hdr(skb);
		udp_plen = ntohs(udph->len) - sizeof(struct udphdr);
		if (udp_plen < 8)
			pmac->tcp_chksum = 0;
	}
	return DP_TX_FN_CONTINUE;
}

static int __init dp_tx_udp_cksum_wa_init(void)
{
	int ret;

	ret = dp_register_tx(DP_TX_UDP_CKSUM_WA, udp_cksum_wa_handler, NULL);
	pr_info("WA: %s: dp tx hook for udp cksum workaround register: %s\n",
		__func__, !ret ? "success" : "failure");

	return ret;
}

static void __exit dp_tx_udp_cksum_wa_exit(void)
{
	int ret;

	ret = dp_register_tx(DP_TX_UDP_CKSUM_WA, NULL, NULL);
	pr_info("WA: %s: dp tx hook for udp cksum workaround de-register: %s\n",
		__func__, !ret ? "success" : "failure");

	return;
}

module_init(dp_tx_udp_cksum_wa_init);
module_exit(dp_tx_udp_cksum_wa_exit);

MODULE_DESCRIPTION("dp_tx_hook udp cksum wa module");
MODULE_LICENSE("GPL");
