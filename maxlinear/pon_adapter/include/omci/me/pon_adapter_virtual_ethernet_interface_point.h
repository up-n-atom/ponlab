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
 * Virtual Ethernet Interface Point (VEIP).
 */

#ifndef _pon_adapter_virtual_ethernet_interface_point_h
#define _pon_adapter_virtual_ethernet_interface_point_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_VEIP\
 * Virtual Ethernet Interface Point
 *
 * This Managed Entity represents the data plane hand-off point in an ONU to a
 * separate (non-OMCI) management domain. The virtual Ethernet interface point
 * is managed by OMCI, and is potentially known to the non-OMCI management
 * domain. One or more Ethernet traffic flows are present at this boundary.
 *
 * Instances of this Managed Entity are automatically created and deleted by
 * the ONU. This is necessary because the required downstream priority queues
 * are subject to physical implementation constraints. The OLT may use one or
 * more of the virtual Ethernet interface points created by the ONU.
 *
 * It is expected that the ONU would create one virtual Ethernet interface
 * point for each non-OMCI management domain. At the vendor's discretion, a
 * virtual Ethernet interface point may be created for each traffic class.
 *
 * @{
 */

/** Virtual_ethernet_interface_point operations structure */
struct pa_virtual_ethernet_interface_point_ops {
	/** Lock a Virtual Ethernet Interface Point
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno(*lock)(void *ll_handle, uint16_t me_id);

	/** Unlock a Virtual Ethernet Interface Point
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 *
	 * \return Return value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno(*unlock)(void *ll_handle, uint16_t me_id);

	/** Create a Virtual Ethernet Interface Point
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 */
	enum pon_adapter_errno (*create)(void *ll_handle, uint16_t me_id);

	/** Destroy a Virtual Ethernet Interface Point
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
