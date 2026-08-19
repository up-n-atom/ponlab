/* cal_cm-v2_symm_crypto.c
 *
 * Implementation of the CAL API.
 *
 * This file implements the dispatcher for the Symmetric Crypto service,
 * calling the Crypto Module or SW implementations for AES, DES, ARC4 and
 * AES-f8.
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

#include "c_cal_cm-v2.h"

#include "basic_defs.h"
#include "clib.h"
#include "log.h"

#include "cal_cm.h"                 // the API to implement

#include "cal_cm-v2_internal.h"     // CAL_CM_AESDES, AESf8, ARC4
#include "cm_tokens_crypto.h"


/*----------------------------------------------------------------------------
 * sfzcrypto_cm_symm_crypt
 */
SfzCryptoStatus
sfzcrypto_cm_symm_crypt(
        SfzCryptoCipherContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction)
{
#ifdef CALCM_STRICT_ARGS
    if (p_dst_len == NULL ||
        p_ctxt == NULL ||
        p_key == NULL ||
        p_src == NULL)
    {
        return SFZCRYPTO_INVALID_PARAMETER;
    }

    if (src_len == 0)
        return SFZCRYPTO_BAD_ARGUMENT;

    if (direction != SFZ_ENCRYPT)
        if (direction != SFZ_DECRYPT)
            return SFZCRYPTO_BAD_ARGUMENT;
#endif /* CALCM_STRICT_ARGS */

#ifdef CALCM_TRACE_sfzcrypto_cm_symm_crypt
    if (p_key->type == SFZCRYPTO_KEY_AES &&
        p_ctxt->fbmode == SFZCRYPTO_MODE_F8)
    {
        Log_FormattedMessageINFO(
            "sfzcrypto_cm_symm_crypt: "
            "algo=%d "
            "fbmode=%d "
            "keylen=%d "
            "f8saltkeylen=%d "
            "src_len=%d "
            "dir=%d "
            "key_asset_id=0x%x "
            "iv_asset_id=0x%x "
            "iv_loc=%d\n",
            p_key->type,
            p_ctxt->fbmode,
            p_key->length,
            p_key->f8_salt_keyLen,
            src_len,
            direction,
            p_key->asset_id,
            p_ctxt->iv_asset_id,
            p_ctxt->iv_loc);
    }
    else
    {
        Log_FormattedMessageINFO(
            "sfzcrypto_cm_symm_crypt: "
            "algo=%d "
            "fbmode=%d "
            "keylen=%d "
            "src_len=%d "
            "dir=%d "
            "key_asset_id=0x%x "
            "iv_asset_id=0x%x "
            "iv_loc=%d\n",
            p_key->type,
            p_ctxt->fbmode,
            p_key->length,
            src_len,
            direction,
            p_key->asset_id,
            p_ctxt->iv_asset_id,
            p_ctxt->iv_loc);
    }
#endif /* CALCM_TRACE_sfzcrypto_cm_symm_crypt */

    // dispatch based on requested algorithm

#ifndef CALCM_REMOVE_ALGO_ARCFOUR
    if (p_key->type == SFZCRYPTO_KEY_ARCFOUR)
    {
        return CAL_CM_ARC4(
                        p_ctxt, p_key,
                        p_src, src_len,
                        p_dst, p_dst_len,
                        direction);
    }
#endif /* !CALCM_REMOVE_ALGO_ARCFOUR */

#ifndef CALCM_REMOVE_ALGO_CAMELLIA
    if (p_key->type == SFZCRYPTO_KEY_CAMELLIA)
    {
        return CAL_CM_CAMELLIA(
                        p_ctxt, p_key,
                        p_src, src_len,
                        p_dst, p_dst_len,
                        direction);
    }
#endif /* !CALCM_REMOVE_ALGO_CAMELLIA */

#ifndef CALCM_REMOVE_ALGO_C2
    if (p_key->type == SFZCRYPTO_KEY_C2)
    {
        return CAL_CM_C2(
                        p_ctxt, p_key,
                        p_src, src_len,
                        p_dst, p_dst_len,
                        direction);
    }
#endif /* !CALCM_REMOVE_ALGO_C2 */

#ifndef CALCM_REMOVE_ALGO_MULTI2
    if (p_key->type == SFZCRYPTO_KEY_MULTI2)
    {
        return CAL_CM_MULTI2(
                        p_ctxt, p_key,
                        p_src, src_len,
                        p_dst, p_dst_len,
                        direction);
    }
#endif /* !CALCM_REMOVE_ALGO_MULTI2 */

#ifndef CALCM_REMOVE_ALGO_AES_F8
    if (p_key->type == SFZCRYPTO_KEY_AES &&
        p_ctxt->fbmode == SFZCRYPTO_MODE_F8)
    {
        return CAL_CM_AESf8(
                        p_ctxt, p_key,
                        p_src, src_len,
                        p_dst, p_dst_len,
                        direction);
    }
#endif /* !CALCM_REMOVE_ALGO_AES_F8 */

    if (p_key->type == SFZCRYPTO_KEY_AES ||
        p_key->type == SFZCRYPTO_KEY_DES ||
        p_key->type == SFZCRYPTO_KEY_TRIPLE_DES)
    {
        return CAL_CM_AESDES(
                        p_ctxt, p_key,
                        p_src, src_len,
                        p_dst, p_dst_len,
                        direction);
    }

    return SFZCRYPTO_INVALID_ALGORITHM;
}


/* end of file cal_cm-v2_symm_crypto.c */
