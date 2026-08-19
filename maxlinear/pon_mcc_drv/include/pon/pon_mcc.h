/******************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_mcc.h
 *
 */

#ifndef __PON_MCC_H
#define __PON_MCC_H

/** \addtogroup PON_MCC_REFFERENCE PON MCC
 *  @{
 */

#define PON_MCC_FAMILY		"pon_mcc"

/** PON MCC attribute list */
enum {
	/** Unspecified attribute */
	PON_MCC_A_UNSPEC,
	/** Interface index */
	PON_MCC_A_IFINDEX,
	/** Protocol type */
	PON_MCC_A_PROTO,
	/** Group address */
	PON_MCC_A_GRP_ADDR,
	/** Source address */
	PON_MCC_A_SRC_ADDR,
	/** Filtering mode */
	PON_MCC_A_FLT_MODE,
	/** Valid */
	PON_MCC_A_VALID,
	/** Initial */
	PON_MCC_A_INITIAL,
	/** IGMP version */
	PON_MCC_A_IGMP_VERSION,
	/** Multicast GEM interface index */
	PON_MCC_A_MC_GEM_IFINDEX,
	/** Bridge Port ID */
	PON_MCC_A_BRIDGE_PORT_ID,
	/** Number of attributes */
	__PON_MCC_A_MAX,
};

#define PON_MCC_A_MAX (__PON_MCC_A_MAX - 1)

/** PON MCC command list */
enum {
	/** Unspecified command */
	PON_MCC_C_UNSPEC,
	/** Add table entry */
	PON_MCC_C_TABLE_ENTRY_ADD,
	/** Remove table entry */
	PON_MCC_C_TABLE_ENTRY_DELETE,
	/** Read table entry */
	PON_MCC_C_TABLE_ENTRY_READ,
	/** Enable multicast */
	PON_MCC_C_MULTICAST_ENABLE,
	/** Disable multicast */
	PON_MCC_C_MULTICAST_DISABLE,
	/** Enable forwarding packets to CPU */
	PON_MCC_C_CPU_FWD_ON,
	/** Disable forwarding packets to CPU */
	PON_MCC_C_CPU_FWD_OFF,
	/** Number of commands */
	__PON_MCC_C_MAX,
};

#define PON_MCC_C_MAX (__PON_MCC_C_MAX - 1)

/** IPv4 address length */
#define PON_MCC_ADDRSZ 4
/** IPv6 address length */
#define PON_MCC_ADDR6SZ 16

/** PON MCC API functions list */

/** Function to add table entries.
 *
 *	\param[in] ifindex Interface index
 *	\param[in] proto IP type
 *	\param[in] grp_addr Multicast Group IP address
 *	\param[in] src_addr Source IP address
 *	\param[in] flt_mode Filtering mode
 *
 *	\remarks The function returns an error code in case of error.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
int mcl_table_entry_add(uint32_t ifindex, uint8_t proto,
			const uint8_t *grp_addr, const uint8_t *src_addr,
			uint8_t flt_mode);

/** Function to remove table entries.
 *
 *	\param[in] ifindex Interface index
 *	\param[in] proto IP type
 *	\param[in] grp_addr Multicast Group IP address
 *	\param[in] src_addr Source IP address
 *	\param[in] flt_mode Filtering mode
 *
 *	\remarks The function returns an error code in case of error.
 *
 *	\return Return value as follows:
 *	- 0: If successful
 *	- Other: An error code in case of error.
 */
int mcl_table_entry_del(uint32_t ifindex, uint8_t proto,
			const uint8_t *grp_addr, const uint8_t *src_addr,
			uint8_t flt_mode);

/**
 * Function to read table entries.
 * If we read entries from forwarding table, function to read them displays
 * entries started from the first after last already read. It means that if
 * we want to read all entries started from the beginning we need to set
 * initial flag to 1 but if we want to read next entries in order we need
 * to use initial flag 0.
 *
 * \param[in] initial Initial value
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_table_entry_read(uint8_t initial);

/**
 * Function to enable Multicast.
 *
 * \param[in] igmp_version IGMP/MLD version
 * \param[in] mc_gem_ifindex Multicast GEM interface index
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_multicast_enable(uint8_t igmp_version, uint32_t mc_gem_ifindex);

/**
 * Function to disable Multicast.
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_multicast_disable(void);

/** End of functions list */

/** PON MCC API debug functions list */

/**
 * Function to set Snooping configuration.
 *
 * \param[in] igmp_mode IGMP mode
 * \param[in] is_igmpv3 Value to distinguish IGMP version
 * \param[in] cross_vlan Cross VLAN
 * \param[in] fwd_port Forward Port
 * \param[in] fwd_port_id Forward Port ID
 * \param[in] cos Class of Service
 * \param[in] robust Robust
 * \param[in] query_ival Query Interval
 * \param[in] suppression_aggregation Suppression Aggregation
 * \param[in] fast_leave Fast Leave
 * \param[in] learning_router Learning router
 * \param[in] mc_unk_drop Multicast unknown drop
 * \param[in] mc_fid_mode Multicast FID mode
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_snoop_cfg_set(uint8_t igmp_mode, uint8_t is_igmpv3, uint8_t cross_vlan,
		      uint8_t fwd_port, uint8_t fwd_port_id, uint8_t cos,
		      uint8_t robust, uint8_t query_ival,
		      uint8_t suppression_aggregation, uint8_t fast_leave,
		      uint8_t learning_router, uint8_t mc_unk_drop,
		      uint8_t mc_fid_mode);

/**
 * Function to get Snooping configuration.
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_snoop_cfg_get(void);

/**
 * Function to set Bridge Port configuration.
 *
 * \param[in] bridge_port_id Bridge Port ID
 * \param[in] mc_dest_ip_lookup_dis Multicast destination IP Lookup Disable
 * \param[in] mc_src_ip_lookup_en Multicast source IP Lookup Enable
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_bridge_port_cfg_set(uint32_t bridge_port_id,
			    uint8_t mc_dest_ip_lookup_dis,
			    uint8_t mc_src_ip_lookup_en);

/**
 * Function to get Bridge Port configuration.
 *
 * \param[in] bridge_port_id Bridge Port ID
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_bridge_port_cfg_get(uint32_t bridge_port_id);

/**
 * Function to set Bridge configuration.
 *
 * \param[in] bridge_id Bridge ID
 * \param[in] fwd_unk_mc_non_ip Forward Unknown Multicast Non IP
 * \param[in] fwd_unk_mc_ip Forward Unknown Multicast IP
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_bridge_cfg_set(uint32_t bridge_id, uint8_t fwd_unk_mc_non_ip,
		       uint8_t fwd_unk_mc_ip);

/**
 * Function to get Bridge configuration.
 *
 * \param[in] bridge_id Bridge ID
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_bridge_cfg_get(uint32_t bridge_id);

/**
 * Function to enable forwarding packets to the CPU.
 *
 * \param[in] bridge_port_id Bridge Port ID
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_cpu_fwd_enable(uint8_t bridge_port_id);

/**
 * Function to disable forwarding packets to the CPU.
 *
 * \param[in] bridge_port_id Bridge Port ID
 *
 * \remarks The function returns an error code in case of error.
 *
 * \return Return value as follows:
 * - 0: If successful
 * - Other: An error code in case of error.
 */
int mcl_cpu_fwd_disable(uint8_t bridge_port_id);

/** End of debug functions list */

/** @} */

#endif /* __PON_MCC_H */
