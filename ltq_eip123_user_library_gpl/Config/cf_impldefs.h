/* cf_impldefs.h
 *
 * Description: Configuration options for Framework/IMPLDEFS implementation
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

/*
   All L_PRINTFs (ie. all debug/trace and panic messages).
 */
#undef  IMPLDEFS_CF_DISABLE_DEBUG_L_PRINTF


/*
   L_DEBUG print outs.
 */
#undef  IMPLDEFS_CF_DISABLE_L_DEBUG

#ifdef CFG_IMPLDEFS_NO_DEBUG
#define IMPLDEFS_CF_DISABLE_L_DEBUG
#endif


/*
   L_TRACE print outs.
 */
#define IMPLDEFS_CF_DISABLE_L_TRACE

#ifdef CFG_IMPLDEFS_ENABLE_TRACE
#undef  IMPLDEFS_CF_DISABLE_L_TRACE
#endif


/*
   ASSERT() macro, i.e. assertion checks.
 */
#undef  IMPLDEFS_CF_DISABLE_ASSERT

#ifdef CFG_IMPLDEFS_NO_DEBUG
#define IMPLDEFS_CF_DISABLE_ASSERT
#endif

/*
   PRECONDITION() macro, ie. function contract input checks.
 */
#undef  IMPLDEFS_CF_DISABLE_PRECONDITION

#ifdef CFG_IMPLDEFS_NO_DEBUG
#define IMPLDEFS_CF_DISABLE_PRECONDITION
#endif

/*
   POSTCONDITION() macro, ie. function contract output checks.
 */
#undef  IMPLDEFS_CF_DISABLE_POSTCONDITION

#ifdef CFG_IMPLDEFS_NO_DEBUG
#define IMPLDEFS_CF_DISABLE_POSTCONDITION
#endif

/**
   All assertion and function contract checks.
   (Ie. ASSERT(), PRECONDITION(), and POSTCONDITION() macros.)
 */
#undef  IMPLDEFS_CF_DISABLE_ASSERTION_CHECK

#ifdef CFG_IMPLDEFS_NO_DEBUG
#define IMPLDEFS_CF_DISABLE_ASSERTION_CHECK
#endif

/* end of file cf_impldefs.h */
