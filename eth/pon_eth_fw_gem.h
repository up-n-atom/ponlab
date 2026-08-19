/******************************************************************************
 *
 *  Copyright (c) 2025 MaxLinear, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

#ifndef __PON_ETH_FW_GEM_H
#define __PON_ETH_FW_GEM_H

#include <linux/types.h>

/* GEM port information */
struct gem_port_info {
	/** ID, logical number
	 * do not change after allocation
	 */
	u16 id;
	/** index, assigned by FW or warmstart configuration */
	u16 idx;
	/** Maximum GEM Payload Size */
	u16 max_size;
	/** Traffic type */
	u8 traffic_type;
	/** Direction */
	u8 dir;
	/** Encryption */
	u8 enc;
	/** Node in the list of GEM ports linked to an allocation */
	struct list_head alloc_node;
	/** Assigned allocation information */
	struct alloc_info *alloc_info;
};

struct alloc_info;

/**
 * @brief Allocate a new GEM port and checks if the id is already used.
 * @param id: GEM ID
 * @param preinit: Optional pre-initialization data for the GEM port.
 * @return Pointer to gem_port_info or an error code encoded in the pointer.
 * @retval -EINVAL if the id is invalid.
 * @retval -EEXIST if the id is already used.
 *
 * The returned pointer is managed internally and must not be freed by the
 * caller. The lifetime of the object is handled by the GEM subsystem and
 * freeing must be done by calling gem_port_info_free().
 */
struct gem_port_info *gem_port_allocate(u16 id,
					const struct gem_port_info *preinit);

/**
 * @brief Free the GEM port info object.
 * @param info: Pointer to gem_port_info previously obtained (may be NULL).
 */
void gem_port_info_free(struct gem_port_info *info);

/** Apply GEM port information to firmware
 * @param info		GEM port information
 * @param alloc		Allocation information (may be NULL)
 * @return		0 on success, negative error code on failure
 */
int gem_port_fw_apply(struct gem_port_info *info, struct alloc_info *alloc);

/** Free GEM port information from firmware
 * @param info		GEM port information
 */
void gem_port_fw_free(struct gem_port_info *info);

void gem_port_enter_operational_state(void);
void gem_port_exit_operational_state(void);

#endif /* __PON_ETH_FW_GEM_H */
