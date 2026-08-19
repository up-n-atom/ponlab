/******************************************************************************

                              Copyright (c) 2011
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#if defined(LINUX) && defined(__KERNEL__)

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,33))
#   include <linux/utsrelease.h>
#else
#   include <generated/utsrelease.h>
#endif

#if defined(CONFIG_PROC_FS)
#  include <linux/proc_fs.h>
#  include <linux/seq_file.h>
#endif

#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <linux/ioport.h>
#include <linux/irq.h>
#include <asm/io.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
#  undef CONFIG_DEVFS_FS
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
#define PDE_DATA(inode) PDE(inode)->data
#endif

#include "ifx_types.h"
#include "lib_cli_config.h"
#include "lib_cli_osmap.h"
#include "lib_cli_debug.h"
#include "lib_cli_core.h"


#ifdef CLI_STATIC
#	undef CLI_STATIC
#endif

#ifdef CLI_DEBUG
#	define CLI_STATIC
#else
#	define CLI_STATIC   static
#endif


#define CLI_MODULE_NAME "cli"

/* ==========================================================================
   Local declarations (LINUX)
   ========================================================================== */

/* ==========================================================================
   Local variables declarations (LINUX)
   ========================================================================== */

/* install parameter debug_level: LOW (1), NORMAL (2), HIGH (3), OFF (4) */
#ifdef DEBUG_CLI
static unsigned char debug_level = 3;
#else
static unsigned char debug_level = 4;
#endif
module_param(debug_level, byte, 0);
MODULE_PARM_DESC(debug_level, "set to get more (1) or fewer (4) debug outputs");


static unsigned char show_banner = 0;
module_param(show_banner, byte, 0);
MODULE_PARM_DESC(show_banner, "if set shows the start up info while module load");

#if defined(CONFIG_PROC_FS)

#if (CLI_DEBUG_PRINTOUT == 1)
#	define CLI_DEBUG_PRINT_INFO_STR	"Dbg     <Y>"
#else
#	define CLI_DEBUG_PRINT_INFO_STR	"Dbg     <N>"
#endif

#if (CLI_ERROR_PRINTOUT == 1)
#	define CLI_ERROR_PRINT_INFO_STR	"Err     <Y>"
#else
#	define CLI_ERROR_PRINT_INFO_STR	"Err     <N>"
#endif

#if (CLI_SUPPORT_BUFFER_OUT == 1)
#	define CLI_BUFFER_OUT_INFO_STR	"BufOut  <Y>"
#else
#	define CLI_BUFFER_OUT_INFO_STR	"BufOut  <N>"
#endif

#if (CLI_SUPPORT_FILE_OUT == 1)
#	define CLI_SUPPORT_FILE_OUT_INFO_STR	"FileOut <Y>"
#else
#	define CLI_SUPPORT_FILE_OUT_INFO_STR	"FileOut <N>"
#endif


extern const char g_cli_version_what[];
/**
   Read the version information from the driver.

   \param buf destination buffer

   \return
   - length of the string
*/
CLI_STATIC void cli_proc_version_show(struct seq_file *s)
{
	seq_printf(s, "%s" CLI_CRLF, &g_cli_version_what[4]);
	seq_printf(s, "Compiled on %s, %s for Linux kernel %s" CLI_CRLF,
		   __DATE__, __TIME__, UTS_RELEASE);

	seq_printf(s, "Feature:" CLI_CRLF);
	seq_printf(s, "   " CLI_DEBUG_PRINT_INFO_STR
		" | " CLI_ERROR_PRINT_INFO_STR
		CLI_CRLF);
	seq_printf(s, "   " CLI_BUFFER_OUT_INFO_STR
		" | " CLI_SUPPORT_FILE_OUT_INFO_STR
		CLI_CRLF);

	return;
}

typedef void (*cli_dump_t) (struct seq_file *s);
CLI_STATIC int cli_proc_show(struct seq_file *s, void *p)
{
	cli_dump_t dump = s->private;

	if (dump)
		dump(s);

	return 0;
}

CLI_STATIC int cli_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cli_proc_show, PDE_DATA(inode));
}

struct proc_entry
{
	char *name;
	void *callback;
	struct file_operations ops;
};

struct proc_entry proc_entries[] =
{
	{"version", cli_proc_version_show}
};

void cli_proc_entry_create(struct proc_dir_entry *parent_node,
			   struct proc_entry *proc_entry)
{
	memset(&proc_entry->ops, 0, sizeof(struct file_operations));
	proc_entry->ops.owner = THIS_MODULE;
	proc_entry->ops.open = cli_proc_open;
	proc_entry->ops.read = seq_read;
	proc_entry->ops.llseek = seq_lseek;
	proc_entry->ops.release = single_release;

	proc_create_data(proc_entry->name, (S_IFREG | S_IRUGO),
			 parent_node, &proc_entry->ops, proc_entry->callback);
}

/**
   Initialize and install the proc entry

   \return
   -1 or 0 on success

   \remarks
   Called by the kernel.
*/
CLI_STATIC int cli_proc_install(void)
{
	struct proc_dir_entry *driver_proc_node;

	driver_proc_node = proc_mkdir("driver/" CLI_MODULE_NAME, IFX_NULL);
	if (driver_proc_node != IFX_NULL)
	{
		unsigned int i;
		for (i = 0; i < sizeof(proc_entries) / sizeof(proc_entries[0]); i++)
			cli_proc_entry_create(driver_proc_node, &proc_entries[i]);
	}
	else
	{
		printk(KERN_ERR "cannot create proc entry" CLI_CRLF);
		return -1;
	}
	return 0;
}
#endif   /* #if defined(CONFIG_PROC_FS) */

/**
   Initialize the module (support devfs - device file system)

\return
   Error code or 0 on success

\remarks
   Called by the kernel.
*/
CLI_STATIC int __init cli_mod_init(void)
{
	IFX_uint8_t dev_num;

	if (show_banner != 0)
	{
		printk(KERN_INFO "%s" CLI_CRLF, &g_cli_version_what[4]);
		printk(KERN_INFO "(c) Copyright 2010, Lantiq Deutschland GmbH" CLI_CRLF);
	}

#if defined(CONFIG_PROC_FS)
	(void)cli_proc_install();
#endif

	return 0;
}

/**
   Clean up the module if unloaded.

\remarks
   Called by the kernel.
*/
CLI_STATIC void __exit cli_mod_exit(void)
{
	unsigned char i;
	char buf[64];

#if defined(CONFIG_PROC_FS)
	for (i = 0; i < sizeof(proc_entries) / sizeof(proc_entries[0]); i++)
	{
		sprintf(buf, "driver/" CLI_MODULE_NAME "/%s", proc_entries[i].name);
		remove_proc_entry(buf, 0);
	}
	remove_proc_entry("driver/" CLI_MODULE_NAME, 0);
#endif
}

/*
   register module init and exit
*/
module_init (cli_mod_init);
module_exit (cli_mod_exit);

/* CLI library functions */
EXPORT_SYMBOL(cli_sscanf);
EXPORT_SYMBOL(cli_core_cfg_cmd_shutdown);
EXPORT_SYMBOL(cli_core_cfg_mode_cmd_reg);
EXPORT_SYMBOL(cli_core_cfg_mode_active);
EXPORT_SYMBOL(cli_core_release);
EXPORT_SYMBOL(cli_core_init);
EXPORT_SYMBOL(cli_core_buildin_register);
EXPORT_SYMBOL(cli_check_help__buffer);
EXPORT_SYMBOL(cli_core_key_add__buffer);
EXPORT_SYMBOL(cli_core_cmd_exec__buffer);
EXPORT_SYMBOL(cli_user_if_register);
EXPORT_SYMBOL(cli_user_if_unregister);


MODULE_AUTHOR("www.lantiq.com");
MODULE_DESCRIPTION("CLI Library - www.lantiq.com");
MODULE_SUPPORTED_DEVICE("CLI Library");
MODULE_LICENSE("Dual BSD/GPL");

#endif /* #if defined(LINUX) && defined(__KERNEL__) */


