/******************************************************************************
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
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <string.h>

#include <fapi_sec_service.h>


static int upgrade_sec_img(ulong src_addr, ulong src_len, char *img_name)
{
	FILE *fp;
	char strimg_dataout[100];
	image_header_t *pimg_header = NULL;

	pimg_header = (image_header_t *)src_addr;

	if (ntohl(pimg_header->ih_magic) == IH_MAGIC) {
		printf("Image contains header with name [%s]\n",pimg_header->ih_name);
		if(pimg_header->ih_type != IH_TYPE_KERNEL) {
			printf("This is not kernel image and so removing header\n");
			src_addr += sizeof(*pimg_header);
			src_len -= sizeof(*pimg_header);
		}
	}
	sprintf_s(strimg_dataout, sizeof(strimg_dataout), "/usr/sbin/vol_mgmt upgrade_dualvol_stdin %s %u %s", img_name, (size_t)src_len, pimg_header->ih_name);
	fp = popen (strimg_dataout, "w");
	if (fp == NULL) {
		fprintf(stderr, "upgrade %s: unable to open file for writing\n", img_name);
		return 1;
	} else {
		if(fwrite((void *)src_addr, sizeof(char), (size_t)src_len, fp) != src_len){
			fprintf(stderr, "upgrade %s: unable to complete writing\n", img_name);
			pclose(fp);
			return 1;
		}
		pclose(fp);
	}
	return 0;
}


static int do_sec_upgrade(const char *path){

	uint32_t cur_par_size=0, pad;
	char name[16];
	int ret = 0;
	image_header_t x_img_header;
	unsigned char *header = NULL;
	uint32_t file_read_size =0, total_file_read_size = 0;
	int file_fd = 0;
	struct stat filestat={0};

	img_param_t img;

	file_fd = open(path, O_RDONLY);
	if (file_fd < 0) {  
		fprintf(stderr, "The file %s could not be opened\n", path);
		return 1;
	}               


	if(fstat(file_fd, &filestat)){
		printf("fstat error: [%s]\n",strerror(errno)); 
		close(file_fd); 
		return 1;
	}

	img.src_img_fd=file_fd;
	img.src_img_len=filestat.st_size;
	if(!img.src_img_len){
		printf("EMPTY file ... \n");
		ret = 1;
		goto finish;
	}else{
		img.src_img_addr = mmap(0, img.src_img_len, PROT_READ, MAP_SHARED, img.src_img_fd, 0);
		if(img.src_img_addr == MAP_FAILED){
			printf("MMAP failed... %s\n",strerror(errno));
			ret = 1;
			goto finish;
		}
	}

	ret = fapi_ssImgAuth(img);
	if(ret != 0){
		printf("Image validation failed");
		if( munmap(img.src_img_addr, img.src_img_len) == -1)
			printf("munmap failed\n");
		ret = 1;  /* reboot not required */
		goto finish;
	}

	/* make sure secure header removed before writing to flash */
	header =  img.src_img_addr;

	do {
		x_img_header = *((image_header_t *)header);

		if(x_img_header.ih_type == IH_TYPE_MULTI){
			cur_par_size = sizeof(image_header_t) + 8;
			total_file_read_size += cur_par_size;
			header =  img.src_img_addr + total_file_read_size;
			continue;
		}

		cur_par_size = sizeof(image_header_t) + x_img_header.ih_size;
		pad = (16 - (cur_par_size % 16)) % 16;
		header =  img.src_img_addr + total_file_read_size;
		file_read_size = cur_par_size + pad;

		switch(x_img_header.ih_type) {
			case IH_TYPE_MULTI:
				cur_par_size = sizeof(image_header_t) + 8;
				continue;
			case IH_TYPE_FILESYSTEM:
				sprintf_s(name, sizeof(name),"rootfs");
				break;
			case IH_TYPE_KERNEL:
				if (strncmp((char *)x_img_header.ih_name, "MIPS 4Kec Bootcore", sizeof(x_img_header.ih_name)) == 0)
					sprintf_s(name, sizeof(name), "bootcore");
				else
					sprintf_s(name, sizeof(name), "kernel");
				break;
			case IH_TYPE_FIRMWARE:
				sprintf_s(name, sizeof(name),"firmware");
				break;
			case IH_TYPE_UBOOT:
				sprintf_s(name, sizeof(name), "uboot");
				break; 
			default:
				cur_par_size = cur_par_size + pad + 8;
				total_file_read_size = (total_file_read_size + file_read_size + 8);
				fprintf(stderr, "Unknown image type!!\n");
				continue;
		}

		fprintf(stderr,"\nImage Header --> Data Size = %d\n Image Name = %s\n",cur_par_size, name);
		if(upgrade_sec_img((ulong)header, cur_par_size, name)){
			fprintf(stderr,"Can not upgrade the image %s\n", name);
			ret=2; /* reboot required as partial image write performed */
			if(munmap(img.src_img_addr,  img.src_img_len) != 0)
				fprintf(stderr, "unmaping failied\n");
			goto finish;
		}

		total_file_read_size += file_read_size;

		header += file_read_size;

	} while(img.src_img_len > total_file_read_size);

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
	while((opt = getopt(argc, argv, ":u:rm")) != -1)
	{
		switch(opt)
		{
			case 'u':
				if((ret=do_sec_upgrade(optarg))){
					fprintf(stderr, "Image upgrade failed \n");
					return ret;
				}
				// return value -  0:success 1:failure reboot not required 2: failure reboot requierd

				break;
			case 'r':
			case 'm':
				break;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}

	for(; optind < argc; optind++){
		printf("extra arguments: %s\n", argv[optind]);
	}

	return ret;
}
