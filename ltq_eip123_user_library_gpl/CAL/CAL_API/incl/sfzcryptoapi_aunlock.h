/* sfzcryptoapi_aunlock.h
 *
 * The Cryptographic Abstraction Layer API: Function Result Codes
 */

/*****************************************************************************
* Copyright (c) 2015 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_AUNLOCK_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_AUNLOCK_H

#include "public_defs.h"            // uint8_t, uint32_t, etc.

#include "sfzcryptoapi_asset.h"     // SfzCryptoAssetId type

SfzCryptoStatus
sfzcrypto_authenticated_unlock_start(
        const uint16_t AuthKeyNumber,
        SfzCryptoAssetId * AuthStateASId_p,
        uint8_t * Nonce_p,
        uint32_t * NonceLength_p);

SfzCryptoStatus
sfzcrypto_authenticated_unlock_verify(
        const SfzCryptoAssetId AuthStateASId,
        SfzCryptoBigInt * const Signature_p,
        const uint8_t * Nonce_p,
        const uint32_t NonceLength);

SfzCryptoStatus
sfzcrypto_secure_debug(
        const SfzCryptoAssetId AuthStateASId,
        const bool bSet);

#endif /* Include Guard */

/* end of file sfzcryptoapi_aunlock.h */
