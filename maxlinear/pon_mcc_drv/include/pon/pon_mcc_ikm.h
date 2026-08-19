/******************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * PON Multicast driver API definition
 *
 */

#ifndef __PON_MCC_IKM_H
#define __PON_MCC_IKM_H

#include <stddef.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/types.h>

/* Cast unused parameters to void */
#define UNUSED(x) (void)(x)

/** \addtogroup PON_MCC_REFFERENCE PON Multicast
 *  @{
 */

#define PORTMAP_SIZE		256

/** \brief Selection to use IPv4 or IPv6.
 * Used along with \ref mcc_l3addr to denote
 * which union member to be accessed.
 */
enum mcc_l3proto {
	/** IPv4 Type */
	MCC_L3_PROTO_IPV4,
	/** IPv6 Type */
	MCC_L3_PROTO_IPV6,
};

/** \brief
 * Defines the IGMP/MLD version according to OMCI code point definitions.
 */
enum igmp_version {
	/** IGMP version 2 */
	IGMP_V2 = 2,
	/** IGMP version 3 */
	IGMP_V3 = 3,
	/** MLD version 1 */
	MLD_V1 = 16,
	/** MLD version 2 */
	MLD_V2 = 17,
};

/** \brief Defines the multicast group source filtering mode */
enum src_flt_mode {
	/** Group source IP does not matter. This means all source IP
	 * addresses (*) are included for the multicast group membership.
	 * This is default mode for IGMPv1 and IGMPv2
	 */
	SRC_FLT_MODE_NA = 0,
	/** Include source IP address membership mode.
	 * Only supported for IGMPv3.
	 */
	SRC_FLT_MODE_INCLUDE = 1,
	/** Exclude source IP address membership mode.
	 * Only supported for IGMPv3.
	 */
	SRC_FLT_MODE_EXCLUDE = 2
};

/** \brief This is a union to describe the IPv4 and IPv6 address
 * The member selection will be based on \ref mcc_l3proto
 */
union mcc_l3addr {
	/** Describe the IPv4 address.
	 * Only used if the IPv4 address should be read or configured.
	 */
	struct in_addr addr;
	/** Describe the IPv6 address.
	 * Only used if the IPv6 address should be read or configured.
	 */
	struct in6_addr addr6;
};

/** \brief This is structure used by provided API
 * to add, delete or read multicast table entry data
 */
struct mcc_drv_l3tbl_key {
	/** net device interface index */
	int ifindex;
	/** protocol type IPv4 or IPv6 */
	enum mcc_l3proto proto;
	/** MC group address IPv4,IPv6 depends on proto */
	union mcc_l3addr grp;
	/** MC group mask */
	union mcc_l3addr grp_mask;
	/** MC source address IPv4,IPv6 depends on proto */
	union mcc_l3addr src;
	/** MC source mask */
	union mcc_l3addr src_mask;
	/** IGMPv3 related mode INCLUDE or EXCLUDE */
	enum src_flt_mode flt_mode;
	/** the flag indicates the validity of data read */
	bool valid;
};

/**
 * @brief Add L3 multicast table entry.
 *
 * @param key is a pointer to struct mcc_drv_l3tbl_key
 * @param flags is a placeholder for future extension
 *
 * @return Returns zero on success
 *	In case of an error a negative error code is returned.
 *	-E...	The regular Unix error codes are used.
 */
int mcc_drv_l3_tbl_add(const struct mcc_drv_l3tbl_key *key, int flags);

/**
 * @brief Remove L3 multicast table entry.
 *
 * @param key is a pointer to struct mcc_drv_l3tbl_key
 * @param flags is a placeholder for future extension
 *
 * @return Returns zero on success
 *	In case of an error a negative error code is returned.
 *	-E...	The regular Unix error codes are used.
 */
int mcc_drv_l3_tbl_del(const struct mcc_drv_l3tbl_key *key, int flags);

/**
 * @brief Read L3 multicast table entry.
 *  The \ref initial parameter must be true when first calling this function.
 *  This ensures that you start reading from the beginning of the table.
 *  Each subsequent call of this function reads the next valid entry.
 *  After reading the last valid entry, the function returns the \ref last
 *  parameter, indicating that all valid table entries have already been read.
 * @param key is a pointer to read multicast table entry data
 * @param activity returns MC group activity indication
 * @param initial must be true to begin read sequence
 * @param last returns indication that last entry has bean reached
 * @param flags is a placeholder for future extension
 * @param portmap stores portmap from hardware entries
 *
 * @return Returns zero on success
 *	In case of an error a negative error code is returned.
 *	-E...	The regular Unix error codes are used.
 */
int mcc_drv_l3_tbl_get(struct mcc_drv_l3tbl_key *key, bool *activity,
		       bool initial, bool *last, int *flags, void *portmap);

/* The function below is intended mainly for checker driver */
/**
 * @brief Set bridge port configuration.
 * @param br_port_id provides bridge port ID
 * @param fwd_mask_to_clear mask of bits to clear in map value
 * @param fwd_mask_to_clear mask of bits to set in map value
 *
 * @return Returns zero on success
 *	In case of an error a negative error code is returned.
 *	-E...	The regular Unix error codes are used.
 */
#define BR_PORT_FWD_MAP_TYPE_H	uint16_t
int mcc_drv_br_port_fwd_map_set_br_port_id(uint8_t br_port_id,
		BR_PORT_FWD_MAP_TYPE_H fwd_mask_to_clear,
		BR_PORT_FWD_MAP_TYPE_H fwd_mask_to_set);

/** @} */

#endif /* __PON_MCC_IKM_H */

