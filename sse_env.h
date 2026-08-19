/******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sse_env.h	 					       *
 *     Project    : UGW                                                        *
 ******************************************************************************/

#ifndef _SSE_ENV_H
#define _SSE_ENV_H

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fapi_sec_service.h>
#include "safe_str_lib.h"
#include <sec_upgrade.h>

#define TEMP_ENV_FILE "/tmp/ubootconfig"
#define CFG_ENV_SIZE CONFIG_UBOOT_CONFIG_ENV_SIZE
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define ENV_HEADER_SIZE (sizeof(uint32_t) + sizeof(char))
#else
#define ENV_HEADER_SIZE sizeof(uint32_t) 
#endif
#define ENV_SIZE (CFG_ENV_SIZE - ENV_HEADER_SIZE)


enum ExpandDir {
    FORWARD = 0,
    BACKWARD = 1,
};

typedef struct environment_s {
	uint32_t  crc;	/* CRC32 over data bytes        */
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
	unsigned char flags;      /* active/obsolete flags    */
#endif
	unsigned char data[ENV_SIZE];	/* Environment data             */
} env_t;

struct conf_header {
	uint32_t size;
	uint32_t crc;
};

void mac_usage(void);
int ValidateMACAddress(unsigned char *mac);
int init_sem(key_t key);
int enter_critical_section(int semid);
int exit_critical_section(int semid);
int del_sem(int semid);
void sig_handler(int signo);
int do_upgrade( int file_fd , int srcLen);
int upgrade_img(ulong srcAddr, ulong srcLen, char *imgName, enum ExpandDir dir, int bSaveEnv);
unsigned long sse_crc(unsigned long crc, char *sBuf, int bread);
int read_env(void);
int envmatch(char *s1, int i2);
int add_env(char *name, char *val);
unsigned char *get_env(char *name);
int set_env(char *name, char *val);
#ifdef PLATFORM_LGM
int mtdwrite(void);
int nandwrite(void);
#endif
int saveenv(void);
unsigned long find_mtd(unsigned long addr_first, char *mtd_dev);
void env_crc_update(void);
int get_partName(unsigned long addr_first, char *name,
		 unsigned long *part_begin_addr, char *mtd_dev);

#endif				/* _SSE_ENV_H */
