/******************************************************************************
 *
 *  Copyright (c) 2026 MaxLinear, Inc.
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
 * MAC Bridge Port Bridge Table Data.
 */

#ifndef _pon_adapter_me_mac_bridge_port_bridge_table_data_h
#define _pon_adapter_me_mac_bridge_port_bridge_table_data_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_PORT_BRIDGE_TABLE_DATA\
 *	      MAC Bridge Port Bridge Table Data
 *
 *  This Managed Entity reports status data associated with a MAC bridge port.
 *  The ONU automatically creates or deletes an instance of this Managed Entity
 *  upon the creation or deletion of a MAC bridge port configuration data.
 *
 *  @{
 */

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the MAC bridge port configuration data Managed Entity. */

/** Bridge Table entry type definition
 *  Info:
 *      Age in seconds (1 to 4095) : 12 bit
 *      Reserved : 1 bit
 *      Dynamic/static : 1 bit
 *      Reserved : 1 bit
 *      Filter/forward : 1 bit
*/
struct pa_mac_bp_bridge_table_data_entry {
	/** Information */
	uint16_t info;
	/** MAC address */
	uint8_t p_mac_adress[6];
};

#define PA_MAC_BPT_INFO_AGE_SET(age)   ((age & 0x0FFF) << 4)
#define PA_MAC_BPT_INFO_TYPE_SET(type) ((type & 0x1) << 2)
#define PA_MAC_BPT_INFO_MODE_SET(mode) (mode & 0x1)

#define PA_MAC_BPT_INFO_AGE_MASK  (0x0FFF << 4)
#define PA_MAC_BPT_INFO_TYPE_MASK (0x1 << 2)
#define PA_MAC_BPT_INFO_MODE_MASK 0x1

/** MAC Bridge Port Bridge Table Data operations structure */
struct pa_mac_bp_bridge_table_data_ops {
	/** Enable Managed Entity identifier mapping to driver index and
	 *  initialize corresponding driver structures
	 *
	 *  \param[in] ll_handle         Lower layer context pointer
	 *  \param[in] me_id             Managed Entity identifier
	 */
	enum pon_adapter_errno (*create)(void *ll_handle, uint16_t me_id);

	/** Disable Managed Entity identifier mapping to driver index and clear
	 *  corresponding driver structures
	 *
	 *  \param[in] ll_handle         Lower layer context pointer
	 *  \param[in] me_id             Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);

	/** Retrieve entry from MAC Bridge Port(bp) Bridge Table Data Managed
	 *  Entity resources.
	 *
	 *  \param[in] ll_handle         Lower layer context pointer
	 *  \param[in] me_id		Managed Entity identifier
	 *  \param[in] bridge_me_id	MAC Bridge Service Profile identifier
	 *  \param[out] entry_num	Number of entries pointed to by entries
	 *  \param[out] entries		The pointer to the first entry
	 *
	 *  \remark
	 *  The memory for entries should be de-allocated
	 */
	enum pon_adapter_errno (*get)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t bridge_me_id,
		unsigned int *entry_num,
		struct pa_mac_bp_bridge_table_data_entry **entries);

	/** Free MAC Bridge Port(bp) Bridge Table resources
	 *
	 *  \param[in] ll_handle         Lower layer context pointer
	 *  \param[in] me_id		Managed Entity identifier
	 *  \param[in] bridge_me_id	MAC Bridge Service Profile identifier
	 *  \param[out] entry_num	Number of entries pointed to by entries
	 *  \param[out] entries		The pointer to the first entry
	 */
	enum pon_adapter_errno (*free)(
		void *ll_handle,
		uint16_t me_id,
		uint16_t bridge_me_id,
		unsigned int *entry_num,
		struct pa_mac_bp_bridge_table_data_entry **entries);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
