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
 * MAC Bridge Port Filter Table Data.
 */

#ifndef _pon_adapter_me_mac_bridge_port_filter_table_data
#define _pon_adapter_me_mac_bridge_port_filter_table_data

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_MAC_BRIDGE_PORT_FILTER_TABLE_DATA\
 *	      MAC Bridge Port Filter Table Data
 *
 * This Managed Entity organizes data associated with a bridge port. The ONU
 * automatically creates or deletes an instance of this Managed Entity upon the
 * creation or deletion of a MAC bridge port configuration data Managed Entity.
 *
 * \remark The OLT should disable learning mode in the MAC bridge service
 * profile before writing to the MAC filter table.
 *
 * @{
 */

/** Specifies new MAC filter index. */
#define PA_MAC_FILTER_NEW_INDEX			(0xFFFFFFFF)
/** MAC Filter allows all addresses */
#define PA_MAC_FILTER_OP_NO_FILTER		(0x0)
/** MAC Filter denies from specific addresses */
#define PA_MAC_FILTER_OP_FILTER			(0x2)
/** MAC Filter allows from specific addresses */
#define PA_MAC_FILTER_OP_FWD			(0x3)

/** MAC bridge configuration data type definition */
struct pa_mac_bridge_data {
	/** Learning indication */
	uint8_t learning_ind;
	/** Port bridging indication */
	uint8_t port_bridging_ind;
	/** Unknown MAC address discard indication */
	uint8_t unknown_mac_discard;
	/** MAC learning depth */
	uint8_t mac_learning_depth;
	/** MAC Bridge data validity flag */
	bool valid;
	/** GEM port direction */
	uint8_t direction;
};

/* Through an identical ID, this Managed Entity is implicitly linked to an
   instance of the MAC bridge port configuration data Managed Entity. */

/** MAC Bridge Port Filter Table Data operations structure */
struct pa_mac_bp_filter_table_data_ops {
	/** Add MAC filter entry.
	 *
	 * \param[in] ll_handle            Lower layer context pointer
	 * \param[in] me_id                Managed Entity identifier
	 * \param[in] bridge_me_id         MAC Bridge Managed Entity identifier
	 * \param[in] mac_filter_index     MAC Filter index.
	 * \param[in] is_src_mac_filter    Is MAC filter Source or Destination
	 *                                 - 0: Destination
	 *                                 - 1: Source.
	 * \param[in] filter_op            Filter operating specifier.
	 *                                 - 0: Do not filter
	 *                                 \ref PA_MAC_FILTER_OP_NO_FILTER
	 *                                 - 2: Filter
	 *                                  \ref PA_MAC_FILTER_OP_FILTER
	 *                                 - 3: Forward
	 *                                  \ref PA_MAC_FILTER_OP_FWD
	 */
	enum pon_adapter_errno (*assign)(
		void *ll_handle,
		const uint16_t me_id,
		const uint16_t bridge_me_id,
		const uint32_t mac_filter_index,
		const uint8_t is_src_mac_filter,
		const uint8_t filter_op);

	/** Add MAC filter entry.
	 *
	 * \param[in] ll_handle        Lower layer context pointer
	 * \param[in] me_id            Managed Entity identifier
	 * \param[in] filter_index_in  MAC Filter Input index.
	 *                             Set to
	 *                             \ref PA_MAC_FILTER_NEW_INDEX
	 *                             to specify a new MAC filter.
	 * \param[in] mac              MAC Address
	 * \param[out] filter_index_out MAC Filter Output index.
	 *                             Actual MAC filter
	 *                             index.
	 */
	enum pon_adapter_errno (*entry_add)(
		void *ll_handle,
		const uint16_t me_id,
		const uint32_t filter_index_in,
		const uint8_t mac[6],
		uint32_t *filter_index_out);

	/** Remove MAC filter entry.
	 *
	 * \param[in] ll_handle        Lower layer context pointer
	 * \param[in] me_id            Managed Entity identifier
	 * \param[in] filter_index_in  MAC Filter Input index.
	 *                             Set to
	 *                             \ref PA_MAC_FILTER_NEW_INDEX
	 *                             to specify a new MAC filter.
	 * \param[in] mac              MAC Address
	 * \param[out] filter_index_out MAC Filter Output index.
	 *                            Actual MAC filter index.
	 */
	enum pon_adapter_errno (*entry_remove)(
		void *ll_handle,
		const uint16_t me_id,
		const uint32_t filter_index_in,
		const uint8_t mac[6],
		uint32_t *filter_index_out);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
