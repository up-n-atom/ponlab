/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_interrupt.c
   This file contains the implementation of interrupt handling.
 */

/* ========================================================================== */
/*                                 Includes                                   */
/* ========================================================================== */
#include "drv_sdd_mbx_handler.h"
#include "drv_sdd_mbx_trace.h"
#include "drv_sdd_mbx_interrupt.h"
#include "drv_sdd_mbx_registers.h"
#include "ifxos_mutex.h"
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/sched.h>

#define SDD_MBX_EVENT_BUFFER_LENGTH 20

extern SDD_MBX_CONTEXT_t sdd_mbx_context;
spinlock_t sdd_mbx_spinlock;
struct semaphore sdd_mbx_ien_protect;

static irqreturn_t sdd_mbx_irq_handler(IFX_int32_t irq, void* pDev)
{
   sdd_mbx_irq_system_disable(pDev);

   return IRQ_WAKE_THREAD;
}

static IFX_uint32_t sdd_mbx_check_mbx_err(SDD_MBX_DEVICE_t* pDev)
{
   IFX_uint16_t      nRegHostIrq2   = 0x0;
   IFX_int32_t       err            = SDD_FW_ERR_NO_ERROR;

   err = sdd_mbx_reg_read_word(pDev, SDD_MBX_HOST_INT2, &nRegHostIrq2);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("error host status 2 interrupt register: %d", err);
      sdd_mbx_irq_system_enable(pDev);
      return IFX_ERROR;
   }

   TRACE("INT2: %x", nRegHostIrq2);

   /* Command Mailbox Out */
   if (CHECK_BIT(nRegHostIrq2, SDD_MBX_HOST_INT2_O1_UFL))
   {
      TRACE("command mailbox underflow");
      pDev->vcodec_error = SDD_FW_ERR_CMD_MBX_UFL;
   }

   if (CHECK_BIT(nRegHostIrq2, SDD_MBX_HOST_INT2_I1_OFL))
   {
      TRACE("command mailbox overflow");
      pDev->vcodec_error = SDD_FW_ERR_CMD_MBX_OFL;
   }

   /* Event Mailbox Out */
   if (CHECK_BIT(nRegHostIrq2, SDD_MBX_HOST_INT2_O2_UFL))
   {
      TRACE("event mailbox underflow");
      pDev->vcodec_error = SDD_FW_ERR_EVT_MBX_UFL;
   }

   /* Event Mailbox Out */
   if (CHECK_BIT(nRegHostIrq2, SDD_MBX_HOST_INT2_I2_OFL))
   {
      ERROR("event mailbox overflow");
      pDev->vcodec_error = SDD_FW_ERR_EVT_MBX_OFL;
   }

   /* Dont clear error flag */
#if 0
   /* clear the irq by writing to the irq register */
   err = sdd_mbx_reg_write(pDev, SDD_MBX_HOST_INT2, &nRegHostIrq2, 1);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("clear error flags");
      err = IFX_ERROR;
   }
#endif
   return SDD_FW_ERR_NO_ERROR;
}

static irqreturn_t sdd_mbx_irq_thread_handler(int irq, void* dev_id)
{
   SDD_MBX_DEVICE_t* pDev           = (SDD_MBX_DEVICE_t*) dev_id;
   IFX_int32_t       err            = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t      nRegHostIrq1   = 0;
   (void)            irq;

   /* No event handling when SPI access has not been verified yet or
      the event handling flag indicates STOP. */
   if (!(pDev->nDevState & DS_SPI_ACTIVE))
   {
      ERROR("IRQ received while SPI is inactive");
      sdd_mbx_irq_system_enable(pDev);
      return IRQ_HANDLED;
   }

   /* read "host status 1 interrupt register" (HOST_INT1) */
   err = sdd_mbx_reg_read_word (pDev, SDD_MBX_HOST_INT1, &nRegHostIrq1);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("error host status 1 interrupt register: %d", err);
   }

   TRACE("INT1: %x", nRegHostIrq1);

   /* Check for errors */
   if (CHECK_BIT(nRegHostIrq1, SDD_MBX_HOST_INT1_ERR))
   {
      /* Check detailed error flags */
      err = sdd_mbx_check_mbx_err(pDev);
      if(err != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error in communication with VCODEC");
      }
      else if(pDev->vcodec_error != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("VCODEC error occurs (%d) - IRQ are disabled", pDev->vcodec_error);
      }
      else
      {
         ERROR("Unknown error");
      }

      if(sdd_mbx_context.interface->error_callback != IFX_NULL)
      {
         sdd_mbx_context.interface->error_callback(pDev->vcodec_error);
      }

      /* We don't reenable IRQ line in error case we stop the communication. */
      return IRQ_HANDLED;
   }

   /* Command Mailbox Out */
   if ((err == SDD_FW_ERR_NO_ERROR) &&
       CHECK_BIT(nRegHostIrq1, SDD_MBX_HOST_INT1_O1_RDY))
   {
      sdd_mbx_command_read(pDev);
   }

   /* Event Mailbox Out - only one reason in one IRQ */
   if (CHECK_BIT(nRegHostIrq1, SDD_MBX_HOST_INT1_O2_RDY) &&
      (!CHECK_BIT(nRegHostIrq1, SDD_MBX_HOST_INT1_O1_RDY)))
   {
      IFX_uint8_t pData[SDD_MBX_EVENT_BUFFER_LENGTH];

      TRACE("event mailbox wake up: %x", nRegHostIrq1);

      /* Do not handle the event mailbox while in debug mode. */
      if(sdd_mbx_context.debug_mode != SDD_DEBUG_ON)
      {
         err = sdd_mbx_event_read(&sdd_mbx_context.device, pData, 20);

         if(err > 0)
         {
#ifdef EVENT_LOGGER_DEBUG
            EL_LOG_EVENT_EVT_MBX_RD(DEV_TYPE_SDD, 0, pData[1], pData, err >> 1);
#endif /* EVENT_LOGGER_DEBUG */
            if(sdd_mbx_context.interface->evt_read_callback != IFX_NULL)
            {
               sdd_mbx_context.interface->evt_read_callback(pData, err);
            }
         }
      }
      else
      {
         ERROR("event IRQ during debug mode - should be disabled");
      }
   }

   sdd_mbx_irq_system_enable(pDev);

   return IRQ_HANDLED;
}


IFX_int32_t sdd_mbx_irq_register()
{
   IFX_int32_t    err = 0;

   if (!sdd_mbx_context.device.irq)
   {
      ERROR("irq number error");
      return -1;
   }

   /* Init spinlock for enable/disable IRQ */
   spin_lock_init(&sdd_mbx_spinlock);

   /* Init semaphore for enable/disable interrupt enable registers. */
   sema_init(&sdd_mbx_ien_protect, 1);

   err = request_threaded_irq(sdd_mbx_context.device.irq, sdd_mbx_irq_handler,
                              sdd_mbx_irq_thread_handler, 0, "sdd_mbx",
                              (IFX_void_t*)&sdd_mbx_context.device);
   if (err != 0)
   {
      ERROR("request_irq error! %d", err);
      return err;
   }

   /* Disable IRQ needs IRQ flag set */
   sdd_mbx_context.device.irq_enabled = 1;
   sdd_mbx_irq_system_disable(&sdd_mbx_context.device);

   return err;
}


IFX_void_t sdd_mbx_irq_unregister()
{
   free_irq(sdd_mbx_context.device.irq, (IFX_void_t*)&sdd_mbx_context.device);
   TRACE("IRQ unregistered");
}

/**
   This function enables the MBX interrupts.

   \param pDev pointer to SDD_MBX_DEVICE_t

   \return IFX_int32_t SDD_FW_ERR_NO_ERROR or error code
*/
IFX_int32_t sdd_mbx_irq_vcodec_enable(SDD_MBX_DEVICE_t* pDev)
{
   IFX_int32_t  err  = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t value = 0;

   SET_BIT(value, SDD_MBX_HOST_CFG_INT_MD);
   SET_BIT(value, SDD_MBX_HOST_CFG_SC_MD);
   SET_BIT(value, SDD_MBX_HOST_CFG_INT_MDH);
   SET_BIT(value, SDD_MBX_HOST_CFG_SC_MDH);

   /* Write auto cleaning configuration. */
   err = sdd_mbx_reg_write_word(pDev, SDD_MBX_HOST_CFG, &value);
   if (err != SDD_FW_ERR_NO_ERROR)
   {
      ERROR("error on config write: %d", err);
      return err;
   }

   /* Enable IRQ line */
   sdd_mbx_irq_system_enable(pDev);

   return err;
}

/**
   This function enables linux IRQ.

   \param pDev pointer to SDD_MBX_DEVICE_t

   \return IFX_void_t
*/
IFX_void_t sdd_mbx_irq_system_enable(SDD_MBX_DEVICE_t* pDev)
{
   unsigned long flags = 0;

   spin_lock_irqsave(&sdd_mbx_spinlock, flags);

   /* Prevent to reenable IRQ to avoid nested lock. */
   if((pDev != NULL) && (pDev->irq_enabled == 0))
   {
      pDev->irq_enabled = 1;
      enable_irq(pDev->irq);
      TRACE("fine");
   }
   else
   {
      ERROR("error");
   }

   spin_unlock_irqrestore(&sdd_mbx_spinlock, flags);
}

/**
   This function disables linux IRQ.

   \param pDev pointer to SDD_MBX_DEVICE_t

   \return IFX_void_t
*/
IFX_void_t sdd_mbx_irq_system_disable(SDD_MBX_DEVICE_t* pDev)
{
   unsigned long flags = 0;

   spin_lock_irqsave(&sdd_mbx_spinlock, flags);

   /* Prevent to redisable IRQ to avoid nested lock. */
   if((pDev != NULL) && (pDev->irq_enabled == 1))
   {
      disable_irq_nosync(pDev->irq);
      pDev->irq_enabled = 0;
   }

   spin_unlock_irqrestore(&sdd_mbx_spinlock, flags);
}

IFX_int32_t sdd_mbx_irq_host_enable(SDD_MBX_DEVICE_t* device, SDD_MBX_t mbx)
{
   IFX_int32_t       ret    = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t      value  = 0;

   down(&sdd_mbx_ien_protect);

   if (mbx == SDD_MBX_HOST_CB_DATA)
   {
      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      value |= (1 << SDD_MBX_HOST_IEN1_O1_RDY) |
               (1 << SDD_MBX_HOST_IEN1_ERR);

      /* Enable ERR and O1_RDY IRQ. */
      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error write to IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Enable UFL and OFL IRQ. */
      value |= (1 << SDD_MBX_HOST_IEN2_O1_UFL) |
               (1 << SDD_MBX_HOST_IEN2_I1_OFL);

      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      TRACE("command mailbox enabled");
   }
   else if (mbx == SDD_MBX_HOST_EB_DATA)
   {
      /* Enable IRQ for event mailbox */
      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      SET_BIT(value, SDD_MBX_HOST_IEN1_O2_RDY);
      SET_BIT(value, SDD_MBX_HOST_IEN1_ERR);

      /* Enable ERR and O1_RDY IRQ. */
      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Enable UFL and OFL IRQ. */
      SET_BIT(value, SDD_MBX_HOST_IEN2_O2_UFL);
      SET_BIT(value, SDD_MBX_HOST_IEN2_I2_OFL);

      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      TRACE("event mailbox enabled");
   }

   up(&sdd_mbx_ien_protect);

   return SDD_FW_ERR_NO_ERROR;
}

IFX_int32_t sdd_mbx_irq_host_disable(SDD_MBX_DEVICE_t* device, SDD_MBX_t mbx)
{
   IFX_int32_t       ret    = SDD_FW_ERR_NO_ERROR;
   IFX_uint16_t      value  = 0;

   down(&sdd_mbx_ien_protect);

   if (mbx == SDD_MBX_HOST_CB_DATA)
   {
      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      if (CHECK_BIT(value, SDD_MBX_HOST_INT1_O2_RDY))
      {
         value &= ~(1 << SDD_MBX_HOST_IEN1_O1_RDY);
      }
      else
      {
         value &= ~((1 << SDD_MBX_HOST_IEN1_O1_RDY) | (1 << SDD_MBX_HOST_IEN1_ERR));
      }

      /* Disable ERR and O1_RDY IRQ. */
      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Disable UFL and OFL IRQ. */
      value &= ~((1 << SDD_MBX_HOST_IEN2_O1_UFL) |
               (1 << SDD_MBX_HOST_IEN2_I1_OFL));

      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      TRACE("command mailbox disabled");
   }
   else if (mbx == SDD_MBX_HOST_EB_DATA)
   {
      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      if (CHECK_BIT(value, SDD_MBX_HOST_INT1_O1_RDY))
      {
         value &= ~(1 << SDD_MBX_HOST_IEN1_O2_RDY);
      }
      else
      {
         value &= ~((1 << SDD_MBX_HOST_IEN1_O2_RDY) |
                    (1 << SDD_MBX_HOST_IEN1_ERR));
      }

      /* Disable ERR and O1_RDY IRQ. */
      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN1, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN1 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Read the current IRQ configuration. */
      ret = sdd_mbx_reg_read_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on read from IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      /* Disable UFL and OFL IRQ. */
      value &= ~((1 << SDD_MBX_HOST_IEN2_O2_UFL) |
               (1 << SDD_MBX_HOST_IEN2_I2_OFL));

      ret = sdd_mbx_reg_write_word(device, SDD_MBX_HOST_IEN2, &value);
      if (ret != SDD_FW_ERR_NO_ERROR)
      {
         ERROR("error on write to IEN2 (%d): %d", mbx, ret);
         up(&sdd_mbx_ien_protect);
         return ret;
      }

      TRACE("event mailbox disabled");
   }

   up(&sdd_mbx_ien_protect);

   return SDD_FW_ERR_NO_ERROR;
}

