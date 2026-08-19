/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_linux.c
   This file contains the implementation SDD mailbox Linux specific functions.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_config.h"
#include "drv_sdd_mbx_trace.h"
#include "drv_sdd_mbx_version.h"
#include "drv_sdd_mbx_registers.h"
#include "drv_sdd_mbx_spi.h"
#include "drv_sdd_mbx_handler.h"
#ifdef EVENT_LOGGER_DEBUG
   #include "el_log_macros.h"
#endif /* EVENT_LOGGER_DEBUG */
#include <linux/module.h>

/* ========================================================================== */
/*                             Macro definitions                              */
/* ========================================================================== */
#define DRV_SDD_MBX_DEV_NAME       "drv_sdd_mbx"

/** Enable proc filesystem support. */
#ifdef SDD_MBX_WITH_PROC_FS
   #include <generated/utsrelease.h>
   #include <linux/proc_fs.h>
   #include <linux/seq_file.h>
   #include <linux/slab.h>
#endif

/** Enable device filesystem support. */
#ifdef SDD_MBX_WITH_DEV_FS
   #include <linux/cdev.h>
#endif

/* Do not printout __DATE__ and __TIME__ with GCC version >= 4.9. */
#if !defined(__GNUC__) || \
    (__GNUC__ < 4) || \
    (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
   #define SDDMBX_FEAT_USE_TIME_DATE
#endif /* GCC version < 4.9 */

/* ========================================================================== */
/*                            Types definitions                               */
/* ========================================================================== */
#ifdef SDD_MBX_WITH_PROC_FS
struct proc_entry
{
   const char* name;
   void* read_function;
   void* write_function;
   struct file_operations ops;
};

typedef void (*sdd_mbx_proc_dump) (struct seq_file *s);
#endif /* SDD_MBX_WITH_DEV_FS */

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
#ifdef SDD_MBX_WITH_PROC_FS
static IFX_int32_t sdd_mbx_proc_create(struct proc_dir_entry* proc_directory);
static IFX_int32_t sdd_mbx_proc_entry_create(struct proc_dir_entry *parent_node, struct proc_entry *proc_entry);
static IFX_int32_t sdd_mbx_proc_version_get(struct seq_file *s);
static IFX_int32_t sdd_mbx_proc_status_get(struct seq_file *s);
static IFX_int32_t sdd_mbx_proc_debug_get(struct seq_file *s);
static IFX_int32_t sdd_mbx_proc_debug_set(struct file *file, const char *buffer, unsigned long count, void *data);
static IFX_int32_t sdd_mbx_proc_cmd_read_get(struct seq_file *s);
static IFX_int32_t sdd_mbx_proc_cmd_read_set(struct file *file, const char *buffer, unsigned long count, void *data);
static IFX_int32_t sdd_mbx_proc_cmd_write_set(struct file *file, const char *buffer, unsigned long count, void *data);
static IFX_int32_t sdd_mbx_proc_evt_mbx_get(struct seq_file *s);
static IFX_int32_t sdd_mbx_proc_reg_set(struct file *file, const char *buffer, unsigned long count, void *data);
static IFX_int32_t sdd_mbx_proc_help_get(struct seq_file *s);
static IFX_int32_t sdd_mbx_proc_show(struct seq_file *s, void *p);
static IFX_int32_t sdd_mbx_proc_open(struct inode* inode, struct file* file);
static IFX_int32_t sdd_mbx_proc_remove(struct proc_dir_entry* proc_directory);
static IFX_int32_t sdd_mbx_proc_configure_get(struct seq_file *s);
#endif /* SDD_MBX_WITH_DEV_FS */

#ifdef SDD_MBX_WITH_DEV_FS
IFX_int32_t sdd_mbx_ioctl_open (struct inode *inode, struct file *file_p);
IFX_int32_t sdd_mbx_ioctl_close (struct inode *inode, struct file *file_p);
long sdd_mbx_ioctl (struct file *file_p, IFX_uint32_t nCmd, IFX_ulong_t arg);
#endif /* SDD_MBX_WITH_DEV_FS */

/* ========================================================================== */
/*                             Local variables                                */
/* ========================================================================== */
extern SDD_MBX_CONTEXT_t sdd_mbx_context;
extern IFX_char_t *dcdc_type;

#ifdef SDD_MBX_WITH_PROC_FS
static struct proc_entry proc_entries[] =
{
   {"version",   sdd_mbx_proc_version_get},
   {"status",    sdd_mbx_proc_status_get},
   {"configure", sdd_mbx_proc_configure_get},
   {"debug",     sdd_mbx_proc_debug_get, sdd_mbx_proc_debug_set},
   {"cmd_read",   sdd_mbx_proc_cmd_read_get, sdd_mbx_proc_cmd_read_set},
   {"cmd_write",   NULL, sdd_mbx_proc_cmd_write_set},
   {"evt_read",   sdd_mbx_proc_evt_mbx_get, NULL},
   {"reg_write",   NULL, sdd_mbx_proc_reg_set},
   {"help",   sdd_mbx_proc_help_get, NULL}
};
#endif /* SDD_MBX_WITH_PROC_FS */

#ifdef SDD_MBX_WITH_DEV_FS
static struct file_operations drv_sdd_mbx_fops =
{
   owner          :  THIS_MODULE,
   poll           :  0,
   unlocked_ioctl :  sdd_mbx_ioctl,
   open           :  sdd_mbx_ioctl_open,
   release        :  sdd_mbx_ioctl_close
};
#endif

#if (defined(MODULE))
MODULE_AUTHOR ("Lantiq Beteiligungs-GmbH & Co.KG");
MODULE_DESCRIPTION ("VCODEC mailbox driver for XWAY(TM) GRX500 family");
MODULE_SUPPORTED_DEVICE ("XWAY(TM) GRX500 family");
MODULE_LICENSE ("Dual BSD/GPL");
module_param(dcdc_type, charp, 0);
MODULE_PARM_DESC(dcdc_type, "DC/DC HW type ID string");
#endif /* defined(MODULE) */

/* ========================================================================== */
/*                         Function implementation                            */
/* ========================================================================== */


#ifdef SDD_MBX_WITH_PROC_FS
static IFX_int32_t sdd_mbx_proc_create(struct proc_dir_entry* proc_directory)
{
   IFX_uint32_t i;

   /* install the proc entry */
   TRACE("using proc fs");

   proc_directory = proc_mkdir("driver/" DRV_SDD_MBX_DEV_NAME, IFX_NULL);

   if(proc_directory != IFX_NULL)
   {
      for(i = 0; i < sizeof(proc_entries)/sizeof(proc_entries[0]); i++)
      {
         if(sdd_mbx_proc_entry_create(proc_directory, &proc_entries[i]) != SDD_FW_ERR_NO_ERROR)
         {
            ERROR("cannot create proc entry");
         }
      }
   }
   else
   {
      ERROR("cannot create /proc/" DRV_SDD_MBX_DEV_NAME);

      return IFX_ERROR;
   }
   return SDD_FW_ERR_NO_ERROR;
}

static IFX_int32_t sdd_mbx_proc_entry_create(struct proc_dir_entry *parent_node, struct proc_entry *proc_entry)
{
   memset(&proc_entry->ops, 0, sizeof(struct file_operations));
   proc_entry->ops.owner   = THIS_MODULE;
   proc_entry->ops.open    = sdd_mbx_proc_open;
   proc_entry->ops.read    = seq_read;
   proc_entry->ops.write   = proc_entry->write_function;
   proc_entry->ops.llseek  = seq_lseek;
   proc_entry->ops.release = single_release;

   if(proc_create_data(proc_entry->name, 0, parent_node, &proc_entry->ops,
                     proc_entry->read_function))
   {
      return SDD_FW_ERR_NO_ERROR;
   }
   else
   {
      return IFX_ERROR;
   }
}

/**
 * Create MPS version proc file output.
 * This function creates the output for the MPS version proc file
 *
 * \param   s        Pointer to seq_file struct.
 * \return  0 on success
 * \ingroup Internal
 */
static IFX_int32_t sdd_mbx_proc_version_get(struct seq_file *s)
{
   seq_printf(s, SDD_MBX_INFO ", version %d.%d.%d.%d\n"
      SDD_MBX_COPYRIGHT "\n",
      MAJORSTEP, MINORSTEP, VERSIONSTEP, VERS_TYPE);

   #ifdef SDDMBX_FEAT_USE_TIME_DATE
      seq_printf(s, "Compiled on %s, %s for Linux kernel %s\n",
         __DATE__, __TIME__, UTS_RELEASE);
   #else
      seq_printf(s, "Compiled for Linux kernel %s\n",
         UTS_RELEASE);
   #endif

   return 0;
}

/**
   This function creates the output for the status proc file

   \param s Pointer to seq_file struct.

   \return IFX_int32_t 0 on success
*/
static IFX_int32_t sdd_mbx_proc_status_get(struct seq_file *s)
{
   IFX_uint16_t value;

   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_CFG, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_CFG:        0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_IEN1, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_IEN1:       0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_IEN2, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_IEN2:       0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_STAT1, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_STAT1:      0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_STAT2, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_STAT2:      0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_INT1, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_INT1:       0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_INT2, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_INT2:       0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_CB_LEN_IN, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_CB_LEN_IN:  0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_CB_LEN_OUT, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_CB_LEN_OUT: 0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_EB_LEN_IN, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_EB_LEN_IN:  0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_EB_LEN_OUT, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_EB_LEN_OUT: 0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_LIMIT1I, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_LIMIT1I:    0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_LIMIT1O, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_LIMIT1O:    0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_LIMIT2I, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_LIMIT2I:    0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_LIMIT2O, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_LIMIT2O:    0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_CMD, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_CMD:        0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_REGB, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_REGB:       0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_REG2, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_REG2:       0x%04x (%u)\n", value, value);
   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_REG4, &value, 2);
   seq_printf(s, "SDD_MBX_HOST_REG4:       0x%04x (%u)\n", value, value);
   return 0;
}

/**
 * Create MPS version proc file output.
 * This function creates the output for the MPS version proc file
 *
 * \param   s        Pointer to seq_file struct.
 * \return  0 on success
 * \ingroup Internal
 */
static IFX_int32_t sdd_mbx_proc_configure_get(struct seq_file *s)
{
   seq_printf(s, "configure:" SDD_MBX_CONFIGURE_STR "\n");

   return 0;
}

static IFX_int32_t sdd_mbx_proc_debug_set(struct file *file, const char *buffer, unsigned long count, void *data)
{
   char input_line[1];

   if (count < 1)
   {
      /* error: No input. */
      return -EFAULT;
   }

   if (copy_from_user(input_line, buffer, 1) != 0)
   {
      /* error: Data copy from user space failed. */
      return -EFAULT;
   }

   /* Look only at the first character. */
   if (input_line[0] > '0' && input_line[0] < '4')
   {
      sdd_mbx_context.trace_level = input_line[0] - '0';
   }
   else
   {
      /* error: debug level out of range (1 -- 4). */
      return -EFAULT;
   }

   return count;
}

static IFX_int32_t sdd_mbx_proc_debug_get(struct seq_file *s)
{
   seq_printf(s, "levels: 1 - all logs, 2 - infos and errors, 3 - off\n");
   seq_printf(s, "current debug level: %d\n", sdd_mbx_context.trace_level);

   return 0;
}

static IFX_int32_t sdd_mbx_proc_cmd_write_set(struct file *file, const char *buffer, unsigned long count, void *data)
{
   FIO_SDD_MBX_READ_t evt_read;
   char* tokens = NULL;

   memset(&evt_read, 0, sizeof(evt_read));

   if (count < 1)
   {
      /* error: No input. */
      return -EFAULT;
   }

   tokens = (char*) kmalloc(count, GFP_KERNEL);

   if(tokens == NULL)
   {
      /* error: Data copy from user space failed. */
      return -EFAULT;
   }

   if (copy_from_user(tokens, buffer, count) != 0)
   {
      /* error: Data copy from user space failed. */
      kfree(tokens);
      return -EFAULT;
   }

   sscanf(tokens, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
      &evt_read.data[0], &evt_read.data[1], &evt_read.data[2], &evt_read.data[3],
      &evt_read.data[4], &evt_read.data[5], &evt_read.data[6], &evt_read.data[7],
      &evt_read.data[8], &evt_read.data[9], &evt_read.data[10], &evt_read.data[11],
      &evt_read.data[12], &evt_read.data[13], &evt_read.data[14], &evt_read.data[15]);

   kfree(tokens);

   sdd_mbx_context.interface->open("command");

   sdd_mbx_context.interface->cmd_write((IFX_uint8_t*) &evt_read, sizeof(evt_read));

   return count;
}

static IFX_int32_t sdd_mbx_proc_cmd_read_set(struct file *file, const char *buffer, unsigned long count, void *data)
{
   FIO_SDD_MBX_READ_t evt_read;
   int i = 0;
   char* tokens = NULL;

   memset(&evt_read, 0, sizeof(evt_read));

   if (count < 1)
   {
      /* error: No input. */
      return -EFAULT;
   }

   tokens = (char*) kmalloc(count, GFP_KERNEL);

   if(tokens == NULL)
   {
      /* error: Data copy from user space failed. */
      return -EFAULT;
   }

   if (copy_from_user(tokens, buffer, count) != 0)
   {
      /* error: Data copy from user space failed. */
      kfree(tokens);
      return -EFAULT;
   }

   sscanf(tokens, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
      &evt_read.data[0], &evt_read.data[1], &evt_read.data[2], &evt_read.data[3],
      &evt_read.data[4], &evt_read.data[5], &evt_read.data[6], &evt_read.data[7],
      &evt_read.data[8], &evt_read.data[9], &evt_read.data[10], &evt_read.data[11],
      &evt_read.data[12], &evt_read.data[13], &evt_read.data[14], &evt_read.data[15]);

   kfree(tokens);

   sdd_mbx_context.interface->open("command");

   sdd_mbx_context.interface->cmd_read((IFX_uint8_t*) &evt_read, sizeof(evt_read));

   for(i = 0; i < sizeof(evt_read) >> 2; i++)
   {
      printk("0x%08X\n", evt_read.data[i]);
   }

   return count;
}

static IFX_int32_t sdd_mbx_proc_cmd_read_get(struct seq_file *s)
{
   IFX_uint16_t tmp;

   sdd_mbx_spi_read(&sdd_mbx_context.device, SDD_MBX_HOST_CB_DATA, &tmp, 2);
   seq_printf(s, "value: 0x%x (%d)\n", tmp, tmp);

   return 0;
}

static IFX_int32_t sdd_mbx_proc_reg_set(struct file *file, const char *buffer, unsigned long count, void *data)
{
   char input_line[10];
   unsigned short val;
   unsigned int reg;

   if (count < 3)
   {
      /* error: No input. */
      return -EFAULT;
   }

   if (copy_from_user(input_line, buffer, count) != 0)
   {
      /* error: Data copy from user space failed. */
      return -EFAULT;
   }

   sscanf(input_line, "%d %hx", &reg, &val);

   sdd_mbx_reg_write_word(&sdd_mbx_context.device, reg, &val);

   return count;
}

static IFX_int32_t sdd_mbx_proc_evt_mbx_get(struct seq_file *s)
{
   FIO_SDD_MBX_READ_t evt_read;
   IFX_int32_t len = 0;
   IFX_int32_t i = 0;

   len = sdd_mbx_event_read(&sdd_mbx_context.device, (IFX_uint8_t*)&evt_read, sizeof(evt_read));

   if((len > 0) && (len < sizeof(evt_read.data)/sizeof(evt_read.data[0])))
   {
      seq_printf(s, "value: ");
      for(i = 0; i < len/4; i++)
      {
         seq_printf(s, "0x%08X ", evt_read.data[i]);
      }
      seq_printf(s, "\n");
   }
   else
   {
      seq_printf(s, "ERROR\n");
   }

   return 0;
}

static IFX_int32_t sdd_mbx_proc_help_get(struct seq_file *s)
{
   seq_printf(s, "============== SDD mailbox driver - usage ==============\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/status (R)\n");
   seq_printf(s, "This file returns the current values of all VCODEC registers,\n");
   seq_printf(s, "except CMD (0x09) and EVENT (0x0A).\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/version (R)\n");
   seq_printf(s, "Returns SDD mailbox driver version and compile time.\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/configure (R)\n");
   seq_printf(s, "Returns all used configuration options\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/debug (R/W)\n");
   seq_printf(s, "Sets or reads current debug level.\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/reg_write (W)\n");
   seq_printf(s, "This file writes a value to a given register.\n");
   seq_printf(s, "eg: echo \"3 0x8801\" > /proc/driver/ltq_sdd_mbx/reg_write\n");
   seq_printf(s, "This command sets IEN1 (0x03) register.\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/evt_read (R)\n");
   seq_printf(s, "This file reads one event from the event mailbox.\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/cmd_read (R/W)\n");
   seq_printf(s, "Writes cmd-read request and returns the values when reading.\n");
   seq_printf(s, "eg: echo \"0x84000310\" > /proc/driver/ltq_sdd_mbx/cmd_read\n");
   seq_printf(s, "This command reads calibration data.\n");
   seq_printf(s, "\n");
   seq_printf(s, "/proc/driver/ltq_sdd_mbx/cmd_write (W)\n");
   seq_printf(s, "Writes executing command write.\n");
   return 0;
}

static IFX_int32_t sdd_mbx_proc_show(struct seq_file *s, void *p)
{
   sdd_mbx_proc_dump dump = s->private;

   if (dump != NULL)
      dump(s);

   return 0;
}

static IFX_int32_t sdd_mbx_proc_open(struct inode* inode, struct file* file)
{
   return single_open(file, sdd_mbx_proc_show, PDE_DATA(inode));
}

static IFX_int32_t sdd_mbx_proc_remove(struct proc_dir_entry* proc_directory)
{
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/version", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/status", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/configure", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/debug", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/cmd_read", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/cmd_write", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/evt_read", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/reg_write", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME "/help", proc_directory);
   remove_proc_entry("driver/" DRV_SDD_MBX_DEV_NAME, proc_directory);
   return 0;
}

#endif /* WITH_PROC_FS */

#ifdef SDD_MBX_WITH_DEV_FS
/**
 * Open SDD MBX device.
 * Open the device from user mode (e.g. application) or kernel mode. An inode
 * value of 0..7 indicates a kernel mode access. In such a case the inode value
 * is used as minor ID.
 *
 * \param   inode   Pointer to device inode
 * \param   file_p  Pointer to file descriptor
 * \return  0       SDD_FW_ERR_NO_ERROR, device opened
 * \return  EMFILE  Device already open
 * \return  EINVAL  Invalid minor ID
 * \ingroup API
 */
IFX_int32_t sdd_mbx_ioctl_open (struct inode* inode, struct file* file_p)
{
   TRACE("file opened");
   return 0;
}

long sdd_mbx_ioctl(struct file* file_p, IFX_uint32_t nCmd, IFX_ulong_t arg)
{
   return sdd_mbx_ioctl_handler(nCmd, (IFX_void_t*) arg);
}

/**
 * Close SDD MBX device.
 * Close the device from user mode (e.g. application) or kernel mode. An inode
 * value of 0..7 indicates a kernel mode access. In such a case the inode value
 * is used as minor ID.
 *
 * \param   inode   Pointer to device inode
 * \param   file_p  Pointer to file descriptor
 * \return  0       SDD_FW_ERR_NO_ERROR, device closed
 * \return  ENODEV  Device invalid
 * \return  EINVAL  Invalid minor ID
 * \ingroup API
 */
IFX_int32_t sdd_mbx_ioctl_close (struct inode* inode, struct file* file_p)
{
   TRACE("file closed");
   return 0;
}

/**
   This function registers char device in kernel.
\param   pDev     pointer to mps_comm_dev structure
\return  0        success
\return  -ENOMEM
\return  -EPERM
*/

IFX_int32_t sdd_mbx_os_register (SDD_MBX_CONTEXT_t* pContext)
{
   IFX_int32_t ret;
   dev_t       dev;
   struct cdev* p_cdev = NULL;

   /* dynamic major */
   ret = alloc_chrdev_region(&dev, 1, 1, DRV_SDD_MBX_DEV_NAME);
   pContext->major_number = MAJOR(dev);

   if (ret < 0)
   {
      ERROR("can't get major");
      return ret;
   }

   TRACE("registered with major %d", pContext->major_number);

   p_cdev = cdev_alloc();

   if (NULL == p_cdev)
      return -ENOMEM;

   cdev_init(p_cdev, &drv_sdd_mbx_fops);
   p_cdev->owner = THIS_MODULE;

   ret = cdev_add(p_cdev, MKDEV(pContext->major_number, pContext->minor_number), 1);

   if (ret != 0)
   {
      cdev_del (p_cdev);
      return -EPERM;
   }

   return ret;
}

/**
   This function unregisters char device from kernel.
   \param   pDev     pointer to mps_comm_dev structure
*/
IFX_void_t sdd_mbx_os_unregister (SDD_MBX_CONTEXT_t *pContext)
{
   unregister_chrdev_region(MKDEV(pContext->major_number, pContext->minor_number), 1);
}
#endif /* SDD_MBX_WITH_DEV_FS */

/**
   This function initializes the module.

   \return  SDD_FW_ERR_NO_ERROR, module initialized
   \return  EPERM    Reset of CPU1 failed
   \return  ENOMEM   No memory left for structures
*/
IFX_int32_t sdd_mbx_init_module (void)
{
   printk(SDD_MBX_INFO " version %d.%d.%d.%d " SDD_MBX_COPYRIGHT "\n",
           MAJORSTEP, MINORSTEP, VERSIONSTEP, VERS_TYPE);

   memset (&sdd_mbx_context, 0, sizeof(SDD_MBX_CONTEXT_t));

   sdd_mbx_context.trace_level = TRACE_ERROR;

   sdd_mbx_context.minor_number = 1;

#ifdef EVENT_LOGGER_DEBUG
   EL_REG_Register(DRV_SDD_MBX_DEV_NAME, DEV_TYPE_SDD, 0, IFX_NULL);
#endif /* EVENT_LOGGER_DEBUG */

#ifdef SDD_MBX_WITH_PROC_FS
   /* Create proc fs entries for driver */
   if(sdd_mbx_proc_create(sdd_mbx_context.proc_dir) != SDD_FW_ERR_NO_ERROR)
   {
      return -ENOMEM;
   }
#endif /* SDD_MBX_WITH_PROC_FS */

#ifdef SDD_MBX_WITH_DEV_FS
   if(sdd_mbx_os_register(&sdd_mbx_context) != SDD_FW_ERR_NO_ERROR)
   {
      return -ENOMEM;
   }
#endif

   sema_init(&sdd_mbx_context.device.spi_access_mtx, 1);
   mutex_init(&sdd_mbx_context.device.cmd_mtx);
   /* init cmd outbox data ready event */
   IFXOS_EventInit(&sdd_mbx_context.device.cmd_irq_evt);

   sdd_mbx_spi_init(&sdd_mbx_context.device);

   return SDD_FW_ERR_NO_ERROR;
}

/**
   This function cleans up the module.

   \return IFX_void_t
*/
IFX_void_t sdd_mbx_cleanup_module (void)
{
#ifdef EVENT_LOGGER_DEBUG
  EL_REG_Unregister(DRV_SDD_MBX_DEV_NAME, DEV_TYPE_SDD, 0);
#endif /* EVENT_LOGGER_DEBUG */

#ifdef SDD_MBX_WITH_PROC_FS
   sdd_mbx_proc_remove(sdd_mbx_context.proc_dir);
#endif /* SDD_MBX_WITH_PROC_FS */

#ifdef SDD_MBX_WITH_DEV_FS
   sdd_mbx_os_unregister(&sdd_mbx_context);
#endif /* SDD_MBX_WITH_DEV_FS */

   sdd_mbx_spi_exit(&sdd_mbx_context.device);

   TRACE("closing done");
}

module_init (sdd_mbx_init_module);
module_exit (sdd_mbx_cleanup_module);
