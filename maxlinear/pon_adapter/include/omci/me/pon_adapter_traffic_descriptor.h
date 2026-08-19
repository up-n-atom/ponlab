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
 * Traffic Descriptor.
 */

#ifndef _pon_adapter_me_traffic_descriptor_h
#define _pon_adapter_me_traffic_descriptor_h

#include "pon_adapter_base.h"
#include "omci/pon_adapter_meter.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TRAFFIC_DESCRIPTOR Traffic Descriptor
 *
 *  The traffic descriptor allows for traffic management. A priority controlled
 *  ONU can point from a MAC bridge port configuration data Managed Entity to a
 *  traffic descriptor in order to implement traffic management
 *  (marking, policing).
 *  A rate controlled ONU can point to a traffic descriptor from either a MAC
 *  bridge port configuration data Managed Entity or GEM port network CTP to
 *  implement traffic management (marking, shaping).
 *
 *  Packets are determined to be green, yellow or red as a function of the
 *  ingress packet rate and the settings in this Managed Entity.
 *  The color indicates drop precedence (eligibility), subsequently used by the
 *  priority queue-G Managed Entity to drop packets conditionally during
 *  congestion conditions. Packet color is also used by the mode 1 DBA status
 *  reporting function described in ITU-T G.984.3.
 *  Red packets are dropped immediately. Yellow packets are marked as drop
 *  eligible, and green packets are marked as not drop eligible, according to
 *  the egress color marking attribute.
 *
 *  @{
 */

/** Maximum possible traffic descriptor Managed Entities */
#define ONU_GPE_MAX_TRAFFIC_DESCRIPTOR		64

/** Supported values of color mode parameter in
   \ref pa_traffic_descriptor_update_data
*/
enum pon_adapter_traffic_descriptor_color_mode {
	/** Color-blind Mode */
	PON_ADAPTER_TD_COLOR_BLIND = 0,
	/** Color-aware Mode */
	PON_ADAPTER_TD_COLOR_AWARE = 1
};

/** Supported values of Ingress/Egress Color Marking parameter in
   \ref pa_traffic_descriptor_update_data
*/
enum pon_adapter_traffic_descriptor_color_marking {
	/** No marking (ignore ingress marking) */
	PON_ADAPTER_TD_NO_MARKING = 0,
	/** Internal marking only */
	PON_ADAPTER_TD_INTERNAL_MARKING = 1,
	/** DEI (802.1ad) Marking */
	PON_ADAPTER_TD_DEI_MARKING = 2,
	/** PCP 8P0D (802.1ad) Marking */
	PON_ADAPTER_TD_PCP8P0D_MARKING = 3,
	/** PCP 7P1D (802.1ad) Marking */
	PON_ADAPTER_TD_PCP7P1D_MARKING = 4,
	/** PCP 6P2D (802.1ad) Marking */
	PON_ADAPTER_TD_PCP6P2D_MARKING = 5,
	/** PCP 5P3D (802.1ad) Marking */
	PON_ADAPTER_TD_PCP5P3D_MARKING = 6,
	/** DSCP AF class (RFC 2597) Marking*/
	PON_ADAPTER_TD_DSCPAFCLASS_MARKING = 7
};

/** Supported values of Meter type parameter in
   \ref pa_traffic_descriptor_update_data
*/
enum pon_adapter_traffic_descriptor_meter_type {
	/** Not specified Meter type */
	PON_ADAPTER_TD_NOTSPECIFIED_METERTYPE = 0,
	/** RFC 4115 specified Meter type */
	PON_ADAPTER_TD_RFC4115_METERTYPE = 1,
	/** RFC 2698 specified Meter type */
	PON_ADAPTER_TD_RFC2698_METERTYPE = 2
};

/** Data structure used to specify a meter/policer block */
struct pon_adapter_block {
	/** Meter/policer index */
	uint8_t index;
	/** Meter/policer enable */
	uint8_t enable;
};

#define MAX_TD_METER 32
#define MAX_TD_SHAPER 32

/** Data structure used by resource management */
struct pon_adapter_traffic_descriptor {
	/** Meter/policer or shaper configuration values */
	struct pon_adapter_meter_cfg meter_cfg;
	/** Meter/policer resources */
	struct pon_adapter_block meter[MAX_TD_METER];
	/** Traffic shaper resources */
	struct pon_adapter_block shaper[MAX_TD_SHAPER];
};

/** Traffic Descriptor update structure */
struct pa_traffic_descriptor_update_data {
	/** CIR (committed information rate, in byte/s) */
	uint32_t cir;
	/** PIR (peak information rate, in byte/s) */
	uint32_t pir;
	/** CBS (committed block size, in bytes) */
	uint32_t cbs;
	/** PBS (peak block size, in bytes) */
	uint32_t pbs;
	/** Color mode */
	uint8_t color_mode;
	/** Ingress color marking */
	uint8_t ingress_color_marking;
	/** Egress color marking */
	uint8_t egress_color_marking;
	/** Meter type */
	uint8_t meter_type;
};

/** Traffic Descriptor operations structure */
struct pa_traffic_descriptor_ops {
	/** Update Traffic Descriptor Managed Entity resources
	 *
	 * \remark Enable Managed Entity identifier mapping to driver index and
	 * initialize corresponding driver structures when it is called first
	 * time for the given Managed Entity ID
	 *
	 * \param[in] ll_handle		     Lower layer context pointer
	 * \param[in] me_id		     Managed Entity identifier
	 * \param[in] update_data	     Traffic Descriptor update data
	 */
	enum pon_adapter_errno
	(*update)(void *ll_handle,
		  uint16_t me_id,
		  const struct pa_traffic_descriptor_update_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] me_id	 Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);

	/** Retrieve the Managed Entity values from the resource management
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] idx	 Traffic descriptor index
	 * \param[out] dest	 Structure which will hold the data
	 */
	enum pon_adapter_errno
	(*get)(void *ll_handle,
	       const uint32_t idx,
	       struct pon_adapter_traffic_descriptor *dest);

	/** Attach meter to the specified TD
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] td_idx	 Traffic descriptor index
	 * \param[in] meter_idx  Meter index
	 */
	enum pon_adapter_errno (*meter_attach)(void *ll_handle,
					       const uint32_t td_idx,
					       const uint8_t meter_idx);

	/** Detach meter from the specified TD
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] td_idx	 Traffic descriptor index
	 * \param[in] meter_idx  Meter index
	 */
	enum pon_adapter_errno (*meter_detach)(void *ll_handle,
					       const uint32_t td_idx,
					       const uint8_t meter_idx);

	/** Attach shaper to the specified TD
	 *
	 * \param[in] ll_handle	  Lower layer context pointer
	 * \param[in] td_idx	  Traffic descriptor index
	 * \param[in] shaper_idx  Shaper index
	 */
	enum pon_adapter_errno (*shaper_attach)(void *ll_handle,
						const uint32_t td_idx,
						const uint32_t shaper_idx);

	/** Detach all shaper from the specified TD
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] td_idx	 Traffic descriptor index
	 */
	enum pon_adapter_errno (*shaper_detach)(void *ll_handle,
						const uint32_t td_idx);

	/** Update the shaper configuration with all possible traffic descriptor
	 *  settings. In case that more than one TD is pointing to one shaper
	 *  the maximum values are used.
	 *
	 * \param[in] ll_handle	    Lower layer context pointer
	 * \param[in] shaper_index  Traffic shaper index
	 */
	enum pon_adapter_errno (*shaper_update)(void *ll_handle,
						const uint32_t shaper_index);

	/** Reset all shaper related to this traffic descriptor to zero.
	 *
	 * \param[in] ll_handle  Lower layer context pointer
	 * \param[in] td_idx	 Traffic descriptor index
	 */
	enum pon_adapter_errno (*shaper_clean)(void *ll_handle,
					       const uint32_t td_idx);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
