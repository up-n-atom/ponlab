/*
 * Copyright 2020 MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2009 - 2014 Lantiq Deutschland GmbH
 * Copyright 2007 - 2008 Infineon Technologies AG
 * (C) 2004-2009  Dominik Brodowski <linux@dominikbrodowski.de>
 *
 * Licensed under the terms of the GNU GPL License version 2.
 */

#include <stdio.h>
#include <errno.h>
#include <safe_str_lib.h>
#include <stdlib.h>
#include <string.h>
#include "include/ltq_pmutil_interface.h"

unsigned long cpufreq_get_max_freq(unsigned int cpu)
{
	return sysfs_get_max_freq(cpu);
}

int cpufreq_set_max_freq(unsigned int cpu, unsigned long max_freq)
{
	return sysfs_set_max_freq(cpu, max_freq);
}

unsigned long cpufreq_get_freq_hardware(unsigned int cpu)
{
	return sysfs_get_freq_hardware(cpu);
}

unsigned long cpufreq_get_freq_scaling(unsigned int cpu)
{
	return sysfs_get_freq_scaling(cpu);
}

int cpufreq_get_hardware_limits(unsigned int cpu,
				unsigned long *min,
				unsigned long *max)
{
	if ((!min) || (!max))
		return -EINVAL;
	return sysfs_get_hardware_limits(cpu, min, max);
}

struct cpufreq_policy *cpufreq_get_policy(unsigned int cpu)
{
	return sysfs_get_policy(cpu);
}

void cpufreq_put_policy(struct cpufreq_policy *policy)
{
	if ((!policy) || (!policy->governor))
		return;

	free(policy->governor);
	policy->governor = NULL;
	free(policy);
}

struct cpufreq_available_governors *cpufreq_get_available_governors(
							unsigned int cpu)
{
	return sysfs_get_available_governors(cpu);
}

void cpufreq_put_available_governors(struct cpufreq_available_governors *any)
{
	struct cpufreq_available_governors *tmp, *next;

	if (!any)
		return;

	tmp = any->first;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

struct cpufreq_available_frequencies *cpufreq_get_available_frequencies(
							unsigned int cpu)
{
	return sysfs_get_available_frequencies(cpu);
}

void cpufreq_put_available_frequencies(
				struct cpufreq_available_frequencies *any)
{
	struct cpufreq_available_frequencies *tmp, *next;

	if (!any)
		return;

	tmp = any->first;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

int cpufreq_modify_policy_governor(unsigned int cpu, char *governor)
{
	if ((!governor) || (strnlen_s(governor, GOV_MAX) > 19))
		return -EINVAL;

	return sysfs_modify_policy_governor(cpu, governor);
}

int cpufreq_set_frequency(unsigned int cpu, unsigned long target_frequency)
{
	return sysfs_set_frequency(cpu, target_frequency);
}

struct cpufreq_stats *cpufreq_get_stats(unsigned int cpu,
					unsigned long long *total_time)
{
	struct cpufreq_stats *ret;

	ret = sysfs_get_stats(cpu, total_time);
	return ret;
}

void cpufreq_put_stats(struct cpufreq_stats *any)
{
	struct cpufreq_stats *tmp, *next;

	if (!any)
		return;

	tmp = any->first;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

unsigned long cpufreq_get_transitions(unsigned int cpu)
{
	unsigned long ret = sysfs_get_transitions(cpu);

	return ret;
}

/* Lantiq functions */
char *cpufreq_get_ltq_module_status(unsigned int cpu)
{
	return sysfs_get_ltq_module_status(cpu);
}

void cpufreq_put_ltq_module_status(char *module_status)
{
	if (!module_status)
		return;

	free(module_status);
}

int cpufreq_set_ltq_alert(unsigned int cpu, char *buf)
{
	return sysfs_set_ltq_alert(cpu, buf);
}

char *cpufreq_get_ltq_alert(unsigned int cpu)
{
	return sysfs_get_ltq_alert(cpu);
}

void cpufreq_put_ltq_alert(char *alert)
{
	if (!alert)
		return;

	free(alert);
}

int cpufreq_set_ltq_force_ds(unsigned int cpu, char *buf)
{
	return sysfs_set_ltq_force_ds(cpu, buf);
}

char *cpufreq_get_ltq_force_ds(unsigned int cpu)
{
	return sysfs_get_ltq_force_ds(cpu);
}

void cpufreq_put_ltq_force_ds(char *force)
{
	if (!force)
		return;

	free(force);
}

char *cpufreq_get_ltq_state_change_control(unsigned int cpu)
{
	return sysfs_get_ltq_state_change_control(cpu);
}

void cpufreq_put_ltq_state_change_control(char *control)
{
	if (!control)
		return;

	free(control);
}

char *cpufreq_get_ltq_dvs_support(unsigned int cpu)
{
	return sysfs_get_ltq_dvs_support(cpu);
}

void cpufreq_put_ltq_dvs_support(char *dvs)
{
	if (!dvs)
		return;

	free(dvs);
}

char *cpufreq_get_ltq_scaling_clks(unsigned int cpu)
{
	return sysfs_get_ltq_scaling_clks(cpu);
}

void cpufreq_put_ltq_scaling_clks(char *scaling)
{
	if (!scaling)
		return;

	free(scaling);
}

char *cpufreq_get_scaling_driver(unsigned int cpu)
{
	return sysfs_get_scaling_driver(cpu);
}

void cpufreq_put_scaling_driver(char *scaling_drv)
{
	if (!scaling_drv)
		return;

	free(scaling_drv);
}

char *cpufreq_get_pstate_status(unsigned int cpu)
{
	return sysfs_get_pstate_status(cpu);
}

void cpufreq_put_pstate_status(char *status)
{
	if (!status)
		return;

	free(status);
}
