/******************************************************************************
  Copyright (C) 2018 Intel Corporation
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

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



#define IH_TYPE_INVALID         0       /* Invalid Image                */
#define IH_TYPE_STANDALONE      1       /* Standalone Program           */
#define IH_TYPE_KERNEL          2       /* OS Kernel Image              */
#define IH_TYPE_RAMDISK         3       /* RAMDisk Image                */
#define IH_TYPE_MULTI           4       /* Multi-File Image             */
#define IH_TYPE_FIRMWARE        5       /* Firmware Image               */
#define IH_TYPE_SCRIPT          6       /* Script file                  */
#define IH_TYPE_FILESYSTEM      7       /* Filesystem Image (any type)  */
#define IH_TYPE_FLATDT          8       /* Binary Flat Device Tree Blob */
#define IH_TYPE_KWBIMAGE        9       /* Kirkwood Boot Image          */
#define IH_TYPE_IMXIMAGE        10      /* Freescale IMXBoot Image      */
#define IH_TYPE_UBOOT       11  /* Lantiq uboot image*/
#define IH_TYPE_UBOOTENV    12  /* Lantiq uboot environment image*/ 
#define IH_TYPE_SYSTEMSW    13  /* Lantiq ubinized image*/

/*
 * Compression Types
 */
#define IH_COMP_NONE            0       /*  No   Compression Used       */
#define IH_COMP_GZIP            1       /* gzip  Compression Used       */
#define IH_COMP_BZIP2           2       /* bzip2 Compression Used       */
#define IH_COMP_LZMA            3       /* lzma  Compression Used       */
#define IH_COMP_LZO             4       /* lzo   Compression Used       */

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
#ifdef IMAGE_EXTRA_CHECKS
    uint8_t     ih_vendor[IH_NMLEN]; /* 32 char Vendor Name String */
    uint8_t     ih_board[IH_NMLEN]; /* 32 char Board Name String */
    uint8_t     ih_boardVer[VERSIONLEN]; /* Board Version 16 char str */
    uint8_t     ih_chip[IH_NMLEN]; /* 32 char Chip Name String */
    uint8_t     ih_chipVer[VERSIONLEN]; /* Chip Version16 char string */
    uint8_t     ih_swVer[VERSIONLEN]; /* Software Version-16 char string*/
#endif // IFX_IMAGE_EXTRA_CHECKS
}image_header_t;


