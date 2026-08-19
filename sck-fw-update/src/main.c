/*****************************************************************************
 *
 * Copyright (c) 2023 - 2024 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <byteswap.h>
#include <unistd.h>
#include "lif_api.h"

#define LID  3
#define PHY  0
#define DEV_IDX 0

/** what string support, version string */
const char sck_whatversion[] = "@(#)MaxLinear MxL862xx Firmware Update, Version "
				VERSION;

int opt_dryrun;

/* expect VERSION given by makefile */

void print_usage(char *bin_name)
{
	printf("Usage: %s [-d] ", bin_name);
	printf("[ -h | -V | <FW file> ");
	printf("| -P1 <PHY> <mmd> <reg> ");
	printf("| -P2 <PHY> <mmd> <reg> <val> ]\n");
}

void print_help(char *bin_name)
{
	print_usage(bin_name);

	printf("MxL862X2 FW version -V:\n");
	printf("%s -V\n", bin_name);
	printf("MxL862X2 FW download:\n");
	printf("%s filename\n", bin_name);
	printf("For option Read/Write PHY register, "
		"refer to MxL862X2 PHY register spec!\n");
	printf("Example, values in dec\n");
	printf("Read PHY 0 mmd 30 value of register 2:\n");
	printf("%s -P1 0 30 2\n", bin_name);
	printf("Write 896 to PHY 0 mmd 30 value register 2:\n");
	printf("%s -P2 0 30 2 896\n", bin_name);
}

void print_fw_version(void *ctx)
{
	int ret;
	struct sys_fw_image_version sys_img_ver = {0};

	ret = gsw_api_cmd(ctx, SYS_MISC_FW_VERSION,
			    &sys_img_ver, sizeof(sys_img_ver),
			    0, sizeof(sys_img_ver));
	if (ret < 0) {
		printf("fw_version failed with ret code %x\n", ret);
		return;
	}
	printf("Version: %u.%u.%u.%u\n",
		sys_img_ver.iv_major,
		sys_img_ver.iv_minor,
		__bswap_16(sys_img_ver.iv_revision),
		__bswap_32(sys_img_ver.iv_build_num));
}

void prx_phy_read(void *ctx, char *port, char *mmd, char *reg)
{
	int ret;
	struct mdio_relay_data phy_reg_access = {0};

	phy_reg_access.phy  = atoi(port); //u8
	phy_reg_access.mmd  = atoi(mmd); //u16
	phy_reg_access.reg  = atoi(reg); //u16
	ret = gsw_api_cmd(ctx,
			  INT_GPHY_READ,
			  (void*)&phy_reg_access,
			  sizeof(phy_reg_access),
			  0,
			  sizeof(phy_reg_access));
	if (ret < 0) {
		printf("Failed Read GSW Dev %d\n", ret);
		return;
	}
	printf("Phy=%u mmdDevice=%u Register=%u value=%u\n",
		phy_reg_access.phy,
		phy_reg_access.mmd,
		phy_reg_access.reg,
		phy_reg_access.data);
}

void prx_phy_write(void *ctx, char *port, char *mmd, char *reg, char *data)
{
	int ret;
	struct mdio_relay_data phy_reg_access = {0};

	phy_reg_access.phy  = atoi(port); //u8
	phy_reg_access.mmd  = atoi(mmd); //u16
	phy_reg_access.reg  = atoi(reg); //u16
	phy_reg_access.data  = atoi(data); //u16
	ret = gsw_api_cmd(ctx,
			  INT_GPHY_WRITE,
			  (void*)&phy_reg_access,
			  sizeof(phy_reg_access),
			  0,
			  0);
	if (ret < 0) {
		printf("Failed Write GSW Dev %d\n", ret);
		return;
	}
}

void fw_update_enter(void *ctx)
{
	/* This command will timeout in normal cases,
	   so ignore that return value (ETIMEDOUT) */
	gsw_api_cmd(ctx, SYS_MISC_FW_UPDATE, NULL, 0, 0, 0);
}

int fw_load(void *ctx, char *fw_path)
{
	int ret, i;
	FILE *fwin;
	int filesize;
	uint8_t *pDataBuf;

	fwin = fopen(fw_path, "rb");
	if (fwin == NULL) {
		printf("Failed to open FW file \"%s\".\n", fw_path);
		return -errno;
	}
	fseek(fwin, 0L, SEEK_END);
	filesize = ftell(fwin);
	rewind(fwin);

	pDataBuf = (uint8_t *)malloc(filesize);
	if (pDataBuf == NULL)
		return -errno;
	ret = fread(pDataBuf, 1, filesize, fwin);
	fclose(fwin);
	if (ret != filesize) {
		free(pDataBuf);
		return -errno;
	}

	ret = ssb_fw_download(ctx, LID, PHY, (uint8_t *) pDataBuf);
	free(pDataBuf);
	if (ret != 0) {
		printf("FW Upload Failed - FW Write Failed\n");
		return -errno;
	}
	/* Wait for PHY to start */
	sleep(2);
	ret = check_registers(LID);
	if (ret != 0) {
		printf("FW Upload Failed - Register Read Failed\n");
		return -errno;
	}

	printf("FW Upload Sucessful\n");
}

int main(int argc, char *argv[])
{
	void *ctx;
	int ret = 0;
	int arg_fw = 0;
	int opt_v = 0;
	int opt_p = 0;
	int num_param, j;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				if (argv[i][1] == 'V')
					opt_v = 1;
				if (argv[i][1] == 'h') {
					print_help(argv[0]);
					goto exit_help;
				}
				if (argv[i][1] == 'P')
					if (argv[i][2] == '1') {
						opt_p = 1;
						j = i;
					} else if (argv[i][2] == '2') {
						opt_p = 2;
						j = i;
					} else {
						opt_p = 3;
						j = i;
					}
				if (argv[i][1] == 'd')
					opt_dryrun = 1;
			} else {
				arg_fw = i;
				num_param = i;
			}
		}
	}
	ctx = lif_mdio_open(DEV_IDX, LID, PHY);
	if (ctx == NULL)
		return -1;
	if (opt_v == 1) {
		printf("Version %s (%s)\n", VERSION, __TIME__);
		if (!opt_dryrun)
			print_fw_version(ctx);
		goto exit_main;
	}
	if (opt_p == 1) {
		printf("Option Read PHY register, "
			"refer to MxL862X2 PHY register spec\n");
		if (!opt_dryrun) {
			if (num_param != 4) {
				printf("Wrong number of parameters "
					"to read the PHY register, see -h\n");
			} else {
				prx_phy_read(ctx, argv[j + 1], argv[j + 2],
					     argv[j + 3]);
			}
		}
		goto exit_main;
	} else if (opt_p == 2) {
		if (ctx == NULL)
			return -1;
		printf("Option Write PHY register, "
			"refer to MxL862X2 PHY register spec\n");
		if (!opt_dryrun) {
			if (num_param != 5) {
				printf("Wrong number of parameters "
					"to write the PHY register, see -h\n");
			} else {
				prx_phy_write(ctx, argv[j + 1], argv[j + 2],
					      argv[j + 3], argv[j + 4]);
			}
		}
		goto exit_main;
	} else if (opt_p == 3) {
		printf("Inavlid -P Option\n");
		goto exit_main;
	} else if (arg_fw != 0) {
		if (opt_dryrun) {
			printf("dry run only\n");
		} else {
			fw_update_enter(ctx);
			sleep(2);
		}
		ret = fw_load(ctx, argv[arg_fw]);
	} else {
		print_usage(argv[0]);
		ret = -1;
		goto exit_main;
	}
exit_main:
	lif_mdio_close(ctx);
exit_help:
	return ret;
}
