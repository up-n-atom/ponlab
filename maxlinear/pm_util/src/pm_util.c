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

#ifdef HAVE_CONFIG_H
#include "pm_util_config.h"
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/ioctl.h>
#include <safe_str_lib.h>
#include "include/ltq_pmutil_interface.h"

#define LINE_LEN		10
#define HWMON_NUM_MAX		20
#define TZONE_NUM_MAX		20
#define CDEV_NUM_MAX		20
#define REG_NUM_MAX		20
#define PWR_MAX_SELECTION	20
#define HWMON_TS_NUM_MAX	10
#define TS_LABEL_MAX		20
#define SMALL_BUF_SIZE		20
#define MEDIUM_BUF_SIZE		60
#define STD_BUF_SIZE		100
#define SPEED_MONITOR_NAME	"spdmon"
#define RESTART "\n\n******** Average Counter has been restartet ********\n\n\n"

/*function select*/
#define PM_DISPLAY_ALL		0x01
#define NOT_USED		0x02
#define PM_REGULATOR		0x03
#define PM_TEMP_SENSOR		0x04
#define PM_WIFI			0x05
#define PM_CONTROL		0x06
#define PM_POWER_MONITOR	0x08
#define PM_AVERAGE_PWR		0x09
#define PM_GOVERNOR		0x10
#define PM_FREQUENCY		0x20
#define PM_ENABLE_DYN_FS	0x100
#define PM_COREVOLT		0x104
#define PM_ENABLE_FORCE_DS	0x120
#define PM_ENABLE_LTQ_ALERT	0x130
#define PM_LOGGER		0x132
#define PM_DISABLE_STATE	0x134
#define PM_MAXFREQUENCY		0x138
#define PM_PMIC			0x140
#define PM_BITFIELD_MASK	0x141

#define PM_SET			0x1
#define PM_LOG			0x1
#define NORM_FREQ_LEN		32
#define PM_STAR "************************************************************"\
		"*********"
#define ETEMPINVAL		-255
#define UNDEF			0

#define ARRAY_SIZE(_a)	(sizeof((_a)) / sizeof((_a)[0]))

enum lgm_soc_ver {
	PM_SOC_VER_UNKNOWN,
	PM_SOC_VER_A2,
	PM_SOC_VER_A3,
	PM_SOC_VER_B0,
};

/******************************************************************************
 * MODULE GLOBAL VARIABLES
 *****************************************************************************/
static int c_opt_g;	/* < parameter(option) fetch from command list */
unsigned int update_time_g;
static unsigned int average_time;
unsigned int log_time = 60; /* seconds */
static int slc_i;
static int param_log;
static int selection[PWR_MAX_SELECTION];
static int func_select;
static int param_set;
static char wifi_name[10];
static char numb_ant[3];
static char pm_control_device[10];
static char pm_level[3];
int pm_soc_id;
static const char *pm_soc_id_str;
static char reg_name[SMALL_BUF_SIZE];
static unsigned int reg_min_uV;
static unsigned int reg_max_uV;

static char *POWER_MONITOR_NAME[] = {
	"ina219",
	"ina226",
	"ltc2945",
};

static char *LGM_BOARD_NAME[] = {
	"lgp",
	"lgb",
	"octopus",
};

static char *ADP_LEVEL[] = {
	"HIGH",
	"MED",
	"LOW",
};

static char *ADP_HW_DFS[] = {
	"OFF",
	"ON",
};

char *pm_power_monitor_name;
static char *pm_pwm_name;
char *pm_pvt_name;
static unsigned int pm_adp_path_index;

static void pm_regulator(void);
static void pm_wifi(void);

static void print_header(void)
{
	printf("\n");
	printf(PM_STAR "\n");
	printf("*                         System Power Info\n");
	printf(PM_STAR "\n");
	printf("\n");
}

static void print_footer(void)
{
	printf(PM_STAR "\n");
	printf("\n");
}

static void print_version(void)
{
	printf(PACKAGE_VERSION "\n");
}

/*
 *	pm_util help menu.
 *	Shows an overview about the parameters and the options which are
 *	supported by the pm_util
 *
 *	return None
 */
static void pm_util_help(void)
{
	printf("\n");
	printf(PACKAGE_NAME ", Version " PACKAGE_VERSION "\n");
	printf("Usage:pm_util [options] [parameter]\n");
	printf("\n");
	printf("options:\n");
	printf("-h --help	Display help information\n");
	printf("-v --version	Display pm_util version information\n");
	printf("-a --all	Display all available power related system info\n");
	printf("-t --temp	Display temperature sensor value\n");
	printf("-r --refresh	Set refresh time[msec]; could be set in addition to -a, -t, -p\n");
	printf("-e		shows device's power mode status: parameter is the device name\n");
	printf("-e... -i	enable/disable different power modes; parameter: 0-3\n");
	printf("		more precise description under -ehelp\n");
	printf("-f --freq	display cpu frequency for all cores [kHz]\n");
	printf("		use -sf<freq> <cpuid> to set cpu frequency\n");
	printf("		if no <cpuid> is given, set ALL cores, cpuid[0,1,2,3]\n");
	printf("-m --maxfreq	display max cpu frequency for all cores [kHz]\n");
	printf("		use -sm<freq> <cpuid> to set max cpu frequency\n");
	printf("		if no <cpuid> is given, set ALL cores, cpuid[0,1,2,3]\n");
	printf("-g --gov	display cpu frequency governor for all cores\n");
	printf("		use -sg<gov> <cpuid> to set governor\n");
	printf("		if no <cpuid> is given, set ALL cores, cpuid[0,1,2,3]\n");
	printf("-l --clk	Display clk/clk_summary\n");
	printf("-L[n] --log	Use together with functions -P -t, -k\n"
		"		to write corresponding values to file(csv-formatted)\n"
		"		add Parameter to determine stop time in min (default = 60)\n"
		"		logfile stored in /tmp by default or use -D option to define new path.\n"
		"		filename(s) : [ts]_log_<timestamp>.csv\n");
	printf("-D[dir] --fdir	additional log option to define new log-file directory other than /tmp/\n"
		"		usage: -D/tmp/mounts/USB-A/\n"
		"		this is useful if you want to log over longer time to an external USB device\n");
	printf("-M[m] --lmsk	Only in conjunction with -L option\n"
		"		This is an optional parameter to configure which information\n"
		"		should be logged. If nothing is given all available information will be logged.\n"
		"		Every bit represents a class of log-info. If the bit is set to '1' the class info will be logged.\n"
		"		The following groups exists:\n"
		"		- JUNCTION_TEMP [m°C]	: bit0	all available junction temperature values\n"
		"		- POWERRAIL [V]		: bit1	power rail info from the extra power monitors\n"
		"		- POWERRAIL [mW]	: bit2	power rail info from the extra power monitors\n");
	if (pm_soc_id == PM_SOC_LGM)
		printf("		- PVT [V]		: bit3	URX on-chip PVT sensor info (only voltage)\n");

	printf("		- CPUFREQ [kHz]		: bit4	CPU core frequency\n"
		"		- CDEV [step]		: bit5	Cooling device steps (0=cooling off)\n");
	if (pm_soc_id == PM_SOC_LGM) {
		printf("		- not used		: bit6	\n"
		"		- MSR [hex]		: bit7	URX only: ATOM msr register 198 and 199 (refer to -k); this needs extra binary to work\n"
		"		- CPU Load [%%]		: bit8	Linux cpu load\n");
	}
	if (pm_soc_id == PM_SOC_LGM) {
		printf("		- ADP Level [0,1,2]	: bit9	URX only: Current ADP Level\n"
			"		- WAV []		: bit10	URX only: Wav info (ant no, stat connected, throughput)\n"
			"		- Ethernet []		: bit11	URX only: Ethernet info (link status, speed, throughput)\n"
			"		- PCIe []		: bit12	URX only: ASPM info (port, width, speed, L0s, L1)\n"
			"		- TOP_NOC [HIGH|LOW]	: bit13	URX only: TOP_NOC frequency level\n");
	}
	printf("-S[p] --sock	additional log option to redirect output to tcp socket instead of File; default port is 49666\n"
		"		if a different port should be used it can be define by -S1234\n"
		"		without this option content will be logged into file. -L option has no function if -S is used.\n");
	printf("-k[p] --pmlog	logging system power information over time into file.\n"
		"		Use together with -L and -M. k require sampling rate in ms (-k1000).\n"
		"		Generates a csv-file format.\n"
		"		URX only: If the binary 'rdmsr' is available on the target under /opt/intel/bin\n"
		"		also msr198 and msr199 will be logged\n");
	if (pm_soc_id == PM_SOC_LGM) {
		printf("-d --idle	get/ena/dis ATOM Idle Power States (C1/C6NS/C6FS)\n");
		printf("		use -sd<cstate> <dis> <cpuid> to ena/dis a c-state\n");
		printf("		dis[1=disable, 0=enable], cstate[1=C1, 2=C6NS, 3=C6FS], cpuid[0,1,2,3]\n");
	}
	printf("-w --wifi	Display number of antennas in use, per wifi + wifi Tj[°C]\n");
	printf("-x		in combination with -sw: argument is the new number of active antennas\n");
	printf("		when a frequency change happens\n");
	printf("-p --pwr	Display power rail monitor values[P,V]\n");
	printf("-P[n] --avgpw	Display power rail average for n seconds\n");
	printf("		default n = 120sec. After this time avg restart's\n");
	printf("-z --select	Select specific rail values to be displayed.\n"
		"		Can ONLY be used with -P option!\n"
		"		If there is more than one power monitor,\n"
		"		select which one(s) should be displayed in average mode.\n"
		"		Use pm_util -p to display the available power monitors to get the\n"
		"		digit selectors	in the first column.\n"
		"		Please refer to the example section\n");
	printf("-s --set	This option must be set if parameters should ");
	printf("be changed.\n");
	printf("		The following parameters can be changed:\n");
	printf("			-f frequency\n");
	printf("			-m max frequency\n");
	printf("			-g governor\n");
	printf("			-w wifi: powersave mode/number of active antennas\n");
	if (pm_soc_id == PM_SOC_LGM) {
		printf("			-u regulator_name min_uV max_uV\n");
		printf("			-d ATOM C-State can be enabled (0) or disabled (1)\n");
		printf("			-o [CPUM0|CPUM1|ADP|ROC] voltage\n");
	}
	printf("-y --dyn	ena dynamic frequency scaling [0|1|off|on]\n");
	printf("-u --regulator	regulator options\n");
	if (pm_soc_id == PM_SOC_LGM)
		printf("-o --pmic	Display power rail voltage\n");
	printf("-j --mask	return bitfield value based on given integer, mask and shift right\n"
		"		0xabcd 0xF0 4  -> 12\n"
		"		0xabcd 0xF0 0  -> 13\n");
	printf("\n");
	printf("Examples:\n");
	printf("pm_util -ewlan0		shows current power mode status of wlan0\n");
	printf("pm_util -ewlan2 -i1	enables powersaving mode (AutoCoCMode) for wlan2\n");
	printf("pm_util -f		display cpu frequency of all cores\n");
	printf("pm_util -sf600000	set frequency to 600MHz on all cores\n");
	printf("pm_util -sf600000 1	set frequency to 600MHz for cpu1\n");
	printf("pm_util -m		display max cpu frequency of all cores\n");
	printf("pm_util -sm600000	set max frequency to 600MHz on all cores\n");
	printf("pm_util -sm600000 1	set max frequency to 600MHz for cpu1\n");
	printf("pm_util -sgpowersave	set governor to conservative on all cores\n");
	printf("pm_util -sgpowersave 1	set governor to conservative for cpu1\n");
	if (pm_soc_id == PM_SOC_LGM) {
		printf("pm_util -d		display ena/dis status for all c-states, all cores\n");
		printf("pm_util -sd1 1		disable C1 on all cores\n");
		printf("pm_util -sd2 0 1	enable C6ns on cpu1\n");
		printf("pm_util -sd2 0		enable C6ns on all cores\n");
		printf("pm_util -so CPUM0 800	set CPU module0 voltage to 800mv\n");
	}
	printf("pm_util -swwlan0	enable powersave mode for wlan0\n");
	printf("pm_util -swwlan0 -x4	disable powersave mode/set number of active antennas to 4\n");
	printf("pm_util -t		display temperature sensor value(s)\n");
	printf("pm_util -tr1000		display temperature sensor value(s) every 1s\n");
	printf("pm_util -tr1000 -L	display temperature every 1s & log-file\n");
	printf("pm_util -k1000 -L10 -M0x21	log T-junc + cooling device info only\n");
	printf("pm_util -k1000 -S	log into tcp socket\n");
	printf("pm_util -k1000 -L10 /tmp/mounts/USB-A/ log-file located at /tmp/mounts/USB-A/\n");
	printf("pm_util -P60 -r500	Averages over 60sec - sample rate 500ms\n");
	printf("pm_util -P60 -r500 -z 1 2 3 -L  Same, only displaying power\n"
		"			rails 1-3 and logging relevant data to file\n"
		"			Note that there has to be a blank\n"
		"			between selector and the first argument!\n");
	printf("pm_util -j0xabcd 0xF0 4\n");
	printf("\n");
}

struct freq_units {
	char	*str_unit;
	int	power_of_ten;
};

struct ts_data {
	int	tempid;
	char	label[TS_LABEL_MAX];
};

static const struct freq_units def_units[] = {
	{"hz", -3},
	{"khz", 0}, /* default */
	{"mhz", 3},
	{"ghz", 6},
	{"thz", 9},
	{NULL, 0}
};

struct pm_soc_type {
	const char	*needle;
	const int	type;
};

static void print_unknown_arg(void)
{
	printf("\ninvalid or unknown argument\n");
	pm_util_help();
}

static unsigned long string_to_frequency(const char *str)
{
	char normalized[NORM_FREQ_LEN];
	const struct freq_units *unit;
	const char *scan;
	char *end;
	unsigned long freq;
	int power = 0, match_count = 0, i, cp, pad;

	memset(normalized, 0, sizeof(normalized[NORM_FREQ_LEN]));
	while (*str == '0')
		str++;

	for (scan = str; isdigit(*scan) || *scan == '.'; scan++) {
		if (*scan == '.' && match_count == 0)
			match_count = 1;
		else if (*scan == '.' && match_count == 1)
			return 0;
	}

	if (*scan) {
		match_count = 0;
		for (unit = def_units; unit->str_unit; unit++) {
			if (strcmp(scan, unit->str_unit) == 0) {
				match_count = 1;
				power = unit->power_of_ten;
				break;
			}
		}
		if (match_count == 0)
			return 0;
	}

	/* count the number of digits to be copied */
	for (cp = 0; isdigit(str[cp]); cp++)
		continue;

	if (str[cp] == '.') {
		while (power > -1 && isdigit(str[cp + 1]))
			cp++, power--;
	}
	if (power >= -1)	/* not enough => pad */
		pad = power + 1;
	else			/* to much => strip */
		pad = 0, cp += power + 1;
	/* check bounds */
	if (cp <= 0 || cp + pad > NORM_FREQ_LEN - 1)
		return 0;

	/* copy digits */
	for (i = 0; i < cp; i++, str++) {
		if (*str == '.')
			str++;
		normalized[i] = *str;
	}
	/* and pad */
	for (; i < cp + pad; i++)
		normalized[i] = '0';

	if (i == 0)
		return 0;
	/* round up, down ? */
	match_count = (normalized[i - 1] >= '5');
	/* and drop the decimal part */
	normalized[i - 1] = 0; /* cp > 0 && pad >= 0 ==> i > 0 */

	/* final conversion (and applying rounding) */
	errno = 0;
	freq = strtoul(normalized, &end, 10);
	if (errno)
		freq = 0;
	else
		if (match_count && freq != ULONG_MAX)
			freq++;
	return freq;
}

static unsigned int count_cpus(void)
{
	FILE *fp;
	char value[LINE_LEN];
	unsigned int ret = 0;
	unsigned int cpunr = 0;

	fp = fopen("/proc/stat", "r");
	if (!fp) {
		printf("Couldn't count the number of CPUs, assuming 1\n");
		return 1;
	}

	while (!feof(fp)) {
		if (!fgets(value, LINE_LEN, fp))
			continue;
		value[LINE_LEN - 1] = '\0';
		if (strnlen_s(value, sizeof(value)) < (LINE_LEN - 2))
			continue;
		if (strstr(value, "cpu "))
			continue;
		if (sscanf_s(value, "cpu%12u ", &cpunr) != 1)
			continue;
		if (cpunr > ret)
			ret = cpunr;
	}
	fclose(fp);

	/* cpu count starts from 0, on error return 1 (UP) */
	return ret + 1;
}

static const struct pm_soc_type intel_soc_type[] = {
	{
		.needle = "GRX350",
		.type = PM_SOC_GRX350,
	}, {
		.needle = "GRX550",
		.type = PM_SOC_GRX550,
	}, {
		.needle = "PRX321",
		.type = PM_SOC_PRX321,
	}, {
		.needle = "PRX126",
		.type = PM_SOC_PRX126,
	}, {
		.needle = "lgp",
		.type = PM_SOC_LGM,
	}, {
		.needle = "octopus-641",
		.type = PM_SOC_LGM,
	}, {
		.needle = "octopus-851",
		.type = PM_SOC_LGM,
	}, {
		.needle = "osp-tb341",
		.type = PM_SOC_LGM,
	}, {
		.needle = "osp-tb341-v2",
		.type = PM_SOC_LGM,
	},
};

#define DTINFO_BUFSIZE		128	/* lines of interest are < 80 chars */
static void pm_find_soc(void)
{
	FILE *fp;
	char temp[DTINFO_BUFSIZE];
	unsigned int i;

	pm_power_monitor_name = POWER_MONITOR_NAME[0];
	pm_pwm_name = "UNKNOWN";
	pm_pvt_name = "UNKNOWN";
	pm_adp_path_index = 0;
	pm_soc_id_str = "UNKNOWN";
	system("grep , /proc/device-tree/model  1> /tmp/plat.txt 2> /dev/null");
	fp = fopen("/tmp/plat.txt", "r");
	if (!fp)
		return;

	/* first, extract the system type line */
	fgets(temp, DTINFO_BUFSIZE, fp);
	fclose(fp);

	/* then, try to identify known SoC, stop at first match */
	for (i = 0; i < ARRAY_SIZE(intel_soc_type); i++) {
		if ((strstr(temp, intel_soc_type[i].needle))) {
			pm_soc_id = intel_soc_type[i].type;
			pm_soc_id_str = intel_soc_type[i].needle;
			break;
		}
	}
	switch (pm_soc_id) {
	case PM_SOC_GRX350:
	case PM_SOC_GRX550:
	case PM_SOC_PRX321:
	case PM_SOC_PRX126:
		pm_power_monitor_name = POWER_MONITOR_NAME[0];
		break;
	case PM_SOC_LGM:
		for (i = 0; i < ARRAY_SIZE(LGM_BOARD_NAME); i++) {
			if ((strstr(temp, LGM_BOARD_NAME[i]))) {
				if (i == 2)
					pm_power_monitor_name = POWER_MONITOR_NAME[2];
				else
					pm_power_monitor_name = POWER_MONITOR_NAME[1];
				break;
			}
		}
		pm_pwm_name = "pwmfan";
		pm_pvt_name = "pvt";
		pm_adp_path_index = 1;
		break;
	default:
		pm_power_monitor_name = POWER_MONITOR_NAME[0];
		break;
	}
}

static void print_speed(unsigned long speed)
{
	unsigned long tmp;

	if (speed > 1000000) {
		tmp = speed % 10000;
		if (tmp >= 5000)
			speed += 10000;
		printf("%u.%02u GHz", ((unsigned int)speed / 1000000),
		       ((unsigned int)(speed % 1000000) / 10000));
	} else if (speed > 100000) {
		tmp = speed % 1000;
		if (tmp >= 500)
			speed += 1000;
		printf("%u MHz", ((unsigned int)speed / 1000));
	} else if (speed > 1000) {
		tmp = speed % 100;
		if (tmp >= 50)
			speed += 100;
		printf("%u.%01u MHz", ((unsigned int)speed / 1000),
		       ((unsigned int)(speed % 1000) / 100));
	} else {
		printf("%lu kHz", speed);
	}
}

int reg_get_max_num(void)
{
	unsigned int i;

	for (i = 0; i < REG_NUM_MAX; i++) {
		if (!sysfs_reg_exists(i)) {
			if (i == 0)
				return -1;
			return i;
		}
	}
	return -1;
}

int hwmon_get_max_num(void)
{
	unsigned int i;

	/* check number of hwmon's */
	for (i = 0; i < HWMON_NUM_MAX; i++) {
		if (sysfs_hwmon_exists(i) < 0) {
			if (i == 0)
				return -1;
			return i;
		}
	}
	return -1;
}

int cdev_get_max_num(void)
{
	unsigned int i;

	/* check number of cooling_device's */
	for (i = 0; i < CDEV_NUM_MAX; i++) {
		if (sysfs_cdev_exists(i) < 0) {
			if (i == 0)
				return -1;
			return i;
		}
	}
	return -1;
}

int tzone_get_max_num(void)
{
	unsigned int i;

	/* check number of hwmon's */
	for (i = 0; i < TZONE_NUM_MAX; i++) {
		if (sysfs_tzone_exists(i) < 0) {
			if (i == 0)
				return -1;
			return i;
		}
	}
	return -1;
}

static char oldstamp[STD_BUF_SIZE];
static void write_to_file(char *newstamp, FILE *fp, int count, ...)
{
	va_list arg_ptr;
	char *arg;
	int i;

	if (!fp)
		return;

	va_start(arg_ptr, count);

	if (strcmp(oldstamp, newstamp) == 0) {
		for (i = 0; i < count; i++) {
			arg = va_arg(arg_ptr, char*);
			fprintf(fp, "%s;", arg);
		}
	} else {
		if (strncpy_s(oldstamp, sizeof(oldstamp), newstamp,
			      (sizeof(oldstamp)) - 1) == 0) {
			oldstamp[(sizeof(oldstamp)) - 1] = '\0';
			fprintf(fp, "%s;", newstamp);
			for (i = 0; i < count; i++) {
				arg = va_arg(arg_ptr, char*);
				fprintf(fp, "%s;", arg);
			}
		}
	}
	va_end(arg_ptr);
}

static void pm_print_temp(unsigned int i, char *name, char *buf, FILE *fp)
{
	long temp;
	struct tz_data tz[TZONE_TRIP_MAX];
	char buffer[MEDIUM_BUF_SIZE];

	memset(tz, 0, sizeof(tz));
	temp = sysfs_get_tzone_temp(i);
	if (temp == ETEMPINVAL)
		return;
	sysfs_get_tzone_trip(i, tz);
	printf("%sThermal Zone    (%-19s) ", buf, name);
	printf("temp: %3ld.%d C  ", temp / 1000, abs((int)temp % 1000) / 100);
	if (param_log == PM_LOG && fp) {
		if (sprintf_s(buffer, MEDIUM_BUF_SIZE - 1,
			      "Thermal Zone %12s:;%6ld", name, temp) <= 0) {
			printf("sprintf_s failed\n");
			return;
		}
		write_to_file(buf, fp, 1, buffer);
	}
	int j = 0;

	while (strcmp(tz[j].type, "\0") != 0) {
		printf("trip%d: %3ld.%d C %-8s  ", j,
		       tz[j].temp / 1000,
		       abs((int)tz[j].temp % 1000) / 100,
		       tz[j].type);
		j++;
	}
	printf("\n");
}

int get_epu_status(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char adp_pm_status[30];
	char adp_pm_control[30];
	long regadp, regadpc;
	int idx=0;

	system("io -4 0xe0180020 1> /tmp/epu_status.txt 2> /dev/null");
	system("io -4 0xe018001C 1>> /tmp/epu_status.txt 2> /dev/null");
	stat("/tmp/epu_status.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/epu_status.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}

	if (!fgets(adp_pm_status, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(adp_pm_control, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	str = strtok(adp_pm_status, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regadp = strtol(str, &p_end, 16);

	str = strtok(adp_pm_control, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regadpc = strtol(str, &p_end, 16);

	printf("\n");
	printf("ADP PM STATUS:\n");
	idx = (regadp & 0x7) >> 0;
	if( idx >= 0 && idx < 3 )
		printf("CURRENT_DVFS_LEVEL : %s\n", ADP_LEVEL[idx]);
	printf("\n");
	printf("ADP PM CONTROL:\n");
	idx = (regadpc & 0xe) >> 1;
	if( idx >= 0 && idx < 3 )
		printf("HW_DFS_ENABLE      : %s   SW_DVFS_LEVEL_REQ : %s\n", ADP_HW_DFS[(regadpc & 0x1) >> 0], ADP_LEVEL[idx]);
	printf("\n\n");
	return 0;
}

int get_p34x_gphy_pd_lp_status(unsigned int id, char *pd, char *lp)
{
	struct stat statbuf;
	char buffer[STD_BUF_SIZE];
	FILE *fp = NULL;
	char pd_gphy_name[20];
	char lp_gphy_name[20];
	char *str;

	if (sprintf_s(pd_gphy_name, 17, "PowerDown: gphy%d", id) <= 0) {
		printf("sprintf_s pd_gphy_name failed\n");
		return -1;
	}
	if (sprintf_s(lp_gphy_name, 17, "LowPower:  gphy%d", id) <= 0) {
		printf("sprintf_s lp_gphy_name failed\n");
		return -1;
	}

	if (sprintf_s(buffer, STD_BUF_SIZE - 1,
		      "cat /sys/devices/platform/soc/soc:p34x@0/phy_control 1> /tmp/p34x_pd_status.txt 2> /dev/null") <= 0) {
		printf("sprintf_s phy_control failed\n");
		return -1;
	}
	system(buffer);
	stat("/tmp/p34x_pd_status.txt", &statbuf);
	if (statbuf.st_size == 0)
		return -1;
	fp = fopen("/tmp/p34x_pd_status.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return -1;
	}

	while (!feof(fp)) {
		if (!fgets(buffer, STD_BUF_SIZE, fp))
			continue;
		buffer[STD_BUF_SIZE - 1] = '\0';
		str = strstr(buffer, pd_gphy_name);
		if (str != 0) {
			if (strcpy_s(pd, 15, (str + 17)) != 0)
				printf("strcpy failed\n");
		}
		str = strstr(buffer, lp_gphy_name);
		if (str != 0) {
			if (strcpy_s(lp, 15, (str + 17)) != 0)
				printf("strcpy failed\n");
		}
	}
	fclose(fp);
	return 0;
}

int get_p34x_gphy_status(void)
{
	char pd_gphy[20];
	char lp_gphy[20];

	get_p34x_gphy_pd_lp_status(0, pd_gphy, lp_gphy);
	printf("P34X GPHY STATUS:\n");
	printf("eth0_2:\n");
	printf("        LP mode %s", lp_gphy);
	printf("        PD mode %s", pd_gphy);
	system("ethtool --show-eee eth0_2 | grep 'EEE status' 2> /dev/null");
	system("ethtool eth0_2 | grep 'Link detected'  2> /dev/null");
	printf("\n");

	get_p34x_gphy_pd_lp_status(1, pd_gphy, lp_gphy);
	printf("eth0_3:\n");
	printf("        LP mode %s", lp_gphy);
	if (strcmp(lp_gphy, "active\n") != 0) {
		printf("        PD mode %s", pd_gphy);
		system("ethtool --show-eee eth0_3 | grep 'EEE status' 2> /dev/null");
		system("ethtool eth0_3 | grep 'Link detected'  2> /dev/null");
		printf("\n");
	}
	get_p34x_gphy_pd_lp_status(2, pd_gphy, lp_gphy);
	printf("eth0_4:\n");
	printf("        LP mode %s", lp_gphy);
	if (strcmp(lp_gphy, "active\n") != 0) {
		printf("        PD mode %s", pd_gphy);
		system("ethtool --show-eee eth0_4 | grep 'EEE status' 2> /dev/null");
		system("ethtool eth0_4 | grep 'Link detected'  2> /dev/null");
		printf("\n");
	}
	get_p34x_gphy_pd_lp_status(3, pd_gphy, lp_gphy);
	printf("eth0_5:\n");
	printf("        LP mode %s", lp_gphy);
	if (strcmp(lp_gphy, "active\n") != 0) {
		printf("        PD mode %s", pd_gphy);
		system("ethtool --show-eee eth0_5 | grep 'EEE status' 2> /dev/null");
		system("ethtool eth0_5 | grep 'Link detected'  2> /dev/null");
		printf("\n");
	}
	return 0;
}

static int pm_display_all(void)
{
	unsigned int i;
	struct cpufreq_available_governors *governors;
	struct cpufreq_available_frequencies *freqs;
	struct cpufreq_policy *policy;
	struct cpufreq_stats *stats;
	unsigned long total_trans;
	unsigned long long total_time;
	unsigned int cpu;
	char *spc;
	char *railname;
	char *name;
	int state;
	char *pwmname = "pwm1";
	int pwm = 0;
	unsigned long pvt_cpu_m0_voltage = 0, pvt_cpu_m1_voltage = 0;
	unsigned long pvt_roc_voltage = 0;
	char *pvt_cpu_m0_name = "cpu_m0_0V8";
	char *pvt_cpu_m1_name = "cpu_m1_0V8";
	char *pvt_roc_name = "roc_0V8";
	char *scaling;
	char *s_drv;
	char *cpuidle_driver = "idle driver";
	char *i_drv = 0;
	char *cpuidle_governor = "idle governor";
	char *i_gov = 0;
	char *i_cstate;
	unsigned long power, voltage;
	double a;
	int max_hwmon_num = hwmon_get_max_num();
	int max_tzone_num = tzone_get_max_num();
	int max_cdev_num = cdev_get_max_num();
	int ivoice_exist;
	int retdatapath;

	print_header();
	printf("SoC: %s : ", pm_soc_id_str);
	printf("pm_util version: ");
	print_version();
	printf("\n");

	for (;;) {
		unsigned int nr_cpus;
		unsigned long cpu_freq[4];
		unsigned long cpu_freq_s[4];
		unsigned long cpu_freq_max[4];
		/* Lantiq CPE SoC's support up to 4 CPU's until now */
		nr_cpus = count_cpus();
		if (nr_cpus > 4)
			nr_cpus = 1;

		for (cpu = 0; cpu < nr_cpus; cpu++) {
			/*get current frequency*/
			cpu_freq[cpu] = cpufreq_get_freq_hardware(cpu) / 1000;
			cpu_freq_s[cpu] = cpufreq_get_freq_scaling(cpu) / 1000;
			cpu_freq_max[cpu] = cpufreq_get_max_freq(cpu) / 1000;
		}
		if (max_hwmon_num > 0) {
			for (i = 0; i < (unsigned int)max_hwmon_num; i++) {
				name = sysfs_get_hwmon_name(i);

				if (!name)
					continue;

				if (strcmp(name, SPEED_MONITOR_NAME) == 0) {
					spc = sysfs_get_hwmon_speed_class_boot(i);
					if (spc) {
						printf("Speed_Monitor   %s %10s:\n\n",
						       name, spc);
						sysfs_put_hwmon_speed_class_boot(spc);
					}
				}
				if (strcmp(name, pm_power_monitor_name) == 0) {
					power = sysfs_get_hwmon_power(i);
					voltage = sysfs_get_hwmon_rail_volt(i);
					railname = sysfs_get_hwmon_rail_name(i);
					if (railname) {
						printf("Power_Monitor   (%s %10s) ",
						       name, railname);
						printf("P[mW]: %5lu  ",
						       power / 1000);
						printf("V[mV]: %5lu\n",
						       voltage);
						sysfs_put_hwmon_rail_name(railname);
					}
				}
				if (strcmp(name, pm_pwm_name) == 0) {
					pwm = sysfs_get_hwmon_pwm(i);
					printf("Pwm_Monitor     (%-23s) %s: %u\n",
					       pm_pwm_name, pwmname, pwm);
				}

				if (strcmp(name, pm_pvt_name) == 0) {
					pvt_cpu_m0_voltage = sysfs_get_hwmon_pvt_cpu_m0(i);
					pvt_cpu_m1_voltage = sysfs_get_hwmon_pvt_cpu_m1(i);
					pvt_roc_voltage = sysfs_get_hwmon_pvt_roc(i);
					printf("Pvt_Monitor     (%-23s) ", pm_pvt_name);
					printf("%s[mV]: %lu  ", pvt_cpu_m0_name, pvt_cpu_m0_voltage);
					printf("%s[mV]: %lu  ", pvt_cpu_m1_name, pvt_cpu_m1_voltage);
					printf("%s[mV]: %lu\n", pvt_roc_name, pvt_roc_voltage);
				}
				if ((strcmp(pm_pwm_name, "UNKNOWN") == 0) || (strcmp(pm_pvt_name, "UNKNOWN") == 0))
					printf("\n");
				sysfs_put_hwmon_name(name);
			}
		}
		if (max_tzone_num > 0) {
			unsigned int found = 0;

			printf("\n");

			for (i = 0; i < (unsigned int)max_tzone_num; i++) {
				name = sysfs_get_tzone_name(i);

				if (!name)
					continue;

				found = 1;
				pm_print_temp(i, name, "", NULL);
				sysfs_put_tzone_name(name);
			}
			if (found == 0)
				printf("No Temperature Sensor found\n");
		}
		printf("\n");

		if (max_cdev_num > 0) {
			unsigned int found = 0;

			for (i = 0; i < (unsigned int)max_cdev_num; i++) {
				name = sysfs_get_cdev_name(i);

				if (!name)
					continue;
				found = 1;

				printf("Cooling_Device%u (%-19s) ", i, name);
				state = sysfs_get_cdev_cur_state(i);
				printf("cur_state: %2u  ", state);
				state = sysfs_get_cdev_max_state(i);
				printf("max_state: %2u\n", state);
				sysfs_put_cdev_name(name);
			}
			if (found == 0)
				printf("No Cooling Device found\n");
		}
		printf("\n");

		pm_regulator();
		printf("\n");

		/* common cpu information of cpuidle driver : get >> */
		if (sysfs_cpuidle_exists() == 0) {
			i_drv = sysfs_get_cpuidle_driver();
			i_gov = sysfs_get_cpuidle_governor();
		}
		/* common cpu information of cpuidle driver : get << */

		for (cpu = 0; cpu < nr_cpus; cpu++) {
			printf("CPU %u:\n", cpu);

			governors = cpufreq_get_available_governors(cpu);
			if (governors) {
				printf(" available governors: ");
				while (governors->next) {
					printf("%s, ", governors->governor);
					governors = governors->next;
				}
				printf("%s\n", governors->governor);
				cpufreq_put_available_governors(governors);
			}

			freqs = cpufreq_get_available_frequencies(cpu);
			if (freqs) {
				printf(" available frequency steps: ");
				while (freqs->next) {
					print_speed(freqs->frequency);
					printf(", ");
					freqs = freqs->next;
				}
				print_speed(freqs->frequency);
				printf("\n");
				cpufreq_put_available_frequencies(freqs);
			}
			printf(" current CPU frequency: [info %lu MHz] [scale %lu MHz] [max %lu MHz]\n",
			       cpu_freq[cpu], cpu_freq_s[cpu], cpu_freq_max[cpu]);

			policy = cpufreq_get_policy(cpu);
			if (policy) {
				printf(" cur governor: %s\n", policy->governor);
				cpufreq_put_policy(policy);
			}

			s_drv = cpufreq_get_scaling_driver(cpu);
			if (s_drv) {
				printf(" scaling driver: %s\n", s_drv);
				cpufreq_put_scaling_driver(s_drv);
			}

			if (sysfs_pstate_exists() == 0) {
				char *st = cpufreq_get_pstate_status(cpu);
				unsigned long a = sysfs_get_pstate_maxperf(cpu);
				unsigned long b = sysfs_get_pstate_minperf(cpu);
				unsigned long c = sysfs_get_pstate_noturbo(cpu);
				unsigned long d = sysfs_get_pstate_numpstates(cpu);
				unsigned long e = sysfs_get_pstate_turbo(cpu);

				printf("     (/sys/devices/system/cpu/intel_pstate/)\n");
				if (st)
					printf("     status      : %s (off, passive, active)\n", st);
				printf("     max_perf_pct: %lu\n", a);
				printf("     min_perf_pct: %lu\n", b);
				printf("     no_turbo    : %lu\n", c);
				printf("     num_pstates : %lu\n", d);
				printf("     turbo_pct   : %lu\n", e);
			}
			stats = cpufreq_get_stats(cpu, &total_time);
			if (stats) {
				printf(" cpufreq stats: ");
				while (stats) {
					print_speed(stats->frequency);
					a = ((100.0 * stats->time_in_state) /
						total_time);
					printf(":%.2f%%", a);
					stats = stats->next;
					if (stats)
						printf(", ");
				}
				cpufreq_put_stats(stats);
				total_trans = cpufreq_get_transitions(cpu);
				if (total_trans)
					printf("  (%lu)\n", total_trans);
				else
					printf("\n");
			}

			if (i_drv)
				printf(" %s: %s\n", cpuidle_driver, i_drv);
			if (i_gov)
				printf(" %s: %s\n", cpuidle_governor, i_gov);

			if (sysfs_cpu_cpuidle_exists(cpu) == 0) {
				char *path = "/sys/devices/system/cpu/cpu";

				i_cstate = sysfs_get_cstate_state(cpu);
				printf("     (%s%u/cpuidle/stateX)\n",
				       path, cpu);
				if (i_cstate) {
					printf("     state       : %s\n",
					       i_cstate);
					sysfs_put_cstate(i_cstate);
				}
				i_cstate = sysfs_get_cstate_disable(cpu);
				if (i_cstate) {
					printf("     disable     :         %s\n", i_cstate);
					sysfs_put_cstate(i_cstate);
				}
				printf("\n");
			}
			printf("\n");
		}
		/* common cpu information of cpuidle driver : put >> */
		if (i_drv)
			sysfs_put_cpuidle_driver(i_drv);
		if (i_gov)
			sysfs_put_cpuidle_governor(i_gov);
		/* common cpu information of cpuidle driver : put << */

		scaling = cpufreq_get_ltq_scaling_clks(0);
		if (scaling)
			printf("%s\n\n", scaling);

		cpufreq_put_ltq_scaling_clks(scaling);
		pm_wifi();
		if (pm_soc_id == PM_SOC_LGM) {
			get_epu_status();
			get_p34x_gphy_status();
		}

		if (pm_soc_id != PM_SOC_LGM) {
			printf("CPUFreq registered modules:\n");
			retdatapath = system("grep dp_coc_ena=1 /sys/kernel/debug/dp/coc 1> /dev/null 2> /dev/null");
			if (retdatapath == -1) {
				printf("System command failed!\n");
			} else {
				if (retdatapath == 0)
					printf("Datapath: registered\n");
				else
					printf("Datapath: none\n");
			}
			ivoice_exist = access("/proc/driver/vmmc/power", R_OK);
			if (ivoice_exist == -1)
				printf("Voice   : none\n");
			else
				printf("Voice   : registered\n");
			printf("\n");
		}
		print_footer();
		if (update_time_g == 0)
			return 0;

		usleep(update_time_g * 1000);
	}
}

static int pm_temp_sensor(char *f_time)
{
	unsigned int i, found = 0;
	char buf[STD_BUF_SIZE];
	time_t rawtime, rawtime_stop;
	int max_tzone_num = tzone_get_max_num();
	char *name;
	int nRet = 0;
	FILE *fp = NULL;
	char filename[MEDIUM_BUF_SIZE];

	memset(buf, 0, sizeof(buf));
	memset(filename, 0, sizeof(filename));
	time(&rawtime);
	rawtime_stop = rawtime + (log_time);
	if (param_log) {
		if (sprintf_s(filename, MEDIUM_BUF_SIZE - 1,
			      "/tmp/ts_log_%s.csv", f_time) <= 0) {
			printf("sprintf_s failed\n");
			nRet = -1;
			goto err;
		}
		fp = fopen(filename, "a");
		if (!fp) {
			printf("fopen error! /tmp\n");
			nRet = 0;
			goto err;
		}
	}

	for (;;) {
		struct tm *timeinfo;
		struct timeval current_time;
		int milli;
		size_t len;

		time(&rawtime);
		gettimeofday(&current_time, NULL);
		milli = current_time.tv_usec / 1000;
		timeinfo = localtime(&rawtime);
		if (timeinfo) {
			len = strftime(buf, 80, "%T", timeinfo);
			if (sprintf_s((buf + len), STD_BUF_SIZE - len, ":%03d", milli) <= 0) {
				printf("sprintf_s failed\n");
				nRet = -1;
				goto err;
			}
			if (strcat_s(buf, sizeof(buf), ": ") != 0)
				break;
		}
		if (max_tzone_num > 0) {
			for (i = 0; i < (unsigned int)max_tzone_num; i++) {
				name = sysfs_get_tzone_name(i);
				if (!name)
					continue;
				found = 1;
				pm_print_temp(i, name, buf, fp);
				sysfs_put_tzone_name(name);
			}
			if (fp)
				fprintf(fp, "\r\n");
			if (found == 0) {
				printf("No Temperature Sensor found\n");
				break;
			}
			if (update_time_g == 0)
				break;

			usleep(update_time_g * 1000);
		} else {
			printf("No Temperature Sensor found\n");
			break;
		}
		if (param_log) {
			if (time(&rawtime) > rawtime_stop)
				break;
		}
	}
err:
	if (param_log) {
		if (fp) {
			fprintf(fp,"\r\n");
			fclose(fp);
		}
	}
	return nRet;
}

struct avgt {
	int num;
	long value;
};

struct avgp {
	char *name;
	char rail_name[STD_BUF_SIZE];
	unsigned long pwr;
	unsigned long volt;
};

static int get_num_mon(int max_hwmon_num)
{
	unsigned int i;
	int num_mon = 0;

	if (max_hwmon_num == -1 || max_hwmon_num == 0) {
		printf("No Power Monitor devices found\n");
		return -1;
	}
	for (i = 0; i < (unsigned int)max_hwmon_num; i++) {
		static char *name;

		name = sysfs_get_hwmon_name(i);
		if (!name)
			continue;
		if (strcmp(name, pm_power_monitor_name) == 0)
			num_mon++;
		sysfs_put_hwmon_name(name);
	}
	return num_mon;
}

static int pm_average_pwr(char *f_time)
{
	FILE *fp = NULL;
	unsigned int i, m;
	unsigned int tracker = 0;
//	unsigned int yac = 0;
	unsigned long power = 0;
	unsigned long voltage = 0;
	int j;
	int first_run = 1;
	int arg_num = 3;
	char sub_pwr[SMALL_BUF_SIZE];
	char sub_vlt[SMALL_BUF_SIZE];
	char filename[MEDIUM_BUF_SIZE];
	char *sub_rail;
	unsigned int count = 0;
	char buf[STD_BUF_SIZE];
	char *name, *railname = NULL;
	struct avgp *avg_p;
	time_t rawtime, rawtime_stop;
	int num_mon, nRet = 0;
	int max_hwmon_num;

	max_hwmon_num = hwmon_get_max_num();
	num_mon = get_num_mon(max_hwmon_num);
	if (num_mon <= 0) {
		printf("No Power Monitor devices found\n");
		return 0;
	}
	/* malloc line doesn't have a corresponding free()
	 *	- >  function-end always terminates program as well
	 */
	avg_p = malloc((sizeof(struct avgp)) * (unsigned int)num_mon);
	if (!avg_p)
		return 0;

	for (i = 0; i < (unsigned int)num_mon; i++) {
		(avg_p + i)->volt = 0;
		(avg_p + i)->pwr = 0;
	}
	printf("************** Getting Values for ina2xx ******************\n");
	printf("Command line argument (Time to average): %u sec\n\n",
	       average_time);
	average_time = (average_time * 1000) / update_time_g;
	time(&rawtime);
	rawtime_stop = rawtime + (log_time);
	for (;;) {
		struct tm *timeinfo;
		struct timeval current_time;
		int milli;
		size_t len;

		time(&rawtime);
		gettimeofday(&current_time, NULL);
		milli = current_time.tv_usec / 1000;
		timeinfo = localtime(&rawtime);
		if (timeinfo) {
			len = strftime(buf, 80, "%T", timeinfo);
			if (sprintf_s((buf + len), STD_BUF_SIZE - len, ":%03d", milli) <= 0) {
				printf("sprintf_s failed\n");
				nRet = -1;
				goto err;
			}
			if (strcat_s(buf, sizeof(buf), ":") != 0)
				break;
		} else {
			printf("timeinfo could not be found! -> Abort\n");
			break; /*jump out of infinite for loop*/
		}
		tracker++;
		if (max_hwmon_num > 0) {
			j = -1;
			for (i = 0; i < (unsigned int)max_hwmon_num; i++) {
				name = sysfs_get_hwmon_name(i);
				if (!name)
					continue;
				if (strcmp(name, pm_power_monitor_name) == 0) {
					j++;

					railname = sysfs_get_hwmon_rail_name(i);
					voltage = sysfs_get_hwmon_rail_volt(i);
					power = sysfs_get_hwmon_power(i) / 1000;
					(avg_p + j)->name = name;
					if (sprintf_s(((avg_p + j)->rail_name),
						      80,
						      "%s", railname) <= 0) {
						printf("sprintf_s failed\n");
						nRet = -1;
						goto err;
					}
					(avg_p + j)->pwr += power;
					(avg_p + j)->volt += voltage;
					sysfs_put_hwmon_rail_name(railname);
				}
				sysfs_put_hwmon_name(name);
			}
			count++;
			if (count == average_time + 1) {
				printf(RESTART);
				count = 0;
				if (tracker != 0)
					tracker -= 1;
				for (i = 0; i < (unsigned int)num_mon; i++) {
					(avg_p + i)->pwr = 0;
					(avg_p + i)->volt = 0;
				}
			}
		}
		if ((((tracker * 1000) >= update_time_g) || first_run) && count != 0) {
			tracker = 0;
			m = 0;
			if (param_log) {
				if (sprintf_s(filename, MEDIUM_BUF_SIZE - 1,
					      "/tmp/pwr_log_%s.csv",
					      f_time) <= 0) {
					printf("sprintf_s failed\n");
					nRet = -1;
					goto err;
				}
				fp = fopen(filename, "a");
				if (!fp) {
					printf("File open error! /tmp +w\n");
					break; /*jump out of infinite for loop*/
				}
			}
			for (i = 0; i < (unsigned int)num_mon; i++) {
				if (sprintf_s(sub_pwr, 10, "%lu",
					      ((avg_p + i)->pwr) / (count)) <= 0) {
					printf("sprintf_s failed\n");
					nRet = -1;
					goto err;
				}
				if (sprintf_s(sub_vlt, 10, "%lu",
					      ((avg_p + i)->volt) / (count)) <= 0) {
					printf("sprintf_s failed\n");
					nRet = -1;
					goto err;
				}
				sub_rail = ((avg_p + i)->rail_name);

				if (!slc_i) {
					printf("-%d-	%s Power_Monitor   (%s %10s) P(avg)[mW]: %5s  V(avg)[mV]: %5s\n",
					       i, buf, pm_power_monitor_name, (avg_p + i)->rail_name,
					       sub_pwr, sub_vlt);
					m++;
					if (param_log)
						write_to_file(buf, fp, arg_num, sub_rail, sub_pwr, sub_vlt);

				} else {
					for (j = 0; j < PWR_MAX_SELECTION; j++) {
						if (i == (unsigned int)selection[j]) {
							printf("-%d-	%s Power_Monitor   (%s %10s) P(avg)[mW]: %5lu  V(avg)[mV]: %5lu\n",
							       i, buf, pm_power_monitor_name, (avg_p + i)->rail_name,
							       ((avg_p + i)->pwr) / count,
							       ((avg_p + i)->volt) / count);
							m++;
							if (param_log)
								write_to_file(buf, fp, arg_num, sub_rail, sub_pwr, sub_vlt);
						}
					}
				}
			}
			if (param_log) {
				if (!fp)
					break; /*jump out of infinite for loop*/

				fprintf(fp, "\r\n");
				fclose(fp);
			}
			if (m > 1)
				printf("\n");
		}
		first_run = 0;
		if (param_log) {
			if (time(&rawtime) > rawtime_stop)
				break;
		}
		usleep(update_time_g * 1000);
	}
	nRet = 0;
err:
	if (avg_p) {
		free(avg_p);
	}
	if (fp) {
		fprintf(fp, "\r\n");
		fclose(fp);
	}
	return nRet;
}

static int pm_power_monitor(void)
{
	unsigned int i, found = 0;
	char buf[STD_BUF_SIZE];
	time_t rawtime;
	char *railname;
	char *name;
	unsigned long power, voltage;
	int max_hwmon_num = hwmon_get_max_num();

	buf[0] = '\0';
	for (;;) {
		struct tm *timeinfo;
		struct timeval current_time;
		int milli;
		size_t len;

		time(&rawtime);
		gettimeofday(&current_time, NULL);
		milli = current_time.tv_usec / 1000;
		timeinfo = localtime(&rawtime);
		if (timeinfo) {
			len = strftime(buf, 80, "%T", timeinfo);
			if (sprintf_s((buf + len), STD_BUF_SIZE - len, ":%03d", milli) <= 0) {
				printf("sprintf_s failed\n");
				return -1;
			}
		} else {
			printf("timeinfo could not be found! -> Abort\n");
		}
		if (max_hwmon_num > 0) {
			for (i = 0; i < (unsigned int)max_hwmon_num; i++) {
				name = sysfs_get_hwmon_name(i);
				if (!name)
					continue;
				if (strcmp(name, pm_power_monitor_name) == 0) {
					found = 1;
					power = sysfs_get_hwmon_power(i);
					voltage = sysfs_get_hwmon_rail_volt(i);
					railname = sysfs_get_hwmon_rail_name(i);
					if (!railname) {
						sysfs_put_hwmon_name(name);
						continue;
					}

					if (update_time_g == 0) {
						printf("-%u-	Power_Monitor   (%s %10s) ",
						       i, name, railname);
						printf("P[mW]: %5lu  ",
						       power / 1000);
						printf("V[mV]: %5lu",
						       voltage);
					} else {
						printf("-%u-	%s Power_Monitor   (%s %10s) ",
						       i, buf, name, railname);
						printf("P[mW]: %5lu  ",
						       power / 1000);
						printf("V[mV]: %5lu",
						       voltage);
					}
					printf("\n");
					sysfs_put_hwmon_rail_name(railname);
				}
				sysfs_put_hwmon_name(name);
			}
			if (found == 0) {
				printf("No Power Monitor devices found\n");
				break;
			}
			//printf("\n");
			if (update_time_g == 0)
				break;

			usleep(update_time_g * 1000);
		} else {
			printf("No Power Monitor devices found\n");
			break;
		}
	}
	return 0;
}

static void pm_regulator(void)
{
	unsigned int i;
	char *name;
	char *i2c;
	char *vol;
	char *ena;
	char *sum;
	int uvolts, min_uvolts, max_uvolts, reg;
	/*char *status;*/
	char dummy[] = "regulator-dummy";
	int max_reg_num = reg_get_max_num();
	int ret;

	if (param_set) {
		reg = sysfs_reg_name_exists(reg_name);
		if (reg < 0) {
			printf("Regulator %s does not exists\n", reg_name);
			return;
		}

		ret = sysfs_reg_virt_consumer_set_voltage(reg, reg_min_uV,
							  reg_max_uV);
		if (ret < 0) {
			printf("Could not set %ui - %ui uV range for %s\n",
			       reg_min_uV, reg_max_uV, reg_name);
			return;
		}
		return;
	}

	if (sysfs_dbg_reg_summary_exists()) {
		sum = sysfs_get_dbg_reg_summary();
		if (sum) {
			printf("%s\n", sum);
			sysfs_put(sum);
		}
		return;
	}
	if (max_reg_num < 1) {
		printf("No Regulators found\n");
		return;
	}
	for (i = 0; i < (unsigned int)max_reg_num; i++) {
		name = sysfs_get_reg_name(i);
		if (!name)
			continue;
		min_uvolts = sysfs_get_reg_min_uvolts(i);
		max_uvolts = sysfs_get_reg_max_uvolts(i);
		uvolts = sysfs_get_reg_uvolts(i);
		if (strcmp(name, dummy)) {
			printf("regulator%u: %s\n", i, name);
			printf("	cur: %d", uvolts);
			printf("	min: %d", min_uvolts);
			printf("	max: %d\n", max_uvolts);
			printf("\n");
		}
		sysfs_put(name);
	}

	/*regulator_cpufreq files*/
	ena = sysfs_get_dbg_enable1();
	if (!ena)
		return;

	printf("regulator_cpufreq: %s\n", ena);
	sysfs_put(ena);

	i2c = sysfs_get_dbg_i2c_ctrl1();
	if (!i2c)
		return;

	printf("regulator_cpufreq: %s\n", i2c);
	sysfs_put(i2c);

	vol = sysfs_get_dbg_vol1();
	if (!vol)
		return;

	printf("regulator_cpufreq: %s\n\n", vol);
	sysfs_put(vol);
}

static int pm_frequency(unsigned long freq, unsigned int cpu_id)
{
	unsigned int cpu;
	unsigned int nr_cpus;

	nr_cpus = count_cpus();
	/* Intel/Lantiq CPE SoC's support up to 4 CPU's */
	if (nr_cpus > 4)
		nr_cpus = 1;

	if (param_set) {
		if (cpu_id != 0xFF) {
			cpufreq_set_frequency(cpu_id, freq);
			printf("set CPU %u frequency: %lu\n", cpu_id, freq);
		} else {
			for (cpu = 0; cpu < nr_cpus; cpu++) {
				cpufreq_set_frequency(cpu, freq);
				printf("set CPU %u frequency: %lu\n", cpu, freq);
			}
		}
	} else {
		//char *scaling;

		for (cpu = 0; cpu < nr_cpus; cpu++) {
			/*get current frequency*/
			printf("CPU %u:\n", cpu);
			printf(" current CPU frequency: [info ");
			print_speed(cpufreq_get_freq_hardware(cpu));
			printf("]");
			printf(" [scale ");
			print_speed(cpufreq_get_freq_scaling(cpu));
			printf("]");
			printf(" [max ");
			print_speed(cpufreq_get_max_freq(cpu));
			printf("]");
			printf("\n\n");
			//scaling = cpufreq_get_ltq_scaling_clks(0);
			//if (scaling)
			//	printf("\n%s\n\n", scaling);

			//cpufreq_put_ltq_scaling_clks(scaling);
		}
	}
	return 0;
}

static int pm_maxfrequency(unsigned long freq, unsigned int cpu_id)
{
	unsigned int cpu;
	unsigned int nr_cpus;

	nr_cpus = count_cpus();
	/* Intel/Lantiq CPE SoC's support up to 4 CPU's */
	if (nr_cpus > 4)
		nr_cpus = 1;

	if (param_set) {
		if (cpu_id != 0xFF) {
			sysfs_set_max_freq(cpu_id, freq);
			printf("set maxCPU %u frequency: %lu\n", cpu_id, freq);
		} else {
			for (cpu = 0; cpu < nr_cpus; cpu++) {
				sysfs_set_max_freq(cpu, freq);
				printf("set maxCPU %u frequency: %lu\n", cpu, freq);
			}
		}
	} else {
		for (cpu = 0; cpu < nr_cpus; cpu++) {
			/*get current frequency*/
			printf("CPU %u:\n", cpu);
			printf(" max CPU frequency: ");
			print_speed(cpufreq_get_max_freq(cpu));
			printf("\n\n");
		}
	}
	return 0;
}

static int pm_governor(char *gov, unsigned int cpu_id)
{
	unsigned int cpu;
	unsigned int nr_cpus;

	nr_cpus = count_cpus();
	/* Intel/Lantiq CPE SoC's support up to 4 CPU's */
	if (nr_cpus > 4)
		nr_cpus = 1;

	if (param_set) {
		if (cpu_id != 0xFF) {
			cpufreq_modify_policy_governor(cpu_id, gov);
			printf("set CPU %u governor: %s\n", cpu_id, gov);
		} else {
			for (cpu = 0; cpu < nr_cpus; cpu++) {
				cpufreq_modify_policy_governor(cpu, gov);
				printf("set CPU %u governor: %s\n", cpu, gov);
			}
		}
	} else {
		/*get current governor*/
		struct cpufreq_policy *policy;

		for (cpu = 0; cpu < nr_cpus; cpu++) {
			policy = cpufreq_get_policy(cpu);
			if (policy) {
				printf("CPU %u:\n", cpu);
				printf(" cur governor: %s\n", policy->governor);
				cpufreq_put_policy(policy);
			} else {
				printf("CPU %u:\n", cpu);
				printf(" cur governor:\n");
			}
			printf("\n");
		}
	}
	return 0;
}

static int pm_ena_dynamic_freq_scaling(char *ena)
{
	unsigned int cpu = 0;
	int ret;

	if (pm_soc_id != PM_SOC_LGM) {
		if ((strcmp(ena, "on") == 0) || (strcmp(ena, "1") == 0)) {
			/*set dynamic(load dependent) governor*/
			cpufreq_modify_policy_governor(cpu, "conservative");
			printf("\nDynamic frequency scaling is enabled\n\n");

		} else if ((strcmp(ena, "off") == 0) || (strcmp(ena, "0") == 0)) {
			/*set performance governor*/
			cpufreq_modify_policy_governor(cpu, "performance");
			printf("\nDynamic frequency scaling is disabled\n\n");
		} else {
			printf("wrong argument was given. Use [0|1|off|on]\n");
		}
	} else {/* LGM */
		if ((strcmp(ena, "on") == 0) || (strcmp(ena, "1") == 0)) {
			/* set P-State driver to ACTIVE */
			ret = system("echo active > /sys/devices/system/cpu/intel_pstate/status");
			if (ret == 0)
				printf("\nDynamic frequency scaling is enabled\n\n");
			else
				printf("no P-State driver found\n");
		} else if ((strcmp(ena, "off") == 0) || (strcmp(ena, "0") == 0)) {
			/* set P-State driver to PASSIVE */
			ret = system("echo passive > /sys/devices/system/cpu/intel_pstate/status");
			if (ret == 0)
				printf("\nDynamic frequency scaling is disabled\n\n");
			else
				printf("no P-State driver found\n");
		} else {
			printf("wrong argument was given. Use [0|1|off|on]\n");
		}
	}
	return 0;
}

static void int_handler(int sig)
{
	signal(sig, SIG_IGN);
	printf("\n***** Terminated by User *****\n\n");
	exit(0);
}

char *get_time(char *main)
{
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);

	if (!tm) {
		if (sprintf_s(main, 20, "0-0-0_00") <= 0) {
			printf("sprintf_s failed\n");
			return main;
		}
	} else {
		if (sprintf_s(main, 20, "%d-%d-%d_%d%d", tm->tm_mday,
			      tm->tm_mon + 1, tm->tm_year - 100,
			      tm->tm_hour, tm->tm_min) <= 0) {
			printf("sprintf_s failed\n");
			return main;
		}
	}
	return main;
}

static void clk_summary(void)
{
	char *sum;

	if (sysfs_dbg_clk_summary_exists()) {
		sum = sysfs_get_dbg_clk_summary();
		if (sum) {
			printf("%s\n", sum);
			sysfs_put(sum);
		}
	}
}

static void pm_control(void)
{
	int ret;
	char str_pm_control[MEDIUM_BUF_SIZE];

	memset(str_pm_control, 0, sizeof(str_pm_control));
	if (sprintf_s(str_pm_control, MEDIUM_BUF_SIZE - 1,
		      "/opt/intel/bin/pwr_control.sh %s %s 2> /dev/null",
		      pm_control_device, pm_level) <= 0) {
		printf("sprintf_s failed\n");
		return;
	}
	ret = system(str_pm_control);
	if (ret == -1)
		printf("calling script failed");
}

static void pm_wifi(void)
{
	int ret_ant;
	int ret_tmp = -1;
	int ret_wifi_ps_on;
	int ret_wifi_ps_of;
	int count_print_ant = 0;
	int count_print_tmp = 0;
	int i;
	char string_ant[MEDIUM_BUF_SIZE];
	char string_tmp[MEDIUM_BUF_SIZE];
	char str_wifi_powersave_on[MEDIUM_BUF_SIZE];
	char str_wifi_powersave_of[MEDIUM_BUF_SIZE];

	memset(string_ant, 0, sizeof(string_ant));
	memset(string_tmp, 0, sizeof(string_tmp));
	memset(str_wifi_powersave_on, 0, sizeof(str_wifi_powersave_on));
	memset(str_wifi_powersave_of, 0, sizeof(str_wifi_powersave_of));
	if (param_set) {
		if (numb_ant[0] == 0) {
			if (sprintf_s(str_wifi_powersave_on, MEDIUM_BUF_SIZE - 1,
				      "iw %s iwlwav sCoCPower 1 > /dev/null",
				      wifi_name) <= 0) {
				printf("sprintf_s failed\n");
				return;
			}
			ret_wifi_ps_on = system(str_wifi_powersave_on);
			if (ret_wifi_ps_on == -1) {
				if (sprintf_s(str_wifi_powersave_on, MEDIUM_BUF_SIZE - 1,
						"iw-mxl %s iwlwav sCoCPower 1 > /dev/null",
						wifi_name) <= 0) {
						printf("sprintf_s failed\n");
						return;
				}
				ret_wifi_ps_on = system(str_wifi_powersave_on);
				if (ret_wifi_ps_on == -1)
					printf("iw-mxl iwlwav failed");
			}
		} else {
			if (sprintf_s(str_wifi_powersave_of, MEDIUM_BUF_SIZE - 1,
					"iw %s iwlwav sCoCPower 0 %s %s > /dev/null",
					wifi_name, numb_ant, numb_ant) <= 0) {
				printf("sprintf_s failed\n");
				return;
			}
			ret_wifi_ps_of = system(str_wifi_powersave_of);
			if (ret_wifi_ps_of == -1) {
				if (sprintf_s(str_wifi_powersave_of, MEDIUM_BUF_SIZE - 1,
						"iw-mxl %s iwlwav sCoCPower 0 %s %s > /dev/null",
						wifi_name, numb_ant, numb_ant) <= 0) {
					printf("sprintf_s failed\n");
					return;
				}
				ret_wifi_ps_of = system(str_wifi_powersave_of);
				if (ret_wifi_ps_of == -1)
					printf("iw-mxl iwlwav failed");
			}
		}
	} else {
		for (i = 0; i <= 5; i++) {
			if (sprintf_s(string_ant, MEDIUM_BUF_SIZE - 1,
					"iw wlan%d iwlwav gCoCPower &> /dev/null",
					i) <= 0) {
				printf("sprintf_s failed\n");
				return;
			}
			ret_ant = system(string_ant);
			if (ret_ant == 0) {
				printf("wlan%i:\n", i);
				if (sprintf_s(string_ant, MEDIUM_BUF_SIZE - 1,
						"iw wlan%d iwlwav gCoCPower",
						i) <= 0) {
					printf("sprintf_s failed\n");
					return;
				}
				ret_ant = system(string_ant);
				if (ret_ant == -1)
				if (sprintf_s(string_tmp, MEDIUM_BUF_SIZE - 1,
						"iw wlan%d iwlwav gTemperature",
						i) <= 0) {
					printf("sprintf_s failed\n");
					return;
				}
				ret_tmp = system(string_tmp);
			}
			// "iw" failed , use iw-mxl
			else {
				if (sprintf_s(string_ant, MEDIUM_BUF_SIZE - 1, 
						"iw-mxl wlan%d iwlwav gCoCPower &> /dev/null",
						i) <= 0) { 
					printf("sprintf_s failed\n");
					return;
				}
				ret_ant = system(string_ant);
				if (ret_ant == 0) { 
					printf("wlan%i:\n", i);
					if (sprintf_s(string_ant, MEDIUM_BUF_SIZE - 1, 
							"iw-mxl wlan%d iwlwav gCoCPower",
							i) <= 0) { 
						printf("sprintf_s failed\n");
						return;
					}	
					ret_ant = system(string_ant);
					if (sprintf_s(string_tmp, MEDIUM_BUF_SIZE - 1, 
							"iw-mxl wlan%d iwlwav gTemperature",
							i) <= 0) { 
						printf("sprintf_s failed\n");
						return;
					}
					ret_tmp = system(string_tmp);
				}
			}	


			if (ret_ant == 0)
				count_print_ant++;

			if (ret_tmp == 0)
				count_print_tmp++;
		}
		if (count_print_ant > 0)
			printf("\nLEGEND: gCoCPower: 1st:Auto_CoC  2nd:#Ant.Tx  3rd:#Ant.Rx\n");
		else
			printf("No WiFi interfaces found\n");

		if (count_print_tmp > 0)
			printf("        gTemperature [°C]\n");

		printf("\n");
		return;
	}
}

int pm_cstate_disable(unsigned int cstate, unsigned int cdisable, unsigned int cpu_id)
{
	unsigned int cpu;
	unsigned int nr_cpus;
	char *i_cstate = NULL;

	nr_cpus = count_cpus();
	/* Intel/Lantiq CPE SoC's support up to 4 CPU's */
	if (nr_cpus > 4)
		nr_cpus = 1;

	if (param_set) {
		if (cdisable > 1)
			return printf("only values 0 or 1 are allowed for cdisable\n");

		if (cstate < 1 || cstate > 4)
			return printf("only values 1,2,3,4 are allowed for cstate\n");

		if (cpu_id != 0xFF) {
			sysfs_disable_state(cpu_id, cstate, cdisable);
			printf("set CPU%u C%d=%s\n", cpu_id, cstate, cdisable ? "disable" : "enable");
		} else {
			for (cpu = 0; cpu < nr_cpus; cpu++) {
				sysfs_disable_state(cpu, cstate, cdisable);
				printf("set CPU%u C%d=%s\n", cpu, cstate, cdisable ? "disable" : "enable");
			}
		}
	} else {
		for (cpu = 0; cpu < nr_cpus; cpu++) {
			if (sysfs_cpu_cpuidle_exists(cpu) == 0) {
				i_cstate = sysfs_get_cstate_disable(cpu);
				if (i_cstate) {
					printf("C-State[disable]: CPU%d: %s\n", cpu, i_cstate);
					sysfs_put_cstate(i_cstate);
				}
			}
		}
		printf("\n");
	}
	return 0;
}

static int pm_pmic(char *power_rail, unsigned int vol)
{
	if (param_set)
		i2c_pmic_set_vol(power_rail, vol);
	else
		i2c_pmic_get_vol();
	return 0;
}

static void pm_getbitfield(unsigned int val, unsigned int mask, unsigned int shift)
{
	printf("%x\n", ((val & mask) >> shift));
}

int main(int argc, char **argv)
{
	unsigned int i;
	unsigned int cpu_no = 0xFF;
	char corevol[SMALL_BUF_SIZE];
	char t_fname[40];
	unsigned long freq = 0;
	unsigned int val2mask = 0;
	unsigned int mask = 0;
	unsigned int shift = 0;
	unsigned int cstate = 0;
	unsigned int cdisable = 0;
	unsigned int log_mask = 0x3F3D;
	unsigned int socket = 0;
	unsigned short socket_port = 49666;
	char gov[SMALL_BUF_SIZE];
	char ctrl[SMALL_BUF_SIZE];
	char dyn[SMALL_BUF_SIZE];
	unsigned int r_period = 1000;
	char dvs[SMALL_BUF_SIZE];
	char force[SMALL_BUF_SIZE];
	char alert[SMALL_BUF_SIZE];
	char power_rail_name[8];
	unsigned int power_rail_vol = 0;
	int option_index = 0;
	char log_dest[MEDIUM_BUF_SIZE];
	char *log_path = log_dest;

	signal(SIGINT, int_handler);
	static struct option long_options[] = {
		{"help", 0, 0, 'h'},
		{"regulator", 2, 0, 'u'},
		{"version", 0, 0, 'v'},
		{"all", 0, 0, 'a'},
		{"temp", 0, 0, 't'},
		{"avgt", 2, 0, 'T'},
		{"select", 0, 0, 'z'},
		{"cfreq", 0, 0, 'c'},
		{"refresh", 1, 0, 'r'},
		{"pwr", 0, 0, 'p'},
		{"avpwr", 2, 0, 'P'},
		{"set", 0, 0, 's'},
		{"freq", 2, 0, 'f'},
		{"gov", 2, 0, 'g'},
		{"ena", 2, 0, 'e'},
		{"info", 0, 0, 'i'},
		{"dyn", 1, 0, 'y'},
		{"mask", 2, 0, 'j'},
		{"idle", 2, 0, 'd'},
		{"wifi", 2, 0, 'w'},
		{"pmlog", 2, 0, 'k'},
		{"clk", 0, 0, 'l'},
		{"log", 2, 0, 'L'},
		{"lmsk", 2, 0, 'M'},
		{"sock", 2, 0, 'S'},
		{"fdir", 2, 0, 'D'},
		{"maxfreq", 2, 0, 'm'},
		{"dvs", 1, 0, 'x'},
		{"vol", 2, 0, 'w'},
		{"pmic", 2, 0, 'o'},
		{0, 0, 0, 0}
	};
	get_time(t_fname);
	memset(corevol, 0, sizeof(corevol));
	memset(gov, 0, sizeof(gov));
	memset(ctrl, 0, sizeof(ctrl));
	memset(dyn, 0, sizeof(dyn));
	memset(dvs, 0, sizeof(dvs));
	memset(force, 0, sizeof(force));
	memset(alert, 0, sizeof(alert));
	memset(wifi_name, 0, sizeof(wifi_name));
	memset(numb_ant, 0, sizeof(numb_ant));
	memset(pm_control_device, 0, sizeof(pm_control_device));
	memset(pm_level, 0, sizeof(pm_level));
	memset(selection, 0xFF, sizeof(selection));
	memset(log_dest, 0, sizeof(log_dest));
	(void)strncpy_s(log_dest, sizeof(log_dest), "/tmp/\0", sizeof("/tmp/\0"));

	func_select = 0;
	pm_find_soc();
	if (pm_soc_id == PM_SOC_LGM)
		log_mask = 0x1FBD;
	if (pm_soc_id == PM_SOC_PRX126 || pm_soc_id == PM_SOC_PRX321)
		log_mask = 0x37;
	if (pm_soc_id == PM_SOC_GRX350 || pm_soc_id == PM_SOC_GRX550)
		log_mask = 0x37;

	while ((c_opt_g = getopt_long(argc, argv, "hu::vatT::cr:pP::z:sf::m::g::e::i::y:k::j::d::l::L::M::S::D::x:w::o::",
				      long_options, &option_index)) != -1) {
		switch (c_opt_g) {
		case 'h':
			pm_util_help();
			exit(0);
		case 'u':
			func_select = PM_REGULATOR;
			if (param_set != PM_SET)
				break;

			if (argc >= optind + 3) {
				if (strcpy_s(reg_name, sizeof(reg_name),
					     argv[optind]) != 0)
					return -EINVAL;
				reg_min_uV = (unsigned int)strtol(argv[optind + 1], NULL, 10);
				reg_max_uV = (unsigned int)strtol(argv[optind + 2], NULL, 10);
			} else {
				print_unknown_arg();
				return -EINVAL;
			}
			break;
		case 'v':
			print_version();
			exit(0);
		case 'a':
			func_select = PM_DISPLAY_ALL;
			break;
		case 't':
			func_select = PM_TEMP_SENSOR;
			break;
		case 'T':
			break;
		case 'c':
			break;
		case 'r':
			if (optarg)
				update_time_g = (unsigned int)strtol(optarg,
								     NULL, 10);
			break;
		case 'p':
			func_select = PM_POWER_MONITOR;
			break;
		case 'P':
			func_select = PM_AVERAGE_PWR;
			update_time_g = 1000;
			if (!optarg)
				average_time = 120;
			else
				average_time = ((unsigned int)strtol(optarg, NULL, 10));
			break;
		case 'z':
			slc_i = optind - 1;
			i = 0;
			while (slc_i < argc) {
				selection[i] = atoi(argv[slc_i]);
				i++;
				slc_i++;
			}
			break;
		case 's':
			param_set = PM_SET;
			break;
		case 'f':
			func_select = PM_FREQUENCY;
			if (optarg) {
				freq = string_to_frequency(optarg);
				if (freq == 0) {
					print_unknown_arg();
					return -EINVAL;
				}
				if (argc > 2)
					cpu_no = (unsigned int)strtol(argv[optind], NULL, 10);
			}
			break;
		case 'm':
			func_select = PM_MAXFREQUENCY;
			if (optarg) {
				freq = string_to_frequency(optarg);
				if (freq == 0) {
					print_unknown_arg();
					return -EINVAL;
				}
				if (argc > 2)
					cpu_no = (unsigned int)strtol(argv[optind], NULL, 10);
			}
			break;
		case 'g':
			func_select = PM_GOVERNOR;
			if (optarg) {
				if ((strnlen_s(optarg, 19) < 3) || (strnlen_s(optarg, 19) > 18)) {
					print_unknown_arg();
					return -EINVAL;
				}
				if ((sscanf_s(optarg, "%19s", gov)) != 1) {
					print_unknown_arg();
					return -EINVAL;
				}
				if (argc > 2)
					cpu_no = (unsigned int)strtol(argv[optind], NULL, 10);
			}
			break;
		case 'e':
			func_select = PM_CONTROL;
			if (optarg)
				if (strncpy_s(pm_control_device,
					      sizeof(pm_control_device), optarg,
					      sizeof(pm_control_device)) != 0)
					return -EINVAL;
			break;
		case 'i':
			if (optarg)
				if (strncpy_s(pm_level, sizeof(pm_level),
					      optarg, sizeof(pm_level)) != 0)
					return -EINVAL;
			break;
		case 'y':
			func_select = PM_ENABLE_DYN_FS;
			if (optarg)
				if ((sscanf_s(optarg, "%19s", dyn)) != 1) {
					print_unknown_arg();
					return -EINVAL;
				}
			break;
		case 'x':
			if (optarg)
				if (strncpy_s(numb_ant, sizeof(numb_ant),
					      optarg, sizeof(numb_ant)) != 0)
					return -EINVAL;
			break;
		case 'w':
			func_select = PM_WIFI;
			if (optarg)
				if (strncpy_s(wifi_name, sizeof(wifi_name),
					      optarg, sizeof(wifi_name)) != 0)
					return -EINVAL;
			break;
		case 'j':
			func_select = PM_BITFIELD_MASK;
			if (optarg)
				val2mask = (unsigned int)strtol(optarg, NULL, 0);
			if (argc > 2)
				mask = (unsigned int)strtol(argv[optind], NULL, 0);
			if (argc > 3)
				shift = (unsigned int)strtol(argv[optind + 1], NULL, 0);
			break;
		case 'd':
			func_select = PM_DISABLE_STATE;
			if (optarg) {
				cstate = ((unsigned int)strtol(optarg, NULL, 10));
				if (argc > 2)
					cdisable = (unsigned int)strtol(argv[optind], NULL, 10);
				if (argc > 3)
					cpu_no = (unsigned int)strtol(argv[optind + 1], NULL, 10);
				if (argc > 4)
					return printf("wrong number of parameters\n");
			}
			break;
		case 'k':
			func_select = PM_LOGGER;
			if (!optarg)
				r_period = 1000;
			else
				r_period = ((unsigned int)strtol(optarg,
								 NULL, 10));
			break;
		case 'l':
			clk_summary();
			exit(0);
			break;
		case 'L':
			param_log = PM_LOG;
			if (!optarg)
				log_time = 3600;
			else
				log_time = ((unsigned int)strtol(optarg, NULL, 10)) * 60;
			break;
		case 'D':
			if (optarg) {
				if (strncpy_s(log_dest, sizeof(log_dest), optarg, sizeof(log_dest)) != 0) {
					printf("ERROR in given path\n");
					return -EINVAL;
				}
				printf("log-file location: %s\n", log_dest);
			}
			break;
		case 'M':
			if (!optarg)
				log_mask = 0x1FFF;
			else
				log_mask = ((unsigned int)strtol(optarg,
					     NULL, 0));
			break;
		case 'S':
			if (!optarg) {
				socket = 1;
			} else {
				socket = 1;
				socket_port = ((unsigned int)strtol(optarg,
					     NULL, 0));
			}
			break;
		case 'o':
			func_select = PM_PMIC;
			if (param_set != PM_SET)
				break;
			if (argc == 4) {
				if (strncpy_s(power_rail_name, sizeof(power_rail_name),
					      argv[optind], sizeof(power_rail_name)) != 0)
					return -EINVAL;
				power_rail_vol = (unsigned int)strtol(argv[optind + 1], NULL, 10);
			} else {
				print_unknown_arg();
				return -EINVAL;
			}
			break;
		default:
			pm_util_help();
			exit(0);
		}
	}

	switch (func_select) {
	case PM_LOGGER:
		pm_logger(r_period, log_path, log_mask, socket, socket_port);
		break;
	case PM_REGULATOR:
		pm_regulator();
		break;
	case PM_DISPLAY_ALL:
		pm_display_all();
		break;
	case PM_TEMP_SENSOR:
		pm_temp_sensor(t_fname);
		break;
	case PM_POWER_MONITOR:
		pm_power_monitor();
		break;
	case PM_AVERAGE_PWR:
		pm_average_pwr(t_fname);
		break;
	case PM_GOVERNOR:
		pm_governor(gov, cpu_no);
		break;
	case PM_FREQUENCY:
		pm_frequency(freq, cpu_no);
		break;
	case PM_MAXFREQUENCY:
		pm_maxfrequency(freq, cpu_no);
		break;
	case PM_ENABLE_DYN_FS:
		pm_ena_dynamic_freq_scaling(dyn);
		break;
	case PM_WIFI:
		pm_wifi();
		break;
	case PM_CONTROL:
		pm_control();
		break;
	case PM_DISABLE_STATE:
		if (pm_soc_id == PM_SOC_LGM)
			pm_cstate_disable(cstate, cdisable, cpu_no);
		break;
	case PM_PMIC:
		if (pm_soc_id == PM_SOC_LGM)
			pm_pmic(power_rail_name, power_rail_vol);
		break;
	case PM_BITFIELD_MASK:
		pm_getbitfield(val2mask, mask, shift);
		break;
	default:
		pm_util_help();
		break;
	}
	return 0;
}
