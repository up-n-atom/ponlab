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
#define pr_fmt(fmt) "pon_eth_fw_alloc: " fmt

#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/hashtable.h>
#include <linux/list.h>
#include <linux/rtnetlink.h>
#include <linux/rwsem.h>
#include <linux/slab.h> /* kmalloc, kfree */

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox_ikm.h>

#include "pon_eth_limits.h"
#include "pon_eth_fw_alloc.h"
#include "pon_eth_fw_gem.h"

struct alloc_storage {
	/** Allocation information, visible to the caller */
	struct alloc_info info;

	/** Node in the allocation hash table */
	struct hlist_node node;

	/* TODO: Check what kind of lookups and cross-references are needed */
	/** List of GEM ports associated with this allocation */
	struct list_head gem_list;
};

#define to_alloc_storage(i) container_of(i, struct alloc_storage, info)

/** Alloc IDX mask.
 *  The lower 8 bit from ALLOC_LINK_REF value are always identical to the
 *  ALLOC_IDX value
 */
#define ALLOC_IDX_MASK 0xFF

/** Number of bits for the hash table */
#define ALLOC_ID_HT_BITS 8
/* Hash table for allocations by id */
static DEFINE_HASHTABLE(alloc_id_hashtable, ALLOC_ID_HT_BITS);

#define ALLOC_IDX_HT_BITS 4
/* Hash table for allocations by idx */
//static DEFINE_HASHTABLE(alloc_idx_hashtable, ALLOC_IDX_HT_BITS);

/* Lock for synchronizing access to allocation hash table */
static DECLARE_RWSEM(alloc_table_lock);

/** This array identifies which QOS Indexes are currently in use.
 *
 * The array is indexed by the QoS index number.
 *
 * Changes to this array must be protected by the rtnl lock, related functions
 * have this documentation and must be called with rtnl lock held.
 */
static bool g_qos_idx_used[PON_ALLOC_MAX];

/** Indicates whether the PON interface is in operational state. */
static bool in_operational_state;

/** Empty callback function for flushing queues in datapath driver */
static void empty_function(const struct alloc_info *info)
{
	/* empty */
}

/** Callback function for flushing queues in datapath driver */
static void (*alloc_flush_cb)(const struct alloc_info *info) = empty_function;

static const struct alloc_id_range {
	u16 min;
	u16 max;
} alloc_id_ranges[] = {
	/* PON_MODE_984_GPON */
	[PON_MODE_984_GPON] = { .min = 256, .max = 4095 },
	/* PON_MODE_987_XGPON */
	[PON_MODE_987_XGPON] = { .min = 1024, .max = 16383 },
	/* PON_MODE_9807_XGSPON */
	[PON_MODE_9807_XGSPON] = { .min = 1024, .max = 16383 },
	/* PON_MODE_989_NGPON2_2G5 */
	[PON_MODE_989_NGPON2_2G5] = { .min = 1024, .max = 16383 },
	/* PON_MODE_989_NGPON2_10G */
	[PON_MODE_989_NGPON2_10G] = { .min = 1024, .max = 16383 },
};

/**
 * alloc_id_is_valid - Validate allocation ID based on current PON mode
 * @id: Allocation ID to validate
 *
 * This function checks if the provided allocation ID is valid
 * based on the active PON mode returned by pon_mbox_get_pon_mode().
 *
 * Return:
 *   0        - ID is valid
 *   -EINVAL  - ID is out of valid range for current PON mode
 *              or the mode is unsupported.
 */
static int alloc_id_is_valid(u16 id)
{
	enum pon_mode mode = pon_mbox_get_pon_mode();
	u16 min, max;

	if (mode >= ARRAY_SIZE(alloc_id_ranges)) {
		pr_err("system in unsupported pon mode\n");
		return -EINVAL;
	}

	min = alloc_id_ranges[mode].min;
	max = alloc_id_ranges[mode].max;

	/* Accept zero ID for the OMCI case */
	if (id == 0)
		return 0;

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

/**
 * alloc_qos_idx_free
 * @info: Allocation info structure. Must be non-NULL and partially initialized.
 *
 * Releases a previously assigned QoS index for an allocation.
 */
static void alloc_qos_idx_free(struct alloc_info *info)
{
	if (!info)
		return;

	if (info->qos_idx >= PON_ALLOC_MAX) {
		pr_warn("QOS Index (%u) out of range, cannot release for Alloc id (%u)\n",
			info->qos_idx, info->id);
		return;
	} else if (!info->qos_idx) {
		pr_debug("QOS Index free skipped for Alloc id (%u)\n",
			 info->id);
		return;
	}

	pr_debug("QOS Index (%u) freed for Alloc id (%u)\n", info->qos_idx,
		 info->id);
	g_qos_idx_used[info->qos_idx] = false;
	info->valid = false;
	info->qos_idx = 0;
}

static int alloc_qos_idx_link(struct alloc_info *info, bool read,
			      const struct ponfw_alloc_id_link *fw_alloc_in)
{
	struct ponfw_alloc_id_link fw_alloc_link = { 0 };
	int err;

	if (fw_alloc_in)
		fw_alloc_link = *fw_alloc_in;
	else
		fw_alloc_link.alloc_id = info->id;

	if (read) {
		fw_alloc_link.ctr = PONFW_ALLOC_ID_LINK_CTR_READ;
	} else {
		if (!info->valid) {
			pr_err("cannot link QOS Index (%u) for Alloc id (%i): invalid QOS Index\n",
			       info->qos_idx, info->id);
			return -EINVAL;
		}
		fw_alloc_link.ctr = PONFW_ALLOC_ID_LINK_CTR_LINK;
		fw_alloc_link.qos_idx = info->qos_idx;
	}

	err = pon_mbox_send(PONFW_ALLOC_ID_LINK_CMD_ID, PONFW_READ,
			    &fw_alloc_link, PONFW_ALLOC_ID_LINK_LENR,
			    &fw_alloc_link, sizeof(fw_alloc_link));
	if (err < 0) {
		/* ignore errors on read to handle not existing entries */
		if (!read)
			pr_err("failed to link QOS Index (%u) for Alloc id (%i): %i\n",
			       info->qos_idx, info->id, err);
		return err;
	}

	if (err != sizeof(fw_alloc_link)) {
		err = -EINVAL;
		pr_err("failed to get answer for link QOS Index (%u) for Alloc id (%i): %i\n",
		       info->qos_idx, info->id, err);
		return err;
	}
	info->idx = fw_alloc_link.alloc_link_ref & ALLOC_IDX_MASK;

	if (info->idx &&
	    fw_alloc_link.hw_status == PONFW_ALLOC_ID_LINK_HW_STATUS_LINKED) {
		info->link_ref = fw_alloc_link.alloc_link_ref;
		info->qos_idx = fw_alloc_link.qos_idx;
		/* not setting info->valid, as reading back does not make it
		 * automatically valid */
		pr_debug(
			"%slinked QOS Index (%u) for Alloc id (%i) with Version number (0x%x)\n",
			read ? "read " : "", info->qos_idx,
			info->id, info->link_ref);

		alloc_id_write_update(info->idx);
	}

	return 0;
}

/**
 * alloc_qos_idx_link_set - Establish link between allocation ID and QoS
 *                          index in firmware.
 * @info: Pointer to allocation info structure containing allocation ID
 *        and QoS index.
 *
 * This function establishes a link between an allocation ID and its
 * assigned QoS index by sending a link command to the firmware. The
 * operation is only performed if:
 * - The allocation index is non-zero (OMCI channel with idx 0 is
 *   skipped)
 * - The allocation has a valid QoS index assigned
 * - The link has not been previously established (link_ref is zero)
 *
 * The function sends a PONFW_ALLOC_ID_LINK_CMD_ID command to the
 * firmware with the allocation ID and QoS index. Upon successful
 * linking, the firmware returns a link reference number (version) and
 * the allocation index, which are stored in the info structure.
 *
 * Return:
 *   0 on success (link established or skipped for valid reasons)
 *   Negative error code on failure:
 *     -EINVAL if firmware communication fails or returns invalid data
 *     Other negative values propagated from alloc_qos_idx_link()
 */
int alloc_qos_idx_link_set(struct alloc_info *info)
{
	int err;

	/* Skip for OMCI channel. */
	if (info->idx == 0)
		return 0;

	/* Check if QOS Index is already linked to allocation */
	if (info->valid && info->link_ref) {
		pr_debug("QOS Index link skipped for Alloc id (%u)\n",
			 info->id);
		return 0;
	}

	err = alloc_qos_idx_link(info, false, NULL);
	if (err)
		return err;

	pr_debug("QOS Index (%u) link requested for Alloc id (%u)\n",
		 info->qos_idx, info->id);

	return 0;
}

static int alloc_qos_idx_link_get(struct alloc_info *info)
{
	int err;

	/* Check if QOS Index is linked to allocation and we already know the
	 * QOS Index
	 */
	if (info->link_ref && info->qos_idx) {
		pr_debug("QOS Index link get skipped for Alloc id (%u)\n",
			 info->id);
		return 0;
	}

	err = alloc_qos_idx_link(info, true, NULL);
	if (err)
		return err;

	/* no valid entry found, indicate with error */
	if (!info->link_ref)
		return -ENOENT;

	pr_debug(
		"QOS Index (%u) already linked for Alloc id (%u) with Version number (0x%x)\n",
		info->qos_idx, info->id, info->link_ref);

	return 0;
}

/**
 * alloc_qos_idx_reuse - Reuse previously assigned QoS index for an allocation.
 * @info: Allocation info structure. Must be non-NULL and partially initialized.
 *
 * Tries to reuse a previously assigned QoS index for the given allocation.
 * It checks if the QoS index is already assigned in firmware. If it is already
 * in use, it returns -EEXIST.
 *
 * Return:
 *  0 : QoS index successfully reused.
 * -EINVAL : Invalid parameter.
 * -EEXIST : QoS index already in use.
 */
static int alloc_qos_idx_reuse(struct alloc_info *info)
{
	int err;

	ASSERT_RTNL();

	if (!info)
		return -EINVAL;

	err = alloc_qos_idx_link_get(info);
	if (err)
		return err;

	if (info->qos_idx >= PON_ALLOC_MAX) {
		pr_err("QOS Index (%u) out of range for Alloc id (%u)\n",
		       info->qos_idx, info->id);
		return -EINVAL;
	}

	/* Forcing a QOS index is not possible if it is already in use. */
	if (g_qos_idx_used[info->qos_idx])
		return -EEXIST;

	/* Mark QOS index as used again */
	g_qos_idx_used[info->qos_idx] = true;
	info->valid = true;

	pr_debug("QOS Index (%u) reused for Alloc id (%u)\n",
		 info->qos_idx, info->id);

	return 0;
}

/**
 * alloc_qos_idx_link_new - Allocate (or reserve) a new QoS index/link entry.
 * @info: Allocation info structure. Must be non-NULL and partially initialized.
 *
 * This routine obtains a free QoS index and associates it with the allocation
 * context described by 'info'. It may update fields inside *info (such as the
 * acquired qos_idx and valid flag).
 * It also tries to assigne the qos_idx to the alloc id in the firmware.
 *
 * Failure modes & return codes (conventional expectations):
 *   == 0 : Success; 0 indicating success with the index stored in info.
 *   <  0 : Error; negative errno-style codes, e.g.:
 *          -EINVAL  Invalid arguments in 'info' (null pointer, bad fields).
 *          -ENOSPC  No available QoS indices left in the pool.
 *          -EFAULT  Internal inconsistency or failed validation.
 *          -EPERM   Operation not permitted (policy / permissions).
 *
 * Parameter validation:
 *   - info must be non-NULL.
 *   - info->valid flag must indicate if a qos_idx is already assigned.
 *   - info->id must be correct to assign the qos_idx to it.
 *
 * Returns:
 *   int: 0 on success;
 *        negative error code on failure.
 */
int alloc_qos_idx_link_new(struct alloc_info *info)
{
	int i, err;

	ASSERT_RTNL();

	if (!info)
		return -EINVAL;

	if (info->valid) {
		pr_debug("QOS Index already assigned for Alloc id (%u)\n",
			 info->id);
		return 0;
	}

	/* Check if the firmware has already a qos idx linked
	 * and try to reuse it.
	 */
	err = alloc_qos_idx_reuse(info);
	if (!err)
		return 0;

	/* if trying to reuse the qos_idx fails, the entry is already in use and
	 * we need to free it in firmware and request a new assignment below.
	 */
	alloc_qos_idx_unlink(info);

	/* QOS Index 0 is used by OMCI channel, so skip it */
	for (i = 1; i < ARRAY_SIZE(g_qos_idx_used); i++) {
		if (g_qos_idx_used[i])
			continue;

		g_qos_idx_used[i] = true;
		info->qos_idx = i;
		info->valid = true;

		pr_debug("QOS Index (%u) assigned for Alloc id (%u)\n",
			 info->qos_idx, info->id);

		return alloc_qos_idx_link_set(info);
	}

	pr_err("failed to assign new QOS Index for Alloc id (%u)\n", info->id);
	return -EINVAL;
}

/**
 * alloc_qos_idx_unlink - Unlink and free a QoS Index for an allocation.
 * @info:  Allocation metadata (must not be NULL).
 *
 * Removes linkage between an allocation (Alloc-ID) and its QoS Index, then
 * frees the QoS Index. Firmware unlink is sent only in_operational_state.
 * After success (or skipped FW command) local QoS Index is freed, allocation
 * bookkeeping updated, and link_ref cleared.
 *
 * Behavior:
 * - Returns immediately if info is NULL.
 * - Skips idx 0 (OMCI channel).
 * - Skips if link_ref is absent.
 * - In operational state: sends FW unlink; on failure logs error and aborts.
 * - On success (or skipped FW) frees QoS resources and clears link_ref.
 *
 * Side effects:
 * - May send mailbox command.
 * - Frees QoS Index via alloc_qos_idx_free().
 *
 * Caller must ensure synchronization.
 */
void alloc_qos_idx_unlink(struct alloc_info *info)
{
	struct ponfw_alloc_id_unlink fw_alloc_unlink = { 0 };
	int err;

	ASSERT_RTNL();

	if (!info)
		return;

	/* Skip for OMCI channel. */
	if (info->idx == 0)
		return;

	/* Skip if not linked. */
	if (!info->link_ref) {
		pr_debug("QOS Index unlink skipped for Alloc id (%u)\n",
			 info->id);
		return;
	}

	if (in_operational_state) {
		fw_alloc_unlink.alloc_id = info->id;
		fw_alloc_unlink.alloc_link_ref = info->link_ref;

		err = pon_mbox_send(PONFW_ALLOC_ID_UNLINK_CMD_ID, PONFW_WRITE,
				    &fw_alloc_unlink, sizeof(fw_alloc_unlink),
				    NULL, 0);
		if (err < 0) {
			pr_err("failed to unlink QOS Index (%u) for Alloc id (%i): %i\n",
			       info->qos_idx, info->id, err);
			return;
		}
	}

	alloc_qos_idx_free(info);
	alloc_id_remove_update(info->idx);

	pr_debug(
		"unlinked QOS Index (%u) for Alloc id (%i) with Version number (0x%x)\n",
		info->qos_idx, info->id, info->link_ref);
	info->link_ref = 0;
}

/**
 * alloc_add_gem_link - Associate a GEM port with an allocation ID.
 * @info: Pointer to allocation info structure to link the GEM port to.
 *        Must not be NULL.
 * @gem_info: Pointer to GEM port info structure to be linked. Must not
 *            be NULL.
 *
 * This function establishes a bidirectional association between an
 * allocation ID and a GEM port. The GEM port is added to the allocation's
 * list of associated GEM ports, and the GEM port's allocation reference
 * is set to point back to the allocation.
 *
 * The function performs validation to ensure:
 * - Both info and gem_info pointers are valid
 * - The GEM port is not already associated with another allocation
 *
 * When successful, the gem_info is added to the head of the allocation's
 * gem_list, and gem_info->alloc_info is set to reference the allocation.
 *
 * Context:
 *   Must be called with rtnl lock held (enforced by ASSERT_RTNL).
 *
 * Return:
 *   0 on success
 *   -EINVAL if either info or gem_info is NULL, or if to_alloc_storage
 *           conversion fails
 *   -EEXIST if the GEM port is already linked to an allocation
 *           (gem_info->alloc_info is not NULL)
 */
int alloc_add_gem_link(struct alloc_info *info, struct gem_port_info *gem_info)
{
	struct alloc_storage *as;

	ASSERT_RTNL();

	if (!info || !gem_info)
		return -EINVAL;

	as = to_alloc_storage(info);
	if (!as)
		return -EINVAL;

	/* do not allow adding multiple times */
	if (gem_info->alloc_info)
		return -EEXIST;

	/* Add the GEM port link */
	list_add(&gem_info->alloc_node, &as->gem_list);
	gem_info->alloc_info = info;

	return 0;
}

/**
 * alloc_remove_gem_link - Remove the association between a GEM port and
 *                         its allocation ID.
 * @gem_info: Pointer to GEM port info structure to be unlinked. May be
 *            NULL (function returns safely).
 *
 * This function removes the bidirectional link between a GEM port and
 * its associated allocation ID. If the GEM port is currently linked to
 * an allocation:
 * - Removes the GEM port from the allocation's gem_list
 * - Clears the gem_info->alloc_info back-reference to NULL
 *
 * If the GEM port is not linked to any allocation (alloc_info is NULL),
 * the function safely returns without performing any operation.
 *
 * Context:
 *   Must be called with rtnl lock held (enforced by ASSERT_RTNL).
 *
 * Return:
 *   None (void function)
 */
void alloc_remove_gem_link(struct gem_port_info *gem_info)
{
	ASSERT_RTNL();

	if (!gem_info)
		return;

	if (gem_info->alloc_info) {
		/* Remove the GEM port link */
		list_del(&gem_info->alloc_node);
		gem_info->alloc_info = NULL;
	}
}

/**
 * alloc_id_allocate - Create and register a new allocation entry by ID.
 * @id: Numeric allocation identifier (must pass alloc_id_is_valid()).
 * @preinit: Optional pointer to a preinitialized alloc_info. Contents
 *           (except @id) will be copied. May be NULL.
 *
 * Allocates and initializes a new allocation record, ensuring uniqueness
 * of @id:
 *   1. Validate @id.
 *   2. Acquire alloc_table_lock (write).
 *   3. Fail with -EEXIST if @id already exists.
 *   4. Allocate zeroed storage; fail with -ENOMEM if allocation fails.
 *   5. Copy @preinit (if provided) and set @id.
 *   6. Insert into alloc_id_hashtable.
 *
 * Concurrency:
 *   Write lock held for validation, existence check, allocation and insert.
 *
 * Errors:
 *   ERR_PTR(-EINVAL) if @id invalid.
 *   ERR_PTR(-EEXIST) if duplicate.
 *   ERR_PTR(-ENOMEM) if allocation fails.
 *
 * Return:
 *   Pointer to new struct alloc_info on success, else ERR_PTR(error).
 *
 * Lifetime:
 *   Returned alloc_info is owned by the allocation table; ref management
 *   defined elsewhere.
 */
struct alloc_info *alloc_id_allocate(u16 id, const struct alloc_info *preinit)
{
	struct alloc_storage *ele;
	int ret = 0;

	ret = alloc_id_is_valid(id);
	if (ret)
		return ERR_PTR(ret);

	down_write(&alloc_table_lock);

	/* Check if the allocation already exists */
	hash_for_each_possible(alloc_id_hashtable, ele, node, id) {
		if (ele->info.id == id) {
			up_write(&alloc_table_lock);
			return ERR_PTR(-EEXIST);
		}
	}

	/* Allocate and initialize a new allocation */
	ele = kzalloc(sizeof(*ele), GFP_KERNEL);
	if (!ele) {
		up_write(&alloc_table_lock);
		return ERR_PTR(-ENOMEM);
	}

	/* Initialize the allocation information */
	if (preinit)
		ele->info = *preinit;
	ele->info.id = id;

	if (id == 0) {
		/* Special handling for OMCI allocation */
		ele->info.idx = 0;
		ele->info.qos_idx = 0;
		ele->info.valid = true;
		alloc_id_write_update(0);
	}

	INIT_LIST_HEAD(&ele->gem_list);

	/* Insert the allocation into the hash table */
	hash_add(alloc_id_hashtable, &ele->node, id);
	up_write(&alloc_table_lock);
	return &ele->info;
}

/**
 * @brief Release an allocation identifier and associated resources.
 *
 * This helper reverses a previous successful allocation performed for
 * the provided alloc_info instance. After calling this function the
 * alloc_info object will no longer hold a valid allocation ID and any
 * internal bookkeeping (reference counts, lists, pools, maps, etc.)
 * related to that ID is updated accordingly.
 *
 * If the allocation ID was already freed or is invalid, the function
 * performs a safe no-op (or logs a warning, depending on build / debug
 * configuration). Callers should not attempt to use the freed ID after
 * this returns.
 *
 * Concurrency:
 * - Expected to be called while holding the lock protecting the
 *   allocation table (if the wider subsystem is multi-threaded).
 * - Not thread-safe by itself; external synchronization is required
 *   unless documented otherwise in the subsystem.
 *
 * Error handling:
 * - Typically does not fail; any internal failure conditions are handled
 *   internally (e.g., logging) because freeing should be best-effort.
 *
 * Performance:
 * - Designed to be O(1) or near constant time by using direct lookup
 *   structures (e.g., arrays or hash maps) maintained at allocation time.
 *
 * @param info Pointer to the allocation info structure whose allocation
 *             identifier is to be released. Must not be NULL.
 *
 * @note After this call, fields inside alloc_info that reference the
 *       allocation (ID, state flags, pointers) may be reset to sentinel
 *       values (e.g., -1, NULL, false).
 * @warning Do not double-free: ensure each successful allocation is
 *          matched by exactly one call to alloc_id_free().
 */
void alloc_id_free(struct alloc_info *info)
{
	struct alloc_storage *ele;

	if (!info)
		return;

	down_write(&alloc_table_lock);

	ele = to_alloc_storage(info);
	hash_del(&ele->node);
	kfree(ele);

	up_write(&alloc_table_lock);
}

/**
 * alloc_enter_operational_state - Link all existing allocation entries to
 * their QoS indexes.
 *
 * Invoked when transitioning the allocation subsystem into operational
 * mode. Iterates the global allocation ID hash table. For every valid
 * allocation without an established link reference (info.link_ref == 0),
 * attempts to bind it to its QoS index via alloc_qos_idx_link_set().
 *
 * Locking:
 * Uses a read lock (down_read) on alloc_table_lock for the traversal.
 *
 * Error Handling:
 * On failure to link, logs an error with pr_err() and continues.
 *
 * Concurrency:
 * New allocations added after traversal begins may be skipped. Removals
 * must ensure safe iteration via higher-level synchronization.
 *
 * Side Effects:
 * Successful links update link_ref in the alloc_info.
 *
 * Return:
 * None. Uses logging for status reporting.
 */
void alloc_enter_operational_state(void)
{
	struct alloc_storage *ele;
	int bkt;

	pr_debug("entering operational state, linking allocations\n");

	in_operational_state = true;

	down_read(&alloc_table_lock);

	/* Iterate over all allocations and link their QOS indexes */
	hash_for_each(alloc_id_hashtable, bkt, ele, node) {
		if (!ele->info.link_ref) {
			int err = alloc_qos_idx_link_set(&ele->info);

			if (err)
				pr_err("failed to link QOS Index (%u) for Alloc id (%i): %i\n",
				       ele->info.qos_idx, ele->info.id, err);
		}
	}

	up_read(&alloc_table_lock);
}

void alloc_exit_operational_state(void)
{
	pr_debug("exiting operational state\n");

	in_operational_state = false;
}

static struct alloc_info *alloc_id_get_by_id(u16 id)
{
	struct alloc_storage *ele;

	down_read(&alloc_table_lock);

	hash_for_each_possible(alloc_id_hashtable, ele, node, id) {
		if (ele->info.id == id) {
			up_read(&alloc_table_lock);
			return &ele->info;
		}
	}

	up_read(&alloc_table_lock);
	return NULL;
}

static int alloc_linked_gem_ports_update(struct alloc_info *info, bool activate)
{
	struct alloc_storage *as;
	struct gem_port_info *gem_info, *tmp;
	int err = 0;

	if (!info)
		return -EINVAL;

	as = to_alloc_storage(info);
	if (!as)
		return -EINVAL;

	/* safe iteration over gem_info list needed as it may be modified */
	list_for_each_entry_safe(gem_info, tmp, &as->gem_list, alloc_node) {
		if (!in_operational_state && !activate) {
			pr_debug("%s: remove gem port %d from Alloc id (%i) in non-operational state\n",
				 __func__, gem_info->id, info->id);
			/* just remove the link from the list */
			alloc_remove_gem_link(gem_info);
			continue;
		}

		err = gem_port_fw_apply(gem_info, activate ? info : NULL);
		if (err) {
			pr_err("failed to %s GEM port (%u) for Alloc id (%i): %i\n",
			       activate ? "activate" : "deactivate",
			       gem_info->id, info->id, err);
			return err;
		}
	}

	return 0;
}

static void alloc_id_link_event(void *module, const void *msg, size_t msg_len,
				u8 seq)
{
	const struct ponfw_alloc_id_link *fw_alloc = msg;
	struct alloc_info *info;
	int err;

	if (msg_len != sizeof(*fw_alloc)) {
		pr_err("ALLOC_ID_LINK event: wrong message size!\n");
		return;
	}

	pr_debug(
		"%s: received ALLOC_ID_LINK event, alloc_idx: %i, alloc_id: %i, alloc_link_ref: 0x%x, qos_idx: %i\n",
		__func__, fw_alloc->alloc_idx, fw_alloc->alloc_id,
		fw_alloc->alloc_link_ref, fw_alloc->qos_idx);

	rtnl_lock();

	info = alloc_id_get_by_id(fw_alloc->alloc_id);
	if (!info) {
		pr_debug("ALLOC_ID_LINK event: alloc_id %i does not exist (yet)\n",
		       fw_alloc->alloc_id);
		goto out_unlock;
	}

	/** Attempt to reactivate US connections by requesting QOS index link
	 *  with allocation ID again after PLOAM Allocation ID activation event.
	 */
	if (fw_alloc->link_status == PONFW_ALLOC_ID_LINK_LINK_STATUS_ASSIGNED) {
		err = alloc_qos_idx_link(info, false, fw_alloc);
		if (err || !info->link_ref) {
			pr_err("failed to link QOS Index (%u) for Alloc id (%i): %i\n",
			       info->qos_idx, info->id, err);

			goto out_unlock;
		}
	}

	err = alloc_linked_gem_ports_update(info, true);
	if (err) {
		pr_err("failed to activate GEM ports for Alloc id (%i): %i\n",
		       info->id, err);
		goto out_unlock;
	}

	alloc_id_write_update(info->idx);

out_unlock:
	rtnl_unlock();
}

static void alloc_id_clear_all(void)
{
	struct alloc_storage *ele;
	struct alloc_info *info;
	int bkt;

	down_read(&alloc_table_lock);

	/* Iterate over all allocations and flush their queues and then disable
	 * them with all linked GEM ports.
	 */
	hash_for_each(alloc_id_hashtable, bkt, ele, node) {
		info = &ele->info;

		alloc_flush_cb(info);
		alloc_qos_idx_unlink(info);
		(void)alloc_linked_gem_ports_update(info, false);
	}

	up_read(&alloc_table_lock);
}

static void alloc_id_unlink_event(void *module, const void *msg, size_t msg_len,
				  u8 seq)
{
	const struct ponfw_alloc_id_unlink *fw_alloc = msg;
	struct alloc_info *info;
	int err;

	if (msg_len != sizeof(*fw_alloc)) {
		pr_err("ALLOC_ID_UNLINK event: wrong message size!\n");
		return;
	}

	if (fw_alloc->all)
		pr_debug(
			"%s: received ALLOC_ID_UNLINK event for ALL allocations\n",
			__func__);
	else
		pr_debug(
			"%s: received ALLOC_ID_UNLINK event, alloc_id: %i, alloc_link_ref: 0x%x\n",
			__func__, fw_alloc->alloc_id, fw_alloc->alloc_link_ref);

	/* acknowledge the event message */
	err = pon_mbox_send_ack(PONFW_ALLOC_ID_UNLINK_CMD_ID, PONFW_WRITE, seq);
	if (err < 0) {
		pr_err("ALLOC_ID_UNLINK event: failed to send acknowledgment\n");
		return;
	}

	rtnl_lock();

	if (fw_alloc->all) {
		/* Unlink all allocations is requested only when leaving
		 * operational state. Set internal state already here to avoid
		 * sending gem port update while not in operational state.
		 */
		in_operational_state = false;

		alloc_id_clear_all();

		/* Sending full message back to firmware to acknowledge that all
		 * cleanup actions are done
		 */
		err = pon_mbox_send(PONFW_ALLOC_ID_UNLINK_CMD_ID, PONFW_WRITE,
				    fw_alloc, sizeof(*fw_alloc), NULL, 0);
		if (err < 0) {
			/* Ignore errors, only dump for debugging */
			pr_debug(
				"pon_eth: ALLOC_ID_UNLINK (all) ack was rejected by FW: %i (ignored)\n",
				err);
		}

		goto out_unlock;
	}

	info = alloc_id_get_by_id(fw_alloc->alloc_id);
	if (!info) {
		pr_err("ALLOC_ID_UNLINK event: alloc_id %i does not exist\n",
		       fw_alloc->alloc_id);
		goto out_unlock;
	}

	alloc_flush_cb(info);
	alloc_qos_idx_unlink(info);
	(void)alloc_linked_gem_ports_update(info, false);

	pr_debug(
		"pon_eth: processed ALLOC_ID_UNLINK event, alloc_id: %u, alloc_link_ref: 0x%x\n",
		fw_alloc->alloc_id, fw_alloc->alloc_link_ref);

out_unlock:
	rtnl_unlock();
}

/**
 * alloc_init - Initialize the allocation ID management subsystem.
 * @flush_cb: Callback function for flushing queues in datapath driver.
 *            If NULL, an empty dummy function is used instead. The
 *            callback is invoked when an allocation needs to flush its
 *            associated queues before unlinking (e.g., during cleanup
 *            or state transitions).
 *
 * This function initializes the allocation ID management subsystem by:
 * - Registering the provided flush callback (or empty function if NULL)
 *   for datapath queue cleanup operations
 * - Registering firmware event handlers for allocation ID lifecycle:
 *   * ALLOC_ID_LINK events - handled by alloc_id_link_event()
 *     Processes allocation activation and QoS index linking
 *   * ALLOC_ID_UNLINK events - handled by alloc_id_unlink_event()
 *     Processes allocation deactivation and cleanup
 *
 * The event handlers use the address of g_qos_idx_used as a unique
 * module reference for later unregistration via alloc_uninit().
 *
 * This function should be called once during module initialization,
 * before any allocation operations are performed. It must be paired
 * with a corresponding call to alloc_uninit() during module cleanup.
 *
 * Context:
 *   Can be called from module init context. Does not acquire locks.
 *   Safe to call before any allocations are created.
 *
 * Return:
 *   0 on success (current implementation always succeeds)
 */
int alloc_init(void (*flush_cb)(const struct alloc_info *info))
{
	pr_debug("Init alloc handling\n");

	if (!flush_cb)
		alloc_flush_cb = empty_function;
	else
		alloc_flush_cb = flush_cb;

	/* use 'g_qos_idx_used' address, as we do not have other per-module
	 * data that could be used as reference. This gives a unique module
	 * reference for unregistering the event handlers later.
	 */
	pon_mbox_register_event_handler(PONFW_ALLOC_ID_LINK_CMD_ID,
					alloc_id_link_event, &g_qos_idx_used);
	pon_mbox_register_event_handler(PONFW_ALLOC_ID_UNLINK_CMD_ID,
					alloc_id_unlink_event, &g_qos_idx_used);

	return 0;
}

/**
 * alloc_uninit - Cleanup the allocation ID management subsystem.
 *
 * This function performs cleanup of the allocation ID management
 * subsystem during module shutdown. It:
 * - Resets the flush callback to the empty dummy function
 * - Unregisters all firmware event handlers that were registered
 *   during alloc_init() by using the g_qos_idx_used module reference
 *
 * This function should be called once during module cleanup/exit,
 * after all allocation operations have been completed and no further
 * allocations will be created. It must be paired with a prior call
 * to alloc_init().
 *
 * Note: This function does not free existing allocations or clean up
 * the allocation hash table. Callers must ensure all allocations are
 * properly freed via alloc_id_free() before calling this function.
 *
 * Context:
 *   Can be called from module exit context. Does not acquire locks.
 *   Should be called when no allocation operations are in progress.
 *
 * Return:
 *   None (void function)
 */
void alloc_uninit(void)
{
	pr_debug("Uninit alloc handling\n");

	alloc_flush_cb = empty_function;

	pon_mbox_unregister_event_handler_module(&g_qos_idx_used);
}
