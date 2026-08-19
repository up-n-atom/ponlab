/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "afc_debug.h"

int afc_debug_level = MSG_INFO;

void afc_printf(int level, const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
        if (level >= afc_debug_level) {
                printf("afcd : ");
                vprintf(fmt, ap);
                printf("\n");
        }
        va_end(ap);
}