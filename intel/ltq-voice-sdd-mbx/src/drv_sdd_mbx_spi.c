/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_spi.c
   Implementation of SPI access functions.

   This platform device driver needs a an entry like this in the SPI master
   node of the device tree:

   vcodec@5 {
           compatible = "lantiq,grx500-vcodec";
           spi-max-frequency = <8000000>;
           reg = <5>;
           interrupts = <217>;
           interrupt-parent = <&gic>;

           slic200@0 {
                   compatible = "lantiq,slic200";
                   dcdc_type = "IBB";
           };
   };
*/

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_trace.h"
#include "drv_sdd_mbx_spi.h"
#include "drv_sdd_mbx_registers.h"
#include "drv_sdd_mbx_interrupt.h"
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/string.h>

/* ========================================================================== */
/*                             Macro definitions                              */
/* ========================================================================== */
/* First value to use to check if the SPI access is working */
#define SDD_MBX_SPI_TEST_INIT_VAL         0x01
/* Set rounds of the detection test should repeat the register read/write */
#define SDD_MBX_MAX_SPI_TEST_ROUNDS       0x10
/* SPI header length in bytes */
#define SDD_MBX_SPI_HDR_LENGTH            2
/* SPI header manipulation flags and masks */
#define SDD_MBX_SPI_HDR_B0_R              0x80
#define SDD_MBX_SPI_HDR_B0_W              0x40
#define SDD_MBX_SPI_HDR_B0_RESERVED       0x3E
#define SDD_MBX_SPI_HDR_B1_I              0x01

/* ========================================================================== */
/*                             Type definitions                               */
/* ========================================================================== */
struct SDD_MBX_SPI_DEV
{
   struct spi_device* spi;
   struct list_head device_entry;
} __PACKED__;

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
static int sdd_mbx_spi_probe(
                        struct spi_device* pdev);

static int sdd_mbx_spi_remove(
                        struct spi_device* pdev);

static IFX_int32_t sdd_mbx_access_test (
                        SDD_MBX_DEVICE_t *pDev);

static IFX_int32_t sdd_mbx_spi_setup(
                        SDD_MBX_DEVICE_t *pDev);

static void sdd_mbx_cpw2b(
                        IFX_uint8_t *pBbuf,
                        const IFX_uint16_t * const pWbuf,
                        const IFX_uint32_t nWoffset,
                        const IFX_uint32_t nB);


/* ========================================================================== */
/*                             Global variables                               */
/* ========================================================================== */
static const struct of_device_id sdd_mbx_match_table[] = {
        { .compatible = "lantiq,grx500-vcodec" },
        { .compatible = "lantiq,sslic" },
        {},
};
MODULE_DEVICE_TABLE(of, sdd_mbx_match_table);

static struct spi_driver sdd_mbx_spi_driver =
{
   .driver =
   {
      .name    = "vcodec",
      .owner   = THIS_MODULE,
      .bus     = &spi_bus_type,
      .of_match_table = sdd_mbx_match_table
   },
   .probe      = sdd_mbx_spi_probe,
   .remove     = sdd_mbx_spi_remove,
};

extern SDD_MBX_CONTEXT_t sdd_mbx_context;

extern IFX_char_t *dcdc_type;

/* ========================================================================== */
/*                         Function implementation                            */
/* ========================================================================== */

/**
   SPI device driver probe function

   Called by the kernel so that the driver can test if the device
   is acutally present. First some configurations need to be done.

   \param  pSpiDev      Pointer to the SPI device.

   \return
   <  0 if configuration failed or device did not respond.
   >= 0 if successful
 */
static int sdd_mbx_spi_probe (struct spi_device *pSpiDev)
{
   struct device_node *node,
                      *child_node;
   const char *string;
   int length;
   int ret;

   sdd_mbx_context.device.pSpiDev = pSpiDev;
   sdd_mbx_context.device.nDevState = 0;

   pSpiDev->mode = SPI_MODE_3;

   /* Enable the clock domains before the first access. */
   if (sdd_mbx_context.device.vcodec_clk == NULL)
   {
      sdd_mbx_context.device.vcodec_clk = clk_get(&pSpiDev->dev, "vcodec");
      if (IS_ERR(sdd_mbx_context.device.vcodec_clk)) {
         /* fallback */
         sdd_mbx_context.device.vcodec_clk = clk_get(NULL, "vcodec");
      }

      if (!IS_ERR(sdd_mbx_context.device.vcodec_clk)) {
         /* clk_prepare() not yet available */
         ret = clk_prepare_enable(sdd_mbx_context.device.vcodec_clk);
      }
      else
      {
         dev_warn(&pSpiDev->dev, "failed to get 8kHz VCODEC clock\n");
      }
   }

   if (sdd_mbx_context.device.sslic_clk == NULL)
   {
      sdd_mbx_context.device.sslic_clk = clk_get(&pSpiDev->dev, "clkout0");
      if (IS_ERR(sdd_mbx_context.device.sslic_clk)) {
         /* fallback */
         sdd_mbx_context.device.sslic_clk = clk_get(NULL, "clkout0");
      }

      if (!IS_ERR(sdd_mbx_context.device.sslic_clk)) {
         ret = clk_prepare_enable(sdd_mbx_context.device.sslic_clk);
      }
      else
      {
         dev_warn(&pSpiDev->dev, "failed to get 8MHz SmartSLIC clock\n");
      }
   }

   ret = sdd_mbx_access_test (&sdd_mbx_context.device);
   if(ret != SDD_FW_ERR_NO_ERROR)
   {
      return ret;
   }

   sdd_mbx_context.device.irq = pSpiDev->irq;
   ret = sdd_mbx_irq_register();
   if(ret != SDD_FW_ERR_NO_ERROR)
   {
      return ret;
   }

   /* When the DC/DC converter type name was not set on the commandline try to
      get it from the device tree. */
   if (strlen(dcdc_type) == 0)
   {
      /* VCODEC SPI client node in the device tree */
      node = pSpiDev->dev.of_node;
      /* SLIC node within the VCODEC in the device tree */
      child_node = of_get_next_child(node, NULL);
      if (child_node != NULL)
      {
         string = of_get_property(child_node, "dcdc_type", &length);
         if (string)
         {
            /* Found a DC/DC type name in the device tree.
               Cast needed to drop the const attribute. */
            dcdc_type = (IFX_char_t *)string;
         }

         of_node_put(child_node);
      }
   }

   return ret;
}


/**
   SPI device driver teardown. Unbinds this driver from the spi device.

   \param  pSpiDev      Pointer to the SPI device.

   \return int 0 indicating success
*/
static int sdd_mbx_spi_remove(struct spi_device* pSpiDev)
{
   sdd_mbx_irq_unregister();

   sdd_mbx_context.device.pSpiDev = IFX_NULL;
   sdd_mbx_context.device.nDevState = 0;

   /* Clock in the vcodec domain is no longer needed. */
   if (!IS_ERR(sdd_mbx_context.device.vcodec_clk))
   {
      clk_disable(sdd_mbx_context.device.vcodec_clk);
      clk_put(sdd_mbx_context.device.vcodec_clk);
      sdd_mbx_context.device.vcodec_clk = NULL;
   }
   /* Clock to the SmartSLIC is no longer needed. */
   if (!IS_ERR(sdd_mbx_context.device.sslic_clk))
   {
      clk_disable(sdd_mbx_context.device.sslic_clk);
      clk_put(sdd_mbx_context.device.sslic_clk);
      sdd_mbx_context.device.sslic_clk = NULL;
   }

   return 0;
}


/**
   Init the SPI interface that is used for communication with the VCODEC

   \param  pDev         Pointer to device structure.
 */
IFX_int32_t sdd_mbx_spi_init(SDD_MBX_DEVICE_t *pDev)
{
   spi_register_driver(&sdd_mbx_spi_driver);
   return SDD_FW_ERR_NO_ERROR;
}


/**
   Release the SPI interface that is used for communication with the VCODEC

   \param  pDev         Pointer to device structure.
 */
IFX_int32_t sdd_mbx_spi_exit(SDD_MBX_DEVICE_t *pDev)
{
   /* unregister driver if no any users */
   spi_unregister_driver(&sdd_mbx_spi_driver);

   return SDD_FW_ERR_NO_ERROR;
}


/**
   Test whether the VCODEC host registers can be accessed.

   \param  pDev         Pointer to device structure.

   \return
   error code
*/
IFX_int32_t sdd_mbx_access_test (SDD_MBX_DEVICE_t *pDev)
{
   IFX_uint16_t val = 0;
   IFX_uint16_t i = 0;

   for (i = SDD_MBX_SPI_TEST_INIT_VAL;
        i < (SDD_MBX_MAX_SPI_TEST_ROUNDS + SDD_MBX_SPI_TEST_INIT_VAL); i++)
   {
      sdd_mbx_spi_write(pDev, SDD_MBX_HOST_REG4, &i, 2);
      sdd_mbx_spi_read(pDev, SDD_MBX_HOST_REG4, &val, 2);

      if (i != val)
      {
         ERROR("dev mismatch wr:0x%04X rd:0x%04X", i, val);
         return SDD_FW_ERR_SPI_ACCESS;
      }
   }

   return SDD_FW_ERR_NO_ERROR;
}


/**
   Wait for the SPI interface of the VCODEC to become accessible and set the
   VCODEC IRQ behaviour in the configuration register.

   After a reset via the RESET_N pin (hard reset), the host controller must
   first wait for the SPI interface to become accessible again and then
   configure the basic behavior of the VCODEC (the polarity of the interrupt
   line and behavior of the interrupt acknowledge) before it can access
   the VCODEC.

   \param  pDev         Pointer to the device structure.

   \return
   -SDD_MBX_statusOk
   -SDD_MBX_statusSpiAccErr
 */
IFX_int32_t sdd_mbx_spi_setup(SDD_MBX_DEVICE_t *pDev)
{
   IFX_uint16_t nRegWrite  = 0,
                nRegRead   = 0;
   IFX_uint16_t nLoop      = 0,
                nRetry     = 0;
   IFX_int32_t  ret        = SDD_FW_ERR_NO_ERROR;

   if (pDev->nDevState & (DS_SPI_ACTIVE | DS_SPI_SETUP))
   {
      /* No need to configure more than once. */
      return SDD_FW_ERR_NO_ERROR;
   }

   /* Set flag that is tested above to get out of the recursion that happens
      when sdd_mbx_spi_write() is called below. */
   pDev->nDevState |= DS_SPI_SETUP;

   nRegWrite = (1 << SDD_MBX_HOST_CFG_SC_MD)   |
               (1 << SDD_MBX_HOST_CFG_SC_MDH)  |
               (1 << SDD_MBX_HOST_CFG_INT_MDH) |
               (1 << SDD_MBX_HOST_CFG_INT_MD);

   while (nLoop <= 400)
   {
      /* Write SPI configuration. */
      ret = sdd_mbx_spi_write(pDev, SDD_MBX_HOST_CFG, &nRegWrite, 2);
      if (ret != SDD_FW_ERR_NO_ERROR)
         break;
      /* Read configuration back. */
      ret = sdd_mbx_spi_read(pDev, SDD_MBX_HOST_CFG, &nRegRead, 2);
      if (ret != SDD_FW_ERR_NO_ERROR)
         break;

      /* While the SPI is not accessible it returns 0xFFFF. During this
         we loop until we read a value which is different. */

      /* The high byte is write only and will return 0 on reading */
      if ((nRegWrite & 0xFF) == nRegRead)
      {
         pDev->nDevState &= ~DS_SPI_SETUP;
         pDev->nDevState |= DS_SPI_ACTIVE;
         return SDD_FW_ERR_NO_ERROR;
      }

      if (nRegRead != 0xFFFF)
      {
         /* Retry up to 3 times when read and write do not match. */
         if (nRetry >= 3)
         {
            break;
         }
         nRetry++;
      }
      /* Wait 5ms between register accesses. */
      set_current_state(TASK_INTERRUPTIBLE);
      schedule_timeout(HZ/200);
      nLoop++;
   }

   pDev->nDevState &= ~DS_SPI_SETUP;
   return SDD_FW_ERR_SPI_ACCESS;
}


/**
   Write a number of bytes to the given VCODEC register using SPI.

   If the length of the data exceeds the SPI_MAXBYTES_SIZE this function
   fragments the data into multiple blocks and calls the SPI write interface
   repeatedly until all data is transmitted.

   \param  pDev         Pointer to device structure.
   \param  offset       Register(s) offset/start address.
   \param  pbuf         Pointer to (16-bit word) buffer with data to write.
                        The buffer must be at least "len" bytes long.
   \param  len          Number of bytes to write.

   \return
   error code
*/
IFX_int32_t sdd_mbx_spi_write(SDD_MBX_DEVICE_t* pDev,
                              IFX_uint8_t offset,
                              IFX_uint16_t *pbuf,
                              IFX_uint32_t len)
{
   IFX_uint8_t   tx_buf[SPI_MAXBYTES_SIZE];
   IFX_uint32_t  tx_buf_length,
                 written,
                 fragment_length;
   IFX_int32_t   ret = SDD_FW_ERR_NO_ERROR;

   /* Wait until SPI becomes accessible and configure it. */
   if (sdd_mbx_spi_setup(pDev) != SDD_FW_ERR_NO_ERROR)
   {
      return SDD_FW_ERR_SPI_ACCESS;
   }

   /* lock on task level */
   down(&pDev->spi_access_mtx);

#if 0
ERROR("SPI write 0x%0X length %d: ",
      offset, len);
#endif

   /* This variable counts the number of bytes already written. */
   written = 0;

   /* Build SPI header (16-bit word) */
   tx_buf[0] = SDD_MBX_SPI_HDR_B0_W | SDD_MBX_SPI_HDR_B0_RESERVED;
   tx_buf[1] = offset << 1;
   /* Set auto increment if register isn't a mailbox register. */
   if ((len > 2) && (offset != SDD_MBX_HOST_CB_DATA)
                 && (offset != SDD_MBX_HOST_EB_DATA))
   {
      tx_buf[1] |= SDD_MBX_SPI_HDR_B1_I;
   }
   /* The tx_buf now contains the SPI header. */
   tx_buf_length = SDD_MBX_SPI_HDR_LENGTH;

   /* This is the loop that fragments the data to be written. */
   while((ret != IFX_ERROR) && (written < len))
   {
      /* Calculate the amount of payload bytes that can be sent during one
         write call. */
      fragment_length = len - written; /* remaining bytes to transfer */
      if (fragment_length > (SPI_MAXBYTES_SIZE - tx_buf_length))
      {
         /* Limit the transfer to the maximum the SPI can transmit. */
         fragment_length = (SPI_MAXBYTES_SIZE - tx_buf_length);
      }
      if ((offset == SDD_MBX_HOST_CB_DATA) && (fragment_length > 2))
      {
         /* For mailboxes ensure that the fragment has a multiple of 2 bytes. */
         fragment_length = fragment_length - (fragment_length % 2);
      }
      /* Copy the payload into the write buffer with correct endianess. */
      sdd_mbx_cpw2b(tx_buf+tx_buf_length, pbuf, written, fragment_length);

      /* Increment by the number of payload bytes written. */
      tx_buf_length += fragment_length;
      written += fragment_length;

#if 0
{
   IFX_uint32_t k;
   for (k=0;k<tx_buf_length;k++)
   {
      printk("W:%02X ", tx_buf[k]);
   }
   printk("\n");
}
#endif
      /* Access SPI for writing (read ptr is NULL). */
      ret = spi_write_then_read(pDev->pSpiDev, tx_buf, tx_buf_length, NULL, 0);
      /* Note on the return value: The macro spi_ll_read_write() is supposed to
         return either SDD_FW_ERR_NO_ERROR or IFX_ERROR */
      if(ret < 0)
      {
         ret = IFX_ERROR;
      }
      /* Preserve and reuse the SPI header for the next fragment. */
      tx_buf_length = SDD_MBX_SPI_HDR_LENGTH;
   }

   /* check error */
   if (ret == IFX_ERROR)
   {
      return SDD_FW_ERR_SPI_ACCESS;
   }

   /* unlock protection */
   up(&pDev->spi_access_mtx);

   return SDD_FW_ERR_NO_ERROR;
}


/**
   Read a number of bytes from a given VCODEC register using SPI.

   If the length of the data to be read exceeds the SPI_MAXBYTES_SIZE this
   function calls the SPI read function repeatedly until all data is read
   and reassembles the data in the return buffer.

   \param  pDev         Pointer to device structure.
   \param  offset       Register(s) offset/start address.
   \param  pbuf         Pointer to (16-bit word) buffer where to store the
                        read data. The buffer must be at least "len" bytes long.
   \param  len          Number of bytes to read and store in the buffer.
*/
IFX_int32_t sdd_mbx_spi_read(SDD_MBX_DEVICE_t* pDev,
                            IFX_uint8_t offset,
                            IFX_uint16_t* pbuf,
                            IFX_uint32_t len)
{
   IFX_uint8_t   tx_buf[SPI_MAXBYTES_SIZE],
                 rx_buf[SPI_MAXBYTES_SIZE];
   IFX_uint32_t  tx_buf_length,
                 read,
                 fragment_length;
   IFX_int32_t   ret = SDD_FW_ERR_NO_ERROR;

   /* Wait until SPI becomes accessible and configure it. */
   if (sdd_mbx_spi_setup(pDev) != SDD_FW_ERR_NO_ERROR)
   {
      return SDD_FW_ERR_SPI_ACCESS;
   }

   /* lock on task level */
   down(&pDev->spi_access_mtx);

#if 0
ERROR("SPI read 0x%0X length %d: ",
      offset, len);
#endif

   /* tx_buf is the buffer for writing the read request. Because SPI in general
      is full duplex the complete buffer is set to zero to avoid sending random
      data after the SPI header that we set below. */
   memset (tx_buf, 0x00, sizeof(tx_buf));
   /* Build SPI header (16-bit word) */
   tx_buf[0] = SDD_MBX_SPI_HDR_B0_R | SDD_MBX_SPI_HDR_B0_RESERVED;
   tx_buf[1] = offset << 1;
   /* Set auto increment if register isn't a mailbox register. */
   if ((len > 2) && (offset != SDD_MBX_HOST_CB_DATA)
                 && (offset != SDD_MBX_HOST_EB_DATA))
   {
      tx_buf[1] |= SDD_MBX_SPI_HDR_B1_I;
   }
   /* The tx_buf contains now the SPI header. */
   tx_buf_length = SDD_MBX_SPI_HDR_LENGTH;

   /* Loop while more data needs to be read. */
   for (read = 0;
        (ret == SDD_FW_ERR_NO_ERROR) && (read < len); read += fragment_length)
   {
      /* Calculate the amount of payload bytes that can be read during one
         read call. */
      fragment_length = len - read; /* remaining bytes to read */
      if (fragment_length > (SPI_MAXBYTES_SIZE - tx_buf_length))
      {
         /* Limit the transfer to the maximum the SPI can transmit. */
         fragment_length = (SPI_MAXBYTES_SIZE - tx_buf_length);
      }
      if (((offset == SDD_MBX_HOST_CB_DATA) || (offset == SDD_MBX_HOST_EB_DATA))
          && (fragment_length > 2))
      {
         /* For mailboxes ensure that the fragment has a multiple of 2 bytes. */
         fragment_length = fragment_length - (fragment_length % 2);
      }

#if 0
{
   IFX_uint32_t k;
   printk(KERN_ERR);
   for (k=0;k<tx_buf_length;k++)
   {
      printk("W:%02X ", tx_buf[k]);
   }
}
#endif

      /* Access SPI for writing and reading. */
      ret = spi_write_then_read(pDev->pSpiDev, tx_buf, tx_buf_length,
                                               rx_buf, fragment_length);

#if 0
{
   IFX_uint32_t k;
   for (k=0;k<fragment_length;k++)
   {
      printk("R:%02X ", rx_buf[k]);
   }
   printk("\n");
}
#endif

      if (ret == SDD_FW_ERR_NO_ERROR)
      {
         /* Copy the byte buffer into the 16-bit-word buffer respecting
            the endianess. */

         /* Received data is aligned at offset 0 in the buffer.
            spi_write_then_read() is a half duplex protocol. */
         sdd_mbx_cpb2w (pbuf + (read/2),
                        rx_buf, fragment_length);
      }
   }

   /* check error */
   if (ret != SDD_FW_ERR_NO_ERROR)
   {
      return SDD_FW_ERR_SPI_ACCESS;
   }

   /* unlock protection */
   up(&pDev->spi_access_mtx);

   return ret;
}


/**
   Return the number of payload bytes that can be transferred over SPI
   without being fragmented into multiple SPI transfers.

   \return
   Maximum number of bytes for a single SPI transfer.
*/
IFX_uint32_t SDD_MBX_spi_blocksize_get(void)
{
   return SPI_MAXBYTES_SIZE - SDD_MBX_SPI_HDR_LENGTH;
}


/* ************************ endianess correct copy ****************************/

/**
   Copy a byte buffer into a 16-bit-word buffer respecting the endianess.

   In place copy is possible but otherwise the buffers must not overlap.
   When an odd number of bytes is to be copied the missing byte in the
   16-bit-word is filled with zero.

   \param  pWbuf        Word buffer.
   \param  pBbuf        Byte buffer.
   \param  nB           Number of Bytes to be copied.
*/
void sdd_mbx_cpb2w (    IFX_uint16_t *pWbuf,
                        const IFX_uint8_t * const pBbuf,
                        const IFX_uint32_t nB)
{
   IFX_uint32_t i;
   IFX_uint16_t nWord;

   for (i=0; i < nB; i+=2)
   {
      /* Copy each byte separately into the word buffer. Make sure that when
         an index out of range is addressed zero is copied instead. */
      nWord  = (IFX_uint16_t)pBbuf[i+0] << 8;
      if ((i+1) < nB)
      {
         /* LSB is only added when index is within range. */
         nWord |= (IFX_uint16_t)pBbuf[i+1];
      }
      pWbuf[i>>1] = nWord;
   }
}


/**
   Copy a 16-bit-word buffer into a byte buffer respecting the endianess.

   Odd and even offsets and length are handled by this function.
   In place copy is possible but otherwise the buffers must not overlap.

   \param  pBbuf        Pointer to byte buffer. (Destination)
   \param  pWbuf        Pointer to 16-bit-word buffer. (Source)
   \param  nWoffset     Offset in bytes within the source buffer.
   \param  nB           Number of bytes to be copied.
*/
void sdd_mbx_cpw2b (    IFX_uint8_t *pBbuf,
                        const IFX_uint16_t * const pWbuf,
                        const IFX_uint32_t nWoffset,
                        const IFX_uint32_t nB)
{
   IFX_uint32_t i;
   IFX_uint16_t nWord;

   for (i = 0; i < nB; i++)
   {
      nWord = pWbuf[(nWoffset + i) >> 1];
      pBbuf[i] = (IFX_uint8_t)
                 ((((nWoffset+i) % 2) ? nWord : nWord >> 8) & 0xFF);
   }
}
