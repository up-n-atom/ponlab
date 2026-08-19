/* cs_cal_hw.h
 *
 * Configuration Settings for the CAL HW module.
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

// enable debug logging
//#define LOG_SEVERITY_MAX  LOG_SEVERITY_INFO

//#define CALHW_TRACE_TOKENS

// the crypto module mailbox pair to use (1..n)
#define CALHW_CM_MAILBOX_NR  3

// this switch selects Interrupts and Polling device interaction
#ifndef CFG_ENABLE_POLLING
#define CALHW_USE_INTERRUPTS
#endif

#ifdef CFG_ENABLE_CM_HW1
#define CAL_HW_CMv1
#endif

#ifdef CFG_ENABLE_CM_HW2
#define CAL_HW_CMv2
#endif

// disable PKA hardware module support when not needed
#ifndef CFG_ENABLE_PKA
#define CALHW_REMOVE_PKA_SUPPORT
#endif

// delay for polling mode; used while waiting for OUT token
#define CALHW_POLLING_DELAY_MS  1

// time limit to wait for a CM operation to complete
// depends on the performance and maximum data size
#define CALHW_CM_WAIT_LIMIT_MS   (30 * 1000)

// time limit to wait for a PKA operation to complete
// depends on the performance and maximum vector size used
#define CALHW_PKA_WAIT_LIMIT_MS  (30 * 1000)

// during initialization of the CM, the DMA RUNPARAMS register
// can be initialized with the value set here.
// See SafeXcel-IP-122/123 Hardware Reference Manual for details.
// this line can be commented-out when this parameters is correct after reset
#define CALHW_DMACONFIG_RUNPARAMS  0x00006800

// comment-out the following line when the TRNG is absent
#define CALHW_ENABLE_TRNGCONFIG

// during initialization of the CM, the TRNG is configured with
// the parameters shown below.
//
#ifdef CFG_ENABLE_CM_HW1
#define CALHW_TRNGCONFIG_STARTTIME      0
#define CALHW_TRNGCONFIG_TAPSETTING     1
#endif

#ifdef CFG_ENABLE_CM_HW2
// we want the TRNG start time to be 20ms (20/1000 = 50)
// the TRNG in the FPGA runs at 35MHz
// clock divider = 256 cycles
// 35MHz / 50 / 256 = 2734
#define CALHW_TRNGCONFIG_STARTTIME      2734
#define CALHW_TRNGCONFIG_MAXREFILLTIME  1
#define CALHW_TRNGCONFIG_MINREFILLTIME  1
#define CALHW_TRNGCONFIG_SAMPLEDIV      0
// after this amount of random bytes generated, the AES post-processor
// seed and key are automatically reloaded with new values
#define CALHW_TRNGCONFIG_AUTOSEED       0 /* 0=64kByte */
#endif /* !CFG_ENABLE_CM_HW12 */

/* end of file cs_cal_hw.h */
