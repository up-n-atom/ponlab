/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Ethernet PMHD.
 */

#ifndef _pon_adapter_me_ethernet_pmhd
#define _pon_adapter_me_ethernet_pmhd

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 *
 */

/** \defgroup PON_ADAPTER_ME_ETHERNET_PMHD Ethernet PMHD
 *
 * This Managed Entity collects some of the performance monitoring data for an
 * Ethernet interface.
 *
 * Instances of this Managed Entity are created and deleted by the OLT.
 *
 * @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the physical path termination point Ethernet UNI. */

/** Ethernet Extended PMHD Counters Direction. */
enum pon_adapter_eth_ext_pmhd_dir {
	/** Upstream counter */
	PON_ADAPTER_ETH_EXT_PMHD_DIR_US = 1,
	/** Downstream counter */
	PON_ADAPTER_ETH_EXT_PMHD_DIR_DS = 2
};

/** Ethernet counter structure for the Ethernet Extended PMHD Counters. */
struct pa_eth_ext_cnt {
	/** Drop events.
	 * The total number of events in which frames were dropped due to lack
	 * of resources. This is not necessarily the number of frames dropped
	 * it is the number of times this event was detected.
	 */
	uint64_t drop_events;
	/** Octets.
	 *  The total number of octets received, including those in bad frames,
	 *  excluding framing bits, but including FCS.
	 */
	uint64_t octets;
	/** Frames.
	 *  The total number of frames received, including bad frames, broadcast
	 *  frames and multicast frames.
	 */
	uint64_t frames;
	/** Broadcast frames.
	 *  The total number of received good frames directed to the broadcast
	 *  address. This does not include multicast frames.
	 */
	uint64_t bc_frames;
	/** Multicast frames.
	 *  The total number of received good frames directed to a multicast
	 *  address. This does not include broadcast frames.
	 */
	uint64_t mc_frames;
	/** CRC errored frames.
	 *  The total number of frames received that had a length (excluding
	 *  framing bits, but including FCS octets) of between 64 and
	 *  1518 octets, inclusive, but had either a bad frame check
	 *  sequence (FCS) with an integral number of octets (FCS error) or a
	 *  bad FCS with a non-integral number of octets (alignment error).
	 */
	uint64_t crc_err_frames;
	/** Undersize frames.
	 *  The total number of frames received that were less than 64 octets
	 *  long but were otherwise well formed (excluding framing bits, but
	 *  including FCS octets).
	 */
	uint64_t usize_frames;
	/** Oversize frames.
	 *  The total number of frames received that were longer
	 *  than 1518 octets (excluding framing bits, but including FCS octets)
	 *  and were otherwise well formed.
	 */
	uint64_t osize_frames;
	/** Frames 64 octets.
	 *  The total number of received frames (including bad frames) that were
	 *  64 octets long, excluding framing bits but including FCS.
	 */
	uint64_t frames_64;
	/** Frames 65 to 127 octets.
	 *  The total number of received frames (including bad frames) that
	 *  were 65 to 127 octets long, excluding framing bits but including
	 *  FCS.
	 */
	uint64_t frames_65_127;
	/** Frames 128 to 255 octets.
	 *  The total number of received frames (including bad frames) that
	 *  were 128 to 255 octets long, excluding framing bits but including
	 *  FCS.
	 */
	uint64_t frames_128_255;
	/** Frames 256 to 511 octets.
	 *  The total number of received frames (including bad frames) that
	 *  were 256 to 511 octets long, excluding framing bits but including
	 *  FCS.
	 */
	uint64_t frames_256_511;
	/** Frames 512 to 1023 octets.
	 *  The total number of received frames (including bad frames) that
	 *  were 512 to 1023 octets long, excluding framing bits but including
	 *  FCS.
	 */
	uint64_t frames_512_1023;
	/** Frames 1024 to 1518 octets.
	 *  The total number of received frames (including bad frames) that
	 *  were 1024 to 1518 octets long, excluding framing bits but including
	 *  FCS.
	 */
	uint64_t frames_1024_1518;
};

/** Ethernet counter structure for the Ethernet PMHD Managed Entity */
struct pa_eth_cnt {
	/** FCS errors */
	uint64_t cnt_fcs_error;
	/** Excessive collision.counter */
	uint64_t cnt_excessive_collisions;
	/** Late collision.counter */
	uint64_t cnt_late_collisions;
	/** Frames too long */
	uint64_t cnt_frames_too_long;
	/** Buffer overflows on receive */
	uint64_t cnt_rx_buffer_overflow_events;
	/** Buffer overflows on transmit */
	uint64_t cnt_tx_buffer_overflow_events;
	/** Single collision frame counter */
	uint64_t cnt_single_collisions;
	/** Multiple collisions frame counter */
	uint64_t cnt_multiple_collisions;
	/** SQE counter */
	uint64_t cnt_sqe_test;
	/** Deferred transmission counter */
	uint64_t cnt_deferred_transmissions;
	/** Internal MAC transmit error counter */
	uint64_t cnt_tx_mac_errors;
	/** Carrier sense error counter */
	uint64_t cnt_carrier_sense_errors;
	/** Alignment error counter */
	uint64_t cnt_alignment_error;
	/** Internal MAC receive error counter */
	uint64_t cnt_rx_mac_errors;
};

/** Ethernet counter structure for the Ethernet Frame PMHD DS and
 *  Ethernet Frame PMHD US Managed Entities
 */
struct pa_eth_ds_us_cnt {
	/** Drop events */
	uint64_t drop_events;
	/** Octets */
	uint64_t octets;
	/** Packets */
	uint64_t packets;
	/** Broadcast packets */
	uint64_t broadcast_packets;
	/** Multicast packets */
	uint64_t multicast_packets;
	/** CRC errored packets */
	uint64_t crc_errored_packets;
	/** Undersize packets */
	uint64_t undersized_packets;
	/** Oversize packets */
	uint64_t oversized_packets;
	/** Packets 64 octets */
	uint64_t cnt64_octets_packets;
	/** Packets 65 to 127 octets */
	uint64_t cnt127_octets_packets;
	/** Packets 128 to 255 octets */
	uint64_t cnt255_octets_packets;
	/** Packets 256 to 511.octets */
	uint64_t cnt511_octets_packets;
	/** Packets 512 to 1023 octets.*/
	uint64_t cnt1023_octets_packets;
	/** Packets 1024 to 1518 octets */
	uint64_t cnt1518_octets_packets;
};

/** Ethernet counter structure for the Ethernet PMHD 3 Managed Entity */
struct pa_eth3_cnt {
	/** Drop events */
	uint64_t cnt_drop_events;
	/** Octets */
	uint64_t cnt_octets;
	/** Packets */
	uint64_t cnt_packets;
	/** Broadcast packets */
	uint64_t cnt_broadcast_packets;
	/** Multicast packets */
	uint64_t cnt_multicast_packets;
	/** Undersize packets */
	uint64_t cnt_undersized_packets;
	/** Fragments */
	uint64_t cnt_fragments;
	/** Jabbers */
	uint64_t cnt_jabbers;
	/** Packets 64 octets */
	uint64_t cnt64_octets_packets;
	/** Packets 65 to 127 octets */
	uint64_t cnt127_octets_packets;
	/** Packets 128 to 255 octets */
	uint64_t cnt255_octets_packets;
	/** Packets 256 to 511.octets */
	uint64_t cnt511_octets_packets;
	/** Packets 512 to 1023 octets */
	uint64_t cnt1023_octets_packets;
	/** Packets 1024 to 1518 octets */
	uint64_t cnt1518_octets_packets;
};

/** Ethernet PMHD attribute handling operations structure */
struct pa_eth_pmhd_ops {
	/** Retrieve total counters for Ethernet PMHD
	 *
	 * \param[in]   ll_handle  Lower layer context pointer
	 * \param[in]   me_id      Managed Entity identifier
	 * \param[out]  eth_cnt    Pointer to eth_cnt structure
	 */
	enum pon_adapter_errno (*eth_cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		struct pa_eth_cnt *eth_cnt);

	/** Retrieve total counters for Ethernet PMHD2
	 *
	 * \param[in]  ll_handle		  Lower layer context pointer
	 * \param[in]  me_id			  Managed Entity identifier
	 * \param[out] cnt_pppoe_filtered_frames  PPPoE filtering counter
	 */
	enum pon_adapter_errno (*eth2_cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		uint64_t *cnt_pppoe_filtered_frames);

	/** Retrieve total counters for Ethernet PMHD3
	 *
	 * \param[in]   ll_handle  Lower layer context pointer
	 * \param[in]   me_id      Managed Entity identifier
	 * \param[out]  eth3_cnt   Pointer to Ethernet PMHD3 counters
	 */
	enum pon_adapter_errno (*eth3_cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		struct pa_eth3_cnt *eth3_cnt);

	/** Retrieve current values of Ethernet Extended PMHD total counters
	 *
	 * \param[in]  ll_handle                Lower layer context pointer
	 * \param[in]  parent_class_id          Parent Managed Entity class
	 *                                      identifier
	 * \param[in]  parent_me_id             Parent Managed Entity instance
	 *                                      identifier
	 * \param[in]  dir                      Counters direction
	 * \param[out] eth_ext_cnt              Counters values
	 */
	enum pon_adapter_errno (*eth_ext_cnt_get)(
		void *ll_handle,
		const uint16_t parent_class_id,
		const uint16_t parent_me_id,
		const enum pon_adapter_eth_ext_pmhd_dir dir,
		struct pa_eth_ext_cnt *eth_ext_cnt);

	/** Retrieve total counters for Ethernet Frame PMHD Upstream
	 *
	 * \param[in]   ll_handle    Lower layer context pointer
	 * \param[in]   me_id        Managed Entity identifier
	 * \param[out]  eth_us_cnt   Ethernet US PMHD counters data
	 */
	enum pon_adapter_errno (*eth_us_cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		struct pa_eth_ds_us_cnt *eth_us_cnt);

	/** Retrieve total counters for Ethernet Frame PMHD Downstream
	 *
	 * \param[in]   ll_handle   Lower layer context pointer
	 * \param[in]   me_id       Managed Entity identifier
	 * \param[out]  eth_ds_cnt  Pointer to eth_cnt_ds data
	 */
	enum pon_adapter_errno (*eth_ds_cnt_get)(
		void *ll_handle,
		uint16_t me_id,
		struct pa_eth_ds_us_cnt *eth_ds_cnt);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
