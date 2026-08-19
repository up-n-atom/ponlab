/* cs_cal_cm-v2.h
 *
 * Configuration Settings for the CAL_CM module.
 */

/*****************************************************************************
* Copyright (c) 2007-2013 INSIDE Secure B.V. All Rights Reserved.
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
//#define LOG_SEVERITY_MAX  LOG_SEVERITY_CRIT

// Set this option to enable strict argument checking
#define CALCM_STRICT_ARGS

// delay for polling mode
// also used while polling for TokenID to arrive
//#define CALCM_POLLING_DELAY_MS  10
#define CALCM_POLLING_DELAY_MS  1

// time limit to wait for a resource lock, to avoid dead-locks
//#define CALCM_WAIT_LIMIT_MS  500000

// (Minimum) required alignment of DMA buffers (due to hardware)
#define CALCM_DMA_ALIGNMENT   1
// bank number provided to DMAResource_Alloc
#define CALCM_DMA_BANK        0

// when defined, CAL will always bounce the buffers provided via the CAL API
// into DMA_safe buffers.
// when undefined, CAL will try to register the buffer provided by the caller
// using DMAResource_CheckAndRegister. When that fails, the buffer will be
// bounced after all.
//#define CALCM_DMA_BOUNCE_ALWAYS

// CAL API call trace options
//#define CALCM_TRACE_sfzcrypto_cm_hash_data
//#define CALCM_TRACE_sfzcrypto_cm_hmac_data
//#define CALCM_TRACE_sfzcrypto_cm_symm_crypt
//#define CALCM_TRACE_sfzcrypto_cipher_mac_data
//#define CALCM_TRACE_sfzcrypto_cm_cprm_c2_derive

//#define CALCM_TRACE_TOKENS
//#define CALCM_TRACE_ASSETSTORE

/* feature removal switches */
//#define CALCM_REMOVE_ALGO_AES_F8
//#define CALCM_REMOVE_ALGO_ARCFOUR
//#define CALCM_REMOVE_ALGO_MD5
//#define CALCM_REMOVE_ALGO_SHA160
//#define CALCM_REMOVE_ALGO_DES_3DES
//#define CALCM_REMOVE_ALGO_ARCFOUR
//#define CALCM_REMOVE_ALGO_CAMELLIA
//#define CALCM_REMOVE_ALGO_C2
//#define CALCM_REMOVE_ALGO_MULTI2

// Static number used to identify the root key (typically in NVM)
#define CALCM_ROOT_KEY_INDEX    1

/*
** LANTIQ Specific
** !<WW: <w.widjaja.ee@lantiq.com> (30/Sept/14)
** this temp fix include :
** - remove writing token ID at the end of Output ( specific API only , mostly Crypto enc/dec AES,TDES etc)
**    I disabled this since it use seperate DMA buff fragment just for it ( this mean scater gather must be enabled )
**    You could append within same output buffer as example in CALAdapter_RandomWrapNvm_PrepareOutput)
**    for in place token ID check
** - the scater gather mode is not really disabled ( but writing to lli DMA linked list in token input is removed )
**    The consequence is you will use Contiguous Transfer , and limit yourself to 2MB Block for using the API )
**
** NOTE : LIMIT YOURSELF TO USE THIS API WITH 2MB INPUT, AND PREPARE 2MB BLOCKS IF U NEED MORE,
**
*/
#define LTQ_EIP123_TMP_HACK_CRYPTO_NOTOKENIDCHK

/* end of file cs_cal_cm-v2.h */
