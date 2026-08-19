// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 * Copyright (c) 2024, MaxLinear, Inc.
 *
 ******************************************************************************/

#include "datapath.h"
#include "datapath_wa.h"
#include "datapath_tx.h"
#include "datapath_rx.h"

/* When we try to debug dpm with gdb, quite a few code and variable are
 * optimized out which cause difficult to debug.
 * So we try to disable gcc optimization, but it cause some compilation issue
 * or some code not work as expected with different GCC version.
 * For example:
 *  1) build failure for memset related API with __write_overflow
 *  2) build failure for clear_bit related API.
 *  3) build failure for rcu_access_pointer/rcu_assign_pointer or even kfree_rcu
 *     related API.
 *  4) build failure for pr_debug
 *  5) WARN_ON

 *  Note:
 *    1)for kmalloc related API, it seems return ZERO_SIZE_PTR some times for
 *     __builtin_constant_p(size) mis-behaves
 */

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DISABLE_OPTIMIZE)
const bool dp_dsable_optimize = true;

int dp_memcmp(const void *dst, const void *src, __kernel_size_t size)
{
	return memcmp(dst, src, size);
}
EXPORT_SYMBOL(dp_memcmp);

void *dp_memset(void *p, int c, __kernel_size_t size)
{
	return memset(p, c, size);
}
EXPORT_SYMBOL(dp_memset);

void *dp_memcpy(void *dst, const void *src, __kernel_size_t size)
{
	return memcpy(dst, src, size);
}
EXPORT_SYMBOL(dp_memcpy);

char *dp_strncpy(char *dest, const char *src, size_t count)
{
	return strncpy(dest, src, count);
}
EXPORT_SYMBOL(dp_strncpy);

size_t dp_strlcpy(char *dst, const char *src, size_t size)
{
	return strlcpy(dst, src, size);
}
EXPORT_SYMBOL(dp_strlcpy);

ssize_t dp_strscpy(char *dest, const char *src, size_t count)
{
	return strscpy(dest, src, count);
}
EXPORT_SYMBOL(dp_strscpy);

unsigned long dp_copy_from_user (void * to, const void __user * from,
	      unsigned long n)
{
	return copy_from_user (to, from, n);
}
EXPORT_SYMBOL(dp_copy_from_user);

void *dp_kmalloc(size_t size, gfp_t flags)
{
	return kmalloc(size, flags);
}
EXPORT_SYMBOL(dp_kmalloc);

void *dp_kzalloc(size_t size, gfp_t flags)
{
	return kzalloc(size, flags);
}
EXPORT_SYMBOL(dp_kzalloc);

void *dp_kmalloc_array(size_t n, size_t size, gfp_t flags)
{
	return kmalloc_array(n, size, flags);
}
EXPORT_SYMBOL(dp_kmalloc_array);

void dp_clear_bit(unsigned long nr, volatile unsigned long *addr)
{
	clear_bit(nr, addr);
}
EXPORT_SYMBOL(dp_clear_bit);

/* Speical handling for dpm only and no need export it */
void dp_rcu_tx_update(struct dp_tx_context *ctx, void *new)
{
	struct tx_hook_list *old_list;

	old_list = rcu_access_pointer(ctx->hook_list);
	rcu_assign_pointer(ctx->hook_list, new);

	if (old_list)
		kfree_rcu(old_list, rcu_head);
}
void dp_rcu_rx_update(struct dp_rx_context *ctx, void *new)
{
	struct rx_hook_list *old_list;

	old_list = rcu_access_pointer(ctx->hook_list);
	rcu_assign_pointer(ctx->hook_list, new);

	if (old_list)
		kfree_rcu(old_list, rcu_head);
}

void dp_bitmap_set(unsigned long *map, unsigned int start,
				unsigned int nbits)
{
	bitmap_set(map, start, nbits);
}
EXPORT_SYMBOL(dp_bitmap_set);

void dp_bitmap_clear(unsigned long *map, unsigned int start,
				unsigned int nbits)
{
	bitmap_clear(map, start, nbits);
}
EXPORT_SYMBOL(dp_bitmap_clear);

#else
const bool dp_dsable_optimize = false;
#endif
