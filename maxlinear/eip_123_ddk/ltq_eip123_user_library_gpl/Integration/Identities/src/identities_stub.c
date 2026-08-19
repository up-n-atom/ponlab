/* identities_stub.c
 *
 * Identities Module stub implementation that returns a constant.
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

#include "identities.h"     // the API to implement

static const uint32_t Identities_Stubbed_ShortID = 0x87654321;
static const uint8_t Identities_Stubbed_LongID[20] =
{
    'S', 't', 'u', 'b', 'b', 'e', 'd',
    'L', 'o', 'n', 'g',
    'I', 'd', 'e', 'n', 't', 'i', 't', 'y',
    '!'
};


/*----------------------------------------------------------------------------
 * Identities_ShortLivedID_Get
 */
int
Identities_ShortLivedID_Get(
        uint32_t * const ID_p)
{
    if (ID_p == NULL)
        return -1;

    *ID_p = Identities_Stubbed_ShortID;

    return 0;
}


/*----------------------------------------------------------------------------
 * Identities_LongLivedID_Get
 */
int
Identities_LongLivedID_Get(
        uint8_t * ID_p)
{
    if (ID_p == NULL)
        return -1;

    if (sizeof (Identities_Stubbed_LongID) != IDENTITIES_LONGLIVEDID_BYTECOUNT)
        return -2;

    memcpy(
        ID_p,
        Identities_Stubbed_LongID,
        IDENTITIES_LONGLIVEDID_BYTECOUNT);

    return 0;
}


/* end of file identities_stub.c */
