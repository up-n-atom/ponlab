/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "common.h"
#include "command.h"
#include <sys/ioctl.h>
#include "crc32.h"
#include "cmd_upgrade.h"	

#define	PASS_SIZE	65535

int main(int argc, char *argv[])
{
	char strimg_addr[32];
	char strimg_size[32];
	char *img_addr = NULL;
	//char *img_size = NULL;
	unsigned long part_begin_addr = 0;
	char mtd_dev[16];
	int dev_fd, file_fd;
	unsigned long limg_addr, limg_size;
	char srcBuf[PASS_SIZE];
	int readLen, bWrote, bread;
	int nPass, i;
	struct conf_header header;	
	unsigned long crc = 0;	

	if (argc != 3) {
		printf("Usage : read_image image_name output_file\n");
		return 1;
	}
#ifdef CONFIG_BOOT_FROM_NAND
	char temp_buf[256];
	temp_buf[0] = '\0';
	if (!strcmp(argv[1], "sysconfig")) {
  #ifdef CONFIG_TARGET_UBI_MTD_SUPPORT
	sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt read_cfg_gz sysconfig %s", argv[2]);
  #else
		sprintf_s(temp_buf, sizeof(temp_buf), "cp /etc/sysconfig/rc.conf.current.gz %s 2>&-; sync",	argv[2]);
  #endif
		if (system(temp_buf)) return 1;

		return 0;
  #ifdef CONFIG_TARGET_UBI_MTD_SUPPORT
	} else if (!strcmp(argv[1], "wlanconfig") || !strcmp(argv[1], "dectconfig")) {
		//sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt read_cfg_gz %s %s",
		//	argv[1], argv[2]);

		//read from calibration mtd partition. Failsafe added on UBI volumes (backward compatability).
		sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt read_calibration %s %s",
			argv[1], argv[2]);
		if (system(temp_buf)) return 1;

		return 0;
  #endif
	}
#endif

	if (read_env()) {
		return 1;
	}

	memset(strimg_addr, 0x00, sizeof(strimg_addr));
	memset(strimg_size, 0x00, sizeof(strimg_size));
	sprintf_s(strimg_addr, sizeof(strimg_addr), "f_%s_addr", argv[1]);
	sprintf_s(strimg_size, sizeof(strimg_size), "f_%s_size", argv[1]);

	if ((img_addr = (char *)get_env(strimg_addr)) == NULL) {
		printf("Could not find the variable %s for the image %s\n",
		       strimg_addr, argv[1]);
		return 1;
	}
	limg_addr = strtoul(img_addr, NULL, 16);
	memset(mtd_dev, 0x00, sizeof(mtd_dev));
	part_begin_addr = find_mtd(limg_addr, mtd_dev);

	if (strcmp(mtd_dev, "") == 0) {
		printf
		    ("mtd device could not be found for image %s with addr 0x%08lx\n",
		     argv[1], limg_addr);
		return 1;
	}

	dev_fd = open(mtd_dev, O_SYNC | O_RDWR);
	if (dev_fd <= 0) {
		printf("Could not open device %s for image %s\n", mtd_dev,
		       argv[1]);
		close(dev_fd);
		return 1;
	}

	if (lseek(dev_fd, limg_addr - part_begin_addr, SEEK_CUR) < 0) {
		printf
		    ("Could not seek to the position 0x%08lx in device %s from start address 0x%08lx\n",
		     limg_addr, mtd_dev, part_begin_addr);
		close(dev_fd);
		return 1;
	}

	file_fd = open(argv[2], O_CREAT | O_RDWR);
	if (file_fd <= 0) {
		printf("Could not open file %s for image %s\n", argv[2],
		       argv[1]);
		close(dev_fd);
		close(file_fd);
		return 1;
	}
	if (read(dev_fd, (void *)&header, sizeof(header)) < (int)sizeof(header)) {
		printf("Error : read header fail\n");
		close(dev_fd);
		close(file_fd);
		return 1;
	}
	limg_size = header.size + sizeof(header);
	if (limg_size == 0xffffffff) {
		//printf("Error : empty data\n");
		close(dev_fd);
		close(file_fd);
		return 1;
	}
	crc = 0x00000000 ^ 0xffffffff;

	nPass = (int)(limg_size / PASS_SIZE) + 1;
	for (i = 0; i < nPass; i++) {
		if (limg_size > PASS_SIZE)
			readLen = PASS_SIZE;
		else
			readLen = limg_size;

		if (i == 0)	//remove header
		{
			readLen -= sizeof(header);
		}

		memset(srcBuf, 0x00, sizeof(srcBuf));
		if ((bread = read(dev_fd, (void *)srcBuf, readLen)) < readLen) {
			printf
			    ("Error : Only %d outof %ld bytes could be read from %s\n",
			     i * PASS_SIZE + readLen, limg_size + i * PASS_SIZE,
			     mtd_dev);
			close(dev_fd);
			close(file_fd);
			return 1;
		}

		crc = crc32(crc, srcBuf, bread);	

		if ((bWrote = write(file_fd, (void *)srcBuf, bread)) < bread) {
			printf
			    ("Error : Only %d outof %ld bytes could be written into %s\n",
			     i * PASS_SIZE + bWrote, limg_size + i * PASS_SIZE,
			     argv[2]);
			close(dev_fd);
			close(file_fd);
			return 1;
		}
		ifx_debug_printf("flash_write : Wrote %d bytes\n", bWrote);
		limg_size -= readLen;
		if (i == 0)	
			limg_size -= sizeof(header);	
	}
	close(dev_fd);
	close(file_fd);

	crc ^= 0xffffffff;
	if (header.crc != crc) {
		ifx_debug_printf("Read image fail, crc error.\n");
		remove(argv[2]);
		return 1;
	} else			
	{
		ifx_debug_printf
		    ("Successfully read %ld bytes of image %s into file %s\n",
		     limg_size, argv[1], argv[2]);
	}
	return 0;
}
