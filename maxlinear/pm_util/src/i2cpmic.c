/*
 *  Copyright 2021 MaxLinear, Inc.
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
#include <dirent.h>
#include "include/ltq_pmutil_interface.h"

enum {
	CPUM0,
	CPUM1,
	ADP,
	ROC,
	MAX_VALUE_POWER_RAIL
};

static const char *power_rail_addr[MAX_VALUE_POWER_RAIL] = {
	[CPUM0] = "0xa",
	[CPUM1] = "0xe",
	[ADP] = "0x12",
	[ROC]  = "0x16"
};

#define MIN_POWER_RAIL_VOL 500
#define MAX_POWER_RAIL_VOL 1200
#define BUF_SIZE 40
#define BASE_VOL 500
#define VOL_STEP 5

static int verify_vol(unsigned int vol)
{
	if ((vol < MIN_POWER_RAIL_VOL) || (vol > MAX_POWER_RAIL_VOL)) {
		printf("wrong number of voltage, range is 500-1200\n");
		return -EINVAL;
	}

	if ((vol % 5) != 0) {
		printf("wrong number of voltage, voltage step is 5\n");
		return -EINVAL;
	}
	return 0;
}

int i2c_pmic_set_vol(char *power_rail, unsigned int vol)
{
	char str_set_vol[BUF_SIZE];
	int ret;

	memset(str_set_vol, 0, sizeof(str_set_vol));
	if (verify_vol(vol))
		return -EINVAL;

	//convert voltage value to register value
	if (vol != 0)
		vol = (vol - BASE_VOL) / VOL_STEP + 1;

	if ((strcmp(power_rail, "CPUM0") == 0) || (strcmp(power_rail, "cpum0") == 0)) {
		sprintf_s(str_set_vol, BUF_SIZE - 1, "i2cset -fy 0 6a %s 0x%x", power_rail_addr[CPUM0], vol);
	} else if ((strcmp(power_rail, "CPUM1") == 0) || (strcmp(power_rail, "cpum1") == 0)) {
		sprintf_s(str_set_vol, BUF_SIZE - 1, "i2cset -fy 0 6a %s 0x%x", power_rail_addr[CPUM1], vol);
	} else if ((strcmp(power_rail, "ADP") == 0) || (strcmp(power_rail, "adp") == 0)) {
		sprintf_s(str_set_vol, BUF_SIZE - 1, "i2cset -fy 0 6a %s 0x%x", power_rail_addr[ADP], vol);
	} else if ((strcmp(power_rail, "ROC") == 0) || (strcmp(power_rail, "roc") == 0)) {
		sprintf_s(str_set_vol, BUF_SIZE - 1, "i2cset -fy 0 6a %s 0x%x", power_rail_addr[ROC], vol);
	} else {
		printf("wrong power rail name was given. Use [CPUM0|CPUM1|ADP|ROC]\n");
	}

	ret = system(str_set_vol);
	if (ret == -1)
		printf("calling i2c cmd failed");

	return 0;
}

int i2c_pmic_get_vol(void)
{
	FILE *fp1;
	char str_get_vol[BUF_SIZE], i2c_cmd[BUF_SIZE];
	int  i, vol;

	for (i = CPUM0; i < MAX_VALUE_POWER_RAIL; ++i) {
		memset(str_get_vol, 0, sizeof(str_get_vol));
		memset(i2c_cmd, 0, sizeof(i2c_cmd));

		sprintf_s(i2c_cmd, BUF_SIZE - 1, "i2cget -fy 0 6a %s", power_rail_addr[i]);
		fp1 = popen(i2c_cmd, "r");
		if (!fp1)
			return -EINVAL;
		fgets(str_get_vol, sizeof(str_get_vol), fp1);
		vol = (int)strtol(str_get_vol, NULL, 16);
		vol = (vol - 1) * VOL_STEP + BASE_VOL;

		if (i == CPUM0)
			printf("CPUM0 = %dmV\n", vol);
		else if (i == CPUM1)
			printf("CPUM1 = %dmV\n", vol);
		else if (i == ADP)
			printf("ADP   = %dmV\n", vol);
		else if (i == ROC)
			printf("ROC   = %dmV\n", vol);
		pclose(fp1);
	}
	return 0;
}

