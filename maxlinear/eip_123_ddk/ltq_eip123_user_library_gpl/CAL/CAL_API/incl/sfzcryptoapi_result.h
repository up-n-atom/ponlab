/* sfzcryptoapi_result.h
 *
 * The Cryptographic Abstraction Layer API: Function Result Codes
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_RESULT_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_RESULT_H

/*----------------------------------------------------------------------------
 * SfzCryptoStatus
 */
typedef enum
{
    /* Success. */
    SFZCRYPTO_SUCCESS = 0,

    /* Not supported. */
    SFZCRYPTO_UNSUPPORTED,

    /* wrong use; not depending on configuration. */
    SFZCRYPTO_BAD_ARGUMENT,

    /* Current implementation does not have this feature. */
    SFZCRYPTO_FEATURE_NOT_AVAILABLE,

    /* sfzcrypto has not been initialized yet. */
    SFZCRYPTO_NOT_INITIALISED,

    /* sfzcrypto has already been initialized. */
    SFZCRYPTO_ALREADY_INITIALIZED,

    /* Invalid parameter. */
    SFZCRYPTO_INVALID_PARAMETER,

    /* Invalid key size. */
    SFZCRYPTO_INVALID_KEYSIZE,

    /* Invalid length. */
    SFZCRYPTO_INVALID_LENGTH,

    /* If invalid algorithm code is used. */
    SFZCRYPTO_INVALID_ALGORITHM,

    /* If invalid mode code is used. */
    SFZCRYPTO_INVALID_MODE,

    /* If the command was invalid. */
    SFZCRYPTO_INVALID_CMD,

    /* Unwrap error. */
    SFZCRYPTO_UNWRAP_ERROR,

    /* If (signature) verification failed. */
    SFZCRYPTO_VERIFY_FAILED,

    /* If signature generation failed. */
    SFZCRYPTO_SIG_GEN_FAILED,

    /* If signature was invalid. */
    SFZCRYPTO_INVALID_SIGNATURE,

    /* Signature check failed. */
    SFZCRYPTO_SIGNATURE_CHECK_FAILED,

    /* Data too short. */
    SFZCRYPTO_DATA_TOO_SHORT,

    /* Buffer supplied is too small for intended use. */
    SFZCRYPTO_BUFFER_TOO_SMALL,

    /* No memory. */
    SFZCRYPTO_NO_MEMORY,

    /* Operation failed. */
    SFZCRYPTO_OPERATION_FAILED,

    /* Internal error. */
    SFZCRYPTO_INTERNAL_ERROR
}
SfzCryptoStatus;

#endif /* Include Guard */

/* end of file sfzcryptoapi_result.h */
