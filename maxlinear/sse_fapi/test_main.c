/*******************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : test_main.c											*
 *         Description  : ctest application to test secure service APIs			*
 *                        CTEST is from the CMake Project.						*
 *  *****************************************************************************/

#include <stdio.h>

#define CTEST_MAIN

#define CTEST_SEGFAULT
//#define CTEST_NO_COLORS
#define CTEST_COLOR_OK

#include "ctest.h"
#include "fapi_sec_service.h"
#include <secure_services.h>

#ifdef SECDBG_CTEST
#include "secdbg_ctest_util.h"
#endif

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

extern char p_name[MAX_USER_NAME];

#ifdef SECDBG_CTEST
extern int nPortNum;
#endif

int main(int argc, const char *argv[])
{
#ifdef SECDBG_CTEST
	ctest_backupnrestore_authkey_file(nPortNum, true);
	ctest_backupnrestore_keypair_file(nPortNum, true);
#endif

    memcpy_s(p_name, MAX_USER_NAME, argv[0], strlen(argv[0]));
    int result = ctest_main(argc, argv);

	printf("NOTE: Please chech the logs for detailed check\n");
#ifdef SECDBG_CTEST
	ctest_backupnrestore_authkey_file(nPortNum, false);
	ctest_backupnrestore_keypair_file(nPortNum, false);
	ctest_remove_backup_files(nPortNum);
#endif
    return result;
}
