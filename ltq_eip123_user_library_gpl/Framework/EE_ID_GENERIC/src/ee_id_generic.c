/* ee_id_generic.c
 *
 * Description: Definitions for Execution Environment identification
 *              and EE Application identification objects routines.
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

#include "implementation_defs.h"
#include "ee_id.h"
#include "c_lib.h"

/* Generic functions for working with EE identifiers.
   These require implementation of EE_GetGlobalApplicationId(),
   for example from ee_id_local.c.
 */

/* Pointer to current EE identifier. */
const EE_Id_t *
EE_GetId(void)
{
    return &(EE_GetGlobalApplicationId()->EEId);
}


/* Pointer to my application identifier. */
const EE_ApplicationId_t *
EE_GetApplicationId(void)
{
    return &(EE_GetGlobalApplicationId()->ApplicationId);
}

/* Building Global Application ID out of EE Id and Application Id. */
void
EE_GlobalApplicationId_Build(
        EE_GlobalApplicationId_t * const    GlobalApplicationId_p,
        const EE_Id_t * const               EEId_p,
        const EE_ApplicationId_t * const    ApplicationId_p)
{
    PRECONDITION(GlobalApplicationId_p != NULL);
    PRECONDITION(EEId_p != NULL);
    PRECONDITION(ApplicationId_p != NULL);

    c_memcpy(&GlobalApplicationId_p->EEId,
             EEId_p,
             sizeof(EE_Id_t));

    c_memcpy(&GlobalApplicationId_p->ApplicationId,
             ApplicationId_p,
             sizeof(EE_ApplicationId_t));
}

bool
EE_GlobalApplicationId_Encode(
        const EE_GlobalApplicationId_t * const GlobalApplicationId_p,
        uint8_t * const                        EncodedGlobalApplicationId_p,
        uint32_t * const                       EncodedGlobalApplicationIdLen_p)
{
    uint32_t origLen;
    PRECONDITION(GlobalApplicationId_p != NULL);
    PRECONDITION(EncodedGlobalApplicationId_p != NULL);
    PRECONDITION(EncodedGlobalApplicationIdLen_p != NULL);

    origLen = *EncodedGlobalApplicationIdLen_p;
    *EncodedGlobalApplicationIdLen_p = (EE_ID_SIZE) +
        (EE_APPLICATION_ID_SIZE);

    if (origLen < (EE_ID_SIZE) + (EE_APPLICATION_ID_SIZE))
    {
        /* Not enough storage for identifier. */
        return false;
    }

    c_memcpy(EncodedGlobalApplicationId_p,
             GlobalApplicationId_p->EEId.SpaceForEEId,
             EE_ID_SIZE);

    c_memcpy(EncodedGlobalApplicationId_p + EE_ID_SIZE,
             GlobalApplicationId_p->ApplicationId.SpaceForApplicationId,
             EE_APPLICATION_ID_SIZE);

    return true;
}


bool
EE_GlobalApplicationId_Decode(
        EE_GlobalApplicationId_t * const       GlobalApplicationId_p,
        const uint8_t * const                  EncodedGlobalApplicationId_p,
        const uint32_t                         EncodedGlobalApplicationIdLen)
{
    PRECONDITION(GlobalApplicationId_p != NULL);
    PRECONDITION(EncodedGlobalApplicationId_p != NULL);

    if (EncodedGlobalApplicationIdLen != (EE_ID_SIZE) +
                                         (EE_APPLICATION_ID_SIZE))
    {
        return false;
    }

    c_memcpy(GlobalApplicationId_p->EEId.SpaceForEEId,
             EncodedGlobalApplicationId_p,
             EE_ID_SIZE);

    c_memcpy(GlobalApplicationId_p->ApplicationId.SpaceForApplicationId,
             EncodedGlobalApplicationId_p + EE_ID_SIZE,
             EE_APPLICATION_ID_SIZE);

    return true;
}

/* end of file ee_id_generic.c */
