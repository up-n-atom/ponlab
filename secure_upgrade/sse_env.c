/******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.
  Copyright (C) 2019 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sse_env.c	 					       *
 *     Project    : UGW                                                        *
 ******************************************************************************/
#include <crc32.h>
#include <ctype.h>
#include "sse_env.h"

env_t env;
char active_cfg;

#define MIN(a,b)({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b);_a < _b ? _a : _b; })
#ifdef PLATFORM_LGM 
#define LGM_NAND 0
#define LGM_EMMC 1
static int lgmBoardType;

#define ENV_A_NAND_DEV "/dev/ubi1_0"
#define ENV_B_NAND_DEV "/dev/ubi1_1"
#define ENV_A_EMMC_DEV "/dev/mmcblk0p7"
#define ENV_B_EMMC_DEV "/dev/mmcblk0p8"

static int check_lgmboardtype(void) {
	FILE *fp = NULL;
	char temp[512];

	if((fp = fopen("/proc/cmdline", "r")) == NULL) {
		return(-1);
	}
	fgets(temp, 512, fp);
	if((strstr(temp,"ubi.mtd")) == NULL){
		lgmBoardType=LGM_EMMC;
	}
	else {
		lgmBoardType=LGM_NAND;
	}
	fclose(fp);
	return lgmBoardType;
}

static char* getEnvAdev(void) {
	//TODO Check for storage media type EBU NAND/EMMC 
	if(lgmBoardType == LGM_EMMC) {
		return ENV_A_EMMC_DEV;
	}
	else {
		return ENV_A_NAND_DEV;
	}
}
static char* getEnvBdev(void) {
	//TODO Check for storage media type EBU NAND/EMMC 
	if(lgmBoardType == LGM_EMMC) {
		return ENV_B_EMMC_DEV;
	}
	else{
		return ENV_B_NAND_DEV;
	}
}
#endif

unsigned long sse_crc(unsigned long crc, char *sBuf, int bread) {
	return crc32(crc, sBuf, bread);
}

int read_env(void)
{
	int dev_fd;
	unsigned long crc = 0;
#ifndef PLATFORM_LGM
	char cmd[255]={0};
#endif

	/* Dual uboot config Mode with ubootconfigA and ubootconfigB
	 * Read the both configs and get the one which have a correct CRC.
	 * If both CRC is proper, then read flags - flags contain latest
	 * revision number. The one which have latest revision is the valid
	 * uboot config to use.
	 */
	env_t envB;
	int flgA = 0, flgB = 0;

#ifdef PLATFORM_LGM
	check_lgmboardtype();
	dev_fd = open(getEnvAdev(), O_RDONLY);
	if(dev_fd < 0) {
		printf(" Opening the ENV device %s failed\n",getEnvAdev());
		return -1;
	}
	else {
		read(dev_fd, (void *)&env, sizeof(env));
		close(dev_fd);
		flgA = 1;
	}
	dev_fd = open(getEnvBdev(), O_RDONLY);
	if(dev_fd < 0) {
		printf(" Opening the ENV device %s failed\n",getEnvAdev());
		return -1;
	}
	else {
		read(dev_fd, (void *)&envB, sizeof(env));
		close(dev_fd);
		flgB = 1;
	}
#else

	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_dump %s 0x%X ubootconfigA", TEMP_ENV_FILE, CFG_ENV_SIZE); system(cmd);
	dev_fd = open(TEMP_ENV_FILE, O_SYNC | O_RDWR);
	if (dev_fd < 0) flgA = 0;
	else {
		read(dev_fd, (void *)&env, sizeof(env));
		close(dev_fd);
		flgA = 1;
	}
	sprintf_s(cmd, sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_dump %s 0x%X ubootconfigB", TEMP_ENV_FILE, CFG_ENV_SIZE); system(cmd);
	dev_fd = open(TEMP_ENV_FILE, O_SYNC | O_RDWR);
	if (dev_fd < 0) flgB = 0;
	else {
		read(dev_fd, (void *)&envB, sizeof(envB));
		close(dev_fd);
		flgB = 1;
	}
#endif
	if ((!flgA) && (!flgB)) {
		active_cfg = 0;
		printf("The device could not be opened\n");
		return 1;
	}
	if (flgA) {
		crc ^= 0xffffffffL;
		crc = sse_crc(crc, (char *)env.data, ENV_SIZE);
		crc ^= 0xffffffffL;

		if (crc != env.crc) flgA = 1;
		else flgA = 2;
		flgA = 2;
	}

	crc = 0;
	if (flgB) {
		crc ^= 0xffffffffL;
		crc = sse_crc(crc, (char *)envB.data, ENV_SIZE);
		crc ^= 0xffffffffL;

		if (crc != envB.crc) flgB = 1;
		else flgB = 2;
	}

	/* If CRC of flgA and flgB is Ok, then read env.flags which contain
	   updated revision of the config. Always take latest config. So
	   take the env which have greater env.flags value. */
	if ((flgA < 2) && (flgB < 2)) { /* Both CRC is failure */
		active_cfg = 0;
		printf("For enviornment CRC32 is not OK\n");
		return 1;
	} else if (flgB > flgA) { /* If only flgB CRC is OK, then use flgB */
		active_cfg = 2;
		env = envB;
	} else if (flgA == flgB) { /* If flgA and flgB is OK, then read flags and use latest */
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
		if(env.flags == 255 && envB.flags == 0) { /* if we get flags as 255 and 0, then 0 is latest */
			active_cfg = 2;
			env = envB;
		} else if (envB.flags > env.flags) {
			active_cfg = 2;
			env = envB;
		} else active_cfg = 1;
#else
		active_cfg = 2;
		env = envB;
#endif

	} else { 
		active_cfg = 1;
	}

	return 0;
}

int envmatch(char *s1, int i2)
{
	while (*s1 == env.data[i2++])
		if (*s1++ == '=')
			return (i2);
	if (*s1 == '\0' && env.data[i2 - 1] == '=')
		return (i2);
	return (-1);
}
unsigned char *get_env(char *name)
{
	unsigned int i, nxt;

	for (i = 0; i < ENV_SIZE && env.data[i] != '\0'; i = nxt + 1) {
		int val;

		for (nxt = i; nxt < ENV_SIZE && env.data[nxt] != '\0'; nxt++) {
			if (nxt >= ENV_SIZE) {
				printf
					("Did not get var %s with nxt = %d\n", name,
					 nxt);
				return (NULL);
			}
		}
		if ((val = envmatch(name, i)) < 0)
			continue;
		return (&env.data[val]);
	}

	return (NULL);
}

void env_crc_update(void)
{
	env.crc = 0x00000000 ^ 0xffffffff;
	env.crc = sse_crc(env.crc, (char *)env.data, ENV_SIZE);
	env.crc ^= 0xffffffff;
}


int add_env(char *name, char *val)
{
	int len, oldval;
	unsigned char *envptr, *nxt = NULL;
	unsigned char *env_data = env.data;

	if (!env_data) {        /* need copy in RAM */
		printf("add_env(): env_data doesn't exist\n");
		return 1;
	}

	/*
	 * search if variable with this name already exists
	 */
	oldval = -1;
	for (envptr = env_data; *envptr; envptr = nxt + 1) {
		for (nxt = envptr; *nxt; ++nxt) ;
		if ((oldval = envmatch(name, envptr - env_data)) >= 0)
		{
			printf("add_env : the value of %s = %s alreay existes..\n", name, envptr);
			return -1;
		}
	}

	/*
	 * Append new definition at the end
	 */
	for (envptr = env_data; *envptr || *(envptr + 1); ++envptr) ;
	if (envptr > env_data)
		++envptr;
	if ((len = strnlen_s(name, MIN(ENV_SIZE, 32UL << 10))) <=0 ) {
		printf("strnlen failed\n"); 
		return -1;
	}
	len += 2;
	/* add '=' for first arg, ' ' for all others */

	if ((len += strnlen_s(val, MIN(ENV_SIZE, 32UL << 10))) <=0) {
		printf("strnlen failed\n"); 
		return -1;
	}
	len += 1;

	printf("set_env : setting %s=%s for %d bytes\n", name, val, len);


	if (len > (&env_data[ENV_SIZE] - envptr)) {
		printf("## Error: environment overflow, \"%s\" deleted\n", name);
		return -1;
	}
	while ((*envptr = *name++) != '\0')
		envptr++;

	*envptr = '=';
	while ((*++envptr = *val++) != '\0') ;

	/* end is marked with double '\0' */
	*++envptr = '\0';


	/* end is marked with double '\0' */
	*++envptr = '\0';

	/* Update CRC */
	env_crc_update();
	printf("set_env(): CRC updated\n");

	return 0;
}
int set_env(char *name, char *val)
{
	int len, oldval;
	unsigned char *envptr, *nxt = NULL;
	unsigned char *env_data = env.data;

	if (!env_data) {	/* need copy in RAM */
		printf("set_env(): env_data doesn't exist\n");
		return 1;
	}

	/*
	 * search if variable with this name already exists
	 */
	oldval = -1;
	for (envptr = env_data; *envptr; envptr = nxt + 1) {
		for (nxt = envptr; *nxt; ++nxt) ;
		if ((oldval = envmatch(name, envptr - env_data)) >= 0)
			break;
	}

	printf("set_env : the old value of %s = %s\n", name, envptr);
	/*
	 * Delete any existing definition
	 */
	if (oldval >= 0) {
		if (*++nxt == '\0') {
			if (envptr > env_data) {
				envptr--;
			} else {
				*envptr = '\0';
			}
		} else {
			for (;;) {
				*envptr = *nxt++;
				if ((*envptr == '\0') && (*nxt == '\0'))
					break;
				++envptr;
			}
		}
		*++envptr = '\0';
	}

	/*
	 * Append new definition at the end
	 */
	for (envptr = env_data; *envptr || *(envptr + 1); ++envptr) ;
	if (envptr > env_data)
		++envptr;
	if ((len = strnlen_s(name,MIN(ENV_SIZE, 32UL << 10))) <=0 ) {
		printf("strnlen failed\n"); 
		return -1;
	}
	len += 2;
	/* add '=' for first arg, ' ' for all others */

	if ((len += strnlen_s(val, MIN(ENV_SIZE, 32UL << 10))) <=0) {
		printf("strnlen for value failed %s\n", val); 
		return -1;
	}
	len += 1;

	printf("set_env : setting %s=%s for %d bytes\n", name, val,
			len);

	if (len > (&env_data[ENV_SIZE] - envptr)) {
		printf
			("## Error: environment overflow, \"%s\" deleted\n", name);
		return 1;
	}
	while ((*envptr = *name++) != '\0')
		envptr++;

	*envptr = '=';
	while ((*++envptr = *val++) != '\0') ;

	/* end is marked with double '\0' */
	*++envptr = '\0';

	/* Update CRC */
	env_crc_update();
	printf("set_env(): CRC updated\n");

	return 0;
}

#define getenv(x)		get_env(x)
#ifdef PLATFORM_LGM
int mtdwrite(void)
{
	char cmd[2000]={0};
	cmd[0] = '\0';
	int fd,fd_temp;
	int bytes_read=0;
	int len=0,total_len=0;

	if ( active_cfg <= 2 ) {
		fd = open(getEnvAdev(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if ( fd < 0 ) {
			printf("Saving enviornment failed for NAND\n");
			return 1;
		}
		fd_temp = open(TEMP_ENV_FILE, O_RDONLY , 0644);
		if ( fd_temp < 0 ) {
			close(fd);
			printf("Saving enviornment failed for NAND\n");
			return 1;
		}
		do{
			bytes_read = read(fd_temp,cmd,sizeof(cmd));
			len = write(fd, cmd, bytes_read);
			total_len+=len;
		}while( bytes_read !=0 );

		if( total_len < (int)sizeof(cmd) )
			printf("len written %d sizeof(env) %ld\n",total_len,sizeof(env));

		close(fd_temp);
		close(fd);
	} else {
		printf("Invalid active_cfg\n");
		return -1;
	}
	printf("Saving environment with CRC 0x%08x\n", env.crc);
	return 0;
}

int nandwrite(void)
{
	char cmd[255]={0};
	cmd[0] = '\0';

	if (active_cfg < 2) {
		sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s",getEnvBdev(), TEMP_ENV_FILE);
		if (system(cmd) != 0) { /* If unable to write in ubootconfigB partition, try ubootconfigA */
			sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s",getEnvAdev(), TEMP_ENV_FILE);
			system(cmd);
		}
	} else {
		sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s",getEnvAdev(), TEMP_ENV_FILE);
		if (system(cmd) != 0) { /* If unable to write in ubootconfigA partition, try ubootconfigB */
			sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s",getEnvBdev(), TEMP_ENV_FILE);
			system(cmd);
		}
	}
	printf("Saving environment with CRC 0x%08x\n", env.crc);
	return 0;
}
#endif

int saveenv(void)
{
	int fd, len = 0;
#ifndef PLATFORM_LGM
	char cmd[255]={0};
#endif

	fd = open(TEMP_ENV_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0) {
		printf("Saving enviornment failed for NAND\n");
		return 1;
	}
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
	env.flags++;
#endif
	
	len = write(fd, (void *)&env, sizeof(env));
	if (len < (int)sizeof(env)) {
		printf
			("Saving enviornment failed for NAND. bytes written = %d acutal size = %zd\n",
			 len, sizeof(env));
		//      return 2;
	}
#ifndef PLATFORM_LGM
	printf("len written %d sizeof(env) %d\n",len,sizeof(env));
#else
	printf("len written %d sizeof(env) %ld\n",len,sizeof(env));
#endif
	close(fd);
#ifdef PLATFORM_LGM

	if(lgmBoardType == LGM_EMMC)
		mtdwrite();
	else
		nandwrite();

#else
	cmd[0] = '\0';
	if (active_cfg < 2) {
		sprintf_s(cmd, sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_write %s ubootconfigB", TEMP_ENV_FILE);
		if (system(cmd) != 0) { /* If unable to write in ubootconfigB partition, try ubootconfigA */
			sprintf_s(cmd, sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_write %s ubootconfigA", TEMP_ENV_FILE);
			system(cmd);
		}
	} else {
		sprintf_s(cmd, sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_write %s ubootconfigA", TEMP_ENV_FILE);
		if (system(cmd) != 0) { /* If unable to write in ubootconfigA partition, try ubootconfigB */
			sprintf_s(cmd, sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_write %s ubootconfigB", TEMP_ENV_FILE);
			system(cmd);
		}
	}
	printf("Saving environment with CRC 0x%08x\n", env.crc);
#endif
	return 0;
}

int get_partName(unsigned long addr_first, char *name,
		unsigned long *part_begin_addr, char *mtd_dev)
{
	int i, state = 0, cnt = 0, mtd = 0, rootfsp = 0, nLen=0;
	char cr, *buf, b_size = 'k';
	char val[10], priv_part[16];
	char *env_data = NULL;
	unsigned long offset_addr = 0;
	unsigned long flash_end = 0;

	buf = (char *)getenv("mtdparts");

	memset(val, 0x00, sizeof(val));
	memset(name, 0x00, 16);
	memset(priv_part, 0x00, sizeof(priv_part));

	env_data = (char *)getenv("part0_begin");
	if (env_data) {
		offset_addr =
			strtoul(env_data, NULL, 16);
	}
	else {
		printf("getnenv part0_begin returned NULL\n");
		return 1;
	}
	env_data = (char *)getenv("flash_end");
	if (env_data) {
		flash_end =
			strtoul(env_data, NULL, 16);
	}
	else {
		printf("getenv flash_end returned NULL\n");
		return 1;
	}
	unsigned long prev_offset = offset_addr;
	if (buf != NULL) {
		nLen = strnlen_s(buf, MIN(ENV_SIZE, 32UL << 10));
		if (nLen <= 0) {
			printf("strnlen failed\n");
			return 1;
		}
		for (i = 0; i <= nLen; i++) {
			cr = *(buf + i);
			switch (state) {
				case 0:
					if (isdigit(cr)) {
						val[cnt++] = cr;
						state = 1;
					}
					if (cr == '-') {
						state = 2;
						if (strncpy_s(val, sizeof(val), "0", sizeof(val)) != EOK){
							printf("strncpy failed\n");
							return 1;
						}
						cnt = 0;
					}
					break;
				case 1:
					if (cr == ':') {
						state = 0;
						cnt = 0;
						memset(val, 0x00, sizeof(val));
					} else if (cr == 'k' || cr == 'm' || cr == 'K'
							|| cr == 'M') {
						state = 2;
						val[cnt] = '\0';
						b_size = cr;
						cnt = 0;
					} else if (isdigit(cr)) {
						val[cnt++] = cr;
					}
					break;
				case 2:
					if (cr == '(') {
						state = 3;
						cnt = 0;
					} else
						state = 0;
					break;
				case 3:
					if (cr == ')') {
						state = 4;
						name[cnt] = '\0';
						cnt = 0;
					} else {
						name[cnt++] = cr;
					}
					break;
				case 4:
					if (cr == ',') {
						if (addr_first == offset_addr) {
							*part_begin_addr = offset_addr;
							sprintf_s(mtd_dev, 16, "/dev/mtd/%d",
									mtd);
							return 0;
						} else if (addr_first < offset_addr) {
							*part_begin_addr = prev_offset;
							if (strncpy_s(name, sizeof(priv_part), priv_part, sizeof(priv_part)) != EOK) {
								printf("strncpy failed\n");
								return 1;
							}
							sprintf_s(mtd_dev, 16, "/dev/mtd/%d",
									mtd - 1 - rootfsp);
							return 0;
						}
						prev_offset = offset_addr;
						if (strncpy_s(priv_part, sizeof(priv_part), name, sizeof(priv_part)) != EOK) {
							printf("strncpy failed\n");
							return 1;
						}
						if (b_size == 'm' || b_size == 'M')
							offset_addr +=
								(atoi(val) * 1024 * 1024);
						else
							offset_addr +=
								(atoi(val) * 1024);
						cnt = 0;
						state = 0;
						mtd++;
					} else if (cr == '\0') {
						if (addr_first < flash_end) {
							*part_begin_addr = offset_addr;
							sprintf_s(mtd_dev, 16, "/dev/mtd/%d",
									mtd);
							return 0;
						}
					}
					break;
				default:
					break;
			}
		}
	}

	return 1;
}


unsigned long find_mtd(unsigned long addr_first, char *mtd_dev)
{
	char part_name[16];
	unsigned long part_addr = 0;

	if (get_partName(addr_first, part_name, &part_addr, mtd_dev) != 0) {
		printf("Unable to find the partion name\n");
		return 0;
	}
	return part_addr;
}

int do_upgrade( int file_fd , int srcLen) {
	uint32_t curParSize=0, dir, pad;
	char name[16];
	image_header_t xImgHeader;
	char *xHeader = NULL;
	uint32_t iFileReadSize =0, iTotalFileReadSize = 0;

	char *p;
	p = mmap(0, srcLen, PROT_READ, MAP_SHARED, file_fd, 0);
	if (p == MAP_FAILED) {
		fprintf(stderr, "mmap failed\n");
	}
	xHeader = p;
	do {

		xImgHeader = *((image_header_t *)xHeader);
		printf("\nImage Header --> Data Size = %d\n Image Name = %s\n",xImgHeader.img_hdr_size , xImgHeader.img_hdr_name);

		if(xImgHeader.img_hdr_type == IMG_HDR_VAR_MULTI){
			curParSize = sizeof(image_header_t) + 8;
			iTotalFileReadSize += curParSize;
			xHeader = p + iTotalFileReadSize;
			continue;
		}

		curParSize = sizeof(image_header_t) + xImgHeader.img_hdr_size;
		pad = (16 - (curParSize % 16)) % 16;

		xHeader = p + iTotalFileReadSize;
		iFileReadSize = curParSize + pad;

		switch(xImgHeader.img_hdr_type) {
			case IMG_HDR_VAR_MULTI:
				curParSize = sizeof(image_header_t) + 8;
				continue;
			case IMG_HDR_VAR_FILESYSTEM:
				sprintf_s(name, sizeof(name), "rootfs");
				dir = 0;
				break;
			case IMG_HDR_VAR_KERNEL:
				if (strncmp((char *)xImgHeader.img_hdr_name, "MIPS 4Kec Bootcore", sizeof(xImgHeader.img_hdr_name)) == 0)
					sprintf_s(name,sizeof(name), "bootcore");
				else
					sprintf_s(name, sizeof(name), "kernel");
				dir = 1;
				break;
			case IMG_HDR_VAR_FIRMWARE:
				sprintf_s(name, sizeof(name), "firmware");
				dir = 0;
				break;
			case IMG_HDR_VAR_UBOOT:
				sprintf_s(name, sizeof(name), "uboot");
				dir = 0;
				break; 
			default:
				curParSize = curParSize + pad + 8;
				iTotalFileReadSize = (iTotalFileReadSize + iFileReadSize + 8);
				printf("Unknown image type!!\n");
				continue;
		}

		if(upgrade_img((ulong)xHeader, curParSize, name, dir, 0)) {
			printf("Can not upgrade the image %s\n", name);
		} 
		iTotalFileReadSize += iFileReadSize;
		xHeader += iFileReadSize; 

	} while(srcLen > (int)iTotalFileReadSize );

	munmap(p, srcLen);
	return 0;
}

int upgrade_img(ulong srcAddr, ulong srcLen, char *imgName, enum ExpandDir dir, int bSaveEnv)
{
	(void)bSaveEnv;
	(void)dir;

	FILE *fp;
	char strimg_dataout[100];
	image_header_t *pimg_header = NULL;

	pimg_header = (image_header_t *)srcAddr;

	if (ntohl(pimg_header->img_hdr_magic) == IMG_HDR_MAGIC) {
		printf("Image contains header with name [%s]\n",pimg_header->img_hdr_name);
		if(pimg_header->img_hdr_type != IMG_HDR_VAR_KERNEL) {
			printf("This is not kernel image and so removing header\n");
			srcAddr += sizeof(*pimg_header);
			srcLen -= sizeof(*pimg_header);
		}
	}
	sprintf_s(strimg_dataout, sizeof(strimg_dataout), "/usr/sbin/vol_mgmt upgrade_dualvol_stdin %s %zu %s", imgName, (size_t)srcLen, pimg_header->img_hdr_name);
	fp = popen (strimg_dataout, "w");
	if (fp == NULL) {
		fprintf(stderr, "upgrade %s: unable to open file for writing\n", imgName);
		return (0);
	} else {
		if(fwrite((void *)srcAddr, sizeof(char), (size_t)srcLen, fp) != srcLen)
			fprintf(stderr, "upgrade %s: unable to complete writing\n", imgName);
		pclose(fp);
	}
	return 0;
}
