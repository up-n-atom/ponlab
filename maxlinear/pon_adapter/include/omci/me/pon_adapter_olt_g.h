/******************************************************************************
 *
 *  Copyright (c) 2023 MaxLinear, Inc.
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
 * OLT-G.
 */

#ifndef _pon_adapter_me_olt_g_h
#define _pon_adapter_me_olt_g_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_OLT_G OLT-G
 *
 *  This Managed Entity identifies the OLT to which an ONU is
 *  connected and provides the configuration for precise timing synchronization.
 *  It provides a way for the ONU to configure itself for operability with
 *  a particular OLT.
 *
 *  An ONU that supports this Managed Entity automatically creates an instance
 *  of it. Immediately following the startup phase, the OLT should set the ONU
 *  to the desired configuration. Interpretation of the attributes is a matter
 *  for negotiation between the two vendors involved.
 *
 *  @{
 */

/** OLT-G update structure */
struct pa_olt_g_update_data {
	/** Time of day information, byte 0 to 3 */
	uint32_t multiframe_count;
	/** Time of day information, byte 4 to 5 */
	uint32_t tod_extended_seconds;
	/** Time of day information, byte 6 to 9 */
	uint32_t tod_seconds;
	/** Time of day information, byte 10 to 13 */
	uint32_t tod_nano_seconds;
	/** OLT Vendor ID like provided by OLT.
	 * This is not a string although it contains typically character
	 * values, but misses the terminator \0 */
	uint8_t olt_vendor_id[4];
	/** OLT Equipment ID like provided by OLT.
	 * Note that this is not a string. */
	uint8_t olt_equipment_id[20];
	/** OLT Version like provided by OLT.
	 * Note that this is not a string. */
	uint8_t olt_version[14];
};

/** OLT-G operations structure */
struct pa_olt_g_ops {
	/** Updates OLT-G Managed Entity resources
	 *
	 * \param[in] ll_handle             Lower layer context pointer
	 * \param[in] me_id                 Managed Entity identifier
	 * \param[in] update_data           OLT-G update data
	 */
	enum pon_adapter_errno (*update)(
		void *ll_handle,
		uint16_t me_id,
		const struct pa_olt_g_update_data *update_data);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
