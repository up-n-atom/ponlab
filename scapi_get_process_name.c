/******************************************************************************

  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/


/*
 * =====================================================================================
 *
 *       Filename:  scapi_get_process_name.c
 *
 *    Description:  API to get process name from process number
 *
 *        Version:  1.0
 *        Created:  08/11/14 19:17:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lantiq Communications (), 
 *   Organization:  Lantiq Communications
 *
 * =====================================================================================
 */

#include <errno.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <ltq_api_include.h>

#define FAIL "INVAL PROCESS"
/* 
** =============================================================================
**   Function Name    :	scapi_get_process_name
**
**   Description      :	Gets process name from process number
**
**   Parameters       :	process_num(IN) -> Process number
**
**   Return Value     :	Success -> Process name string
**			Failure -> FAIL
** 
**   Notes            :  
**
** ============================================================================
*/

char process_name[16]; /*  To BSS */

char* scapi_get_process_name(IN pid_t process_num)
{
	char* ret = NULL;
	char cBuff[128] = {0};
	if (sprintf_s(cBuff, sizeof(cBuff), "/proc/%d/stat", (int)process_num) <= 0) {
                fprintf( stderr, "Error : sprintf_s failed %s:%d %s\n", __func__, __LINE__, strerror(errno)); 
		return FAIL;
	}
	char* start = NULL, *end = NULL;


	FILE* process_stat_file = scapi_getFilePtr(cBuff, "r");
	if(NULL == process_stat_file)
	{
		ret = FAIL; /* String literals are stored in data/code memory of the process. So no prob */
		goto returnHandler;
	}

	/*  Using the same cBuff for reading line. Reading one line is enough as process
	 *  name will definetely be in first line */
	if(NULL == fgets(cBuff, sizeof(cBuff), process_stat_file))
	{
		ret = FAIL;
		goto returnHandler;
	}
	if(NULL == (start = strchr(cBuff, '(') ))
	{
		ret = FAIL;
		goto returnHandler;
	}

	++start;

	if(NULL == (end = strchr(cBuff, ')') ))
	{
		ret = FAIL;
		goto returnHandler;
	}
	/*  null terminating after our string of interest */
	*end = '\0';
	memset(process_name, 0, sizeof(process_name));
	if (sprintf_s(process_name, sizeof(process_name), "%s", start) <= 0) {
                fprintf( stderr, "Error : sprintf_s failed %s:%d %s\n", __func__, __LINE__, strerror(errno)); 
		ret = FAIL;
		goto returnHandler;
	}
	ret = process_name;

returnHandler:
	if(process_stat_file != NULL)
		fclose(process_stat_file);
	return ret;
}


