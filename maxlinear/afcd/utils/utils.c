/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "afc.h"

/* helper fuctions */
int get_reltime(struct reltime *t)
{
	int ret;
	struct timespec ts;

	ret = clock_gettime(CLOCK_REALTIME, &ts);
	if (!ret) {
		t->sec = ts.tv_sec;
		t->usec = ts.tv_nsec / 1000;
		return 0;
	}

	return ret;
}

int reltime_before(struct reltime *a,
		   struct reltime *b)
{
	return (a->sec < b->sec) ||
		(a->sec == b->sec && a->usec < b->usec);
}

void reltime_sub(struct reltime *a, struct reltime *b,
		 struct reltime *res)
{
	res->sec = a->sec - b->sec;
	res->usec = a->usec - b->usec;
	if (res->usec < 0) {
		res->sec--;
		res->usec += 1000000;
	}
}

int reltime_expired(struct reltime *now,
		    struct reltime *ts,
		    time_t timeout_secs)
{
	struct reltime age;

	reltime_sub(now, ts, &age);
	return (age.sec > timeout_secs) ||
		(age.sec == timeout_secs && age.usec > 0);
}

int reltime_expired_ms(struct reltime *now,
		       struct reltime *ts,
		       time_t timeout_ms)
{
	struct reltime age;
	time_t age_ms;

	reltime_sub(now, ts, &age);
	age_ms = age.sec * 1000 + age.usec / 1000;
	return (age_ms > timeout_ms);
}

void *realloc_array(void *ptr, size_t nmemb, size_t size)
{
	if (size && nmemb > (~(size_t)0) / size)
		return NULL;
	return realloc(ptr, nmemb * size);
}

void *zalloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}