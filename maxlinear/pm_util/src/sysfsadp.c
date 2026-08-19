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

#define MAX_LINE_LEN 4096
#define SYSFS_PATH_MAX 255

enum {
	LGM_PATH_TO_ADP = 1,
	MAX_STRING_PATH_ADP
};

static char epu_path[50];
static char *string_path[MAX_STRING_PATH_ADP] = {
	[LGM_PATH_TO_ADP] = "/sys/devices/soc0/e0180000.epu",
};

static void set_epu_path(void)
{
	FILE *fp;

	system("find /sys/devices/ -name e0180000.epu  1> /tmp/epu_path.txt 2> /dev/null");
	fp = fopen("/tmp/epu_path.txt", "r");
	if (!fp)
		return;

	/* first, extract the system type line */
	fgets(epu_path, 50, fp);
	epu_path[strcspn(epu_path, "\r\n")] = '\0';
	fclose(fp);
	system("rm -fr /tmp/epu_path.txt");
	string_path[LGM_PATH_TO_ADP] = epu_path;
}

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "socX/e0180000.epu" dir */
static size_t sysfs_read_file_adp(unsigned int adp, const char *fname,
				  char *buf, size_t buflen)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numread;

	set_epu_path();
	if (sprintf_s(path, sizeof(path), "%s/%s",
		      string_path[adp], fname) <= 0) {
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
	ADP_DVFS_LEVEL,
	MAX_VALUE_FILES_ADP
};

static const char *value_files[MAX_VALUE_FILES_ADP] = {
	[ADP_DVFS_LEVEL] = "adp_dvfs_level",
};

static unsigned long sysfs_get_one_value(unsigned int adp, unsigned int which)
{
	unsigned long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES_ADP)
		return 0;

	len = sysfs_read_file_adp(adp, value_files[which], linebuf,
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
	NAME_ADP,
	ADP_SCALING,
	MAX_STRING_FILES_ADP
};

static const char *string_files[MAX_STRING_FILES_ADP] = {
	[NAME_ADP] = "of_node/name",
	[ADP_SCALING] = "adp_scaling",
};

static char *sysfs_get_one_string(unsigned int adp, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES_ADP)
		return NULL;

	len = sysfs_read_file_adp(adp, string_files[which], linebuf,
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

int sysfs_adp_exists(unsigned int adp)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	set_epu_path();
	statbuf.st_mode = 0;
	if (sprintf_s(file, SYSFS_PATH_MAX, "%s", string_path[adp]) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -EINVAL;

	return S_ISDIR(statbuf.st_mode) ? 0 : -EINVAL;
}

char *sysfs_get_adp_name(unsigned int adp)
{
	return sysfs_get_one_string(adp, NAME_ADP);
}

void sysfs_put_adp_name(char *name)
{
	if (!name)
		return;

	free(name);
}

char *sysfs_get_adp_scaling(unsigned int adp)
{
	return sysfs_get_one_string(adp, ADP_SCALING);
}

void sysfs_put_adp_scaling(char *adpscaling)
{
	if (!adpscaling)
		return;

	free(adpscaling);
}

int sysfs_get_adp_dvfs_level(unsigned int adp)
{
	return sysfs_get_one_value(adp, ADP_DVFS_LEVEL);
}

