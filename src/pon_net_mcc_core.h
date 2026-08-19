/*****************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file holds definitions used for multicast control handling.
 */

#include <omci/pon_adapter_mcc.h>
#include "pon_net_common.h"

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_MCC_CORE PON Adapter Multicast Control Core
 *  This covers network functions used for multicast control.
 *
 * @{
 */

/* Forward declaration */
extern const struct pa_omci_mcc_ops pon_net_omci_mcc_ops;

/** MOP vlan actions for upstream Multicast control packets
 *  It represent the functionality of the "Upstream IGMP Tag Control"
 *  attribute of the "Multicast Operations Profile" OMCI Managed Entity.
 *  Values not covered by the enumeration are not defined by OMCI.
 *
 * TODO: Check if needed for tc based config or remove.
 */
enum us_mop_action {
	/** Do not modify the VLAN tag (if there is any). */
	us_mop_vlan_transparent = 0,
	/** Add an outer VLAN tag with new VID,
	 *  DEI and p-bits given by the attribute.
	 */
	us_mop_vlan_add = 1,
	/** Replace the outer VLAN tag with new VID,
	 *  DEI, and p-bits given by the attribute.
	 */
	us_mop_vlan_replace_tci = 2,
	/** Replace the outer VLAN tag with new VID
	 *  given by the attribute but keep DEI and p-bits.
	 */
	us_mop_vlan_replace_vid = 3
};

/** MOP vlan actions for upstream Multicast control packets
 *  It represent the functionality of the "Downstream IGMP and Multicast TCI"
 *  attribute of the "Multicast Operations Profile" OMCI Managed Entity.
 *  Values not covered by the enumeration are not defined by OMCI.
 *
 * TODO: Check if needed for tc based config or remove.
 */
enum ds_mop_action {
	/** Do not change the VLAN(s) (if there is any). */
	ds_mop_vlan_transparent = 0,
	/** Strip the outer VLAN tag (if there is one). */
	ds_mop_vlan_strip = 1,
	/** Add an outer VLAN tag with new VID, DEI and p-bits as given by
	 *  the attribute.
	 */
	ds_mop_vlan_add = 2,
	/** Replace the outer VLAN tag with new VID, DEI, and p-bits given by
	 *  the attribute.
	 */
	ds_mop_vlan_replace_tci = 3,
	/** Replace the outer VLAN tag with a new VID given by this attribute
	 *  but keep DEI and p-bit values.
	 */
	ds_mop_vlan_replace_vid = 4,
	/** Add an outer VLAN tag with a new VID, DEI and p-bits given by the
	 *  "Multicast Subscriber Config Info" Managed Entity which is related.
	 */
	ds_mop_vlan_add_from_msc = 5,
	/** Replace the outer VLAN tag with a new VID, DEI, and p-bits given by
	 *  the "Multicast Subscriber Config Info" Managed Entity which is
	 *  related.
	 */
	ds_mop_vlan_replace_tci_from_msc = 6,
	/** Replace the outer VLAN tag with a new VID given by the "Multicast
	 *  Subscriber Config Info" Managed Entity which is related, but keep
	 *  the received DEI and p-bits.
	 */
	ds_mop_vlan_replace_vid_from_msc = 7
};

/** @} */ /* PON_NET_MCC_CORE */

/** @} */ /* PON_NET_LIB */
