/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file holds definitions used for ethtool handling.
 */

#ifndef _PON_NET_ETHTOOL_H
#define _PON_NET_ETHTOOL_H

#include <linux/ethtool.h>
#include <stdint.h>

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/** \defgroup PON_NET_ETHTOOL PON Network Library Ethtool Functions
 *  These functions are used to run ethtool commands.
 *  @{
 */

struct pon_net_context;

/** Issue an ethtool command
 *
 * \param[in]     ctx PON NET context pointer
 * \param[in]     ifname Interface name
 * \param[in]     cmd ethtool command
 * \param[in,out] data ethtool data
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ethtool(struct pon_net_context *ctx,
		const char *ifname,
		uint32_t cmd,
		void *data);

/** Disable tx pause frames on the interface
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] ifname Interface name
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ethtool_pause_frames_tx_disable(struct pon_net_context *ctx,
					const char *ifname);

/** Set ethtool private flags
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] ifname Interface name
 * \param[in] flagname Private flag name
 * \param[in] enable Enable/disable forwarding
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_ethtool_priv_flag_set(struct pon_net_context *ctx,
			      const char *ifname,
			      const char *flagname,
			      uint8_t enable);

/* Number of link mode words is kernel dependent. Use
 * higher number here for allocation. Real number of
 * words in taken later on from ETHTOOL_GLINKSETTINGS.
 */
#define ETHTOOL_LINK_MODE_NUM_U32	3

struct pon_ethtool_link_settings {
	struct ethtool_link_settings req;
	/* The link_mode_data is only used to allocate space,
	 * only use link_mode_masks from above structure for accessing it. */
	uint32_t link_mode_data[3 * ETHTOOL_LINK_MODE_NUM_U32];
};

enum pon_adapter_errno pon_net_ethtool_link_settings_get
	(struct pon_net_context *ctx,
	 const char *ifname,
	 struct pon_ethtool_link_settings *link_settings);

static inline int
pon_net_ethtool_lm_test_bit(unsigned int nr, const uint32_t *mask)
{
	if (nr >= 32 * ETHTOOL_LINK_MODE_NUM_U32)
		return !!0;
	return !!(mask[nr / 32] & (1U << (nr % 32)));
}

/** Get ethtool strings for specified type.
 *
 * \param[in] ctx	PON NET context pointer
 * \param[in] ifname	Interface name
 * \param[in] set_id	Strings type
 *
 * \return returns value as follows:
 * - Allocated memory address: If successful
 * - NULL: in case of error
 *
 * \remark User needs to free allocated memory.
 */
struct ethtool_gstrings *
pon_net_ethtool_strings_get(struct pon_net_context *ctx,
			    const char *ifname,
			    enum ethtool_stringset set_id);

/** Get ethtool index from strings.
 *
 * \param[in] strings	Strings set
 * \param[in] str	String to lookup
 *
 * \return returns value as follows:
 * - Index: If successful found str in strings set
 * - -1: when missing.
 */
int pon_net_ethtool_find_string_idx(const struct ethtool_gstrings *strings,
				    const char *str);

/** Get ethtool statistics (ETHTOOL_GSTATS).
 *
 * \param[in] ctx	PON NET context pointer
 * \param[in] ifname	Interface name
 *
 * \return returns value as follows:
 * - Allocated memory address: If successful
 * - NULL: in case of error
 *
 * \remark User needs to free allocated memory.
 */
struct ethtool_stats *
pon_net_ethtool_stats_get(struct pon_net_context *ctx,
			  const char *ifname);

/** @} */ /* PON_NET_ETHTOOL */

/** @} */ /* PON_NET_LIB */

#endif
