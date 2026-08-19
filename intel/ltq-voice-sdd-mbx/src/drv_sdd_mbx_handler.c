/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_handler.c
   This file contains the implementation of TAPI Command line interface
   functions.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_handler.h"
#include "drv_sdd_mbx_firmware.h"
#include "drv_sdd_mbx_interrupt.h"
#include "drv_sdd_mbx_registers.h"
#include "drv_sdd_mbx_trace.h"
#include "ifxos_memory_alloc.h"
#include "ifxos_copy_user_space.h"
#include <linux/sched.h>
#include <linux/module.h>

/* ========================================================================== */
/*                            Macro definitions                               */
/* ========================================================================== */

/* Size of the event mailbox (Event Box Out) in bytes */
#define SDD_MBX_EBO_SIZE                      64

/* Size of the event header in bytes */
#define SDD_MBX_EVENT_HEADER_SIZE              4

/** Message header length. */
#define SDD_MSG_HEADER_LENGTH                  4

/** Payload byte in the header. */
#define SDD_MSG_PAYLOAD_LENGTH                 3

/** Command read bit position */
#define SDD_MSG_READ_BIT_POSITION              7

/* Polling time 10ms */
#define SDD_MBX_WAIT_POLLTIME     (10*(HZ/1000))

/* Loops for command mailbox */
#define SDD_MBX_CMD_MBX_POLL_LOOP            100

/* Wait for command responce */
#define SDD_MBX_WAIT_CMD_TIME                500

/* ========================================================================== */
/*                              Local variables                               */
/* ========================================================================== */
/* Structure holds all settings of loader */
SDD_MBX_CONTEXT_t sdd_mbx_context;

/* String from commandline - overwritten by OS specific code. */
IFX_char_t *dcdc_type = "";


/* ========================================================================== */
/*                         Function implementation                            */
/* ========================================================================== */
/**
   This function reads one command from the mailbox.

   \param pDev pointer to device structure

   \return IFX_int32_t SDD_FW_ERR_NO_ERROR or error code.
*/
IFX_int32_t sdd_mbx_command_read(SDD_MBX_DEVICE_t* pDev)
{
   IFX_uint32_t err = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t nMbxOutLen;

   /* Command size in bytes */
   IFX_uint16_t len = pDev->cmd_buffer[SDD_MSG_PAYLOAD_LENGTH] +
      SDD_MSG_HEADER_LENGTH;

   err = sdd_mbx_reg_read_word(pDev, SDD_MBX_HOST_CB_LEN_OUT, &nMbxOutLen);
   TRACE("command mailbox size: %d, ret: %d", nMbxOutLen, err);

   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("command mbx length reading failed");
      return err;
   }

   if (len > (nMbxOutLen << 1))
   {
      ERROR("command mbx length is too short");
      return SDD_FW_ERR_ENODATA;
   }

   if ((err == SDD_FW_ERR_NO_ERROR))
   {
      err = sdd_mbx_reg_read(pDev, SDD_MBX_HOST_CB_DATA,
                            (IFX_uint16_t*)sdd_mbx_context.device.cmd_buffer,
                             len>>1);

      sdd_mbx_context.device.cmd_buffer_length = len;

      /* Wake up command read semaphore */
      IFXOS_EventWakeUp (&sdd_mbx_context.device.cmd_irq_evt);
   }

   return err;
}

/**
   This function reads one event from the mailbox.

   \param pDev pointer to device structure
   \param pData pointer to buffer
   \param buffer_len length of provided buffer

   \return IFX_int32_t number of readed bytes or -errno
*/
IFX_int32_t sdd_mbx_event_read(SDD_MBX_DEVICE_t* pDev, IFX_uint8_t* pData,
                                      IFX_uint32_t buffer_len)
{
   IFX_uint32_t err = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t nMbxOutLen = 0;
   IFX_uint16_t len = 0;

   /* read event outbox data length register (HOST_LEN_EB) */
   err = sdd_mbx_reg_read_word(&sdd_mbx_context.device,
                          SDD_MBX_HOST_EB_LEN_OUT, &nMbxOutLen);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("event mbx length reading failed");
      return -err;
   }

   if ((nMbxOutLen << 2) < SDD_MBX_EVENT_HEADER_SIZE)
   {
      ERROR("error sdd_mbx_read EBO length too short %d", nMbxOutLen);
      return -SDD_FW_ERR_ENODATA;
   }

   /* Read the event header into output buffer. */
   err = sdd_mbx_reg_read(&sdd_mbx_context.device,
                          SDD_MBX_HOST_EB_DATA, (IFX_uint16_t*)pData, SDD_MSG_HEADER_LENGTH/2);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("error reading event header");
      return -err;
   }

   len = pData[SDD_MSG_PAYLOAD_LENGTH];

   /* Check if mailbox length is enough to read payload */
   if(((nMbxOutLen - SDD_MBX_EVENT_HEADER_SIZE/2) << 2) < len)
   {
      ERROR("payload is too short!");
      return -SDD_FW_ERR_ENODATA;
   }

   /* Read the event payload into output buffer. */
   err = sdd_mbx_reg_read(&sdd_mbx_context.device,
                          SDD_MBX_HOST_EB_DATA, (IFX_uint16_t*)(pData+4), len/2);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("error reading event payload");
      return -err;
   }

   /* Add the header to the length field. */
   len += SDD_MSG_HEADER_LENGTH;

   return len;
}

/**
   Open the mailbox

   \param s       Mailbox name that can be either "command" or "event"

   \return
      Mailbox descriptor of SDD_MBX_t type on success
      IFX_NULL in case of an error. last_err should be used to retrieve
      the cause value. Cause value is of e_SDD_FW_Errors type.
*/
static IFX_int32_t sdd_mbx_open(IFX_char_t *s)
{
   TRACE("%s mailbox", s);

   if (!strcmp(s, "command"))
   {
#ifdef MODULE
      try_module_get(THIS_MODULE);
#endif
      IFXOS_EventDelete(&sdd_mbx_context.device.cmd_irq_evt);
      IFXOS_EventInit(&sdd_mbx_context.device.cmd_irq_evt);
      sdd_mbx_irq_host_enable(&sdd_mbx_context.device, SDD_MBX_HOST_CB_DATA);
      sdd_mbx_context.device.vcodec_error = SDD_FW_ERR_NO_ERROR;
      return SDD_FW_ERR_NO_ERROR;
   }
   else if (!strcmp(s, "event"))
   {

#ifdef MODULE
      try_module_get(THIS_MODULE);
#endif
      sdd_mbx_irq_host_enable(&sdd_mbx_context.device, SDD_MBX_HOST_EB_DATA);
      sdd_mbx_context.device.vcodec_error = SDD_FW_ERR_NO_ERROR;
      return SDD_FW_ERR_NO_ERROR;
   }
   else
   {
      ERROR("unknown mailbox");
      return SDD_FW_ERR_INVALID_PARAMS;
   }
}

/**
   This function cleans-up reserved memory.

   \param mbx pointer to SDD_MBX_t

   \return IFX_int32_t always 0
*/
static IFX_int32_t sdd_mbx_close(
                        IFX_char_t *s)
{
   TRACE("%s mailbox", s);
   if (!strcmp(s, "command"))
   {
#ifdef MODULE
      module_put(THIS_MODULE);
#endif
      /* Disable rising commands IRQs */
      sdd_mbx_irq_host_disable(&sdd_mbx_context.device, SDD_MBX_HOST_CB_DATA);
      /* Close cmd outbox data ready event */
      IFXOS_EventDelete(&sdd_mbx_context.device.cmd_irq_evt);
      return SDD_FW_ERR_NO_ERROR;
   }
   else if (!strcmp(s, "event"))
   {

#ifdef MODULE
      module_put(THIS_MODULE);
#endif
      /* Disable rising event IRQs */
      sdd_mbx_irq_host_disable(&sdd_mbx_context.device, SDD_MBX_HOST_EB_DATA);
      return SDD_FW_ERR_NO_ERROR;
   }
   else
   {
      ERROR("unknown mailbox");
      return SDD_FW_ERR_INVALID_PARAMS;
   }
}

/**
   This function count additional delay in the driver. It is needed in case of
   event logger and debug traces level.

   \return IFX_int32_t delay time in ms.
*/
static IFX_int32_t sdd_mbx_path_delay(IFX_void_t)
{
   IFX_int32_t delay = 0;

#ifdef EVENT_LOGGER_DEBUG
   delay += 10;
#endif

   if(sdd_mbx_context.trace_level == TRACE_DEBUG)
   {
      delay += 20;
   }

   TRACE("added: %d ms", delay);

   return delay;
}

/**
   Waits until the requested amount of space is available in the command-inbox
   or until the timeout occurs.

   The function returns immediately with success (SDD_FW_ERR_NO_ERROR) if the
   requested amount of space is available in the command-inbox. Otherwise it
   repeatedly reads the field ILEN of register HOST_LEN_CB until the requested
   amount of bytes become available in the command-inbox. A maximum time of
   (SDD_MBX_WAIT_POLLTIME * SDD_MBX_CMD_MBX_POLL_LOOP)us can be waited in the
   process. Upon timeout the function resets the command-inbox, waits for end of
   the reset and returns the error SDD_FW_ERR_CMD_MBX_NO_SPACE.

   \param nCount        Requested amount of space in bytes to wait for.
                        No check is done on this parameter!

   \return
   - SDD_FW_ERR_NO_ERROR
   - SDD_FW_ERR_MBX_READ
   - SDD_FW_ERR_CMD_MBX_NO_SPACE

   \remarks
   This function does not perform any protection against concurrent accesses
   (either interrupt or multi-task). It is an obligation to the calling function
   to provide such protection.
*/
IFX_int32_t sdd_mbx_wait_for_space (IFX_uint8_t nCount)
{
   IFX_int32_t    nWaitCnt = 0;
   IFX_uint16_t   nSpace   = 0;
   IFX_int32_t    err      = SDD_FW_ERR_NO_ERROR;

   do
   {
      err = sdd_mbx_reg_read_word (&sdd_mbx_context.device,
                              SDD_MBX_HOST_CB_LEN_IN, &nSpace);

      if (err != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error read length register: %d", err);
         return SDD_FW_ERR_MBX_READ;
      }

      /* shift to get number of bytes from 16bit words */
      if (nCount <= (nSpace << 1))
      {
         /* Sufficient free space in command-inbox to grant the request. */
         return SDD_FW_ERR_NO_ERROR;
      }
      /* Not enough free space in the command-inbox, we have to wait until more
         space becomes available and start the loop again after a short delay.
         The iterations are limited and the resulting timeout is:
         (WAIT_POLLTIME * SDD_MBX_FIBXMS_POLL_LOOP)us. */
      set_current_state(TASK_INTERRUPTIBLE);
      /* Wait 10 ms */
      schedule_timeout(SDD_MBX_WAIT_POLLTIME);
   } while (++nWaitCnt < SDD_MBX_CMD_MBX_POLL_LOOP);

   /* errmsg: Not enough inbox space for writing command. */
   return SDD_FW_ERR_CMD_MBX_NO_SPACE;
}

/**
   Write to the mailbox

   \param mbx      Mailbox descriptor of \ref SDD_MBX_t type
   \param pData    Starting of buffer to be written
   \param len      Byte count to be written
   \return
      SDD_FW_ERR_NO_ERROR on success
      Negative value in case of an error
*/
static IFX_int32_t sdd_mbx_cmd_write (
                        IFX_uint8_t *pData,
                        IFX_uint16_t len)
{
   IFX_int32_t ret;

   /* Check for VCODEC error flags, in this case we stop the communication */
   if(sdd_mbx_context.device.vcodec_error != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("VCODEC error flag is set: %d", sdd_mbx_context.device.vcodec_error);
      return sdd_mbx_context.device.vcodec_error;
   }

   /* Command length must have at least the header! */
   if(len < SDD_MSG_HEADER_LENGTH)
   {
      ERROR("command length is too short: %d", len);
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   ret = sdd_mbx_wait_for_space(len);

   if (ret == SDD_FW_ERR_NO_ERROR)
   {
      /* Command length is in bytes - div by 2 */
      ret = sdd_mbx_reg_write(&sdd_mbx_context.device, SDD_MBX_HOST_CB_DATA,
                              (IFX_uint16_t*) pData, (IFX_uint32_t) len >> 1);
#ifdef EVENT_LOGGER_DEBUG
      /* Log only command write messages */
      if(!CHECK_BIT(pData[0], SDD_MSG_READ_BIT_POSITION))
      {
         EL_LOG_EVENT_CMD_WR(DEV_TYPE_SDD, 0, 0, pData, len >> 1, ret);
      }
#endif /* EVENT_LOGGER_DEBUG */
      if(ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("write to %d mbx error: %d", SDD_MBX_HOST_CB_DATA, ret);
      }
      else
      {
         TRACE("write to %d mbx %d bytes", SDD_MBX_HOST_CB_DATA, len);
      }
   }
   else
   {
      ERROR("wait for space error: %d", ret);
   }

   return ret;
}

/**
   Read from the mailbox

   \param pData    Starting of buffer into which the data will be stored
   \param len      Number of bytes to read into the buffer. The buffer
                   needs to have this minimum size.

   \return
      Read byte count on success
      IFX_ERROR in case of an error. last_err should be used to retrieve
      the cause value. Cause value is of \ref e_SDD_FW_Errors type.
   \remarks
      This function reads a single data message from the mailbox. The
      message length is provided by the underlying firmware driver and is
      used as a return value of this function.
*/
static IFX_int32_t sdd_mbx_cmd_read(
                        IFX_uint8_t *pData,
                        IFX_uint16_t len)
{
   IFX_int32_t    ret = SDD_FW_ERR_NO_ERROR;

   /* Lock accesss to the mailbox. */
   mutex_lock(&sdd_mbx_context.device.cmd_mtx);

   /* Check for VCODEC error flags, in this case we stop the communication */
   if(sdd_mbx_context.device.vcodec_error != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("VCODEC error flag is set: %d", sdd_mbx_context.device.vcodec_error);
      mutex_unlock(&sdd_mbx_context.device.cmd_mtx);
      return sdd_mbx_context.device.vcodec_error;
   }

   /* Copy header of a message to internal buffer. */
   memcpy(sdd_mbx_context.device.cmd_buffer, pData, SDD_MSG_HEADER_LENGTH);

   /* Write the header of a message. */
   ret = sdd_mbx_cmd_write(pData, SDD_MSG_HEADER_LENGTH);

   if (ret == SDD_FW_ERR_NO_ERROR)
   {
      TRACE("wait for response");

      /* Read command was written successfully. Wait for data and read it. */
      if(IFXOS_EventWait(&sdd_mbx_context.device.cmd_irq_evt,
             SDD_MBX_WAIT_CMD_TIME + sdd_mbx_path_delay(), &ret) == IFX_SUCCESS)
      {
         TRACE("wait done");
         if(sdd_mbx_context.device.cmd_buffer_length <= len)
         {
            /* Copy the message from internal buffer. */
            memcpy(pData, sdd_mbx_context.device.cmd_buffer,
                   sdd_mbx_context.device.cmd_buffer_length);
#ifdef EVENT_LOGGER_DEBUG
            EL_LOG_EVENT_CMD_RD(DEV_TYPE_SDD, 0, 0, sdd_mbx_context.device.cmd_buffer,
                                sdd_mbx_context.device.cmd_buffer,
                                sdd_mbx_context.device.cmd_buffer_length >> 1,
                                SDD_FW_ERR_NO_ERROR);
#endif /* EVENT_LOGGER_DEBUG */
            ret = SDD_FW_ERR_NO_ERROR;
         }
         else
         {
            ERROR("no space in the output buffer");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      else
      {
         if(ret == 1)
         {
            ERROR("Wait timeout");

            /* Additional information in case of timeout. */
            if(sdd_mbx_context.trace_level == TRACE_DEBUG)
            {
               IFX_uint16_t reg = 0;
               sdd_mbx_reg_read_word(&sdd_mbx_context.device,
                                SDD_MBX_HOST_STAT1, &reg);
               TRACE("SDD_MBX_HOST_STAT1: 0x%X", reg);
               sdd_mbx_reg_read_word(&sdd_mbx_context.device,
                                SDD_MBX_HOST_INT1, &reg);
               TRACE("SDD_MBX_HOST_INT1: 0x%X", reg);
               sdd_mbx_reg_read_word(&sdd_mbx_context.device,
                                SDD_MBX_HOST_CB_LEN_OUT, &reg);
               TRACE("SDD_MBX_HOST_CB_LEN_OUT: 0x%X", reg);
               TRACE("IRQ status: %d", sdd_mbx_context.device.irq_enabled);
            }
            ret = SDD_FW_ERR_WAIT_TIMEOUT;
         }
         else
         {
            ERROR("wait interrupted");
            ret = SDD_FW_ERR_WAIT_INT;
         }
      }
   }
   else
   {
      ERROR("write header error");
   }

   /* Unlock protection */
   mutex_unlock(&sdd_mbx_context.device.cmd_mtx);

   return ret;
}

/**
   This function downloads and starts the firmware

   \param  pFwImage     Pointer to address with FW image.
   \param  nFwSize      Size of the firmware image in bytes.

   \return IFX_int32_t last error
*/
static IFX_int32_t sdd_mbx_fw_dwld (
                        IFX_uint8_t *pFwImage,
                        IFX_uint32_t nFwSize)
{
   IFX_int32_t ret = SDD_FW_ERR_NO_ERROR;

   ret = sdd_mbx_firmware(&sdd_mbx_context.device, pFwImage, nFwSize);
   if(ret != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("error: %d", ret);
   }

   return ret;
}

/**
   Mailbox ioctl

   \param cmd      Ioctl command
   \param param    Ioctl command parameters
   \return
      IFX_SUCCESS on success
      IFX_ERROR in case of an error
   \remarks
      This function is optional
*/
IFX_int32_t sdd_mbx_ioctl_handler(
                        IFX_uint32_t cmd,
                        IFX_void_t *param)
{
   IFX_long_t ret = SDD_FW_ERR_NO_ERROR;

   if((param == IFX_NULL) && (cmd != SDD_MBX_RESET))
   {
      ERROR("invalid parameter");
      return SDD_FW_ERR_INVALID_PARAMS;
   }

   switch (cmd)
   {
      case SDD_MBX_DEBUG_MODE:
      {
         enum SDD_Debug_Mode_t debug_mode;
         IFXOS_CpyFromUser((IFX_uint8_t*)&debug_mode, (IFX_uint8_t*)param,
                           sizeof(debug_mode));
         TRACE("call SDD_MBX_TEST_STATE");
         if(debug_mode == SDD_DEBUG_ON)
         {
            TRACE("start test interface");
            sdd_mbx_open("command");
            sdd_mbx_close("event");

            sdd_mbx_irq_system_enable(&sdd_mbx_context.device);

            sdd_mbx_context.debug_mode = SDD_DEBUG_ON;
         }
         else if(debug_mode == SDD_DEBUG_OFF)
         {
            TRACE("stop test interface");
            sdd_mbx_irq_host_enable(&sdd_mbx_context.device, SDD_MBX_HOST_EB_DATA);
            sdd_mbx_context.debug_mode = SDD_DEBUG_OFF;
         }
         else
         {
            ERROR("unknown test interface state: 0x%x@%p", debug_mode, param);
            ret = SDD_FW_ERR_INVALID_PARAMS;
         }
      }
      break;

      case SDD_MBX_REG_READ:
      {
         FIO_SDD_MBX_REG_IO_t *reg_read = IFXOS_BlockAlloc(sizeof(FIO_SDD_MBX_REG_IO_t));

         TRACE("call SDD_MBX_REG_READ");
         if(reg_read != IFX_NULL)
         {
            IFXOS_CpyFromUser((IFX_uint8_t*)reg_read, (IFX_uint8_t*)param,
                              sizeof (FIO_SDD_MBX_REG_IO_t));

            ret = sdd_mbx_reg_read_word(&sdd_mbx_context.device, reg_read->reg,
                                   &reg_read->data);

            if (ret != SDD_FW_ERR_NO_ERROR)
            {
               ERROR("error write to %d: %d", reg_read->reg, ret);
            }
            else
            {
               IFXOS_CpyToUser((IFX_uint8_t*)param,
                                 reg_read, sizeof (FIO_SDD_MBX_REG_IO_t));
            }

            IFXOS_BlockFree(reg_read);
         }
         else
         {
            ERROR("memory allocate error");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      break;

      case SDD_MBX_REG_WRITE:
      {
         FIO_SDD_MBX_REG_IO_t *reg_write = IFXOS_BlockAlloc(sizeof(FIO_SDD_MBX_REG_IO_t));

         TRACE("call SDD_MBX_REG_WRITE");
         if(reg_write != IFX_NULL)
         {
            IFXOS_CpyFromUser((IFX_uint8_t*)reg_write, (IFX_uint8_t*)param,
                              sizeof (FIO_SDD_MBX_REG_IO_t));

            ret = sdd_mbx_reg_write_word(&sdd_mbx_context.device, reg_write->reg,
                                   &reg_write->data);

            if (ret != SDD_FW_ERR_NO_ERROR)
            {
               ERROR("error read from %d: %d", reg_write->reg, ret);
            }

            IFXOS_BlockFree(reg_write);
         }
         else
         {
            ERROR("memory allocate error");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      break;

      case SDD_MBX_RESET:
      {
         TRACE("call SDD_MBX_RESET");
         ret = sdd_mbx_reset_controller(&sdd_mbx_context.device, SDD_MBX_BOOT_SPI);
      }
      break;

      case SDD_MBX_EVT_READ:
      {
         FIO_SDD_MBX_READ_t *evt_read = IFXOS_BlockAlloc(sizeof(FIO_SDD_MBX_READ_t));

         TRACE("call SDD_MBX_EVT_READ");
         if(evt_read != IFX_NULL)
         {
            ret = sdd_mbx_event_read(&sdd_mbx_context.device, (IFX_uint8_t*) evt_read,
                                    sizeof(FIO_SDD_MBX_READ_t));

            if (ret <= 0)
            {
               ERROR("error during event read: %d", ret);
            }
            else
            {
               IFXOS_CpyToUser((IFX_uint8_t*)param,
                                 evt_read, sizeof (FIO_SDD_MBX_READ_t));
               ret = SDD_FW_ERR_NO_ERROR;
            }

            IFXOS_BlockFree(evt_read);
         }
         else
         {
            ERROR("memory allocate error");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      break;

      case SDD_MBX_CMD_READ:
      {
         FIO_SDD_MBX_READ_t *cmd_read = IFXOS_BlockAlloc(sizeof(FIO_SDD_MBX_READ_t));

         TRACE("call SDD_MBX_CMD_READ");
         if(cmd_read != IFX_NULL)
         {
            IFXOS_CpyFromUser((IFX_uint8_t*)cmd_read, (IFX_uint8_t*)param,
                              sizeof (FIO_SDD_MBX_READ_t));
            ret = sdd_mbx_cmd_read((IFX_uint8_t*) cmd_read,
                                    sizeof(FIO_SDD_MBX_READ_t));

            if (ret != SDD_FW_ERR_NO_ERROR)
            {
               ERROR("error during command read: %d", ret);
            }
            else
            {
               IFXOS_CpyToUser((IFX_uint8_t*)param,
                                 cmd_read, sizeof (FIO_SDD_MBX_READ_t));
               ret = SDD_FW_ERR_NO_ERROR;
            }

            IFXOS_BlockFree(cmd_read);
         }
         else
         {
            ERROR("memory allocate error");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      break;

      case SDD_MBX_CMD_WRITE:
      {
         FIO_SDD_MBX_READ_t *cmd_write = IFXOS_BlockAlloc(sizeof(FIO_SDD_MBX_READ_t));

         TRACE("call SDD_MBX_CMD_WRITE");
         if(cmd_write != IFX_NULL)
         {
            IFX_uint32_t len = 0;

            IFXOS_CpyFromUser((IFX_uint8_t*)cmd_write, (IFX_uint8_t*)param,
                              sizeof (FIO_SDD_MBX_READ_t));

            len = (cmd_write->data[0] & 0xFF) + SDD_MSG_HEADER_LENGTH;

            sdd_mbx_cmd_write((IFX_uint8_t*) cmd_write, len);

            IFXOS_BlockFree(cmd_write);
            ret = SDD_FW_ERR_NO_ERROR;
         }
         else
         {
            ERROR("memory allocate error");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      break;

      case SDD_MBX_FW_DOWNLOAD:
      {
         SDD_MBX_FW_Download_t *fw_header = IFXOS_BlockAlloc(sizeof(SDD_MBX_FW_Download_t));

         TRACE("call SDD_MBX_FW_DOWNLOAD");
         if(fw_header != IFX_NULL)
         {
            IFXOS_CpyFromUser((IFX_uint8_t*)fw_header, (IFX_uint8_t*)param,
                              sizeof (SDD_MBX_FW_Download_t));

            sdd_mbx_fw_dwld(fw_header->pPRAMfw, fw_header->pram_size);

            IFXOS_BlockFree(fw_header);
            ret = SDD_FW_ERR_NO_ERROR;
         }
         else
         {
            ERROR("memory allocate error");
            ret = SDD_FW_ERR_NO_MEMORY;
         }
      }
      break;

      default:
      {
         ret = SDD_FW_ERR_INVALID_PARAMS;
      }
      break;
   }

   return ret;
}

/**
   Returns the DC/DC converter name

   \param  pDcDcName    Pointer to a buffer with the name.

   \return IFX_int32_t last error
*/
static IFX_int32_t sdd_get_dcdc_name(
                        IFX_char_t **pDcDcName)
{
   *pDcDcName = dcdc_type;

   return SDD_FW_ERR_NO_ERROR;
}

/* this api will be registered by TAPI LL driver */
SDD_FIRMWARE_IF_t sdd_mbx_interface =
{
   sdd_mbx_open,
   sdd_mbx_close,
   sdd_mbx_fw_dwld,
   sdd_mbx_cmd_read,
   sdd_mbx_cmd_write,
   IFX_NULL,
   IFX_NULL,
   sdd_mbx_ioctl_handler,
   sdd_mbx_path_delay,
   sdd_get_dcdc_name
};

SDD_FIRMWARE_IF_t* sdd_mbx_interface_get(IFX_void_t)
{
   sdd_mbx_context.interface = &sdd_mbx_interface;
   return &sdd_mbx_interface;
}

IFX_void_t sdd_mbx_interface_release(IFX_void_t)
{
   sdd_mbx_interface.evt_read_callback = IFX_NULL;
   sdd_mbx_interface.error_callback = IFX_NULL;
}

EXPORT_SYMBOL(sdd_mbx_interface_get);
EXPORT_SYMBOL(sdd_mbx_interface_release);


