/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#ifdef __GNUC__
#define PRINTF_FORMAT(a,b) __attribute__ ((format (printf, (a), (b))))
#else
#define PRINTF_FORMAT(a,b)
#endif

extern int afc_debug_level;

enum afc_status {
	AFC_STATUS_SUCCESS, AFC_STATUS_FAILURE
};

enum {
	MSG_DEBUG, MSG_INFO, MSG_WARNING, MSG_ERROR
};

void afc_printf(int level, const char *fmt, ...) PRINTF_FORMAT(2, 3);
