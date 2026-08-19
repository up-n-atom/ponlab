/**
*  Definitions for Execution Environment identification and
*  EE Application identification objects.
*
*  File: ee_id.h
*
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
*/



#ifndef INCLUDE_GUARD_EE_ID_H
#define INCLUDE_GUARD_EE_ID_H

#include "public_defs.h"

/* Define object sizes used by EE and application identification objects. */

/** The Execution Environment identifier size.
    For typical embedded devices a single byte is enough
    (up to 256 distinct execution environments). */
#define EE_ID_SIZE 1

/** The Execution Environment Application identifier size.
    Identifier for application within an execution environment.

    16 byte long identifiers allow UUIDs used as application identifiers
    and therefore prevent accidental identifier collisions. */
#define EE_APPLICATION_ID_SIZE 16

/** Define a constant that tells how much space is needed by the
    Global EE Application identifier in encoded format.

    This is sum of sizes of Execution Environment and Application Identifier.
    */
#define GLOBAL_APPLICATION_ID_ENCODED_SIZE \
    ((EE_ID_SIZE) + (EE_APPLICATION_ID_SIZE))

/*---------------------------------------------------------------------
  Define the objects as structures.

  Note: Although you can use these objects within your code,
  you must never access the members inside the structures directly.
 */

/** Struct to store TEE (or EE) identifier (eg. the identifier byte).

    Note: You must never access this struct directly. However, you may
    take advantage of struct size for local/stack memory allocation.
    */
typedef struct EE_Id
{
    uint8_t SpaceForEEId[(EE_ID_SIZE)];
}
EE_Id_t;

/** Struct to store application ID (currently UUID).

    Note: You must never access this struct directly. However, you may
    take advantage of struct size for local/stack memory allocation.
    */
typedef struct EE_ApplicationId
{
    uint8_t SpaceForApplicationId[(EE_APPLICATION_ID_SIZE)];
}
EE_ApplicationId_t;

/** Struct to store the global application ID
    (the EE ID and Application ID pair).
*/
typedef struct EE_GlobalApplicationId
{
    EE_Id_t EEId;
    EE_ApplicationId_t ApplicationId;
}
EE_GlobalApplicationId_t;

/*---------------------------------------------------------------------
  Functions to use with Application and EE identifiers.
 */

/**
 *
 *  Retrieve pointer to the current Execution Environment identifier.
 *  Note: This function always succeeds.
 *
 *  @return
 *  Pointer to the current EE identifier.
 */
const EE_Id_t *
EE_GetId(void);

/**
 *
 *  Retrieve the pointer to the current Application identifier.
 *  Note: This function always succeeds.
 *
 *  @return
 *  Pointer to the current Application identifier.
 */
const EE_ApplicationId_t *
EE_GetApplicationId(void);

/**
 *
 *  Retrieve pointer to the current Global Application identifier.
 *  The result is combination of current Execution Environment and
 *  application identifiers.
 *  Note: this function always succeeds.
 *
 *  @return
 *  Pointer to the current Global Application identifier.
 */
const EE_GlobalApplicationId_t *
EE_GetGlobalApplicationId(void);

/** Building Global Application ID out of EE Id and Application Id.
 *
 * @param GlobalApplicationId_p
 * [out] Global Application Id shall be built here.
 * @param EEId_p
 * [in] Pointer to Execution Environment Identifier.
 * @param ApplicationId_p
 * [in] Pointer to Application Identifier.
 */
void
EE_GlobalApplicationId_Build(
        EE_GlobalApplicationId_t * const         GlobalApplicationId_p,
        const EE_Id_t * const                    EEId_p,
        const EE_ApplicationId_t * const         ApplicationId_p);

/** Express GlobalApplicationId as opaque data.
 *  The GlobalApplicationId is typically expressed as a structure while its
 *  kept in memory. Encoding it transforms it into a byte array that is at
 *  most as large as memory presentation of EE_GlobalApplicationId_t
 *  (exact size required is provided by constant
 *  GLOBAL_APPLICATION_ID_ENCODED_SIZE). The byte array can be stored unto
 *  storage and later transformed back into EE_GlobalApplicationId.
 *
 * @param GlobalApplicationId_p
 * [in] Global application identifier tom encode shall be provided
 * in this parameter.
 * @param EncodedGlobalApplicationId_p
 * [out] Pointer to byte array to receive Encoded global
 * application identifier.
 * @param EncodedGlobalApplicationIdLen_p
 * [in, out] Amount of space available in EncodedGlobalApplicationId_p,
 * amount of space used or needed in EncodedGlobalApplicationId_p.
 * @return
 * True if encoding was successful (enough space was provided).
 */
bool
EE_GlobalApplicationId_Encode(
        const EE_GlobalApplicationId_t * const GlobalApplicationId_p,
        uint8_t * const                        EncodedGlobalApplicationId_p,
        uint32_t * const                       EncodedGlobalApplicationIdLen_p);

/** Decode GlobalApplicationId from opaque data.
 *  Reverse effect of EE_GlobalApplicationId_Encode.
 *
 * @param GlobalApplicationId_p
 * [out] EE_GlobalApplicationId_t pointer to receive the decoded
 * global application identifier shall be provided in this parameter.
 * @param EncodedGlobalApplicationId_p
 * [in] Pointer to byte array to receive Encoded global
 * application identifier.
 * @param EncodedGlobalApplicationIdLen
 * [in] Amount of data provided in EncodedGlobalApplicationId_p.
 * @return
 * True if decoding was successful (the correct length was provided).
*/
bool
EE_GlobalApplicationId_Decode(
        EE_GlobalApplicationId_t * const       GlobalApplicationId_p,
        const uint8_t * const                  EncodedGlobalApplicationId_p,
        const uint32_t                         EncodedGlobalApplicationIdLen);

/** Set Application identification information.

    In trusted environments, this Id/ApplicationId information is
    typically retrieved from application metadata that is protected
    with a signature, and therefore the data is immutable.

    Note: This call can only be used in test software etc. and should only
    be used in the beginning of the program.

   */
void
EE_SetGlobalApplicationId(
        const EE_GlobalApplicationId_t * const   GlobalApplicationId_p);

/** Set only the local application Id, and leave EE Identification
 *  as it is.
 */
void
EE_SetApplicationId(
        const EE_ApplicationId_t * const         ApplicationId_p);

/* Comparison functions. */

/** Compare two EE identifiers.

    The definition of the return values of this function is equivalent
    to memcmp. However, as this function may not perform direct memory
    comparison, the results may be something different than a straight
    memcmp over the structs. At the very least this function will
    ignore any padding within the structures.

    Note: In the future, this function is likely to be inlined and/or
    macroized for smaller codesize. Take suitable precautions for
    macrolike side-effects for any parameters you pass to this
    function.

    @param EEId_1_p [in] First execution environment identifier.

    @param EEId_2_p [in] Second execution environment identifier.

    @return
    This function returns smaller than zero if the first identifier
    can be considered to be of smaller order than the second.
*/
int
EE_Id_Cmp(
        const EE_Id_t * const                    EEId_1_p,
        const EE_Id_t * const                    EEId_2_p);

/** Compare two EE Application identifiers.

    The definition of the return values of this function is equivalent
    to memcmp. However, as this function may not perform direct memory
    comparison, the results may be something different than a straight
    memcmp over the structs. At the very least this function will
    ignore any padding within the structures.

    Note: In the future, this function is likely to be inlined and/or
    macroized for smaller codesize. Take suitable precautions for
    macrolike side-effects for any parameters you pass to this
    function.

    @param ApplicationId_1_p [in] First application identifier.
    @param ApplicationId_2_p [in] Second application identifier.
    @return
    This function returns smaller than zero is first identifier
    can be considered to be of smaller order than the second.
*/
int
EE_ApplicationId_Cmp(
        const EE_ApplicationId_t * const         ApplicationId_1_p,
        const EE_ApplicationId_t * const         ApplicationId_2_p);

/** Compare two Global Application identifiers.

    The definition of the return values of this function is equivalent
    to memcmp. However, as this function may not perform direct memory
    comparison, the results may be something different than a straight
    memcmp over the structs. At the very least this function will
    ignore any padding within the structures.

    Note: In the future, this function is likely to be inlined and/or
    macroized for smaller codesize. Take suitable precautions for
    macrolike side-effects for any parameters you pass to this
    function.

    @param GlobalApplicationId_1_p
    [in] First global application identifier.

    @param GlobalApplicationId_2_p
    [in] Second global application identifier.

    @return
    This function returns smaller than zero is first identifier
    can be considered to be of smaller order than the second.
*/
int
EE_GlobalApplicationId_Cmp(
        const EE_GlobalApplicationId_t * const   GlobalApplicationId_1_p,
        const EE_GlobalApplicationId_t * const   GlobalApplicationId_2_p);

/* These functions return true if identifier is equivalent to other
   identifier, i.e. these functions return true only if the equivalent
   EE_*Id_Cmp() function returns 0. */

/** Test if two EE identifiers are equivalent.

    @param EEId_1_p
    [in] First execution environment identifier.

    @param EEId_2_p
    [in] Second execution environment identifier.

    @return
    This function returns true if the first identifier is
    equivalent to the second identifier, i\.e\. this function returns
    true only if the equivalent EE_*Id_Cmp() function returns 0.

*/
bool
EE_Id_Eq(
        const EE_Id_t * const                    EEId_1_p,
        const EE_Id_t * const                    EEId_2_p);

/** Test if two EE Application identifiers are equivalent.

    @param ApplicationId_1_p
    [in] First application identifier.

    @param ApplicationId_2_p
    [in] Second application identifier.

    @return
    This function returns true if the first identifier is
    equivalent to the second identifier, meaning that this function
    returns true only if the equivalent EE_*Id_Cmp() function returns 0.

*/
bool
EE_ApplicationId_Eq(
        const EE_ApplicationId_t * const         ApplicationId_1_p,
        const EE_ApplicationId_t * const         ApplicationId_2_p);

/** Test if two EE Global Application identifiers are equivalent.

    @param GlobalApplicationId_1_p [in] First global application identifier.

    @param GlobalApplicationId_2_p [in] Second global application identifier.

    @return
    This function returns true if the first identifier is
    equivalent to the second identifier, i\.e\. this function returns
    true only if the equivalent EE_*Id_Cmp() function returns 0.

*/
bool
EE_GlobalApplicationId_Eq(
        const EE_GlobalApplicationId_t * const   GlobalApplicationId_1_p,
        const EE_GlobalApplicationId_t * const   GlobalApplicationId_2_p);

#endif /* INCLUDE_GUARD_EE_ID_H */

/* end of file ee_id.h */
