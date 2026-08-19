/* identities.h
 *
 * Identities Module API.
 *
 * This module is the central customization point for the identities used
 * by the CAL TRUST module.
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

#ifndef INCLUDE_GUARD_IDENTITIES_H
#define INCLUDE_GUARD_IDENTITIES_H

#include "basic_defs.h"     // uint32_t

/*----------------------------------------------------------------------------
 * Identities_ShortLivedID_Get
 *
 * This functions returns the 32bit identity that is used to keep requests to
 * the Crypto Module separated for each execution context, typically for each
 * application and service. This identity typically changes every time an
 * application starts.
 *
 * ID_p (output parameter)
 *     Pointer to the memory location where the 32bit identity will be
 *     written by the implementation.
 *
 * Return Value:
 *     0    Success
 *    <0    Error
 *    >0    Reserved
 */
int
Identities_ShortLivedID_Get(
        uint32_t * const ID_p);


#define IDENTITIES_LONGLIVEDID_BYTECOUNT  20

/*----------------------------------------------------------------------------
 * Identities_LongLivedID_Get
 *
 * This functions returns the 20-byte identity that is used to keep
 * cryptographic secrets (like key material) separated between applications or
 * groups of applications. This identity must remain the same for each reboot.
 * Applications with the same long-lived identity can potentially access each
 * other's cryptographic secrets.
 *
 * ID_p (output parameter)
 *     Pointer to the memory location where the 20-byte identity will be
 *     written by the implementation.
 *
 * Return Value:
 *     0    Success
 *    <0    Error
 *    >0    Reserved
 */
int
Identities_LongLivedID_Get(
        uint8_t * ID_p);


#endif /* Include Guard */

/* end of file identities.h */
