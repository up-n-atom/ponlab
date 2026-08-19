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
 * VLAN Tagging Filter Data.
 */

#ifndef _pon_adapter_me_vlan_tagging_filter_data
#define _pon_adapter_me_vlan_tagging_filter_data

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_VLAN_TAGGING_FILTER_DATA VLAN Tagging Filter Data
 *
 *  This Managed Entity organizes data associated with VLAN tagging.
 *  Instances of this Managed Entity are created and deleted by the OLT.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to
 * an instance of the MAC bridge port configuration data Managed Entity.
 */

/** VLAN Tagging Filter Data operations structure */
struct pa_vlan_tagging_filter_data_ops {
	/** Update VLAN Tagging Filter Data Managed Entity resources
	 *
	 * \remark Enable Managed Entity identifier mapping to driver index
	 *  initialize corresponding driver structures when it is called first
	 *  time
	 *
	 * \param[in] ll_handle		Lower layer context pointer
	 * \param[in] me_id		Managed Entity identifier
	 * \param[in] filter_list	VLAN filter list
	 * \param[in] entries_num	Number of entries (specifies the
	 *				number of valid entries in the VLAN
	 *				filter list)
	 * \param[in] forward_operation	Forward operation
	 * \param[in] use_g988		Use the g988 table of VLAN ops
	 */
	enum pon_adapter_errno (*update)(
		void *ll_handle,
		uint16_t me_id,
		const uint16_t *filter_list,
		uint8_t entries_num,
		uint8_t forward_operation,
		bool use_g988);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle	Lower layer context pointer
	 *  \param[in] me_id		Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
