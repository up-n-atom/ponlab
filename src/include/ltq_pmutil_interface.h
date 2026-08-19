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

#ifndef _ltq_pmutil_interface_h
#define _ltq_pmutil_interface_h

#define LOG2FILE		0x0
#define LOG2SOCKET		0x1

enum soc_id {
	PM_SOC_UNKNOWN,
	PM_SOC_GRX350,
	PM_SOC_GRX550,
	PM_SOC_PRX321,
	PM_SOC_PRX126,
	PM_SOC_LGM,
};

/* logger interface
 * this function will log temp and power into a log file under /tmp
 */
extern int pm_logger(unsigned int r_period, char *log_path,
		     unsigned int log_mask, unsigned int flag,
		     uint16_t socket_port);
extern char *get_time(char *main);

/* CPUFREQ help interface
 * API to access /sys/devices/system/cpu/cpuX/cpufreq .
 */
extern unsigned long sysfs_get_max_freq(unsigned int cpu);
extern int sysfs_set_max_freq(unsigned int cpu, unsigned long max_freq);
extern unsigned long sysfs_get_freq_hardware(unsigned int cpu);
extern unsigned long sysfs_get_freq_scaling(unsigned int cpu);
extern int sysfs_get_hardware_limits(unsigned int cpu, unsigned long *min,
							unsigned long *max);
extern struct cpufreq_policy *sysfs_get_policy(unsigned int cpu);
extern struct cpufreq_available_governors *sysfs_get_available_governors(
							unsigned int cpu);
extern struct cpufreq_available_frequencies *sysfs_get_available_frequencies(
							unsigned int cpu);
extern struct cpufreq_stats *sysfs_get_stats(unsigned int cpu,
					unsigned long long *total_time);
extern unsigned long sysfs_get_transitions(unsigned int cpu);
extern int sysfs_modify_policy_governor(unsigned int cpu, char *governor);
extern int sysfs_set_frequency(unsigned int cpu,
					unsigned long target_frequency);
extern char *sysfs_get_ltq_module_status(unsigned int cpu);
extern int sysfs_set_ltq_state_change_control(unsigned int cpu, char *buf);

extern char *sysfs_get_ltq_state_change_control(unsigned int cpu);
extern char *sysfs_get_ltq_scaling_clks(unsigned int cpu);
extern char *sysfs_get_scaling_driver(unsigned int cpu);
extern char *sysfs_get_ltq_dvs_support(unsigned int cpu);

extern char *sysfs_get_ltq_force_ds(unsigned int cpu);
extern int sysfs_set_ltq_force_ds(unsigned int cpu, char *buf);

extern char *sysfs_get_ltq_alert(unsigned int cpu);
extern int sysfs_set_ltq_alert(unsigned int cpu, char *buf);

extern int sysfs_pstate_exists(void);
extern unsigned long sysfs_get_pstate_maxperf(unsigned int cpu);
extern unsigned long sysfs_get_pstate_minperf(unsigned int cpu);
extern unsigned long sysfs_get_pstate_noturbo(unsigned int cpu);
extern unsigned long sysfs_get_pstate_numpstates(unsigned int cpu);
extern unsigned long sysfs_get_pstate_turbo(unsigned int cpu);
extern char *sysfs_get_pstate_status(unsigned cpu);

#ifndef TARGET_X86
/* MISC interface
 * API to access the /sys/class/misc devices.
 */
#if 0
struct misc_temp_thresholds {
	struct pmcu_temp threshold;
	struct misc_temp_thresholds *next;
	struct misc_temp_thresholds *first;
};

extern struct misc_temp_thresholds *sysfs_get_misc_temp_thresholds(void);
extern void sysfs_put_misc_temp_thresholds(struct misc_temp_thresholds *first);
extern unsigned long sysfs_get_misc_temp_read_period(void);
extern int sysfs_modify_temp_threshold(long *threshold);
extern int sysfs_modify_read_period(unsigned long period);
#endif
#endif

/* REGULATOR interface
 * API to access the /sys/class/regulator devices.
 */
/* checks number regulator devices */
extern int sysfs_reg_exists(unsigned int reg);
extern int reg_get_max_num(void);

/* gets stats of the regulators */
extern char *sysfs_get_reg_state(unsigned int reg);
extern int sysfs_get_reg_uvolts(unsigned int reg);
extern int sysfs_get_reg_min_uvolts(unsigned int reg);
extern int sysfs_get_reg_max_uvolts(unsigned int reg);
extern char *sysfs_get_reg_name(unsigned int reg);
extern void sysfs_put_reg_name(char *name);
extern char *sysfs_get_reg_extctrl(unsigned int reg);
extern int sysfs_reg_virt_consumer_set_voltage(int reg, unsigned int min_uV,
					unsigned int max_uV);

/* HWMON interface
 * API to access the /sys/class/hwmon devices.
 */
extern int sysfs_hwmon_exists(unsigned int hwmon);
extern int sysfs_hwmon_tempx_exists(unsigned int hwmon, unsigned int temp);
extern unsigned long sysfs_get_hwmon_temp(unsigned int hwmon, unsigned int ts);
extern unsigned long sysfs_get_hwmon_temp_crit(unsigned int hwmon, int tempid);
extern unsigned long sysfs_get_hwmon_power(unsigned int hwmon);
extern unsigned long sysfs_get_hwmon_rail_volt(unsigned int hwmon);
extern unsigned long sysfs_get_hwmon_pwm(unsigned int hwmon);
extern unsigned long sysfs_get_hwmon_pvt_cpu_m0(unsigned int hwmon);
extern unsigned long sysfs_get_hwmon_pvt_cpu_m1(unsigned int hwmon);
extern unsigned long sysfs_get_hwmon_pvt_roc(unsigned int hwmon);
extern int hwmon_get_max_num(void);

/* TZONE interface
 * API to access the /sys/class/thermal devices.
 */
extern int tzone_get_max_num(void);
extern int sysfs_tzone_exists(unsigned int tzone);
extern long sysfs_get_tzone_temp(unsigned tzone);
extern char *sysfs_get_tzone_name(unsigned int hwmon);
extern void sysfs_put_tzone_name(char *name);
#define TZ_TYPE_MAX 		15
#define TZONE_TRIP_MAX		10
struct tz_data {
	int	hyst;
	long	temp;
	char	type[TZ_TYPE_MAX];
};
extern void sysfs_get_tzone_trip(unsigned tzone, struct tz_data *tz);


/* CDEV interface
 * API to access the /sys/class/thermal devices.
 */
extern int sysfs_cdev_exists(unsigned int cdev);
extern char *sysfs_get_cdev_name(unsigned int cdev);
extern void sysfs_put_cdev_name(char *name);
extern int sysfs_get_cdev_cur_state(unsigned int cdev);
extern int sysfs_get_cdev_max_state(unsigned int cdev);
extern int cdev_get_max_num(void);

/* ADP-DVFS interface
 * API to access the /sys/devices/soc0/e0180000.epu devices.
 */
extern int sysfs_adp_exists(unsigned int adp);
extern char *sysfs_get_adp_name(unsigned int adp);
extern void sysfs_put_adp_name(char *name);
extern char *sysfs_get_adp_scaling(unsigned int adp);
extern void sysfs_put_adp_scaling(char *adpscaling);
extern int sysfs_get_adp_dvfs_level(unsigned int adp);

/* get temp label
 * Please free allocated memory by calling sysfs_put_temp_label
 * after use.
 */
extern char *sysfs_get_temp_label(unsigned int hwmon, int tempid);
extern void sysfs_put_temp_label(char *temp_label);

/* get rail name
 * Please free allocated memory by calling sysfs_put_hwmon_rail_name
 * after use.
 */
extern char *sysfs_get_hwmon_rail_name(unsigned int hwmon);
extern void sysfs_put_hwmon_rail_name(char *rail_name);

/* get monitor name
 * Please free allocated memory by calling sysfs_put_hwmon_name
 * after use.
 */
extern char *sysfs_get_hwmon_name(unsigned int hwmon);
extern void sysfs_put_hwmon_name(char *name);

/* get speed class info calculated during boot-up
 * Please free allocated memory by calling sysfs_put_hwmon_name
 * after use.
 */
extern char *sysfs_get_hwmon_speed_class_boot(unsigned int hwmon);
extern void sysfs_put_hwmon_speed_class_boot(char *name);


/* CPUFREQ interface
 * API to access the cpufreq parameters.
 */
struct cpufreq_policy {
	unsigned long min;
	unsigned long max;
	char *governor;
};

#define GOV_MAX	20
struct cpufreq_available_governors {
	char governor[GOV_MAX];
	struct cpufreq_available_governors *next;
	struct cpufreq_available_governors *first;
};

struct cpufreq_available_frequencies {
	unsigned long frequency;
	struct cpufreq_available_frequencies *next;
	struct cpufreq_available_frequencies *first;
};


struct cpufreq_stats {
	unsigned long frequency;
	unsigned long long time_in_state;
	struct cpufreq_stats *next;
	struct cpufreq_stats *first;
};


/* determine current CPU frequency
 * - _kernel variant means kernel's opinion of CPU frequency
 * - _hardware variant means actual hardware CPU frequency,
 *    which is only available to root.
 *
 * returns 0 on failure, else frequency in kHz.
 */

extern unsigned long cpufreq_get_max_freq(unsigned int cpu);
extern int cpufreq_set_max_freq(unsigned int cpu, unsigned long max_freq);
extern unsigned long cpufreq_get_freq_hardware(unsigned int cpu);
extern unsigned long cpufreq_get_freq_scaling(unsigned int cpu);

#define cpufreq_get(cpu) cpufreq_get_freq_kernel(cpu);


/* determine hardware CPU frequency limits
 *
 * These may be limited further by thermal, energy or other
 * considerations by cpufreq policy notifiers in the kernel.
 */

extern int cpufreq_get_hardware_limits(unsigned int cpu,
							unsigned long *min,
							unsigned long *max);

/* determine CPUfreq policy currently used
 *
 * Remember to call cpufreq_put_policy when no longer needed
 * to avoid memory leakage, please.
 */


extern struct cpufreq_policy *cpufreq_get_policy(unsigned int cpu);

extern void cpufreq_put_policy(struct cpufreq_policy *policy);


/* determine CPUfreq governors currently available
 *
 * may be modified by modprobe'ing or rmmod'ing other governors. Please
 * free allocated memory by calling cpufreq_put_available_governors
 * after use.
 */


extern struct cpufreq_available_governors *cpufreq_get_available_governors(
							unsigned int cpu);

extern void cpufreq_put_available_governors(
				struct cpufreq_available_governors *first);


/* determine CPU frequency states available
 *
 * only present on _some_ ->target() cpufreq drivers. For information purposes
 * only. Please free allocated memory by calling cpufreq_put_available_
 * frequencies after use.
 */

extern struct cpufreq_available_frequencies *cpufreq_get_available_frequencies(
							unsigned int cpu);

extern void cpufreq_put_available_frequencies(
				struct cpufreq_available_frequencies *first);


/* determine stats for cpufreq subsystem
 *
 * This is not available in all kernel versions or configurations.
 */

extern struct cpufreq_stats *cpufreq_get_stats(unsigned int cpu,
					unsigned long long *total_time);

extern void cpufreq_put_stats(struct cpufreq_stats *stats);

extern unsigned long cpufreq_get_transitions(unsigned int cpu);


/* modify a policy by only changing min/max freq or governor
 *
 * Does not check whether result is what was intended.
 */

extern int cpufreq_modify_policy_governor(unsigned int cpu, char *governor);


/* set a specific frequency
 *
 * Does only work if userspace governor can be used and no external
 * interference (other calls to this function or to set/modify_policy)
 * occurs. Also does not work on ->range() cpufreq drivers.
 */

extern int cpufreq_set_frequency(unsigned int cpu,
					unsigned long target_frequency);

/* Lantiq functions */
/* get each single module status from the registered Lantiq modules
 * Please free allocated memory by calling cpufreq_put_ltq_module_status
 * after use.
 */

extern char *cpufreq_get_ltq_module_status(unsigned int cpu);
extern void cpufreq_put_ltq_module_status(char *module_status);

/* enable/disable the frequency scaling inside the Lantiq CPUFREQ driver.
 *
 * if DISABLE is set, the low level hardware driver doesn't accept any
 * frequency change request.
 */

extern int cpufreq_set_ltq_state_change_control(unsigned int cpu, char *buf);

extern int cpufreq_set_ltq_force_ds(unsigned int cpu, char *buf);
extern char *cpufreq_get_ltq_force_ds(unsigned int cpu);
extern void cpufreq_put_ltq_force_ds(char *force);

extern int cpufreq_set_ltq_alert(unsigned int cpu, char *buf);
extern char *cpufreq_get_ltq_alert(unsigned int cpu);
extern void cpufreq_put_ltq_alert(char *alert);


extern char *cpufreq_get_ltq_scaling_clks(unsigned int cpu);
extern void cpufreq_put_ltq_scaling_clks(char *scaling);

/* get the status of the change control
 * Please free allocated memory by calling cpufreq_put_ltq_state_change_control
 * after use.
 */

extern char *cpufreq_get_ltq_state_change_control(unsigned int cpu);
extern void cpufreq_put_ltq_state_change_control(char *control);

/* get status of the DVS
 * Please free allocated memory by calling cpufreq_put_ltq_dvs_support
 * after use.
 */

extern char *cpufreq_get_ltq_dvs_support(unsigned int cpu);
extern void cpufreq_put_ltq_dvs_support(char *dvs);

/* get scaling driver
 * Please free allocated memory by calling cpufreq_put_scaling_driver
 * after use.
 */

extern char *cpufreq_get_scaling_driver(unsigned int cpu);
extern void cpufreq_put_scaling_driver(char *s_drv);

/* get pstate status
 * Please free allocated memory by calling cpufreq_put_pstate_status
 * after use.
 */
extern char *cpufreq_get_pstate_status(unsigned int cpu);
extern void cpufreq_put_pstate_status(char *status);

/* CPUFREQ_DVS interface to control the core voltage regulator
 * Please free allocated memory by calling cpufreq_put_ltq_dvs_support
 * after use.
 */

extern int sysfs_dbg_reg_summary_exists(void);
extern int sysfs_dbg_clk_summary_exists(void);
extern int sysfs_reg_name_exists(char *name);

extern void sysfs_put(char *name);

extern char *sysfs_get_dbg_vol1(void);
extern char *sysfs_get_dbg_i2c_ctrl1(void);
extern char *sysfs_get_dbg_enable1(void);
extern char *sysfs_get_dbg_reg_summary(void);
extern char *sysfs_get_dbg_clk_summary(void);
extern int sysfs_modify_dbg_i2c_ctrl1(char *i2c);
extern int sysfs_modify_dbg_vol1(char *vol);

/* cpuidle interface
 * API to access the /sys/devices/system/cpu  cpuidle devices.
 */

extern int sysfs_cpuidle_exists(void);
extern int sysfs_cpu_cpuidle_exists(unsigned int cpuidle);
extern char *sysfs_get_cpuidle_driver(void);
extern void sysfs_put_cpuidle_driver(char *name);
extern char *sysfs_get_cpuidle_governor(void);
extern void sysfs_put_cpuidle_governor(char *name);
extern char *sysfs_get_cstate_state(unsigned int cpuidle);
extern char *sysfs_get_cstate_disable(unsigned int cpuidle);
extern void sysfs_put_cstate(char *name);
extern int sysfs_disable_state(unsigned int cpu, unsigned int state, unsigned int dis);

/* i2c interface to pmic to control power rails voltage
 * API to access the pmic to control power rails voltage.
 */

extern int i2c_pmic_set_vol(char *power_rail, unsigned int vol);
extern int i2c_pmic_get_vol(void);

#endif /* _ltq_pmutil_interface_h */
