/*
 *  Copyright 2020 MaxLinear, Inc.
 *  Copyright 2016 - 2020 Intel Corporation
 *  Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *  Copyright 2009 - 2014 Lantiq Deutschland GmbH
 *  Copyright 2007 - 2008 Infineon Technologies AG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <limits.h>
#include <safe_str_lib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "include/ltq_pmutil_interface.h"

#define PATH_TO_TZONE "/sys/class/thermal/"
#define MAX_LINE_LEN 4096
#define SYSFS_PATH_MAX 255
#define TRIP_POINT_MAX 20

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "tzoneX/device" dir */
static size_t sysfs_read_file_tzone(unsigned int tzone, const char *fname,
				    char *buf, size_t buflen)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numread;

	if (sprintf_s(path, sizeof(path), PATH_TO_TZONE "thermal_zone%u/%s",
		      tzone, fname) <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return 0;

	numread = read(fd, buf, buflen - 1);
	if (numread < 1) {
		(void)close(fd);
		return 0;
	}

	buf[numread] = '\0';
	(void)close(fd);

	return (size_t)numread;
}

/* read access to files which contain one numeric value */

enum {
	TEMP,
	TRIP_TEMP,
	TRIP_HYST,
	MAX_VALUE_FILES_TZONE
};

static const char *value_files[MAX_VALUE_FILES_TZONE] = {
	[TEMP] = "temp",
	[TRIP_TEMP] = "trip_point",
	[TRIP_HYST] = "trip_point",
};

static long sysfs_get_one_value(unsigned int tzone, unsigned int which)
{
	long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES_TZONE)
		return 0;

	len = sysfs_read_file_tzone(tzone, value_files[which], linebuf,
				    sizeof(linebuf));
	if (len == 0)
		return 0;

	value = strtol(linebuf, &endp, 0);

	if (endp == linebuf || errno == ERANGE)
		return 0;

	return value;
}

static unsigned long sysfs_get_one_value_trip(unsigned int tzone, char *which)
{
	unsigned long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (!which)
		return 0;

	len = sysfs_read_file_tzone(tzone, which, linebuf, sizeof(linebuf));
	if (len == 0)
		return 0;

	value = strtoul(linebuf, &endp, 0);

	if (endp == linebuf || errno == ERANGE)
		return 0;

	return value;
}

/* read access to files which contain one string */

enum {
	NAME_TZONE,
	TRIP_TYPE,
	MAX_STRING_FILES_TZONE
};

static const char *string_files[MAX_STRING_FILES_TZONE] = {
	[NAME_TZONE] = "type",
	[TRIP_TYPE] = "trip_point",
};

static char *sysfs_get_one_string(unsigned int tzone, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_TZONE)
		return NULL;

	len = sysfs_read_file_tzone(tzone, string_files[which], linebuf,
				    sizeof(linebuf));
	if (len == 0)
		return NULL;

/* strdup() returnes  pointer to duplicated string, or NULL if insufficient
 * memory was available
 */
	result = strdup(linebuf);
	if (!result)
		return NULL;

	if (result[strnlen_s(result, sizeof(linebuf)) - 1] == '\n')
		result[strnlen_s(result, sizeof(linebuf)) - 1] = '\0';

	return result;
}

static char *sysfs_get_one_string_trip(unsigned int tzone, char *which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (!which)
		return NULL;

	len = sysfs_read_file_tzone(tzone, which, linebuf, sizeof(linebuf));
	if (len == 0)
		return NULL;

/* strdup() returnes  pointer to duplicated string, or NULL if insufficient
 * memory was available
 */
	result = strdup(linebuf);
	if (!result)
		return NULL;

	if (result[strnlen_s(result, sizeof(linebuf)) - 1] == '\n')
		result[strnlen_s(result, sizeof(linebuf)) - 1] = '\0';

	return result;
}

int sysfs_tzone_exists(unsigned int tzone)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	statbuf.st_mode = 0;

	if (sprintf_s(file, SYSFS_PATH_MAX, PATH_TO_TZONE "thermal_zone%u/",
		      tzone) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -EINVAL;

	return S_ISDIR(statbuf.st_mode) ? 0 : -EINVAL;
}

char *sysfs_get_tzone_name(unsigned int tzone)
{
	return sysfs_get_one_string(tzone, NAME_TZONE);
}

void sysfs_put_tzone_name(char *name)
{
	if (!name)
		return;
	free(name);
}

long sysfs_get_tzone_temp(unsigned int tzone)
{
	return sysfs_get_one_value(tzone, TEMP);
}

void sysfs_get_tzone_trip(unsigned int tzone, struct tz_data *tz)
{
	int i;
	char trip_point[TRIP_POINT_MAX];
	char *p;

	for (i = 0; i < TZONE_TRIP_MAX; i++) {
		if (sprintf_s(trip_point, sizeof(trip_point),
			      "trip_point_%d_temp", i) <= 0) {
			printf("sprintf_s failed\n");
			return;
		}
		(tz + i)->temp = sysfs_get_one_value_trip(tzone, trip_point);
		if (sprintf_s(trip_point, sizeof(trip_point),
			      "trip_point_%d_hyst", i) <= 0) {
			printf("sprintf_s failed\n");
			return;
		}
		(tz + i)->hyst = sysfs_get_one_value_trip(tzone, trip_point);
		if (sprintf_s(trip_point, sizeof(trip_point),
			      "trip_point_%d_type", i) <= 0) {
			printf("sprintf_s failed\n");
			return;
		}
		p = sysfs_get_one_string_trip(tzone, trip_point);
		if (p) {
			strncpy_s((tz + i)->type, TZ_TYPE_MAX, p,
				  (sizeof((tz + i)->type)) - 1);
			(tz + i)->type[(sizeof((tz + i)->type)) - 1] = '\0';
			free(p);
		}
	}
}
