/* ee_id_local.c
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

/* Global storage for current application id. */
static EE_GlobalApplicationId_t stored_application_id = {
    {
#ifdef EE_CFG_ID
        { EE_CFG_ID }
#else /* !EE_CFG_ID */
        { 0, }
#endif /* EE_CFG_ID */
    },
    {
#ifdef EE_CFG_APPLICATIONID
        { EE_CFG_APPLICATIONID }
#else /* !EE_CFG_APPLICATIONID */
        { 0, }
#endif /* EE_CFG_APPLICATIONID */
    }
};

/* And pointer to my global application identifier. */
const EE_GlobalApplicationId_t *
EE_GetGlobalApplicationId(void)
{
    return &stored_application_id;
}

void
EE_SetGlobalApplicationId(
        const EE_GlobalApplicationId_t * const   GlobalApplicationId_p)
{
    PRECONDITION(GlobalApplicationId_p != NULL);

    c_memcpy(&stored_application_id,
             GlobalApplicationId_p,
             sizeof(EE_GlobalApplicationId_t));
}

void
EE_SetApplicationId(
        const EE_ApplicationId_t * const   ApplicationId_p)
{
    PRECONDITION(ApplicationId_p != NULL);

    c_memcpy(&stored_application_id.ApplicationId,
             ApplicationId_p,
             sizeof(EE_ApplicationId_t));
}

/* end of file ee_id_local.c */
