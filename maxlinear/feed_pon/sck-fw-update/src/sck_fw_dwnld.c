/*****************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <byteswap.h>
#include <time.h>
#include <sys/time.h>
#include "lif_api.h"

//#define PRINT printf
#define PRINT

#define SB_PDI_CTRL 0xE100
#define SB_PDI_ADDR 0xE101
#define SB_PDI_DATA 0xE102
#define SB_PDI_STAT 0xE103

#define SB1_ADDR 0x7800
#define SB_PDI_CTRL_RD 0x01
#define SB_PDI_CTRL_WR 0x02
#define SB_PDI_RST 0x0

#define FW_DL_MDIO_START_MAGIC 0xF48F
#define FW_DL_FLASHLESS_MAGIC 0xC33C
#define RESCUE_IMAGE_HEADER_SIZE 20

#define sys_le16_to_cpu(val) __bswap_16(val)
#define sys_cpu_to_le16(val) __bswap_16(val)

#define sys_le32_to_cpu(val) __bswap_32(val)
#define sys_cpu_to_le32(val) __bswap_32(val)

#define SMDIO_PHY_NR 8

extern int opt_dryrun;

static uint8_t SMDIO_PHY_ID[SMDIO_PHY_NR] = { 16, 17, 18, 19, 20, 21, 22, 23 };

static void msleep(uint32_t ms)
{
	struct timespec tv;
	/* Construct the timespec from the number of whole seconds... */
	tv.tv_sec = ms / 1000;
	/* ... and the remainder in nanoseconds. */
	tv.tv_nsec = (long) ((ms - (tv.tv_sec * 1000)) * 1000 * 1000);
	nanosleep(&tv, &tv);
}

/**
 * Reset SB PDI registers
 */
static void smdio_ssb_pdi_reset(uint8_t lif_id, uint8_t phy_id)
{
	smdio_write(lif_id, phy_id, SB_PDI_CTRL, SB_PDI_RST);
	smdio_write(lif_id, phy_id, SB_PDI_ADDR, SB_PDI_RST);
	smdio_write(lif_id, phy_id, SB_PDI_DATA, SB_PDI_RST);
}

static bool wait_pdi_stat_is(uint8_t lif_id, uint32_t phy_id, uint16_t exp_val)
{
	/* Test 10 seconds to avoid endless loop */
	int loop = 1000;
	uint16_t sb_pdi_stat;

	while (loop > 0) {
		msleep(50);
		if (smdio_read(lif_id, phy_id, SB_PDI_STAT) == exp_val) {
			return true;
		}
		if (opt_dryrun)
			return true;
		msleep(50);
		loop--;
		if (loop % 400 == 0)
			printf(".");
	}
	printf("PDI_STAT not %x within 10 secs\n", exp_val);
	return false;
}

static int wait_pdi_stat_is_not(uint8_t lif_id, uint32_t phy_id,
	uint16_t exp_val, uint16_t *ret_val)
{
	/* Test 1 second to avoid endless loop */
	int loop = 50;
	uint16_t sb_pdi_stat;

	while (loop > 0) {
		msleep(20);
		sb_pdi_stat = smdio_read(lif_id, phy_id, SB_PDI_STAT);

		if (sb_pdi_stat != exp_val) {
			*ret_val = sb_pdi_stat;
			return 0;
		}
		loop--;
	}

	return -1;
}

/**
 * Check ID and FW registers after FW Download
 */
int check_registers(uint8_t lif_id)
{
	int ret, i;

	for (int i = 0; i < SMDIO_PHY_NR; i++) {
		if ((mdio_read(lif_id, SMDIO_PHY_ID[i], 3) == 0xffff) ||
		    (mdio_read(lif_id, SMDIO_PHY_ID[i], 0x1e) == 0xffff))
			return -1;
		i++;
	}
	return 0;
}

/**
 * Write data to SSB from offset 0x0
 *
 * pdata - data pointer to be writen to SB
 * len   - size of data
 *
 * return size of data writen to SB if successful
 */
int smdio_ssb_write(uint8_t lif_id, uint8_t phy_id, uint8_t *pdata,
	uint32_t len)
{
	uint32_t word_idx = 0,
			 data_size = 0;
	uint16_t data_arr[8] = {0};
	uint16_t sb_pdi_stat;
	uint8_t num = 0;
	uint32_t idx = 0;
	int ret;

	/* Trigger write */
	smdio_write(lif_id, phy_id, SB_PDI_CTRL, SB_PDI_CTRL_WR);

	while (idx < len) {
		num = 0;
		do {
			/* 16 bits data */
			uint16_t fdata = 0x0;

			if (idx + 1 < len) {
				fdata = ((pdata[idx + 1]) << 8) | pdata[idx];
				idx += 2;
				data_size += 2;
			} else if (idx < len) {
				/* last byte of data,
				 * padding high 8 bits with 0s
				 */
				fdata |= pdata[idx++];
				data_size++;
			} else {
				/* no more data */
				break;
			}
			data_arr[num] = fdata;
			num++;
		} while (num < 8);

		smdio_cont_write(lif_id, phy_id, SB_PDI_DATA, data_arr, num);
		word_idx += num;
		/* check download is completed? */
		if (idx >= len) {
			// Send data size to target MCUBoot
			smdio_ssb_pdi_reset(lif_id, phy_id);
			/* last batch data */
			smdio_write(lif_id, phy_id, SB_PDI_STAT, data_size);
			break;
		}

		if (word_idx == 16384) {
			/* 32KB is done, need to set SB PDI addr to 0x7800 */
			smdio_write(lif_id, phy_id, SB_PDI_CTRL, SB_PDI_RST);
			smdio_write(lif_id, phy_id, SB_PDI_ADDR, SB1_ADDR);
			// Continue to write SB1 32KB
			smdio_write(lif_id, phy_id, SB_PDI_CTRL,
				SB_PDI_CTRL_WR);
		} else if (word_idx == 32760) {
			/* One slice is done: 32768 - 8 Word */
			smdio_ssb_pdi_reset(lif_id, phy_id);
			// Send data size to target MCUBoot
			smdio_write(lif_id, phy_id, SB_PDI_STAT, data_size);
			/* 64KB - 16B */
			word_idx = 0;
			data_size = 0;
			/*
			 * Paused here to wait for target MCUBoot to
			 * program flash, and then continue
			 */
			if (wait_pdi_stat_is(lif_id, phy_id, 0)) {
				//Trigger write
				smdio_write(lif_id, phy_id, SB_PDI_CTRL,
					SB_PDI_CTRL_WR);
			} else{
				printf("ERROR wait_pdi_stat_is not 0\n");
				return -ETIMEDOUT;
			}
		}
		if (idx % 5000 == 0)
			printf(".");
	}

	ret = wait_pdi_stat_is_not(lif_id, phy_id, data_size, &sb_pdi_stat);
	if (ret == 0)
		/* write to avoid slave hang */
		smdio_write(lif_id, phy_id, SB_PDI_STAT, 0x3CC3);
	if (ret == 0 && sb_pdi_stat == 0)
		return 0;
	else
		return -1;
}

int ssb_fw_download(void *ctx, uint8_t lif_id, uint8_t phy_id, uint8_t *pdata)
{
	uint16_t sb_pdi_stat = FW_DL_MDIO_START_MAGIC;
	uint32_t image_type, image_size_1, image_checksum_1, image_size_2,
			image_checksum_2;
	uint16_t *pimage_header;
	int rc = 0;
	int i;
	uint16_t dst_array[RESCUE_IMAGE_HEADER_SIZE];
	uint16_t *psrc;
	uint16_t *pdst = dst_array;

	image_type =  sys_cpu_to_le32(*(uint32_t *)pdata);
	image_size_1 = sys_cpu_to_le32(*((uint32_t *)pdata + 1));
	image_checksum_1 = sys_cpu_to_le32(*((uint32_t *)pdata + 2));
	image_size_2 = sys_cpu_to_le32(*((uint32_t *)pdata + 3));
	image_checksum_2 = sys_cpu_to_le32(*((uint32_t *)pdata + 4));
	printf("image type: %x, size 1: %x, checksum 1: %x, size 2: %x, checksum 2: %x\n",
		image_type, image_size_1, image_checksum_1,
		image_size_2, image_checksum_2);
	wait_pdi_stat_is(lif_id, phy_id, 0xc55c);
	smdio_ssb_pdi_reset(lif_id, phy_id);

	// Send START signal to target which is rescue mode
	smdio_write(lif_id, phy_id, SB_PDI_STAT, sb_pdi_stat);

	sb_pdi_stat += 1;
	if (!wait_pdi_stat_is(lif_id, phy_id, sb_pdi_stat))
		return -ETIMEDOUT;
	sb_pdi_stat = smdio_read(lif_id, phy_id, SB_PDI_STAT);
	/* send image header, do little endian conversion (20 bytes) */
	psrc = (uint16_t *)pdata;
	for (i = 0; i < RESCUE_IMAGE_HEADER_SIZE / 2; i++) {
		*pdst = sys_cpu_to_le16 (*psrc);
		psrc++;
		pdst++;
	}
	pimage_header = dst_array;

	/* Trigger write */
	smdio_write(lif_id, phy_id, SB_PDI_CTRL, SB_PDI_CTRL_WR);
	smdio_cont_write(lif_id, phy_id, SB_PDI_DATA, pimage_header, 8);
	smdio_cont_write(lif_id, phy_id, SB_PDI_DATA, pimage_header + 8, 2);
	smdio_ssb_pdi_reset(lif_id, phy_id);
	smdio_write(lif_id, phy_id, SB_PDI_STAT, RESCUE_IMAGE_HEADER_SIZE);

	sb_pdi_stat = RESCUE_IMAGE_HEADER_SIZE + 1;
	printf("erasing flash\n");
	if (!wait_pdi_stat_is(lif_id, phy_id, sb_pdi_stat)) {
		printf("ERROR wait for PDI_STAT to be %x\n", sb_pdi_stat);
		return -ETIMEDOUT;
	}
	/* skip image headers */
	pdata += RESCUE_IMAGE_HEADER_SIZE;

	if (wait_pdi_stat_is(lif_id, phy_id, 0)) {
		printf("download file\n");
		rc = smdio_ssb_write(lif_id, phy_id, pdata,
			image_size_1 + image_size_2);
	} else{
		printf("ERROR wait for PDI_STAT timeout\n");
		return -ETIMEDOUT;
	}

	return rc;
}
