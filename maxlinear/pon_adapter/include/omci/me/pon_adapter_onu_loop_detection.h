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
 * ONU Loop Detection (vendor specific).
 */

#ifndef _pon_adapter_me_onu_loop_detection_h
#define _pon_adapter_me_onu_loop_detection_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_ONU_LOOP_DETECTION ONU Loop Detection
 *
 * This Managed Entity supports the detection of logical network loops at the
 * UNI interface. To avoid network confusion, packets sent out on a UNI
 * interface shall never be returned back on the same interface.
 * To detect this, special packets are sent on the UNI interface in regular
 * time intervals. The test packets are addressed at the port, using the
 * broadcast MAC address. If these packets are received back, the UNI is
 * switched off for a configurable time and the related alarm is triggered.
 * After this time has expired, the UNI is enabled again and the sending of
 * test packets commences. If the loop still exists, the UNI is switched off
 * again but no new alarm is triggered.
 *
 * This is a vendor-specific Managed Entity defined by China Telecom.
 * @{
 */

/** Callback for the Loop Detection action.
 *
 * \param[in] ctx			Callback context
 * \param[in] meid			Managed Entity identifier
 * \param[in] eth_uni_meid		PPTP ETH UNI Managed Entity identifier
 */
typedef int
(*pon_adapter_onu_loop_detected_action_t)(void *ctx,
					  const uint16_t meid,
					  const uint16_t eth_uni_meid);

/** Loop Detection operations structure */
struct pa_onu_loop_detection_ops {
	/** Create ONU Loop Detection Managed Entity.
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[in] me_id               Managed Entity identifier
	 */
	enum pon_adapter_errno (*create)(
		void *ll_handle,
		uint16_t me_id);

	/** Update ONU Loop Detection Managed Entity.
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[in] me_id               Managed Entity identifier
	 * \param[in] uni_me_id           PPTP ETH UNI Managed Entity identifier
	 * \param[in] loop_det_enable     Enable Loop Detection
	 * \param[in] uni_auto_shutdown   PPTP ETH UNI Auto Shutdown
	 */
	enum pon_adapter_errno (*update)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t uni_me_id,
		uint16_t loop_det_enable,
		uint16_t uni_auto_shutdown);

	/** Destroy ONU Loop Detection Managed Entity.
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[in] me_id               Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(
		void *ll_handle,
		uint16_t me_id);

	/** Send Loop Detection packet.
	 *
	 * \param[in] ll_handle           Lower layer context pointer
	 * \param[in] me_id               ONU Loop Detection Managed Entity
	 *                                identifier
	 * \param[in] svlan               Outer VLAN, 0 if no VLAN
	 * \param[in] cvlan               Inner VLAN, 0 if no VLAN
	 * \param[in] uni_me_id           PPTP ETH UNI Managed Entity identifier
	 */
	enum pon_adapter_errno (*packet_send)(
		void *ll_handle,
		uint16_t me_id,
		const uint16_t svlan,
		const uint16_t cvlan,
		const uint16_t uni_me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
