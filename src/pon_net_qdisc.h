/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This contains structures to manage qdiscs
 */

#ifndef _PON_NET_QDISC_H_
#define _PON_NET_QDISC_H_

struct pon_net_context;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

enum class_state {
	CLASS_UNUSED = 0,
	CLASS_USED = 1,
};

/** Qdisc traffic class */
struct qdisc_class {
	/* Index of child qdisc in pon_net_qdiscs */
#define CHILD_PRIORITY_QUEUE (unsigned int)(-1)
	unsigned int child_idx;
	/* Is this class used */
	enum class_state state;
	/* Parameter. For drr qdisc this is weight */
	uint32_t arg;
};

/** Represents a qdisc */
struct pon_net_qdisc {
	/** Index of parent in pon_net_qdiscs array. */
	unsigned int idx;
	/**
	 * Index of parent in pon_net_qdiscs array.
	 * If root then this is same as idx
	 */
	unsigned int parent_idx;
	/** Qdisc ops, which determine the type of qdisc*/
	const struct pon_net_qdisc_type *type;
	/** Type private data */
	void *type_data;
	/** Qdisc handle */
	uint32_t handle;
	/** Parent handle. If qdisc is root then this is 0 */
	uint32_t parent;
#define MAX_QUEUES 8
	/**
	 * Parent queue.  If parent is 0, then this will be ignored by the
	 * qdisc creation routines
	 * Valid values are from 0 to MAX_QUEUES
	 */
	uint32_t parent_queue;
#define DEF_DRR_WEIGHT 4
	/**
	 * This is a specific values used for creating a class
	 * Example: for drr class this is quantum
	 */
	uint32_t parent_arg;
#define MAX_CLASSES_PER_QDISC 64
	/** Array of qdisc classes */
	struct qdisc_class classes[MAX_CLASSES_PER_QDISC];
};

/** A tree of qdisc */
struct pon_net_qdiscs {
/* Maximum number of supported qdiscs */
#define MAX_QDISCS 256
	/* Stores handles of created qdiscs that represent traffic schedulers */
	struct pon_net_qdisc qdiscs[MAX_QDISCS];
	/* Length of the qdisc array */
	unsigned int length;
};

/** Calculate quantum for given weight
 *
 *  \param[in] weight Weight
 *
 *  \return returns quantum value
 */
uint32_t pon_net_calculate_quantum(uint8_t weight);

/** Return number of classes that qdisc has
 *
 *  \param[in]	qdisc		Qdisc
 *
 *  \return returns number of active classes
 */
unsigned int pon_net_qdisc_classes_len(const struct pon_net_qdisc *qdisc);

/** Return last occupied class
 *
 *  \param[in]	qdisc		Qdisc
 *
 *  \return returns number of active classes
 */
unsigned int pon_net_qdisc_classes_last(const struct pon_net_qdisc *qdisc);

struct pon_net_qdiscs;
/* Structure representing the qdisc type */
struct pon_net_qdisc_type {
	/* Create qdisc */
	enum pon_adapter_errno (*create)(struct pon_net_context *ctx,
					 const char *ifname,
					 const struct pon_net_qdisc *qdisc);
	/* Remove qdisc */
	enum pon_adapter_errno (*destroy)(struct pon_net_context *ctx,
					  const char *ifname,
					  const struct pon_net_qdisc *qdisc);

	/* Create a class for qdisc */
	enum pon_adapter_errno
	(*class_create)(struct pon_net_context *ctx,
			const char *ifname,
			const struct pon_net_qdisc *qdisc,
			unsigned int i);

	/* Destroy a class for qdisc */
	enum pon_adapter_errno
	(*class_destroy)(struct pon_net_context *ctx,
			 const char *ifname,
			 const struct pon_net_qdisc *qdisc,
			 unsigned int i);

	/* Length of the private qdisc type data */
	const unsigned int data_len;

	/** Compares the qdisc type private data in order to detect changes */
	int (*data_eq)(void *a, void *b);

	/**
	 * Perform type specific adjustments, after qdisc partitioning, like
	 * qdisc type change or drr quantum adjustments
	 */
	void (*after_partition)(struct pon_net_qdiscs *qdiscs,
				unsigned int qdisc_idx);
};

/** Change the type of qdisc
 *
 *  \param[in]  qdisc		Qdisc
 *  \param[in]  type		Type of qdisc
 *  \param[in]  params		Type's private data
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_qdisc_type_set(struct pon_net_qdisc *qdisc,
		       const struct pon_net_qdisc_type *type,
		       const void *params);

/** A tree of qdiscs */
struct pon_net_qdiscs;

/** Create a tree of qdiscs
 *
 *  \return returns new instance of empty qdisc tree
 */
struct pon_net_qdiscs *pon_net_qdiscs_create(void);

/** Get qdisc by its handle
 *
 *  \param[in]  tree		Collection of qdiscs
 *  \param[in]  qdisc_handle	Qdisc handle
 *
 *  \return returns a qdisc or NULL if no qdisc corresponds to given
 *          handle
 */
struct pon_net_qdisc *qdiscs_get_by_handle(struct pon_net_qdiscs *tree,
					   uint16_t qdisc_handle);

/** Get qdisc by its index in the tree
 *
 *  \param[in]  qdiscs		Collection of qdiscs
 *  \param[in]  qdisc_idx	Qdisc index
 *
 *  \return returns a qdisc or NULL if no qdisc corresponds to given
 *          index number
 */
struct pon_net_qdisc *pon_net_qdiscs_get(struct pon_net_qdiscs *qdiscs,
					 unsigned int qdisc_idx);

/** Compute the difference between 2 collections of qdiscs and apply
 *  this difference in configuration to the TC
 *
 *  \param[in]  qdiscs		Collection of qdiscs
 *  \param[in]  old_qdiscs	"Old" tree of qdiscs
 *  \param[in]  ifname		Network interface name
 *  \param[in]  ctx		PON Net Lib context
 *
 *  \return returns a qdisc or NULL if no qdisc corresponds to given
 *          index number
 */
enum pon_adapter_errno
pon_net_qdiscs_diff_apply(const struct pon_net_qdiscs *qdiscs,
			  const struct pon_net_qdiscs *old_qdiscs,
			  const char *ifname, struct pon_net_context *ctx);

/** Swap contents of 2 qdisc collections
 *
 *  \param[in]  dst	First tree
 *  \param[in]  src	Second tree
 */
void pon_net_qdiscs_swap(struct pon_net_qdiscs *dst,
			 struct pon_net_qdiscs *src);

/** Destroy tree of qdiscs
 *
 *  This only destroy the tree and does not issue any TC commands
 *  to clean up the qdiscs from the device. To perform the cleanup please
 *  use first \ref pon_net_qdiscs_diff_apply()
 *
 *  \param[in]  qdiscs	Collection of qdiscs
 */
void pon_net_qdiscs_destroy(struct pon_net_qdiscs *qdiscs);

/** Assigns qdiscs to classes that are specified by qdisc->parent_queue
 *
 *  This function will update struct pon_net_qdisc->classes array.
 *  If you create a tree and set struct pon_net_qdisc->parent_queue and
 *  struct pon_net_qdisc->parent_arg, this function will copy this information
 *  into parent's classes array.
 *
 *  \param[in]  qdiscs	Collection of qdiscs
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_qdiscs_assign_to_classes(struct pon_net_qdiscs *qdiscs);

/** Changes parents of drr qdiscs to prio qdisc
 *
 *  \param[in]  qdiscs	Collection of qdiscs
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_qdiscs_change_drr_parents_to_prio(struct pon_net_qdiscs *qdiscs);

/** Returns number of qdiscs in the tree
 *
 *  \param[in]  qdiscs	Collection of qdiscs
 *
 *  \return returns number of qdiscs in the tree
 */
unsigned int pon_net_qdiscs_len(const struct pon_net_qdiscs *qdiscs);

/** Retrieves a qdisc from the tree by its handle or creates a new one
 *
 *  \param[in]  qdiscs			Collection of qdiscs
 *  \param[in]  handle			Qdisc handle
 *  \param[in]  type_of_created		Type of created qdisc
 *  \param[in]  data_of_created		Private data of created qdisc
 *  \param[in]  parent_of_created	Parent of created qdisc
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
struct pon_net_qdisc *
pon_net_qdiscs_get_or_add(struct pon_net_qdiscs *qdiscs, uint16_t handle,
			  const struct pon_net_qdisc_type *type_of_created,
			  const void *data_of_created,
			  struct pon_net_qdisc *parent_of_created);

/** Partitions the qdisc with number of classes above the given thresholds, into
 *  multiple child qdiscs with classes below the given threshold
 *
 *  \param[in]  qdiscs			Collection of qdiscs
 *  \param[in]  i			Qdisc index
 *  \param[in]  threshold		threshold above which the partition
 *					will occur
 *  \param[in]  handle_gen		Function that will generate handles
 *					for newly created qdiscs
 *  \param[in]  ctx			Context passed to handle generating
 *					function
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_qdiscs_partition(struct pon_net_qdiscs *qdiscs,
			 unsigned int i,
			 unsigned int threshold,
			 uint16_t (*handle_gen)(unsigned int child_number,
						void *ctx),
			 void *ctx);

/** Deficit Round Robin qdisc type */
extern const struct pon_net_qdisc_type pon_net_drr_type;
/** prio qdisc type */
extern const struct pon_net_qdisc_type pon_net_prio_type;

/** Private data of RED qdisc */
struct pon_net_red_type_data {
	/* Minimum threshold value for RED to start dropping packets */
	uint32_t min;
	/* Maximum threshold value for RED */
	uint32_t max;
	/* Probability of dropping packet when queue reaches the maximum
	 * threshold */
	double probability;
};

/** RED qdisc type */
extern const struct pon_net_qdisc_type pon_net_red_type;

/** Private data of TBF qdisc */
struct pon_net_tbf_type_data {
	/* Committed information rate [bytes/second] */
	int cir;
	/* Peak information rate [bytes/second] */
	int pir;
	/* Committed burst size [bytes] */
	int cbs;
	/* Peak burst size [bytes] */
	int pbs;
};

/** TBF (token bucket filter) qdisc type */
extern const struct pon_net_qdisc_type pon_net_tbf_type;

/** @} */ /* PON_NET_LIB */

#endif
