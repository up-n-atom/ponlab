/******************************************************************************
 *
 *  Copyright (c) 2020 - 2025 MaxLinear, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

/* define prefix for pr functions */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/errno.h>
#include <linux/list.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h> /* kmalloc, kfree */

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox_ikm.h>

#include "pon_eth_limits.h"
#include "pon_eth_fw_alloc.h"
#include "pon_eth_fw_gem.h"

/* Internal structure to store GEM port information along with
 * housekeeping data.
 */
struct gem_storage {
	/** GEM port information, will be updated from caller */
	struct gem_port_info info;

	/** Linked list node for GEM storage.
	 * Contains all allocated GEM ports.
	 * Used for checking duplicate IDs.
	 */
	struct list_head node;
};

#define to_gem_storage(i) container_of(i, struct gem_storage, info)

/** This array identifies which GEM Indexes are currently used by
 *  GEM netdevices.
 */
static bool g_gem_idx_used[PON_GEM_MAX];

/* List of all allocated GEM ports */
static LIST_HEAD(allocated_gem_list);

/** Indicates whether the PON interface is in operational state. */
static bool in_operational_state;

/** Valid GEM ID ranges for different PON modes */
static const struct gem_id_range {
	u16 min;
	u16 max;
} gem_id_ranges[] = {
	/* PON_MODE_984_GPON */
	[PON_MODE_984_GPON] = { .min = 0, .max = 4095 },
	/* PON_MODE_987_XGPON */
	[PON_MODE_987_XGPON] = { .min = 1023, .max = 65534 },
	/* PON_MODE_9807_XGSPON */
	[PON_MODE_9807_XGSPON] = { .min = 1021, .max = 65534 },
	/* PON_MODE_989_NGPON2_2G5 */
	[PON_MODE_989_NGPON2_2G5] = { .min = 1021, .max = 65534 },
	/* PON_MODE_989_NGPON2_10G */
	[PON_MODE_989_NGPON2_10G] = { .min = 1021, .max = 65534 },
};

/**
 * gem_is_valid_id - Validate a GEM (GPON/XG(S)PON/NGPON2) port identifier for
 * the current PON mode.
 * @id: Candidate GEM port ID (u16) to validate.
 *
 * The valid inclusive ranges depend on the active PON mode returned by
 * pon_mbox_get_pon_mode():
 *
 * Return:
 *   0        - ID is valid for the current PON mode.
 *   -EINVAL  - ID is out of the allowed range for that mode,
 *              or the mode is unsupported.
 */
static int gem_is_valid_id(u16 id)
{
	enum pon_mode mode = pon_mbox_get_pon_mode();
	u16 min, max;

	if (mode >= ARRAY_SIZE(gem_id_ranges)) {
		pr_err("system in unsupported pon mode\n");
		return -EINVAL;
	}

	min = gem_id_ranges[mode].min;
	max = gem_id_ranges[mode].max;

	/* check for empty range, no valid IDs */
	if (min == 0 && max == 0) {
		pr_err("system in unsupported pon mode\n");
		return -EINVAL;
	}
	if (id < min || id > max) {
		pr_err("id (%i) not in valid range [%u ... %u]\n", id, min,
		       max);
		return -EINVAL;
	}
	return 0;
}

/** check for unique id */
static int gem_port_id_unique_check(u16 id)
{
	struct gem_storage *ele;

	list_for_each_entry(ele, &allocated_gem_list, node) {
		/* Check if the ID is already assigned */
		if (ele->info.id == id)
			return -EEXIST;
	}
	return 0;
}

/**
 * gem_port_info_free - Free the GEM port info object.
 * @info: Pointer to gem_port_info previously obtained (may be NULL).
 *
 * This function frees the GEM port info object and its associated storage.
 *
 * Concurrency / locking:
 *   Caller must hold RTNL lock.
 *
 * Return:
 *   None.
 */
void gem_port_info_free(struct gem_port_info *info)
{
	struct gem_storage *gs;

	if (!info)
		return;

	ASSERT_RTNL();

	alloc_remove_gem_link(info);

	g_gem_idx_used[info->idx] = false;
	gs = to_gem_storage(info);
	/* Remove from allocated GEM list */
	list_del(&gs->node);
	kfree(gs);
}

/**
 * gem_port_allocate - Allocate and register a GEM port entry by ID
 * @id: GEM port identifier to allocate (must satisfy gem_is_valid_id()).
 * @param preinit: Optional pre-initialization data for the GEM port.
 *
 * Allocates and initializes a new GEM port (struct gem_storage) wrapper and
 * exposes its embedded struct gem_port_info to the caller.
 *
 * Concurrency / locking:
 *   Caller must hold RTNL lock.
 *
 * Return:
 *   Pointer to struct gem_port_info on success.
 *   ERR_PTR(<neg error>) on failure:
 *     -EINVAL (or other code from gem_is_valid_id()): @id failed validation.
 *     -EEXIST: An entry with the given @id already exists.
 *     -ENOMEM: Memory allocation failed.
 *
 * Notes:
 *   - No partial initialization is exposed: either fully inserted or an ERR_PTR
 *     is returned.
 *   - Caller must not assume zeroed memory beyond what kzalloc provides and
 *     should complete any additional field initialization required by higher
 *     layers before making the port available externally.
 *   - The returned pointer is managed internally and must not be freed by the
 *     caller. The lifetime of the object is handled by the GEM subsystem,
 *     and freeing must be done by calling gem_port_info_free().
 */
struct gem_port_info *gem_port_allocate(u16 id,
					const struct gem_port_info *preinit)
{
	int ret = 0;
	struct gem_storage *ele;

	/** id not known for OMCI gem, skip validation if 0 */
	if (id) {
		ret = gem_is_valid_id(id);
		if (ret)
			return ERR_PTR(ret);
	}

	ASSERT_RTNL();

	ret = gem_port_id_unique_check(id);
	if (ret)
		return ERR_PTR(ret);

	ele = kzalloc(sizeof(*ele), GFP_KERNEL);
	if (!ele)
		return ERR_PTR(-ENOMEM);

	if (preinit)
		ele->info = *preinit;
	ele->info.id = id;

	/* Insert into allocated GEM list */
	list_add(&ele->node, &allocated_gem_list);
	return &ele->info;
}

/** Pre-assign a GEM port index
 * @param info		GEM port information
 * @return		0 on success, negative error code on failure
 *
 * The function assigns an available GEM index to the provided GEM port,
 * to be used if not in operational state yet.
 */
static int gem_port_idx_preassign(struct gem_port_info *info)
{
	struct gem_storage *gs;
	int i;

	/* If the GEM port index is already assigned, we are done */
	if (info->idx)
		return 0;

	gs = to_gem_storage(info);

	ASSERT_RTNL();

	/* GEM Index 0 is used by OMCI channel, so skip it */
	for (i = 1; i < PON_GEM_MAX; i++) {
		if (!g_gem_idx_used[i]) {
			gs->info.idx = i;
			g_gem_idx_used[i] = true;
			break;
		}
	}
	return 0;
}

static int gem_port_omci_update_id(struct gem_port_info *info)
{
	struct ponfw_gem_port_idx fw_gem_port_idx = { 0 };
	struct ponfw_gem_port_idx *fw_gem = &fw_gem_port_idx;
	int err;

	/* Set GEM port index for OMCI GEM, should be 0 by definition */
	fw_gem->gem_port_idx = info->idx;

	/* Retrieve ID for OMCI GEM from FW */
	err = pon_mbox_send(PONFW_GEM_PORT_IDX_CMD_ID, PONFW_READ, fw_gem,
			    PONFW_GEM_PORT_IDX_LENR, fw_gem, sizeof(*fw_gem));
	if (err != sizeof(*fw_gem)) {
		pr_err("failed to get GEM id for GEM idx (%i): %i\n",
		       fw_gem->gem_port_idx, err);
		return -EINVAL;
	}
	/* Update assigned ID from FW response */
	info->id = fw_gem->gem_port_id;
	return 0;
};

/**
 * gem_port_fw_apply - Apply GEM port configuration to firmware.
 * @info: Pointer to GEM port info structure to configure. Must not be
 *        NULL.
 * @alloc: Pointer to allocation info to associate with this GEM port.
 *         Can be NULL if no allocation association is needed.
 *
 * This function configures a GEM port in the firmware with the settings
 * from the provided info structure. The behavior depends on the system
 * state and GEM port type:
 *
 * For OMCI GEM ports (TT_OMCI):
 * - Retrieves the GEM port ID from firmware using the GEM index
 * - Updates info->id with the firmware-assigned value
 *
 * For other GEM port types:
 * - In non-operational state: Pre-assigns a GEM index from the local
 *   pool without contacting firmware
 * - In operational state: Sends configuration to firmware including:
 *   * GEM port ID, max size, traffic type, direction, encryption
 *   * Optional allocation ID and link reference (if alloc provided)
 *   * Optional pre-assigned index for warmstart scenarios
 *
 * On successful firmware configuration:
 * - Updates the GEM-to-allocation association (links/unlinks as needed)
 * - Retrieves and stores the firmware-assigned GEM index if not already
 *   assigned
 * - Marks the GEM index as used in g_gem_idx_used array
 *
 * Context:
 *   Must be called with rtnl lock held (enforced by ASSERT_RTNL).
 *
 * Return:
 *   0 on success
 *   -EINVAL if info is NULL, or firmware communication fails
 *   Other negative error codes from alloc_add_gem_link() or firmware
 */
int gem_port_fw_apply(struct gem_port_info *info, struct alloc_info *alloc)
{
	struct ponfw_gem_port_id fw_gem_port_id = { 0 };
	struct ponfw_gem_port_id *fw_gem = &fw_gem_port_id;
	int err;

	if (!info)
		return -EINVAL;

	ASSERT_RTNL();

	if (info->traffic_type == PONFW_GEM_PORT_ID_TT_OMCI)
		return gem_port_omci_update_id(info);

	if (!in_operational_state) {
		err = gem_port_idx_preassign(info);
		return err;
	}

	fw_gem->gem_port_id = info->id;
	fw_gem->max_gem_size = info->max_size;
	fw_gem->tt = info->traffic_type;
	fw_gem->dir = PONFW_GEM_PORT_ID_DIR_DS;
	fw_gem->enc = info->enc;
	fw_gem->use_idx = PONFW_GEM_PORT_ID_USE_IDX_DIS;

	if (info->idx) {
		/* Warmstart: If the GEM port was not removed while leaving the
		 * operational state, we need to configure it to the same
		 * GEM port index value as before.
		 */
		fw_gem->gem_port_idx = info->idx;
		fw_gem->use_idx = PONFW_GEM_PORT_ID_USE_IDX_EN;
	}

	if (alloc && alloc->link_ref) {
		fw_gem->alloc_id = alloc->id;
		fw_gem->alloc_link_ref = alloc->link_ref;
		fw_gem->dir = info->dir;
	}

	err = pon_mbox_send(PONFW_GEM_PORT_ID_CMD_ID, PONFW_WRITE, fw_gem,
			    PONFW_GEM_PORT_ID_LENW, NULL, 0);
	if (err < 0) {
		pr_err("%s: set gem port %d failed with %d\n", __func__,
		       info->id, err);
		return err;
	}

	/* Check if alloc_info is changing and update the reference to this
	 * gem-port in the old and new alloc_info (which can be NULL).
	 */
	if (info->alloc_info != alloc) {
		alloc_remove_gem_link(info);
		if (alloc) {
			err = alloc_add_gem_link(alloc, info);
			if (err < 0) {
				pr_err("%s: link gem port %d to alloc %d failed with %d\n",
				       __func__, info->id, alloc->id, err);
				return err;
			}
		}
	}
	/* If the GEM port index is already assigned, we are done */
	if (fw_gem->gem_port_idx)
		return 0;

	/* Retrieve assigned index from FW */
	err = pon_mbox_send(PONFW_GEM_PORT_ID_CMD_ID, PONFW_READ, fw_gem,
			    PONFW_GEM_PORT_ID_LENR, fw_gem, sizeof(*fw_gem));
	if (err != sizeof(*fw_gem)) {
		pr_err("failed to get GEM idx for GEM id (%i): %i\n",
		       fw_gem->gem_port_id, err);
		return -EINVAL;
	}

	/* Update assigned index from FW response */
	info->idx = fw_gem->gem_port_idx;
	g_gem_idx_used[info->idx] = true;
	return 0;
}

/**
 * gem_port_fw_free - Remove GEM port configuration from firmware.
 * @info: Pointer to GEM port info structure to remove. May be NULL
 *        (function returns safely).
 *
 * This function removes a GEM port configuration from the firmware and
 * cleans up associated resources. The behavior depends on the system
 * state and GEM port type:
 *
 * For OMCI GEM ports (TT_OMCI):
 * - Skips firmware removal (OMCI GEM is managed differently)
 * - Proceeds directly to cleanup
 *
 * For other GEM port types:
 * - Removes the GEM-to-allocation association via alloc_remove_gem_link()
 * - In non-operational state: Skips firmware communication, logs debug
 *   message
 * - In operational state: Sends PONFW_GEM_PORT_ID_REMOVE_CMD_ID to
 *   firmware to remove the GEM port configuration
 *
 * On all paths (success or failure):
 * - Marks the GEM index as unused in g_gem_idx_used array
 * - Does not free the gem_port_info structure itself (caller must use
 *   gem_port_info_free())
 *
 * Error handling:
 * - Firmware removal failures are logged but do not prevent cleanup
 * - Function continues with local cleanup even if firmware command fails
 *
 * Context:
 *   Must be called with rtnl lock held (enforced by ASSERT_RTNL).
 *
 * Return:
 *   None (void function)
 */
void gem_port_fw_free(struct gem_port_info *info)
{
	struct ponfw_gem_port_id_remove fw_gem_port_id_remove = { 0 };
	struct ponfw_gem_port_id_remove *fw_gem = &fw_gem_port_id_remove;
	int err;

	if (!info)
		return;

	ASSERT_RTNL();

	g_gem_idx_used[info->idx] = false;

	if (info->traffic_type == PONFW_GEM_PORT_ID_TT_OMCI)
		return;

	alloc_remove_gem_link(info);

	if (!in_operational_state) {
		pr_debug("%s: skip removing gem port %d in non-operational state\n",
			 __func__, info->id);
		return;
	}

	fw_gem->gem_port_id = info->id;
	err = pon_mbox_send(PONFW_GEM_PORT_ID_REMOVE_CMD_ID, PONFW_WRITE,
			    fw_gem, sizeof(*fw_gem), fw_gem, sizeof(*fw_gem));
	if (err < 0)
		pr_err("%s: remove gem port %d failed with %d\n", __func__,
		       info->id, err);
}

void gem_port_enter_operational_state(void)
{
	struct gem_storage *ele, *tmp;

	ASSERT_RTNL();

	in_operational_state = true;

	/* Apply all GEM ports to FW */
	list_for_each_entry_safe(ele, tmp, &allocated_gem_list, node) {
		gem_port_fw_apply(&ele->info, ele->info.alloc_info);
	}
}

void gem_port_exit_operational_state(void)
{
	struct gem_storage *ele, *tmp;

	ASSERT_RTNL();

	in_operational_state = false;

	/* Free all GEM ports from FW */
	list_for_each_entry_safe(ele, tmp, &allocated_gem_list, node) {
		gem_port_fw_free(&ele->info);
	}
}
