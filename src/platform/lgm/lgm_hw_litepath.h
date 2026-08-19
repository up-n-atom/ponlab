#ifndef __LGM_HW_LITEPATH_H__
#define __LGM_HW_LITEPATH_H__

/******************************************************************************
 *
 * FILE NAME	: lgm_hw_litepath.h
 * PROJECT	: LGM
 * MODULES	: PPA Hardware litepath acceleration.
 *
 * DATE		: 09 June 2022
 * DESCRIPTION	: Litepath acceleration support for LGM
 * COPYRIGHT	: Copyright © 2020-2022 MaxLinear, Inc.
 *                Copyright (c) 2014, Intel Corporation.
 *
 * For licensing information, see the file 'LICENSE' in the root folder
 * of this software module.
 *
 * Authors:	Kamal Eradath
 *		Gaurav Sharma <gsharma@maxlinear.com>
 *		Aanth Garai <agarai@maxlinear.com>
 *
 * HISTORY
 * $Date                 $Author                 $Comment
 * 29 Oct  2018          Kamal Eradath           Initial Version
 * 09 JUNE 2022          Gaurav sharma           Litepath adaptations
 *
 ******************************************************************************/

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
#define LITEPATH_HW_OFFLOAD	1
#endif /* IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)  || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH) */

/*!
 * \brief Data structure for litepath info used for session lookup.
 */
struct lp_info {
	bool is_pppoe;			/* if packet is pppoe */
	uint16_t proto;			/* l2 proto */
	uint16_t ip_proto;		/* l3 proto */
	uint8_t l3_offset;		/* ip layer offset*/
	uint8_t l4_offset;		/* transport layer offset*/
	struct dst_entry *dst;		/* route entry */
	PPA_NETIF *netif;		/* rx net device*/
	bool rxcsum_enabled;	/* rxcsum status */
	bool is_soft_lro;		/* is soft lro session */
	refcount_t refcnt;		/* ref count */
	struct sock *sock;		/* socket reference */
	uint16_t lro_sessid;		/* LRO session index */
};

/*
 * ####################################
 *              APIs
 * ####################################
 */

/*! APIs for Litepath initialization */
int32_t init_app_lp(void);
void uninit_app_lp(void);


bool is_hw_litepath_enabled(void);
uint16_t ppa_get_lp_gpid(void);
uint16_t ppa_get_lp_qid(void);
uint16_t ppa_get_lp_subif(void);
struct net_device *ppa_get_lp_dev(void);
const char *get_lp_dummy_l2_header(void);
struct sock *lpdev_sk_lookup(struct uc_session_node *p_item, int ifindex);

static inline void set_sk_hw_learnt(struct sock *sk, u8 val)
{
	sk->sk_hw_learnt = val;
}

static inline void lp_rxinfo_put(struct lp_info *lp_rxinfo)
{
	if (refcount_dec_and_test(&lp_rxinfo->refcnt)) {
		if (lp_rxinfo->dst)
			dst_release(lp_rxinfo->dst);
		if (lp_rxinfo->netif)
			ppa_put_netif(lp_rxinfo->netif);
		ppa_free(lp_rxinfo);
	}
}

/*! RFS support */
#ifdef CONFIG_RFS_ACCEL
void ppa_rfs_proc_file_create(void);
void ppa_rfs_proc_file_remove(void);
void ppa_rfs_alloc_steer_db(void);
void ppa_rfs_free_steer_db(void);
#endif /* CONFIG_RFS_ACCEL */

#endif /* __LGM_HW_LITEPATH_H__ */
