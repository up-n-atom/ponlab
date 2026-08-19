/******************************************************************************
**
** FILE NAME    : command.c
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

#include "common.h"
#include "cmd_upgrade.h"
#include "command.h"
#include "crc32.h"

#include <ctype.h>

env_t env;

int get_partName(unsigned long addr_first, char *name,
		 unsigned long *part_begin_addr, char *mtd_dev);

#ifdef CONFIG_BOOT_FROM_NAND
  #define TEMP_ENV_FILE "/tmp/ubootconfig"
  #ifdef CONFIG_UBOOT_CONFIG_ENV_REDUND
     char active_cfg;
  #endif
#endif

int read_env(void)
{
	int dev_fd;
	unsigned long crc = 0;

#ifdef CONFIG_BOOT_FROM_NAND
	char cmd[255];
	cmd[0] = '\0';

  #ifdef CONFIG_TARGET_UBI_MTD_SUPPORT
    #ifdef CONFIG_UBOOT_CONFIG_ENV_REDUND
	/* Dual uboot config Mode with ubootconfigA and ubootconfigB
	 * Read the both configs and get the one which have a correct CRC.
	 * If both CRC is proper, then read flags - flags contain latest
	 * revision number. The one which have latest revision is the valid
	 * uboot config to use.
	 */
	env_t envB;
	int flgA = 0, flgB = 0;

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
	if ((!flgA) && (!flgB)) {
		active_cfg = 0;
		ifx_debug_printf("The device %s could not be opened\n",
                        MTD_CONFIG_DEV_NAME);
		return 1;
	}
#if 1
	if (flgA) {
		crc ^= 0xffffffffL;
		crc = crc32(crc, env.data, ENV_SIZE);
		crc ^= 0xffffffffL;

		if (crc != env.crc) flgA = 1;
		else flgA = 2;
	}

	crc = 0;
	if (flgB) {
		crc ^= 0xffffffffL;
		crc = crc32(crc, envB.data, ENV_SIZE);
		crc ^= 0xffffffffL;

		if (crc != envB.crc) flgB = 1;
		else flgB = 2;
	}

	/* If CRC of flgA and flgB is Ok, then read env.flags which contain
	   updated revision of the config. Always take latest config. So
	   take the env which have greater env.flags value. */
	if ((flgA < 2) && (flgB < 2)) { /* Both CRC is failure */
		active_cfg = 0;
		ifx_debug_printf("For enviornment CRC32 is not OK\n");
		return 1;
	} else if (flgB > flgA) { /* If only flgB CRC is OK, then use flgB */
		active_cfg = 2;
		env = envB;
	} else if (flgA == flgB) { /* If flgA and flgB is OK, then read flags and use latest */
		if(env.flags == 255 && envB.flags == 0) { /* if we get flags as 255 and 0, then 0 is latest */
			active_cfg = 2;
			env = envB;
		} else if (envB.flags > env.flags) {
			active_cfg = 2;
			env = envB;
		} else active_cfg = 1;
	} else active_cfg = 1;
#endif
	return 0;

    #else
	/* Non dual config mode with UBI */
	sprintf_s(cmd, sizeof(cmd),"/usr/sbin/vol_mgmt ubootcfg_dump %s 0x%X", TEMP_ENV_FILE, CFG_ENV_SIZE); system(cmd);
	dev_fd = open(TEMP_ENV_FILE, O_SYNC | O_RDWR);
    #endif
  #else
	/* Non dual config mode with JFFS2 */
	sprintf_s(cmd, sizeof(cmd), "nanddump -q -f %s -o -l %u %s", TEMP_ENV_FILE,
		CFG_ENV_SIZE, MTD_CONFIG_DEV_NAME); system(cmd);
	dev_fd = open(TEMP_ENV_FILE, O_SYNC | O_RDWR);
  #endif
#else
	/* Non dual config mode with NOR/SPI flash */
	dev_fd = open(MTD_CONFIG_DEV_NAME, O_SYNC | O_RDWR);
#endif
	if (dev_fd < 0) {
		ifx_debug_printf("The device %s could not be opened\n",
				 MTD_CONFIG_DEV_NAME);
		return 1;
	}
	read(dev_fd, (void *)&env, sizeof(env));
	close(dev_fd);

	crc ^= 0xffffffffL;
	crc = crc32(crc, env.data, ENV_SIZE);
	crc ^= 0xffffffffL;


	if (crc != env.crc) {
		ifx_debug_printf("For enviornment CRC32 is not OK\n");
		return 1;
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
				ifx_debug_printf
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
	env.crc = crc32(env.crc, env.data, ENV_SIZE);
	env.crc ^= 0xffffffff;
}


int add_env(char *name, char *val)
{
        int len, oldval;
        unsigned char *envptr, *nxt = NULL;
        unsigned char *env_data = env.data;

        if (!env_data) {        /* need copy in RAM */
                ifx_debug_printf("add_env(): env_data doesn't exist\n");
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
                        ifx_debug_printf("add_env : the value of %s = %s alreay existes..\n", name, envptr);
                        return -1;
                }
        }

        /*
         * Append new definition at the end
         */
        for (envptr = env_data; *envptr || *(envptr + 1); ++envptr) ;
        if (envptr > env_data)
                ++envptr;
        if ((len = strnlen_s(name, ENV_SIZE)) <=0 ) {
		ifx_debug_printf("strnlen failed\n"); 
		return -1;
	}
	len += 2;
        /* add '=' for first arg, ' ' for all others */

	if ((len += strnlen_s(val, ENV_SIZE)) <=0) {
		ifx_debug_printf("strnlen failed\n"); 
		return -1;
	}
        len += 1;

        ifx_debug_printf("set_env : setting %s=%s for %d bytes\n", name, val, len);


        if (len > (&env_data[ENV_SIZE] - envptr)) {
                ifx_debug_printf("## Error: environment overflow, \"%s\" deleted\n", name);
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
        ifx_debug_printf("set_env(): CRC updated\n");

        return 0;
}
int set_env(char *name, char *val)
{
	int len, oldval;
	unsigned char *envptr, *nxt = NULL;
	unsigned char *env_data = env.data;

	if (!env_data) {	/* need copy in RAM */
		ifx_debug_printf("set_env(): env_data doesn't exist\n");
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

	ifx_debug_printf("set_env : the old value of %s = %s\n", name, envptr);
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

	if ((len = strnlen_s(name, ENV_SIZE)) <=0 ) {
		ifx_debug_printf("strnlen failed\n"); 
		return -1;
	}
	len += 2;
        /* add '=' for first arg, ' ' for all others */

	if ((len += strnlen_s(val, ENV_SIZE)) <=0) {
		ifx_debug_printf("strnlen failed\n"); 
		return -1;
	}
        len += 1;
	
	ifx_debug_printf("set_env : setting %s=%s for %d bytes\n", name, val,
			 len);

	if (len > (&env_data[ENV_SIZE] - envptr)) {
		ifx_debug_printf
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
	ifx_debug_printf("set_env(): CRC updated\n");

	return 0;
}

#define getenv(x)		get_env(x)

int saveenv(void)
{
#ifdef CONFIG_BOOT_FROM_NAND
	int fd, len = 0;

	char cmd[255];
	cmd[0] = '\0';

  #if !defined(CONFIG_TARGET_UBI_MTD_SUPPORT)
	sprintf_s(cmd, sizeof(cmd), "flash_erase %s", MTD_CONFIG_DEV_NAME);
	system(cmd);
  #endif
	fd = open(TEMP_ENV_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0) {
		close(fd);
		ifx_debug_printf("Saving enviornment failed for NAND\n");
		return 1;
	}

   #ifdef CONFIG_UBOOT_CONFIG_ENV_REDUND
	/* In dual uboot config mode, update latest revision in env.flags */
	env.flags++;
   #endif

	len = write(fd, (void *)&env, sizeof(env));
	if (len < (int)sizeof(env)) {
		ifx_debug_printf
		    ("Saving enviornment failed for NAND. bytes written = %d acutal size = %zd\n",
		     len, sizeof(env));
		//      return 2;
	}
	close(fd);
	cmd[0] = '\0';
  #ifdef CONFIG_TARGET_UBI_MTD_SUPPORT
    #ifdef CONFIG_UBOOT_CONFIG_ENV_REDUND
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
    #else
	sprintf_s(cmd,sizeof(cmd), "/usr/sbin/vol_mgmt ubootcfg_write %s", TEMP_ENV_FILE); system(cmd);
    #endif
  #else
	sprintf_s(cmd, sizeof(cmd), "nandwrite -q %s %s", MTD_CONFIG_DEV_NAME, TEMP_ENV_FILE); system(cmd);
  #endif
	ifx_debug_printf("Saving enviornment with CRC 0x%08lx\n", env.crc);
#else
	ifx_debug_printf("Saving enviornment with CRC 0x%08lx\n", env.crc);
	program_img(&env, sizeof(env), CFG_ENV_ADDR);
#endif
	return 0;
}

int saveenv_copy(void)
{
	unsigned long ubootconfig_copy_addr;
	char *kernel_addr;
	char ubootconfig_copy_data[sizeof(env) +
				   sizeof(UBOOTCONFIG_COPY_HEADER)];

	kernel_addr = (char *)getenv("f_kernel_addr");
	if (kernel_addr != NULL) {
		ubootconfig_copy_addr =
		    strtoul(kernel_addr, NULL,
			    16) - sizeof(ubootconfig_copy_data);
		memset(ubootconfig_copy_data, 0x00,
		       sizeof(ubootconfig_copy_data));
		sprintf_s(ubootconfig_copy_data, sizeof(ubootconfig_copy_data),"%s", UBOOTCONFIG_COPY_HEADER);
		if (memcpy_s(ubootconfig_copy_data + sizeof(UBOOTCONFIG_COPY_HEADER),sizeof(env) +
                                   sizeof(UBOOTCONFIG_COPY_HEADER), &env, sizeof(env)) != EOK) {
			return -1;
		}

		flash_sect_erase((unsigned long)ubootconfig_copy_addr,
				 ubootconfig_copy_addr +
				 sizeof(ubootconfig_copy_data) - 1);

		flash_write((unsigned long)ubootconfig_copy_data, ubootconfig_copy_addr,
			    sizeof(ubootconfig_copy_data));

	}
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
		nLen = strnlen_s(buf, ENV_SIZE);
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
