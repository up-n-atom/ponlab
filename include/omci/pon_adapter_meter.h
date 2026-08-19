/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_meter.h
 *
 * This is a PON Adapter header file, defining traffic metering/policing
 * functions.
 */

#ifndef _PON_ADAPTER_METER_H_
#define _PON_ADAPTER_METER_H_

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *   @{
 */

/** \defgroup PON_ADAPTER_METER Traffic Metering and Policing
 *
 * These functions are used for measurement and control of data rates by
 * policing.
 *
 * @{
 */

/** Structure to specify a meter/policer */
struct pon_adapter_meter {
	/** Identifier */
	uint32_t id;
	/** Enable flag. */
	bool enable;
};

/** Structure to specify a meter/policer configuration */
struct pon_adapter_meter_cfg {
	/** Committed information rate (byte/s) */
	uint32_t cir;
	/** Peak information rate (byte/s) */
	uint32_t pir;
	/** Committed burst size (byte) */
	uint32_t cbs;
	/** Peak burst size (byte) */
	uint32_t pbs;
	/** Coloring mode
	 * - 0: UNAWARE, color-unaware metering/policing.
	 * - 1: AWARE, color-aware metering/policing.
	 */
	uint8_t color_aware;
	/** Meter type selection.
	 * - 0: NONE, No meter mode given, this will use the RFC 2698 policy.
	 * - 1: RFC2698, Use the RFC 2698 policy.
	 * - 2: RFC4115, Use the RFC 4115 policy.
	 */
	uint8_t mode;
};

/** Policer operations structure for OMCI low-level meter (policer) operations
 */
struct pa_omci_meter_ops {
	/** Create a policer for unknown unicast and multicast traffic (UXC).
	 *
	 *  \param[in]     ll_handle  Lower layer context pointer
	 *  \param[in]     uxc_meter  Meter configuration
	 */
	enum pon_adapter_errno (*uxc_create)(void *ll_handle,
			const struct pon_adapter_meter_cfg *uxc_meter);
	/** Delete a policer for unknown unicast and multicast traffic (UXC).
	 *
	 *  \param[in]     ll_handle  Lower layer context pointer
	 */
	enum pon_adapter_errno (*uxc_delete)(void *ll_handle);
	/* TODO: Remove this function later */
	/** Get policer data for unknown unicast and multicast traffic (UXC).
	 *
	 *  \param[in]     ll_handle  Lower layer context pointer
	 *  \param[in]     uxc_meter  Meter
	 */
	enum pon_adapter_errno (*uxc_get)(void *ll_handle,
					  struct pon_adapter_meter *uxc_meter);
	/** Update the global ANI exception meter configuration.
	 *
	 *  \param[in]     ll_handle  Lower layer context pointer
	 *  \param[in]     rate       Meter rate, packets/sec
	 */
	enum pon_adapter_errno (*ani_exc_update)(void *ll_handle,
						 const uint32_t rate);
	/** Reset the global ANI exception meter configuration.
	 *
	 *  \param[in]     ll_handle  Lower layer context pointer
	 */
	enum pon_adapter_errno (*ani_exc_reset)(void *ll_handle);
};

/** @} */ /* PON_ADAPTER_METER */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
