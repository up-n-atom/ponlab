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

#define PATH_TO_DBG "/sys/kernel/debug/"
#define MAX_LINE_LEN 12288
#define SYSFS_DBG_PATH_MAX 255

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "/sys/kernel/debug/" dir */
static size_t sysfs_read_file_dbg(const char *fname, char *buf, size_t buflen)
{
	char path[SYSFS_DBG_PATH_MAX];
	int fd;
	ssize_t numread;

	if (sprintf_s(path, sizeof(path), PATH_TO_DBG "%s", fname) <= 0) {
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

/* helper function to write a new value to a /sys file */
/* fname is a relative path under "/sys/kernel/debug/" dir */
static ssize_t sysfs_write_file_dbg(const char *fname, const char *value,
				    ssize_t len)
{
	char path[SYSFS_DBG_PATH_MAX];
	int fd;
	ssize_t numwrite;

	if (sprintf_s(path, sizeof(path), PATH_TO_DBG "%s", fname) <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}

	fd = open(path, O_WRONLY);
	if (fd == -1)
		return 0;

	numwrite = write(fd, value, (unsigned long)len);
	if (numwrite < 1) {
		(void)close(fd);
		return 0;
	}

	(void)close(fd);

	return numwrite;
}

/*****************************************************************************/
/* defines which variable to use */
enum {
	ENABLE1,
	I2C_CTRL1,
	VOL1,
	REG_SUMMARY,
	CLK_SUMMARY,
	MAX_STRING_FILES_DBG
};

static const char *string_files[MAX_STRING_FILES_DBG] = {
	[ENABLE1] = "ltq_regulator_cpufreq/enable1",
	[I2C_CTRL1] = "ltq_regulator_cpufreq/i2c_ctrl1",
	[VOL1] = "ltq_regulator_cpufreq/vol1",
	[REG_SUMMARY] = "regulator/regulator_summary",
	[CLK_SUMMARY] = "clk/clk_summary",
};

/*****************************************************************************/
static int sysfs_write_one_value_dbg(unsigned int which, const char *new_value,
				     ssize_t len)
{
	if (which >= MAX_STRING_FILES_DBG)
		return 0;

	if (sysfs_write_file_dbg(string_files[which], new_value, len) != len)
		return -ENODEV;

	return 0;
}

static char *sysfs_get_one_string(unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_DBG)
		return NULL;

	len = sysfs_read_file_dbg(string_files[which], linebuf,
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

static int sysfs_dbg_exists(const char *file)
{
	char path[SYSFS_DBG_PATH_MAX];

	if (sprintf_s(path, sizeof(path), PATH_TO_DBG "/%s", file) <= 0) {
		printf("sprintf_s failed\n");
		return false;
	}
	return access(path, R_OK) == -1 ? false : true;
}

int sysfs_dbg_reg_summary_exists(void)
{
	return  sysfs_dbg_exists(string_files[REG_SUMMARY]);
}

int sysfs_dbg_clk_summary_exists(void)
{
	return  sysfs_dbg_exists(string_files[CLK_SUMMARY]);
}

void sysfs_put(char *name)
{
	if (!name)
		return;
	free(name);
}

char *sysfs_get_dbg_enable1(void)
{
	return sysfs_get_one_string(ENABLE1);
}

char *sysfs_get_dbg_i2c_ctrl1(void)
{
	return sysfs_get_one_string(I2C_CTRL1);
}

char *sysfs_get_dbg_vol1(void)
{
	return sysfs_get_one_string(VOL1);
}

char *sysfs_get_dbg_reg_summary(void)
{
	return sysfs_get_one_string(REG_SUMMARY);
}

char *sysfs_get_dbg_clk_summary(void)
{
	return sysfs_get_one_string(CLK_SUMMARY);
}

int sysfs_modify_dbg_i2c_ctrl1(char *i2c)
{
	ssize_t len;
	char value[SYSFS_DBG_PATH_MAX];

	len = sprintf_s(value, SYSFS_DBG_PATH_MAX, "%s", i2c);
	if (len <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}

	return sysfs_write_one_value_dbg(I2C_CTRL1, value, len + 1);
}

int sysfs_modify_dbg_vol1(char *vol)
{
	ssize_t len;
	char value[SYSFS_DBG_PATH_MAX];

	len = sprintf_s(value, SYSFS_DBG_PATH_MAX, "%s", vol);
	if (len <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}

	return sysfs_write_one_value_dbg(VOL1, value, len + 1);
}
