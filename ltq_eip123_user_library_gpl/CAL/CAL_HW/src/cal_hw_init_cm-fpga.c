/* cal_hw_init_cm-fpga.c
 *
 * CAL_HW, (custom) HW initialization.
 *
 * This implementation is for the SafeXcel-IP-123-FPGA with soft-reset.
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "c_cal_hw.h"               // configuration

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "device_mgmt.h"
#include "device_rw.h"

#include "eip201.h"


#define FPGA_SW_RESET 0xFF0
#define FPGA_OPTIONS  0xFF8
#define FPGA_REVISION 0xFFC

//#define FPGA_EXPECTED_VERSION 0x0221847B      /* v2.2.1 */
#define FPGA_EXPECTED_VERSION 0x0222847B      /* v2.2.2 */


/*----------------------------------------------------------------------------
 * CALHW_Init_CheckFPGAVersion
 *
 * This function checks that the FPGA revision is supported.
 * Returns 0 on success or <0 on error.
 */
static int
CALHW_Init_CheckFPGAVersion(void)
{
    Device_Handle_t Device;

    Device = Device_Find("SZFPGA_CTRL");
    if (Device == NULL)
    {
        LOG_WARN(
            "CALHW_Init_CheckFPGAVersion: "
            "Failed to find device SZFPGA_CTRL\n");

        return -1;
    }

    // read and check the device ID
    {
        uint32_t Rev = Device_Read32(Device, FPGA_REVISION);

        if (Rev != FPGA_EXPECTED_VERSION)
        {
            LOG_WARN(
                "CALHW_Init_CheckFPGAVersion: "
                "Unexpected FPGA revision: 0x%08x\n",
                Rev);

            return -2;
        }
    }

    return 0;   // success
}


/*----------------------------------------------------------------------------
 * CALHW_Init_CheckPKA
 *
 * This function checks that the PKA is ready and the Firmware has been
 * downloaded already (or is in ROM).
 *
 * Returns 0 on success or <0 on error.
 */
#ifndef CALHW_REMOVE_PKA_SUPPORT

#define EIP150_REG_REVISION    0xBFFC
#define EIP150_REG_PKA_OPTIONS 0x40F4
#define EIP150_REG_PKA_SEQCTRL 0x40C8

static int
CALHW_Init_CheckPKA(void)
{
    Device_Handle_t Device;

    Device = Device_Find("EIP150");
    if (Device == NULL)
    {
        LOG_WARN(
            "CALHW_Init_CheckPKA: "
            "Failed to find device EIP150\n");

        return -1;
    }

    // check the EIP-150 revision
    {
        uint32_t v;

        v = Device_Read32(Device, EIP150_REG_REVISION);
        v &= 0xFFF0FFFF;
        if (v != 0x02106996)
        {
            LOG_WARN(
                "CALHW_Init_CheckPKA: "
                "Unexpected EIP150 revision: 0x%08x\n",
                v);

            return -2;
        }
    }

    // read the PKA OPTIONS
    {
        uint32_t v;

        v = Device_Read32(Device, EIP150_REG_PKA_OPTIONS);
        if (v & BIT_7)
        {
            // PKA has Program RAM
            // ensure it is not held in reset, waiting for firmware
            v = Device_Read32(Device, EIP150_REG_PKA_SEQCTRL);
            if (v & BIT_31)
            {
                LOG_WARN(
                    "CALHW_Init_CheckPKA: "
                    "PKA is in reset - has the firmware been loaded?\n");

                return -3;
            }
        }

        // toggle the reset bit in PKA_SEQ_CTRL
        Device_Write32(Device, EIP150_REG_PKA_SEQCTRL, BIT_31);
        Device_Write32(Device, EIP150_REG_PKA_SEQCTRL, 0);
    }

    return 0;   // success
}
#endif /* !CALHW_REMOVE_PKA_SUPPORT */


/*----------------------------------------------------------------------------
 * CALHW_Init_ResetCM
 *
 * This function resets the Crypto Module using the FPGA register to control
 * the SW_RESET feature. This must not be done in a real system, but ensures
 * a known state when testing with the FPGA.
 *
 * Returns 0 on success or <0 on error.
 */
static int
CALHW_Init_ResetCM(void)
{
    Device_Handle_t Device;

    Device = Device_Find("SZFPGA_CTRL");
    if (Device == NULL)
    {
        LOG_WARN(
            "CALHW_Init_ResetCM: "
            "Failed to find device SZFPGA_CTRL\n");

        return -1;
    }

#if 0 //!<WW : I don't believe it exist this soft reset.
    // EIP123-FGPA with "soft-reset" support

    // try to stop any ongoing DMA in a clean way using abort-request
    Device_Write32(Device, FPGA_SW_RESET, BIT_0);

    // wait for abort-ack, with loop-limiter
    {
        int Count = 100;
        uint32_t Ack;

        do
        {
            Ack = Device_Read32(Device, FPGA_SW_RESET) & BIT_1;
            Count--;
        }
        while (!Ack && Count > 0);

        if (!Ack)
        {
            LOG_WARN(
                "CALHW_Init_ResetCM: "
                "EIP-123 AbortReq: no ack\n");
        }
    }

    // now hard-reset EIP-123 by pulsing bit 2
    Device_Write32(Device, FPGA_SW_RESET, BIT_2);
    Device_Write32(Device, FPGA_SW_RESET, 0);
#endif

    return 0;   // success
}


/*----------------------------------------------------------------------------
 * CALHW_Init_Setup_FPGA_AIC
 *
 * Set up the top-level EIP-123-FPGA Interrupt Controller. This is where the
 * interrupts from all EIP-123 hosts and the EIP-150 PKA come together.
 *
 * Returns 0 on success or <0 on error.
 */
static int
CALHW_Init_Setup_FPGA_AIC(void)
{
#ifdef CALHW_USE_INTERRUPTS
    Device_Handle_t Device;
    EIP201_Status_t res201;

    static const EIP201_SourceSettings_t FPGA_AIC_Settings[] =
    {
#ifndef CALHW_REMOVE_PKA_SUPPORT
        { BIT_8, EIP201_CONFIG_ACTIVE_HIGH, true},  // EIP-150 PKA
#endif
        { 0xFF,  EIP201_CONFIG_ACTIVE_HIGH, true}   // EIP-123 host ID 7..0
    };

    Device = Device_Find("SZFPGA_AIC");
    if (Device == NULL)
    {
        LOG_WARN(
            "CALHW_Init_Setup_FPGA_AIC: "
            "Failed to find device SZFPGA_AIC\n");

        return -1;
    }

    res201 = EIP201_Initialize(
                    Device,
                    FPGA_AIC_Settings,
                    sizeof(FPGA_AIC_Settings) / sizeof(FPGA_AIC_Settings[0]));

    if (res201 != EIP201_STATUS_SUCCESS)
    {
        LOG_WARN(
            "CALHW_Init_Setup_FPGA_AIC: "
            "EIP201_Initialize returned %d\n",
            res201);

        return -2;
    }
#endif /* CALHW_USE_INTERRUPTS */
    return 0;
}


/*----------------------------------------------------------------------------
 * CAL_HW_ClockAndReset
 *
 * This function is called from CAL_HW_Init to initialize the hardware modules
 * into a known and usable state. A typical implementation should reset the HW
 * blocks. When this is not possible, a check should be made to ensure the HW
 * is not in a state that can trigger problems when the SW tries to use it.
 *
 * Return 0 for succes and <0 upon error.
 */
int
CAL_HW_ClockAndReset(void)
{
    if (CALHW_Init_CheckFPGAVersion() < 0)
        return -1;

#ifndef CALHW_REMOVE_PKA_SUPPORT
    if (CALHW_Init_CheckPKA() < 0)
        return -2;
#endif

    if (CALHW_Init_ResetCM() < 0)
        return -3;

    if (CALHW_Init_Setup_FPGA_AIC() < 0)
        return -4;

    return 0;       // 0 = success
}


/* end of file cal_hw_init_cm-fpga.c */
