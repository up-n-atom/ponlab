/* ee_id_cmp.c
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

/* Comparison functions for EE Identifiers. */

int
EE_Id_Cmp(
        const EE_Id_t * const EEId_1_p,
        const EE_Id_t * const EEId_2_p)
{
    PRECONDITION(EEId_1_p != NULL);
    PRECONDITION(EEId_2_p != NULL);

    return c_memcmp(EEId_1_p, EEId_2_p, sizeof(EE_Id_t));
}

int
EE_ApplicationId_Cmp(
        const EE_ApplicationId_t * const ApplicationId_1_p,
        const EE_ApplicationId_t * const ApplicationId_2_p)
{
    PRECONDITION(ApplicationId_1_p != NULL);
    PRECONDITION(ApplicationId_2_p != NULL);

    return c_memcmp(ApplicationId_1_p,
                    ApplicationId_2_p,
                    sizeof(EE_ApplicationId_t));
}

int
EE_GlobalApplicationId_Cmp(
        const EE_GlobalApplicationId_t * const GlobalApplicationId_1_p,
        const EE_GlobalApplicationId_t * const GlobalApplicationId_2_p)
{
    int cmpRes;
    PRECONDITION(GlobalApplicationId_1_p != NULL);
    PRECONDITION(GlobalApplicationId_2_p != NULL);

    cmpRes = EE_Id_Cmp(&GlobalApplicationId_1_p->EEId,
                       &GlobalApplicationId_2_p->EEId);

    if (cmpRes == 0)
    {
        /* When Execution Environment is the same, compare
           Application Ids as well. */

        cmpRes = EE_ApplicationId_Cmp(&GlobalApplicationId_1_p->ApplicationId,
                                      &GlobalApplicationId_2_p->ApplicationId);
    }

    return cmpRes;
}

bool
EE_Id_Eq(
        const EE_Id_t * const EEId_1_p,
        const EE_Id_t * const EEId_2_p)
{
    return EE_Id_Cmp(EEId_1_p, EEId_2_p) == 0;
}

bool
EE_ApplicationId_Eq(
        const EE_ApplicationId_t * const ApplicationId_1_p,
        const EE_ApplicationId_t * const ApplicationId_2_p)
{
    return EE_ApplicationId_Cmp(ApplicationId_1_p, ApplicationId_2_p) == 0;
}

bool
EE_GlobalApplicationId_Eq(
        const EE_GlobalApplicationId_t * const GlobalApplicationId_1_p,
        const EE_GlobalApplicationId_t * const GlobalApplicationId_2_p)
{
    return EE_GlobalApplicationId_Cmp(GlobalApplicationId_1_p,
                                      GlobalApplicationId_2_p) == 0;
}

/* end of file ee_id_cmp.c */
