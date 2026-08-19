/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
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

#ifndef __MXL_VPN_H_
#define __MXL_VPN_H_

#include <net/xfrm.h>

#include <net/datapath_api_vpn.h>
#include <net/datapath_api.h>

#include "vpn_genconf.h"

/* Helper macro for genconf poll.
 * We cannot use readl_poll_timeout() directly since
 * genconf member is bitwise
 */
#define vpn_poll_timeout(cond, sleep_us, timeout_us)	\
({ \
	u32 dummy_val; \
	readl_poll_timeout(&dummy_val, dummy_val, cond, sleep_us, timeout_us); \
})

/* chiptop register offset */
#define ARC_EM6_FM_BASE 0xF8
#define ARC_EM6_NFM_BASE 0xFC
#define ARC_EM6_CR 0x184
#define ARC_EM6_SR 0x188
#define ATOM2EM6 0x600
#define EM62ATOM 0x604

struct tunnel_info {
	void *ctx[2]; /* one each for inbound/outbound */
	u32 spi;
	int id;
	u32 iv_sz;
	u16 family;
	struct net_device *tx_if;
	struct net_device *rx_if;
	enum mxl_vpn_mode mode;
	enum mxl_vpn_mode ip_mode;
	struct session_info {
		u32 on;
	} session[IPSEC_TUN_SESS_MAX];
};

struct vpn_data {
	struct device *dev;
	struct regmap *syscfg;
	struct reset_control *rst;
	struct clk *clk;
	int irq;

	const struct vpn_soc_data {
		u32 use_irq;
		enum umt_cnt_mode cnt_mode;
		u32 timeout_ms;
		u8 soc;
	} *soc_data;

	/* lock to protect tunnel/session updates */
	spinlock_t lock;

	u32 ext_id;
	struct net_device *netdev;

	struct dentry *debugfs;

	/* sram */
	struct gen_pool *sram_pool;
	dma_addr_t sram_phys;
	void __iomem *sram_virt;

	/* ddr */
	dma_addr_t ddr_phys;
	void *ddr_virt;
	u32 ddr_size;
	void *dbg_base;

	u32 umt_period_us;

	struct genconf *genconf;
	struct fw_hdr *fw_hdr;
	const struct firmware *fw;

	int sai;

	/* ring buffer for arc-em debug output */
	void *dbg_addr[DBG_BUF_COUNT];
	struct delayed_work dbg_work;

	struct mxl_vpn_ops ops;

	struct tunnel_info tunnels[IPSEC_TUN_MAX];

	/* bitmask for shared umt users */
	unsigned long umt_txin_dpid_mask[MAX_ARC_TIMER][MAX_PORT_PER_TIMER];

	u32 spl_id;
	u32 egp_id;
	u32 qid;
	u32 gpid;
	u32 subif;
	u32 dp_port;
};

struct vpn_net_data {
	struct vpn_data *priv;
};

struct vpn_sa_params {
	char *enc_algo;
	char *auth_algo;

	enum mxl_vpn_mode mode;
	enum mxl_vpn_ip_mode ip_mode;
	enum mxl_vpn_direction direction;

	u8 *key;
	u32 key_len;
	u8 *authkey;
	u32 authkeylen;
	u8 *iv;
	u8 *authkey3;
	u8 *nonce;

	u32 spi;

	void *ctx_buffer;
	void *token_buffer;
	void *cdr_buffer;

	/* returned info */
	int iv_size;
	int icv_size;
	int pad_blk_size;

	/* offset of instructions within token */
	u32 ipcsum_offs;
	u32 total_pad_offs;
	u32 crypto_offs;
	u32 hash_pad_offs;
	u32 msg_len_offs;
};

int vpn_init_debugfs(struct vpn_data *priv);
int vpn_add_xfrm_sa(struct xfrm_state *x);
void vpn_delete_xfrm_sa(struct xfrm_state *x);
void *vpn_eip197_create_sa(struct vpn_data *priv,
			   struct vpn_sa_params *params);
void vpn_eip197_delete_sa(struct vpn_data *priv, void *ctx);
#endif
