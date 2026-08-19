/*
 *  Copyright 2020 - 2025 MaxLinear, Inc.
 *  Copyright 2016 - 2020 Intel Corporation
 *  Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *  Copyright 2009 - 2014 Lantiq Deutschland GmbH
 *  Copyright 2007 - 2008 Infineon Technologies AG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <safe_str_lib.h>

#define PM_STAR "************************************************************"\
		"*********"
#define ETEMPINVAL		-255
#define UNDEF			0
#define TYPICAL_L		290000000
#define TYPICAL_H		310000000
#define GRADE_UNDEF		0
#define GRADE_SLOW		1
#define GRADE_TYPICAL		2
#define GRADE_FAST		3
#define ARRAY_SIZE(_a)	(sizeof((_a)) / sizeof((_a)[0]))

enum lgm_soc_ver {
	PM_SOC_VER_UNKNOWN,
	PM_SOC_VER_A2,
	PM_SOC_VER_A3,
	PM_SOC_VER_B0,
};

static void print_header(void)
{
	printf("\n");
	printf(PM_STAR "\n");
	printf("*                         Chip Info\n");
	printf(PM_STAR "\n");
	printf("\n");
}

int get_chip_id_lgm(int *soc_ver, int prn_ena)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char iostring[100];
	long reg;
	int manid, pnum, version;
	char *manid_str;
	char *chipid_str;
	char *ver_str;
	int ver;

	system("io -4 0xecc80004 1> /tmp/chip_id_reg.txt 2> /dev/null");
	stat("/tmp/chip_id_reg.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/chip_id_reg.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(iostring, 100, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	str = strtok(iostring, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reg = strtol(str, &p_end, 16);

	manid = (reg & 0xFFE) >> 1;
	pnum = (reg & 0xFFFF000) >> 12;
	version = (reg & 0xF0000000) >> 28;
	switch (manid) {
	case 0x389:
		manid_str = "Manufacturer: Lantig";
		break;
	case 0x89:
		manid_str = "Manufacturer: Intel";
		break;
	case 0x461:
		manid_str = "Manufacturer: Maxlinear";
		break;
	default:
		manid_str = "Manufacturer: unknown";
		break;
	}

	switch (pnum) {
	case 0x80:
		chipid_str = "ChipId: URX651";
		break;
	case 0x83:
		chipid_str = "ChipId: URX850";
		break;
	case 0x90:
		chipid_str = "ChipId: URX851";
		break;
	default:
		chipid_str = "ChipId: unknown";
		break;
	}

	switch (version) {
	case 0x1:
		ver_str = "Version: A2";
		ver = PM_SOC_VER_A2;
		break;
	case 0x2:
		ver_str = "Version: A3";
		ver = PM_SOC_VER_A3;
		break;
	case 0x3:
		ver_str = "Version: B0";
		ver = PM_SOC_VER_B0;
		break;
	default:
		ver_str = "Version: unknown";
		ver = PM_SOC_VER_UNKNOWN;
		break;
	}
	if (prn_ena)
		printf("%s  %s  %s  ChipIdReg:%lx  ", manid_str, chipid_str,
		       ver_str, reg);
	if (soc_ver)
		*soc_ver = ver;
	return 0;
}

const char *grade_str[] = {"UNDEF", "SLOW", "TYP", "FAST"};
int get_speed_grade(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char fab_pvt_reg[30];
	long regfabpvt, speed_val;
	int grade_idx;
	int soc_ver, split_lot_const;

	system("io -4 0xecc80028 1> /tmp/fabpvt.txt 2> /dev/null");
	stat("/tmp/fabpvt.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/fabpvt.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(fab_pvt_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);

	get_chip_id_lgm(&soc_ver, 0);
	switch (soc_ver) {
	case PM_SOC_VER_A2:
	case PM_SOC_VER_A3:
		split_lot_const = 1568627;
		break;
	case PM_SOC_VER_B0:
		split_lot_const = 1254901;
		break;
	default:
		split_lot_const = 1254901;
		break;
	}

	str = strtok(fab_pvt_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regfabpvt = strtol(str, &p_end, 16);
	regfabpvt &= 0x3FF;
	speed_val = regfabpvt * split_lot_const;
	if (speed_val == UNDEF)
		grade_idx = GRADE_UNDEF;
	else if (speed_val > UNDEF && speed_val < TYPICAL_L)
		grade_idx = GRADE_SLOW;
	else if (speed_val >= TYPICAL_L && speed_val <= TYPICAL_H)
		grade_idx = GRADE_TYPICAL;
	else
		grade_idx = GRADE_FAST;

	printf("Speed Grade : %s\n\n", grade_str[grade_idx]);
	return 0;
}

int get_p34_version(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char iostring[40];
	char *p_end;
	int version;

	system("dmesg -n3");
	system("echo smdio 0xa222 > /sys/devices/platform/soc/soc:p34x@0/reg 2> /dev/null");
	system("dmesg | tail -50 | grep -m 1 'a222' | awk -F' ' '{print $7}' 1> /tmp/p34_ver.txt 2> /dev/null");
	system("dmesg -n4");
	stat("/tmp/p34_ver.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/p34_ver.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(iostring, 9, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	version = strtol(iostring, &p_end, 16);

	system("rm -fr /tmp/p34_ver.txt");
	if (((version & 0x7000) >> 12) == 0)
		printf("P34 Version : 000 (A-Step)\n\n");
	else
		printf("P34 Version : 001 (B-Step)\n\n");

	return 0;
}

int get_epu_status(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char devices_0_pm_status_l[30];
	char devices_0_pm_status_h[30];
	char devices_1_pm_status[30];
	char devices_2_pm_status[30];
	long reg0l, reg0h, reg1, reg2;

	system("io -4 0xe0180034 1> /tmp/epu_devices_status.txt 2> /dev/null");
	system("io -4 0xe0180038 1>> /tmp/epu_devices_status.txt 2> /dev/null");
	system("io -4 0xe018003C 1>> /tmp/epu_devices_status.txt 2> /dev/null");
	system("io -4 0xe0180040 1>> /tmp/epu_devices_status.txt 2> /dev/null");
	stat("/tmp/epu_devices_status.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/epu_devices_status.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(devices_0_pm_status_l, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(devices_0_pm_status_h, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(devices_1_pm_status, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(devices_2_pm_status, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	str = strtok(devices_0_pm_status_l, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reg0l = strtol(str, &p_end, 16);

	str = strtok(devices_0_pm_status_h, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reg0h = strtol(str, &p_end, 16);

	str = strtok(devices_1_pm_status, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reg1 = strtol(str, &p_end, 16);

	str = strtok(devices_2_pm_status, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reg2 = strtol(str, &p_end, 16);

	printf("DEVICE PM STATUS (EPU D-States):\n");
	printf("DDRCTL0 : %ld   DDRCTL1 : %ld\n", (reg0h & 0x3) >> 0, (reg1 & 0x3000) >> 12);
	printf("PON     : %ld   XPCS5   : %ld\n", (reg0l & 0x300) >> 8, (reg1 & 0x300000) >> 20);
	printf("PPV4    : %ld   GSWIP   : %ld\n", (reg0l & 0x3) >> 0, (reg0l & 0xC) >> 2);
	printf("CQM     : %ld   VOICE   : %ld\n", (reg0l & 0x30) >> 4, (reg0l & 0xC0) >> 6);
	printf("TEP     : %ld   ARCEM4  : %ld\n", (reg0l & 0xC0000) >> 18, (reg1 & 0xC000) >> 14);
	printf("EIP197  : %ld   VAULT   : %ld\n", (reg0l & 0x300000) >> 20, (reg0l & 0xC00000) >> 22);
	printf("USB1    : %ld   USB2    : %ld\n", (reg0h & 0xC00) >> 10, (reg0h & 0x3000) >> 12);
	printf("I2S0    : %ld   I2S1    : %ld\n", (reg1 & 0x300) >> 8, (reg1 & 0xC00) >> 10);
	printf("SATA0   : %ld   SATA1   : %ld\n", (reg1 & 0x30000000) >> 28, (reg1 & 0xC0000000) >> 30);
	printf("LEDC0   : %ld   LEDC1   : %ld\n", (reg1 & 0xC) >> 2, (reg1 & 0xC00000) >> 22);
	printf("ASC0    : %ld   ASC1    : %ld   ASC2    : %ld\n", (reg0h & 0xC000) >> 14, (reg0h & 0x30000) >> 16, (reg0h & 0xC0000) >> 18);
	printf("I2C1    : %ld   I2C2    : %ld   I2C3    : %ld\n", (reg0h & 0x30000000) >> 28, (reg0h & 0xC0000000) >> 30, (reg1 & 0x3) >> 0);
	printf("PCM0    : %ld   PCM1    : %ld   PCM2    : %ld\n", (reg1 & 0x30) >> 4, (reg1 & 0x30000) >> 16, (reg1 & 0xC0000) >> 18);
	printf("RTIT    : %ld   ROM     : %ld   SSB1MB  : %ld   TOE     : %ld\n", (reg0h & 0xC) >> 2, (reg0h & 0xC0) >> 6, (reg0h & 0x300) >> 8, (reg0l & 0xC000000) >> 26);
	printf("SSC0    : %ld   SSC1    : %ld   SSC2    : %ld   SSC3    : %ld\n", (reg0h & 0x300000) >> 20, (reg0h & 0xC00000) >> 22, (reg0h & 0x3000000) >> 24, (reg0h & 0xC000000) >> 26);
	printf("HSIO1   : %ld   HSIO2   : %ld   HSIO3   : %ld   HSIO4   : %ld\n", (reg0l & 0xC00) >> 10, (reg0l & 0x3000) >> 12, (reg0l & 0xC000) >> 14, (reg0l & 0x30000) >> 16);
	printf("EBU     : %ld   QSPI    : %ld   SDXC    : %ld   EMMC    : %ld\n", (reg1 & 0xC0) >> 6, (reg0h & 0x30) >> 4, (reg0l & 0x30000000) >> 28, (reg0l & 0xC0000000) >> 30);
	printf("PCIE10  : %ld   PCIE11  : %ld   PCIE20  : %ld   PCIE21  : %ld   PCIE30  : %ld   PCIE31  : %ld   PCIE40  : %ld   PCIE41  : %ld\n", (reg2 & 0x300) >> 8, (reg2 & 0x3000) >> 12, (reg2 & 0xC00) >> 10, (reg2 & 0xC000) >> 14, (reg1 & 0x3000000) >> 24, (reg2 & 0x3) >> 0, (reg1 & 0xC000000) >> 26, (reg2 & 0xC) >> 2);
	printf("\n");
	return 0;
}

int get_rcu_status(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char rst_stat_tnoc[30];
	char rst_stat_analog[30];
	char rst_stat_peri[30];
	char rst_stat_aon[30];
	char rst_stat_hsiol[30];
	char rst_stat_hsior[30];
	char rst_stat_ethnoc[30];
	char rst_stat_ppnoc[30];
	long regtnoc, regana, regperi, regaon, reghsiol, reghsior, regethnoc, regppnoc;

	system("io -4 0xe0000014 1> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000024 1>> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000034 1>> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000044 1>> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000054 1>> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000064 1>> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000074 1>> /tmp/rcu_status.txt 2> /dev/null");
	system("io -4 0xe0000084 1>> /tmp/rcu_status.txt 2> /dev/null");
	stat("/tmp/rcu_status.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/rcu_status.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(rst_stat_tnoc, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_analog, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_peri, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_aon, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_hsiol, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_hsior, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_ethnoc, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(rst_stat_ppnoc, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}

	fclose(fp);
	str = strtok(rst_stat_tnoc, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regtnoc = strtol(str, &p_end, 16);

	str = strtok(rst_stat_analog, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regana = strtol(str, &p_end, 16);

	str = strtok(rst_stat_peri, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regperi = strtol(str, &p_end, 16);

	str = strtok(rst_stat_aon, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regaon = strtol(str, &p_end, 16);

	str = strtok(rst_stat_hsiol, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reghsiol = strtol(str, &p_end, 16);

	str = strtok(rst_stat_hsior, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reghsior = strtol(str, &p_end, 16);

	str = strtok(rst_stat_ethnoc, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regethnoc = strtol(str, &p_end, 16);

	str = strtok(rst_stat_ppnoc, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regppnoc = strtol(str, &p_end, 16);

	printf("DEVICE RESET STATUS (RCU Status; 0=noReset, 1=Reset):\n");
	printf("PLL0CZ       : %ld   PLL0CM0      : %ld   PLL0CM1       : %ld   PLL0B         : %ld   PLL1        : %ld\n", (regana & 0x20) >> 5, (regana & 0x40) >> 6, (regana & 0x80) >> 7, (regana & 0x100) >> 8, (regana & 0x200) >> 9);
	printf("PLL2         : %ld   LJPLL3       : %ld   LJPLL4        : %ld   DPLL          : %ld   PLLPP       : %ld\n", (regana & 0x400) >> 10, (regana & 0x800) >> 11, (regana & 0x1000) >> 12, (regana & 0x2000) >> 13, (regana & 0x4000) >> 14);
	printf("I2S0         : %ld   I2S1         : %ld   PCM0          : %ld   PCM1          : %ld   PCM2        : %ld\n", (regperi & 0x2000000) >> 25, (regperi & 0x4000000) >> 26, (regperi & 0x10000000) >> 28, (regperi & 0x20000000) >> 29, (regperi & 0x40000000) >> 30);
	printf("IDMAR1       : %ld   IDMAT0       : %ld   IDMAT1        : %ld   IDMAT2        : %ld   PON_NIU     : %ld   PON         : %ld\n", (regethnoc & 0x1) >> 0, (regethnoc & 0x2) >> 1, (regethnoc & 0x4) >> 2, (regethnoc & 0x8) >> 3, (regethnoc & 0x10) >> 4, (regethnoc & 0x200) >> 9);
	printf("HSIOL_NOC    : %ld   HSIOR_NOC    : %ld   AON_NOC       : %ld   PERI_NOC      : %ld   ETHNOC_NOC  : %ld   PPV4_NOC    : %ld\n", (reghsiol & 0x80000000) >> 31, (reghsior & 0x80000000) >> 31, (regaon & 0x80000000) >> 31, (regperi & 0x80000000) >> 31, (regethnoc & 0x80000000) >> 31, (regppnoc & 0x80000000) >> 31);
	printf("QSPI         : %ld   VOICE_SS     : %ld   VOICE_CODEC_0 : %ld   VOICE_CODEC_1 : %ld   EIP197      : %ld   VAULTIP130  : %ld   WDT        : %ld\n", (regtnoc & 0x2) >> 1, (regtnoc & 0x4) >> 2, (regtnoc & 0x8) >> 3, (regtnoc & 0x10) >> 5, (regtnoc & 0x40) >> 6, (regtnoc & 0x80) >> 7, (regtnoc & 0x80000000) >> 31);
	printf("DMA0         : %ld   EBU          : %ld   SSC0          : %ld   SSC1          : %ld   SSC2        : %ld   SSC3        : %ld   PVTC       : %ld\n", (regperi & 0x1) >> 0, (regperi & 0x2) >> 1, (regperi & 0x10) >> 4, (regperi & 0x20) >> 5, (regperi & 0x40) >> 6, (regperi & 0x80) >> 7, (regaon & 0x80) >> 7);
	printf("ASC0         : %ld   ASC1         : %ld   ASC2          : %ld   ASC3          : %ld   PWM         : %ld   LEDC0       : %ld   LEDC1      : %ld\n", (regperi & 0x20000) >> 17, (regperi & 0x40000) >> 18, (regperi & 0x80000) >> 19, (regaon & 0x8) >> 3, (regperi & 0x200000) >> 21, (regperi & 0x400000) >> 22, (regperi & 0x800000) >> 23);
	printf("GSWIPO       : %ld   CQM          : %ld   ARCEM6        : %ld   XPCS5_PCS0    : %ld   XPCS5_PHY0  : %ld   XPCS5_CPHY  : %ld   ADPSUP_RST : %ld\n", (regethnoc & 0x100) >> 8, (regethnoc & 0x800) >> 11, (regethnoc & 0x1000) >> 12, (regethnoc & 0x8000) >> 15, (regethnoc & 0x10000) >> 16, (regethnoc & 0x20000) >> 17, (regethnoc & 0x4000000) >> 30);
	printf("TOE          : %ld   DMA3         : %ld   SDXC          : %ld   EMMC          : %ld   CPU_CLUSTER : %ld   DBG_SPI     : %ld   GLB_SW     : %ld\n", (regtnoc & 0x100) >> 8, (regtnoc & 0x200) >> 9, (regtnoc & 0x400) >> 10, (regtnoc & 0x800) >> 11, (regtnoc & 0x2000) >> 13, (regtnoc & 0x10000) >> 16, (regtnoc & 0x40000000) >> 30);
	printf("HRST         : %ld   SSB          : %ld   OTP           : %ld   MSI_GEN       : %ld   DDR_CTL     : %ld   DDR_APB     : %ld   DDR_PHY    : %ld   DDR_IED    : %ld\n", (regtnoc & 0x20000) >> 17, (regtnoc & 0x80000) >> 19, (regtnoc & 0x100000) >> 20, (regtnoc & 0x200000) >> 21, (regtnoc & 0x1000000) >> 24, (regtnoc & 0x4000000) >> 26, (regtnoc & 0x8000000) >> 27, (regtnoc & 0x10000000) >> 28);
	printf("I2C0         : %ld   I2C1         : %ld   I2C2          : %ld   I2C3          : %ld   GPT0        : %ld   GPT1        : %ld   GPT2       : %ld   GPT3       : %ld\n", (regaon & 0x2) >> 1, (regperi & 0x100) >> 9, (regperi & 0x200) >> 10, (regperi & 0x400) >> 11, (regperi & 0x2000) >> 13, (regperi & 0x4000) >> 14, (regperi & 0x8000) >> 15, (regaon & 0x10) >> 4);
	printf("USB_CTRL0    : %ld   USB_CTRL1    : %ld   USB_PHY0      : %ld   USB_PHY1      : %ld   USB_APB0    : %ld   USB_APB1    : %ld   USB0_PHY31 : %ld   USB1_PHY31 : %ld\n", (regethnoc & 0x400000) >> 22, (regethnoc & 0x800000) >> 23, (regethnoc & 0x1000000) >> 24, (regethnoc & 0x2000000) >> 25, (regethnoc & 0x4000000) >> 26, (regethnoc & 0x8000000) >> 27, (regethnoc & 0x10000000) >> 28, (regethnoc & 0x20000000) >> 29);
	printf("PCIE10_PERST : %ld   PCIE11_PERST : %ld   PCIE30_PERST  : %ld   PCIE31_PERST  : %ld\n", (reghsiol & 0x800000) >> 23, (reghsiol & 0x1000000) >> 24, (reghsiol & 0x2000000) >> 25, (reghsiol & 0x4000000) >> 26);
	printf("PCIE20_PERST : %ld   PCIE21_PERST : %ld   PCIE40_PERST  : %ld   PCIE41_PERST  : %ld\n", (reghsior & 0x800000) >> 23, (reghsior & 0x1000000) >> 24, (reghsior & 0x2000000) >> 25, (reghsior & 0x4000000) >> 26);
	printf("PCIE10_PCIE0 : %ld   PCIE11_PCIE1 : %ld   PCIE30_PCIE0  : %ld   PCIE31_PCIE1  : %ld   PCIE10_PHY  : %ld   PCIE30_PHY  : %ld\n", (reghsiol & 0x1) >> 0, (reghsiol & 0x2) >> 1, (reghsiol & 0x4) >> 2, (reghsiol & 0x8) >> 3, (reghsiol & 0x40) >> 6, (reghsiol & 0x100) >> 8);
	printf("PCIE20_PCIE0 : %ld   PCIE21_PCIE1 : %ld   PCIE40_PCIE0  : %ld   PCIE41_PCIE1  : %ld   PCIE20_PHY  : %ld   PCIE40_PHY  : %ld\n", (reghsior & 0x1) >> 0, (reghsior & 0x2) >> 1, (reghsior & 0x4) >> 2, (reghsior & 0x8) >> 3, (reghsior & 0x40) >> 6, (reghsior & 0x100) >> 8);
	printf("PCIE10_PCS   : %ld   PCIE11_PCS   : %ld   HSIOL_SATA0   : %ld   HSIOL_SATA1   : %ld   PCIE1X_CPHY : %ld   PCIE3X_CPHY : %ld\n", (reghsiol & 0x1000) >> 12, (reghsiol & 0x2000) >> 13, (reghsiol & 0x8000) >> 15, (reghsiol & 0x10000) >> 16, (reghsiol & 0x20000) >> 17, (reghsiol & 0x40000) >> 18);
	printf("PCIE20_PCS   : %ld   PCIE21_PCS   : %ld   HSIOR_SATA0   : %ld   HSIOR_SATA1   : %ld   PCIE2X_CPHY : %ld   PCIE4X_CPHY : %ld\n", (reghsior & 0x1000) >> 12, (reghsior & 0x2000) >> 13, (reghsior & 0x8000) >> 15, (reghsior & 0x10000) >> 16, (reghsior & 0x20000) >> 17, (reghsior & 0x40000) >> 18);
	printf("\n");
	return 0;
}

int get_hsiortop_status(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char hsiotop_cfg_reg[30];
	char hsiotop_clk_dis_reg[30];
	char hsiotop_gen3_cfg_reg[30];
	char hsiotop_pcie3_clk_dis_reg[30];
	long regcfg, regclkdis, reggen3cfg, regpcie3clkdis;

	system("io -4 0xe0080118 1> /tmp/hsiortop_status.txt 2> /dev/null");
	system("io -4 0xe0080124 1>> /tmp/hsiortop_status.txt 2> /dev/null");
	system("io -4 0xe0080318 1>> /tmp/hsiortop_status.txt 2> /dev/null");
	system("io -4 0xe0080324 1>> /tmp/hsiortop_status.txt 2> /dev/null");
	stat("/tmp/hsiortop_status.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/hsiortop_status.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(hsiotop_cfg_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(hsiotop_clk_dis_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(hsiotop_gen3_cfg_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(hsiotop_pcie3_clk_dis_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	str = strtok(hsiotop_cfg_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regcfg = strtol(str, &p_end, 16);

	str = strtok(hsiotop_clk_dis_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regclkdis = strtol(str, &p_end, 16);

	str = strtok(hsiotop_gen3_cfg_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reggen3cfg = strtol(str, &p_end, 16);

	str = strtok(hsiotop_pcie3_clk_dis_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regpcie3clkdis = strtol(str, &p_end, 16);

	printf("HSIOR_TOP CFG_REG:\n");
	printf("TEST_PWRDWN(IDDQ) : %ld\n", (regcfg & 0x1) >> 0);
	printf("\n");
	printf("HSIOR_TOP CLK_DIS_REG (0=clk enabled, 1=clk disabled):\n");
	printf("SATA1_CLK_DIS     : %ld   SATA0_CLK_DIS   : %ld   XPCS1_CLK_DIS   : %ld   XPCS0_CLK_DIS   : %ld   PCIE1_CLK_DIS   : %ld   PCIE0_CLK_DIS   : %ld\n", (regclkdis & 0x20) >> 5, (regclkdis & 0x10) >> 4, (regclkdis & 0x8) >> 3, (regclkdis & 0x4) >> 2, (regclkdis & 0x2) >> 1, (regclkdis & 0x1) >> 0);
	printf("\n");
	printf("HSIOR_TOP GEN3_CFG_REG:\n");
	printf("TEST_PWRDWN(IDDQ) : %ld\n", (reggen3cfg & 0x1) >> 0);
	printf("\n");
	printf("HSIOR_TOP PCIE3_CLK_DIS_REG (0=clk enabled, 1=clk disabled):\n");
	printf("PCIE1_CLK_DIS     : %ld   PCIE0_CLK_DIS   : %ld\n", (regpcie3clkdis & 0x2) >> 1, (regpcie3clkdis & 0x1) >> 0);
	printf("\n");
	return 0;
}

int get_hsioltop_status(void)
{
	struct stat statbuf;
	FILE *fp = NULL;
	char *p_end;
	char *str;
	char hsiotop_cfg_reg[30];
	char hsiotop_clk_dis_reg[30];
	char hsiotop_gen3_cfg_reg[30];
	char hsiotop_pcie3_clk_dis_reg[30];
	long regcfg, regclkdis, reggen3cfg, regpcie3clkdis;

	system("io -4 0xe00a0118 1> /tmp/hsioltop_status.txt 2> /dev/null");
	system("io -4 0xe00a0124 1>> /tmp/hsioltop_status.txt 2> /dev/null");
	system("io -4 0xe00a0318 1>> /tmp/hsioltop_status.txt 2> /dev/null");
	system("io -4 0xe00a0324 1>> /tmp/hsioltop_status.txt 2> /dev/null");
	stat("/tmp/hsioltop_status.txt", &statbuf);
	if (statbuf.st_size == 0)
		return 0;
	fp = fopen("/tmp/hsioltop_status.txt", "r");
	if (!fp) {
		perror("Error opening file");
		return 0;
	}
	if (!fgets(hsiotop_cfg_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(hsiotop_clk_dis_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(hsiotop_gen3_cfg_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	if (!fgets(hsiotop_pcie3_clk_dis_reg, 30, fp)) {
		perror("Error reading file");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	str = strtok(hsiotop_cfg_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regcfg = strtol(str, &p_end, 16);

	str = strtok(hsiotop_clk_dis_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regclkdis = strtol(str, &p_end, 16);

	str = strtok(hsiotop_gen3_cfg_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	reggen3cfg = strtol(str, &p_end, 16);

	str = strtok(hsiotop_pcie3_clk_dis_reg, " :");
	str = strtok(NULL, " :");
	if (!str) {
		perror("Error string");
		return 0;
	}
	regpcie3clkdis = strtol(str, &p_end, 16);

	printf("HSIOL_TOP CFG_REG:\n");
	printf("TEST_PWRDWN(IDDQ) : %ld\n", (regcfg & 0x1) >> 0);
	printf("\n");
	printf("HSIOL_TOP CLK_DIS_REG (0=clk enabled, 1=clk disabled):\n");
	printf("SATA1_CLK_DIS     : %ld   SATA0_CLK_DIS   : %ld   XPCS1_CLK_DIS   : %ld   XPCS0_CLK_DIS   : %ld   PCIE1_CLK_DIS   : %ld   PCIE0_CLK_DIS   : %ld\n", (regclkdis & 0x20) >> 5, (regclkdis & 0x10) >> 4, (regclkdis & 0x8) >> 3, (regclkdis & 0x4) >> 2, (regclkdis & 0x2) >> 1, (regclkdis & 0x1) >> 0);
	printf("\n");
	printf("HSIOL_TOP GEN3_CFG_REG:\n");
	printf("TEST_PWRDWN(IDDQ) : %ld\n", (reggen3cfg & 0x1) >> 0);
	printf("\n");
	printf("HSIOL_TOP PCIE3_CLK_DIS_REG (0=clk enabled, 1=clk disabled):\n");
	printf("PCIE1_CLK_DIS     : %ld   PCIE0_CLK_DIS   : %ld\n", (regpcie3clkdis & 0x2) >> 1, (regpcie3clkdis & 0x1) >> 0);
	printf("\n");
	return 0;
}

int main(void)
{
	print_header();
	get_chip_id_lgm(NULL, 1);
	get_speed_grade();
	get_p34_version();
	get_epu_status();
	get_rcu_status();
	get_hsiortop_status();
	get_hsioltop_status();
	return 0;
}
