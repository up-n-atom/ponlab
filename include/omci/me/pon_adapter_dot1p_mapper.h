/******************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * IEEE 802.1p mapper.
 */

#ifndef _pon_adapter_me_dot1p_mapper_h
#define _pon_adapter_me_dot1p_mapper_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_8021P_MAPPER_SERVICE_PROFILE\
 *	      IEEE 802.1p Mapper Service Profile
 *
 *  This Managed Entity associates the priorities of priority tagged frames with
 *  specific connections. The operation of this Managed Entity affects only
 *  upstream traffic.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 * @{
 */

/** Derive implied PCP field from DSCP bits of received frame */
#define PA_DOT1P_MAPPER_PBIT_FROM_DSCP 0

/**
 * Set implied PCP field to a fixed value specified by
 * the default P-bit assumption attribute
 */
#define PA_DOT1P_MAPPER_PBIT_FIXED 1

/** IEEE 802.1p Mapper Service Profile update data structure */
struct pa_dot1p_mapper_update_data {
	/** TP pointer */
	uint16_t tp_pointer;
	/**
	 * Array of Interworking TP pointers for 802.1p priority assignment
	 * - X: GEM port network CTP connectivity
	 * - pointer: GEM Port ID value
	 */
	uint16_t *gem_port_id;
	/** Unmarked frame option */
	uint8_t unmarked_frame_opt;
	/** DSCP to P-bit mapping */
	uint8_t *dscp_prio_map;
	/** Default P-bit marking */
	uint8_t default_priority_marking;
	/** TP type */
	uint8_t tp_pointer_type;
};

/** IEEE 802.1p Mapper Service Profile destroy data structure */
struct pa_dot1p_mapper_destroy_data {
	/** TP Pointer */
	uint16_t tp_pointer;
	/** TP type */
	uint8_t tp_pointer_type;
};

/** IEEE 802.1p Mapper Service Profile operations structure */
struct pa_dot1p_mapper_ops {
	/** Update IEEE 802.1p Mapper Service Profile Managed Entity resources
	 *
	 *  \remark Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures when it is called first
	 *  time for the given Managed Entity ID.
	 *
	 *  \param[in] ll_handle                Lower layer context pointer
	 *  \param[in] me_id                    Managed Entity identifier
	 *  \param[in] update_data		Update data
	 */
	enum pon_adapter_errno (*update)(void *ll_handle, const uint16_t me_id,
			 const struct pa_dot1p_mapper_update_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle               Lower layer context pointer
	 *  \param[in] me_id                   Managed Entity identifier
	 *  \param[in] destroy_data	       Destroy data
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, const uint16_t me_id,
		const struct pa_dot1p_mapper_destroy_data *destroy_data);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
