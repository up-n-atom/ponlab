#ifndef _SSE_ENV_H
#define _SSE_ENV_H
/******************************************************************************
  Copyright (C) 2019 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sse_env.h	 					       *
 *     Project    : UGW                                                        *
 ******************************************************************************/

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

#include "libsafec/safe_str_lib.h"

#define TEMP_ENV_FILE "/tmp/ubootconfig"
#define CFG_ENV_SIZE CONFIG_UBOOT_CONFIG_ENV_SIZE
#define ENV_HEADER_SIZE (sizeof(unsigned long) + sizeof(char))
#define ENV_SIZE (CFG_ENV_SIZE - ENV_HEADER_SIZE)

#define IH_TYPE_INVALID         0       /* Invalid Image                */
#define IH_TYPE_STANDALONE      1       /* Standalone Program           */
#define IH_TYPE_KERNEL          2       /* OS Kernel Image              */
#define IH_TYPE_RAMDISK         3       /* RAMDisk Image                */
#define IH_TYPE_MULTI           4       /* Multi-File Image             */
#define IH_TYPE_FIRMWARE        5       /* Firmware Image               */
#define IH_TYPE_SCRIPT          6       /* Script file                  */
#define IH_TYPE_FILESYSTEM      7       /* Filesystem Image (any type)  */
#define IH_TYPE_UBOOT       11  /* Lantiq uboot image*/


#define IH_MAGIC        0x27051956      /* Image Magic Number           */
#define IH_NMLEN                32      /* Image Name Length            */

/*
 * Legacy format image header,
 * all data in network byte order (aka natural aka bigendian).
 */
typedef struct image_header {
        uint32_t        ih_magic;       /* Image Header Magic Number    */
        uint32_t        ih_hcrc;        /* Image Header CRC Checksum    */
        uint32_t        ih_time;        /* Image Creation Timestamp     */
        uint32_t        ih_size;        /* Image Data Size              */
        uint32_t        ih_load;        /* Data  Load  Address          */
        uint32_t        ih_ep;          /* Entry Point Address          */
        uint32_t        ih_dcrc;        /* Image Data CRC Checksum      */
        uint8_t         ih_os;          /* Operating System             */
        uint8_t         ih_arch;        /* CPU architecture             */
        uint8_t         ih_type;        /* Image Type                   */
        uint8_t         ih_comp;        /* Compression Type             */
        uint8_t         ih_name[IH_NMLEN];      /* Image Name           */
#ifdef CONFIG_LTQ_IMAGE_EXTRA_CHECKS
    uint8_t     ih_vendor[IH_NMLEN]; /* 32 char Vendor Name String */
    uint8_t     ih_board[IH_NMLEN]; /* 32 char Board Name String */
    uint8_t     ih_boardVer[VERSIONLEN]; /* Board Version 16 char str */
    uint8_t     ih_chip[IH_NMLEN]; /* 32 char Chip Name String */
    uint8_t     ih_chipVer[VERSIONLEN]; /* Chip Version16 char string */
    uint8_t     ih_swVer[VERSIONLEN]; /* Software Version-16 char string*/
#endif // IFX_IMAGE_EXTRA_CHECKS
}image_header_t;




enum ExpandDir { FORWARD = 0, BACKWARD = 1 };

typedef struct environment_s {
	unsigned long crc;	/* CRC32 over data bytes        */
	unsigned char flags;      /* active/obsolete flags    */
	unsigned char data[ENV_SIZE];	/* Environment data             */
} env_t;

struct conf_header {
	unsigned long size;
	unsigned long crc;
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
int saveenv(void);
unsigned long find_mtd(unsigned long addr_first, char *mtd_dev);
void env_crc_update(void);
int get_partName(unsigned long addr_first, char *name,
		 unsigned long *part_begin_addr, char *mtd_dev);

#endif				/* _SSE_ENV_H */
