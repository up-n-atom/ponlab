
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <safe_str_lib.h>

#define MAX_LINE_LEN 4096
#define SYSFS_PATH_MAX 255

enum {
	STATE_C1 = 1,
	STATE_C6NS = 2,
	STATE_C6FS = 3,
	STATE_C7 = 4
};

enum {
	PATH_TO_CPUX_CPUIDLE = 0,
	PATH_TO_CPUIDLE      = 4,
	PATH_TO_UPTIME       = 6,
	MAX_STRING_PATH_CPUIDLE
};

static const char *string_path[MAX_STRING_PATH_CPUIDLE] = {
	[PATH_TO_CPUIDLE]      = "/sys/devices/system/cpu",
	[PATH_TO_UPTIME]       = "/proc",
};

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "cpuidle" dir */
static size_t sysfs_read_file_cpuidle(unsigned int cpuidle, const char *fname,
				      char *buf, size_t buflen)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numread;

	switch (cpuidle) {
	case PATH_TO_CPUIDLE:
		if (sprintf_s(path, sizeof(path), "%s/cpuidle/%s",
			      string_path[PATH_TO_CPUIDLE], fname) <= 0) {
			printf("sprintf_s failed\n");
			return 0;
		}
		break;
	case PATH_TO_UPTIME:
		if (sprintf_s(path, sizeof(path), "%s/%s",
			      string_path[PATH_TO_UPTIME], fname) <= 0) {
			printf("sprintf_s failed\n");
			return 0;
		}
		break;
	default: /* 0~3 cpu0 cpu1 cpu2 cpu3 */
		if (sprintf_s(path, sizeof(path), "%s/cpu%u/cpuidle/%s",
			      string_path[PATH_TO_CPUIDLE],
			      cpuidle, fname) <= 0) {
			printf("sprintf_s failed\n");
			return 0;
		}
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
	CPUIDLE_C0_TIME = 0,
	CPUIDLE_C1_TIME,
	CPUIDLE_C6NS_TIME,
	CPUIDLE_C6FS_TIME,
	CPUIDLE_C7_TIME,
	CPUIDLE_UPIME,
	CPUIDLE_C1_DISABLE,
	CPUIDLE_C6NS_DISABLE,
	CPUIDLE_C6FS_DISABLE,
	CPUIDLE_C7_DISABLE,
	MAX_VALUE_FILES_CPUIDLE
};

static const char *value_files[MAX_VALUE_FILES_CPUIDLE] = {
	[CPUIDLE_C0_TIME]   = "state0/time",
	[CPUIDLE_C1_TIME]   = "state1/time",
	[CPUIDLE_C6NS_TIME] = "state2/time",
	[CPUIDLE_C6FS_TIME] = "state3/time",
	[CPUIDLE_C7_TIME]   = "state4/time",
	[CPUIDLE_C1_DISABLE]   = "state1/disable",
	[CPUIDLE_C6NS_DISABLE] = "state2/disable",
	[CPUIDLE_C6FS_DISABLE] = "state3/disable",
	[CPUIDLE_C7_DISABLE]   = "state4/disable",
	[CPUIDLE_UPIME]   = "uptime",
};

/* helper function to write a new value to a /sys file */
/* fname is the path to dir */
static size_t sysfs_write_file(unsigned int cpu, const char *fname,
			       const char *value, size_t len)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numwrite;

	if (sprintf_s(path, sizeof(path), "/sys/devices/system/cpu/cpu%u/cpuidle/%s",
		      cpu, fname) <= 0) {
		printf("sprintf_s failed\n");
		return 0;
	}

	fd = open(path, O_WRONLY);
	if (fd == -1)
		return 0;

	numwrite = write(fd, value, len);
	if (numwrite < 1) {
		(void)close(fd);
		return 0;
	}

	(void)close(fd);

	return (size_t)numwrite;
}

static int sysfs_get_one_value(unsigned int cpuidle,
			       unsigned int which, unsigned long *val)
{
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES_CPUIDLE)
		return -EINVAL;

	len = sysfs_read_file_cpuidle(cpuidle, value_files[which], linebuf,
				      sizeof(linebuf));
	if (len == 0)
		return -EINVAL;

	*val = strtoul(linebuf, &endp, 0);

	if (endp == linebuf || errno == ERANGE)
		return -EINVAL;

	return 0;
}

static int sysfs_write_one_value(unsigned int cpu, unsigned int which,
				 const char *new_value, size_t len)
{
	if (which >= MAX_VALUE_FILES_CPUIDLE)
		return 0;

	if (sysfs_write_file(cpu, value_files[which], new_value, len) != len)
		return -ENODEV;

	return 0;
}

/* read access to files which contain one string */

enum {
	CPUIDLE_DRIVER,
	CPUIDLE_GOVERNOR,
	MAX_STRING_FILES_CPUIDLE
};

static const char *string_files[MAX_STRING_FILES_CPUIDLE] = {
	[CPUIDLE_DRIVER]   = "current_driver",
	[CPUIDLE_GOVERNOR] = "current_governor_ro",

};

static char *sysfs_get_one_string(unsigned int cpuidle, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_CPUIDLE)
		return NULL;

	len = sysfs_read_file_cpuidle(cpuidle, string_files[which], linebuf,
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

int sysfs_cpuidle_exists(void)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	statbuf.st_mode = 0;
	if (sprintf_s(file, SYSFS_PATH_MAX, "%s/cpuidle",
		      string_path[PATH_TO_CPUIDLE]) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -ENOENT;

	return S_ISDIR(statbuf.st_mode) ? 0 : -ENOENT;
}

char *sysfs_get_cpuidle_driver(void)
{
	return sysfs_get_one_string(PATH_TO_CPUIDLE, CPUIDLE_DRIVER);
}

void sysfs_put_cpuidle_driver(char *name)
{
	if (!name)
		return;

	free(name);
}

char *sysfs_get_cpuidle_governor(void)
{
	return sysfs_get_one_string(PATH_TO_CPUIDLE, CPUIDLE_GOVERNOR);
}

void sysfs_put_cpuidle_governor(char *name)
{
	if (!name)
		return;

	free(name);
}

int sysfs_cpu_cpuidle_exists(unsigned int cpuidle)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	statbuf.st_mode = 0;
	if (sprintf_s(file, SYSFS_PATH_MAX, "%s/cpu%u/cpuidle",
		      string_path[PATH_TO_CPUIDLE], cpuidle) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -ENOENT;

	return S_ISDIR(statbuf.st_mode) ? 0 : -ENOENT;
}

char *sysfs_get_cstate_disable(unsigned int cid)
{
	unsigned long c7 = 0xA;
	unsigned long c6ns = 0xA;
	unsigned long c6fs = 0xA;
	unsigned long c1 = 0xA;
	char linebuf[MAX_LINE_LEN] = {'\0'};
	char *result =  NULL;
	static const char * const sta[] = {"ena", "dis", "off"};

	if (sysfs_get_one_value(cid, CPUIDLE_C1_DISABLE, &c1) < 0)
		c1 = 2;
	if (sysfs_get_one_value(cid, CPUIDLE_C6NS_DISABLE, &c6ns) < 0)
		c6ns = 2;
	if (sysfs_get_one_value(cid, CPUIDLE_C6FS_DISABLE, &c6fs) < 0)
		c6fs = 2;
	if (sysfs_get_one_value(cid, CPUIDLE_C7_DISABLE, &c7) < 0)
		c7 = 2;
	if (( c1 >=0 && c1 < 3) && ( c6ns >=0 && c6ns < 3) && ( c6fs >=0 && c6fs < 3)) {
		sprintf_s(linebuf, sizeof(linebuf),
			  "C1: %s C6NS: %s C6FS: %s\n",
			   c1  ? sta[c1] : sta[c1],
			   c6ns  ? sta[c6ns] : sta[c6ns],
			   c6fs  ? sta[c6fs] : sta[c6fs]);
		result = strdup(linebuf);
	}
	if (!result)
		return NULL;
	if (result[strnlen_s(result, sizeof(linebuf)) - 1] == '\n')
		result[strnlen_s(result, sizeof(linebuf)) - 1] = '\0';

	return result;
}

char *sysfs_get_cstate_state(unsigned int cid)
{
	unsigned long pc7 = 0;
	unsigned long pc6ns = 0;
	unsigned long pc6fs = 0;
	unsigned long pc1 = 0;
	unsigned long pc0 = 0;
	unsigned long uptime_usec;
	unsigned long c1_usec;
	unsigned long c6ns_usec;
	unsigned long c6fs_usec;
	unsigned long c7_usec;
	char linebuf[MAX_LINE_LEN];
	char *result;

	if (sysfs_get_one_value(PATH_TO_UPTIME, CPUIDLE_UPIME, &uptime_usec) < 0)
		return NULL;
	if (sysfs_get_one_value(cid, CPUIDLE_C1_TIME, &c1_usec) < 0)
		c1_usec = 0;
	if (sysfs_get_one_value(cid, CPUIDLE_C6NS_TIME, &c6ns_usec) < 0)
		c6ns_usec = 0;
	if (sysfs_get_one_value(cid, CPUIDLE_C6FS_TIME, &c6fs_usec) < 0)
		c6fs_usec = 0;
	if (sysfs_get_one_value(cid, CPUIDLE_C7_TIME, &c7_usec) < 0)
		c7_usec = 0;

	uptime_usec *= 1000000;
	if (uptime_usec) {
		pc7   = (c7_usec * 100) / uptime_usec;
		pc6ns = (c6ns_usec * 100) / uptime_usec;
		pc6fs = (c6fs_usec * 100) / uptime_usec;
		pc1   = (c1_usec * 100) / uptime_usec;
		pc0   = 100 - pc7 - pc6ns - pc6fs - pc1;
	}

	sprintf_s(linebuf, sizeof(linebuf),
		  "C0: %2ld%% C1: %2ld%% C6NS: %2ld%% C6FS: %2ld%%\n",
		  pc0, pc1, pc6ns, pc6fs);

	result = strdup(linebuf);
	if (!result)
		return NULL;

	if (result[strnlen_s(result, sizeof(linebuf)) - 1] == '\n')
		result[strnlen_s(result, sizeof(linebuf)) - 1] = '\0';

	return result;
}

void sysfs_put_cstate(char *name)
{
	if (!name)
		return;

	free(name);
}

void sysfs_disable_state(unsigned int cpu, unsigned int state, unsigned int dis)
{
	char disable[SYSFS_PATH_MAX];
	size_t len;

	if (sprintf_s(disable, SYSFS_PATH_MAX, "%lu", dis) <= 0) {
		printf("sprintf_s failed\n");
		return;
	}

	len = strnlen_s(disable, sizeof(disable));
	/* Sanity check due to klocwork error */
	if (len > SYSFS_PATH_MAX)
		return;

	switch (state) {
	case STATE_C1:
		sysfs_write_one_value(cpu, CPUIDLE_C1_DISABLE, disable, len);
		break;
	case STATE_C6NS:
		sysfs_write_one_value(cpu, CPUIDLE_C6NS_DISABLE, disable, len);
		break;
	case STATE_C6FS:
		sysfs_write_one_value(cpu, CPUIDLE_C6FS_DISABLE, disable, len);
		break;
	case STATE_C7:
		sysfs_write_one_value(cpu, CPUIDLE_C7_DISABLE, disable, len);
		break;
	default:
		break;
	}
}

