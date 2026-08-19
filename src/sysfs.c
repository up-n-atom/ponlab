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
#include <safe_mem_lib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifndef TARGET_X86
#endif
#include "include/ltq_pmutil_interface.h"

#define PATH_TO_CPU "/sys/devices/system/cpu/"
#define MAX_LINE_LEN 4096
#define SYSFS_PATH_MAX 255

enum {
	CPUINFO_CUR_FREQ,
	CPUINFO_MIN_FREQ,
	CPUINFO_MAX_FREQ,
	CPUINFO_LATENCY,
	SCALING_CUR_FREQ,
	SCALING_MIN_FREQ,
	SCALING_MAX_FREQ,
	STATS_NUM_TRANSITIONS,
	DUMMY0,
	DUMMY1,
	MAX_VALUE_CPUFREQ_FILES = 10,
	MAX_PERF_PCT,
	MIN_PERF_PCT,
	NO_TURBO,
	NUM_PSTATES,
	TURBO_PCT,
	MAX_VALUE_FILES
};

static const char *value_files[MAX_VALUE_FILES] = {
	[CPUINFO_CUR_FREQ] = "cpuinfo_cur_freq",
	[CPUINFO_MIN_FREQ] = "cpuinfo_min_freq",
	[CPUINFO_MAX_FREQ] = "cpuinfo_max_freq",
	[CPUINFO_LATENCY]  = "cpuinfo_transition_latency",
	[SCALING_CUR_FREQ] = "scaling_cur_freq",
	[SCALING_MIN_FREQ] = "scaling_min_freq",
	[SCALING_MAX_FREQ] = "scaling_max_freq",
	[STATS_NUM_TRANSITIONS] = "stats/total_trans",
	[MAX_VALUE_CPUFREQ_FILES] = "",
	[MAX_PERF_PCT] = "max_perf_pct",
	[MIN_PERF_PCT] = "min_perf_pct",
	[NO_TURBO] = "no_turbo",
	[NUM_PSTATES] = "num_pstates",
	[TURBO_PCT] = "turbo_pct"
};

/* helper function to read file from /sys into given buffer */
/* fname is a relative path under "cpuX/cpufreq" dir */
static size_t sysfs_read_file(unsigned int cpu, const char *fname,
			      unsigned int which, char *buf,
			      size_t buflen)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numread;

	if (which < MAX_VALUE_CPUFREQ_FILES) {
		if (sprintf_s(path, sizeof(path),
			      PATH_TO_CPU "cpu%u/cpufreq/%s",
			      cpu, fname) <= 0) {
			printf("sprintf_s failed\n");
			return 0;
		}
	} else {
		if (sprintf_s(path, sizeof(path), PATH_TO_CPU "intel_pstate/%s",
			      fname) <= 0) {
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

/* helper function to write a new value to a /sys file */
/* fname is a relative path under "cpuX/cpufreq" dir */
static size_t sysfs_write_file(unsigned int cpu, const char *fname,
			       const char *value, size_t len)
{
	char path[SYSFS_PATH_MAX];
	int fd;
	ssize_t numwrite;

	if (sprintf_s(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s",
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

/* read access to files which contain one numeric value */

static unsigned long sysfs_get_one_value(unsigned int cpu, unsigned int which)
{
	unsigned long value;
	unsigned int len;
	char linebuf[MAX_LINE_LEN];
	char *endp;

	if (which >= MAX_VALUE_FILES)
		return 0;

	len = sysfs_read_file(cpu, value_files[which], which, linebuf,
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
	SCALING_DRIVER,
	SCALING_GOVERNOR,
	LTQ_MODULE_STATUS,
	LTQ_STATE_CHANGE_CONTROL,
	LTQ_DVS_SUPPORT,
	LTQ_FORCE_DS,
	LTQ_ALERT,
	LTQ_SCALING_CLKS,
	DUMMY_0,
	DUMMY_1,
	MAX_STRING_CPUFREQ_FILES = 10,
	PST_STATUS,
	MAX_STRING_FILES
};

static const char *string_files[MAX_STRING_FILES] = {
	[SCALING_DRIVER] = "scaling_driver",
	[SCALING_GOVERNOR] = "scaling_governor",
	[LTQ_MODULE_STATUS] = "ltq_stats/ltq_moduleinfo",
	[LTQ_STATE_CHANGE_CONTROL] = "ltq_stats/ltq_control",
	[LTQ_DVS_SUPPORT] = "ltq_stats/ltq_dvs",
	[LTQ_FORCE_DS] = "ltq_stats/ltq_force_ds",
	[LTQ_ALERT] = "ltq_stats/ltq_alert",
	[MAX_STRING_CPUFREQ_FILES] = "",
	[PST_STATUS] = "status",
};

static char *sysfs_get_one_string(unsigned int cpu, unsigned int which)
{
	char linebuf[MAX_LINE_LEN];
	char *result;
	unsigned int len;

	if (which >= MAX_STRING_FILES)
		return NULL;

	len = sysfs_read_file(cpu, string_files[which], which,
			      linebuf, sizeof(linebuf));
	if (len == 0)
		return NULL;

	result = strdup(linebuf);
	if (!result)
		return NULL;

	if (result[strnlen_s(result, MAX_LINE_LEN) - 1] == '\n')
		result[strnlen_s(result, MAX_LINE_LEN) - 1] = '\0';

	return result;
}

/* write access */

enum {
	WRITE_SCALING_MIN_FREQ,
	WRITE_SCALING_MAX_FREQ,
	WRITE_SCALING_GOVERNOR,
	WRITE_SCALING_SET_SPEED,
	WRITE_LTQ_MODULE_STATUS,
	WRITE_LTQ_STATE_CHANGE_CONTROL,
	WRITE_LTQ_FORCE_DS,
	WRITE_LTQ_ALERT,
	MAX_WRITE_FILES
};

static const char *write_files[MAX_WRITE_FILES] = {
	[WRITE_SCALING_MIN_FREQ] = "scaling_min_freq",
	[WRITE_SCALING_MAX_FREQ] = "scaling_max_freq",
	[WRITE_SCALING_GOVERNOR] = "scaling_governor",
	[WRITE_SCALING_SET_SPEED] = "scaling_setspeed",
	[WRITE_LTQ_MODULE_STATUS] = "ltq_stats/ltq_moduleinfo",
	[WRITE_LTQ_STATE_CHANGE_CONTROL] = "ltq_stats/ltq_control",
	[WRITE_LTQ_FORCE_DS] = "ltq_stats/ltq_force_ds",
	[WRITE_LTQ_ALERT] = "ltq_stats/ltq_alert",
};

static int sysfs_write_one_value(unsigned int cpu, unsigned int which,
				 const char *new_value, size_t len)
{
	if (which >= MAX_WRITE_FILES)
		return 0;

	if (sysfs_write_file(cpu, write_files[which], new_value, len) != len)
		return -ENODEV;

	return 0;
}

unsigned long sysfs_get_max_freq(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, SCALING_MAX_FREQ);
}

int sysfs_set_max_freq(unsigned int cpu, unsigned long max_freq)
{
	char freq[SYSFS_PATH_MAX];
	size_t len;

	if (sprintf_s(freq, SYSFS_PATH_MAX, "%lu", max_freq) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	len = strnlen_s(freq, sizeof(freq));
	/* Sanity check due to klocwork error */
	if (len > SYSFS_PATH_MAX)
		return -EINVAL;

	return sysfs_write_one_value(cpu, WRITE_SCALING_MAX_FREQ, freq, len);
}

unsigned long sysfs_get_freq_hardware(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, CPUINFO_CUR_FREQ);
}

unsigned long sysfs_get_freq_scaling(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, SCALING_CUR_FREQ);
}

int sysfs_get_hardware_limits(unsigned int cpu,
			      unsigned long *min,
			      unsigned long *max)
{
	if (!min || !max)
		return -EINVAL;

	*min = sysfs_get_one_value(cpu, CPUINFO_MIN_FREQ);
	if (!*min)
		return -ENODEV;

	*max = sysfs_get_one_value(cpu, CPUINFO_MAX_FREQ);
	if (!*max)
		return -ENODEV;

	return 0;
}

struct cpufreq_policy *sysfs_get_policy(unsigned int cpu)
{
	struct cpufreq_policy *policy;

	policy = malloc(sizeof(struct cpufreq_policy));
	if (!policy)
		return NULL;

	policy->governor = sysfs_get_one_string(cpu, SCALING_GOVERNOR);
	if (!policy->governor) {
		free(policy);
		return NULL;
	}
	policy->min = sysfs_get_one_value(cpu, SCALING_MIN_FREQ);
	policy->max = sysfs_get_one_value(cpu, SCALING_MAX_FREQ);
	if (!policy->min || !policy->max) {
		free(policy->governor);
		free(policy);
		return NULL;
	}

	return policy;
}

struct cpufreq_available_governors *sysfs_get_available_governors(unsigned int cpu)
{
	struct cpufreq_available_governors *first = NULL;
	struct cpufreq_available_governors *current = NULL;
	char one_value[SYSFS_PATH_MAX];
	char linebuf[MAX_LINE_LEN];
	unsigned int pos, i;
	unsigned int len;

	len = sysfs_read_file(cpu, "scaling_available_governors", 0,
			      linebuf, sizeof(linebuf));
	if (len == 0)
		return NULL;

	pos = 0;
	for (i = 0; i < len; i++) {
		if (linebuf[i] == ' ' || linebuf[i] == '\n') {
			if (i - pos < 2)
				continue;
			if (i - pos >= SYSFS_PATH_MAX)
				goto error_out;
			if (current) {
				current->next = malloc(sizeof(*current));
				if (!current->next)
					goto error_out;
				current = current->next;
			} else {
				first = malloc(sizeof(*first));
				if (!first)
					goto error_out;
				current = first;
			}
			current->first = first;
			current->next = NULL;

			memcpy_s(one_value, sizeof(one_value), linebuf + pos,
				 i - pos);
			one_value[i - pos] = '\0';
			strncpy_s(current->governor, GOV_MAX, one_value,
				  GOV_MAX - 1);
			pos = i + 1;
		}
	}

	return first;

 error_out:
	while (first) {
		current = first->next;
		free(first);
		first = current;
	}
	return NULL;
}

struct cpufreq_available_frequencies *sysfs_get_available_frequencies(unsigned int cpu)
{
	struct cpufreq_available_frequencies *first = NULL;
	struct cpufreq_available_frequencies *cur = NULL;
	char one_value[SYSFS_PATH_MAX];
	char linebuf[MAX_LINE_LEN];
	unsigned int pos, i;
	unsigned int len;

	len = sysfs_read_file(cpu, "scaling_available_frequencies", 0,
			      linebuf, sizeof(linebuf));
	if (len == 0)
		return NULL;

	pos = 0;
	for (i = 0; i < len; i++) {
		if (linebuf[i] == ' ' || linebuf[i] == '\n') {
			if (i - pos < 2)
				continue;
			if (i - pos >= SYSFS_PATH_MAX)
				goto error_out;
			if (cur) {
				cur->next = malloc(sizeof(*cur));
				if (!cur->next)
					goto error_out;
				cur = cur->next;
			} else {
				first = malloc(sizeof(*first));
				if (!first)
					goto error_out;
				cur = first;
			}
			cur->first = first;
			cur->next = NULL;

			memcpy_s(one_value, sizeof(one_value), linebuf + pos,
				 i - pos);
			one_value[i - pos] = '\0';
			if (sscanf_s(one_value, "%12lu", &cur->frequency) != 1)
				goto error_out;

			pos = i + 1;
		}
	}

	return first;

 error_out:
	while (first) {
		cur = first->next;
		free(first);
		first = cur;
	}
	return NULL;
}

struct cpufreq_stats *sysfs_get_stats(unsigned int cpu,
				      unsigned long long *total_time)
{
	struct cpufreq_stats *first = NULL;
	struct cpufreq_stats *cur = NULL;
	char one_value[SYSFS_PATH_MAX];
	char linebuf[MAX_LINE_LEN];
	unsigned int pos, i;
	unsigned int len;

	len = sysfs_read_file(cpu, "stats/time_in_state", 0, linebuf,
			      sizeof(linebuf));
	if (len == 0)
		return NULL;

	*total_time = 0;
	pos = 0;
	for (i = 0; i < len; i++) {
		if (i == strnlen_s(linebuf, MAX_LINE_LEN) ||
		    linebuf[i] == '\n') {
			if (i - pos < 2)
				continue;
			if ((i - pos) >= SYSFS_PATH_MAX)
				goto error_out;
			if (cur) {
				cur->next = malloc(sizeof(*cur));
				if (!cur->next)
					goto error_out;
				cur = cur->next;
			} else {
				first = malloc(sizeof(*first));
				if (!first)
					goto error_out;
				cur = first;
			}
			cur->first = first;
			cur->next = NULL;

			memcpy_s(one_value, sizeof(one_value), linebuf + pos,
				 i - pos);
			one_value[i - pos] = '\0';
			if (sscanf_s(one_value, "%12lu %12llu", &cur->frequency,
				     &cur->time_in_state) != 2)
				goto error_out;

			*total_time = *total_time + cur->time_in_state;
			pos = i + 1;
		}
	}

	return first;

error_out:
	while (first) {
		cur = first->next;
		free(first);
		first = cur;
	}
	return NULL;
}

unsigned long sysfs_get_transitions(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, STATS_NUM_TRANSITIONS);
}

static int verify_gov(char *new_gov, char *passed_gov)
{
	unsigned int i, j;
	size_t len_passed;

	if (!passed_gov)
		return -EINVAL;

	len_passed = strnlen_s(passed_gov, GOV_MAX);
	if (len_passed > GOV_MAX - 1)
		return -EINVAL;

	j = 0;
	strncpy_s(new_gov, GOV_MAX, passed_gov, len_passed + 1);
	for (i = 0; i < len_passed; i++) {
		if (j) {
			new_gov[i] = '\0';
			continue;
		}
		if ((new_gov[i] >= 'a') && (new_gov[i] <= 'z'))
			continue;
		if ((new_gov[i] >= 'A') && (new_gov[i] <= 'Z'))
			continue;
		if (new_gov[i] == '-')
			continue;
		if (new_gov[i] == '_')
			continue;
		if (new_gov[i] == '\0') {
			j = 1;
			continue;
		}
		return -EINVAL;
	}
	new_gov[19] = '\0';
	return 0;
}

int sysfs_modify_policy_governor(unsigned int cpu, char *governor)
{
	char new_gov[GOV_MAX];
	size_t len;

	if (!governor)
		return -EINVAL;

	if (verify_gov(new_gov, governor))
		return -EINVAL;

	len = strnlen_s(new_gov, sizeof(new_gov));
	/* Sanity check due to klocwork error */
	if (len > GOV_MAX)
		return -EINVAL;

	return sysfs_write_one_value(cpu, WRITE_SCALING_GOVERNOR, new_gov, len);
}

int sysfs_set_frequency(unsigned int cpu, unsigned long target_frequency)
{
	struct cpufreq_policy *pol = sysfs_get_policy(cpu);
	char userspace_gov[] = "userspace";
	char freq[SYSFS_PATH_MAX];
	size_t len;

	if (!pol)
		return -ENODEV;

	if (strncmp(pol->governor, userspace_gov, 9) != 0) {
		int ret;

		ret = sysfs_modify_policy_governor(cpu, userspace_gov);
		if (ret) {
			cpufreq_put_policy(pol);
			return ret;
		}
	}

	cpufreq_put_policy(pol);

	if (sprintf_s(freq, SYSFS_PATH_MAX, "%lu", target_frequency) <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	len = strnlen_s(freq, sizeof(freq));
	/* Sanity check due to klocwork error */
	if (len > SYSFS_PATH_MAX)
		return -EINVAL;

	return sysfs_write_one_value(cpu, WRITE_SCALING_SET_SPEED, freq, len);
}

/* Lantiq functions */
char *sysfs_get_ltq_module_status(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, LTQ_MODULE_STATUS);
}

int sysfs_set_ltq_alert(unsigned int cpu, char *buf)
{
	if (!buf)
		return -ENODEV;
	return sysfs_write_one_value(cpu, WRITE_LTQ_ALERT, buf,
				     strnlen_s(buf, 4));
}

int sysfs_set_ltq_force_ds(unsigned int cpu, char *buf)
{
	if (!buf)
		return -ENODEV;
	return sysfs_write_one_value(cpu, WRITE_LTQ_FORCE_DS, buf,
				     strnlen_s(buf, 4));
}

char *sysfs_get_ltq_scaling_clks(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, LTQ_SCALING_CLKS);
}

char *sysfs_get_scaling_driver(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, SCALING_DRIVER);
}

char *sysfs_get_ltq_alert(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, LTQ_ALERT);
}

char *sysfs_get_ltq_force_ds(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, LTQ_FORCE_DS);
}

char *sysfs_get_ltq_state_change_control(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, LTQ_STATE_CHANGE_CONTROL);
}

char *sysfs_get_ltq_dvs_support(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, LTQ_DVS_SUPPORT);
}

unsigned long sysfs_get_pstate_maxperf(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, MAX_PERF_PCT);
}

unsigned long sysfs_get_pstate_minperf(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, MIN_PERF_PCT);
}

unsigned long sysfs_get_pstate_noturbo(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, NO_TURBO);
}

unsigned long sysfs_get_pstate_numpstates(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, NUM_PSTATES);
}

unsigned long sysfs_get_pstate_turbo(unsigned int cpu)
{
	return sysfs_get_one_value(cpu, TURBO_PCT);
}

char *sysfs_get_pstate_status(unsigned int cpu)
{
	return sysfs_get_one_string(cpu, PST_STATUS);
}

int sysfs_pstate_exists(void)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	statbuf.st_mode = 0;
	if (sprintf_s(file, SYSFS_PATH_MAX,
		      "/sys/devices/system/cpu/intel_pstate/") <= 0) {
		printf("sprintf_s failed\n");
		return -EINVAL;
	}

	if (stat(file, &statbuf) != 0)
		return -EINVAL;

	return S_ISDIR(statbuf.st_mode) ? 0 : -EINVAL;
}
