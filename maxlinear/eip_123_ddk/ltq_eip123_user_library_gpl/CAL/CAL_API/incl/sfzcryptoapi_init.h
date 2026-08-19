/* sfzcryptoapi_init.h
 *
 * The Cryptographic Abstraction Layer API: Initialization Functions
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_INIT_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_INIT_H

#include "public_defs.h"
#include "sfzcryptoapi_result.h"        // SfzCryptoStatus


/*----------------------------------------------------------------------------
 * SfzCryptoContext
 *
 * Crypto context.
 *
 * Applications MUST allocate an object of this type and pass its reference to
 * the sfzcrypto_xxx API function that require it.
 * There can be many objects of this type across the execution context of an
 * applications (process/thread/applet/etc.)
 *
 * Applications should use sfzcrypto_context_get() to acquire suitable buffer.
 */
typedef struct
{
    // Buffer that implementations can use to avoid calling malloc
    uint32_t misc_buf[1024];
}
SfzCryptoContext;


/*----------------------------------------------------------------------------
 * sfzcrypto_init
 *
 * Initialize the CAL implementation. Each application using the CAL API must
 * call this function before using any other CAL API function (sfzcrypto_*).
 *
 * If the implementation is already initialized, it will return the special
 * return value SFZCRYPTO_ALREADY_INITIALIZED. This is not an error.
 *
 * p_sfzcryptoctx
 *     Reference to a SfzCryptoContext returned by sfzcrypto_context_get.
 *
 * Return Value:
 *     SFZCRYPTO_SUCCESS or SFZCRYPTO_ALREADY_INITIALIZED inidicate successful
 *     initialization.
 *     One of the return values (see SfzCryptoStatus definition) indicate an
 *     error situation.
 */
SfzCryptoStatus
sfzcrypto_init(
        SfzCryptoContext * const p_sfzcryptoctx);


#endif /* Include Guard */

/* end of file sfzcryptoapi_init.h */
