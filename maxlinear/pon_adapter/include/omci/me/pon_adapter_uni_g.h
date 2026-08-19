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
 * UNI-G.
 */

#ifndef _pon_adapter_me_uni_g_h
#define _pon_adapter_me_uni_g_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_UNI_G UNI-G
 *
 * This Managed Entity organizes data associated with User Network Interfaces
 * (UNIs) supported by GEM. One instance of the UNI-G Managed Entity exists for
 * each UNI supported by the ONU.
 *
 * The ONU automatically creates or deletes instances of this Managed Entity
 * upon the creation or deletion of a real or virtual circuit pack managed
 * entity, one per port.
 *
 * @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of a physical path termination point. */

/** UNI-G operations structure */
struct pa_uni_g_ops {
	/** Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 * \param[in] config_option_status Configuration option status
	 */
	enum pon_adapter_errno (*create)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t config_option_status);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(
		void *ll_handle,
		uint16_t me_id);

	/** Update UNI-G resources
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 * \param[in] config_option_status Configuration option status
	 */
	enum pon_adapter_errno (*update)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t config_option_status);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
