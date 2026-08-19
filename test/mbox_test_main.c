#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <pon/pon_mbox_ikm.h>
#include <pon/pon_ip_msg.h>

static ssize_t version_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	struct ponfw_version fw_out;
	int res = 0;

	res = pon_mbox_send(PONFW_VERSION_CMD_ID, PONFW_READ, NULL, 0, &fw_out,
			    sizeof(fw_out));

	if (res < 0) {
		pr_info("Unable to send message to mbox, ret: %d\n", res);
		return -1;
	}

	if (res != sizeof(fw_out)) {
		pr_info("Message smaller (%i) than expected: %zu\n", res,
			sizeof(fw_out));
		return -1;
	}

	return sprintf(buf, "%d.%d\n", fw_out.maj, fw_out.min);
}

static const struct kobj_attribute pon_version_attribute =
						__ATTR_RO(version);

static struct kobject *sysfs_module_root;

static int __init mbox_test_init(void)
{
	int res;

	pr_info("Starting mbox_test module\n");

	sysfs_module_root = kobject_create_and_add("pon", NULL);

	if (!sysfs_module_root) {
		pr_info("Unable to create 'pon' object in /sys\n");
		return -1;
	}

	res = sysfs_create_file(sysfs_module_root, &pon_version_attribute.attr);
	if (res < 0) {
		pr_info("Failed to create sysfs entry (%d)\n", res);
		return res;
	}

	return 0;
}

static void __exit mbox_test_cleanup(void)
{
	pr_info("Stopping mbox_test module\n");
	sysfs_remove_file(sysfs_module_root, &pon_version_attribute.attr);
	kobject_del(sysfs_module_root);
}

module_init(mbox_test_init);
module_exit(mbox_test_cleanup);
MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
