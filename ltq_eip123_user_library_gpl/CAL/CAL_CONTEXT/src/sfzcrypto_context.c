/* sfzcrypto_context.c
 *
 * This file provides the SfzCryptoContext memory. It can be customized for
 * the concurrent-use needs of the application. The default implementation
 * provides one context.
 */

/*****************************************************************************
* Copyright (c) 2007-2015 INSIDE Secure B.V. All Rights Reserved.
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

#include "implementation_defs.h"       // ASSERT
#include "c_lib.h"                     // c_memset
#include "spal_memory.h"               // SPAL_Memory_Alloc
#include "sfzcryptoapi_init.h"         // SfzCryptoContext
#include "sfzcrypto_context.h"         // API to implement

/* Helper function for acquiring crypto context. */
SfzCryptoContext *
sfzcrypto_context_get(void)
{
    static uint32_t DummyContext;            /* Note: static! */
    static SfzCryptoContext * ctx_p = NULL;  /* Note: static! */

    if (ctx_p == NULL)
    {
        // since context is not used by the implementation
        // return some non-NULL pointer
        ctx_p = (SfzCryptoContext *)&DummyContext;

        // alternative implementation for implementations that need the context
        //ctx_p = SPAL_Memory_Alloc(sizeof(SfzCryptoContext));
        //ASSERT(ctx_p != NULL);
        //c_memset(ctx_p, 0, sizeof(SfzCryptoContext));
    }

    return ctx_p;
}

/* end of file sfzcrypto_context.c */
