/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : logdefs.h                                                  *
 *     Project    : UGW                                                        *
 *     Description: logs infrastructure for sysuprade                          *
 *                                                                             *
 ******************************************************************************/

/*! \file logdefs.h
    \brief This file provides necessary structures and macro based infrastructure
	to hook up sysupgrade and.
*/

/** \addtogroup SYSUPGRADE */
/* @{ */

#ifndef _LOGDEFS_H
#define _LOGDEFS_H

#include "ugw_proto.h"
#include "ulogging.h"

/*!
	\brief Sysupgrade exported enumerated error codes.
*/
/*! \brief inline function to retrieve log level from combined unLogFlg integer.
	\unLogFlg integer stores both log level and log type.
	\param[in] Input current unLogFlg integer.
	\return Log level.
*/
static inline uint16_t get_logLevel(IN uint16_t unLogFlg)
{
	return unLogFlg / 100;
}

/*! \brief inline function to retrieve log type from combined unLogFlg integer.
	\unLogFlg integer stores both log level and log type.
	\param[in] Input current unLogFlg integer.
	\return Log Type.
*/
static inline uint16_t get_logType(IN uint16_t unLogFlg)
{
	return (unLogFlg % 100)/10;
}

/*! \brief inline function to update log level in combined unLogFlg integer.
	\unLogFlg integer stores both log level and log type.
	\param[in] Input current unLogFlg integer.
	\param[in] Input log level to update.
	\return updated log value. Assign it back to unLogFlg integer.
*/
static inline uint16_t put_logLevel(IN uint16_t unLogFlg, IN uint16_t unLevel)
{
	/* MAX logLevel is 7 */
	if (unLevel < 8)
		return (unLevel * 100) + (unLogFlg % 100);
	else
		/*More than that fix it to MAX*/
		return (7 * 100) + (unLogFlg % 100);
}

/*! \brief inline function to update log type in combined unLogFlg integer.
	\unLogFlg integer stores both log level and log type.
	\param[in] Input current unLogFlg integer.
	\param[in] Input log type to update.
	\return updated log value. Assign it back to unLogFlg integer.
*/
static inline uint16_t put_logType(IN uint16_t unLogFlg, IN uint16_t unType)
{
	/* Max logType is 4. Actually 0x0, 0x1, 0x2 and 0x4*/
	if (unType < 5)
		return ((unLogFlg / 100) * 100) + unType * 10 + ((unLogFlg%100)%10);
	else
		/* More than that fix it to MAX*/
		return ((unLogFlg / 100) * 100) + 4 * 10 + ((unLogFlg%100)%10);
}
#endif
/* @} */
