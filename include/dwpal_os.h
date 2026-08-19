/******************************************************************************

         Copyright (c) 2022, MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/
#ifndef __DWPAL_OS_H__
#define __DWPAL_OS_H__

#include <pthread.h>

/* Result check for pthread functions */
#define DWPAL_CHECK_RET(func) ({ \
	int __res = (func); \
	if (__res) { \
		/* printf("%s; " #func " error!\n", __FUNCTION__); */ \
	} \
	__res; \
})

#define MUTEX_LOCK(__mutex)   DWPAL_CHECK_RET(pthread_mutex_lock(__mutex))
#define MUTEX_UNLOCK(__mutex) DWPAL_CHECK_RET(pthread_mutex_unlock(__mutex))

#endif // __DWPAL_OS_H__
