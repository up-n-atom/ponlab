/******************************************************************************
 *
 *  Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pon_adapter_mapper.h"
#include "pon_adapter_debug.h"

/** ID to Index (and Index to ID) mapping list definition
 *
 * \note List is sorted by ID and Index growth
 */
struct mapper_map_list {
	/** Key array (ID and Index) */
	uint32_t key[KEY_TYPE_NUM];
	/** Array of pointers to the next list node
	 * with bigger key value (ID or Index)
	 */
	struct mapper_map_list *next[KEY_TYPE_NUM];
};

/** Mapper definition */
struct mapper {
	/** m_entry pointer */
	const struct mapper_entry *m_entry;
	/** List heads array*/
	struct mapper_map_list *list_head[KEY_TYPE_NUM];
};

/** Verify key
 *
 * \note Verify key using range (minimal and maximal values)
 *    of key defined in appropriate Mapper
 *
 * \param[in] m        Pointer to mapper
 * \param[in] key_type Key Type
 * \param[in] key      Key
 *
 * \return
 *    - PON_ADAPTER_SUCCESS Key is valid
 *    - PON_ADAPTER_ERROR   Key is invalid (out of range)
 */
static enum pon_adapter_errno key_verify(const struct mapper *m,
					 enum mapper_key_type key_type,
					 uint32_t key)
{
	if (!m)
		return PON_ADAPTER_ERROR;

	if ((key >= m->m_entry->min_key[key_type]) &&
		(key <= m->m_entry->max_key[key_type]))
		return PON_ADAPTER_SUCCESS;

	PA_DBG_PRINT("Key %d is out of range [%d .. %d] (key type %d) for entry {%s}\n",
			key,
			m->m_entry->min_key[key_type],
			m->m_entry->max_key[key_type],
			key_type,
			m->m_entry->name);

	return PON_ADAPTER_ERROR;
}

/** Create Mapping List Item
 *
 * \param[in]  key  Array of keys
 * \param[out] item Pointer to pointer to created item
 *
 * \return
 *    - PON_ADAPTER_SUCCESS Item was created successfully
 *    - PON_ADAPTER_ERROR   Item creation failed
 */
static enum pon_adapter_errno item_create(uint32_t key[KEY_TYPE_NUM],
					  struct mapper_map_list **item)
{
	enum mapper_key_type key_type;

	*item = malloc(sizeof(struct mapper_map_list));
	if (!(*item)) {
		PA_DBG_PRINT("%s", "Not enough memory for new mapper item!\n");
		return PON_ADAPTER_ERROR;
	}

	for (key_type = KEY_TYPE_ID; key_type < KEY_TYPE_NUM; key_type++) {
		(*item)->key[key_type] = key[key_type];
		(*item)->next[key_type] = NULL;
	}

	return PON_ADAPTER_SUCCESS;
}

/** Delete Mapping List Item
 *
 * \param[in] item Pointer to Item to delete
 */
static void item_delete(struct mapper_map_list *item)
{
	free(item);
}

/** Add Item to Mapping list
 *
 * \param[in] m       Pointer to mapper
 * \param[in] pos     Array of position where to add Item
 * \param[in] item    Pointer to Item to add
 *
 * \return
 *    - PON_ADAPTER_SUCCESS Item was added successfully
 *    - PON_ADAPTER_ERROR   Item addition failed
 */
static enum pon_adapter_errno item_add(struct mapper *m,
				struct mapper_map_list *pos[KEY_TYPE_NUM],
				struct mapper_map_list *item)
{
	enum mapper_key_type key_type;

	if (!m)
		return PON_ADAPTER_ERROR;

	for (key_type = KEY_TYPE_ID; key_type < KEY_TYPE_NUM; key_type++) {
		if (pos[key_type] == NULL) {
			item->next[key_type] = m->list_head[key_type];
			m->list_head[key_type] = item;
		} else {
			item->next[key_type] = pos[key_type]->next[key_type];
			pos[key_type]->next[key_type] = item;
		}
	}

	return PON_ADAPTER_SUCCESS;
}

/** Remove Item from Mapping List
 *
 * \note Just remove Item from the Mapping list, but NOT delete Item
 *       (for deletion use \ref item_delete)
 *
 * \param[in] m       Pointer to mapper
 * \param[in] pos     Array of Item positions
 *
 * \return
 *    - Pointer to the removed Item
 *    - NULL pointer if Item removing failed
 */
static struct mapper_map_list *item_remove(struct mapper *m,
					   struct mapper_map_list
						*pos[KEY_TYPE_NUM])
{
	struct mapper_map_list *removed_item = NULL;
	enum mapper_key_type key_type;

	if (!m)
		return removed_item;

	for (key_type = KEY_TYPE_ID; key_type < KEY_TYPE_NUM; key_type++) {
		if (!(pos[key_type])) {
			removed_item = m->list_head[key_type];
			m->list_head[key_type] =
				m->list_head[key_type]->next[key_type];
		} else {
			removed_item = pos[key_type]->next[key_type];
			pos[key_type]->next[key_type] =
				pos[key_type]->next[key_type]->next[key_type];
		}
	}

	if (!removed_item)
		PA_DBG_PRINT("Failed to remove item {%s}\n",
			m->m_entry->name);

	return removed_item;
}

/** Retrieve Item for the given position
 *
 * \param[in] m        Pointer to mapper
 * \param[in] key_type Key Type
 * \param[in] pos      Item Position
 *
 * \return
 *    - Pointer to the Item
 *    - NULL if Item retrieving failed
 */
static struct mapper_map_list *item_get(const struct mapper *m,
					enum mapper_key_type key_type,
					const struct mapper_map_list *pos)
{
	struct mapper_map_list *item = NULL;

	if (!m)
		return item;

	if (!pos)
		item = m->list_head[key_type];
	else
		item = pos->next[key_type];

	return item;
}

/** Find Item in the Mapping List with the given Key number
 *
 * \param[in]  m        Pointer to mapper
 * \param[in]  key_type Key Type
 * \param[in]  key      Key
 * \param[out] pos      Pointer to array of Item positions
 *
 * \return
 *    - PON_ADAPTER_SUCCESS   Item was found successfully
 *    - PON_ADAPTER_ERR_NOT_FOUND Item was not found
 *    - PON_ADAPTER_ERROR     Item searching failed
 */
static enum pon_adapter_errno item_find(const struct mapper *m,
				      enum mapper_key_type key_type,
				      uint32_t key,
				      struct mapper_map_list **pos)
{
	struct mapper_map_list *item = NULL;

	if (!m)
		return PON_ADAPTER_ERROR;

	*pos = NULL;

	item = m->list_head[key_type];
	if ((!item))
		return PON_ADAPTER_ERR_NOT_FOUND;

	if (key < item->key[key_type])
		return PON_ADAPTER_ERR_NOT_FOUND;

	if (item->key[key_type] == key)
		return PON_ADAPTER_SUCCESS;

	while (item->next[key_type]) {
		if (item->next[key_type]->key[key_type] == key) {
			*pos = item;
			return PON_ADAPTER_SUCCESS;
		}
		if (key < item->next[key_type]->key[key_type]) {
			*pos = item;
			return PON_ADAPTER_ERR_NOT_FOUND;
		}
		item = item->next[key_type];
	}

	*pos = item;

	return PON_ADAPTER_ERR_NOT_FOUND;
}

/** Retrieve key from the given Item with respect to the key Type
 *
 * \param[in]  item     Item to retrieve key from
 * \param[in]  key_type Key Type
 * \param[out] key      Retrieved item
 */
static void key_get(struct mapper_map_list *item,
		    enum mapper_key_type key_type,
		    uint32_t *key)
{
	*key = item->key[key_type];
}

/** Find lowest NOT mapped key with respect to the key Type
 *
 * \param[in]  m        Pointer to mapper
 * \param[in]  key_type Key Type
 * \param[out] free_key Free key value
 * \param[out] pos      Free position of Item with found Key
 *
 * \return
 *    - PON_ADAPTER_SUCCESS   Free key was found successfully
 *    - PON_ADAPTER_ERR_NOT_FOUND Free key was not found
 *    - PON_ADAPTER_ERROR     Free key searching failed
 */
static enum pon_adapter_errno free_key_find(const struct mapper *m,
					  enum mapper_key_type key_type,
					  uint32_t *free_key,
					  struct mapper_map_list **pos)
{
	struct mapper_map_list *item = NULL;

	if (!m)
		return PON_ADAPTER_ERROR;

	*pos = NULL;

	item = m->list_head[key_type];
	if ((!item) ||
		((item->key[key_type] - m->m_entry->min_key[key_type]) > 0)) {
		*free_key = m->m_entry->min_key[key_type];
		return PON_ADAPTER_SUCCESS;
	}

	while (item->next[key_type]) {
		if ((item->next[key_type]->key[key_type] -
		     item->key[key_type]) > 1) {
			*pos = item;
			*free_key = item->key[key_type] + 1;
			return PON_ADAPTER_SUCCESS;
		}
		item = item->next[key_type];
	}

	if ((item->key[key_type]) == (m->m_entry->max_key[key_type])) {
		PA_DBG_PRINT("No free key for type %d, name %s key %x max_key %x\n",
			key_type, m->m_entry->name, item->key[key_type],
			m->m_entry->max_key[key_type]);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	*pos = item;
	*free_key = item->key[key_type] + 1;

	return PON_ADAPTER_SUCCESS;
}

struct mapper *mapper_alloc(const struct mapper_entry *m_entry)
{
	struct mapper *m;

	PA_DBG_PRINT("Setup map item={%s} id=[0x%08x,0x%08x] idx=[0x%08x,0x%08x]\n",
			m_entry->name,
			m_entry->min_key[KEY_TYPE_ID],
			m_entry->max_key[KEY_TYPE_ID],
			m_entry->min_key[KEY_TYPE_INDEX],
			m_entry->max_key[KEY_TYPE_INDEX]);

	m = malloc(sizeof(struct mapper));
	if (!m)
		return NULL;

	m->m_entry = m_entry;

	m->list_head[KEY_TYPE_ID] = NULL;

	m->list_head[KEY_TYPE_INDEX] = NULL;

	return m;
}

enum pon_adapter_errno mapper_cleanup(struct mapper *m)
{
	enum pon_adapter_errno ret;
	struct mapper_map_list *item_pos[KEY_TYPE_NUM] = { NULL };
	struct mapper_map_list *item = NULL;
	uint32_t key;

	if (m) {
		item = item_get(m, KEY_TYPE_ID, item_pos[KEY_TYPE_ID]);
		while (item) {
			key_get(item, KEY_TYPE_INDEX, &key);

			ret = item_find(m, KEY_TYPE_INDEX, key,
					&item_pos[KEY_TYPE_INDEX]);
			if (ret != PON_ADAPTER_SUCCESS) {
				PA_DBG_PRINT
				    ("Index %d searching error %d\n",
				     key, ret);
				return PON_ADAPTER_ERROR;
			}

			item = item_remove(m, item_pos);
			if (!item)
				return PON_ADAPTER_ERROR;

			item_delete(item);

			item = item_get(m, KEY_TYPE_ID,
					item_pos[KEY_TYPE_ID]);
		}

		return PON_ADAPTER_SUCCESS;
	}

	return PON_ADAPTER_ERROR;
}

enum pon_adapter_errno mapper_free(struct mapper *m)
{
	enum pon_adapter_errno ret;

	ret = mapper_cleanup(m);
	if (ret != PON_ADAPTER_SUCCESS)
		return ret;

	free(m);

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_explicit_map(struct mapper *m,
					   uint32_t id,
					   uint32_t idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t key[KEY_TYPE_NUM] = { 0 };
	struct mapper_map_list *item_pos[KEY_TYPE_NUM] = { NULL };
	struct mapper_map_list *item = NULL;
	enum mapper_key_type key_type;

	PA_DBG_PRINT("%s(%u, %u)\n", __func__, id, idx);

	key[KEY_TYPE_ID] = id;
	key[KEY_TYPE_INDEX] = idx;

	for (key_type = KEY_TYPE_ID; key_type < KEY_TYPE_NUM; key_type++) {
		ret = key_verify(m, key_type, key[key_type]);
		if (ret != PON_ADAPTER_SUCCESS)
			return PON_ADAPTER_ERROR;
	}

	for (key_type = KEY_TYPE_ID; key_type < KEY_TYPE_NUM; key_type++) {
		ret = item_find(m, key_type, key[key_type],
				&item_pos[key_type]);
		if (ret == PON_ADAPTER_SUCCESS) {
			PA_DBG_PRINT
			    ("Key %d with type %d is already mapped for item {%s}\n",
			     key[key_type], key_type, m->m_entry->name);
			return PON_ADAPTER_ERROR;
		}
	}

	ret = item_create(key, &item);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_add(m, item_pos, item);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_id_map(struct mapper *m,
				     uint32_t id,
				     uint32_t *idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct mapper_map_list *item_pos[KEY_TYPE_NUM] = { NULL };
	struct mapper_map_list *item = NULL;
	uint32_t key[KEY_TYPE_NUM] = { 0 };

	PA_DBG_PRINT("%s(%u, %p) item {%s}\n", __func__, id, idx,
		     m->m_entry->name);

	ret = key_verify(m, KEY_TYPE_ID, id);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_find(m, KEY_TYPE_ID, id, &item_pos[KEY_TYPE_ID]);
	if (ret == PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Id %d is already mapped for item {%s}\n",
			     id, m->m_entry->name);
		return PON_ADAPTER_ERROR;
	}

	ret = free_key_find(m, KEY_TYPE_INDEX, idx, &item_pos[KEY_TYPE_INDEX]);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	key[KEY_TYPE_ID] = id;
	key[KEY_TYPE_INDEX] = *idx;

	ret = item_create(key, &item);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_add(m, item_pos, item);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_index_map(struct mapper *m,
					uint32_t *id,
					uint32_t idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct mapper_map_list *item_pos[KEY_TYPE_NUM] = { NULL };
	struct mapper_map_list *item = NULL;
	uint32_t key[KEY_TYPE_NUM] = { 0 };

	PA_DBG_PRINT("%s(%p, %u) item {%s}\n", __func__, id, idx,
		     m->m_entry->name);

	ret = key_verify(m, KEY_TYPE_INDEX, idx);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_find(m, KEY_TYPE_INDEX, idx, &item_pos[KEY_TYPE_INDEX]);
	if (ret == PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Index %d is already mapped for item {%s}\n",
			idx, m->m_entry->name);
		return PON_ADAPTER_ERROR;
	}

	ret = free_key_find(m, KEY_TYPE_ID, id, &item_pos[KEY_TYPE_ID]);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	key[KEY_TYPE_ID] = *id;
	key[KEY_TYPE_INDEX] = idx;

	ret = item_create(key, &item);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_add(m, item_pos, item);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_index_get(struct mapper *m,
					uint32_t id,
					uint32_t *idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct mapper_map_list *item_pos = NULL;
	struct mapper_map_list *item = NULL;

	PA_DBG_PRINT("%s(%u, %p) item {%s}\n", __func__, id, idx,
		     m->m_entry->name);

	ret = key_verify(m, KEY_TYPE_ID, id);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_find(m, KEY_TYPE_ID, id, &item_pos);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		PA_DBG_PRINT("Id %d is not found (index_get), item {%s}\n",
			id, m->m_entry->name);
		return PON_ADAPTER_ERR_NOT_FOUND;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Id %d searching error %d (index_get), item {%s}\n",
				   id, ret, m->m_entry->name);
		return PON_ADAPTER_ERROR;
	}

	item = item_get(m, KEY_TYPE_ID, item_pos);
	if (!item) {
		PA_DBG_PRINT("%s", "Id retrieving failed (index_get)\n");
		return PON_ADAPTER_ERROR;
	}

	key_get(item, KEY_TYPE_INDEX, idx);

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_id_get(struct mapper *m,
				     uint32_t *id,
				     uint32_t idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct mapper_map_list *item_pos = NULL;
	struct mapper_map_list *item = NULL;

	PA_DBG_PRINT("%s(%p, %u)\n", __func__, id, idx);

	ret = key_verify(m, KEY_TYPE_INDEX, idx);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_find(m, KEY_TYPE_INDEX, idx, &item_pos);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		PA_DBG_PRINT("Index %d is not found (id_get)\n", idx);
		return PON_ADAPTER_ERR_NOT_FOUND;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Index %d searching error %d (id_get)\n",
				   idx, ret);
		return PON_ADAPTER_ERROR;
	}

	item = item_get(m, KEY_TYPE_INDEX, item_pos);
	if (!item) {
		PA_DBG_PRINT("%s", "Index retrieving failed (id_get)\n");
		return PON_ADAPTER_ERROR;
	}

	key_get(item, KEY_TYPE_ID, id);

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_index_map_or_get(struct mapper *m,
					       const uint32_t id,
					       uint32_t *idx)
{
	enum pon_adapter_errno ret = mapper_index_get(m, id, idx);

	if (ret == PON_ADAPTER_ERR_NOT_FOUND)
		ret = mapper_id_map(m, id, idx);

	return ret;
}

enum pon_adapter_errno mapper_index_array_get(struct mapper *m,
					      uint32_t **idx,
					      uint32_t *size)
{
	struct mapper_map_list *item = NULL;
	uint32_t sz = 0;
	uint32_t i = 0;

	PA_DBG_PRINT("%s(%p, %p)\n", __func__, idx, size);

	item = m->list_head[KEY_TYPE_INDEX];
	if (!item) {
		*idx = NULL;
		*size = 0;
		return PON_ADAPTER_SUCCESS;
	}

	while (item) {
		sz++;
		item = item->next[KEY_TYPE_INDEX];
	}

	*idx = malloc(sz * sizeof(uint32_t));
	if (!(*idx)) {
		PA_DBG_PRINT("%s", "Not enough memory for index array\n");
		return PON_ADAPTER_ERROR;
	}

	item = m->list_head[KEY_TYPE_INDEX];
	while (item) {
		(*idx)[i++] = item->key[KEY_TYPE_INDEX];
		item = item->next[KEY_TYPE_INDEX];
	}
	*size = sz;

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_id_array_get(struct mapper *m,
					   uint32_t **id,
					   uint32_t *size)
{
	struct mapper_map_list *item = NULL;
	uint32_t sz = 0;
	uint32_t i = 0;

	PA_DBG_PRINT("%s(%p, %p)\n", __func__, id, size);

	item = m->list_head[KEY_TYPE_ID];
	if (!item) {
		*id = NULL;
		*size = 0;
		return PON_ADAPTER_SUCCESS;
	}

	while (item) {
		sz++;
		item = item->next[KEY_TYPE_ID];
	}

	*id = malloc(sz * sizeof(uint32_t));
	if (!(*id)) {
		PA_DBG_PRINT("%s", "Not enough memory for id array\n");
		return PON_ADAPTER_ERROR;
	}

	item = m->list_head[KEY_TYPE_ID];
	while (item) {
		(*id)[i++] = item->key[KEY_TYPE_ID];
		item = item->next[KEY_TYPE_ID];
	}
	*size = sz;

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_id_remove(struct mapper *m,
					uint32_t id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct mapper_map_list *item_pos[KEY_TYPE_NUM] = { NULL };
	struct mapper_map_list *item = NULL;
	uint32_t idx = 0;

	PA_DBG_PRINT("%s(%u)\n", __func__, id);

	ret = key_verify(m, KEY_TYPE_ID, id);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_find(m, KEY_TYPE_ID, id, &item_pos[KEY_TYPE_ID]);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		PA_DBG_PRINT("Id %d is not found (id_remove)\n", id);
		return PON_ADAPTER_ERR_NOT_FOUND;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Id %d searching error %d (id_remove)\n",
				   id, ret);
		return PON_ADAPTER_ERROR;
	}

	item = item_get(m, KEY_TYPE_ID, item_pos[KEY_TYPE_ID]);
	if (!item) {
		PA_DBG_PRINT("%s", "Id retrieving failed (id_remove)\n");
		return PON_ADAPTER_ERROR;
	}

	key_get(item, KEY_TYPE_INDEX, &idx);

	ret = item_find(m, KEY_TYPE_INDEX, idx, &item_pos[KEY_TYPE_INDEX]);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		PA_DBG_PRINT("Index %d is not found (id_remove)\n", idx);
		return PON_ADAPTER_ERR_NOT_FOUND;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Index %d searching error %d (id_remove)\n",
				   idx, ret);
		return PON_ADAPTER_ERROR;
	}

	item = item_remove(m, item_pos);
	if (!item)
		return PON_ADAPTER_ERROR;

	item_delete(item);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno mapper_index_remove(struct mapper *m,
					   uint32_t idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct mapper_map_list *item_pos[KEY_TYPE_NUM] = { NULL };
	struct mapper_map_list *item = NULL;
	uint32_t id = 0;

	PA_DBG_PRINT("%s(%u)\n", __func__, idx);

	ret = key_verify(m, KEY_TYPE_INDEX, idx);
	if (ret != PON_ADAPTER_SUCCESS)
		return PON_ADAPTER_ERROR;

	ret = item_find(m, KEY_TYPE_INDEX, idx, &item_pos[KEY_TYPE_INDEX]);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		PA_DBG_PRINT("Index %d is not found (index_remove)\n",
				   idx);
		return PON_ADAPTER_ERR_NOT_FOUND;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT
		    ("Index %d searching error %d (index_remove)\n", idx, ret);
		return PON_ADAPTER_ERROR;
	}

	item = item_get(m, KEY_TYPE_INDEX, item_pos[KEY_TYPE_INDEX]);
	if (!item) {
		PA_DBG_PRINT("%s", "Index retrieving failed (index_remove)\n");
		return PON_ADAPTER_ERROR;
	}

	key_get(item, KEY_TYPE_ID, &id);

	ret = item_find(m, KEY_TYPE_ID, id, &item_pos[KEY_TYPE_ID]);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		PA_DBG_PRINT("Id %d is not found (index_remove)\n", id);
		return PON_ADAPTER_ERR_NOT_FOUND;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		PA_DBG_PRINT("Id %d searching error %d (index_remove)\n",
				   id, ret);
		return PON_ADAPTER_ERROR;
	}

	item = item_remove(m, item_pos);
	if (!item)
		return PON_ADAPTER_ERROR;

	item_delete(item);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pa_mapper_init(struct mapper **m,
				      size_t size,
				      const struct mapper_entry *m_entry)
{
	size_t i;
	uint32_t id_type;

	PA_DBG_PRINT("%s\n", __func__);

	for (i = 0; i < size; i++) {
		id_type = m_entry[i].type;

		m[id_type] = mapper_alloc(&m_entry[i]);

		if (!m[id_type]) {
			PA_DBG_PRINT("Can't initialize mapping for %s\n",
			    m_entry[i].name);

			return PON_ADAPTER_ERROR;
		}
	}

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pa_mapper_reset(struct mapper **m,
				       size_t size)
{
	uint32_t id_type;

	PA_DBG_PRINT("%s\n", __func__);

	for (id_type = 0; id_type < size; id_type++) {
		if (m[id_type])
			mapper_cleanup(m[id_type]);
	}

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pa_mapper_shutdown(struct mapper **m,
					  size_t size)
{
	uint32_t id_type;
	enum pon_adapter_errno mapper_ret;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	for (id_type = 0; id_type < size; id_type++) {
		if (m[id_type]) {
			mapper_ret = mapper_free(m[id_type]);
			if (mapper_ret != PON_ADAPTER_SUCCESS) {
				PA_DBG_PRINT("Can't free mapping for %s\n",
					     m[id_type]->m_entry->name);
				ret = mapper_ret;
			}
		}
	}

	return ret;
}

enum pon_adapter_errno pa_mapper_dump(struct mapper **m,
				      size_t size,
				      const struct mapper_entry *m_entry)
{
	enum pon_adapter_errno ret;
	int32_t type;
	uint32_t *id_list = NULL, id_size, idx;
	uint32_t i, j;

	PA_DBG_PRINT("%s\n", __func__);

	printf("+----------------------------------------------------+------------+------------\n");
	printf("| Type                                               | Id         | Index\n");
	printf("+----------------------------------------------------+------------+------------\n");

	for (i = 0; i < size; i++) {
		type = m_entry[i].type;

		if (id_list)
			free(id_list);
		id_list = NULL;

		if (m[type] == NULL)
			continue;

		if (mapper_id_array_get(m[type], &id_list, &id_size) !=
			PON_ADAPTER_SUCCESS)
			continue;
		if (!id_size)
			continue;

		for (j = 0; j < id_size; j++) {
			ret = mapper_index_get(m[type], *(id_list + j), &idx);
			if (ret != PON_ADAPTER_SUCCESS)
				continue; /* ignore and try next index */

			printf("| 0x%02x %-45s | 0x%08x | 0x%08x\n",
				type, m_entry[i].name, *(id_list + j), idx);
		}
		printf("+----------------------------------------------------+------------+------------\n");
	}

	if (id_list)
		free(id_list);

	return PON_ADAPTER_SUCCESS;
}
