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

#define PATH_TO_HWMON "/sys/class/hwmon/"
#define MAX_LINE_LEN 4096
#define SYSFS_PATH_MAX 255

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "hwmonX/device" dir */
static size_t sysfs_read_file_hwmon(unsigned int hwmon, const char *fname,
				    char *buf, size_t buflen)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numread;

	if (sprintf_s(path, sizeof(path), PATH_TO_HWMON "hwmon%u/%s",
		      hwmon, fname) <= 0) {
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
	CURR1_INPUT,
	POWER1_INPUT,
	IN0_INPUT,
	IN1_INPUT,
	IN2_INPUT,
	PWM1,
	MAX_VALUE_FILES_MON
};

static const char *value_files[MAX_VALUE_FILES_MON] = {
	[CURR1_INPUT] = "curr1_input",
	[POWER1_INPUT] = "power1_input",
	[IN0_INPUT] = "in0_input",
	[IN1_INPUT] = "in1_input",
	[IN2_INPUT] = "in2_input",
	[PWM1] = "pwm1",
};

static unsigned long sysfs_get_one_value(unsigned int hwmon, unsigned int which)
{
	unsigned long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES_MON)
		return 0;

	len = sysfs_read_file_hwmon(hwmon, value_files[which], linebuf,
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
	NAME_MON,
	RAIL_NAME,
	SPEED_CLASS_BOOT,
	MAX_STRING_FILES_MON
};

static const char *string_files[MAX_STRING_FILES_MON] = {
#ifndef TARGET_X86
	[NAME_MON] = "device/name",
#else
	[NAME_MON] = "name",
#endif
	[RAIL_NAME] = "of_node/rail-name",
	[SPEED_CLASS_BOOT] = "device/speed_class_boot",
};

static char *sysfs_get_one_string(unsigned int hwmon, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_MON)
		return NULL;

	len = sysfs_read_file_hwmon(hwmon, string_files[which], linebuf,
				    sizeof(linebuf));
	if (len == 0)
		return NULL;

/* strdup() returnes  pointer to duplicated string, or NULL if insufficient
 *	memory was available
 */
	result = strdup(linebuf);
	if (!result)
		return NULL;

	if (result[strnlen_s(result, sizeof(linebuf)) - 1] == '\n')
		result[strnlen_s(result, sizeof(linebuf)) - 1] = '\0';

	return result;
}

int sysfs_hwmon_exists(unsigned int hwmon)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	statbuf.st_mode = 0;
	if (sprintf_s(file, SYSFS_PATH_MAX, PATH_TO_HWMON "hwmon%u/",
		      hwmon) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -EINVAL;

	return S_ISDIR(statbuf.st_mode) ? 0 : -EINVAL;
}

char *sysfs_get_hwmon_name(unsigned int hwmon)
{
	return sysfs_get_one_string(hwmon, NAME_MON);
}

void sysfs_put_hwmon_name(char *name)
{
	if (!name)
		return;

	free(name);
}

char *sysfs_get_hwmon_rail_name(unsigned int hwmon)
{
	return sysfs_get_one_string(hwmon, RAIL_NAME);
}

void sysfs_put_hwmon_rail_name(char *rail_name)
{
	if (!rail_name)
		return;
	free(rail_name);
}

char *sysfs_get_hwmon_speed_class_boot(unsigned int hwmon)
{
	return sysfs_get_one_string(hwmon, SPEED_CLASS_BOOT);
}

void sysfs_put_hwmon_speed_class_boot(char *speed_class_boot)
{
	if (!speed_class_boot)
		return;
	free(speed_class_boot);
}

unsigned long sysfs_get_hwmon_power(unsigned int hwmon)
{
	return sysfs_get_one_value(hwmon, POWER1_INPUT);
}

unsigned long sysfs_get_hwmon_rail_volt(unsigned int hwmon)
{
	return sysfs_get_one_value(hwmon, IN1_INPUT);
}

unsigned long sysfs_get_hwmon_pwm(unsigned int hwmon)
{
	return sysfs_get_one_value(hwmon, PWM1);
}

unsigned long sysfs_get_hwmon_pvt_cpu_m0(unsigned int hwmon)
{
	return sysfs_get_one_value(hwmon, IN0_INPUT);
}

unsigned long sysfs_get_hwmon_pvt_cpu_m1(unsigned int hwmon)
{
	return sysfs_get_one_value(hwmon, IN1_INPUT);
}

unsigned long sysfs_get_hwmon_pvt_roc(unsigned int hwmon)
{
	return sysfs_get_one_value(hwmon, IN2_INPUT);
}
