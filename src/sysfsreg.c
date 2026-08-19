/*
 *  Copyright 2020 - 2024 MaxLinear, Inc.
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
#include <dirent.h>
#include "include/ltq_pmutil_interface.h"

#define SYSFS_PATH_MAX 255

#define PATH_TO_REG "/sys/class/regulator/"
#define MAX_LINE_LEN 4096
#define SYSFS_REG_PATH_MAX 255

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "hwmonX/device" dir */
static size_t sysfs_read_file_reg(unsigned int reg, const char *fname,
				  char *buf, size_t buflen)
{
	char path[SYSFS_REG_PATH_MAX];
	int fd;
	ssize_t numread;

	if (sprintf_s(path, sizeof(path), PATH_TO_REG "regulator.%u/%s",
		      reg, fname) <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return 0;

/* read (buflen -1) bytes from fd and puts it into buf */
	numread = read(fd, buf, buflen - 1);
	if (numread < 1) {
		(void)close(fd);
		return 0;
	}

	buf[numread] = '\0';
	(void)close(fd);

	return (size_t)numread;
}

/* defines which variable to use */
enum {
	NAME_REG,
	STATE,
	STATUS,
	EXTCTRL,
	MAX_STRING_FILES_REG
};

static const char *string_files[MAX_STRING_FILES_REG] = {
	[NAME_REG] = "name",
	[STATE] = "state",
	[STATUS] = "status",
	[EXTCTRL] = "extctrl",
};

enum {
	VOLTS,
	MAX_VOLTS,
	MIN_VOLTS,
	MAX_VALUE_FILES_REG
};

static const char *value_files[MAX_VALUE_FILES_REG] = {
	[VOLTS]     = "microvolts",
	[MAX_VOLTS] = "max_microvolts",
	[MIN_VOLTS] = "min_microvolts",
};

static unsigned long sysfs_get_one_value(unsigned int reg, unsigned int which)
{
	unsigned long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES_REG)
		return 0;

	len = sysfs_read_file_reg(reg, value_files[which], linebuf,
				  sizeof(linebuf));
	if (len == 0)
		return 0;

	value = strtoul(linebuf, &endp, 0);

	if (endp == linebuf || errno == ERANGE)
		return 0;

	return value;
}

static char *sysfs_get_one_string(unsigned int reg, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_REG)
		return NULL;

	len = sysfs_read_file_reg(reg, string_files[which], linebuf,
				  sizeof(linebuf));

	if (len == 0)
		return NULL;

/* strdup() returnes  pointer to duplicated string, or NULL if insufficient
 * memory was available
 */
	result = strdup(linebuf);
	if (!result)
		return NULL;

/* if last character is a linefeed change it to the 0 bit */
	if (result[strnlen_s(result, sizeof(linebuf)) - 1] == '\n')
		result[strnlen_s(result, sizeof(linebuf)) - 1] = '\0';
	return result;
}

int sysfs_reg_exists(unsigned int reg)
{
	char path[SYSFS_REG_PATH_MAX];

	if (sprintf_s(path, sizeof(path), PATH_TO_REG "regulator.%u/",
		      reg) <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}

	return access(path, R_OK) == -1 ? false : true;
}

int sysfs_reg_name_exists(char *name)
{
	char *n;
	int i, ind, ret;
	int rnum = reg_get_max_num();

	for (i = 0; i < rnum; i++) {
		if (!sysfs_reg_exists(i))
			continue;

		n = sysfs_get_reg_name(i);
		if (!n)
			continue;

		ret = strcmp_s(n, MAX_LINE_LEN, name, &ind);
		if (ret != 0) {
			sysfs_put(n);
			continue; /* error*/
		}
		if (ind != 0) {
			sysfs_put(n);
			continue; /* not equal*/
		}

		sysfs_put(n);
		return i;
	}

	return -ENOENT;
}

static size_t sysfs_write_file(const char *path, const void *value, size_t len)
{
	int fd;
	ssize_t numwrite;

	fd = open(path, O_WRONLY);
	if (fd == -1)
		return 0;

	numwrite = write(fd, value, len);
	if (numwrite < 1) {
		close(fd);
		return 0;
	}

	close(fd);
	return numwrite;
}

int sysfs_reg_virt_consumer_set_voltage(int reg, unsigned int min_uV,
					unsigned int max_uV)
{
	char path[SYSFS_REG_PATH_MAX] = {'\0'};
	char reg_virt_consumer_str[] =  "reg-virt-consumer.";
	char *rname = NULL;
	char *rname_cons = NULL; /* virtual consumer folder name */
	DIR *dir = NULL;
	struct dirent *ent = NULL;
	int ret = -EINVAL;
	char uv_min_str[10] = {'\0'};
	char uv_max_str[10] = {'\0'};

	memset(uv_min_str, 0, sizeof(uv_min_str));
	memset(uv_max_str, 0, sizeof(uv_max_str));

	if (sprintf_s(path, sizeof(path), PATH_TO_REG "regulator.%u/",
		      reg) <= 0) {
		printf("sprintf_s failed\n");
		goto err;
	}

	rname = sysfs_get_reg_name(reg);
	if (!rname)
		goto err;

	dir = opendir(path);
	if (!dir)
		goto err;

	while ((ent = readdir(dir)) != NULL) {
		if (!strstr(ent->d_name, reg_virt_consumer_str))
			continue;

		if (!strstr(ent->d_name, rname))
			continue;

		rname_cons = strdup(ent->d_name);
		break;
	}

	if (!rname_cons)
		goto err; /* did not find virtual consumer */

	if (sprintf_s(uv_min_str, sizeof(uv_min_str), "%d", min_uV) <= 0) {
		printf("sprintf_s failed\n");
		goto err;
	}
	if (sprintf_s(uv_max_str, sizeof(uv_max_str), "%d", max_uV) <= 0) {
		printf("sprintf_s failed\n");
		goto err;
	}

	/* update regulator max_uV */
	if (sprintf_s(path, sizeof(path),
		      PATH_TO_REG "regulator.%u/%s/max_microvolts",
		      reg, rname_cons) <= 0) {
		printf("sprintf_s failed\n");
		goto err;
	}
	sysfs_write_file(path, &uv_max_str, sizeof(uv_max_str));

	/* update regulator min_uV */
	if (sprintf_s(path, sizeof(path),
		      PATH_TO_REG "regulator.%u/%s/min_microvolts",
		      reg, rname_cons) <= 0) {
		printf("sprintf_s failed\n");
		goto err;
	}
	sysfs_write_file(path, &uv_min_str, sizeof(uv_min_str));

	ret = 0;
err:
	if (rname_cons)
		free(rname_cons);
	if (rname)
		sysfs_put(rname);
	if (dir)
		closedir(dir);
	return ret;
}

char *sysfs_get_reg_name(unsigned int reg)
{
	return sysfs_get_one_string(reg, NAME_REG);
}

void sysfs_put_reg_name(char *name)
{
	if (!name)
		return;

	free(name);
}

char *sysfs_get_reg_extctrl(unsigned int reg)
{
	return sysfs_get_one_string(reg, EXTCTRL);
}

int sysfs_get_reg_uvolts(unsigned int reg)
{
	return sysfs_get_one_value(reg, VOLTS);
}

int sysfs_get_reg_min_uvolts(unsigned int reg)
{
	return sysfs_get_one_value(reg, MIN_VOLTS);
}

int sysfs_get_reg_max_uvolts(unsigned int reg)
{
	return sysfs_get_one_value(reg, MAX_VOLTS);
}
