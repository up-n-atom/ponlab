/* identities_ee_id.c
 *
 * Identities Module implementation that uses EE_ID.
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

#include "basic_defs.h"     // uint32_t
#include "clib.h"           // memset, memcpy

#include "identities.h"     // the API to implement
#include "clib.h"

#include "spal_thread.h"
#include "ee_id.h"

static uint32_t Identities_AppId = 0;

/*----------------------------------------------------------------------------
 * Identities_ShortLivedID_Get
 *
 * This function takes the identity of the first thread and stores it.
 * The same identity is then returned, regardless of the thread calling this
 * function.
 */
int
Identities_ShortLivedID_Get(
        uint32_t * const ID_p)
{
    if (ID_p == NULL)
        return -1;

    if (Identities_AppId == 0)
    {
        Identities_AppId = (uint32_t)SPAL_Thread_Self();
    }

    *ID_p = Identities_AppId;

    return 0;
}


/*----------------------------------------------------------------------------
 * Identities_LongLivedID_Get
 */
int
Identities_LongLivedID_Get(
        uint8_t * ID_p)
{
    const EE_GlobalApplicationId_t * EE_ID_p;

    if (ID_p == NULL)
        return -1;

    memset(
        ID_p,
        0,
        IDENTITIES_LONGLIVEDID_BYTECOUNT);

    if (EE_ID_SIZE + EE_APPLICATION_ID_SIZE > IDENTITIES_LONGLIVEDID_BYTECOUNT)
        return -2;

    EE_ID_p = EE_GetGlobalApplicationId();
    if (EE_ID_p == NULL)
        return -3;

    memcpy(
        ID_p,
        &EE_ID_p->EEId,
        EE_ID_SIZE);

    ID_p += EE_ID_SIZE;

    memcpy(
        ID_p,
        &EE_ID_p->ApplicationId,
        EE_APPLICATION_ID_SIZE);

    return 0;
}


/* end of file identities_ee_id.c */
