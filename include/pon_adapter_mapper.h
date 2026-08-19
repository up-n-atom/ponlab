/******************************************************************************
 *
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_mapper.h
 *
 * This is the PON adapter header file for internal OMCI resource mapping.
 */

#ifndef _palib_mapper_h
#define _palib_mapper_h

#include "pon_adapter_base.h"
#include "pon_adapter_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *  @{
 */

/** \defgroup PON_ADAPTER_MAPPER PON Adapter Low-level Index Mapper
 *
 * These functions are used to handle the mapping between management-controlled
 * items.
 * @{
 */

 /** Index mapper */
struct mapper;

/** Key type definition */
enum mapper_key_type {
	/** Key type */
	KEY_TYPE_ID,
	/** Index type */
	KEY_TYPE_INDEX,
	/** Number of key types */
	KEY_TYPE_NUM
};

/** Index mapper data entry */
struct mapper_entry {
	/** Item type */
	int32_t type;
	/** ID minimum value */
	uint32_t min_key[KEY_TYPE_NUM];
	/** ID maximum value */
	uint32_t max_key[KEY_TYPE_NUM];
	/** Item type name */
	const char *name;
};

/** Allocate a mapper element
 *
 * \param[in] m_entry	m_entry array pointer
 *
 * \return
 *    New allocated mapper or NULL on error.
 */
struct mapper *mapper_alloc(const struct mapper_entry *m_entry);

/** Cleanup stored mappers
 *
 * \param[in]  m       Pointer to mapper
 */
enum pon_adapter_errno mapper_cleanup(struct mapper *m);

/** Release all memory for a mapper
 *
 * \param[in]  m       Pointer to mapper
 */
enum pon_adapter_errno mapper_free(struct mapper *m);

/** Explicitly map ID to index
 *
 * \param[in] m       Pointer to mapper
 * \param[in] id      ID to map
 * \param[in] idx     Index to map
 */
enum pon_adapter_errno mapper_explicit_map(struct mapper *m,
					   uint32_t id,
					   uint32_t idx);

/** Map ID to index
 *
 * \param[in]  m       Pointer to mapper
 * \param[in]  id      ID to map
 * \param[out] idx     Return mapped index
 */
enum pon_adapter_errno mapper_id_map(struct mapper *m,
				     uint32_t id,
				     uint32_t *idx);

/** Map index to ID
 *
 * \param[in]  m       Pointer to mapper
 * \param[out] id      Return mapped ID
 * \param[in]  idx     Index to map
 */
enum pon_adapter_errno mapper_index_map(struct mapper *m,
					uint32_t *id,
					uint32_t idx);

/** Retrieve ID for the given index
 *
 * \param[in]  m       Pointer to mapper
 * \param[out] id      Return mapped ID
 * \param[in]  idx     Index
 */
enum pon_adapter_errno mapper_id_get(struct mapper *m,
				     uint32_t *id,
				     uint32_t idx);

/** Retrieve index for the given ID
 *
 * \param[in]  m       Pointer to mapper
 * \param[in]  id      ID
 * \param[out] idx     Return mapped index
 */
enum pon_adapter_errno mapper_index_get(struct mapper *m,
					uint32_t id,
					uint32_t *idx);

/** Retrieve index for the given ID, map if not mapped yet
 *
 * \param[in]  m       Pointer to mapper
 * \param[in]  id      ID
 * \param[out] idx     Return mapped index
 */
enum pon_adapter_errno mapper_index_map_or_get(struct mapper *m,
					       uint32_t id,
					       uint32_t *idx);

/** Retrieve the array of mapped ID
 *
 * \param[in]  m       Pointer to mapper
 * \param[out] id      Pointer to the mapped ID array
 * \param[out] size    Size of the mapped ID array
 *
 * \remark It is the user's responsibility to free up the memory which has been
 * allocated for the array.
 */
enum pon_adapter_errno mapper_id_array_get(struct mapper *m,
					   uint32_t **id,
					   uint32_t *size);

/** Retrieve the array of mapped Indexes
 *
 * \param[in]  m       Pointer to mapper
 * \param[out] idx     Pointer to the mapped Index array
 * \param[out] size    Size of the mapped Index array
 *
 * \remark It is the user's responsibility to free up the memory which has been
 * allocated for the array.
 */
enum pon_adapter_errno mapper_index_array_get(struct mapper *m,
					      uint32_t **idx,
					      uint32_t *size);

/** Remove id from the mapping
 *
 * \param[in] m       Pointer to mapper
 * \param[in] id      Remove index with given ID
 */
enum pon_adapter_errno mapper_id_remove(struct mapper *m,
					uint32_t id);

/** Remove index from the mapping
 *
 * \param[in] m       Pointer to mapper
 * \param[in] idx     Remove ID with given index
 */
enum pon_adapter_errno mapper_index_remove(struct mapper *m,
					   uint32_t idx);

/** Initialize mapper generic function
 *
 * \param[in] m		Mapper pointer
 * \param[in] size	Size of m_entry array
 * \param[in] m_entry	m_entry array pointer
 */
enum pon_adapter_errno pa_mapper_init(struct mapper **m,
				      size_t size,
				      const struct mapper_entry *m_entry);

/** Shutdown mapper generic function
 *
 * \param[in] m		Mapper pointer
 * \param[in] size	Size of m_entry array
 */
enum pon_adapter_errno pa_mapper_shutdown(struct mapper **m,
					  size_t size);

/** Reset mapper generic function
 *
 * \param[in] m		Mapper pointer
 * \param[in] size	Size of m_entry array
 */
enum pon_adapter_errno pa_mapper_reset(struct mapper **m,
				       size_t size);

/** Dump mapper generic function
 *
 * \param[in] m		Mapper pointer
 * \param[in] size	Size of m_entry array
 * \param[in] m_entry	m_entry array pointer
 */

enum pon_adapter_errno pa_mapper_dump(struct mapper **m,
				size_t size,
				const struct mapper_entry *m_entry);

/** @} */ /* PON_ADAPTER_MAPPER */
/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
