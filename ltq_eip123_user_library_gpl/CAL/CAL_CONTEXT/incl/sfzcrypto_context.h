/* sfzcrypto_context.h
 *
 * Description: This file is a part of sfzcrypto, and provides declarations
 *              for services for maintaining sfzcrypto contexts.
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

#ifndef INCLUDE_GUARD_SFZCRYPTO_CONTEXT_H
#define INCLUDE_GUARD_SFZCRYPTO_CONTEXT_H

#include "sfzcryptoapi_init.h"  // SfzCryptoContext

/** Gets pointer to sfzcrypto context that may be passed to sfzcrypto_init
    or other sfzcrypto functions.

    There is no way to release this object. However, only single
    instance of the object is created.

    @return
    Pointer to SfzCryptoContext

 */
SfzCryptoContext *
sfzcrypto_context_get(void);

#endif /* Include Guard */

/* end of file sfzcrypto_context.h */
