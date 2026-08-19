/******************************************************************************
 *
 *  Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
/**
 *  \file
 *  This file holds the firmware ALARMS helpers definitions.
 */

#ifndef _FAPI_PON_ALARMS_H_
#define _FAPI_PON_ALARMS_H_

/** Alarm type structure.
 *  Used by callback functions of type \ref alarms_visitor_t
 *  to maintain operations on alarms tables:
 *  - pon_alarm_level[]
 *  - pon_alarm_edge[]
 */
struct alarm_type {
	/* alarm id */
	uint32_t	code;
	/* alarm id as string */
	const char	*name;
	/* alarm description */
	const char	*desc;
};

/**
 *	Callback function pointer definition used to define functions
 *	to maintain operations on alarms tables
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open
 *	\param[in] alarm Pointer to struct \ref alarm_type which gives
 *	read access to subsequent message definition passed by external
 *	loop controller
 *	\param[out] data Pointer to a user defined data which can be used
 *	to pass any kind of data to and from the function.
 *
 *	\remarks The function returns a code which is used by external loop
 *	controller to control iterations.
 *
 *	\return Return value as follows:
 *	- -1: If function failed and iterations should be broken
 *	- 1: If function succeeded and next loop is forbidden
 *	- 0: If function succeeded and next loop is alloved
 */
typedef int (*alarms_visitor_t)(void *ctx, const struct alarm_type *alarm,
				void *data);

/**
 *	General purpose LEVEL (static) alarms visitor function.
 *	This function iterates by all entries in pon_alarm_level[] table
 *	calling cb callback function for each table entry.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	The ctx is passed to cb function
 *	\param[in] cb Callback function pointer of type \ref alarms_visitor_t
 *	The pointed function is called for each alarm definition in the
 *	pon_alarm_level[] table
 *	\param[out] data Pointer to a user defined data which can be used
 *	to pass any kind of data to and from the cb function.
 *
 *	\remarks The function reads a code returned by the cb callback function
 *	and uses it to control iterations.
 *
 *	\return Return value as follows:
 *	- -1: If cb function returned -1
 *	- 1: If function succeeded and next loop is forbidden
 *	- 0: If function succeeded and next loop is allowed
 */
int fapi_pon_visit_alarms_level(void *ctx, alarms_visitor_t cb, void *data);

/**
 *	General purpose EDGE alarms visitor function.
 *	This function iterates by all entries in pon_alarm_edge[] table
 *	calling cb callback function for each table entry.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	The ctx is passed to cb function
 *	\param[in] cb Callback function pointer of type \ref alarms_visitor_t
 *	The pointed function is called for each alarm definition in the
 *	pon_alarm_edge[] table
 *	\param[out] data Pointer to a user defined data which can be used
 *	to pass any kind of data to and from the cb function.
 *
 *	\remarks The function reads a code returned by the cb callback function
 *	and uses it to control iterations.
 *
 *	\return Return value as follows:
 *	- -1: If cb function returned -1
 *	- 1: If function succeeded and next loop is forbidden
 *	- 0: If function succeeded and next loop is allowed
 */
int fapi_pon_visit_alarms_edge(void *ctx, alarms_visitor_t cb, void *data);

#endif

