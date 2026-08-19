/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_firmware.c
   This file contains the implementation of the GRX 500 common download and
   the CRC Functions.
*/

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_spi.h"
#include "drv_sdd_mbx_registers.h"
#include "drv_sdd_mbx_trace.h"
#include "drv_sdd_mbx_interrupt.h"
#include "drv_sdd_mbx_firmware.h"
#include "ifxos_copy_user_space.h"
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/ltq_system_reset.h>

/* ========================================================================== */
/*                            Macro definitions                               */
/* ========================================================================== */
/* Size of the command in mailbox (Command Box In) in bytes */
#define SDD_MBX_CBI_SIZE                      64

/* VCODEC FW footer struct values */
#define VCODEC_FIRMWARE_MAGIC_VALUE 0x000C0DEC

/* ========================================================================== */
/*                             Global variables                               */
/* ========================================================================== */
extern SDD_MBX_CONTEXT_t sdd_mbx_context;

/* ========================================================================== */
/*                             Type definitions                               */
/* ========================================================================== */
/* VCODEC firmware image footer format */
struct vcodec_fw_footer
{
   /** firmware version */
   IFX_uint32_t fw_version;
   /** timestamp */
   IFX_uint32_t timestamp;
   /** magic word 0x000C0DEC */
   IFX_uint32_t magic;
   /** binary image size in words */
   IFX_uint32_t size;
   /** CRC-32 checksum */
   IFX_uint32_t crc32;
};

/* ========================================================================== */
/*                         Function implementation                            */
/* ========================================================================== */
/**
   Swaps every two 16-bit-words in the buffer

   This swap is needed because of the data format in the FW download and has
   nothing to do with endianess.

   \param  buffer        Pointer to a buffer with the 16-bit-words.
   \param  length        Number of words in the buffer that must be swapped
                         (must be a multiple of 2).
*/
static IFX_void_t sdd_mbx_word_swap (IFX_uint16_t* buffer, IFX_uint32_t length)
{
   IFX_uint32_t i;
   IFX_uint16_t tmp;

   /* swap words in each pair of two words, e.g. word 2-1, 4-3, etc. */
   for ( i = 0; i < length; i+=2 )
   {
      tmp = buffer[i];
      buffer[i] = buffer[i+1];
      buffer[i+1] = tmp;
   }
}

/**
   Download PRAM binary.

   \param pDev       pointer to the device interface

   \param pBuffer    Pointer to Buffer for download

   \param nSize      size of the buffer in bytes

   \return
   - SDD_FW_ERR_NO_ERROR
   - SDD_MBX_statusParam
   - SDD_MBX_statusCmdMbWrErr
   - SDD_MBX_statusCmdIbNotAvail

   \remarks
   -  This function receives a byte buffer in a defined format
      bytebuffer and size
*/
static IFX_int32_t sdd_mbx_binary_download(SDD_MBX_DEVICE_t *pDev,
                                   IFX_uint8_t *buffer,
                                   IFX_ulong_t length)
{
   IFX_int32_t    err = SDD_FW_ERR_NO_ERROR;
   IFX_ulong_t    pos = 0, count = length;
   IFX_uint8_t    len;
   IFX_uint16_t   pCmd[SDD_MBX_CBI_SIZE/2] = {0};
   IFX_uint8_t    pBuf[SDD_MBX_CBI_SIZE];
   IFX_uint32_t   fragment_limit;

   if ((buffer == IFX_NULL) || (length == 0))
   {
      ERROR("at least one parameter is wrong");
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   /* Find the maximum size a fragment may have to pass through SPI without
      incurring addtional fragmentation there. */
   fragment_limit = SDD_MBX_spi_blocksize_get();

   /* write all data */
   while ((count > 0) && (SDD_FW_ERR_NO_ERROR == err))
   {
      /* calculate length of fragment: note that count is in bytes */
      len = (count > SDD_MBX_CBI_SIZE) ? SDD_MBX_CBI_SIZE : count;
      len = (len > fragment_limit) ? fragment_limit : len;
      /* ensure that the fragment has a multiple of 4 bytes */
      len = len - (len % 4);

      if (copy_from_user(pBuf, &buffer[pos], len) != 0)
      {
         /* error: Data copy from user space failed. */
         ERROR("drv failed to copy data from userspace");
         return -EFAULT;
      }
      sdd_mbx_cpb2w (&pCmd[0], pBuf, len);

      /* The ROM boot expects first the low 16-bit then the high 16-bit of
         each 32-bit word while the FW binary contains the data in 32-bit
         big endian format. */
      sdd_mbx_word_swap(&pCmd[0], len >> 1);

      /* write Data, len in bytes */
      err = sdd_mbx_spi_write(pDev, SDD_MBX_HOST_CB_DATA, pCmd, len);
      if (err != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error write to CB data: %d", err);
         err = SDD_FW_ERR_MBX_READ;
         break;
      }

      /* increment position */
      pos += len;
      /* decrement count */
      count -= len;
   }

#if 0
   {
      IFX_uint32_t j;

      for (j=0; j<5; j++)
      {
         IFX_uint8_t    i;
         IFX_uint16_t   nReg[16];

         printk(KERN_ERR "Final register dump\n");
         err = sdd_mbx_spi_read (pDev, 0x20, nReg, 2*16);
         if (err != SDD_FW_ERR_NO_ERROR)
         {
            TRACE("error on host register access");
            return SDD_FW_ERR_MBX_READ;
         }
         for (i=0; i<16; i++)
         {
            printk("REG%i %04X, ", i, nReg[i]);
         }
         printk(KERN_ERR "\n");
      }
   }
#endif

   return err;
}


/**
   Write info for bootloader, how to boot.

   \param  pDev         Pointer to the device structure.
   \param  boot_source  Enum describing the boot mode.

   \return
   -SDD_MBX_statusOk
   -SDD_MBX_statusSpiAccErr
*/
IFX_int32_t sdd_mbx_set_boot_config(SDD_MBX_DEVICE_t *pDev, SDD_MBX_BOOT_SOURCE_t boot_source)
{
   IFX_int32_t    err = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t   host_reg = 0;

   if(boot_source == SDD_MBX_BOOT_ROM)
   {
      host_reg = SDD_MBX_HOST_REGB_BOOT_ROM;
   }
   else if(boot_source == SDD_MBX_BOOT_SPI)
   {
      host_reg = SDD_MBX_HOST_REGB_BOOT_SPI;
   }
   else
   {
      return SDD_FW_ERR_NO_ERROR;
   }

   /* write the boot info to boot from SPI */
   err = sdd_mbx_reg_write_word(pDev, SDD_MBX_HOST_REGB, &host_reg);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("set boot config error: %d", err);
      err = SDD_FW_ERR_SPI_ACCESS;
   }

   return err;
}

/**
   Initiate a controller reset.

   \param device          Pointer to the device structure.
   \param bootInfo        boot type

   \return IFX_int32_t SDD_MBX_statusOk or SDD_MBX_statusSpiAccErr or
           SDD_FW_ERR_SET_BOOT_CFG
*/
IFX_int32_t sdd_mbx_reset_controller (SDD_MBX_DEVICE_t* pDev, SDD_MBX_BOOT_SOURCE_t bootInfo)
{
   IFX_int32_t    err = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t   reg_value = 0x0;
   IFX_uint16_t   loop = 0;

   /* Diable IRQ system */
   sdd_mbx_irq_host_disable(pDev, SDD_MBX_HOST_EB_DATA);
   sdd_mbx_irq_host_disable(pDev, SDD_MBX_HOST_CB_DATA);

   /* Disable IRQ line while FW is not downloaded */
   sdd_mbx_irq_system_disable(pDev);

   /* Reset flag until alive event is reported via system event. */
   pDev->nDevState &= ~DS_DEV_UP;

   /* No more SPI access. */
   pDev->nDevState &= ~DS_SPI_ACTIVE;

   sysrst_rst(SYSRST_DOMAIN_VCODEC, SYSRST_MODULE_TAPI, 0);

#if 0 /* Soft reset was replaced by hard reset. */
   /* set value for HOST_CFG register to reset controller */
   reg_value = (1 << SDD_MBX_HOST_CFG_RSTH) | (1 << SDD_MBX_HOST_CFG_RST);

   /* initiate a controller reset */
   err = sdd_mbx_spi_write(pDev, SDD_MBX_HOST_CFG, &reg_value, 2);
   if(err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("SPI write error: %d", err);
      return SDD_FW_ERR_SPI_ACCESS;
   }
#endif

   /*
      now poll register HOST_REG0 until value 0x10 can be read out, which
      indicates a successful boot of the embedded controller
   */
   if(bootInfo == SDD_MBX_BOOT_ROM)
   {
      do
      {
         /* read the boot state indication */
         err = sdd_mbx_reg_read_word(pDev, SDD_MBX_HOST_CMD, &reg_value);
         if(err != SDD_FW_ERR_NO_ERROR)
         {
            ERROR("SPI read error: %d", err);
            return SDD_FW_ERR_SPI_ACCESS;
         }

         /* wait 2ms between register accesses */
         set_current_state(TASK_INTERRUPTIBLE);
         schedule_timeout(HZ/500);
         loop++;
      }
      while((reg_value < SDD_MBX_HOST_REG2_BOOTSTATE) && (loop <= 500));

      if(loop <= 500)
      {
         /* Set bootmode again to ROM in case a recovery is needed. */
         err = sdd_mbx_set_boot_config(pDev, SDD_MBX_BOOT_ROM);
         if (SDD_FW_ERR_NO_ERROR != err)
         {
            ERROR("set boot config error: %d", err);
            return SDD_FW_ERR_SET_BOOT_CFG;
         }
      }
      else
      {
         TRACE("loop counter error");
         return SDD_FW_ERR_SPI_ACCESS;
      }
   }

   return SDD_FW_ERR_NO_ERROR;
}

/**
   Download PRAM firmware

   This function executes the whole flow required for fimware download
   and returns PRAM checksum after a sucessfull downlod.

   \param  pDev         Pointer to the device interface.
   \param  pFwImage     Pointer to address with FW image.
   \param  nFwSize      Size of the firmware image in bytes.

   \return
   - SDD_FW_ERR_NO_ERROR
   - SDD_MBX_statusParam
   - SDD_MBX_statusNoFwDwld
   - SDD_MBX_statusSetBootCfgErr
   - SDD_MBX_statusCtrlResErr
   - SDD_MBX_statusDwldBinErr
   - SDD_MBX_statusSpiAccErr
   - SDD_MBX_statusFwDwldTimeout
*/
IFX_int32_t sdd_mbx_firmware (
                        SDD_MBX_DEVICE_t* pDev,
                        IFX_uint8_t *pFwImage,
                        IFX_uint32_t nFwSize)
{
   IFX_int32_t    err            = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t   nRegHostCmd    = 0x0;
   IFX_uint8_t    loop           = 0;
   IFX_uint32_t   retry          = 0;

   struct vcodec_fw_footer fw_footer;

   TRACE("firmware download start");

   memset(&fw_footer, 0, sizeof(fw_footer));

   /* check if the pointer is valid */
   if ((IFX_NULL == pFwImage) || (nFwSize == 0))
   {
      /* errmsg: At least one parameter is wrong. */
      ERROR("invaild parameter");
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   /* copy FW footer from user space */
   if (IFXOS_CpyFromUser(&fw_footer, pFwImage + nFwSize - sizeof(fw_footer),
                         sizeof(fw_footer)) == IFX_NULL)
   {
      ERROR("VCODEC FW copy footer error");
      /* errmsg: Firmware download failed. */
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   if(fw_footer.magic != VCODEC_FIRMWARE_MAGIC_VALUE)
   {
      ERROR("VCODEC FW magic value error: 0x%08X != 0x%08X",
             fw_footer.magic, VCODEC_FIRMWARE_MAGIC_VALUE);
      /* errmsg: Firmware download failed. */
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   if (fw_footer.size * 4 + sizeof(fw_footer) != nFwSize)
   {
      ERROR("VCODEC FW size missmatch: 0x%08X != 0x%08X",
             fw_footer.size * 4, nFwSize - sizeof(fw_footer));
      /* errmsg: Firmware download failed. */
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   TRACE ("VCODEC FW info - version %08X, magic: 0x%08X, "
          "timestamp: 0x%08X, mem: 0x%08X (%d), CRC32: %08X.\n",
          fw_footer.fw_version, fw_footer.magic,
          fw_footer.timestamp, fw_footer.size * 4, fw_footer.size * 4,
          fw_footer.crc32);

   /*
      write the controller startup configuration to the Boot Configuration
      Register (HOST_REG0) to boot from SPI command inbox
   */
   err = sdd_mbx_set_boot_config(pDev, SDD_MBX_BOOT_SPI);
   if(err != SDD_FW_ERR_NO_ERROR)
   {
      /* errmsg: Setting of boot configuration register failed. */
      ERROR("sdd_mbx_set_boot_config err: %d", err);
      return SDD_FW_ERR_SET_BOOT_CFG;
   }

   for(retry = 0; retry < SDD_MBX_FW_DOWNLOAD_RETRY_MAX; retry++)
   {
      loop = 0;

      /* Reset controller to start the boot process from SPI */
      err = sdd_mbx_reset_controller(pDev, SDD_MBX_BOOT_SPI);
      if(err != SDD_FW_ERR_NO_ERROR)
      {
         /* errmsg: Controller reset failed. */
         ERROR("sdd_mbx_reset_controller err: %d", err);
         return SDD_FW_ERR_CTRL_RES;
      }

      /* now download firmware */
      TRACE("image size: %d", nFwSize);

      /* The footer needs to be stripped from the image to load. */
      err = sdd_mbx_binary_download(pDev, pFwImage, nFwSize - sizeof(fw_footer));
      if(err != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("sdd_mbx_binary_download err: %d", err);
         return SDD_FW_ERR_DOWNLOAD_BINARY;
      }

      /* check success of download by reading boot info register */
      do
      {
         /* read the boot state indication from register HOST_REG2 */
         err = sdd_mbx_reg_read_word(pDev, SDD_MBX_HOST_REG2, &nRegHostCmd);
         if (err == SDD_FW_ERR_SPI_ACCESS)
         {
            ERROR("read boot state error: %d", err);
            return SDD_FW_ERR_SPI_ACCESS;
         }
         loop++;

         /*lint -save -e(62) 'udelay' incompatible types for operator ":" */
         /* wait 5ms between register accesses */
         set_current_state(TASK_INTERRUPTIBLE);
         schedule_timeout(HZ/200);
         /* lint -restore */
      } while((nRegHostCmd < SDD_MBX_HOST_REG2_BOOTSTATE) && (loop <= 10));

      if (loop > 10)
      {
         ERROR("Timeout while waiting for FW boot ready indication4. "
               "Host2: 0x%X, retry: %u", nRegHostCmd, retry);

         /* Delay 100ms between retries */
         set_current_state(TASK_INTERRUPTIBLE);
         schedule_timeout(HZ/10);
      }
      else
      {
         /* VCODEC status register is correct */
         pDev->nDevState |= DS_FW_DLD;
         break;
      }
   }

   /* Set bootmode back to ROM in case a recovery is needed. */
   err = sdd_mbx_set_boot_config(pDev, SDD_MBX_BOOT_ROM);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      /* errmsg: Setting of boot configuration register failed. */
      ERROR("sdd_mbx_set_boot_config err: %d", err);
      return SDD_FW_ERR_SET_BOOT_CFG;
   }

   if(!(pDev->nDevState & DS_FW_DLD))
   {
      /* Firmware download failed */
      return SDD_FW_ERR_FW_DWLD_TIMEOUT;
   }

   TRACE("firmware download end");

   if(sdd_mbx_context.trace_level == TRACE_DEBUG)
   {
      err = sdd_mbx_reg_read_word(pDev, SDD_MBX_HOST_CFG, &nRegHostCmd);
      if (err != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("config read err: %d", err);
         return SDD_FW_ERR_SPI_ACCESS;
      }
      TRACE("config register: %x", nRegHostCmd);
   }

   /* Enable VCODEC IRQ. */
   err = sdd_mbx_irq_vcodec_enable(pDev);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("sdd_mbx_irq_vcodec_enable err: %d", err);
      return SDD_FW_ERR_SPI_ACCESS;
   }

   TRACE("firmware ready");

   return err;
}

