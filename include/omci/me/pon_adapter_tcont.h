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
 * T-CONT.
 */

#ifndef _pon_adapter_me_tcont
#define _pon_adapter_me_tcont

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_TCONT T-CONT
 *
 *  An instance of the traffic container Managed Entity T-CONT represents a
 *  logical connection group associated with a PLOAM layer alloc-id. A T-CONT
 *  can accommodate GEM packets in priority queues or traffic schedulers that
 *  exist in the GEM layer.
 *
 *  The ONU autonomously creates instances of this Managed Entity.
 *  The OLT can discover the number of T-CONT instances via the ANI-G
 *  Managed Entity. When the ONU's MIB is reset or created for the first time,
 *  all supported T-CONTs are created. The OLT provisions allocation-IDs to the
 *  ONU via the PLOAM channel. The OLT must then set the alloc-id attributes
 *  in the T-CONTs that it wants to activate for user traffic,
 *  to create the appropriate association between the allocation ID set in the
 *  PLOAM channel and the T-CONT in the OMCI. Once that association is created,
 *  the mode indicator attribute assumes the mode specified in the PLOAM
 *  channel. Note that there should be a one-to-one relationship between
 *  allocation IDs and T-CONT Managed Entities, and the connection of
 *  multiple T-CONTs to a single allocation ID is undefined.
 *
 *  @{
 */

/** T-CONT operations structure */
struct pa_tcont_ops {
	/** Set T-CONT Managed Entity resources during update
	 *
	 *  \param[in] ll_handle    Lower layer context pointer
	 *  \param[in] me_id        Managed Entity identifier
	 *  \param[in] policy       Policy
	 *  \param[in] alloc_id     Alloc-ID
	 *  \param[in] create_only  T-CONT creation only action
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
				      uint16_t me_id,
				      uint16_t policy,
				      uint16_t alloc_id,
				      uint8_t create_only);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle    Lower layer context pointer
	 *  \param[in] me_id        Managed Entity identifier
	 *  \param[in] alloc_id     Alloc-ID
	 *  \param[in] deact_mode   T-CONT deactivation mode
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle,
					  uint16_t me_id,
					  uint16_t alloc_id,
					  uint8_t deact_mode);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
