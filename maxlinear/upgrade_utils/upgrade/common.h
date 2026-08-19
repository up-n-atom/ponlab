/*******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"


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
#ifdef CONFIG_LTQ_IMAGE_EXTRA_CHECKS
    uint8_t     ih_vendor[IH_NMLEN]; /* 32 char Vendor Name String */
    uint8_t     ih_board[IH_NMLEN]; /* 32 char Board Name String */
    uint8_t     ih_boardVer[VERSIONLEN]; /* Board Version 16 char str */
    uint8_t     ih_chip[IH_NMLEN]; /* 32 char Chip Name String */
    uint8_t     ih_chipVer[VERSIONLEN]; /* Chip Version16 char string */
    uint8_t     ih_swVer[VERSIONLEN]; /* Software Version-16 char string*/
#endif // IFX_IMAGE_EXTRA_CHECKS
}image_header_t;


typedef enum {
        MTD_OOB_PLACE,
        MTD_OOB_AUTO,
        MTD_OOB_RAW,
}mtd_oob_mode_t; 

struct mtd_oob_ops {
	mtd_oob_mode_t	mode;
	size_t		len;
	size_t		retlen;
	size_t		ooblen;
	size_t		oobretlen;
	uint32_t	ooboffs;
	uint8_t		*datbuf;
	uint8_t		*oobbuf;
};


struct erase_info {
	struct mtd_info *mtd;
	uint64_t addr;
	uint64_t len;
	uint64_t fail_addr;
	u_long time;
	u_long retries;
	u_int dev;
	u_int cell;
	void (*callback) (struct erase_info *self);
	u_long priv;
	u_char state;
	struct erase_info *next;
};

struct mtd_info {
	u_char type;
	u_int32_t flags;
	uint64_t size;	 /* Total size of the MTD */

	/* "Major" erase size for the device. Naïve users may take this
	 * to be the only erase size available, or may use the more detailed
	 * information below if they desire
	 */
	u_int32_t erasesize;
	/* Minimal writable flash unit size. In case of NOR flash it is 1 (even
	 * though individual bits can be cleared), in case of NAND flash it is
	 * one NAND page (or half, or one-fourths of it), in case of ECC-ed NOR
	 * it is of ECC block size, etc. It is illegal to have writesize = 0.
	 * Any driver registering a struct mtd_info must ensure a writesize of
	 * 1 or larger.
	 */
	u_int32_t writesize;

	u_int32_t oobsize;   /* Amount of OOB data per block (e.g. 16) */
	u_int32_t oobavail;  /* Available OOB bytes per block */

	/* Kernel-only stuff starts here. */
	const char *name;
	int index;

	/* ecc layout structure pointer - read only ! */
	struct nand_ecclayout *ecclayout;

	/* Data for variable erase regions. If numeraseregions is zero,
	 * it means that the whole device has erasesize as given above.
	 */
	int numeraseregions;
	struct mtd_erase_region_info *eraseregions;

	/*
	 * Erase is an asynchronous operation.  Device drivers are supposed
	 * to call instr->callback() whenever the operation completes, even
	 * if it completes with a failure.
	 * Callers are supposed to pass a callback function and wait for it
	 * to be called before writing to the block.
	 */
	int (*erase) (struct mtd_info *mtd, struct erase_info *instr);

	/* This stuff for eXecute-In-Place */
	/* phys is optional and may be set to NULL */
	int (*point) (struct mtd_info *mtd, loff_t from, size_t len,
			size_t *retlen, void **virt, phys_addr_t *phys);

	/* We probably shouldn't allow XIP if the unpoint isn't a NULL */
	void (*unpoint) (struct mtd_info *mtd, loff_t from, size_t len);


	int (*read) (struct mtd_info *mtd, loff_t from, u64 len, u64 *retlen, u_char *buf);
	int (*write) (struct mtd_info *mtd, loff_t to, u64 len, u64 *retlen, const u_char *buf);
  int (*write_partial) (struct mtd_info *mtd, loff_t to, u64 len, u64 *retlen, const u_char *buf);
	/* In blackbox flight recorder like scenarios we want to make successful
	   writes in interrupt context. panic_write() is only intended to be
	   called when its known the kernel is about to panic and we need the
	   write to succeed. Since the kernel is not going to be running for much
	   longer, this function can break locks and delay to ensure the write
	   succeeds (but not sleep). */

	int (*panic_write) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf);

	int (*read_oob) (struct mtd_info *mtd, loff_t from,
			 struct mtd_oob_ops *ops);
	int (*write_oob) (struct mtd_info *mtd, loff_t to,
			 struct mtd_oob_ops *ops);

	/*
	 * Methods to access the protection register area, present in some
	 * flash devices. The user data is one time programmable but the
	 * factory data is read only.
	 */
	int (*get_fact_prot_info) (struct mtd_info *mtd, struct otp_info *buf, size_t len);
	int (*read_fact_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);
	int (*get_user_prot_info) (struct mtd_info *mtd, struct otp_info *buf, size_t len);
	int (*read_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);
	int (*write_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);
	int (*lock_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len);

/* XXX U-BOOT XXX */
#if 0
	/* kvec-based read/write methods.
	   NB: The 'count' parameter is the number of _vectors_, each of
	   which contains an (ofs, len) tuple.
	*/
	int (*writev) (struct mtd_info *mtd, const struct kvec *vecs, unsigned long count, loff_t to, size_t *retlen);
#endif

	/* Sync */
	void (*sync) (struct mtd_info *mtd);

	/* Chip-supported device locking */
	int (*lock) (struct mtd_info *mtd, loff_t ofs, uint64_t len);
	int (*unlock) (struct mtd_info *mtd, loff_t ofs, uint64_t len);

	/* Power Management functions */
	int (*suspend) (struct mtd_info *mtd);
	void (*resume) (struct mtd_info *mtd);

	/* Bad block management functions */
	int (*block_isbad) (struct mtd_info *mtd, loff_t ofs);
	int (*block_markbad) (struct mtd_info *mtd, loff_t ofs);

/* XXX U-BOOT XXX */
#if 0
	struct notifier_block reboot_notifier;  /* default mode before reboot */
#endif

	/* ECC status information */
	struct mtd_ecc_stats ecc_stats;
	/* Subpage shift (NAND) */
	int subpage_sft;

	void *priv;

	struct module *owner;
	int usecount;

	/* If the driver is something smart, like UBI, it may need to maintain
	 * its own reference counting. The below functions are only for driver.
	 * The driver may register its callbacks. These callbacks are not
	 * supposed to be called by MTD users */
	int (*get_device) (struct mtd_info *mtd);
	void (*put_device) (struct mtd_info *mtd);
};

typedef struct mtd_info nand_info_t;

#define uswap_32(x) \
        ((((x) & 0xff000000) >> 24) | \
         (((x) & 0x00ff0000) >>  8) | \
         (((x) & 0x0000ff00) <<  8) | \
         (((x) & 0x000000ff) << 24))


#define be32_to_cpu(x)                uswap_32(x)
#define cpu_to_be32(x)                uswap_32(x)

#define uimage_to_cpu(x)                be32_to_cpu(x)
#define cpu_to_uimage(x)                cpu_to_be32(x)

#define image_get_hdr_l(f) \
        static inline uint32_t image_get_##f(const image_header_t *hdr) \
        { \
                return uimage_to_cpu (hdr->ih_##f); \
        }
image_get_hdr_l (magic);        /* image_get_magic */
image_get_hdr_l (hcrc);         /* image_get_hcrc */
image_get_hdr_l (time);         /* image_get_time */
image_get_hdr_l (size);         /* image_get_size */
image_get_hdr_l (load);         /* image_get_load */
image_get_hdr_l (ep);           /* image_get_ep */
image_get_hdr_l (dcrc);         /* image_get_dcrc */

#define image_set_hdr_l(f) \
        static inline void image_set_##f(image_header_t *hdr, uint32_t val) \
        { \
                hdr->ih_##f = cpu_to_uimage (val); \
        }
image_set_hdr_l (magic);        /* image_set_magic */
image_set_hdr_l (hcrc);         /* image_set_hcrc */
image_set_hdr_l (time);         /* image_set_time */
image_set_hdr_l (size);         /* image_set_size */
image_set_hdr_l (load);         /* image_set_load */
image_set_hdr_l (ep);           /* image_set_ep */
image_set_hdr_l (dcrc);         /* image_set_dcrc */


static inline uint32_t image_get_header_size (void)
{
        return (sizeof (image_header_t));
}

static inline uint32_t image_get_data_size (const image_header_t *hdr) 
{ 
        return image_get_size (hdr); 
}

static inline ulong image_get_data (const image_header_t *hdr)
{
        return ((ulong)hdr + image_get_header_size ());
}

enum ExpandDir { FORWARD = 0, BACKWARD = 1 };

int nand_flash_write(ulong srcAddr, ulong destAddr, int srcLen);
void flash_sect_protect(int mode, unsigned long addr_first,unsigned long addr_last);
int flash_write(unsigned long srcAddr, unsigned long destAddr, int srcLen);
int flash_sect_erase(unsigned long addr_first, unsigned long addr_last);
int FindNPImgLoc(ulong img_addr,ulong *nextStartAddr,ulong *preEndAddr);
int program_img(ulong srcAddr,int srcLen,ulong destAddr);
int upgrade_img(ulong srcAddr, ulong srcLen, char *imgName, enum ExpandDir Dir, int bSaveEnv);
int FindPartBoundary(ulong img_addr,ulong *curpart_begin,ulong *nextpart_begin);
void env_crc_update(void);
void mac_usage(void);
int ValidateMACAddress(unsigned char *mac);
int init_sem(key_t key);
int enter_critical_section(int semid);
int exit_critical_section(int semid);
int del_sem(int semid);
void sig_handler(int signo);
#if !defined(BUILD_FROM_IFX_UTILITIES)
int do_upgrade( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
#else
int do_upgrade( int file_fd , int srcLen);
#endif 

int image_check_dcrc (const image_header_t *hdr);
int image_check_hcrc (const image_header_t *hdr);

#define IFX_DEBUG

#ifdef IFX_DEBUG
#define	ifx_debug_printf(format, args...) printf(format, ##args)
#else
#define	ifx_debug_printf(format, args...)
#endif
