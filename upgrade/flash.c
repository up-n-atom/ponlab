/******************************************************************************
**
** FILE NAME    : flash.c
**
** DATE         : 26 May 2014
** AUTHOR       :
** COPYRIGHT    : Copyright (c) 2014
**                Lantiq Communications
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         $Comment
** 26 May 2014
*******************************************************************************/

#include "cmd_upgrade.h"
#include "common.h"
#include "command.h"
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

int flash_sect_erase(unsigned long addr_first, unsigned long addr_last)
{
	char mtd_dev[16];
	unsigned long part_begin_addr;
	struct mtd_info_user mtd;
	struct erase_info_user erase;
	unsigned long erase_begin, erase_sect_begin;
	unsigned long erase_end, erase_sect_end;
	long preImageSize, postImageSize;
	int dev_fd;
	char *preImage = NULL, *postImage = NULL;
	int blocks = 0;

	memset(mtd_dev, 0x00, sizeof(mtd_dev));
	part_begin_addr = find_mtd(addr_first, mtd_dev);
	if (strcmp(mtd_dev, "") == 0) {
		ifx_debug_printf
		    ("For addr_first, partition can not be found out\n");
		return 1;
	}

	erase_begin = addr_first - part_begin_addr;
	erase_end = addr_last - part_begin_addr;

//      erase_end += 1; /*Kamal: Fix for upgrade problem */

	ifx_debug_printf
	    ("addr_first 0x%08lx, addr_last 0x%08lx, part_begin_addr 0x%08lx, erase_begin 0x%08lx, erase_end 0x%08lx\n",
	     addr_first, addr_last, part_begin_addr, erase_begin, erase_end);

	dev_fd = open(mtd_dev, O_SYNC | O_RDWR);
	if (dev_fd < 0) {
		ifx_debug_printf("The device %s could not be opened\n",
				 mtd_dev);
		return 1;
	}
	/* get some info about the flash device */
	if (ioctl(dev_fd, MEMGETINFO, &mtd) < 0) {
		ifx_debug_printf
		    ("%s This doesn't seem to be a valid MTD flash device!\n",
		     mtd_dev);
		close(dev_fd);
		return 1;
	}

	if (erase_begin >= mtd.size || erase_end > mtd.size
	    || erase_end <= erase_begin) {
		ifx_debug_printf
		    ("Erase begin 0x%08lx or Erase end 0x%08lx are out of boundary of mtd size %d\n",
		     erase_begin, erase_end, mtd.size);
		close(dev_fd);
		return 1;
	}

	erase_sect_begin = erase_begin & ~(mtd.erasesize - 1);
	erase_sect_end = erase_end & ~(mtd.erasesize - 1);
	if (erase_sect_end < erase_end)
		erase_sect_end = erase_sect_end + mtd.erasesize;
	preImageSize = erase_begin - erase_sect_begin;
	if (preImageSize > 0) {
		ifx_debug_printf
		    ("Saving %ld data as erase_begin 0x%08lx is not on sector boundary 0x%08lx\n",
		     preImageSize, erase_begin, erase_sect_begin);
		preImage = (char *)calloc(preImageSize, sizeof(char));
		if (preImage == NULL) {
			ifx_debug_printf
			    ("flash_erase : Could not allocate memory for preImage of size %ld\n",
			     preImageSize);
			close(dev_fd);
			return 1;
		}
		lseek(dev_fd, 0L, SEEK_SET);
		lseek(dev_fd, erase_sect_begin, SEEK_CUR);
		if (read(dev_fd, preImage, preImageSize) != preImageSize) {
			printf
			    ("flash_erase : Could not read %ld bytes of data from %s for preImage\n",
			     preImageSize, mtd_dev);
			close(dev_fd);
			if (preImage)
				free(preImage);
			return 1;
		}
		ifx_debug_printf("flash_erase : read %ld bytes for preImage\n",
				 preImageSize);
	}

	postImageSize = erase_sect_end - erase_end;
	if (postImageSize > 0) {
		ifx_debug_printf
		    ("Saving %ld data as erase_end 0x%08lx is not on sector boundary 0x%08lx\n",
		     postImageSize, erase_end, erase_sect_end);
		postImage = (char *)calloc(postImageSize, sizeof(char));
		if (postImage == NULL) {
			ifx_debug_printf
			    ("flash_erase : Could not allocate memory for postImage of size %ld\n",
			     postImageSize);
			close(dev_fd);
			if (preImage)
				free(preImage);
			return 1;
		}
		lseek(dev_fd, 0L, SEEK_SET);
		lseek(dev_fd, erase_end, SEEK_CUR);
		if (read(dev_fd, postImage, postImageSize) != postImageSize) {
			printf
			    ("flash_erase : Could not read %ld bytes of data from %s for preImage\n",
			     postImageSize, mtd_dev);
			close(dev_fd);
			if (preImage)
				free(preImage);
			if (postImage)
				free(postImage);
			return 1;
		}
		ifx_debug_printf("flash_erase : read %ld bytes for postImage\n",
				 postImageSize);
	}

	blocks = (erase_sect_end - erase_sect_begin) / mtd.erasesize;
	erase.start = erase_sect_begin;
	erase.length = mtd.erasesize * blocks;
	if (ioctl(dev_fd, MEMERASE, &erase) < 0) {
		ifx_debug_printf
		    ("Error : While erasing 0x%.8x-0x%.8x on %s: %m\n",
		     (unsigned int)erase.start,
		     (unsigned int)(erase.start + erase.length), mtd_dev);
		close(dev_fd);
		free(postImage);
		free(preImage);
		return 1;
	}

	if (preImageSize > 0) {
		ifx_debug_printf
		    ("Writing back %ld data as erase_begin 0x%08lx is not on sector boundary 0x%08lx\n",
		     preImageSize, erase_begin, erase_sect_begin);
		lseek(dev_fd, 0L, SEEK_SET);
		lseek(dev_fd, erase_sect_begin, SEEK_CUR);
		preImageSize = write(dev_fd, preImage, preImageSize);
		ifx_debug_printf("Wrote back at 0x%08lx size %ld\n",
				 erase_sect_begin, preImageSize);
		free(preImage);
	}

	if (postImageSize > 0) {
		ifx_debug_printf
		    ("Writing back %ld data as erase_end 0x%08lx is not on sector boundary 0x%08lx\n",
		     postImageSize, erase_end, erase_sect_end);
		lseek(dev_fd, 0L, SEEK_SET);
		lseek(dev_fd, erase_end, SEEK_CUR);
		postImageSize = write(dev_fd, postImage, postImageSize);
		ifx_debug_printf("Wrote back at 0x%08lx size %ld\n", erase_end,
				 postImageSize);
		free(postImage);
	}

	close(dev_fd);
	return 0;
}

#define PASS_SIZE 0x10000

int flash_write(unsigned long srcAddr, unsigned long destAddr, int srcLen)
{
	char mtd_dev[16];
	unsigned long part_begin_addr;
	int bWrote = 0;
	int dev_fd;
	int i, writeLen, nPass;

	memset(mtd_dev, 0x00, sizeof(mtd_dev));
	part_begin_addr = find_mtd(destAddr, mtd_dev);
	if (strcmp(mtd_dev, "") == 0) {
		ifx_debug_printf("For srcAddr, partition can not be found out\n");
		return 1;
	}
	destAddr -= part_begin_addr;

	dev_fd = open(mtd_dev, O_SYNC | O_RDWR);
	if (dev_fd < 0) {
		ifx_debug_printf("The device %s could not be opened\n",
				 mtd_dev);
		return 1;
	}

	nPass = (int)(srcLen / PASS_SIZE) + 1;
	for (i = 0; i < nPass; i++) {
		if (srcLen > PASS_SIZE)
			writeLen = PASS_SIZE;
		else
			writeLen = srcLen;
		lseek(dev_fd, 0L, SEEK_SET);
		lseek(dev_fd, destAddr + i * PASS_SIZE, SEEK_CUR);
		if ((bWrote =
		     write(dev_fd, (char *)srcAddr + i * PASS_SIZE,
			   writeLen)) < writeLen) {
			ifx_debug_printf
			    ("Error : Only %d outof %d bytes could be written into %s\n",
			     i * PASS_SIZE + bWrote, srcLen + i * PASS_SIZE,
			     mtd_dev);
			close(dev_fd);
			return 1;
		}
		ifx_debug_printf("flash_write : Wrote %d bytes\n", bWrote);
		srcLen -= writeLen;
		printf("#");
	}
	printf("\n");
	close(dev_fd);
	return 0;
}

void flash_sect_protect(int mode, unsigned long addr_first,
			unsigned long addr_last)
{
	(void)mode; (void)addr_first; (void)addr_last ;
	return;
}

int nand_flash_write(ulong srcAddr, ulong destAddr, int srcLen)
{
	(void)srcAddr;
	(void)destAddr;
	(void)srcLen;
        return 0;
}
