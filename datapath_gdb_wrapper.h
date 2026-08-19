// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 * Copyright (c) 2023, MaxLinear, Inc.
 *
 ******************************************************************************/

#ifndef DATAPATH_GDB_WRAPPER_H
#define DATAPATH_GDB_WRAPPER_H

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DISABLE_OPTIMIZE)
/* once __attribute__((optimize("O0"))) is applied for friendly kgdb debugging,
 * some workaround is required to make build and execution work
 */
/* wrapper API for clear_bit */
void dp_clear_bit(unsigned long nr, volatile unsigned long *addr);
/* wrapper API for memcpy */
int dp_memcmp(const void *,const void *,__kernel_size_t);
/* wrapper API for memset */
void *dp_memset(void *p, int c, __kernel_size_t size);
/* wrapper API for memcpy */
void *dp_memcpy(void *dst, const void *src, __kernel_size_t size);
/* wrapper API for strncpy */
char *dp_strncpy(char *dest, const char *src, size_t count);
/* wrapper API for strscpy */
ssize_t dp_strscpy(char *dest, const char *src, size_t count);
/* wrapper API for strlcpy */
size_t dp_strlcpy(char *dst, const char *src, size_t size);
/* wrapper API for copy_from_user */
unsigned long dp_copy_from_user(void * to, const void __user * from,
					unsigned long n);
/* wrapper API for kmalloc */
void *dp_kmalloc(size_t size, gfp_t flags);
/* wrapper API for kzalloc */
void *dp_kzalloc(size_t size, gfp_t flags);
/* wrapper API for kmalloc_array */
void *dp_kmalloc_array(size_t n, size_t size, gfp_t flags);
/* wrapper API for bitmap_set */
void dp_bitmap_set(unsigned long *map, unsigned int start, unsigned int nbits);
/* wrapper API for bitmap_clear */
void dp_bitmap_clear(unsigned long *map, unsigned int start,
					unsigned int nbits);
#else
#define dp_clear_bit clear_bit
#define dp_memcmp memcmp
#define dp_memset memset
#define dp_memcpy memcpy
#define dp_strncpy strncpy
#define dp_strlcpy strlcpy
#define dp_copy_from_user copy_from_user
#define dp_kmalloc kmalloc
#define dp_kzalloc kzalloc
#define dp_kmalloc_array kmalloc_array
#define dp_strscpy strscpy
#define dp_bitmap_set bitmap_set
#define dp_bitmap_clear bitmap_clear
#endif

#endif
