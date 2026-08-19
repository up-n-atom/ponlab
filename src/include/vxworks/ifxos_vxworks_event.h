/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_VXWORKS_EVENT_H
#define _IFXOS_VXWORKS_EVENT_H

#ifdef VXWORKS

/** \file
   This file contains VxWorks definitions for Event Synchronization and Signalization.
*/

/** \defgroup IFXOS_SYNC_VXWORKS Synchronization.

   This Group collect the VxWorks synchronization and signaling mechanism used within
   IFXOS.

   The IFX OS differs between the synchronization on processes level
   (threads / tasks) and between user and driver space.

\par Task level Synchronization
   For synchronization on task level a "Event feature" is provided.

\note
   The intention of these signaling feature is to communicate between different
   tasks within the driver space or within the application space.

\par Application-Driver Synchronization
   Therefore the poll/select mechanism is prepared.

\ingroup IFXOS_LAYER_VXWORKS
*/

/** \defgroup IFXOS_EVENT_VXWORKS Event Synchronization (VxWorks).

   This Group contains the VxWorks Event definitions and function for
   communication of tasks.

\par Implementation
   The event handling is based on binary semaphores (FIFO queuing, with timeout).

\ingroup IFXOS_SYNC_VXWORKS
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX VxWorks adaptation - Includes
   ========================================================================= */
#include <vxWorks.h>
#include <semLib.h>


/* ============================================================================
   IFX VxWorks adaptation - supported features
   ========================================================================= */

/** IFX VxWorks adaptation - support "EVENT feature" */
#ifndef IFXOS_HAVE_EVENT
#  define IFXOS_HAVE_EVENT                           1
#endif

/* ============================================================================
   IFX VxWorks adaptation - EVENT types
   ========================================================================= */
/** \addtogroup IFXOS_EVENT_VXWORKS
@{ */

/** Vxworks - EVENT, type for synchronization. */
typedef struct
{
   /** event object */
   SEM_ID object;
   /** valid flag */
   IFX_boolean_t bValid;
} IFXOS_event_t;

/** @} */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef VXWORKS */
#endif      /* #ifndef _IFXOS_VXWORKS_EVENT_H */

