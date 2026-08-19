/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "logs.h"

#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <linux/limits.h>
#ifdef CONFIG_ALLOW_SYSLOG
#include <syslog.h>
#endif /* CONFIG_ALLOW_SYSLOG */

#define __RST "\033[0m"

int __log_level = 1;
int __use_syslog = 1;

#ifdef CONFIG_ALLOW_SYSLOG
static int __opened_syslog = 0;

static int wv_syslog_priority(wv_log_type type)
{
	switch (type) {
	case WV_LOG_INFO:
		return LOG_INFO;
	case WV_LOG_ERR:
		return LOG_ERR;
	case WV_LOG_BUG:
		return LOG_CRIT;
	}
	return LOG_INFO;
}

int wv_open_syslog(const char *name)
{
	if (__opened_syslog)
		return 1;

	openlog(name, LOG_PID | LOG_NDELAY, LOG_DAEMON);
	__opened_syslog = 1;
	return 0;
}

int wv_close_syslog(void)
{
	if (!__opened_syslog)
		return 1;

	closelog();
	__opened_syslog = 0;
	return 0;
}
#else /* CONFIG_ALLOW_SYSLOG */
int wv_open_syslog(const char *name)
{
	(void)name;
	return 1;
}

int wv_close_syslog(void)
{
	return 1;
}
#endif /* CONFIG_ALLOW_SYSLOG */

static char g_app_fullPath[ PATH_MAX ] = "";
static const char* g_app_path = g_app_fullPath;

/* Read executable name */
static void __attribute__((constructor)) wv_log_init(void)
{
	const char* basename;
	ssize_t count = readlink( "/proc/self/exe", g_app_fullPath, PATH_MAX );
	if (count < 0) count = 0;
	else if (count >= PATH_MAX) count = PATH_MAX - 1;
	g_app_fullPath[count] = '\0';

	/* get basename */
	g_app_path = g_app_fullPath;
	basename = g_app_fullPath + count;
	while (basename > g_app_fullPath) {
		if (*(basename-1) == '/') {
			g_app_path = basename;
			break;
		}
		--basename;
	}
}

const char* wv_get_app_path(bool fullpath)
{
	return fullpath ? g_app_fullPath : g_app_path;
}

#ifdef CONFIG_NO_LOGS
void wv_log_msg(int lvl, wv_log_type type, const char *clr,
		const char *file, int line, const char *fmt, ...) {}
#else /* CONFIG_NO_LOGS */
void wv_log_msg(int lvl, wv_log_type type, const char *clr,
		const char *file, int line, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	if (__log_level >= lvl) {
#ifdef CONFIG_ALLOW_SYSLOG
		if (__use_syslog)
			vsyslog(wv_syslog_priority(type), fmt, ap);
		else
#endif /* CONFIG_ALLOW_SYSLOG */
		{
			struct timespec ts;
			clock_gettime(7 ,&ts);

			printf("%ld.%06u <%s> [%s:%d]: ",
			       ts.tv_sec, (unsigned int)(ts.tv_nsec / 1000),
			       g_app_path, file, line);
			if (clr)
				printf("%s", clr);
			vprintf(fmt, ap);
			if (clr)
				printf(__RST);
			printf("\n");
		}
	}
	va_end(ap);
}
#endif /* CONFIG_NO_LOGS */
