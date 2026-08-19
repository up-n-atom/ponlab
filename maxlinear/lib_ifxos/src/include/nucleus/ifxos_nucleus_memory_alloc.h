/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_NUCLEUS_MEM_ALLOC_H
#define _IFXOS_NUCLEUS_MEM_ALLOC_H

#ifdef NUCLEUS_PLUS

/** \file
   This file contains Nucleus definitions for Memory handling.
*/

/** \defgroup IFXOS_MEMORY_NUCLEUS Memory Handling (Nucleus)

   This Group contains the Nucleus memory allocation and mapping definitions and
   function.

   Here we have to differ between:\n
   - memory handling on application space.
   - memory handling on driver space.

\par Driver Space
   There are several special memory handling functions which makes only sense
   in driver space.
   - Allocation of an continuous memory block (HW interfaces).
   - Physical to virtual address mapping.
   - Data exchange between user and driver space (copy from/to user).

\note
   This split comes up with the LINUX OS. Under LINUX the user and Kernel space are
   independent.
   Further it makes sense to handle driver related functions separately.

\ingroup IFXOS_LAYER_NUCLEUS
*/

/** \defgroup IFXOS_MEM_ALLOC_NUCLEUS Memory Allocation (Nucleus).

   This Group contains the Nucleus Memory Allocation definitions.

\par Implementation - Memory allocation
   Within Nucleus there is no special handling for allocation a continuous
   memory block. So the standard malloc function is used for all adaptations.

\ingroup IFXOS_MEMORY_NUCLEUS
*/


/** \defgroup IFXOS_CPY_USER_SPACE_NUCLEUS_DRV Data Exchange, Driver and User Space (Nucleus).

   This Group contains the Nucleus definitions for data exchange between
   driver and application.

\par Implementation
   Under Nucleus no special handling for Data Exchange between driver and user
   space is required. But such a border makes sense we provide the corresponding
   IFXOS Layer functions for compatibility.

   For data exchange the standard memcpy function is used.

\ingroup IFXOS_MEMORY_NUCLEUS
*/

/** \defgroup IFXOS_MEMORY_MAP_NUCLEUS_DRV Physical to Virtual Address Mapping (Nucleus).

   This Group contains the Nucleus definitions for Physical to
   Virtual Address Mapping.

\par Implementation
   Under Nucleus no special handling for Physical to Virtual Address Mapping is
   required (no Virtual Memory Management).
   To keep the compatibility within the driver code we provide the corresponding
   IFXOS Layer functions.
   For the mapping the physical address is simply assigned.

\ingroup IFXOS_MEMORY_NUCLEUS
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX Nucleus adaptation - Includes
   ========================================================================= */

/* ============================================================================
   IFX Nucleus adaptation - supported features
   ========================================================================= */

/** IFX Nucleus adaptation - support "mem alloc" */
#ifndef IFXOS_HAVE_BLOCK_ALLOC
#  define IFXOS_HAVE_BLOCK_ALLOC                   1
#endif

/** IFX Nucleus adaptation - support "virtual mem alloc" */
#ifndef IFXOS_HAVE_MEM_ALLOC
#  define IFXOS_HAVE_MEM_ALLOC                     1
#endif

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef NUCLEUS_PLUS */
#endif      /* #ifndef _IFXOS_NUCLEUS_MEM_ALLOC_H */


