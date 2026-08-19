/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __OBJ_POOL__H__
#define __OBJ_POOL__H__

#include <stddef.h>
#include <stdint.h>

typedef struct _obj_pool obj_pool;

/* Define OBJPOOL_DEBUG for extended debug printouts */
/* #define OBJPOOL_DEBUG */

#define OBJ void*

typedef void (*object_cb)(obj_pool* pool, const OBJ object);

size_t obj_pool_destroy(obj_pool *pool);

obj_pool* obj_pool_init(const char *pool_name, size_t object_byte_size,
			size_t min_size, size_t max_size,
			int thread_safety_needed);

OBJ obj_pool_alloc_object(obj_pool *pool);

void obj_pool_put_object(obj_pool *pool, OBJ object);

/* Helper functions */

/* Get the object pool name */
const char* obj_pool_get_name(obj_pool *pool);

/* Set a callback function. If obj_pool_destroy() detects unreleased objects,
   it invokes a callback function for each unreleased function. The callback function can dump unreleased objects
   for debugging purposes */
void obj_pool_set_callback(obj_pool *pool, object_cb cb);

/* This function walks through all allocated objects and calls the 'cb' callback function for each object.
   The function returns the number of allocated objects */
size_t obj_pool_walk(obj_pool *pool, object_cb cb);

#endif /* __OBJ_POOL__H__ */
