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
#include <safe_str_lib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <regex.h>
#include <json-c/json.h>
#include "include/ltq_pmutil_interface.h"

#define BUF_SIZE		300
#define BUF_SIZE_WRITE		400
#define THERMAL_TEMP_MSK	0x1
#define POWER_VOLT_MSK		0x2
#define POWER_PWR_MSK		0x4
#define PVT_VOLT_MSK		0x8
#define CPU_FREQ_MSK		0x10
#define CDEV_MSK		0x20
#define PMIC_VOLT_MSK		0x40
#define MSR_MSK			0x80
#define CPULOAD_MSK		0x100
#define ADP_LVL_MSK		0x200
#define WAV_MSK			0x400
#define ETH_MSK			0x800
#define PCIE_MSK		0x1000
#define TOP_NOC_MSK		0x2000

extern char *pm_power_monitor_name;
extern char *pm_pvt_name;
extern unsigned int log_time;
extern int pm_soc_id;
static FILE *fp;
static int fp_sock;
int tempsocket;
int loop_break = 0xF;

static int sel_sock = LOG2FILE;
static char *buf;

static unsigned long eth0_2_rx_old, eth0_2_tx_old;
static unsigned long eth0_3_rx_old, eth0_3_tx_old;
static unsigned long eth0_4_rx_old, eth0_4_tx_old;
static unsigned long eth0_5_rx_old, eth0_5_tx_old;
static unsigned long eth0_1_rx_old, eth0_1_tx_old;
static unsigned long eth1_rx_old, eth1_tx_old;
static unsigned long VANI0_rx_old, VANI0_tx_old;

static unsigned long eth0_2_rx_throughput, eth0_2_tx_throughput;
static unsigned long eth0_3_rx_throughput, eth0_3_tx_throughput;
static unsigned long eth0_4_rx_throughput, eth0_4_tx_throughput;
static unsigned long eth0_5_rx_throughput, eth0_5_tx_throughput;
static unsigned long eth0_1_rx_throughput, eth0_1_tx_throughput;
static unsigned long eth1_rx_throughput, eth1_tx_throughput;
static unsigned long VANI0_rx_throughput, VANI0_tx_throughput;
static unsigned long wav24g_rx_old, wav24g_tx_old;
static unsigned long wav5g_rx_old, wav5g_tx_old;
static unsigned long wav6g_rx_old, wav6g_tx_old;
static unsigned long wav24g_rx_throughput, wav24g_tx_throughput;
static unsigned long wav5g_rx_throughput, wav5g_tx_throughput;
static unsigned long wav6g_rx_throughput, wav6g_tx_throughput;
char pcie_path_ls[5][BUF_SIZE] = {"cat ", "cat ", "cat ", "cat ", "cat "};
char pcie_path_l0s[5][BUF_SIZE] = {"cat ", "cat ", "cat ", "cat ", "cat "};
char pcie_path_l1[5][BUF_SIZE] = {"cat ", "cat ", "cat ", "cat ", "cat "};

static void write_to_file(int flag, int count, ...)
{
	va_list args;
	char *arg;
	int i, ret;

	va_start(args, count);

	for (i = 0; i < count; i++) {
		arg = va_arg(args, char*);
		if (sel_sock == LOG2FILE) {
			if (flag == 1)
				fprintf(fp, "%s;", arg);
			else
				fprintf(fp, "%s", arg);
		} else {
			if (!buf) {
				printf("NULL-Pointer Error during sent\n");
				return;
			}
			if (flag == 1)
				ret = sprintf_s(buf, 190, "%s;", arg);
			else
				ret = sprintf_s(buf, 190, "%s", arg);
			if (ret < 0) {
				printf("sprintf error during sent\n");
				return;
			}
			int sent = send(fp_sock, buf, strnlen_s(buf, BUF_SIZE_WRITE), 0);

			if (sent < 0) {
				printf("Sent error: %i, Message: %s\n",
				       errno, strerror(errno));
				printf("Probably Session was closed\n");
				if (buf)
					free(buf);
				buf = NULL;
				assert(sent >= 0);
			}
		}
	}
	va_end(args);
}

static void pm_log_temp(unsigned int i)
{
	int ret;
	long temp;
	char buffer[BUF_SIZE];

	temp = sysfs_get_tzone_temp(i);
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu", temp);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
}

static void pm_log_pwr(unsigned int i, unsigned int mask)
{
	unsigned long power, voltage;
	char *railname = NULL;
	char buffer[BUF_SIZE];
	int rn_found = 1;
	int ret;

	if (mask & POWER_PWR_MSK)
		power = sysfs_get_hwmon_power(i);

	if (mask & POWER_VOLT_MSK)
		voltage = sysfs_get_hwmon_rail_volt(i);

	if ((mask & POWER_PWR_MSK) || (mask & POWER_VOLT_MSK)) {
		railname = sysfs_get_hwmon_rail_name(i);
		if (!railname) {
			railname = "not defined";
			rn_found = 0;
		}
	}
	if (mask & POWER_VOLT_MSK) {
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu", voltage);
		assert(ret > 0);
		write_to_file(1, 1, buffer);
	}
	if (mask & POWER_PWR_MSK) {
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu", power / 1000);
		assert(ret > 0);
		write_to_file(1, 1, buffer);
	}
	if ((mask & POWER_PWR_MSK) || (mask & POWER_VOLT_MSK)) {
		if (rn_found == 1)
			sysfs_put_hwmon_rail_name(railname);
	}
}

static void pm_log_pvt(unsigned int i)
{
	unsigned long pvt_cpu_m0_voltage, pvt_cpu_m1_voltage, pvt_roc_voltage;
	char buffer[BUF_SIZE];
	int ret;

	pvt_cpu_m0_voltage = sysfs_get_hwmon_pvt_cpu_m0(i);
	pvt_cpu_m1_voltage = sysfs_get_hwmon_pvt_cpu_m1(i);
	pvt_roc_voltage = sysfs_get_hwmon_pvt_roc(i);
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu;%lu",
			pvt_cpu_m0_voltage,
			pvt_cpu_m1_voltage,
			pvt_roc_voltage);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
}

static void pm_log_freq(void)
{
	unsigned long cpu0, cpu1, cpu2, cpu3;
	char buffer[BUF_SIZE];
	int ret;

	cpu0 = cpufreq_get_freq_hardware(0) / 1000;
	cpu1 = cpufreq_get_freq_hardware(1) / 1000;
	cpu2 = cpufreq_get_freq_hardware(2) / 1000;
	cpu3 = cpufreq_get_freq_hardware(3) / 1000;

	ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu;%lu;%lu",
			cpu0, cpu1, cpu2, cpu3);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
}

static void pm_log_cdev(unsigned int i)
{
	unsigned long cd_cur;
	char *cdevname;
	char buffer[BUF_SIZE];
	int cdevn_found = 1;
	int ret;

	cd_cur = sysfs_get_cdev_cur_state(i);
	cdevname = sysfs_get_cdev_name(i);
	if (!cdevname) {
		cdevname = "not defined";
		cdevn_found = 0;
	}

	ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu", cd_cur);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	if (cdevn_found == 1)
		sysfs_put_cdev_name(cdevname);
}

#if 0
static void pm_log_reg(unsigned int i)
{
	char buffer[BUF_SIZE];
	int uvolts;
	int ret;

	uvolts = sysfs_get_reg_uvolts(i);
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%d", uvolts / 1000);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
}
#endif
static void pm_log_msr(void)
{
	char buffer[200];
	char *msrbuf198_0;
	char *msrbuf198_1;
	char *msrbuf198_2;
	char *msrbuf198_3;
	char *msrbuf199_0;
	char *msrbuf199_1;
	char *msrbuf199_2;
	char *msrbuf199_3;
	size_t len;
	FILE *f;
	struct stat statbuf;

	if (stat("/opt/intel/bin/rdmsr", &statbuf))
		return;

	msrbuf198_0 = calloc(20, sizeof(char));
	msrbuf198_1 = calloc(20, sizeof(char));
	msrbuf198_2 = calloc(20, sizeof(char));
	msrbuf198_3 = calloc(20, sizeof(char));
	msrbuf199_0 = calloc(20, sizeof(char));
	msrbuf199_1 = calloc(20, sizeof(char));
	msrbuf199_2 = calloc(20, sizeof(char));
	msrbuf199_3 = calloc(20, sizeof(char));
	if (!msrbuf198_0 ||
	    !msrbuf198_1 ||
	    !msrbuf198_2 ||
	    !msrbuf198_3 ||
	    !msrbuf199_0 ||
	    !msrbuf199_1 ||
	    !msrbuf199_2 ||
	    !msrbuf199_3)
		goto log_exit;

	system("rdmsr -a 0x198 > /tmp/readmsr.txt");
	system("rdmsr -a 0x199 >> /tmp/readmsr.txt");
	f = fopen("/tmp/readmsr.txt", "rb");
	if (f) {
		fgets(msrbuf198_0, 20, f);
		strtok(msrbuf198_0, "\n");
		len = strlen(msrbuf198_0);
		msrbuf198_0 = strncpy(msrbuf198_0, &msrbuf198_0[len - 4], 5);
		fgets(msrbuf198_1, 20, f);
		strtok(msrbuf198_1, "\n");
		len = strlen(msrbuf198_1);
		msrbuf198_1 = strncpy(msrbuf198_1, &msrbuf198_1[len - 4], 5);
		fgets(msrbuf198_2, 20, f);
		strtok(msrbuf198_2, "\n");
		len = strlen(msrbuf198_2);
		msrbuf198_2 = strncpy(msrbuf198_2, &msrbuf198_2[len - 4], 5);
		fgets(msrbuf198_3, 20, f);
		strtok(msrbuf198_3, "\n");
		len = strlen(msrbuf198_3);
		msrbuf198_3 = strncpy(msrbuf198_3, &msrbuf198_3[len - 4], 5);

		fgets(msrbuf199_0, 20, f);
		strtok(msrbuf199_0, "\n");
		fgets(msrbuf199_1, 20, f);
		strtok(msrbuf199_1, "\n");
		fgets(msrbuf199_2, 20, f);
		strtok(msrbuf199_2, "\n");
		fgets(msrbuf199_3, 20, f);
		strtok(msrbuf199_3, "\n");
		fclose(f);
	}
	if (sprintf_s(buffer, 200 - 1,
		      "%lu;%lu;%lu;%lu;"
		      "%lu;%lu;%lu;%lu",
		      strtol(msrbuf198_0, NULL, 16),
		      strtol(msrbuf198_1, NULL, 16),
		      strtol(msrbuf198_2, NULL, 16),
		      strtol(msrbuf198_3, NULL, 16),
		      strtol(msrbuf199_0, NULL, 16),
		      strtol(msrbuf199_1, NULL, 16),
		      strtol(msrbuf199_2, NULL, 16),
		      strtol(msrbuf199_3, NULL, 16)) <= 0) {
		printf("sprintf_s failed\n");
		goto log_exit;
	}
	write_to_file(1, 1, buffer);
log_exit:
	free(msrbuf198_0);
	free(msrbuf198_1);
	free(msrbuf198_2);
	free(msrbuf198_3);
	free(msrbuf199_0);
	free(msrbuf199_1);
	free(msrbuf199_2);
	free(msrbuf199_3);
}

#if 0
static int helper_md_freq(int reg_val)
{
	switch (reg_val) {
	case 0x2F:
		return 624000 / 1000;
	case 0x2B:
		return 780000 / 1000;
	case 0x2A:
		return 936000 / 1000;
	case 0x1A:
		return 1092000 / 1000;
	case 0x26:
		return 1248000 / 1000;
	case 0x19:
		return 1404000 / 1000;
	case 0x36:
		return 1560000 / 1000;
	case 0x1D:
		return 1716000 / 1000;
	case 0x37:
		return 1872000 / 1000;
	case 0x2D:
		return 2028000 / 1000;
	default:
		return 9999999 / 1000;
	}
}

static int pm_log_mod_freq(void)
{
	struct stat statbuf;
	char buffer[BUF_SIZE];
	FILE *fp1 = NULL;
	char *p_end;
	char *str;
	char iostring[100];
	long reg;
	long cpum0, cpum1;
	int ret;

	system("io -4 0xe02001b0 1> /tmp/mod_freq_reg.txt 2> /dev/null");
	stat("/tmp/mod_freq_reg.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;

	fp1 = fopen("/tmp/mod_freq_reg.txt", "r");
	if (!fp1) {
		perror("Error opening file");
		return 0;
	}

	if (!fgets(iostring, 100, fp1)) {
		perror("Error reading file");
		fclose(fp1);
		return 0;
	}

	fclose(fp1);
	str = strtok(iostring, " :");
	str = strtok(NULL, " :");
	reg = strtol(str, &p_end, 16);

	cpum0 = (reg & 0xFFFF);
	cpum1 = (reg & 0xFFFF0000) >> 16;

	ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu",
			helper_md_freq(cpum0),
			helper_md_freq(cpum1));
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}
#endif
static int pm_log_adp_lvl(void)
{
	struct stat statbuf;
	char cmd1[1024];
	char cmd2[1024];
	char line[1024];
	FILE *fp = NULL;
	char buffer[BUF_SIZE];
	char swdvfs_str[10] = {'\0'};
	char hwdfs_str[10] = {'\0'};
	int adplvl = 0, ret, swadplvl = 0, hwadpcfg = 0;

	if (stat("/sys/devices/soc0/e0180000.epu/adp_dvfs_level", &statbuf) == 0) {
		snprintf(cmd1, sizeof(cmd1), "/sys/devices/soc0/e0180000.epu/adp_dvfs_level");
		snprintf(cmd2, sizeof(cmd2), "/sys/devices/soc0/e0180000.epu/adp_level_config");
	} else if (stat("/sys/devices/platform/soc/e0180000.epu/adp_dvfs_level", &statbuf) == 0) {
		snprintf(cmd1, sizeof(cmd1), "/sys/devices/platform/soc/e0180000.epu/adp_dvfs_level");
		snprintf(cmd2, sizeof(cmd2), "/sys/devices/platform/soc/e0180000.epu/adp_level_config");
	} else {
		perror("Error opening file");
		goto write_buffer;
	}
	fp = fopen(cmd1, "r");
	if (!fp) {
		perror("fopen");
		goto level_config;
	}

	fgets(line, sizeof(line), fp);
	adplvl = atoi(line);

	fclose(fp);
	if (adplvl == 0)
		adplvl = 2;
	else if (adplvl == 2)
		adplvl = 0;

level_config:
	fp = fopen(cmd2, "r");
	if (!fp) {
		perror("fopen");
		goto write_buffer;
	}

	while (fgets(line, sizeof(line), fp)) {
		sscanf_s(line, " sw_adp_dvfs_level_set: %s", swdvfs_str);
		sscanf_s(line, " hw_adp_dfs_set: %s", hwdfs_str);
	}
	fclose(fp);
	if (strcmp(swdvfs_str, "high") == 0)
		swadplvl = 2;
	if (strcmp(swdvfs_str, "med") == 0)
		swadplvl = 1;
	if (strcmp(swdvfs_str, "low") == 0)
		swadplvl = 0;

	if (strcmp(hwdfs_str, "off") == 0)
		hwadpcfg = 0;
	else
		hwadpcfg = 1;

write_buffer:
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%d;%d;%d", adplvl, swadplvl, hwadpcfg);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

#define PROC_STAT_READ_INTERVAL 100000 /*usec*/
static int pm_log_cpuload(struct timeval *time_start)
{
	struct timeval time_now;
	char buffer[BUF_SIZE];
	FILE *fp1 = NULL;
	char *p_end;
	char *str;
	char iostring[100];
	int user_time = 0, nice_time = 0, system_time = 0, idle_time = 0;
	int iowait_time = 0, irq_time = 0, sirq_time = 0;
	static int cpu_load_perc;
	static int old_time_doing_things;
	static int old_time_doing_nothing;
	int time_doing_things, time_doing_nothing;
	int ret;
	int found = 0;
	unsigned long time_elapse;

	gettimeofday(&time_now, NULL);
	time_elapse = (time_now.tv_sec - time_start->tv_sec) * 1e6;
	time_elapse = (time_elapse + (time_now.tv_usec - time_start->tv_usec));
	if (time_elapse < PROC_STAT_READ_INTERVAL) {
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu", cpu_load_perc);
		assert(ret > 0);
		write_to_file(1, 1, buffer);
		return 0;
	}
	time_start->tv_sec = time_now.tv_sec;
	time_start->tv_usec = time_now.tv_usec;
	fp1 = fopen("/proc/stat", "r");

	if (!fp1) {
		perror("Error opening file");
		return 0;
	}

	while (fgets(iostring, sizeof(iostring), fp1)) {
		if (strstr(iostring, "cpu ")) {
			found = 1;
			break;
		}
	}
	if (!found) {
		perror("Error reading cpu load info");
		fclose(fp1);
		return 0;
	}

	fclose(fp1);
	str = strtok(iostring, " ");
	str = strtok(NULL, " ");
	if (str != NULL)
		user_time = (int)strtol(str, &p_end, 0);
	str = strtok(NULL, " ");
	if (str != NULL)
		nice_time = (int)strtol(str, &p_end, 0);
	str = strtok(NULL, " ");
	if (str != NULL)
		system_time = (int)strtol(str, &p_end, 0);
	str = strtok(NULL, " ");
	if (str != NULL)
		idle_time = (int)strtol(str, &p_end, 0);
	str = strtok(NULL, " ");
	if (str != NULL)
		iowait_time = (int)strtol(str, &p_end, 0);
	str = strtok(NULL, " ");
	if (str != NULL)
		irq_time = (int)strtol(str, &p_end, 0);
	str = strtok(NULL, " ");
	if (str != NULL)
		sirq_time = (int)strtol(str, &p_end, 0);

	time_doing_things = (user_time + nice_time + system_time + irq_time + sirq_time + iowait_time) - old_time_doing_things;
	time_doing_nothing = idle_time - old_time_doing_nothing;
	cpu_load_perc = time_doing_things * 100 / (time_doing_things + time_doing_nothing);
	old_time_doing_things = user_time + nice_time + system_time + irq_time + sirq_time + iowait_time;
	old_time_doing_nothing = idle_time;
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu", cpu_load_perc);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

static int pm_log_eth(char *ethname)
{
	char cmd[1024];
	char line[1024];
	char buffer[BUF_SIZE];
	FILE *fp = NULL;
	int link = 0, ret, status;
	char active_str[10] = {'\0'};
	int active = 0;
	int speed = -1;

	snprintf(cmd, sizeof(cmd), "/sys/class/net/%s/carrier", ethname);
	fp = fopen(cmd, "r");
	if (!fp) {
		link = 0;
		goto write_buffer;
	}

	while (fgets(line, sizeof(line), fp))
		sscanf_s(line, "%d", &link);

	fclose(fp);

	snprintf(cmd, sizeof(cmd), "/sys/class/net/%s/speed", ethname);
	fp = fopen(cmd, "r");
	if (!fp) {
		speed = 0;
		goto eee_check;
	}
	while (fgets(line, sizeof(line), fp))
		sscanf_s(line, "%d", &speed);

	fclose(fp);

eee_check:
	/* ethtool takes 8ms per call */
	snprintf(cmd, sizeof(cmd), "ethtool --show-eee %s 2> /dev/null | grep 'EEE status'", ethname);
	fp = popen(cmd, "r");
	if (!fp) {
		perror("popen");
		active = 0;
		goto write_buffer;
	}
	while (fgets(line, sizeof(line), fp))
		sscanf_s(line, "%*s %*s %*s %*s %s", active_str);

	pclose(fp);
	if (strcmp(active_str, "active") == 0)
		active = 1;
	else
		active = 0;

write_buffer:
	if (link == 0)
		status = 0;
	else if ((link == 1) && (active == 0))
		status = 1;
	else
		status = 2;

	ret = sprintf_s(buffer, BUF_SIZE - 1, "%d;%d", status, speed);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

static int pm_log_eth_throughput(char *inf, int idx, struct timeval *time_start)
{
	unsigned long rx_bytes, tx_bytes, rx_bytes_temp=0, tx_bytes_temp=0;
	char buffer[BUF_SIZE];
	char tx_file[128], rx_file[128];
	char tx_file_pon[128], rx_file_pon[128];
	FILE *fp;
	float factor;
	struct timeval time_now;
	unsigned long time_elapse;
	int ret;

	gettimeofday(&time_now, NULL);
	time_elapse = (time_now.tv_sec - time_start->tv_sec) * 1e6;
	time_elapse = (time_elapse + (time_now.tv_usec - time_start->tv_usec));
	factor = (float)1000000 / (float)time_elapse;
	time_start->tv_sec = time_now.tv_sec;
	time_start->tv_usec = time_now.tv_usec;

	if (idx != 20) { /* check for PON */
		sprintf(tx_file, "/sys/class/net/%s/statistics/tx_bytes", inf);
		sprintf(rx_file, "/sys/class/net/%s/statistics/rx_bytes", inf);

		fp = fopen(rx_file, "r");
			if (!fp) {
				/* printf("Failed to open file: %s\n", rx_file); */
				rx_bytes = 0;
			} else {
				fscanf(fp, "%lu", &rx_bytes);
				fclose(fp);
			}

			fp = fopen(tx_file, "r");
			if (!fp) {
				/* printf("Failed to open file: %s\n", tx_file); */
				tx_bytes = 0;
			} else  {
				fscanf(fp, "%lu", &tx_bytes);
				fclose(fp);
			}

	} else {
		/* PON; this is a very specific workaround */
		sprintf(rx_file, "/sys/class/net/VANI0/statistics/rx_bytes");
		sprintf(rx_file_pon, "/sys/class/net/VUNI0_0/statistics/rx_bytes");
		sprintf(tx_file, "/sys/class/net/VANI0/statistics/tx_bytes");
		sprintf(tx_file_pon, "/sys/class/net/VUNI0_0/statistics/tx_bytes");

		fp = fopen(rx_file, "r");
			if (!fp) {
				/* printf("Failed to open file: %s\n", rx_file); */
				rx_bytes = 0;
			} else {
				fscanf(fp, "%lu", &rx_bytes_temp);
				fclose(fp);
			}
			fp = fopen(tx_file, "r");
			if (!fp) {
				/* printf("Failed to open file: %s\n", tx_file); */
				tx_bytes = 0;
			} else  {
				fscanf(fp, "%lu", &tx_bytes_temp);
				fclose(fp);
			}

			fp = fopen(rx_file_pon, "r");
			if (!fp) {
				/* printf("Failed to open file: %s\n", rx_file); */
				rx_bytes = 0;
			} else {
				fscanf(fp, "%lu", &rx_bytes);
				fclose(fp);
			}
			fp = fopen(tx_file_pon, "r");
			if (!fp) {
				/* printf("Failed to open file: %s\n", tx_file); */
				tx_bytes = 0;
			} else  {
				fscanf(fp, "%lu", &tx_bytes);
				fclose(fp);
			}

			rx_bytes += rx_bytes_temp;
			tx_bytes += tx_bytes_temp;
	}

	switch (idx) {
	case 2: /* eth0_2 */
		if (eth0_2_rx_old == 0) { /* fix for first run */
			eth0_2_rx_old = rx_bytes;
			eth0_2_tx_old = tx_bytes;
		}
		eth0_2_rx_throughput = rx_bytes - eth0_2_rx_old;
		eth0_2_tx_throughput = tx_bytes - eth0_2_tx_old;
		eth0_2_rx_throughput *= factor;
		eth0_2_tx_throughput *= factor;
		eth0_2_rx_old = rx_bytes;
		eth0_2_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", eth0_2_rx_throughput, eth0_2_tx_throughput);
		break;
	case 3: /* eth0_3 */
		if (eth0_3_rx_old == 0) { /* fix for first run */
			eth0_3_rx_old = rx_bytes;
			eth0_3_tx_old = tx_bytes;
		}
		eth0_3_rx_throughput = rx_bytes - eth0_3_rx_old;
		eth0_3_tx_throughput = tx_bytes - eth0_3_tx_old;
		eth0_3_rx_throughput *= factor;
		eth0_3_tx_throughput *= factor;
		eth0_3_rx_old = rx_bytes;
		eth0_3_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", eth0_3_rx_throughput, eth0_3_tx_throughput);
		break;
	case 4: /* eth0_4 */
		if (eth0_4_rx_old == 0) { /* fix for first run */
			eth0_4_rx_old = rx_bytes;
			eth0_4_tx_old = tx_bytes;
		}
		eth0_4_rx_throughput = rx_bytes - eth0_4_rx_old;
		eth0_4_tx_throughput = tx_bytes - eth0_4_tx_old;
		eth0_4_rx_throughput *= factor;
		eth0_4_tx_throughput *= factor;
		eth0_4_rx_old = rx_bytes;
		eth0_4_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", eth0_4_rx_throughput, eth0_4_tx_throughput);
		break;
	case 5: /* eth0_5 */
		if (eth0_5_rx_old == 0) { /* fix for first run */
			eth0_5_rx_old = rx_bytes;
			eth0_5_tx_old = tx_bytes;
		}
		eth0_5_rx_throughput = rx_bytes - eth0_5_rx_old;
		eth0_5_tx_throughput = tx_bytes - eth0_5_tx_old;
		eth0_5_rx_throughput *= factor;
		eth0_5_tx_throughput *= factor;
		eth0_5_rx_old = rx_bytes;
		eth0_5_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", eth0_5_rx_throughput, eth0_5_tx_throughput);
		break;
	case 1: /* eth0_1 */
		if (eth0_1_rx_old == 0) { /* fix for first run */
			eth0_1_rx_old = rx_bytes;
			eth0_1_tx_old = tx_bytes;
		}
		eth0_1_rx_throughput = rx_bytes - eth0_1_rx_old;
		eth0_1_tx_throughput = tx_bytes - eth0_1_tx_old;
		eth0_1_rx_throughput *= factor;
		eth0_1_tx_throughput *= factor;
		eth0_1_rx_old = rx_bytes;
		eth0_1_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", eth0_1_rx_throughput, eth0_1_tx_throughput);
		break;
	case 10: /* eth1 */
		if (eth1_rx_old == 0) { /* fix for first run */
			eth1_rx_old = rx_bytes;
			eth1_tx_old = tx_bytes;
		}
		eth1_rx_throughput = rx_bytes - eth1_rx_old;
		eth1_tx_throughput = tx_bytes - eth1_tx_old;
		eth1_rx_throughput *= factor;
		eth1_tx_throughput *= factor;
		eth1_rx_old = rx_bytes;
		eth1_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", eth1_rx_throughput, eth1_tx_throughput);
		break;
	case 20: /* VANI0 (PON) */
		if (VANI0_rx_old == 0) { /* fix for first run */
			VANI0_rx_old = rx_bytes;
			VANI0_tx_old = tx_bytes;
		}
		VANI0_rx_throughput = rx_bytes - VANI0_rx_old;
		VANI0_tx_throughput = tx_bytes - VANI0_tx_old;
		VANI0_rx_throughput *= factor;
		VANI0_tx_throughput *= factor;
		VANI0_rx_old = rx_bytes;
		VANI0_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", VANI0_rx_throughput, VANI0_tx_throughput);
		break;
	default:
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", 0, 0);
		break;
	}

	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

#if 0
static int pm_log_json(void)
{
	json_object *obj = json_object_new_object();
	json_object *name = json_object_new_string("John");
	json_object *age = json_object_new_int(25);
	json_object *address = json_object_new_string("New York");

	json_object_object_add(obj, "Name", name);
	json_object_object_add(obj, "Age", age);
	json_object_object_add(obj, "Address", address);

	printf("%s\n", json_object_to_json_string(obj));
	return 0;
}
#endif

#define PCIE_LNK_STAT_CUT	51
#define PCIE_L0S_CUT		 9
static int pm_log_prepare_for_pcie_aspm(void)
{
	FILE *fp;
	char line[256];
	int i, len;
	int num; /* number of interfaces */

	system("find /sys/devices/platform/soc/ -name 'l0s_aspm' > /tmp/pm_log_pcie_l0s_aspm.txt");
	system("find /sys/devices/platform/soc/ -name 'l1_aspm' > /tmp/pm_log_pcie_l1_aspm.txt");
	fp = fopen("/tmp/pm_log_pcie_l0s_aspm.txt", "r");
	if (!fp)
		return -1;
	i = 0;
	num = 0;
	while (fgets(line, sizeof(line), fp)) {
		strcat(&pcie_path_ls[i][0], line);
		strcat(&pcie_path_l0s[i][0], line);
		strcat(&pcie_path_l1[i][0], line);
		len = strlen(&pcie_path_ls[i][0]);

		if (len > PCIE_LNK_STAT_CUT)
			strncpy(&pcie_path_ls[i][0] + len - PCIE_LNK_STAT_CUT, "", PCIE_LNK_STAT_CUT);
		strcat(&pcie_path_ls[i][0], "pcie_link_status");

		if (len > PCIE_L0S_CUT)
			strncpy(&pcie_path_l1[i][0] + len - PCIE_L0S_CUT, "", PCIE_L0S_CUT);
		strcat(&pcie_path_l1[i][0], "l1_aspm");
		/*
		 * printf("pcie_path_ls[%d]: %s\n", i, &pcie_path_ls[i][0]);
		 * printf("pcie_path_l0s[%d]: %s\n", i, &pcie_path_l0s[i][0]);
		 * printf("pcie_path_l1[%d]: %s\n", i, &pcie_path_l1[i][0]);
		 */
		num++;
		i++;
		if (i > 4)
			break;
	}
	fclose(fp);
	return num;
}

static int pm_log_pcie_aspm(int num)
{
	char buffer[BUF_SIZE];
	char *p = buffer;
	FILE *fp;
	char line[256];
	int ret, i;
	int port[5] = {0}, speed[5] = {0}, width[5] = {0};
	int l0s[5] = {0}, l1[5] = {0};

	/* check if interface exists */
	if (!num)
		return 0;

	i = 0;
	while (i < num) {
		fp = popen(&pcie_path_ls[i][0], "r");
		if(fp){
			if (!fgets(line, sizeof(line), fp)) {
				pclose(fp);
				break;
			}
			sscanf_s(line, " Port%d %*s x%d %*s %d", &port[i], &width[i], &speed[i]);
			//printf("port:%d, width: %d, speed:%d\n", port[i], width[i], speed[i]);
			i++;
			pclose(fp);
		}
	}

	i = 0;
	while (i < num) {
		fp = popen(&pcie_path_l0s[i][0], "r");
		if(fp){
			if (!fgets(line, sizeof(line), fp)) {
				pclose(fp);
				break;
			}
			sscanf_s(line, " %d", &l0s[i]);
			//printf("l0s_aspm:%d\n", l0s[i]);
			i++;
			pclose(fp);
		}
	}
	i = 0;
	while (i < num) {
		fp = popen(&pcie_path_l1[i][0], "r");
		if(fp){
			if (!fgets(line, sizeof(line), fp)) {
				pclose(fp);
				break;
			}
			sscanf_s(line, " %d", &l1[i]);
			//printf("l1_aspm:%d\n", l1[i]);
			i++;
			pclose(fp);
		}
	}
	i = 0;
	while (i < num) {
		ret = sprintf_s(p, BUF_SIZE - 1, "%d;%d;%d;%d;%d;", port[i], width[i], speed[i], l0s[i], l1[i]);
		assert(ret > 0);
		p = p + ret;
		i++;
	}

	write_to_file(0, 1, buffer);
	return 0;
}

static int pm_log_top_noc(void)
{
	char buffer[BUF_SIZE];
	FILE *fp = NULL;
	char iostring[100];
	int clk = 0, ret;

	/* Open the command for reading. */
	fp = popen("cat /proc/sys/lgm_sysctl/pll_clk1_div", "r");
	if (!fp)
		goto write_buffer;

	if (!fgets(iostring, sizeof(iostring) - 1, fp))
		goto write_buffer;

	clk = atoi(iostring);
	if (clk == 3)
		clk = 1;
	else if (clk == 14)
		clk = 0;
	else
		clk = -1;

write_buffer:
	if(fp)
		pclose(fp);
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%d", clk);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

static int pm_log_wav_num_ant(char *inf)
{
	struct stat statbuf;
	char cmd[1024];
	char line[1024];
	char buffer[BUF_SIZE];
	FILE *fp = NULL;
	int coc = 0, tx = 0, rx = 0, ret;
	char directory_name[255];

	/* check if interface exists */
	snprintf(cmd, sizeof(cmd), "/proc/sys/net/ipv4/conf/%s/accept_local", inf);
	if (stat(cmd, &statbuf)) {
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%d;%d;%d", 0, 0, 0);
		goto write_buffer;
	}

	snprintf(cmd, sizeof(cmd), "iw %s iwlwav gCoCPower 2> /dev/null", inf);
	fp = popen(cmd, "r");
	if (!fp) {
		perror("popen");
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%d;%d;%d", 0, 0, 0);
		goto write_buffer;
	}

	while (fgets(line, sizeof(line), fp))
		sscanf_s(line, " gCoCPower:%d %d %d", &coc, &tx, &rx);

	pclose(fp);

	/* fix to report 0 antennas if wlan4.3 / wan2.2 or wlan0.1 are switched off */
	if (strcmp(inf, "wlan4") == 0)
		snprintf(directory_name, sizeof(directory_name), "/sys/class/net/wlan4.3");
	else if (strcmp(inf, "wlan2") == 0)
		snprintf(directory_name, sizeof(directory_name), "/sys/class/net/wlan2.2");
	else if (strcmp(inf, "wlan0") == 0)
		snprintf(directory_name, sizeof(directory_name), "/sys/class/net/wlan0.1");

	if (stat(directory_name, &statbuf) != 0) { /* directory does not exist */
		coc = 0;
		tx = 0;
		rx = 0;
	}

write_buffer:
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%d;%d;%d", coc, tx, rx);
	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

static int pm_log_wav_throughput(char *inf, int idx, struct timeval *time_start)
{
	char tx_file[128], rx_file[128];
	unsigned long rx_bytes, tx_bytes;
	char buffer[BUF_SIZE];
	FILE *fp;
	int ret;
	float factor;
	struct timeval time_now;
	unsigned long time_elapse;

	gettimeofday(&time_now, NULL);
	time_elapse = (time_now.tv_sec - time_start->tv_sec) * 1e6;
	time_elapse = (time_elapse + (time_now.tv_usec - time_start->tv_usec));
	factor = (float)1000000 / (float)time_elapse;
	time_start->tv_sec = time_now.tv_sec;
	time_start->tv_usec = time_now.tv_usec;

	sprintf(tx_file, "/sys/class/net/%s/statistics/tx_bytes", inf);
	sprintf(rx_file, "/sys/class/net/%s/statistics/rx_bytes", inf);

	fp = fopen(rx_file, "r");
	if (!fp) {
		rx_bytes = 0;
	} else {
		fscanf(fp, "%lu", &rx_bytes);
		fclose(fp);
	}

	fp = fopen(tx_file, "r");
	if (!fp) {
		tx_bytes = 0;
	} else {
		fscanf(fp, "%lu", &tx_bytes);
		fclose(fp);
	}

	if (idx == 0) {
		if (wav24g_rx_old == 0) { /* fix for first run */
			wav24g_rx_old = rx_bytes;
			wav24g_tx_old = tx_bytes;
		}
		wav24g_rx_throughput = rx_bytes - wav24g_rx_old;
		wav24g_tx_throughput = tx_bytes - wav24g_tx_old;
		wav24g_rx_throughput *= factor;
		wav24g_tx_throughput *= factor;
		wav24g_rx_old = rx_bytes;
		wav24g_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", wav24g_rx_throughput, wav24g_tx_throughput);
	} else if (idx == 2) {
		if (wav5g_rx_old == 0) { /* fix for first run */
			wav5g_rx_old = rx_bytes;
			wav5g_tx_old = tx_bytes;
		}
		wav5g_rx_throughput = rx_bytes - wav5g_rx_old;
		wav5g_tx_throughput = tx_bytes - wav5g_tx_old;
		wav5g_rx_throughput *= factor;
		wav5g_tx_throughput *= factor;
		wav5g_rx_old = rx_bytes;
		wav5g_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", wav5g_rx_throughput, wav5g_tx_throughput);
	} else if (idx == 4) {
		if (wav6g_rx_old == 0) { /* fix for first run */
			wav6g_rx_old = rx_bytes;
			wav6g_tx_old = tx_bytes;
		}
		wav6g_rx_throughput = rx_bytes - wav6g_rx_old;
		wav6g_tx_throughput = tx_bytes - wav6g_tx_old;
		wav6g_rx_throughput *= factor;
		wav6g_tx_throughput *= factor;
		wav6g_rx_old = rx_bytes;
		wav6g_tx_old = tx_bytes;
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", wav6g_rx_throughput, wav6g_tx_throughput);
	} else {
		ret = sprintf_s(buffer, BUF_SIZE - 1, "%lu;%lu", 0, 0);
	}

	assert(ret > 0);
	write_to_file(1, 1, buffer);
	return 0;
}

static int pm_log_wav_peers_connected(char *inf)
{
	char buffer[BUF_SIZE];
	FILE *fp;
	int ret, num = 0;
	char path[100];
	char command[100];
	int count = 0;

	snprintf(command, sizeof(command), "/proc/net/mtlk/%s/sta_list", inf);
	fp = fopen(command, "r");
	if (!fp)
		goto write_buffer;

	while (fgets(path, sizeof(path) - 1, fp))
		count++; /* count number of lines */

	fclose(fp);

	if (count >= 7)
		num = count - 7;

write_buffer:
	ret = sprintf_s(buffer, BUF_SIZE - 1, "%d;", num);
	assert(ret > 0);
	write_to_file(0, 1, buffer);
	return 0;
}

void pm_log_column_name(unsigned int mask, unsigned int cnt, int max_tzone_num, int max_hwmon_num)
{
	unsigned int i;
	char buffer[BUF_SIZE];
	int max_cdev_num;
//	int max_reg_num;
	char *name = NULL;
	char *railname = NULL;
	int ret, ret0, ret1;

	write_to_file(1, 1, "time-stamp");
	memset(buffer, 0, sizeof(buffer));
	/******************tz name *******************************************/
	if (mask & THERMAL_TEMP_MSK) {
		if (max_tzone_num == 0)
			goto powerrail;

		for (i = 0; i < (unsigned int)max_tzone_num; i++) {
			name = sysfs_get_tzone_name(i);
			if (!name)
				continue;
			ret = sprintf_s(buffer, BUF_SIZE - 1,
					"%s[mC]", name);
			assert(ret > 0);
			write_to_file(1, 1, buffer);
			sysfs_put_tzone_name(name);
		}
	}
powerrail:
	/****************** Power Rail name  *********************************/
	if ((mask & POWER_VOLT_MSK) ||
	    (mask & POWER_PWR_MSK) ||
	    (mask & PVT_VOLT_MSK)) {
		if (max_hwmon_num == 0)
			goto cpufreq;

		for (i = 0; i < (unsigned int)max_hwmon_num; i++) {
			name = sysfs_get_hwmon_name(i);
			if (!name)
				continue;
			if (strcmp(name, pm_power_monitor_name) == 0) {
				railname = sysfs_get_hwmon_rail_name(i);
				if (mask & POWER_VOLT_MSK) {
					memset(buffer, 0, sizeof(buffer));
					ret = sprintf_s(buffer, BUF_SIZE - 1,
							"%s_volt[mV]", railname);
					assert(ret > 0);
					write_to_file(1, 1, buffer);
				}
				if (mask & POWER_PWR_MSK) {
					memset(buffer, 0, sizeof(buffer));
					ret = sprintf_s(buffer, BUF_SIZE - 1,
							"%s_pwr[mW]", railname);
					assert(ret > 0);
					write_to_file(1, 1, buffer);
				}
				sysfs_put_hwmon_rail_name(railname);
			}
			if ((strcmp(name, pm_pvt_name) == 0) &&
			    (mask & PVT_VOLT_MSK)) {
				write_to_file(1, 1, "pvt_cpum0[mV];"
					      "pvt_cpum1[mV];"
					      "pvt_roc[mV]");
			}
			sysfs_put_hwmon_name(name);
		}
	}
cpufreq:
	/****************** CPU Frequency name  ******************************/
	if (mask & CPU_FREQ_MSK) {
		write_to_file(1, 1, "cpu0_freq[MHz];"
			      "cpu1_freq[MHz];"
			      "cpu2_freq[MHz];"
			      "cpu3_freq[MHz]");
	}
#if 0
	if ((pm_soc_id == PM_SOC_LGM) && (mask & CPU_FREQ_MSK)) {
		/****************** CPU Module Frequency name ****************/
		write_to_file(1, 1, "cpumod0_freq[MHz];"
			      "cpumod1_freq[MHz]");
	}
#endif
	/****************** Cooling Device cur_state Values ******************/
	if (mask & CDEV_MSK) {
		max_cdev_num = cdev_get_max_num();
		if (max_cdev_num == 0)
			goto pmicreg;

		for (i = 0; i < (unsigned int)max_cdev_num; i++) {
			name = sysfs_get_cdev_name(i);
			if (!name)
				continue;
			memset(buffer, 0, sizeof(buffer));
			if (strcmp(name, "pwm-fan") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_fan[0..8]");
				assert(ret > 0);
			} else if ((strcmp(name, "thermal-cpufreq-0") == 0) ||
				   (strcmp(name, "cpufreq-cpu0") == 0)) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_cpu0[0..6]");
				assert(ret > 0);
			} else if ((strcmp(name, "thermal-cpufreq-1") == 0) ||
				   (strcmp(name, "cpufreq-cpu1") == 0)) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_cpu1[0..6]");
				assert(ret > 0);
			} else if ((strcmp(name, "thermal-cpufreq-2") == 0) ||
				   (strcmp(name, "cpufreq-cpu2") == 0)) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_cpu2[0..6]");
				assert(ret > 0);
			} else if ((strcmp(name, "thermal-cpufreq-3") == 0) ||
				   (strcmp(name, "cpufreq-cpu3") == 0)) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_cpu3[0..6]");
				assert(ret > 0);
			} else if (strcmp(name, "adp") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_adp[0..1]");
				assert(ret > 0);
			} else if (strcmp(name, "eth_thermal") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev_eth[0..4]");
				assert(ret > 0);
			} else if (strcmp(name, "cdev-wave_6_0-therm") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev-wave_2_4[0..3]");
				assert(ret > 0);
			} else if (strcmp(name, "cdev-wave_5_0-therm") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev-wave_5[0..3]");
				assert(ret > 0);
			} else if (strcmp(name, "cdev-wave_2_4-therm") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "cdev-wave_6[0..3]");
				assert(ret > 0);
			}
			write_to_file(1, 1, buffer);
			sysfs_put_cdev_name(name);
		}
	}
pmicreg:
#if 0
	if ((pm_soc_id == PM_SOC_LGM) && (mask & PMIC_VOLT_MSK)) {
		/****************** PMIC regulator name **********************/
		max_reg_num = reg_get_max_num();
		if (max_reg_num == 0)
			goto msr;
		for (i = 0; i < (unsigned int)max_reg_num; i++) {
			name = sysfs_get_reg_name(i);
			if (!name)
				continue;

			memset(buffer, 0, sizeof(buffer));
			if (strcmp(name, "cpu0") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "pmic_cpum0[mV]");
				assert(ret > 0);
			} else if (strcmp(name, "cpu1") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "pmic_cpum1[mV]");
				assert(ret > 0);
			} else if (strcmp(name, "adp") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "pmic_adp[mV]");
				assert(ret > 0);
			} else if (strcmp(name, "roc") == 0) {
				ret = sprintf_s(buffer, BUF_SIZE - 1, "pmic_roc[mV]");
				assert(ret > 0);
			} else {
				sysfs_put_reg_name(name);
				continue;
			}
			write_to_file(1, 1, buffer);
			sysfs_put_reg_name(name);
		}
	}
msr:
#endif
	/****************** MSR name *****************************************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & MSR_MSK)) {
		if (system("which rdmsr > /dev/null 2>&1"))
			goto cpu_load;

		write_to_file(1, 1, "msr198_3[hex];msr198_1[hex];msr198_2[hex];msr198_0[hex];"
				    "msr199_3[hex];msr199_1[hex];msr199_2[hex];msr199_0[hex]");
	}
cpu_load:
	/****************** CPU load *****************************************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & CPULOAD_MSK))
		write_to_file(1, 1, "cpuload[%]");
	/****************** ADP Level ****************************************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & ADP_LVL_MSK)) {
		ret0 = system("cat /sys/devices/soc0/e0180000.epu/adp_dvfs_level > /dev/null 2>&1");
		ret1 = system("cat /sys/devices/platform/soc/e0180000.epu/adp_dvfs_level > /dev/null 2>&1");
		if (ret0 && ret1)
			goto wav;

		write_to_file(1, 1, "adp_lvl[LOW=0,MED=1,HIGH=2];"
				    "swadp_lvl[LOW=0,MED=1,HIGH=2];"
				    "hwadp_cfg[OFF=0,ON=1]");
	}
wav:
	/****************** WAV num_ant ***********************************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & WAV_MSK)) {
		write_to_file(1, 1, "wav24g_coc[0..1];wav24g_tx_ant[1..4];wav24g_rx_ant[1..4];wav24g_peer[];wav24g_rxt[];wav24g_txt[]");
		write_to_file(1, 1, "wav5g_coc[0..1];wav5g_tx_ant[1..4];wav5g_rx_ant[1..4];wav5g_peer[];wav5g_rxt[];wav5g_txt[]");
		write_to_file(1, 1, "wav6g_coc[0..1];wav6g_tx_ant[1..4];wav6g_rx_ant[1..4];wav6g_peer[];wav6g_rxt[];wav6g_txt[]");
	}
	/****************** ETH Interfaces (link and EEE status) ************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & ETH_MSK)) {
		write_to_file(1, 1, "eth0_2;eth0_2_speed;eth0_2_rxt;eth0_2_txt;"
				    "eth0_3;eth0_3_speed;eth0_3_rxt;eth0_3_txt;"
				    "eth0_4;eth0_4_speed;eth0_4_rxt;eth0_4_txt");
		write_to_file(1, 1, "eth0_5;eth0_5_speed;eth0_5_rxt;eth0_5_txt;"
				    "eth0_1;eth0_1_speed;eth0_1_rxt;eth0_1_txt;"
				    "eth1;eth1_speed;eth1_rxt;eth1_txt;"
					"VANI0;VANI0_speed;VANI0_rxt;VANI0_txt");
	}

	/****************** TOP_NOC Frequency *******************************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & TOP_NOC_MSK))
		write_to_file(1, 1, "top_noc");

	/****************** PCIE ASPM Status ********************************/
	if ((pm_soc_id == PM_SOC_LGM) && (mask & PCIE_MSK)) {
		for (i = 1; i <= cnt; i++) {
			sprintf_s(buffer, BUF_SIZE - 1, "pcie_lnk%d_port", i);
			write_to_file(1, 1, buffer);
			sprintf_s(buffer, BUF_SIZE - 1, "pcie_lnk%d_width", i);
			write_to_file(1, 1, buffer);
			sprintf_s(buffer, BUF_SIZE - 1, "pcie_lnk%d_speed", i);
			write_to_file(1, 1, buffer);
			sprintf_s(buffer, BUF_SIZE - 1, "pcie_lnk%d_l0s", i);
			write_to_file(1, 1, buffer);
			sprintf_s(buffer, BUF_SIZE - 1, "pcie_lnk%d_l1", i);
			write_to_file(1, 1, buffer);
		}
	}

	write_to_file(1, 1, "EOH");
	write_to_file(1, 1, "\r\n");
}

static void signal_handler(int signal)
{
	printf("abnormal termination of pm_util: %d\n", signal);
	if (loop_break == 0)
		loop_break = 1;
	else
		abort();
}

int pm_logger(unsigned int r_period, char *log_path,
	      unsigned int log_mask, unsigned int flag,
	      uint16_t s_port)
{
	char f_time[40];
	time_t rawtime, rawtime_stop = 0;
	struct timeval time_start_cpu_load;
	struct timeval time_start_wifi0_tp; /* wifi throughput */
	struct timeval time_start_wifi2_tp; /* wifi throughput */
	struct timeval time_start_wifi4_tp; /* wifi throughput */
	struct timeval time_start_eth0_2_tp = {0}; /* wifi throughput */
	struct timeval time_start_eth0_3_tp = {0}; /* wifi throughput */
	struct timeval time_start_eth0_4_tp = {0}; /* wifi throughput */
	struct timeval time_start_eth0_5_tp = {0}; /* wifi throughput */
	struct timeval time_start_eth0_1_tp = {0}; /* wifi throughput */
	struct timeval time_start_eth1_tp = {0}; /* wifi throughput */
	struct timeval time_start_pon_tp = {0}; /* wifi throughput */

	unsigned int i;
	char buffer[BUF_SIZE];
	int max_tzone_num = tzone_get_max_num();
	int max_hwmon_num = hwmon_get_max_num();
	int max_cdev_num;
//	int max_reg_num;
	int pcie_num;
	int ret;
	char *name;
	char filename[BUF_SIZE];
	unsigned int time_taken;

	int c = sizeof(struct sockaddr_in);
//	int tempsocket;
	struct sockaddr_in server, client = {0};
	int success = 0;
	int s_flag = 1; /* Enable TCP_NODELAY */
	int c_flag = 1; /* Enable TCP_CORK */
	int r_flag = 1; /* Enable reuse of socket address */
	struct linger so_linger;

	so_linger.l_onoff = 1;
	so_linger.l_linger = 2; /* value in sconds until the socket can be reused */

	signal(SIGINT, signal_handler);
	sel_sock = flag;
	if (sel_sock == LOG2FILE) {
		get_time(f_time);
		time(&rawtime);
		rawtime_stop = rawtime + (log_time);
		memset(buffer, 0, sizeof(buffer));
		memset(filename, 0, sizeof(filename));
		if (sprintf_s(filename, BUF_SIZE - 1,
			      "%spm_log_%s.csv", log_path, f_time) <= 0) {
			printf("customized log file sprintf_s failed\n");
			printf("filename: %s", filename);
			return -1;
		}
		fp = fopen(filename, "a");
		if (!fp) {
			printf("fopen error! /tmp\n");
			return -1;
		}
	} else {
		tempsocket = socket(AF_INET, SOCK_STREAM, 0);
		memset(&server, '0', sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(s_port);
		bind(tempsocket, (struct sockaddr *)&server, sizeof(server));
		success = listen(tempsocket, 10);
		assert(success >= 0);
		printf("Listening on port %d for incoming client connection...\n", s_port);
		fp_sock = accept(tempsocket, (struct sockaddr *)&client,
				 (socklen_t *)&c);
		assert(fp_sock >= 0);
		printf("Accepted connection from IP: %s\n",
		       inet_ntoa(client.sin_addr));
		setsockopt(fp_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&s_flag,
			   sizeof(int));
		setsockopt(fp_sock, SOL_SOCKET, SO_REUSEADDR, &r_flag,
			   sizeof(r_flag));
		setsockopt(fp_sock, SOL_SOCKET, SO_LINGER, &so_linger,
			   sizeof(so_linger));

		/* allocate write buffer for write_to_file function */
		buf = (char *)malloc(BUF_SIZE_WRITE);
	}
	gettimeofday(&time_start_cpu_load, NULL);
	gettimeofday(&time_start_wifi0_tp, NULL);
	gettimeofday(&time_start_wifi2_tp, NULL);
	gettimeofday(&time_start_wifi4_tp, NULL);

	//pm_log_json();
	/* get the path's to the available pcie devices once to save time */
	pcie_num = pm_log_prepare_for_pcie_aspm();

/****************** create column names **************************************/
//	pcie_cnt = pm_log_column_pcie_aspm();
	pm_log_column_name(log_mask, pcie_num, max_tzone_num, max_hwmon_num);
	sleep(1);
	loop_break = 0;

	for (;;) {
		struct tm *time_str_tm;
		struct timeval time_now, time_end;

		if (sel_sock == LOG2SOCKET) {
			c_flag = 1;
			setsockopt(fp_sock, IPPROTO_TCP, TCP_CORK,
				   (char *)&c_flag,
				   sizeof(int));
		}
		gettimeofday(&time_now, NULL);
		time_str_tm = gmtime(&time_now.tv_sec);
		if (time_str_tm) {
			ret = sprintf_s(buffer, 80, "%02i:%02i:%02i:%06i",
					time_str_tm->tm_hour,
					time_str_tm->tm_min,
					time_str_tm->tm_sec,
					time_now.tv_usec);
			assert(ret > 0);
			write_to_file(1, 1, buffer);
		}
/****************** Temperature Values ***************************************/
		if (log_mask & THERMAL_TEMP_MSK) {
			if (max_tzone_num > 0) {
				for (i = 0; i < (unsigned int)max_tzone_num; i++)
					pm_log_temp(i);
			}
		}
/****************** Power Rail Values  ***************************************/
		if ((log_mask & POWER_VOLT_MSK) ||
		    (log_mask & POWER_PWR_MSK) ||
		    (log_mask & PVT_VOLT_MSK)) {
//			printf("power values\n");
			if (max_hwmon_num > 0) {
				for (i = 0; i < (unsigned int)max_hwmon_num; i++) {
					name = sysfs_get_hwmon_name(i);
					if (!name)
						continue;
					if (strcmp(name, pm_power_monitor_name) == 0)
						pm_log_pwr(i, log_mask);
					if ((strcmp(name, pm_pvt_name) == 0) &&
					    (log_mask & PVT_VOLT_MSK))
						pm_log_pvt(i);
					sysfs_put_hwmon_name(name);
				}
			}
		}
/****************** CPU Frequency Values  ************************************/
		if (log_mask & CPU_FREQ_MSK) {
//			printf("cpufreq values\n");
			pm_log_freq();
		}
#if 0
/****************** CPU Module Frequency Values ******************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & CPU_FREQ_MSK))
			pm_log_mod_freq();
#endif
/****************** Cooling Device cur_state Values **************************/
		if (log_mask & CDEV_MSK) {
//			printf("cdev values\n");
			max_cdev_num = cdev_get_max_num();
			if (max_cdev_num > 0) {
				for (i = 0; i < (unsigned int)max_cdev_num; i++) {
					name = sysfs_get_cdev_name(i);
					if (!name)
						continue;
					pm_log_cdev(i);
					sysfs_put_cdev_name(name);
				}
			}
		}

#if 0
/****************** PMIC regulator name **************************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & PMIC_VOLT_MSK)) {
//			printf("pmic values\n");
			max_reg_num = reg_get_max_num();
			if (max_reg_num > 0) {
				for (i = 0; i < (unsigned int)max_reg_num; i++) {
					name = sysfs_get_reg_name(i);
					if (!name)
						continue;
					if ((strcmp(name, "cpu0") == 0) ||
					    (strcmp(name, "cpu1") == 0) ||
					    (strcmp(name, "adp") == 0) ||
					    (strcmp(name, "roc") == 0))
						pm_log_reg(i);
					sysfs_put_reg_name(name);
				}
			}
		}
#endif
/****************** CPU load *************************************************/
		if (log_mask & CPULOAD_MSK)
			pm_log_cpuload(&time_start_cpu_load);
/****************** ADP level ************************************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & ADP_LVL_MSK))
			pm_log_adp_lvl();
/****************** Read msr 0x199/198 Values ********************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & MSR_MSK))
			pm_log_msr();
/****************** Read WAVE inf ********************************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & WAV_MSK)) {
			pm_log_wav_num_ant("wlan0");
			pm_log_wav_peers_connected("wlan0.1");
			pm_log_wav_throughput("wlan0.1", 0, &time_start_wifi0_tp);
			pm_log_wav_num_ant("wlan2");
			pm_log_wav_peers_connected("wlan2.2");
			pm_log_wav_throughput("wlan2.2", 2, &time_start_wifi2_tp);
			pm_log_wav_num_ant("wlan4");
			pm_log_wav_peers_connected("wlan4.3");
			pm_log_wav_throughput("wlan4.3", 4, &time_start_wifi4_tp);
		}
/****************** Read ethernet status *************************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & ETH_MSK)) {
			pm_log_eth("eth0_2");
			pm_log_eth_throughput("eth0_2", 2, &time_start_eth0_2_tp);
			pm_log_eth("eth0_3");
			pm_log_eth_throughput("eth0_3", 3, &time_start_eth0_3_tp);
			pm_log_eth("eth0_4");
			pm_log_eth_throughput("eth0_4", 4, &time_start_eth0_4_tp);
			pm_log_eth("eth0_5");
			pm_log_eth_throughput("eth0_5", 5, &time_start_eth0_5_tp);
			pm_log_eth("eth0_1");
			pm_log_eth_throughput("eth0_1", 1, &time_start_eth0_1_tp);
			pm_log_eth("eth1");
			pm_log_eth_throughput("eth1", 10, &time_start_eth1_tp);
			pm_log_eth("VANI0");
			pm_log_eth_throughput("VANI0", 20, &time_start_pon_tp);
		}
/****************** TOP_NOC_FREQ *********************************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & TOP_NOC_MSK))
			pm_log_top_noc();
/****************** TOP_PCIE_ASPM ********************************************/
		if ((pm_soc_id == PM_SOC_LGM) && (log_mask & PCIE_MSK))
			pm_log_pcie_aspm(pcie_num);
/****************** END of Log ***********************************************/
		write_to_file(1, 1, "EOD");
		write_to_file(1, 1, "\r\n");

		if (sel_sock == LOG2FILE) {
			if (time(&rawtime) > rawtime_stop)
				break;
		} else {
			c_flag = 0;
			setsockopt(fp_sock, IPPROTO_TCP, TCP_CORK,
				   (char *)&c_flag, sizeof(int));
		}

		gettimeofday(&time_end, NULL);
		time_taken = (time_end.tv_sec - time_now.tv_sec) * 1e6;
		time_taken = time_taken + (time_end.tv_usec - time_now.tv_usec);
		//printf("time_taken[usec]: %d\n", time_taken);

		if ((r_period * 1000) > (time_taken / 1000))
			usleep((r_period * 1000) - (time_taken / 1000));

		/* check end of loop */
		if (loop_break == 1) {
			loop_break = 0;
			break;
		}
	}
	write_to_file(1, 1, "QUIT");
	write_to_file(0, 1, "\r\n");
	if (sel_sock == LOG2FILE) {
		fclose(fp);
	} else {
		shutdown(tempsocket, SHUT_RDWR);
		close(fp_sock);
		close(tempsocket);
	}
	if (buf)
		free(buf);
	return 0;
}
