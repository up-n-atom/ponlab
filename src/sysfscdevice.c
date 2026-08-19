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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <safe_str_lib.h>

#define PATH_TO_CDEV "/sys/class/thermal/"
#define MAX_LINE_LEN 4096
#define SYSFS_PATH_MAX 255

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "cooling_deviceX" dir */
static size_t sysfs_read_file_cdev(unsigned int cdev, const char *fname,
				   char *buf, size_t buflen)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numread;

	if (sprintf_s(path, sizeof(path), PATH_TO_CDEV "cooling_device%u/%s",
		      cdev, fname) <= 0) {
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
	CUR_STATE,
	MAX_STATE,
	MAX_VALUE_FILES_CDEV
};

static const char *value_files[MAX_VALUE_FILES_CDEV] = {
	[CUR_STATE] = "cur_state",
	[MAX_STATE] = "max_state",
};

static unsigned long sysfs_get_one_value(unsigned int cdev, unsigned int which)
{
	unsigned long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES_CDEV)
		return 0;

	len = sysfs_read_file_cdev(cdev, value_files[which], linebuf,
				   sizeof(linebuf));
	if (len == 0)
		return 0;

	value = strtoul(linebuf, &endp, 0);

	if (endp == linebuf || errno == ERANGE)
		return 0;
	return value;
}

/* read access to files which contain one string */

enum {
	NAME_CDEV,
	MAX_STRING_FILES_CDEV
};

static const char *string_files[MAX_STRING_FILES_CDEV] = {
	[NAME_CDEV] = "type",
};

static char *sysfs_get_one_string(unsigned int cdev, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_CDEV)
		return NULL;

	len = sysfs_read_file_cdev(cdev, string_files[which], linebuf,
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

int sysfs_cdev_exists(unsigned int cdev)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	statbuf.st_mode = 0;
	if (sprintf_s(file, SYSFS_PATH_MAX, PATH_TO_CDEV "cooling_device%u/",
		      cdev) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -EINVAL;

	return S_ISDIR(statbuf.st_mode) ? 0 : -EINVAL;
}

char *sysfs_get_cdev_name(unsigned int cdev)
{
	return sysfs_get_one_string(cdev, NAME_CDEV);
}

void sysfs_put_cdev_name(char *name)
{
	if (!name)
		return;

	free(name);
}

int sysfs_get_cdev_cur_state(unsigned int cdev)
{
	return sysfs_get_one_value(cdev, CUR_STATE);
}

int sysfs_get_cdev_max_state(unsigned int cdev)
{
	return sysfs_get_one_value(cdev, MAX_STATE);
}
