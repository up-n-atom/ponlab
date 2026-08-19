// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2017-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/errno.h>
#include <linux/capability.h>
#include <linux/debugfs.h>
#include <linux/netfilter.h>
#include <linux/skb_cookie.h>
#include "skb_cookie_internal.h"

#define COOKIE_API_PATTERN 0xAAAAAAAAUL
#define COOKIE_API_NAME    "cookie_api_test"

static u32  test_num_add;
static u32  test_num_get;
static u32  test_add_hook;
static u32  test_get_hook;
static s32 *test_c_handles;
static s32  test_num_handles;
static u32  test_add_err;
static u32  test_get_err;
static bool test_is_running;

static bool test_api_cookie_allocated;
static struct dentry *dir;

/**
 * @brief cookie destructor function
 * @param cookie skb cookie to free
 */
static void cookie_destructor(skb_cookie cookie)
{
	if (cookie == COOKIE_API_PATTERN)
		test_api_cookie_allocated = false;
}

/**
 * @brief verifyied the skb cookie extension module API's
 */
static int dbg_api_test_run(struct seq_file *s, void *unused)
{
	s32 test_handle = -1;
	s32 tmp_test_handle = -1;
	skb_cookie cookie;
	struct sk_buff *skb1;
	struct sk_buff *skb2;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	test_api_cookie_allocated = true;

	seq_puts(s, "\n");
	seq_puts(s, "start cookie api unit test...\n");
	/* register new handle */
	seq_puts(s, " 1. Register to a new cookie handle\n");
	if (register_skb_cookie(&test_handle, COOKIE_API_NAME)) {
		seq_puts(s, "register_skb_cookie failed\n");
		goto err;
	}

	/* verify can't register with same name */
	seq_puts(s, " 2. Verify can't register with same name\n");
	if (!register_skb_cookie(&tmp_test_handle, COOKIE_API_NAME)) {
		seq_puts(s, "register with same name is possible, failed\n");
		unregister_skb_cookie(tmp_test_handle);
		goto unregister_cookie;
	}

	/* allocate new SKB */
	seq_puts(s, " 3. Allocate new SKB\n");
	skb1 = alloc_skb(64, GFP_KERNEL);

	if (unlikely(!skb1)) {
		seq_puts(s, "SKB1 allocation failed\n");
		goto unregister_cookie;
	}

	/* set cookie */
	seq_puts(s, " 4. Set cookie\n");
	cookie = COOKIE_API_PATTERN;

	/* attach cookie to SKB */
	seq_puts(s, " 5. Attach cookie to SKB\n");
	if (skb_cookie_attach(skb1, test_handle,
			      cookie, cookie_destructor)) {
		seq_puts(s, "Attach cookie to SKB failed\n");
		goto free_skb1;
	}

	/* clone SKB */
	seq_puts(s, " 6. Clone the SKB\n");
	skb2 = skb_clone(skb1, GFP_ATOMIC);
	if (unlikely(!skb2)) {
		seq_puts(s, "SKB2 allocation failed\n");
		goto free_skb1;
	}

	/* read both cookies */
	seq_puts(s, " 7. Validate both SKBs\n");
	if (skb_cookie_get(skb1, test_handle) != COOKIE_API_PATTERN) {
		seq_puts(s, "SKB1 cookie read failed\n");
		goto free_skb2;
	}
	if (skb_cookie_get(skb2, test_handle) != COOKIE_API_PATTERN) {
		seq_puts(s, "SKB2 cookie read failed\n");
		goto free_skb2;
	}

	/* release original cookie */
	seq_puts(s, " 8. Release cookie from the original SKB\n");
	if (skb_cookie_release(skb1, test_handle)) {
		seq_puts(s, "SKB1 cookie release failed\n");
		goto free_skb2;
	}

	/* verify cookie is still there on the cloned SKB */
	seq_puts(s, " 9. Verify cookie is still valid on the cloned SKB\n");
	if (skb_cookie_get(skb2, test_handle) != COOKIE_API_PATTERN) {
		seq_puts(s, "SKB2 cookie read failed\n");
		goto free_skb2;
	}

	/* free SKB's */
	seq_puts(s, "10. Free both SKBs\n");
	kfree_skb(skb1);
	kfree_skb(skb2);

	/* verify destructor was called */
	seq_puts(s, "11. Verify destructor callback was called\n");
	if (test_api_cookie_allocated) {
		seq_puts(s, "cookie destructor failed\n");
		goto unregister_cookie;
	}

	/* verify get by name API */
	seq_puts(s, "12. Verify get by name API\n");
	if (test_handle != skb_cookie_get_handle_by_name(COOKIE_API_NAME)) {
		seq_puts(s, "get handle name failed\n");
		goto unregister_cookie;
	}

	/* unregister handle */
	seq_puts(s, "13. Unregister cookie handle\n");
	unregister_skb_cookie(test_handle);

	/* verify handle is unregistered */
	seq_puts(s, "14. Verify handle is unregistered\n");
	if (skb_cookie_get_handle_by_name(COOKIE_API_NAME) >= 0) {
		seq_puts(s, "handle unregister failed\n");
		goto unregister_cookie;
	}
	seq_puts(s, "\nAPI test done successfully\n\n");

	return 0;

free_skb2:
	kfree_skb(skb2);
free_skb1:
	kfree_skb(skb1);
unregister_cookie:
	unregister_skb_cookie(test_handle);
err:
	seq_puts(s, "\nCookie api test failed\n\n");
	return 0;
}

static int dbg_api_test_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_api_test_run, NULL);
}

static const struct file_operations dbg_api_test_fops = {
	.open = dbg_api_test_open,
	.read = seq_read,
	.release = single_release,
};

static int dbg_ver_show(struct seq_file *s, void *unused)
{
	if (capable(CAP_SYS_PACCT))
		seq_printf(s, "%s version %s\n", MOD_NAME, MOD_VERSION);

	return 0;
}

static int dbg_ver_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_ver_show, NULL);
}

static const struct file_operations dbg_ver_fops = {
	.open = dbg_ver_open,
	.read = seq_read,
	.release = single_release,
};

#ifdef SKBEXT_MOD_STATS
static int dbg_stats_show(struct seq_file *s, void *unused)
{
	struct skb_ext_db *db = s->private;

	if (unlikely(!db))
		goto done;

	if (!capable(CAP_SYS_PACCT))
		goto done;

	seq_printf(s,
		   "handles         %d\nactive          %d\n"
		   "max             %d\nadded           %d\n"
		   "removed         %d\ncollision       %d\n"
		   "copy-hdr-events %d\nrelease-events  %d\n"
		   "refc-alloc      %d\nrefc-free       %d\n\n",
		   atomic_read(&db->stat.act_handles),
		   atomic_read(&db->stat.act),
		   atomic_read(&db->stat.max),
		   atomic_read(&db->stat.added),
		   atomic_read(&db->stat.removed),
		   atomic_read(&db->stat.bkt_collision),
		   atomic_read(&db->stat.copy_hdr_events),
		   atomic_read(&db->stat.release_events),
		   atomic_read(&db->stat.ext_refc_alloc),
		   atomic_read(&db->stat.ext_refc_free));

done:
	return 0;
}

static int dbg_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_stats_show, inode->i_private);
}

static const struct file_operations dbg_stats_fops = {
	.open = dbg_stats_open,
	.read = seq_read,
	.release = single_release,
};

static int dbg_bkt_show(struct seq_file *s, void *unused)
{
	u32 idx;
	struct skb_ext_db *db = s->private;

	if (unlikely(!db))
		goto done;

	if (!capable(CAP_SYS_PACCT))
		goto done;

	seq_printf(s,
		   atomic_read(&db->stat.max) ?
		   "skb extension buckets statistics:\n"
		   "idx: act max\n" :
		   "skb extensions not added\n");

	FOR_EACH_SKB_EXT_BUCKET(idx) {
		if (atomic_read(&db->stat.bkt_max[idx])) {
			seq_printf(s,
				   "%04u: %03d %03d\n",
				   idx,
				   atomic_read(&db->stat.bkt_act[idx]),
				   atomic_read(&db->stat.bkt_max[idx]));
		}
	}

done:
	return 0;
}

static int dbg_bkt_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_bkt_show, inode->i_private);
}

static const struct file_operations dbg_bkt_fops = {
	.open = dbg_bkt_open,
	.read = seq_read,
	.release = single_release,
};
#endif /* SKBEXT_MOD_STATS */

static unsigned int __test_nf_cb(void *priv, struct sk_buff *skb,
				 const struct nf_hook_state *state)
{
	unsigned long i, j;

	if (state->hook == test_add_hook) {
		for (i = 0; i < test_num_handles; i++) {
			if (skb_cookie_attach(skb, test_c_handles[i],
					      i + 1, NULL))
				test_add_err++;
		}
	} else if (state->hook == test_get_hook) {
		for (i = 0; i < test_num_handles; i++) {
			for (j = 0; j < test_num_get; j++) {
				if (skb_cookie_get(skb, test_c_handles[i]) !=
				    i + 1)
					test_get_err++;
			}
		}
	}

	return NF_ACCEPT;
}

static struct nf_hook_ops test_ops[] = {
	{
		.hook     = __test_nf_cb,
		.pf       = NFPROTO_IPV4,
		.priority = INT_MIN,
	},
	{
		.hook     = __test_nf_cb,
		.pf       = NFPROTO_IPV4,
		.priority = INT_MIN,
	},
	{
		.hook     = __test_nf_cb,
		.pf       = NFPROTO_IPV6,
		.priority = INT_MIN,
	},
	{
		.hook     = __test_nf_cb,
		.pf       = NFPROTO_IPV6,
		.priority = INT_MIN,
	},
};

static int test_start(struct seq_file *s, void *unused)
{
	s32 i;
	char buf[32];

	if (unlikely(!s))
		goto done;

	if (!capable(CAP_NET_ADMIN))
		goto done;

	seq_puts(s, "\n");
	if (test_is_running) {
		seq_puts(s, "ERROR: test is running, stop the test first\n\n");
		goto done;
	}

	/* turn on the test */
	test_is_running = true;

	/* check number of required cookie add */
	if (test_num_add > MAX_COOKIE_HANDLES) {
		seq_printf(s, "Invalid number of add, add=%u, max=%u\n",
			   test_num_add, (u32)MAX_COOKIE_HANDLES);
		goto err;
	}

	/* check required add hook type */
	if (test_add_hook >= NF_INET_NUMHOOKS) {
		seq_printf(s, "Invalid add hook type, type=%u, max=%u\n",
			   test_add_hook, (u32)(NF_INET_NUMHOOKS - 1));
		goto err;
	}

	/* check required get hook type */
	if (test_get_hook >= NF_INET_NUMHOOKS) {
		seq_printf(s, "Invalid get hook type, type=%u, max=%u\n",
			   test_get_hook, (u32)(NF_INET_NUMHOOKS - 1));
		goto err;
	}

	/* allocate cookie handles */
	test_c_handles =
		kcalloc(test_num_add, sizeof(*test_c_handles), GFP_KERNEL);
	if (!test_c_handles) {
		seq_puts(s, "Failed to allocate memory\n");
		goto err;
	}

	/* register cookies */
	for (i = 0; i < test_num_add; i++) {
		scnprintf(buf, sizeof(buf), "cookie_%d_name", i);
		if (register_skb_cookie(&test_c_handles[i], buf)) {
			seq_printf(s, "Failed to register cookie %s\n", buf);
			goto unregister_cookies;
		}
	}

	/* update required nf hooks */
	test_ops[0].hooknum = test_add_hook;
	test_ops[1].hooknum = test_get_hook;
	test_ops[2].hooknum = test_add_hook;
	test_ops[3].hooknum = test_get_hook;

	/* store number of handles */
	test_num_handles = i;

	/* register nf hooks */
	if (unlikely(nf_register_net_hooks(&init_net, test_ops,
					   ARRAY_SIZE(test_ops)))) {
		seq_puts(s, "Install nf hooks failed\n");
		goto unregister_cookies;
	}

	seq_printf(s, "Test is running, add=%u, add_hook=%u, get=%u, get_hook=%u\n\n",
		   test_num_add, test_add_hook, test_num_get, test_get_hook);

	goto done;

unregister_cookies:
	/* unregister cookies */
	while (test_num_handles)
		unregister_skb_cookie(test_c_handles[--test_num_handles]);

	/* free cookie handles */
	kfree(test_c_handles);
err:
	/* turn off the test */
	test_is_running = false;
	test_num_handles = 0;
done:
	return 0;
}

static int test_start_open(struct inode *inode, struct file *file)
{
	return single_open(file, test_start, NULL);
}

static const struct file_operations test_start_fops = {
	.open = test_start_open,
	.read = seq_read,
	.release = single_release,
};

static void test_shut_down(void)
{
	/* unregister nf hooks */
	nf_unregister_net_hooks(&init_net, test_ops, ARRAY_SIZE(test_ops));

	/* unregister cookies */
	while (test_num_handles)
		unregister_skb_cookie(test_c_handles[--test_num_handles]);

	/* free cookie handles */
	kfree(test_c_handles);
}

static int test_stop(struct seq_file *s, void *unused)
{
	if (unlikely(!s))
		goto out;

	if (!capable(CAP_NET_ADMIN))
		goto out;

	seq_puts(s, "\n");
	if (!test_is_running) {
		seq_puts(s, "ERROR: test is not running\n\n");
		goto out;
	}

	/* turn off the test */
	test_is_running = false;
	test_shut_down();

	seq_puts(s, "Test stopped\n\n");

out:
	return 0;
}

static int test_stop_open(struct inode *inode, struct file *file)
{
	return single_open(file, test_stop, NULL);
}

static const struct file_operations test_stop_fops = {
	.open = test_stop_open,
	.read = seq_read,
	.release = single_release,
};

static int test_help(struct seq_file *s, void *unused)
{
	if (unlikely(!s))
		goto out;

	if (!capable(CAP_NET_ADMIN))
		goto out;

	seq_puts(s, "\n");
	seq_puts(s, "add_num........get/set the number of cookie 'add' per skb\n");
	seq_puts(s, "add_hook.......get/set the 'add' cookie NF_HOOKS\n");
	seq_puts(s, "add_err........get the 'add' cookie error counter\n");
	seq_puts(s, "get_num........get/set the number of cookie 'get' per skb\n");
	seq_puts(s, "get_hook.......get/set the 'get' cookie NF_HOOKS\n");
	seq_puts(s, "get_err........get the 'get' cookie error counter\n");
	seq_puts(s, "start..........start test\n");
	seq_puts(s, "stop...........stop test\n");

	seq_puts(s, "\n\n");
	seq_puts(s, "          NF_HOOKS:\n");
	seq_puts(s, "          0 - PRE_ROUTING\n");
	seq_puts(s, "          1 - LOCAL_IN\n");
	seq_puts(s, "          2 - FORWARD\n");
	seq_puts(s, "          3 - LOCAL_OUT\n");
	seq_puts(s, "          4 - POST_ROUTING\n");
	seq_puts(s, "\n");
out:
	return 0;
}

static int test_help_open(struct inode *inode, struct file *file)
{
	return single_open(file, test_help, NULL);
}

static const struct file_operations test_help_fops = {
	.open = test_help_open,
	.read = seq_read,
	.release = single_release,
};

static int test_get_err_show(void *data, u64 *val)
{
	if (capable(CAP_NET_ADMIN))
		*val = (u64)test_get_err;
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(test_get_err_fops, test_get_err_show, NULL, "%llu\n");

static int test_add_err_show(void *data, u64 *val)
{
	if (capable(CAP_NET_ADMIN))
		*val = (u64)test_add_err;
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(test_add_err_fops, test_add_err_show, NULL, "%llu\n");

static int test_num_add_set(void *data, u64 val)
{
	if (capable(CAP_NET_ADMIN))
		test_num_add = (u32)val;
	return 0;
}

static int test_num_add_show(void *data, u64 *val)
{
	if (capable(CAP_NET_ADMIN))
		*val = (u64)test_num_add;
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(test_num_add_fops, test_num_add_show, test_num_add_set,
			 "%llu\n");

static int test_num_get_set(void *data, u64 val)
{
	if (capable(CAP_NET_ADMIN))
		test_num_get = (u32)val;
	return 0;
}

static int test_num_get_show(void *data, u64 *val)
{
	if (capable(CAP_NET_ADMIN))
		*val = (u64)test_num_get;
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(test_num_get_fops, test_num_get_show, test_num_get_set,
			 "%llu\n");

static int test_add_hook_set(void *data, u64 val)
{
	if (capable(CAP_NET_ADMIN))
		test_add_hook = (u32)val;
	return 0;
}

static int test_add_hook_show(void *data, u64 *val)
{
	if (capable(CAP_NET_ADMIN))
		*val = (u64)test_add_hook;
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(test_add_hook_fops, test_add_hook_show,
			 test_add_hook_set, "%llu\n");

static int test_get_hook_set(void *data, u64 val)
{
	if (capable(CAP_NET_ADMIN))
		test_get_hook = (u32)val;
	return 0;
}

static int test_get_hook_show(void *data, u64 *val)
{
	if (capable(CAP_NET_ADMIN))
		*val = (u64)test_get_hook;
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(test_get_hook_fops, test_get_hook_show,
			 test_get_hook_set, "%llu\n");

s32 __init skb_ext_dbg_init(struct skb_ext_db *db)
{
	struct dentry *dent;
	struct dentry *test_dir;

	if (unlikely(!db)) {
		pr_err("Invalid module data pointer\n");
		return -EINVAL;
	}

	dir = debugfs_create_dir("skb_cookie_ext", NULL);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir failed\n");
		return -ENOENT;
	}

	dent = debugfs_create_file("version",
				   0400,
				   dir,
				   NULL,
				   &dbg_ver_fops);

	if (unlikely(IS_ERR_OR_NULL(dent)))
		goto fail;

	dent = debugfs_create_file("api_test",
				   0400,
				   dir,
				   NULL,
				   &dbg_api_test_fops);

	if (IS_ERR_OR_NULL(dent))
		goto fail;
#ifdef SKBEXT_MOD_STATS
	dent = debugfs_create_file("stats",
				   0400,
				   dir,
				   db,
				   &dbg_stats_fops);

	if (unlikely(IS_ERR_OR_NULL(dent)))
		goto fail;

	dent = debugfs_create_file("buckets",
				   0400,
				   dir,
				   db,
				   &dbg_bkt_fops);

	if (unlikely(IS_ERR_OR_NULL(dent)))
		goto fail;
#endif

	test_dir = debugfs_create_dir("test", dir);
	if (IS_ERR_OR_NULL(test_dir)) {
		pr_err("debugfs_create_dir failed\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("get_err", 0400, test_dir,
					  NULL, &test_get_err_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create get_err debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("add_err", 0400, test_dir,
					  NULL, &test_add_err_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create add_err debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("add_num", 0600, test_dir,
					  NULL, &test_num_add_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_num_add debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("get_num", 0600, test_dir,
					  NULL, &test_num_get_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_num_get debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("add_hook", 0600, test_dir,
					  NULL, &test_add_hook_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_add_hook debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("get_hook", 0600, test_dir,
					  NULL, &test_get_hook_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_add_hook debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file("start", 0400, test_dir,
				   NULL, &test_start_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_start debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file("stop", 0400, test_dir,
				   NULL, &test_stop_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_stop debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file("help", 0400, test_dir,
				   NULL, &test_help_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("Failed to create test_help debugfs file\n");
		goto fail;
	}

	return 0;

fail:
	pr_err("debugfs_create_file failed\n");
	debugfs_remove_recursive(dir);
	return -ENOENT;
}

void skb_ext_dbg_clean(void)
{
	if (test_is_running) {
		/* turn off the test */
		test_is_running = false;
		test_shut_down();
	}

	debugfs_remove_recursive(dir);
}
