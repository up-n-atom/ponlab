/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __WAVE_GEN_LOGS__H__
#define __WAVE_GEN_LOGS__H__

#include <stdio.h>
#ifdef CONFIG_ALLOW_SYSLOG
#include <syslog.h>
#endif /* CONFIG_ALLOW_SYSLOG */

#define __GEN_LOG_CLR_ERR "\033[0;31m" // Red

extern int __gen_log_level;
extern int __gen_use_syslog;

int gen_open_syslog(const char *name);
int gen_close_syslog(void);

typedef enum {
	GEN_LOG_INFO,
	GEN_LOG_ERR,
	GEN_LOG_BUG,
} gen_log_type;

void gen_log_msg(int lvl, gen_log_type type, const char *clr,
		const char *file, int line, const char *fmt, ...)
		__attribute__ ((format(printf,6,7)));

#define GENLOG(__lvl, fmt, ...) \
gen_log_msg(__lvl, GEN_LOG_INFO, NULL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define console_printf(fmt, ...) \
gen_log_msg(1, GEN_LOG_INFO, NULL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define console_printf_err(fmt, ...) \
gen_log_msg(1, GEN_LOG_ERR, __GEN_LOG_CLR_ERR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif /* __WAVE_GEN_LOGS__H__ */
