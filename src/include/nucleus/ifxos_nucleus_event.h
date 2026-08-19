/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_NUCLEUS_EVENT_H
#define _IFXOS_NUCLEUS_EVENT_H

#ifdef NUCLEUS_PLUS

/** \file
   This file contains Nucleus definitions for Event Synchronization and Signalization.
*/

/** \defgroup IFXOS_SYNC_NUCLEUS Synchronization.

   This Group collect the Nucleus synchronization and signaling mechanism used within
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

\ingroup IFXOS_LAYER_NUCLEUS
*/

/** \defgroup IFXOS_EVENT_NUCLEUS Event Synchronization (Nucleus).

   This Group contains the Nucleus Event definitions and function for
   communication of tasks.

\par Implementation
   The event handling is based on binary semaphores (FIFO queuing, with timeout).

\ingroup IFXOS_SYNC_NUCLEUS
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX Nucleus adaptation - Includes
   ========================================================================= */
#include <nucleus.h>

/* ============================================================================
   IFX Nucleus adaptation - supported features
   ========================================================================= */

/** IFX Nucleus adaptation - support "EVENT feature" */
#ifndef IFXOS_HAVE_EVENT
#  define IFXOS_HAVE_EVENT                           1
#endif

/* ============================================================================
   IFX Nucleus adaptation - EVENT types
   ========================================================================= */
/** \addtogroup IFXOS_EVENT_NUCLEUS
@{ */

/** Nucleus - EVENT, type for synchronization. */
typedef struct
{
   /** event object */
   NU_EVENT_GROUP object;
   /** valid flag */
   IFX_boolean_t bValid;
} IFXOS_event_t;

/** @} */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef NUCLEUS_PLUS */
#endif      /* #ifndef _IFXOS_NUCLEUS_EVENT_H */

