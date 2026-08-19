/******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.
  Copyright (C) 2019 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sec_upgrade.c 					       *
 *     Project    : UGW                                                        *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include "type-up.h"
#include <mtd/mtd-user.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>

#include <fapi_sec_service.h>
#ifdef PLATFORM_LGM
#include <fapi_img_auth.h>
#include <sec_upgrade.h>
#endif

#include "safe_str_lib.h"

static bool activate=true;

#ifndef PLATFORM_LGM
static int upgrade_sec_img(ulong src_addr, ulong src_len, char *img_name)
{
	FILE *fp;
	char strimg_dataout[100];
	image_header_t *pimg_header = NULL;

	pimg_header = (image_header_t *)src_addr;

	if (ntohl(pimg_header->img_hdr_magic) == IMG_HDR_MAGIC) {
		printf("Image contains header with name [%s]\n",pimg_header->img_hdr_name);
		if(pimg_header->img_hdr_type != IMG_HDR_VAR_KERNEL) {
			printf("This is not kernel image and so removing header\n");
			src_addr += sizeof(*pimg_header);
			src_len -= sizeof(*pimg_header);
		}
	}
	if (activate) {
		sprintf_s(strimg_dataout, sizeof(strimg_dataout), "/usr/sbin/vol_mgmt upgrade_dualvol_stdin %s %u %s", img_name, (size_t)src_len, pimg_header->img_hdr_name);
	} else {
		sprintf_s(strimg_dataout, sizeof(strimg_dataout), "/usr/sbin/vol_mgmt upgrade_dualvol_stdin noactivation %s %u %s", img_name, (size_t)src_len, pimg_header->img_hdr_name);
	}
	fp = popen (strimg_dataout, "w");
	if (fp == NULL) {
		fprintf(stderr, "upgrade %s: unable to open file for writing\n", img_name);
		return 1;
	} else {
		if(fwrite((void *)src_addr, sizeof(char), (size_t)src_len, fp) != src_len) {
			fprintf(stderr, "upgrade %s: unable to complete writing\n", img_name);
			pclose(fp);
			return 1;
		}
		pclose(fp);
	}
	return 0;
}
#endif
#ifdef PLATFORM_LGM
static int set_udt(char *img_name, int status)
{
	FILE *fp ;
	char cmd[100];

#ifdef LINUX_UPGRADE
	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --set --name %s --value %d", img_name, status);
#else
	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --set --name udt_file --value %s", img_name);
	fp = popen(cmd, "w");
	if (fp == NULL) {
		printf("Error launching the cmd to update the uboot environment\n");
		return 1;
	}
	if( pclose(fp) < 0 ) {
		printf("Error setting the environement variable\n");
		return 1;
	}
	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/uboot_env --set --name udt_status --value %d", status);
#endif
	fp = popen(cmd, "w");
	if (fp == NULL) {
		printf("Error launching the cmd to update the uboot environment\n");
		return 1;
	}
	if( pclose(fp) < 0 ) {
		printf("Error setting the environement variable\n");
		return 1;
	}
	return 0;
	
}
#endif

static int do_sec_upgrade(const char *path)
{
#if !defined(PLATFORM_LGM) || defined(LINUX_UPGRADE)
	uint32_t cur_par_size=0, pad;
	unsigned char *header = NULL;
	uint32_t file_read_size =0, total_file_read_size = 0;
#endif
	image_header_t x_img_header;
	char name[16];
	int ret = 0;
	int file_fd = 0;
	struct stat filestat={0};

	img_param_t img;
#ifdef LINUX_UPGRADE
	img_param_t img_param;
#endif

	file_fd = open(path, O_RDONLY);
	if (file_fd < 0) {
		fprintf(stderr, "The file %s could not be opened\n", path);
		return 1;
	}

	if (fstat(file_fd, &filestat)) {
		printf("fstat error: [%s]\n",strerror(errno));
		close(file_fd);
		return 1;
	}

	img.src_img_fd=file_fd;
	img.src_img_len=filestat.st_size;
	if (!img.src_img_len) {
		printf("EMPTY file ... \n");
		ret = 1;
		goto finish;
	} else {
		img.src_img_addr = mmap(0, img.src_img_len, PROT_READ, MAP_SHARED, img.src_img_fd, 0);
		if(img.src_img_addr == MAP_FAILED) {
			printf("MMAP failed... %s\n",strerror(errno));
			ret = 1;
			goto finish;
		}
	}
#if !defined(PLATFORM_LGM) || defined(LINUX_UPGRADE)
#if !defined(NOAUTH) && !defined(LINUX_UPGRADE)
	ret = fapi_ssImgAuth(img);
	if (ret != 0) {
		printf("Image validation failed");
		if( munmap(img.src_img_addr, img.src_img_len) == -1)
			printf("munmap failed\n");
		ret = 1;  /* reboot not required */
		goto finish;
	}
#endif
	/* make sure secure header removed before writing to flash */
	header =  img.src_img_addr;

	do {
		x_img_header = *((image_header_t *)header);

		if(x_img_header.img_hdr_type == IMG_HDR_VAR_MULTI){
			cur_par_size = sizeof(image_header_t) + 8;
			total_file_read_size += cur_par_size;
			header =  img.src_img_addr + total_file_read_size;
			continue;
		}

		cur_par_size = sizeof(image_header_t) + ntohl(x_img_header.img_hdr_size);
		pad = (16 - (cur_par_size % 16)) % 16;
		header =  img.src_img_addr + total_file_read_size;
		file_read_size = cur_par_size + pad;
#else
	x_img_header = *((image_header_t *)img.src_img_addr);
#endif

#ifdef LINUX_UPGRADE
		if (ntohl(*(uint32_t *)header) == FLATDT_MAGIC) {
			memcpy(&img_param, &img, sizeof(img));
			strcpy(img_param.img_name, "dtb");
			img_param.src_img_addr = header;
			ret = fapi_ssImgAuth(img_param);
			if (ret != 0) {
				printf("%s Image validation failed\n", img.img_name);
				if( munmap(img.src_img_addr, img.src_img_len) == -1)
					printf("munmap failed\n");
				ret = 1;  /* reboot not required */
				goto finish;
			}
		}
#endif /* LINUX_UPGRADE */
		switch(x_img_header.img_hdr_type) {
			case IMG_HDR_VAR_MULTI:
#ifndef PLATFORM_LGM
				cur_par_size = sizeof(image_header_t) + 8;
				continue;
#else
				sprintf_s(name, sizeof(name),"fullimage");
				break;
#endif
			case IMG_HDR_VAR_FILESYSTEM:
				sprintf_s(name, sizeof(name),"rootfs");
				break;
			case IMG_HDR_VAR_KERNEL:
#ifndef PLATFORM_LGM
				if (strncmp((char *)x_img_header.img_hdr_name, "MIPS 4Kec Bootcore", sizeof(x_img_header.img_hdr_name)) == 0)
					sprintf_s(name, sizeof(name), "bootcore");
				else
					sprintf_s(name, sizeof(name), "kernel");
				break;
#else
				sprintf_s(name, sizeof(name),"kernel");
				break;
#endif
			case IMG_HDR_VAR_FIRMWARE:
				sprintf_s(name, sizeof(name),"firmware");
				break;
			case IMG_HDR_VAR_UBOOT:
				sprintf_s(name, sizeof(name), "uboot");
				break;
			case IMG_HDR_VAR_FLATDT:
				sprintf_s(name, sizeof(name), "dtb"); 
				break;
			default:
#ifndef PLATFORM_LGM
				cur_par_size = cur_par_size + pad + 8;
				total_file_read_size = (total_file_read_size + file_read_size + 8);
				fprintf(stderr, "Unknown image type!!\n");
				continue;
		}
#else
				fprintf(stderr, "Unknown image type!!\n");
				if(munmap(img.src_img_addr,  img.src_img_len) != 0)
					fprintf(stderr, "unmaping failied\n");
				goto finish;
		}
		strcpy(img.img_name, name);
		img.write_to_upg_part = 1;

#ifdef LINUX_UPGRADE
		memcpy(&img_param, &img, sizeof(img));

		if (ntohl(x_img_header.img_hdr_magic) == IMG_HDR_MAGIC) {
			printf("Image contains header with name [%s]\n",x_img_header.img_hdr_name);
			if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
				(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT)) {
				printf("This is not kernel or uboot image and so removing header\n");
				header += sizeof(image_header_t);
				cur_par_size -= sizeof(image_header_t);
			}
		}

		img_param.src_img_addr = header;
		img_param.src_img_len = file_read_size;

#ifndef NOAUTH
		ret = fapi_ssImgAuth(img_param);
		if (ret != 0) {
			printf("%s Image validation failed\n", img.img_name);
			if( munmap(img.src_img_addr, img.src_img_len) == -1)
				printf("munmap failed\n");
			ret = 1;  /* reboot not required */
			goto finish;
		}
#endif

		if (set_udt("udt_status", 2) < 0) {
			printf("Setting the udt status Failed\n");
			if (munmap(img.src_img_addr, img.src_img_len) == -1)
				printf("munmap failed\n");
			ret = 1;  /* reboot not required */
			goto finish;
		}
#else /* LINUX_UPGRADE */
#ifndef NOAUTH
		ret = fapi_ssImgAuth(img);
		if (ret != 0) {
			printf("Image validation failed");
			if( munmap(img.src_img_addr, img.src_img_len) == -1)
				printf("munmap failed\n");
			ret = 1;  /* reboot not required */
			goto finish;
		}
#endif

	if(set_udt(name, 1) < 0) {
		printf("Setting the udt status Failed\n");
		if( munmap(img.src_img_addr, img.src_img_len) == -1)
			printf("munmap failed\n");
		ret = 1;  /* reboot not required */
		goto finish;
	}
#endif /* !LINUX_UPGRADE */
#endif

#if !defined(PLATFORM_LGM) || defined(LINUX_UPGRADE)
#ifdef LINUX_UPGRADE
		if (set_udt("recovery_image", 1) < 0) {
			printf("Setting the recovery image status Failed\n");
			if (munmap(img.src_img_addr, img.src_img_len) == -1)
				printf("munmap failed\n");
			ret = 1;  /* reboot not required */
			goto finish;
		}

		total_file_read_size += file_read_size;

		if ((x_img_header.img_hdr_type != IMG_HDR_VAR_KERNEL) &&
			(x_img_header.img_hdr_type != IMG_HDR_VAR_UBOOT))
			header += cur_par_size;
		else
			header += file_read_size;
#else /* LINUX_UPGRADE */
		fprintf(stderr,"\nImage Header --> Data Size = %d\n Image Name = %s\n",cur_par_size, name);
		if (upgrade_sec_img((ulong)header, cur_par_size, name)) {
			fprintf(stderr,"Can not upgrade the image %s\n", name);
			ret=2; /* reboot required as partial image write performed */
			if(munmap(img.src_img_addr,  img.src_img_len) != 0)
				fprintf(stderr, "unmaping failied\n");
			goto finish;
		}

		total_file_read_size += file_read_size;

		header += file_read_size;
#endif /* !LINUX_UPGRADE */

	} while (img.src_img_len > total_file_read_size);
#endif

	if(munmap(img.src_img_addr,  img.src_img_len) != 0)
		fprintf(stderr, "unmaping failied\n");

finish:
	if(file_fd>=0)
		close(file_fd);

	remove(path);

	return ret;
}

int main(int argc, char *argv[])
{
	int opt,ret=1;
	while((opt = getopt(argc, argv, "u:rm")) != -1)
	{
		switch(opt)
		{
			case 'u':
				if((ret=do_sec_upgrade(optarg))) {
					fprintf(stderr, "Image upgrade failed \n");
					return ret;
				}
				// return value -  0:success 1:failure reboot not required 2: failure reboot requierd

				break;
			case 'r':
				// No reboot - dont activate image or reboot board automatically
				activate=false;
				break;
			case 'm':
				break;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}

	for(; optind < argc; optind++) {
		printf("extra arguments: %s\n", argv[optind]);
	}

	return ret;
}
