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
 * GEM Interworking Termination Point (TP).
 */

#ifndef _pon_adapter_me_gem_interworking_tp_h
#define _pon_adapter_me_gem_interworking_tp_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pa_bridge_data;

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_GEM_INTERWORKING_TP\
 *  GEM Interworking Termination Point
 *
 *  An instance of this Managed Entity represents a point in the ONU where the
 *  interworking of a service (for example CES or IP) or underlying physical
 *  infrastructure (such as n x DS0, DS1, DS3, E3, or Ethernet) to GEM layer
 *  takes place. At this point, GEM packets are generated from a bit stream
 *  (Ethernet, for instance) or a bit stream is reconstructed from GEM packets.
 *
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/** This enum holds the values that identify the type of non-GEM function that
    is being interworked.
*/
enum pa_gem_itp_non_gem_function_type {
	/** TDM circuit emulation (not supported) */
	PA_GEM_ITP_CIRCUIT_EMULATED_TDM = 0x00,
	/** MAC bridged LAN */
	PA_GEM_ITP_MAC_BRIDGE_LAN = 0x01,
	/** Reserved */
	PA_GEM_ITP_RESERVED = 0x02,
	/** IP data service */
	PA_GEM_ITP_IP_DATA_SERVICE = 0x03,
	/** Video returnpath (not supported) */
	PA_GEM_ITP_VIDEO_RETURN_PATH = 0x04,
	/** IEEE 802.1p mapper */
	PA_GEM_ITP_P_MAPPER = 0x05,
	/** Downstream broadcast */
	PA_GEM_ITP_DOWNSTREAM_BROADCAST = 0x06,
	/** MPLS service (not supported) */
	PA_GEM_ITP_MPLS_PW_TDM_SERVICE = 0x07
};

/** GEM interworking TP update structure */
struct pa_gem_interworking_tp_update_data {
	/** Non-GEM function that is being interworked */
	uint8_t interworking_option;
	/** Service profile pointer */
	uint16_t service_profile_pointer;
	/** Interworking termination point pointer */
	uint16_t interworking_tp_pointer;
	/** GAL loopback configuration */
	uint8_t gal_loopback_configuration;
};

/** GEM interworking TP operations structure */
struct pa_gem_interworking_tp_ops {
	/** Update GEM Interworking TP Managed Entity resources.
	 *
	 * \remark Enable Managed Entity identifier mapping to driver index and
	 *	 initialize corresponding driver structures when it is called
	 *	 first time for the given Managed Entity ID
	 *
	 * \param[in] ll_handle                  Lower layer context pointer
	 * \param[in] me_id                      Managed Entity identifier
	 * \param[in] gem_port_id                GEM Port Id
	 * \param[in] bridge                     Bridge data
	 * \param[in] max_gem_payload_size       Max GEM payload size
	 * \param[in] update_data                GEM interworking TP update data
	 */
	enum pon_adapter_errno (*update)(void *ll_handle, uint16_t me_id,
		uint16_t gem_port_id, struct pa_bridge_data *bridge,
		uint16_t max_gem_payload_size,
		const struct pa_gem_interworking_tp_update_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle               Lower layer context pointer
	 * \param[in] me_id                   Managed Entity identifier
	 * \param[in] gem_port_id             GEM Port Id
	 * \param[in] interworking_option     Non-GEM function that is being
	 *				      interworked
	 * \param[in] service_profile_pointer Service profile pointer
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id,
					  uint16_t gem_port_id,
					  uint8_t interworking_option,
					  uint16_t service_profile_pointer);

	/** Retrieve the operational state
	 *
	 * \param[in]  ll_handle               Lower layer context pointer
	 * \param[in]  me_id                   Managed Entity identifier
	 * \param[in]  gem_port_id             GEM Port Id
	 * \param[out] op_state                Operational state
	 */
	enum pon_adapter_errno (*op_state_get)(void *ll_handle,
					       const uint16_t me_id,
					       const uint16_t gem_port_id,
					       uint8_t *op_state);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
