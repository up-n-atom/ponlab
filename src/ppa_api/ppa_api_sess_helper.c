/*******************************************************************************
 **
 ** FILE NAME	: ppa_api_sess_helper.c
 ** PROJECT	  : PPA
 ** MODULES	  : PPA API - Routing/Bridging(flow based) helper routines
 **
 ** DATE		 : 24 Feb 2015
 ** AUTHOR	   : Mahipati Deshpande
 ** DESCRIPTION  : PPA Protocol Stack Hook API Session Operation Functions
 ** COPYRIGHT : Copyright © 2020-2024 MaxLinear, Inc.
 **             Copyright (c) 2009
 **	        Lantiq Deutschland GmbH
 **	        Am Campeon 3; 85579 Neubiberg, Germany
 **
 **   For licensing information, see the file 'LICENSE' in the root folder of
 **   this software module.
 **
 ** HISTORY
 ** $Date		$Author				$Comment
 ** 24 Feb 2015  Mahipati			   The helper functions are moved from
 **									 ppa_api_session.c
 *******************************************************************************/

#include <linux/version.h>
#include <generated/autoconf.h>
#include <linux/swap.h>

#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include <net/ppa/ppa_hal_wrapper.h>
#include <net/ppa/ppa_stack_al.h>

#include "ppa_api_misc.h"
#include "ppa_api_netif.h"
#include "ppa_api_session.h"
#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
#include "ppa_api_session_limit.h"
#endif
#include "ppa_api_mib.h"
#include "ppa_api_hal_selector.h"
#include "ppa_api_tools.h"
#if defined(CONFIG_LTQ_CPU_FREQ) || defined(CONFIG_LTQ_PMCU) || defined(CONFIG_LTQ_PMCU_MODULE)
#include "ppa_api_pwm.h"
#endif
#include "ppa_api_sess_helper.h"

#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH)
#include "ppa_sae_hal.h"
#endif

#if IS_ENABLED(CONFIG_SOC_GRX500)
#include "../platform/xrx500/ltq_pae_hal.h"
#endif

/*routing session list item operation*/
static void ppa_session_list_init_item(struct uc_session_node *);

static PPA_LOCK		g_session_list_lock;
PPA_HLIST_HEAD		g_session_list_hash_table[SESSION_LIST_HASH_TABLE_SIZE];
static PPA_LOCK		g_session_bucket_lock[SESSION_LIST_HASH_TABLE_SIZE];

static PPA_MEM_CACHE	*g_session_item_cache = NULL;
uint32_t		g_session_item_max = SZ_128K;
PPA_ATOMIC		g_session_item_num;
PPA_ATOMIC		g_session_item_alloc_fail;

#define PPA_SESSION_RCU_LIST

#ifdef	PPA_SESSION_RCU_LIST
#define	PPA_SESSION_LIST_ADD			ppa_hlist_add_head_rcu
#define	PPA_SESSION_LIST_DEL			ppa_hlist_del_rcu
#define	PPA_SESSION_LIST_FOR_EACH_NODE		ppa_hlist_for_each_rcu
#define PPA_SESSION_LIST_FOR_EACH_ENTRY		ppa_hlist_for_each_entry_safe
#define PPA_SESSION_LIST_FOR_EACH_ENTRY_READ	ppa_hlist_for_each_entry_rcu
#define list_get_first_node(head)(rcu_dereference(hlist_first_rcu((head))))
#define list_get_next_node(node)(rcu_dereference(hlist_next_rcu((node))))
#else
#define PPA_SESSION_LIST_ADD			ppa_hlist_add_head
#define PPA_SESSION_LIST_DEL			ppa_hlist_del
#define PPA_SESSION_LIST_FOR_EACH_NODE		ppa_hlist_for_each
#define PPA_SESSION_LIST_FOR_EACH_ENTRY		ppa_hlist_for_each_entry_safe
#define PPA_SESSION_LIST_FOR_EACH_ENTRY_READ	ppa_hlist_for_each_entry
#define list_get_first_node(head)((head)->first)
#define list_get_next_node(node)((node)->next)
#endif

int ppa_init_session_item_max(void)
{
	int32_t ret = PPA_SUCCESS;
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	uint32_t pp_max_sess;

	ret = pp_max_sessions_get(&pp_max_sess);
	if (ret) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"Failed to get PP session count\n");
		return PPA_FAILURE;
	}
	g_session_item_max = pp_max_sess * SZ_2;
#endif
	return ret;
}

int ppa_session_list_lock_init(void)
{
	return  ppa_lock_init(&g_session_list_lock);
}

void ppa_session_list_lock_destroy(void)
{
	ppa_lock_destroy(&g_session_list_lock);
}

void ppa_session_list_lock(void)
{
	ppa_lock_get(&g_session_list_lock);
}

void ppa_session_list_unlock(void)
{
	ppa_lock_release(&g_session_list_lock);
}

int ppa_session_bucket_lock_init(void)
{
	int32_t ret = PPA_FAILURE, i = 0;

	for (i = 0; i < SESSION_LIST_HASH_TABLE_SIZE; i++)
		ret = ppa_lock_init(&g_session_bucket_lock[i]);

	/* returning result of last operation only */
	return ret;
}

void ppa_session_bucket_lock_destroy(void)
{
	int32_t i = 0;

	for (i = 0; i < SESSION_LIST_HASH_TABLE_SIZE; i++)
		ppa_lock_destroy(&g_session_bucket_lock[i]);
}

void ppa_session_bucket_lock(uint32_t idx)
{
	ppa_lock_get(&g_session_bucket_lock[idx]);
}

void ppa_session_bucket_unlock(uint32_t idx)
{
	ppa_lock_release(&g_session_bucket_lock[idx]);
}

void ppa_session_list_read_lock(void)
{
	/*
	 * If session list is RCU list, then call ppa_rcu_read_lock
	 */
#ifdef PPA_SESSION_RCU_LIST
	ppa_rcu_read_lock();
#else
	ppa_lock_get(&g_session_list_lock);
#endif
}

void ppa_session_list_read_unlock(void)
{
	/*
	 * If session list is RCU list, then call ppa_rcu_read_unlock
	 */
#ifdef PPA_SESSION_RCU_LIST
	ppa_rcu_read_unlock();
#else
	ppa_lock_release(&g_session_list_lock);
#endif
}

int32_t ppa_session_cache_shrink(void)
{
	return ppa_kmem_cache_shrink(g_session_item_cache);
}

int32_t ppa_session_cache_create(void)
{

	ppa_atomic_set(&g_session_item_num, 0);
	ppa_atomic_set(&g_session_item_alloc_fail, 0);
	return ppa_mem_cache_create( "ppa_session_item",
			sizeof(struct uc_session_node),
			&g_session_item_cache);
}

void ppa_session_cache_destroy(void)
{
	if( g_session_item_cache ) {
		if (ppa_atomic_read(&g_session_item_num) != 0)
			pr_warn("%s: possible leak on ppa_session_item cache!\n", __func__);

		ppa_mem_cache_destroy(g_session_item_cache);
		g_session_item_cache = NULL;
		ppa_atomic_set(&g_session_item_num, 0);
		ppa_atomic_set(&g_session_item_alloc_fail, 0);
	}
}

int ppa_session_list_init(void)
{
	int i;

	for(i = 0; i < SESSION_LIST_HASH_TABLE_SIZE; i ++) {
		PPA_INIT_HLIST_HEAD(&g_session_list_hash_table[i]);
	}
	return PPA_SUCCESS;
}

void ppa_session_list_free(void)
{
	PPA_HLIST_NODE *tmp;
	struct uc_session_node *p_item;
	uint32_t idx;

	for(idx = 0; idx < SESSION_LIST_HASH_TABLE_SIZE; idx ++) {
		ppa_session_bucket_lock(idx);
		PPA_SESSION_LIST_FOR_EACH_ENTRY( p_item, tmp, &g_session_list_hash_table[idx], hlist) {
			__ppa_session_delete_item(p_item);
		}
		ppa_session_bucket_unlock(idx);
	}
}

static void ppa_session_list_init_item(struct uc_session_node *p_item)
{
	ppa_memset(p_item, 0, sizeof(*p_item));
	PPA_INIT_HLIST_NODE(&p_item->hlist);
	p_item->mtu	= g_ppa_ppa_mtu;
	p_item->routing_entry	= ~0;
	p_item->tunnel_idx	= ~0;
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	INIT_LIST_HEAD(&p_item->tun_node);
#endif
}

struct uc_session_node* ppa_session_alloc_item(void)
{
	struct uc_session_node *p_item;

	if (ppa_atomic_read(&g_session_item_num) >= g_session_item_max) {
		ppa_atomic_inc(&g_session_item_alloc_fail);
		return NULL;
	}

	p_item = ppa_mem_cache_alloc(g_session_item_cache);
	if ( p_item ) {
		ppa_atomic_inc(&g_session_item_num);
		ppa_session_list_init_item(p_item);
	}
	return p_item;
}

static void ppa_session_rcu_free(struct rcu_head *rp)
{
	struct uc_session_node *p_item;

	p_item = container_of(rp, struct uc_session_node, rcu);
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	if (!list_empty(&p_item->tun_node))
		list_del_init(&p_item->tun_node);
#endif
	ppa_mem_cache_free(p_item, g_session_item_cache);
	ppa_atomic_dec(&g_session_item_num);
}

void ppa_session_list_free_item(struct uc_session_node *p_item)
{
#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER)
	if(p_item->flag2 & SESSION_FLAG2_LRO) {
		if(ppa_del_lro_entry(p_item)!=PPA_SUCCESS) {
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"lro entry delete failed\n");
		}
	}
#endif
	ppa_hsel_del_routing_session(p_item);

#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH)
#if defined(PPA_IF_MIB) && PPA_IF_MIB
	if ( (p_item->flags & SESSION_ADDED_IN_SW) ) {
		sw_del_session_mgmt_stats(p_item);
	}
#endif
#endif
	p_item->num_adds=0x0;

#ifdef PPA_SESSION_RCU_LIST
	ppa_call_rcu(&p_item->rcu, ppa_session_rcu_free);
#else
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	if (!list_empty(&p_item->tun_node))
		list_del_init(&p_item->tun_node);
#endif
	ppa_mem_cache_free(p_item, g_session_item_cache);
	ppa_atomic_dec(&g_session_item_num);
#endif
}

/*
static uint32_t ppa_session_get_index(uint32_t key)
{
	return ((u64)key* (SESSION_LIST_HASH_TABLE_SIZE)) >> 32;
}
*/

/* Must be inside write lock */
void __ppa_session_insert_item(struct uc_session_node *p_item)
{
	uint32_t idx;

	idx = ppa_session_get_index(p_item->hash);
	/*ppa_session_print(p_item);*/
	PPA_SESSION_LIST_ADD(&p_item->hlist, &g_session_list_hash_table[idx]);
}

/* Must be inside write lock */
void __ppa_session_delete_item(struct uc_session_node *p_item)
{
	PPA_SESSION_LIST_DEL(&p_item->hlist);
	/*ppa_session_print(p_item);*/
	__ppa_session_put(p_item);
}

/* delete all the sessions linked to a connectiontrack hash*/
int32_t ppa_session_del(uint32_t hash, PPA_SESSION *p_session)
{
	PPA_HLIST_NODE *tmp;
	uint32_t index;
	struct uc_session_node *p_item;
	int32_t ret = PPA_FAILURE;

	index = ppa_session_get_index(hash);
	ppa_session_bucket_lock(index);
	/* for each entry in the given index compare the session and hash */
	PPA_SESSION_LIST_FOR_EACH_ENTRY(p_item,tmp, (g_session_list_hash_table+index), hlist) {
		if(p_item->hash == hash && p_item->session == p_session ) {
			if( !ppa_atomic_inc_not_zero(&p_item->used) ) {
				continue;
			}
			/* matching entries needs to be deleted */
			__ppa_session_put(p_item);
			__ppa_session_delete_item(p_item);
			ret = PPA_SUCCESS;
		}
	}
	ppa_session_bucket_unlock(index);

	return ret;
}

uint32_t ppa_session_get_routing_count(uint16_t bf_lan, uint32_t count_flag, uint32_t hash_index)
{
	struct uc_session_node *p_item;
	uint32_t i;
	uint32_t count = 0, start_pos=0;
	uint32_t session_flag;

	if( hash_index ) {
		start_pos = hash_index -1;
	}

	if( bf_lan == 0 ) {
		session_flag = SESSION_WAN_ENTRY;
	} else if ( bf_lan == 1 ) {
		session_flag = SESSION_LAN_ENTRY;
	} else if ( bf_lan == 2 ) {
		/*non lan/wan, it means unknow session */
		ppa_session_list_read_lock();
		for(i = start_pos; i < SESSION_LIST_HASH_TABLE_SIZE; i ++) {
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"i=%d\n", i);
			PPA_SESSION_LIST_FOR_EACH_ENTRY_READ(p_item,&g_session_list_hash_table[i],hlist) {

				if( !(p_item->flags & SESSION_LAN_ENTRY) && !(p_item->flags & SESSION_WAN_ENTRY) )
					count++;

				ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"p_item=%px with index=%u count=%u\n", p_item, i, count);
			}
			if( hash_index ) break;
		}
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"ppa_get_non_lan_wan_routing_session_count=%d\n", count);
		ppa_session_list_read_unlock();
		return count;
	} else {
		critial_err("wrong bf_flab value:%u\n", bf_lan);
		return 0;
	}

	ppa_session_list_read_lock();

	for(i = start_pos; i < SESSION_LIST_HASH_TABLE_SIZE; i ++) {

		PPA_SESSION_LIST_FOR_EACH_ENTRY_READ(p_item,&g_session_list_hash_table[i],hlist) {

			if(p_item->flags & session_flag){

				if( count_flag == 0 ||
					/* get all PPA sessions with acceleratted and non-accelearted  */
					( (count_flag == SESSION_ADDED_IN_HW) && (p_item->flags & SESSION_ADDED_IN_HW) )  ||
					/*get all accelerated sessions only */
					( (count_flag == SESSION_ADDED_IN_SW) && (p_item->flags & SESSION_ADDED_IN_SW) )  ||
					/*get all software accelerated sessions only */
					( (count_flag == SESSION_NON_ACCE_MASK) && !(p_item->flags & SESSION_ADDED_IN_HW )
					&& !(p_item->flags & SESSION_ADDED_IN_SW )) ){
					count++;
					ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"ppa_session_get_routing_count=%d\n", count);
				}

			}
		}
		if( hash_index ) break;
	}

	ppa_session_list_read_unlock();
	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"ppa_session_get_routing_count=%d with count_flag=%x\n", count, count_flag);

	return count;
}
EXPORT_SYMBOL(ppa_session_get_routing_count);

/**
 * @brief PPA uc session list match and action
 * @param user defined data pointer
 * @param user defined match handler
 * @param user defined action handler
 * @return Void
 */
void ppa_test_and_session_action(void *data,
				 bool (*match)(struct uc_session_node *p_item,
					       void *data),
				 void (*action)(struct uc_session_node *p_item))
{
	uint32_t idx;
	struct uc_session_node *p_item;
	PPA_HLIST_NODE *tmp;

	for (idx = 0; idx < SESSION_LIST_HASH_TABLE_SIZE; idx++) {
		ppa_session_bucket_lock(idx);
		PPA_SESSION_LIST_FOR_EACH_ENTRY(p_item, tmp,
						&g_session_list_hash_table[idx],
						hlist) {
			if (match(p_item, data))
				action(p_item);
		}
		ppa_session_bucket_unlock(idx);
	}
}

static bool match_netif(struct uc_session_node *p_item, void *data)
{
	PPA_NETIF *netif = (PPA_NETIF *)data;

	return (ppa_is_netif_equal(p_item->rx_if, netif) ||
		ppa_is_netif_equal(p_item->br_rx_if, netif) ||
		ppa_is_netif_equal(p_item->br_tx_if, netif) ||
		ppa_is_netif_equal(p_item->tx_if, netif));
}

void ppa_session_delete_by_netif(PPA_NETIF *netif)
{
	ppa_test_and_session_action(netif, match_netif,
				    __ppa_session_delete_item);
}

static bool match_accel_flags(struct uc_session_node *p_item,
			      void *data)
{
	return (p_item->flags & (SESSION_ADDED_IN_HW | SESSION_ADDED_IN_SW));
}

#if IS_ENABLED(CONFIG_PPA_API_DIRECTCONNECT)
static bool match_subif_with_accel_flags(struct uc_session_node *p_item,
					   void *data)
{
	PPA_DP_SUBIF *subif = (PPA_DP_SUBIF *)data;

	if (!match_accel_flags(p_item, NULL))
		return false;

	return ((p_item->dest_ifid == subif->port_id) &&
		(p_item->dest_subifid == subif->subif));
}

void ppa_session_delete_by_subif(PPA_DP_SUBIF *subif)
{
	ppa_test_and_session_action(subif, match_subif_with_accel_flags,
				    __ppa_session_delete_item);
}
#endif

static bool match_mac(struct uc_session_node *p_item,
		      void *data)
{
	uint8_t *mac = (uint8_t *)data;

	return (!ppa_memcmp(p_item->pkt.dst_mac, mac,
			    sizeof(p_item->pkt.dst_mac)) ||
		!ppa_memcmp(p_item->pkt.src_mac, mac,
			    sizeof(p_item->pkt.src_mac)));
}

static bool match_mac_with_accel_flags(struct uc_session_node *p_item,
					 void *data)
{
	uint8_t *mac = (uint8_t *)data;

	if (!match_accel_flags(p_item, NULL))
		return false;

	return match_mac(p_item, mac);
}

void ppa_session_delete_by_macaddr(uint8_t *mac)
{
	ppa_test_and_session_action(mac, match_mac_with_accel_flags,
				    __ppa_session_delete_item);
}
EXPORT_SYMBOL(ppa_session_delete_by_macaddr);

void ppa_session_delete_all_by_macaddr(uint8_t *mac)
{
	ppa_test_and_session_action(mac, match_mac,
				    __ppa_session_delete_item);
}

#if IS_ENABLED(CONFIG_SOC_GRX500)
void ppa_get_src_mac_pitem_pae(PPA_BUF *ppa_buf, uint8_t *src_mac)
{
	struct uc_session_node *p_item = NULL;

	ppa_find_session_from_skb(ppa_buf, 0, &p_item);
	if (p_item != NULL) {
		ppa_memcpy(src_mac, p_item->pkt.src_mac, sizeof(p_item->pkt.src_mac));
	}
}
EXPORT_SYMBOL(ppa_get_src_mac_pitem_pae);
#endif

static bool match_ip(struct uc_session_node *p_item,
		     void *data)
{
	PPA_IPADDR *ip = (PPA_IPADDR *)data;

	return (!ppa_memcmp(&(p_item->pkt.dst_ip.ip),
			    &(ip->ip), sizeof(p_item->pkt.dst_ip.ip)) ||
		!ppa_memcmp(&(p_item->pkt.src_ip.ip),
			    &(ip->ip), sizeof(p_item->pkt.src_ip.ip)));
}

void ppa_session_delete_all_by_ip(PPA_IPADDR *ip)
{
	ppa_test_and_session_action(ip, match_ip,
				    __ppa_session_delete_item);
}

static int ppa_compare_with_tuple(PPA_TUPLE *t1, struct uc_session_node *t2)
{
	unsigned short l3num;

	l3num = (t2->flags & SESSION_IS_IPV6)?AF_INET6:AF_INET;

	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d \n" , __FUNCTION__, __LINE__);
	if(l3num==AF_INET6) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"t1->dst.protonum = %x t1.src ip= %pI6 t1.dst ip=%pI6 t1.src port=%d t1.dst port%d\n"
			, t1->dst.protonum, &t1->src.u3.in, &t1->dst.u3.in, be16_to_cpup(&t1->src.u.all), be16_to_cpup(&t1->dst.u.all));
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"node.protonum=%x node.src ip= %pI6 node.dst ip=%pI6 node.src port=%d node.dst port=%d\n"
			,  t2->pkt.ip_proto, &t2->pkt.src_ip.ip6, &t2->pkt.dst_ip.ip6, t2->pkt.src_port, t2->pkt.dst_port);
	} else {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"t1->dst.protonum = %x t1.src ip= %pI4 t1.dst ip=%pI4 t1.src port=%d t1.dst port%d\n"
			, t1->dst.protonum, &t1->src.u3.in, &t1->dst.u3.in, be16_to_cpup(&t1->src.u.all), be16_to_cpup(&t1->dst.u.all));
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"node.protonum=%x node.src ip= %pI4 node.dst ip=%pI4 node.src port=%d node.dst port=%d\n"
			,  t2->pkt.ip_proto, &t2->pkt.src_ip.ip, &t2->pkt.dst_ip.ip, t2->pkt.src_port, t2->pkt.dst_port);
	}
	return ppa_compare_connection_tuple(t1, l3num, t2->pkt.ip_proto, t2->pkt.src_port, t2->pkt.dst_port, &t2->pkt.src_ip, &t2->pkt.dst_ip);

}

/*
 * Search the session from sbk. Should be called when lock is held
 */
int __ppa_find_session_from_skb(PPA_BUF* skb, uint8_t pf, struct uc_session_node **pp_item)
{
	uint32_t hash;
	PPA_TUPLE tuple;
	struct uc_session_node *p_item;
	uint32_t index;
	int ret;
	PPA_HLIST_NODE *tmp;

	if(ppa_get_hash_from_packet(skb,pf, &hash, &tuple))
		return PPA_SESSION_NOT_ADDED;

	ret = PPA_SESSION_NOT_ADDED;

	index = ppa_session_get_index(hash);

	ppa_session_list_read_lock();
	PPA_SESSION_LIST_FOR_EACH_ENTRY(p_item,tmp, (g_session_list_hash_table+index), hlist) {

		if(p_item->hash == hash && ppa_compare_with_tuple(&tuple,p_item) ) {

			if( !ppa_atomic_inc_not_zero(&p_item->used) )
				break;

			ret = PPA_SESSION_EXISTS;
			*pp_item = p_item;
			break;
		}
	}
	ppa_session_list_read_unlock();
	return ret;
}

#if IS_ENABLED(CONFIG_PPA_EXT_PKT_LEARNING)
int32_t ppa_get_hw_hash_from_skb(PPA_BUF *skb, uint32_t* h1, uint32_t* h2, uint32_t* h3, uint8_t *rx_port)
{

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	struct pp_desc *ppdesc = NULL;

	/* Get the ingress gpid information from the skb cookie */
	ppdesc = pp_pkt_desc_get(skb);

	if(ppdesc) {
		/* Fill in the Hash information */
		*h1 = ppdesc->ud.hash_h1;
		*h2 = ppdesc->ud.hash_h2;
		*h3 = ppdesc->ud.hash_sig;
		*rx_port = ppdesc->ud.rx_port;

		return PPA_SUCCESS;
	}
#endif
	return PPA_FAILURE;
}

extern u32 ppa_hash_conntrack_raw(const struct nf_conntrack_tuple *tuple, const struct net *net);
int ppa_find_sess_frm_tuple(PPA_TUPLE *tuple, PPA_NETIF *netif, uint32_t *hash, uint32_t hwhash1, uint32_t hwhash2, struct uc_session_node **pp_item)
{
	int ret = PPA_SESSION_NOT_ADDED;
	struct uc_session_node *p_item=NULL;
	uint32_t index;

	*hash = ppa_hash_conntrack_raw(tuple, ppa_dev_net(netif));
	if(*hash) {
		index = ppa_session_get_index(*hash);

		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d hash = %x index = %d\n", __FUNCTION__, __LINE__, *hash, index);
		ppa_session_list_read_lock();
		PPA_SESSION_LIST_FOR_EACH_ENTRY_READ(p_item,
				(g_session_list_hash_table+index), hlist) {

			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d p_item->hash = %x\n", __FUNCTION__, __LINE__, p_item->hash);
			if(p_item->hash == *hash
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
				&& p_item->hwhash.h1 == hwhash1 && p_item->hwhash.h2 == hwhash2
#endif
				&& ppa_compare_with_tuple(tuple,p_item) ) {
				if( !ppa_atomic_inc_not_zero(&p_item->used) )
						break;

				ret = PPA_SESSION_EXISTS;
				*pp_item = p_item;
				break;
			}
		}
		ppa_session_list_read_unlock();
	}
	return ret;
}
#endif /*(CONFIG_PPA_EXT_PKT_LEARNING)*/

/*
 * Find the session from skb.
 */
int ppa_find_sess_frm_skb(PPA_BUF* skb, uint8_t pf, uint32_t *hash, uint32_t hwhash1, uint32_t hwhash2, struct uc_session_node **pp_item)
{
	PPA_TUPLE tuple;
	struct uc_session_node *p_item=NULL;
	uint32_t index;
	int ret;

	if(ppa_get_hash_from_packet(skb,pf, hash, &tuple))
		return PPA_SESSION_NOT_ADDED;

	ret = PPA_SESSION_NOT_ADDED;

	index = ppa_session_get_index(*hash);

	ppa_session_list_read_lock();
	PPA_SESSION_LIST_FOR_EACH_ENTRY_READ(p_item,
			(g_session_list_hash_table+index), hlist) {

		if(p_item->hash == *hash
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
			&& p_item->hwhash.h1 == hwhash1 && p_item->hwhash.h2 == hwhash2
#endif
			&& ppa_compare_with_tuple(&tuple,p_item) ) {
			if( !ppa_atomic_inc_not_zero(&p_item->used) )
				break;

			ret = PPA_SESSION_EXISTS;
			*pp_item = p_item;
			break;
		}
	}
	ppa_session_list_read_unlock();

	return ret;
}

int ppa_find_session_from_skb(PPA_BUF* skb, uint8_t pf, struct uc_session_node **pp_item)
{
	uint32_t hwhash1=0, hwhash2=0, hash=0;

#if IS_ENABLED(CONFIG_PPA_EXT_PKT_LEARNING)
	uint32_t hwhash3=0;
	uint8_t rx_port=0;
	ppa_get_hw_hash_from_skb(skb,&hwhash1, &hwhash2, &hwhash3, &rx_port);
#endif /*(CONFIG_PPA_EXT_PKT_LEARNING)*/

	return ppa_find_sess_frm_skb(skb, pf, &hash, hwhash1, hwhash2, pp_item);
}
EXPORT_SYMBOL(ppa_find_session_from_skb);

/*
 * Search the session by ct when hash is known. Must be called within lock
 */
int __ppa_session_find_ct_hash(const PPA_SESSION *p_session,
		uint32_t hash,
		struct uc_session_node **pp_item)
{
	struct uc_session_node *p_item;
	PPA_HLIST_NODE *tmp;
	uint32_t index;
	int ret = PPA_SESSION_NOT_ADDED;

	index = ppa_session_get_index(hash);

	*pp_item = NULL;
	ppa_session_bucket_lock(index);
	PPA_SESSION_LIST_FOR_EACH_ENTRY(p_item,tmp, (g_session_list_hash_table+index), hlist) {

		if(p_item->hash == hash && p_item->session == p_session ) {

			if( !ppa_atomic_inc_not_zero(&p_item->used) )
				break;

			ret = PPA_SESSION_EXISTS;
			*pp_item = p_item;
			break;
		}
	}
	ppa_session_bucket_unlock(index);

	return ret;
}

/*
 * This function searches session using hash and connection track pointer.
 * Call this function outside the PRE/POST routing hooks.
 * LOCK should be held.
 * The hash is computed using connection track's tuple
 */
int32_t __ppa_session_find_by_ct(PPA_SESSION *p_session,
		uint32_t is_reply,
		struct uc_session_node **pp_item)
{
	uint32_t hash;
	PPA_TUPLE tuple;

	if(p_session) {
		hash = ppa_get_hash_from_ct(p_session, is_reply?1:0,&tuple);
		return __ppa_session_find_ct_hash(p_session, hash, pp_item);
	}
	return PPA_SESSION_NOT_ADDED;
}

/*
 * This function searches session using hash and connection track pointer.
 * Call this function outside the PRE/POST routing hooks.
 */
int32_t ppa_session_find_by_ct(PPA_SESSION *p_session,
		uint32_t is_reply,
		struct uc_session_node **pp_item)
{
	uint32_t ret ;

	ret = __ppa_session_find_by_ct(p_session,is_reply,pp_item);

	return ret;
}

/*
 * This function searches the session using tuple from connection track
 * The tuple is taken from ct.
 */
int32_t ppa_sess_find_by_tuple( PPA_SESSION *p_session,
		uint32_t is_reply,
		uint32_t *hash, uint32_t hwhash1, uint32_t hwhash2,
		struct uc_session_node **pp_item)
{
	PPA_TUPLE tuple={0};
	struct uc_session_node *p_item;
	uint32_t index;
	int ret = PPA_SESSION_NOT_ADDED;

	if(!p_session) {
		return PPA_SESSION_NOT_ADDED;
	}

	*hash = ppa_get_hash_from_ct(p_session, is_reply?1:0,&tuple);
	index = ppa_session_get_index(*hash);

	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d hash = %x index = %d\n", __FUNCTION__, __LINE__, *hash, index);
	ppa_session_list_read_lock();
	PPA_SESSION_LIST_FOR_EACH_ENTRY_READ(p_item,
			(g_session_list_hash_table+index), hlist) {

		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d p_item->hash = %x\n", __FUNCTION__, __LINE__, p_item->hash);
		if(p_item->hash == *hash
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
			&& p_item->hwhash.h1 == hwhash1 && p_item->hwhash.h2 == hwhash2
#endif
			&& ppa_compare_with_tuple(&tuple,p_item) ) {
			if( !ppa_atomic_inc_not_zero(&p_item->used) ) {
				break;
			}

			ret = PPA_SESSION_EXISTS;
			*pp_item = p_item;
			break;
		}
	}
	ppa_session_list_read_unlock();
	return ret;
}

int32_t ppa_session_find_by_tuple( PPA_SESSION *p_session,
				uint32_t is_reply,
				struct uc_session_node **pp_item)
{
	uint32_t hash=0, hwhash1=0, hwhash2=0;

	return ppa_sess_find_by_tuple(p_session, is_reply, &hash, hwhash1, hwhash2, pp_item);
}

struct uc_session_node *__ppa_session_find_by_session(PPA_SESSION *session)
{
	uint32_t idx = 0;
	struct uc_session_node *p_item = NULL;
	PPA_HLIST_NODE *tmp;

	for (idx = 0; idx < SESSION_LIST_HASH_TABLE_SIZE; idx++) {
		PPA_SESSION_LIST_FOR_EACH_ENTRY(p_item, tmp, &g_session_list_hash_table[idx], hlist) {
			if (p_item->session == session && p_item->rx_if != NULL) {
				if (!ppa_atomic_inc_not_zero(&p_item->used))
					return NULL;

				return p_item;
			}
		}
	}

	return NULL;
}

struct uc_session_node* __ppa_session_find_by_routing_entry( uint32_t routingEntry)
{
	uint32_t idx;
	struct uc_session_node *p_item = NULL;
	PPA_HLIST_NODE *tmp;

	for(idx = 0; idx < SESSION_LIST_HASH_TABLE_SIZE; idx ++) {
		PPA_SESSION_LIST_FOR_EACH_ENTRY( p_item, tmp, &g_session_list_hash_table[idx], hlist)
		{
			if(p_item->routing_entry == routingEntry) {
				if( !ppa_atomic_inc_not_zero(&p_item->used) )
					return NULL;

				return p_item;
			}
		}
	}

	return NULL;
}

/*
 * Put the session back. If reference count is zero, then session is freed.
 * LOCK should be held
 */
void __ppa_session_put(struct uc_session_node* p_item)
{
	/*ppa_session_print(p_item);*/
	if(p_item && ppa_atomic_dec_and_test(&p_item->used) ) {
		ppa_session_list_free_item(p_item);
	}
}

/*
 * Put the session back. If reference count is zero, then session is freed.
 */
void ppa_session_put(struct uc_session_node* p_item)
{
	uint32_t idx;
	/*ppa_session_print(p_item);*/
	if(p_item && ppa_atomic_dec_and_test(&p_item->used) ) {
		idx = ppa_session_get_index(p_item->hash);
		ppa_session_bucket_lock(idx);
		ppa_session_list_free_item(p_item);
		ppa_session_bucket_unlock(idx);
	}
}
EXPORT_SYMBOL(ppa_session_put);

/*
 * Find the session that is in use in a given hash(bucket).
 */
static struct uc_session_node * ppa_session_itr_next(PPA_HLIST_NODE* node)
{
	struct uc_session_node *p_item;

	while( node != NULL ) {

		p_item = ppa_hlist_entry(node, struct uc_session_node, hlist);
		if( ppa_atomic_read(&p_item->used ) )
			return p_item;
		node = list_get_next_node(node);
	}

	return NULL;
}

/*
 * Iterate through the session list.
 * Call this function to start the iteration through list.
 * Call ppa_session_iterate_next to get /next item
 * Call ppa_session_stop_iteration to stop interation.
 * Note: Once this function is ivoked, the iteration must be stopped by calling
 * ppa_session_stop_iteration
 *
 *			  ***** NOTE *****
 * There could be need to increment the reference count in this function and also
 * in ppa_session_iterate_next. If reference count is increameted, then caller
 * should put back the session after use. OR other approach :-
 * In ppa_session_iterate_next put session(decrement the ref count) and
 * ppa_session_stop_iteration should take uc_session_node ptr and decrement ref
 * count if ptr is not null(this is required in case user breaks iteration)
 */
int32_t ppa_session_start_iteration(uint32_t *ppos, struct uc_session_node **pp_item)
{
	PPA_HLIST_NODE *node = NULL;
	int index;
	uint32_t l;

	l = *ppos + 1;

	ppa_session_list_read_lock();

	*pp_item = NULL;
	if( !ppa_is_init() ) {
		return PPA_FAILURE;
	}

	for ( index = 0; l && index < SESSION_LIST_HASH_TABLE_SIZE; index++ ) {

		PPA_SESSION_LIST_FOR_EACH_NODE(node, &g_session_list_hash_table[index]) {
			if( !--l ) break;
		}
	}

	if ( l == 0 && node ) {
		*pp_item = ppa_session_itr_next(node);
		while(((*pp_item) == NULL ) && index <  SESSION_LIST_HASH_TABLE_SIZE) {
			node = list_get_first_node(g_session_list_hash_table + index);
			*pp_item = ppa_session_itr_next(node);
			index++;
		}
	}

	if( *pp_item ) {
		(*ppos)++;
		return PPA_SUCCESS;
	}

	return PPA_FAILURE;
}
EXPORT_SYMBOL(ppa_session_start_iteration);

/*
 * Get next item from session list
 */
int32_t ppa_session_iterate_next(uint32_t *ppos, struct uc_session_node **pp_item)
{
	uint32_t index;
	struct uc_session_node *p_item=NULL;
	PPA_HLIST_NODE *node;

	if(likely(*pp_item != NULL)) {

		node = list_get_next_node(&((*pp_item)->hlist));
		p_item = ppa_session_itr_next(node);

		if( p_item == NULL ) {
			index = ppa_session_get_index((*pp_item)->hash) + 1;
			for( ;p_item == NULL && index < SESSION_LIST_HASH_TABLE_SIZE; index++ ) {
				node = list_get_first_node(g_session_list_hash_table+index);
				p_item = ppa_session_itr_next(node);
			}
		}
	}

	if( ((*pp_item) = p_item) ) {
		(*ppos)++;
		return PPA_SUCCESS;
	}

	return PPA_FAILURE;
}
EXPORT_SYMBOL(ppa_session_iterate_next);

/*
 * Call this function to stop the iteration.
 */
void ppa_session_stop_iteration(void)
{
	ppa_session_list_read_unlock();
}
EXPORT_SYMBOL(ppa_session_stop_iteration);

/*
 * This function retrieves the session in a given hash.
 */
int32_t ppa_session_get_items_in_hash(uint32_t index,
		struct uc_session_node **pp_item,
		uint32_t maxToCopy,
		uint32_t *copiedItems,
		uint32_t flag)
{
	/*note, pp_item will allocate memory for pp_item*/
	struct uc_session_node *p, *p_tmp = NULL;
	uint32_t nCopiedItems;
	uint32_t count;

	if( !copiedItems) {
		critial_err("copiedItems is NULL\n");
		return PPA_FAILURE;
	}
	*copiedItems = 0;

	if( index >= SESSION_LIST_HASH_TABLE_SIZE) {
		return PPA_INDEX_OVERFLOW;
	}
#if defined(MAX_ITEM_PER_HASH)
	count=MAX_ITEM_PER_HASH;
#else
	count = ppa_sesssion_get_count_in_hash(index);
#endif
	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "Session[%d] has %d items\n", index, count );

	if( count == 0 ) return PPA_SUCCESS;

	if( !*pp_item ) {
		/*If the buffer is not allocated yet, then allocate it*/
		p_tmp = ppa_malloc( sizeof(struct uc_session_node) * ( count + 1 ) );
		if( p_tmp == NULL ) {
			err("ppa_malloc failed to get %u bytes memory\n", (unsigned int)(sizeof(struct uc_session_node) * count));
			return PPA_ENOMEM;
		}
		ppa_memset( (void *)p_tmp, 0, sizeof(struct uc_session_node) * count);
		*pp_item = p_tmp;
	} else {
		/*buffer is preallocated already */
		p_tmp = *pp_item ;
		if( count > maxToCopy)
			count = maxToCopy;
	}

	if( count > 100 ) {
		err("Why counter=%d in one single hash index\n", count);
		count = 100;
	}

	nCopiedItems = 0;
	ppa_session_list_read_lock();
	PPA_SESSION_LIST_FOR_EACH_ENTRY_READ(p,&g_session_list_hash_table[index],hlist) {

		if( ppa_atomic_read(&p->used) ) {

			ppa_memcpy( &p_tmp[nCopiedItems], p, sizeof(struct uc_session_node) );

			/*add below codes for session management purpose from shivaji --start*/
			if( (flag & SESSION_BYTE_STAMPING) && (p->flags & SESSION_ADDED_IN_HW) ) {
				p->prev_sess_bytes = p->acc_bytes - p->prev_clear_acc_bytes;
			} else if((flag & SESSION_BYTE_STAMPING) && !(p->flags & SESSION_ADDED_IN_HW)) {
				p->prev_sess_bytes = p->host_bytes;
			}

			nCopiedItems++;
			if(nCopiedItems >= count ) break;
		}
	}
	ppa_session_list_read_unlock();

	*copiedItems = nCopiedItems;
	return PPA_SUCCESS;
}

/*
 * This function retunrs the number of sessions in a hash
 */
uint32_t ppa_sesssion_get_count_in_hash(uint32_t hashIndex)
{
	uint32_t num = 0;
	PPA_HLIST_NODE *node;

	if( hashIndex >= SESSION_LIST_HASH_TABLE_SIZE )
		return 0;

	ppa_session_list_read_lock();
	PPA_SESSION_LIST_FOR_EACH_NODE(node,g_session_list_hash_table+hashIndex) {
		num ++;
		if( num > 100 ) {
			err("Why num=%d in one single hash index\n", num);
			break;
		}
	}
	ppa_session_list_read_unlock();

	return num;
}

#if IS_ENABLED(CONFIG_SOC_GRX500)
/* Template buffer APIs */
extern void (*ppa_session_mc_destroy_tmplbuf_hook)(void* sessionAction);
extern struct session_action * (*ppa_construct_mc_template_buf_hook) (void *p_item, uint32_t dest_list);
#endif /*IS_ENABLED(CONFIG_SOC_GRX500)*/

struct session_action *ppa_session_mc_construct_tmplbuf(void *p_item, uint32_t dest_list)
{
#if IS_ENABLED(CONFIG_SOC_GRX500)
	if (ppa_construct_mc_template_buf_hook)
		return ppa_construct_mc_template_buf_hook(p_item, dest_list);
#endif /*IS_ENABLED(CONFIG_SOC_GRX500)*/

	return NULL;
}

void ppa_session_mc_destroy_tmplbuf(void *sessionAction)
{
#if IS_ENABLED(CONFIG_SOC_GRX500)
	if (sessionAction && ppa_session_mc_destroy_tmplbuf_hook)
		ppa_session_mc_destroy_tmplbuf_hook(sessionAction);
#endif /*IS_ENABLED(CONFIG_SOC_GRX500)*/
}

int32_t ppa_is_sess_accelable(struct uc_session_node *p_item)
{
	/* skip the telnet, ssh, ftps, and https sessions : tcp port 23, 22, 990 and 443 */
	if (p_item->pkt.src_port == 23 || p_item->pkt.dst_port == 23 ||
		p_item->pkt.src_port == 22 || p_item->pkt.dst_port == 22 ||
		p_item->pkt.src_port == 990 || p_item->pkt.dst_port == 990 ||
		p_item->pkt.src_port == 443 || p_item->pkt.dst_port == 443) {
			p_item->flags |= SESSION_NOT_ACCELABLE; /* to avoid hitting the complete learning path again */
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d Session cannot accelerate\n", __FUNCTION__, __LINE__);

		return PPA_FAILURE;
	}
	return PPA_SUCCESS;
}

#if IS_ENABLED(CONFIG_PPA_IPTABLE_EVENT_HANDLING)
int ppa_iptable_event(PPA_NOTIFIER_BLOCK *nb, unsigned long action, void *ptr)
{
	/* This event handler is responsible for flushing all the
	current sessions which are programmed into the hardware,
	software and removing p_items from the PPA database */

	ppa_test_and_session_action(NULL, match_accel_flags,
				    __ppa_session_delete_item);
	return PPA_NOTIFY_OK;
}
#endif
