/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <time.h>

struct reltime {
	time_t sec;
	time_t usec;
};

int get_reltime(struct reltime *t);
int reltime_before(struct reltime *a, struct reltime *b);
void reltime_sub(struct reltime *a, struct reltime *b, struct reltime *res);
int reltime_expired(struct reltime *now, struct reltime *ts, time_t timeout_secs);
int reltime_expired_ms(struct reltime *now, struct reltime *ts, time_t timeout_ms);
void *realloc_array(void *ptr, size_t nmemb, size_t size);
void *zalloc(size_t size);