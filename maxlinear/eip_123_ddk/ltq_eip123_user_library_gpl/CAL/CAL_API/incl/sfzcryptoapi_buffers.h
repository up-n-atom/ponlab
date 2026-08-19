/* sfzcryptoapi_buffers.h
 *
 * The Cryptographic Abstraction Layer API: Data Buffer handling.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_BUFFERS_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_BUFFERS_H

#include "public_defs.h"        // uint8_t

/*----------------------------------------------------------------------------
 * SfzCryptoOctet*Ptr
 *
 * The types can be used in pointer-parameter declarations to clarify the
 * direction of the parameter (input-only, output-only or input-output).
 * Use the _Optional variant when the pointer parameter may be NULL.
 */
typedef const uint8_t SfzCryptoOctetsIn;
typedef uint8_t SfzCryptoOctetsOut;
typedef uint8_t SfzCryptoOctetsInOut;

typedef SfzCryptoOctetsIn    SfzCryptoOctetsIn_Optional;
typedef SfzCryptoOctetsOut   SfzCryptoOctetsOut_Optional;
typedef SfzCryptoOctetsInOut SfzCryptoOctetsInOut_Optional;


/*----------------------------------------------------------------------------
 * SfzCryptoSize
 *
 * This type is large enough to hold the size of a buffer, or the length of
 * the data in the buffer. The maximum size for the entire API is defined by
 * SFZCRYPTO_SIZE_MAX.
 */

typedef size_t SfzCryptoSize;

#define SFZCRYPTO_SIZE_MAX         0x7FFFFFFF


#endif /* Include Guard */

/* end of file sfzcryptoapi_buffers.h */
