/******************************************************************************
 *
 *  Copyright (c) 2025 MaxLinear, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

 #ifndef __PON_ETH_FW_ALLOC_H
#define __PON_ETH_FW_ALLOC_H

#include <linux/types.h>

/* Allocation (T-CONT) information */
struct alloc_info {
	/** ID, logical number
	 * do not change after allocation
	 */
	u16 id;
	/** index, assigned by FW or warmstart configuration */
	u16 idx;
	/** This link reference identifies a specific linking of an allocation
	 *  ID to an allocation's hardware index.
	 */
	u32 link_ref;
	/** This value identifies the dequeue port from which the data for
	 *  the Allocation ID shall be pulled by hardware.
	 */
	u16 qos_idx;
	/** Indicates if the qos_idx of the allocation is valid */
	bool valid;
};

struct gem_port_info;

struct alloc_info *alloc_id_allocate(u16 id, const struct alloc_info *preinit);

void alloc_id_free(struct alloc_info *info);

int alloc_qos_idx_link_set(struct alloc_info *info);

int alloc_qos_idx_link_new(struct alloc_info *info);
void alloc_qos_idx_unlink(struct alloc_info *info);

int alloc_add_gem_link(struct alloc_info *info, struct gem_port_info *gem_info);
void alloc_remove_gem_link(struct gem_port_info *gem_info);

void alloc_enter_operational_state(void);
void alloc_exit_operational_state(void);

int alloc_init(void (*flush_cb)(const struct alloc_info *info));
void alloc_uninit(void);

#endif /* __PON_ETH_FW_ALLOC_H */
