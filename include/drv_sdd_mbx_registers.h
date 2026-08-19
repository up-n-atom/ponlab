#ifndef _DRV_SDD_MBX_REGISTERS_H
#define _DRV_SDD_MBX_REGISTERS_H
/******************************************************************************

                            Copyright (c) 2014-2015
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/**
   \file drv_sdd_mbx_registers.h
   This file contains register description.
 */

#include "drv_sdd_mbx_handler.h"
#include "ifxos_time.h"
#ifdef EVENT_LOGGER_DEBUG
#include "el_log_macros.h"
#endif /* EVENT_LOGGER_DEBUG */

/* ========================================================================== */
/* HOST Register offsets                                                      */
/* ========================================================================== */
/* Port Base Address */
#define SDD_MBX_HOST_BASE           (0x0000)
/* Configuration Register */
#define SDD_MBX_HOST_CFG            (SDD_MBX_HOST_BASE + 0x02)
/* Interrupt Enable Register 1 */
#define SDD_MBX_HOST_IEN1           (SDD_MBX_HOST_BASE + 0x03)
/* Interrupt Enable Register 2 */
#define SDD_MBX_HOST_IEN2           (SDD_MBX_HOST_BASE + 0x04)
/* Status 1 Register */
#define SDD_MBX_HOST_STAT1          (SDD_MBX_HOST_BASE + 0x05)
/* Status 2 Register */
#define SDD_MBX_HOST_STAT2          (SDD_MBX_HOST_BASE + 0x06)
/* Interrupt Register 1 */
#define SDD_MBX_HOST_INT1           (SDD_MBX_HOST_BASE + 0x07)
/* Interrupt Register 2 */
#define SDD_MBX_HOST_INT2           (SDD_MBX_HOST_BASE + 0x08)
/* DATA 1 Register */
#define SDD_MBX_HOST_CB_DATA        (SDD_MBX_HOST_BASE + 0x09)
/* DATA 2 Register */
#define SDD_MBX_HOST_EB_DATA        (SDD_MBX_HOST_BASE + 0x0A)
/* FIFO 1 INBOX Length Register */
#define SDD_MBX_HOST_CB_LEN_IN      (SDD_MBX_HOST_BASE + 0x0B)
/* FIFO 1 OUTBOX Length Register */
#define SDD_MBX_HOST_CB_LEN_OUT     (SDD_MBX_HOST_BASE + 0x0C)
/* FIFO 2 INBOX Length Register */
#define SDD_MBX_HOST_EB_LEN_IN      (SDD_MBX_HOST_BASE + 0x0D)
/* FIFO 2 OUTBOX Length Register */
#define SDD_MBX_HOST_EB_LEN_OUT     (SDD_MBX_HOST_BASE + 0x0E)
/* FIFO 1 INBOX Limit Register */
#define SDD_MBX_HOST_LIMIT1I        (SDD_MBX_HOST_BASE + 0x14)
/* FIFO 1 OUTBOX Limit Register */
#define SDD_MBX_HOST_LIMIT1O        (SDD_MBX_HOST_BASE + 0x15)
/* FIFO 2 INBOX Limit Register */
#define SDD_MBX_HOST_LIMIT2I        (SDD_MBX_HOST_BASE + 0x16)
/* FIFO 2 OUTBOX Limit Register */
#define SDD_MBX_HOST_LIMIT2O        (SDD_MBX_HOST_BASE + 0x17)
/* Command Register */
#define SDD_MBX_HOST_CMD            (SDD_MBX_HOST_BASE + 0x1A)
/* Boot Mode register */
#define SDD_MBX_HOST_REGB           (SDD_MBX_HOST_BASE + 0x1F)
/* General Purpose Dual-Port Register 2 */
#define SDD_MBX_HOST_REG2           (SDD_MBX_HOST_BASE + 0x22)
/* General Purpose Dual-Port Register 4 */
#define SDD_MBX_HOST_REG4           (SDD_MBX_HOST_BASE + 0x24)

/* ========================================================================== */
/* HOST Configuration Register                               SDD_MBX_HOST_CFG */
/* ========================================================================== */
/* Reset high */
#define SDD_MBX_HOST_CFG_RSTH            15
/* RDYQ Delay Mode (PIF) high */
#define SDD_MBX_HOST_CFG_RDYQ_DLYH       14
/* RDYQ Mode (PIF) high */
#define SDD_MBX_HOST_CFG_RDYQ_MDH        13
/* Endian Mode (PIF) high */
#define SDD_MBX_HOST_CFG_END_MDH         12
/* Transfer-Order Mode (PIF 8bit) high */
#define SDD_MBX_HOST_CFG_TO_MDH          11
/* Self-Clearing Mode high */
#define SDD_MBX_HOST_CFG_SC_MDH           9
/* Interrupt Line Polarity high */
#define SDD_MBX_HOST_CFG_INT_MDH          8
/* Reset */
#define SDD_MBX_HOST_CFG_RST              7
/* RDYQ Delay Mode (PIF) */
#define SDD_MBX_HOST_CFG_RDYQ_DLY         6
/* RDYQ Mode (PIF) */
#define SDD_MBX_HOST_CFG_RDYQ_MD          5
/* Endian Mode (PIF) */
#define SDD_MBX_HOST_CFG_END_MD           4
/* Transfer-Order Mode (PIF 8bit) */
#define SDD_MBX_HOST_CFG_TO_MD            3
/* Self-Clearing Mode */
#define SDD_MBX_HOST_CFG_SC_MD            1
/* Interrupt Line Polarity */
#define SDD_MBX_HOST_CFG_INT_MD           0

/* ========================================================================== */
/* HOST Interrupt Enable Register 1                         SDD_MBX_HOST_IEN1 */
/* ========================================================================== */
/* OUT-Box 2 Ready Interrupt Mask */
#define SDD_MBX_HOST_IEN1_O2_RDY         15
/* OUT-Box 2 Data Interrupt Mask */
#define SDD_MBX_HOST_IEN1_O2_DATA        14
/* IN-Box 2 Data Interrupt Mask */
#define SDD_MBX_HOST_IEN1_I2_EMP         13
/* IN-Box 2 Ready Interrupt Mask */
#define SDD_MBX_HOST_IEN1_I2_RDY         12
/* OUT-Box 1 Ready Interrupt Mask */
#define SDD_MBX_HOST_IEN1_O1_RDY         11
/* OUT-Box 1 Data Interrupt Mask */
#define SDD_MBX_HOST_IEN1_O1_DATA        10
/* IN-Box 1 Data Interrupt Mask */
#define SDD_MBX_HOST_IEN1_I1_EMP          9
/* IN-Box 1 Ready Interrupt Mask */
#define SDD_MBX_HOST_IEN1_I1_RDY          8
/* Error Interrupt Mask */
#define SDD_MBX_HOST_IEN1_ERR             0

/* ========================================================================== */
/* HOST Interrupt Enable Register 2                         SDD_MBX_HOST_IEN2 */
/* ========================================================================== */
/* Out-Box 2 Underflow */
#define SDD_MBX_HOST_IEN2_O2_UFL         11
/* In-Box 2 Overflow */
#define SDD_MBX_HOST_IEN2_I2_OFL         10
/* Out-Box 1 Underflow */
#define SDD_MBX_HOST_IEN2_O1_UFL          9
/* In-Box 1 Overflow */
#define SDD_MBX_HOST_IEN2_I1_OFL          8

/* ========================================================================== */
/* HOST Interrupt Register 1                                SDD_MBX_HOST_INT1 */
/* ========================================================================== */
/* Out-Box 2 Ready Interrupt */
#define SDD_MBX_HOST_INT1_O2_RDY         15
/* Out-Box 2 Data Interrupt */
#define SDD_MBX_HOST_INT1_O2_DATA        14
/* In-Box 2 Empty Interrupt */
#define SDD_MBX_HOST_INT1_I2_EMP         13
/* In-Box 2 Ready Interrupt */
#define SDD_MBX_HOST_INT1_I2_RDY         12
/* Out-Box 1 Ready Interrupt */
#define SDD_MBX_HOST_INT1_O1_RDY         11
/* Out-Box 1 Data Interrupt */
#define SDD_MBX_HOST_INT1_O1_DATA        10
/* In-Box 1 Empty Interrupt */
#define SDD_MBX_HOST_INT1_I1_EMP          9
/* In-Box 1 Ready Interrupt */
#define SDD_MBX_HOST_INT1_I1_RDY          8
/* Error Interrupt Pending */
#define SDD_MBX_HOST_INT1_ERR             0

/* ========================================================================== */
/* HOST Interrupt Register 2                                SDD_MBX_HOST_INT2 */
/* ========================================================================== */
/* Out-Box 2 Underflow Interrupt */
#define SDD_MBX_HOST_INT2_O2_UFL         11
/* In-Box 2 Overflow Interrupt */
#define SDD_MBX_HOST_INT2_I2_OFL         10
/* Out-Box 1 Underflow Interrupt */
#define SDD_MBX_HOST_INT2_O1_UFL          9
/* In-Box 1 Overflow Interrupt */
#define SDD_MBX_HOST_INT2_I1_OFL          8

/* ========================================================================== */
/* HOST Boot Mode register                                  SDD_MBX_HOST_REGB */
/* ========================================================================== */
/* Direct jump to ROM */
#define SDD_MBX_HOST_REGB_BOOT_ROM     0x00
/* Boot from UART (binary mode with baudrate detection) */
#define SDD_MBX_HOST_REGB_BOOT_UART    0x01
/* Boot from host */
#define SDD_MBX_HOST_REGB_BOOT_SPI     0x03
/* Boot Mode Freeze */
#define SDD_MBX_HOST_REGB_BOOT_FREEZE  0x06

/* ========================================================================== */
/* HOST General Purpose Dual-Port Register 2                SDD_MBX_HOST_REG2 */
/* ========================================================================== */
#define SDD_MBX_HOST_REG2_BOOTSTATE    0x40

/* ========================================================================== */
/*                            Macro definitions                               */
/* ========================================================================== */
/* Event logger type */
#define DEV_TYPE_SDD                          14
/* Number of retries to download firmware */
#define SDD_MBX_FW_DOWNLOAD_RETRY_MAX        3

/* ========================================================================== */
/*                           Function prototypes                              */
/* ========================================================================== */
/**
   This function reads from VCODEC register.

   \param pDev    Pointer to device structure.
   \param offset  Register offset
   \param pbuf    Pointer to (16-bit word) buffer where to store the
                  read data. The buffer must be at least "len" bytes long.
   \param len     Number of bytes to read and store in the buffer.

   \return IFX_int32_t error code
*/
extern IFX_int32_t sdd_mbx_reg_read(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf,
                                 IFX_uint32_t len);

/**
   This function reads one word from VCODEC register.

   \param pDev    Pointer to device structure.
   \param offset  Register offset
   \param pbuf    Pointer to (16-bit word) buffer where to store the
                  read data. The buffer must be at least "len" bytes long.

   \return IFX_int32_t error code
*/
extern IFX_int32_t sdd_mbx_reg_read_word(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf);
/**
   This function writes to VCODEC register.

   \param pDev    Pointer to device structure.
   \param offset  Register offset
   \param pbuf    Pointer to (16-bit word) buffer with data to write.
                  The buffer must be at least "len" bytes long.
   \param len     Number of bytes to write.

   \return IFX_int32_t error code
*/
extern IFX_int32_t sdd_mbx_reg_write(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf,
                                 IFX_uint32_t len);
/**
   This function writes one word to VCODEC register.

   \param pDev    Pointer to device structure.
   \param offset  Register offset
   \param pbuf    Pointer to (16-bit word) buffer with data to write.
                  The buffer must be at least "len" bytes long.

   \return IFX_int32_t error code
*/
extern IFX_int32_t sdd_mbx_reg_write_word(SDD_MBX_DEVICE_t *pDev, IFX_uint8_t offset,
                                 IFX_uint16_t *pbuf);

#ifdef EVENT_LOGGER_DEBUG
/* To make the EventLogger happy which uses a no longer existing IFXOS API. */
#define IFXOS_GET_TICK()                  IFXOS_ElapsedTimeMSecGet(0)
#endif /* EVENT_LOGGER_DEBUG */
#endif /* _DRV_SDD_MBX_REGISTERS_H */
