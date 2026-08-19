/* cal_cm_internal.h
 *
 * CAL_CM module internal interfaces and definitions.
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

#ifndef INCLUDE_GUARD_CAL_CM_INTERNAL_H
#define INCLUDE_GUARD_CAL_CM_INTERNAL_H

#include "cm_tokens_common.h"       // CMTokens_*
#include "cm_tokens_systeminfo.h"   // CMTokens_SystemInfo_t

#include "sfzcryptoapi.h"           // SfzCryptoStatus, SfzCryptoCipher*

int
CAL_CM_Init(void);

SfzCryptoStatus
CAL_CM_ExchangeToken(
        CMTokens_Command_t * const CommandToken_p,
        CMTokens_Response_t * const ResponseToken_p);


/* Symmetric Crypto */

/*----------------------------------------------------------------------------
 * sfzcrypto_symm_crypt
 */
SfzCryptoStatus
CAL_CM_AESDES(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

SfzCryptoStatus
CAL_CM_ARC4(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

SfzCryptoStatus
CAL_CM_AESf8(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

SfzCryptoStatus
CAL_CM_CAMELLIA(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

SfzCryptoStatus
CAL_CM_C2(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

SfzCryptoStatus
CM_CAL_C2_H(
        SfzCryptoCipherMacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
CAL_CM_MULTI2(
        SfzCryptoCipherContext * p_ctxt,
        SfzCryptoCipherKey * p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

int
CAL_CM_SysInfo_Get(
        CMTokens_SystemInfo_t * const SysInfo_p);

#endif /* Include Guard */

/* end of file cal_cm_internal.h */
