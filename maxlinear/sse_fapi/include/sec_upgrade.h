/******************************************************************************

  Copyright (C) 2020-2024 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sec_upgrade.h	                                       *
 *     Project    : UGW                                                        *
 *     Description: secure fapi header file inclusions.			       *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>
#include <mtd/mtd-user.h>
#include <errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

#define IMG_HDR_VAR_INVALID         0       /* Image header invalid         */
#define IMG_HDR_VAR_STANDALONE      1       /* Header type Standalone       */
#define IMG_HDR_VAR_KERNEL          2       /* Image of Kernel for OS       */
#define IMG_HDR_VAR_RAMDISK         3       /* Image for RamDisk            */
#define IMG_HDR_VAR_MULTI           4       /* Image for MultiFile          */
#define IMG_HDR_VAR_FIRMWARE        5       /* Image for firmware           */
#define IMG_HDR_VAR_SCRIPT          6       /* Header for script file       */
#define IMG_HDR_VAR_FILESYSTEM      7       /* Image for Filesystem         */
#define IMG_HDR_VAR_FLATDT          8       /* Flat DTB Binary              */
#define IMG_HDR_VAR_KWBIMAGE        9       /* Boot image of Kirk-wood      */
#define IMG_HDR_VAR_IMXIMAGE        10      /* IMX Boot image               */
#define IMG_HDR_VAR_UBOOT           29      /* uboot name Header            */
#define IMG_HDR_VAR_UBOOTENV        30      /* uboot env image              */
#define IMG_HDR_VAR_SYSTEMSW        31      /* ubinized image of lantiq     */

/*
 * Compression Types
 */
#define IMG_HDR_COMP_NONE            0       /* Compression not used        */
#define IMG_HDR_COMP_GZIP            1       /* Compression type gzip used  */
#define IMG_HDR_COMP_BZIP2           2       /* Compression type bzip2 used */
#define IMG_HDR_COMP_LZMA            3       /* Compression type lzma used  */
#define IMG_HDR_COMP_LZO             4       /* Compression type lzo used   */
#define IMG_HDR_MAGIC        0x27051956      /* Magic number of images      */
#define IMG_HDR_NMLEN                32      /* Name length of img          */

#define FLATDT_MAGIC            0xd00dfeed      /* FDT Image Magic Number           */
#define FIT_IMAGES_PATH         "/images"
#define FIT_DATA_PROP           "data"

/*
 * Legacy format image header,
 * all data in network byte order (aka natural aka bigendian).
 */
typedef struct image_header {
        uint32_t        img_hdr_magic;       /* Image hdr Magic Num         */
        uint32_t        img_hdr_hcrc;        /* Image hdr CRC Checksum      */
        uint32_t        img_hdr_time;        /* Image hdr Time-stamp        */
        uint32_t        img_hdr_size;        /* Size of image               */
        uint32_t        img_hdr_load;        /* Image hdr data load addrs   */
        uint32_t        img_hdr_ep;          /* Image hdr Entry Point Addrs */
        uint32_t        img_hdr_dcrc;        /* Image data CRC              */
        uint8_t         img_hdr_os;          /* Img header OS               */
        uint8_t         img_hdr_arch;        /* Architecture of CPU         */
        uint8_t         img_hdr_type;        /* Type of IMG                 */
        uint8_t         img_hdr_comp;        /* Type of compression         */
        uint8_t         img_hdr_name[IMG_HDR_NMLEN];/* Header for Image Name*/
#ifdef IMAGE_EXTRA_CHECKS
    uint8_t     img_hdr_vendor[IMG_HDR_NMLEN]; /*Vendor Name    */
    uint8_t     img_hdr_board[IMG_HDR_NMLEN]; /* Board Name     */
    uint8_t     img_hdr_boardVer[VERSIONLEN]; /* Board Version  */
    uint8_t     img_hdr_chip[IMG_HDR_NMLEN]; /* Chip Name       */
    uint8_t     img_hdr_chipVer[VERSIONLEN]; /* Chip Version    */
    uint8_t     img_hdr_swVer[VERSIONLEN]; /* Software Version  */
#endif // IFX_IMAGE_EXTRA_CHECKS
} image_header_t;

enum flash_type {
	FLASH_TYPE_NAND,
	FLASH_TYPE_EMMC,
};

unsigned long sse_crc(unsigned long crc, char *sBuf, int bread);
int readenv(void);
/* UGW_SW-87803: This is a wrapper function called from fwupgrade daemon */
int read_env(void);
int envmatch(char *s1, int i2);
/* Function to add a new variable and the value it takes */
int addenv(char *name, char *val);
/* Function to get the value present in the variable name */
unsigned char *get_env(char *name);
/* Function to modify the variable name with the value passed as argument */
int set_env(char *name, char *val);
/* Function to save the changes to uboot */
int saveenv(void);
void env_crc_update(void);
