/******************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_mmc.h
 *
 * This is a PON Adapter header file, defining multicast control
 * abstraction.
 */

#ifndef _pon_adapter_mcc_h
#define _pon_adapter_mcc_h

#include "pon_adapter_base.h"
#include "pon_adapter_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *
 * @{
 */

/** \defgroup PON_ADAPTER_MCC PON Adapter Multicast Control (MCC)
 *
 * MCC Platform Abstraction
 * @{
 */

/** The special value of VID = 4096 is used to mark untagged packets. */
#define PA_MCC_VLAN_UNTAGGED		4096

/** The special value of VID = 4097 specifies VLAN unaware forwarding.
 *  This means that the default FID value is used for a forwarding entry.
 */
#define PA_MCC_VLAN_UNAWARE_SVID	4097

/** Packet information details.
 *  This is provided from the lower layer to support packet decoding.
 */
struct pa_mcc_pkt_info {
	/** Outer VLAN ID of the packet (prior to VLAN handling) */
	uint16_t cvid;
	/** Outer VLAN ID of the packet (after VLAN handling) */
	uint16_t svid;
	/** LAN port index, only for upstream packets */
	uint8_t port_idx;
	/** Flag for the direction of this packet */
	bool dir_us;
	/** Offset from buffer start to IP header */
	unsigned int offset_iph;
	/** Meta data, which is ignored by higher layer.
	 * It is free to use by lower layer and will be part of the information
	 * given to the pkt_send function.
	 */
	uint8_t meta_info[32];
};

/** Direction definition */
enum pa_mcc_dir {
	/** Downstream */
	PA_MCC_DIR_DS = 0,
	/** Upstream */
	PA_MCC_DIR_US = 1
};

/** Multicast IP address */
union pa_mcc_ip_addr {
	/** IPv4 Address */
	uint8_t ipv4[4];
	/** IPv6 Address */
	uint8_t ipv6[16];
};

/** Multicast Source Filter Definition*/
struct pa_mcc_src_filter {
	/** LAN Port map */
	uint32_t port_map;
	/** Multicast Source IP address */
	union pa_mcc_ip_addr sa;
};

/** PON adapter OMCI multicast control operations */
struct pa_omci_mcc_ops {
	/** MCC Lower Layer Initialization
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[out] max_ports	Maximal number of downstream (LAN) ports
	 */
	enum pon_adapter_errno (*init)(void *ll_handle, uint32_t *max_ports);

	/** MCC Lower Layer Shutdown
	 *
	 * \note Shutdown of packet handling thread will be done
	 * by calling \ref pkt_receive_cancel before.
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 */
	enum pon_adapter_errno (*shutdown)(void *ll_handle);

	/** Receive MCC packets (blocking)
	 * This is called in a thread and should return only if a packet is
	 * received.
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[out] msg		Buffer for receive packet
	 * \param[in,out] len		Length of buffer (in),
	 *				Length of received packet (out)
	 * \param[out] info		Info of packet
	 */
	enum pon_adapter_errno (*pkt_receive)
		(void *ll_handle, uint8_t *msg, uint16_t *len,
		 struct pa_mcc_pkt_info *info);

	/** Cancel packet receiving of MCC
	 * This will be called from a different thread than \ref pkt_receive
	 * during shutdown.
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 */
	enum pon_adapter_errno (*pkt_receive_cancel)(void *ll_handle);

	/** MCC packets sending
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] msg		Pointer to packet buffer
	 * \param[in] len		Length of packet to send
	 * \param[in] info		Info of packet
	 */
	enum pon_adapter_errno (*pkt_send)(void *ll_handle,
					   const uint8_t *msg,
					   const uint16_t len,
					   const struct pa_mcc_pkt_info *info);

	/** Get the forwarding ID for a specific outer VLAN ID
	 *
	 * \param[in]  ll_handle	Lower layer context pointer
	 * \param[in]  o_vid		Outer VLAN id
	 * \param[out] fid		Forwarding ID
	 */
	enum pon_adapter_errno (*fid_get)(void *ll_handle,
					  const uint16_t o_vid, uint8_t *fid);

	/** Set VLAN unaware multicast forwarding mode
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] enable		Enable/disable
	 */
	enum pon_adapter_errno (*vlan_unaware_mode_set)(void *ll_handle,
							const bool enable);

	/** Update the multicast forwarding table
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] fid		Forwarding ID
	 * \param[in] include_enable	Include a matching frame
	 * \param[in] bridge_id		Bridge ID
	 * \param[in] port_map		Port Map
	 * \param[in] da		Destination Address
	 * \param[in] sf		Source filter array
	 * \param[in] filter_size	Size of source filter array
	 */
	enum pon_adapter_errno (*fwd_update)(void *ll_handle,
					     const uint8_t fid,
					     const bool include_enable,
					     const uint16_t bridge_id,
					     const uint8_t port_map,
					     const union pa_mcc_ip_addr *da,
					     const struct pa_mcc_src_filter *sf,
					     const uint8_t filter_size);

	/** Add or update a port in multicast group
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] dir		Direction
	 * \param[in] lan_port		Lan Port
	 * \param[in] fid		Forwarding ID
	 * \param[in] ip		IP Address
	 */
	enum pon_adapter_errno (*port_add)(void *ll_handle,
					   const enum pa_mcc_dir dir,
					   const uint8_t lan_port,
					   const uint8_t fid,
					   const union pa_mcc_ip_addr *ip);

	/** Remove a port from a multicast group
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] lan_port		Lan Port
	 * \param[in] fid		Forwarding ID
	 * \param[in] ip		IP Address
	 */
	enum pon_adapter_errno (*port_remove)(void *ll_handle,
					      const uint8_t lan_port,
					      const uint8_t fid,
					      const union pa_mcc_ip_addr *ip);

	/** Get the port activity
	 *
	 * \param[in]  ll_handle	Lower layer context pointer
	 * \param[in]  lan_port		LAN port number
	 * \param[in]  fid		Forwarding ID
	 * \param[in]  ip		IP address
	 * \param[out] is_active	Activity status
	 */
	enum pon_adapter_errno (*port_activity_get)
		(void *ll_handle, const uint8_t lan_port, const uint8_t fid,
		 const union pa_mcc_ip_addr *ip, bool *is_active);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
