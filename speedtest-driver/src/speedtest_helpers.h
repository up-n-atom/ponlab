// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022 - 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * This file contains common helper functions used in speed test kernel module
 */

#ifndef __SPEEDTEST_HELPERS__
#define __SPEEDTEST_HELPERS__

#include <linux/time64.h>
#include <linux/timekeeping.h>
#include <linux/mutex.h>
#include <linux/math64.h>

#define KB		(1024L)
#define MB		(1024L * KB)
#define GB		(1024L * MB)

#define LOG_TAG		"speedtest"
/*! prints informational message */
#define LOG_INFO(fmt, args...)									\
	do {														\
		pr_info("["LOG_TAG"] "fmt"\n", ## args);				\
	} while (0)

/*! prints error message */
#define LOG_ERROR(fmt, args...)									\
	do {														\
		pr_err("["LOG_TAG" %s()] "fmt"\n", __func__, ## args);	\
	} while (0)

/*! prints debug message like function name and file */
#ifdef SPEEDTEST_DEBUG
#define LOG_DEBUG(fmt, args...)									\
	do {														\
		pr_info("["LOG_TAG" %s()] "fmt"\n",	__func__, ## args);	\
	} while (0)
#else
#define LOG_DEBUG(fmt, args...)		{ }
#endif

#ifndef UINT_MAX
#define UINT_MAX (~0U)
#endif

#define PAGE_SIZE_BY_ORDER(order)	(PAGE_SIZE * (1 << order))

#define GET_GREATER_U64(num1, num2)		\
		((num1 > num2) ? num1 : num2)

#define GET_GREATER_TIME(time_us1, time_us2)	\
		((time_us1 > time_us2) ? time_us1 : time_us2)

#define GET_LOWER_TIME(time_us1, time_us2)	\
		((time_us1 < time_us2) ? time_us1 : time_us2)

#define NOTEDOWN_TIME(time)	(time = sptest_get_time_now_us())

#define SAFE_SUB(num1, num2) (((num1) < (num2)) ? (UINT_MAX - (num2) + (num1)) : ((num1) - (num2)))

static __always_inline bool safe_mul64(uint64_t num1, uint64_t num2, uint64_t *result)
{
	*result = (num1 * num2);
	if ((num2 != 0) && (div64_u64(*result, num2) != num1)) {
		*result = 0;
		return false;
	}
	return true;
}

static inline time64_t sptest_get_time_now_us(void)
{
	struct timespec64 now = {0};
	time64_t microseconds = 0;

	ktime_get_real_ts64(&now);

	if (!safe_mul64(now.tv_sec, USEC_PER_SEC, &microseconds)) {
		LOG_ERROR("interger overflowed while multiplying (now.tv_sec * USEC_PER_SEC)");
	} else {
		microseconds += (now.tv_nsec / NSEC_PER_USEC);
	}

	return microseconds;
}

#endif //__SPEEDTEST_HELPERS__
