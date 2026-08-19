/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "mtlkinc.h"

#ifndef CPTCFG_IWLWAV_SILENT

#include "mtlk_snprintf.h"

#define LOG_LOCAL_GID   GID_LOG
#define LOG_LOCAL_FID   1

#define MAX_CLOG_LEN  2048
#define MAX_CLOG_BUFS 6

struct clog_lock
{
  mtlk_osal_spinlock_t list;
  mtlk_osal_spinlock_t emerg;
};

struct clog_buf
{
  struct list_head lentry;
  char             buf[MAX_CLOG_LEN];
};

struct clog_auxiliary
{
  struct list_head     list;
  struct clog_lock     lock;
  struct clog_buf      bufs[MAX_CLOG_BUFS];
};

static struct clog_auxiliary clog_aux;

/* Special handling of LOG locks is required if OBJPOOL is enabled
 * because OBJPOOL is not available during LOG initialization/cleanup
 */

static __INLINE void
__log_osdep_lock_init (void)
{
#ifdef CPTCFG_IWLWAV_ENABLE_OBJPOOL
  mtlk_osal_lock_init_objpool(&clog_aux.lock.list);
  mtlk_osal_lock_init_objpool(&clog_aux.lock.emerg);
#else
  mtlk_osal_lock_init(&clog_aux.lock.list);
  mtlk_osal_lock_init(&clog_aux.lock.emerg);
#endif /* CPTCFG_IWLWAV_ENABLE_OBJPOOL */
}

static __INLINE void
__log_osdep_lock_cleanup (void)
{
#ifdef CPTCFG_IWLWAV_ENABLE_OBJPOOL
  mtlk_osal_lock_cleanup_objpool(&clog_aux.lock.list);
  mtlk_osal_lock_cleanup_objpool(&clog_aux.lock.emerg);
#else
  mtlk_osal_lock_cleanup(&clog_aux.lock.list);
  mtlk_osal_lock_cleanup(&clog_aux.lock.emerg);
#endif /* CPTCFG_IWLWAV_ENABLE_OBJPOOL */
}

static __INLINE void
__log_osdep_list_init (void)
{
  unsigned int i;

  INIT_LIST_HEAD(&clog_aux.list);

  for (i = 0; i < MAX_CLOG_BUFS; ++i) {
    list_add(&clog_aux.bufs[i].lentry, &clog_aux.list);
  }
}

void __MTLK_IFUNC
log_osdep_init (void)
{
  __log_osdep_lock_init();
  __log_osdep_list_init();
}

void __MTLK_IFUNC
log_osdep_cleanup (void)
{
  __log_osdep_lock_cleanup();
}

static __INLINE char *
__log_osdep_get_cbuf (void)
{
  char             *buf = NULL;
  struct list_head *e;

  mtlk_osal_lock_acquire(&clog_aux.lock.list);
  if (!list_empty(&clog_aux.list)) {
    e = clog_aux.list.next;
    list_del(e);

    buf = list_entry(e, struct clog_buf, lentry)->buf;
  }
  mtlk_osal_lock_release(&clog_aux.lock.list);
  return buf;
}

static __INLINE void
__log_osdep_put_cbuf (char *buf)
{
  struct clog_buf *cb = container_of(buf, struct clog_buf, buf[0]);

  mtlk_osal_lock_acquire(&clog_aux.lock.list);
  list_add(&cb->lentry, &clog_aux.list);
  mtlk_osal_lock_release(&clog_aux.lock.list);
}

#ifdef CPTCFG_IWLWAV_TSF_TIMER_TIMESTAMPS_IN_DEBUG_PRINTOUTS
uint32 __MTLK_IFUNC get_hw_time_stamp(void);
#endif /* CPTCFG_IWLWAV_TSF_TIMER_TIMESTAMPS_IN_DEBUG_PRINTOUTS */

static __INLINE void
__log_osdep_do_cbuf (char       *cmsg_str,
                     const char *fname,
                     int         line_no,
                     const char *level,
                     const char *suffix,
                     const char *fmt,
                     va_list     args)
{
  const int cmsg_ln =
#ifdef CPTCFG_IWLWAV_TSF_TIMER_TIMESTAMPS_IN_DEBUG_PRINTOUTS
  mtlk_snprintf(cmsg_str, MAX_CLOG_LEN,
                "[%010u|%010u] mtlk%s%s(%s:%d): ",
                mtlk_log_get_timestamp(), get_hw_time_stamp(), level, suffix, fname, line_no);
#else
  mtlk_snprintf(cmsg_str, MAX_CLOG_LEN,
                "[%010u] mtlk%s%s(%s:%d): ",
                mtlk_log_get_timestamp(), level, suffix, fname, line_no);
#endif /* CPTCFG_IWLWAV_TSF_TIMER_TIMESTAMPS_IN_DEBUG_PRINTOUTS */

  mtlk_vsnprintf(cmsg_str + cmsg_ln, MAX_CLOG_LEN - cmsg_ln,
                 fmt, args);
}

static __INLINE void
__log_osdep_do_emerg_cbuf (const char *fname,
                           int         line_no,
                           const char *level,
                           const char *kern_level,
                           const char *fmt,
                           va_list     args)
{
  static char emerg_buf[MAX_CLOG_LEN];

  mtlk_osal_lock_acquire(&clog_aux.lock.emerg);
  __log_osdep_do_cbuf(emerg_buf, fname, line_no, level, " **EB** ", fmt, args);
#ifdef CONFIG_WAVE_DEBUG
  printk("%s%s\n", kern_level, emerg_buf);
#else
  printk("%s\n", emerg_buf);
#endif
  mtlk_osal_lock_release(&clog_aux.lock.emerg);
}

void __MTLK_IFUNC
log_osdep_do (const char *fname,
              int         line_no,
              const char *level,
              const char *kern_level,
              const char *fmt,
              ...)
{
  char *buf;
  va_list args;

  va_start(args, fmt);
  buf = __log_osdep_get_cbuf();

  if (__LIKELY(buf != NULL)) {
    __log_osdep_do_cbuf(buf, fname, line_no, level, "", fmt, args);
#ifdef CONFIG_WAVE_DEBUG
    printk("%s%s\n", kern_level, buf);
#else
    printk("%s\n", buf);
#endif
    __log_osdep_put_cbuf(buf);
  } else {
    __log_osdep_do_emerg_cbuf(fname, line_no, level, kern_level, fmt, args);
  }
  va_end(args);
}

#endif /* CPTCFG_IWLWAV_SILENT */
