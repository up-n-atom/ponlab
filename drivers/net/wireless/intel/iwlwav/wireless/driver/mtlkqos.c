/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*
 * $Id:
 *
 * 
 *
 * Shared QoS
 *
 */
#include "mtlkinc.h"

#include "mtlkqos.h"
#include "mtlk_packets.h"
#include "mtlk_core_iface.h"
#include "mtlk_coreui.h"
#include "mtlk_df_nbuf.h"
#include "mtlk_param_db.h"

#define LOG_LOCAL_GID   GID_QOS
#define LOG_LOCAL_FID   1


static __INLINE int
_mtlk_qos_get_priority (const uint8 *dscp_table, mtlk_nbuf_t* nbuf, uint16* ppriority)
{
  uint8 dscp_field, dscp_value;
  uint8 ether_type_and_tos[4];
  uint8 *p_type_tos = nbuf->data + (MTLK_ETH_HDR_SIZE - MTLK_ETH_SIZEOF_ETHERNET_TYPE);
  struct ethhdr *ether_header = (struct ethhdr *)nbuf->data;

  ASSERT (nbuf != NULL);  
  ASSERT (nbuf->len >= MTLK_ETH_HDR_SIZE);

  ether_type_and_tos[0] = *p_type_tos++;
  ether_type_and_tos[1] = *p_type_tos++;
  ether_type_and_tos[2] = *p_type_tos++;
  ether_type_and_tos[3] = *p_type_tos++;

  switch (ntohs(ether_header->h_proto))
  {
  case MTLK_ETH_DGRAM_TYPE_IP:
    dscp_field = get_ip4_tos(ether_type_and_tos + MTLK_ETH_SIZEOF_ETHERNET_TYPE);
    break;
  case MTLK_ETH_DGRAM_TYPE_IPV6:
    dscp_field = get_ip6_tos(ether_type_and_tos + MTLK_ETH_SIZEOF_ETHERNET_TYPE);
    break;
  case ETH_P_ARP:
  case ETH_P_RARP:
    *ppriority = MTLK_WMM_ACI_VO_CLASS;
    return MTLK_ERR_OK;
  case MTLK_ETH_TYPE_VLAN_TAG:
    *ppriority = mtlk_df_buf_get_vlan_pcp(nbuf->data, mtlk_df_nbuf_get_data_length(nbuf));
    return MTLK_ERR_OK;
  default:
    /* Unknown protocol */
    *ppriority = MTLK_WMM_ACI_DEFAULT_CLASS;
    return MTLK_ERR_OK;
  }

  dscp_value = dscp_field >> 2;
  *ppriority = dscp_table[dscp_value];

  return MTLK_ERR_OK;
}

static __INLINE bool
_mtlk_scs_match_rules (mtlk_core_t *core, mtlk_nbuf_t* nbuf, uint16* ppriority)
{
  bool type10_match = false;
  u8 version, protocol, hdr_len, next, nxt_hdr, idx;
  u8 *proto_octet = NULL, *pos = NULL;
  u32 offset = 0, ext_len = 0;
  mtlk_dlist_entry_t *head;
  mtlk_dlist_entry_t *entry;
  sta_entry *sta = NULL;
  wave_scs_list_info_t *scs_rules = NULL;
  struct iphdr *iph = NULL;
  struct ipv6hdr *ip6h = NULL;
  struct ethhdr *ether_header = NULL;
  bool ret = false;
  unsigned iphdrlen;
  struct tcphdr *tcph=NULL;
  struct udphdr *udph = NULL;
  wave_ip_classifier_t tuple;

  ASSERT (nbuf != NULL);

  ether_header = (struct ethhdr *)nbuf->data;
  sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, ether_header->h_dest);
  if (sta == NULL) {
    ILOG2_V("STA not found \n");
    goto FINISH;
  }

  *ppriority = MTLK_WMM_ACI_DEFAULT_CLASS;

  switch (ntohs(ether_header->h_proto))
  {
    case ETH_P_IP:
      iph = (struct iphdr *)(nbuf->data + sizeof(struct ethhdr));
      if (!iph)
        return ret;

      iphdrlen = iph->ihl*4;
      wave_memcpy(&tuple.u.ipv4.src_ip, MTLK_IP4_ALEN, &iph->saddr, MTLK_IP4_ALEN);
      wave_memcpy(&tuple.u.ipv4.dst_ip, MTLK_IP4_ALEN, &iph->daddr, MTLK_IP4_ALEN);
      tuple.u.ipv4.protocol = iph->protocol;
      protocol = iph->protocol;
      tuple.ip_version = MTLK_IP4_VER;
      version = MTLK_IP4_VER;

      if (protocol == IPPROTO_TCP) {
        tcph = (struct tcphdr*)(nbuf->data + sizeof(struct ethhdr) + iphdrlen);
        tuple.u.ipv4.src_port = ntohs(tcph->source);
        tuple.u.ipv4.dst_port = ntohs(tcph->dest);
      } else if (protocol == IPPROTO_UDP) {
        udph = (struct udphdr *)(nbuf->data + sizeof(struct ethhdr) + iphdrlen);
        tuple.u.ipv4.src_port = ntohs(udph->source);
        tuple.u.ipv4.dst_port = ntohs(udph->dest);
      }
      break;
    case ETH_P_IPV6:
      ip6h = (struct ipv6hdr *)(nbuf->data + sizeof(struct ethhdr));
      if (!ip6h)
        return ret;

      wave_memcpy(&tuple.u.ipv6.src_ip, MTLK_IP6_ALEN, &ip6h->saddr, MTLK_IP6_ALEN);
      wave_memcpy(&tuple.u.ipv6.dst_ip, MTLK_IP6_ALEN, &ip6h->daddr, MTLK_IP6_ALEN);
      tuple.u.ipv6.next_header = ip6h->nexthdr;
      protocol = ip6h->nexthdr;
      tuple.ip_version = MTLK_IP6_VER;
      version = MTLK_IP6_VER;

      if (protocol == IPPROTO_TCP) {
        tcph = (struct tcphdr*)(nbuf->data + sizeof(struct ethhdr) + sizeof(struct ipv6hdr));
        tuple.u.ipv6.src_port = ntohs(tcph->source);
        tuple.u.ipv6.dst_port = ntohs(tcph->dest);
      } else if (protocol == IPPROTO_UDP) {
        udph = (struct udphdr *)(nbuf->data + sizeof(struct ethhdr) + sizeof(struct ipv6hdr));
        tuple.u.ipv6.src_port = ntohs(udph->source);
        tuple.u.ipv6.dst_port = ntohs(udph->dest);
      }
      break;
    default:
      return ret;
  }

  mtlk_osal_lock_acquire(&sta->scs_db.scs_list_lock);
  if (mtlk_dlist_is_empty(&sta->scs_db.scs_list))
  {
     ILOG2_V("SCS session not created for the STA\n");
     ret = false;
     goto DONE;
  }

  mtlk_dlist_foreach(&sta->scs_db.scs_list, entry, head)
  {
    scs_rules = MTLK_LIST_GET_CONTAINING_RECORD(entry, wave_scs_list_info_t, lentry);
    if (!(mtlk_compare_ip_tuple(scs_rules->tclas_info.tclass_mask, tuple, scs_rules->ip_tuple)))
      continue;

    if (scs_rules->tclas_info.tclass_type == FRAME_CLASSIFIER_TYPE10) {
      type10_match = true;

      if ((protocol != IPPROTO_UDP && protocol != IPPROTO_ESP) &&
           (protocol != scs_rules->clas_type10.proto_num))
        continue;

      if (version == MTLK_IP4_VER) { //for ipv4, class10
        proto_octet = (u8*)iph + (iph->ihl * 4); //iph->ihl * 4 - ipv4 header len, include options field
      } else if (version == MTLK_IP6_VER) { //for ipv6, class10
        offset = sizeof(struct ipv6hdr); //points to the start of the next header field of the ipv6 frame(~40bytes)
        nxt_hdr = ip6h->nexthdr;

        while (offset < nbuf->len) {
          pos = (u8*)ip6h + offset;
          if (nxt_hdr == scs_rules->ip_tuple.u.ipv6.next_header) {
            proto_octet = pos;
            break;
          }
          next = pos[0]; //read next header from the current ext header
          hdr_len = pos[1];
          ext_len = (hdr_len + 1) * 8; //total length of current ext header

          nxt_hdr = next;
          offset += ext_len;
        }
      }

      if (proto_octet) {//Compare protocol octets with filter value for both ipv6 and ipv4
        for (idx = 0; idx < scs_rules->clas_type10.filter_len; idx++)
        {
           if ((proto_octet[idx] & scs_rules->clas_type10.filter_mask[idx]) !=
                (scs_rules->clas_type10.filter_val[idx] & scs_rules->clas_type10.filter_mask[idx]))
           {
             type10_match = false;
             break;
           }
         }
       }

       if (type10_match) {
         *ppriority = scs_rules->req_up;
         ret = true;
         goto DONE;
       }
    } else if (scs_rules->tclas_info.tclass_type == FRAME_CLASSIFIER_TYPE4) {
      *ppriority = scs_rules->req_up;
      ret = true;
      goto DONE;
    }
  }
DONE:
  mtlk_osal_lock_release(&sta->scs_db.scs_list_lock);
FINISH:
  if (sta) {
    mtlk_sta_decref(sta);
  }
  return ret;
}

bool __MTLK_IFUNC mtlk_extract_ip_tuple (mtlk_nbuf_t *nbuf, sta_entry *sta, wave_ip_classifier_t *tuple)
{

  bool ret = true;
  struct ethhdr *ether_header = NULL;
  unsigned iph_len;
  struct iphdr *iph = NULL;
  struct ipv6hdr *ip6h = NULL;
  struct tcphdr *tcph = NULL;
  struct udphdr *udph = NULL;

  ASSERT (nbuf != NULL || sta!= NULL);

  ether_header = (struct ethhdr *)nbuf->data;

  switch (ntohs(ether_header->h_proto))
  {
    case ETH_P_IP:
      iph = (struct iphdr *)(nbuf->data + sizeof(struct ethhdr));
      iph_len = iph->ihl*4;
      wave_memcpy(&tuple->u.ipv4.src_ip, MTLK_IP4_ALEN, &iph->saddr, MTLK_IP4_ALEN);
      wave_memcpy(&tuple->u.ipv4.dst_ip, MTLK_IP4_ALEN, &iph->daddr, MTLK_IP4_ALEN);
      tuple->u.ipv4.protocol = iph->protocol;
      tuple->ip_version = MTLK_IP4_VER;
      tuple->u.ipv4.dscp = (iph->tos >> 2) & 0x3f;

      if (tuple->u.ipv4.protocol == IPPROTO_TCP) {
        tcph = (struct tcphdr*)(nbuf->data + sizeof(struct ethhdr) + iph_len);
        tuple->u.ipv4.src_port = ntohs(tcph->source);
        tuple->u.ipv4.dst_port = ntohs(tcph->dest);
      } else if (tuple->u.ipv4.protocol == IPPROTO_UDP) {
        udph = (struct udphdr *)(nbuf->data + sizeof(struct ethhdr) + iph_len);
        tuple->u.ipv4.src_port = ntohs(udph->source);
        tuple->u.ipv4.dst_port = ntohs(udph->dest);
      }
      break;
    case ETH_P_IPV6:
      ip6h = (struct ipv6hdr *)(nbuf->data + sizeof(struct ethhdr));
      wave_memcpy(&tuple->u.ipv6.src_ip, MTLK_IP6_ALEN, &ip6h->saddr, MTLK_IP6_ALEN);
      wave_memcpy(&tuple->u.ipv6.dst_ip, MTLK_IP6_ALEN, &ip6h->daddr, MTLK_IP6_ALEN);
      tuple->u.ipv6.next_header = ip6h->nexthdr;
      tuple->ip_version = MTLK_IP6_VER;
      tuple->u.ipv6.dscp = (ntohl(*(__be32 *)ip6h) >> 22) & 0x3f;

      if (tuple->u.ipv6.next_header == IPPROTO_TCP) {
        tcph = (struct tcphdr*)(nbuf->data + sizeof(struct ethhdr) + sizeof(struct ipv6hdr));
        tuple->u.ipv6.src_port = ntohs(tcph->source);
        tuple->u.ipv6.dst_port = ntohs(tcph->dest);
      } else if (tuple->u.ipv6.next_header == IPPROTO_UDP) {
        udph = (struct udphdr *)(nbuf->data + sizeof(struct ethhdr) + sizeof(struct ipv6hdr));
        tuple->u.ipv6.src_port = ntohs(udph->source);
        tuple->u.ipv6.dst_port = ntohs(udph->dest);
      }
      break;
    default:
      ret = false;
  }

  return ret;
}

bool __MTLK_IFUNC
mtlk_compare_ip_tuple(u8 mask_bitmap, wave_ip_classifier_t tuple1, wave_ip_classifier_t tuple2)
{
  if ((mask_bitmap & BIT(CLASS_TYPE4_VERSION)) &&
      (tuple1.ip_version != tuple2.ip_version))
    return false;

   if (mask_bitmap & BIT(CLASS_TYPE4_SRC_IP)) {
     if (tuple1.ip_version == MTLK_IP4_VER) {
       if (memcmp(&tuple1.u.ipv4.src_ip, &tuple2.u.ipv4.src_ip, MTLK_IP4_ALEN) !=0)
         return false;
     } else if (tuple1.ip_version == MTLK_IP6_VER) {
       if (memcmp(&tuple1.u.ipv6.src_ip, &tuple2.u.ipv6.src_ip, MTLK_IP6_ALEN) !=0)
         return false;
     }
   }

   if (mask_bitmap & BIT(CLASS_TYPE4_DST_IP)) {
     if (tuple1.ip_version == MTLK_IP4_VER) {
       if (memcmp(&tuple1.u.ipv4.dst_ip, &tuple2.u.ipv4.dst_ip, MTLK_IP4_ALEN) !=0)
         return false;
     } else if (tuple1.ip_version == MTLK_IP6_VER) {
       if (memcmp(&tuple1.u.ipv6.dst_ip, &tuple2.u.ipv6.dst_ip, MTLK_IP6_ALEN) !=0)
         return false;
     }
   }

   if (mask_bitmap & BIT(CLASS_TYPE4_SRC_PORT)) {
     if (tuple1.ip_version == MTLK_IP4_VER) {
       if (tuple1.u.ipv4.src_port != tuple2.u.ipv4.src_port)
         return false;
     } else if (tuple1.ip_version == MTLK_IP6_VER) {
       if (tuple1.u.ipv6.src_port != tuple2.u.ipv6.src_port)
         return false;
     }
   }

   if (mask_bitmap & BIT(CLASS_TYPE4_DST_PORT)) {
     if (tuple1.ip_version == MTLK_IP4_VER) {
       if (tuple1.u.ipv4.dst_port != tuple2.u.ipv4.dst_port)
         return false;
     } else if (tuple1.ip_version == MTLK_IP6_VER) {
       if (tuple1.u.ipv6.dst_port != tuple2.u.ipv6.dst_port)
         return false;
     }
   }

   if (mask_bitmap & BIT(CLASS_TYPE4_DSCP)) {
     if (tuple1.ip_version == MTLK_IP4_VER) {
       if (tuple1.u.ipv4.dscp != tuple2.u.ipv4.dscp)
         return false;
     } else if (tuple1.ip_version == MTLK_IP6_VER) {
        if (tuple1.u.ipv6.dscp != tuple2.u.ipv6.dscp)
          return false;
     }
   }

   if (mask_bitmap & BIT(CLASS_TYPE4_PROTO)) {
     if (tuple1.ip_version == MTLK_IP4_VER) {
       if (tuple1.u.ipv4.protocol != tuple2.u.ipv4.protocol)
         return false;
     } else if (tuple1.ip_version == MTLK_IP6_VER) {
        if (tuple1.u.ipv6.next_header != tuple2.u.ipv6.next_header)
          return false;
     }
   }

   if (mask_bitmap & BIT(CLASS_TYPE4_FLOW_LABEL)) {
     if (tuple1.ip_version != MTLK_IP6_VER)
       return false;
      if (memcmp(tuple1.u.ipv6.flow_label, tuple2.u.ipv6.flow_label, MTLK_IP6_FLOW_HDR_SIZE) != 0)
        return false;
    }

   return true;
}

static __INLINE bool
_mtlk_mscs_process_dl_pkt (mtlk_core_t *core, mtlk_nbuf_t* nbuf, uint16* ppriority)
{
  bool ret = false;
  bool mscs_tuple_match = false;
  u64 current_time = 0;
  struct ethhdr *ether_header = NULL;
  mtlk_dlist_entry_t *head;
  mtlk_dlist_entry_t *entry;
  wave_mscs_list_info_t *mscs_entry = NULL;
  sta_entry *sta = NULL;
  wave_ip_classifier_t ip_tuple;

  ASSERT (nbuf != NULL);

  ether_header = (struct ethhdr *)nbuf->data;
  sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, ether_header->h_dest);
  if (sta == NULL) {
    ILOG2_V("STA not found \n");
    goto FINISH;
  }

  mtlk_osal_lock_acquire(&sta->mscs_db.mscs_list_lock);
  if (!sta->mscs_db.mscs_active) {
    ILOG3_V("MSCS is not active for STA \n");
    goto DONE;
  }

  if (!(mtlk_extract_ip_tuple(nbuf, sta, &ip_tuple))) {
    goto DONE;
  }

  if (mtlk_dlist_is_empty(&sta->mscs_db.mscs_list)) {
     ILOG2_V("MSCS session not created for the STA\n");
     goto DONE;
  }

  mtlk_dlist_foreach(&sta->mscs_db.mscs_list, entry, head) {
    mscs_entry = MTLK_LIST_GET_CONTAINING_RECORD(entry, wave_mscs_list_info_t, lentry);
    if (!(mtlk_compare_ip_tuple(sta->mscs_db.class_mask, ip_tuple, mscs_entry->ip_tuple)))
      continue;
    current_time = ktime_get_mono_fast_ns() / NSEC_PER_MSEC;
    if (current_time - mscs_entry->timestamp <= sta->mscs_db.stream_timeout_ms) {
      *ppriority = MIN(mscs_entry->user_priority, sta->mscs_db.user_priority_limit);
      mscs_tuple_match = true;
      ret = true;
      break;
    } else { /* free up the tuple */
      mtlk_dlist_remove(&sta->mscs_db.mscs_list, entry);
      mtlk_osal_mem_free(mscs_entry);
      break;
    }
  }
DONE:
  mtlk_osal_lock_release(&sta->mscs_db.mscs_list_lock);
FINISH:
  if (sta) {
    mtlk_sta_decref(sta);
  }
  return ret;
}

void __MTLK_IFUNC
mtlk_qos_adjust_priority(mtlk_core_t *core, mtlk_nbuf_t *nbuf)
{
  uint16 priority;
  uint8 scs_enable = 0;
  uint8 mscs_enable = 0;
  /* TODO: WLANRTSYS-88975, thr priority precedence i.e QOS > SCS will
   * carried out in the next phase. The SCS rules match will be carried
   * out only if enable scs set. This is flag will be set in hostapd.conf */
  scs_enable = MTLK_CORE_PDB_GET_INT(core, PARAM_DB_CORE_SCS_ENABLE);
  mscs_enable = MTLK_CORE_PDB_GET_INT(core, PARAM_DB_CORE_MSCS_ENABLE);

  if (scs_enable) {
    if (!_mtlk_scs_match_rules(core, nbuf, &priority) &&
	     (!mscs_enable || !_mtlk_mscs_process_dl_pkt(core, nbuf, &priority))) {
      _mtlk_qos_get_priority(core->dscp_table, nbuf, &priority);
    }
  } else if(mscs_enable) {
    if (!(_mtlk_mscs_process_dl_pkt(core, nbuf, &priority)))
       _mtlk_qos_get_priority(core->dscp_table, nbuf, &priority);
  } else
    /* Get priority from DSCP bits (IP priority) or PCP from vlan header*/
    _mtlk_qos_get_priority(core->dscp_table, nbuf, &priority);
 
  /* update packet's priority */
  mtlk_df_nbuf_set_priority(nbuf, priority);
}

void __MTLK_IFUNC mtlk_qos_dscp_table_init (uint8 *dscp_table)
{
  uint32 up;

  for (up = 0; up < NTS_TIDS; up++)
  {
    memset(&dscp_table[up * DSCP_PER_UP], up, DSCP_PER_UP);
  }
}
