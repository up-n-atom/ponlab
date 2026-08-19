/*
 * Event loop based on select() loop
 * Copyright (c) 2002-2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "list.h"
#include "eloop.h"

struct eloop_sock {
	int sock;
	void *eloop_data;
	void *user_data;
	eloop_sock_handler handler;
};

struct eloop_timeout {
	struct dl_list list;
	struct reltime time;
	void *eloop_data;
	void *user_data;
	eloop_timeout_handler handler;
};

struct eloop_signal {
	int sig;
	void *user_data;
	eloop_signal_handler handler;
	int signaled;
};

struct eloop_sock_table {
	int count;
	struct eloop_sock *table;
	eloop_event_type type;
	int changed;
};

struct eloop_data {
	int max_sock;

	int count; /* sum of all table counts */
	struct eloop_sock_table readers;
	struct eloop_sock_table writers;
	struct eloop_sock_table exceptions;

	struct dl_list timeout;

	int signal_count;
	struct eloop_signal *signals;
	int signaled;
	int pending_terminate;

	int terminate;
};

static struct eloop_data eloop;


#define eloop_trace_sock_add_ref(table) do { } while (0)
#define eloop_trace_sock_remove_ref(table) do { } while (0)


int eloop_init(void)
{
	memset(&eloop, 0, sizeof(eloop));
	dl_list_init(&eloop.timeout);
	return 0;
}


static int eloop_sock_table_add_sock(struct eloop_sock_table *table,
                                     int sock, eloop_sock_handler handler,
                                     void *eloop_data, void *user_data)
{
	struct eloop_sock *tmp;
	int new_max_sock;

	if (sock > eloop.max_sock)
		new_max_sock = sock;
	else
		new_max_sock = eloop.max_sock;

	if (table == NULL)
		return -1;

	eloop_trace_sock_remove_ref(table);
	tmp = realloc_array(table->table, table->count + 1,
			       sizeof(struct eloop_sock));
	if (tmp == NULL) {
		eloop_trace_sock_add_ref(table);
		return -1;
	}

	tmp[table->count].sock = sock;
	tmp[table->count].eloop_data = eloop_data;
	tmp[table->count].user_data = user_data;
	tmp[table->count].handler = handler;
	table->count++;
	table->table = tmp;
	eloop.max_sock = new_max_sock;
	eloop.count++;
	table->changed = 1;
	eloop_trace_sock_add_ref(table);

	return 0;
}


static void eloop_sock_table_remove_sock(struct eloop_sock_table *table,
                                         int sock)
{
	int i;

	if (table == NULL || table->table == NULL || table->count == 0)
		return;

	for (i = 0; i < table->count; i++) {
		if (table->table[i].sock == sock)
			break;
	}
	if (i == table->count)
		return;
	eloop_trace_sock_remove_ref(table);
	if (i != table->count - 1) {
		memmove(&table->table[i], &table->table[i + 1],
			   (table->count - i - 1) *
			   sizeof(struct eloop_sock));
	}
	table->count--;
	eloop.count--;
	table->changed = 1;
	eloop_trace_sock_add_ref(table);
}


static void eloop_sock_table_set_fds(struct eloop_sock_table *table,
				     fd_set *fds)
{
	int i;

	FD_ZERO(fds);

	if (table->table == NULL)
		return;

	for (i = 0; i < table->count; i++) {
		assert(table->table[i].sock >= 0);
		FD_SET(table->table[i].sock, fds);
	}
}


static void eloop_sock_table_dispatch(struct eloop_sock_table *table,
				      fd_set *fds)
{
	int i;

	if (table == NULL || table->table == NULL)
		return;

	table->changed = 0;
	for (i = 0; i < table->count; i++) {
		if (FD_ISSET(table->table[i].sock, fds)) {
			table->table[i].handler(table->table[i].sock,
						table->table[i].eloop_data,
						table->table[i].user_data);
			if (table->changed)
				break;
		}
	}
}


int eloop_sock_requeue(void)
{
	int r = 0;

	return r;
}


static void eloop_sock_table_destroy(struct eloop_sock_table *table)
{
	if (table) {
		free(table->table);
	}
}


int eloop_register_read_sock(int sock, eloop_sock_handler handler,
			     void *eloop_data, void *user_data)
{
	return eloop_register_sock(sock, EVENT_TYPE_READ, handler,
				   eloop_data, user_data);
}

int eloop_register_write_sock(int sock, eloop_sock_handler handler,
                             void *eloop_data, void *user_data)
{
        return eloop_register_sock(sock, EVENT_TYPE_WRITE, handler,
                                   eloop_data, user_data);
}

void eloop_unregister_read_sock(int sock)
{
	eloop_unregister_sock(sock, EVENT_TYPE_READ);
}

void eloop_unregister_write_sock(int sock)
{
        eloop_unregister_sock(sock, EVENT_TYPE_WRITE);
}

static struct eloop_sock_table *eloop_get_sock_table(eloop_event_type type)
{
	switch (type) {
	case EVENT_TYPE_READ:
		return &eloop.readers;
	case EVENT_TYPE_WRITE:
		return &eloop.writers;
	case EVENT_TYPE_EXCEPTION:
		return &eloop.exceptions;
	}

	return NULL;
}


int eloop_register_sock(int sock, eloop_event_type type,
			eloop_sock_handler handler,
			void *eloop_data, void *user_data)
{
	struct eloop_sock_table *table;

	assert(sock >= 0);
	table = eloop_get_sock_table(type);
	return eloop_sock_table_add_sock(table, sock, handler,
					 eloop_data, user_data);
}


void eloop_unregister_sock(int sock, eloop_event_type type)
{
	struct eloop_sock_table *table;

	table = eloop_get_sock_table(type);
	eloop_sock_table_remove_sock(table, sock);
}


int eloop_register_timeout(unsigned int secs, unsigned int usecs,
			   eloop_timeout_handler handler,
			   void *eloop_data, void *user_data)
{
	struct eloop_timeout *timeout, *tmp;
	time_t now_sec;

	timeout = zalloc(sizeof(*timeout));
	if (timeout == NULL)
		return -1;
	if (get_reltime(&timeout->time) < 0) {
		free(timeout);
		return -1;
	}
	now_sec = timeout->time.sec;
	timeout->time.sec += secs;
	if (timeout->time.sec < now_sec) {
		/*
		 * Integer overflow - assume long enough timeout to be assumed
		 * to be infinite, i.e., the timeout would never happen.
		 */
		free(timeout);
		return 0;
	}
	timeout->time.usec += usecs;
	while (timeout->time.usec >= 1000000) {
		timeout->time.sec++;
		timeout->time.usec -= 1000000;
	}
	timeout->eloop_data = eloop_data;
	timeout->user_data = user_data;
	timeout->handler = handler;

	/* Maintain timeouts in order of increasing time */
	dl_list_for_each(tmp, &eloop.timeout, struct eloop_timeout, list) {
		if (reltime_before(&timeout->time, &tmp->time)) {
			dl_list_add(tmp->list.prev, &timeout->list);
			return 0;
		}
	}
	dl_list_add_tail(&eloop.timeout, &timeout->list);

	return 0;
}


static void eloop_remove_timeout(struct eloop_timeout *timeout)
{
	dl_list_del(&timeout->list);
	free(timeout);
}


int eloop_cancel_timeout(eloop_timeout_handler handler,
			 void *eloop_data, void *user_data)
{
	struct eloop_timeout *timeout, *prev;
	int removed = 0;

	dl_list_for_each_safe(timeout, prev, &eloop.timeout,
			      struct eloop_timeout, list) {
		if (timeout->handler == handler &&
		    (timeout->eloop_data == eloop_data ||
		     eloop_data == ELOOP_ALL_CTX) &&
		    (timeout->user_data == user_data ||
		     user_data == ELOOP_ALL_CTX)) {
			eloop_remove_timeout(timeout);
			removed++;
		}
	}

	return removed;
}

int eloop_is_timeout_registered(eloop_timeout_handler handler,
				void *eloop_data, void *user_data)
{
	struct eloop_timeout *tmp;

	dl_list_for_each(tmp, &eloop.timeout, struct eloop_timeout, list) {
		if (tmp->handler == handler &&
		    tmp->eloop_data == eloop_data &&
		    tmp->user_data == user_data)
			return 1;
	}

	return 0;
}


int eloop_deplete_timeout(unsigned int req_secs, unsigned int req_usecs,
			  eloop_timeout_handler handler, void *eloop_data,
			  void *user_data)
{
	struct reltime now, requested, remaining;
	struct eloop_timeout *tmp;

	dl_list_for_each(tmp, &eloop.timeout, struct eloop_timeout, list) {
		if (tmp->handler == handler &&
		    tmp->eloop_data == eloop_data &&
		    tmp->user_data == user_data) {
			requested.sec = req_secs;
			requested.usec = req_usecs;
			get_reltime(&now);
			reltime_sub(&tmp->time, &now, &remaining);
			if (reltime_before(&requested, &remaining)) {
				eloop_cancel_timeout(handler, eloop_data,
						     user_data);
				eloop_register_timeout(requested.sec,
						       requested.usec,
						       handler, eloop_data,
						       user_data);
				return 1;
			}
			return 0;
		}
	}

	return -1;
}


int eloop_replenish_timeout(unsigned int req_secs, unsigned int req_usecs,
			    eloop_timeout_handler handler, void *eloop_data,
			    void *user_data)
{
	struct reltime now, requested, remaining;
	struct eloop_timeout *tmp;

	dl_list_for_each(tmp, &eloop.timeout, struct eloop_timeout, list) {
		if (tmp->handler == handler &&
		    tmp->eloop_data == eloop_data &&
		    tmp->user_data == user_data) {
			requested.sec = req_secs;
			requested.usec = req_usecs;
			get_reltime(&now);
			reltime_sub(&tmp->time, &now, &remaining);
			if (reltime_before(&remaining, &requested)) {
				eloop_cancel_timeout(handler, eloop_data,
						     user_data);
				eloop_register_timeout(requested.sec,
						       requested.usec,
						       handler, eloop_data,
						       user_data);
				return 1;
			}
			return 0;
		}
	}

	return -1;
}


static void eloop_handle_alarm(int sig)
{
	exit(1);
}


static void eloop_handle_signal(int sig)
{
	int i;

	if ((sig == SIGINT || sig == SIGTERM) && !eloop.pending_terminate) {
		/* Use SIGALRM to break out from potential busy loops that
		 * would not allow the program to be killed. */
		eloop.pending_terminate = 1;
		signal(SIGALRM, eloop_handle_alarm);
		alarm(20);
	}

	eloop.signaled++;
	for (i = 0; i < eloop.signal_count; i++) {
		if (eloop.signals[i].sig == sig) {
			eloop.signals[i].signaled++;
			break;
		}
	}
}


static void eloop_process_pending_signals(void)
{
	int i;

	if (eloop.signaled == 0)
		return;
	eloop.signaled = 0;

	if (eloop.pending_terminate) {
		alarm(0);
		eloop.pending_terminate = 0;
	}

	for (i = 0; i < eloop.signal_count; i++) {
		if (eloop.signals[i].signaled) {
			eloop.signals[i].signaled = 0;
			eloop.signals[i].handler(eloop.signals[i].sig,
						 eloop.signals[i].user_data);
		}
	}
}


int eloop_register_signal(int sig, eloop_signal_handler handler,
			  void *user_data)
{
	struct eloop_signal *tmp;

	tmp = realloc_array(eloop.signals, eloop.signal_count + 1,
			       sizeof(struct eloop_signal));
	if (tmp == NULL)
		return -1;

	tmp[eloop.signal_count].sig = sig;
	tmp[eloop.signal_count].user_data = user_data;
	tmp[eloop.signal_count].handler = handler;
	tmp[eloop.signal_count].signaled = 0;
	eloop.signal_count++;
	eloop.signals = tmp;
	signal(sig, eloop_handle_signal);

	return 0;
}


int eloop_register_signal_terminate(eloop_signal_handler handler,
				    void *user_data)
{
	int ret = eloop_register_signal(SIGINT, handler, user_data);
	if (ret == 0)
		ret = eloop_register_signal(SIGTERM, handler, user_data);
	return ret;
}


int eloop_register_signal_reconfig(eloop_signal_handler handler,
				   void *user_data)
{
	return eloop_register_signal(SIGHUP, handler, user_data);
}


void eloop_run(void)
{
	fd_set *rfds, *wfds, *efds;
	struct timeval _tv;
	int res;
	struct reltime tv, now;

	rfds = malloc(sizeof(*rfds));
	wfds = malloc(sizeof(*wfds));
	efds = malloc(sizeof(*efds));
	if (rfds == NULL || wfds == NULL || efds == NULL)
		goto out;

	while (!eloop.terminate &&
	       (!dl_list_empty(&eloop.timeout) || eloop.readers.count > 0 ||
		eloop.writers.count > 0 || eloop.exceptions.count > 0)) {
		struct eloop_timeout *timeout;

		if (eloop.pending_terminate) {
			/*
			 * This may happen in some corner cases where a signal
			 * is received during a blocking operation. We need to
			 * process the pending signals and exit if requested to
			 * avoid hitting the SIGALRM limit if the blocking
			 * operation took more than two seconds.
			 */
			eloop_process_pending_signals();
			if (eloop.terminate)
				break;
		}

		timeout = dl_list_first(&eloop.timeout, struct eloop_timeout,
					list);
		if (timeout) {
			get_reltime(&now);
			if (reltime_before(&now, &timeout->time))
				reltime_sub(&timeout->time, &now, &tv);
			else
				tv.sec = tv.usec = 0;
			_tv.tv_sec = tv.sec;
			_tv.tv_usec = tv.usec;
		}

		eloop_sock_table_set_fds(&eloop.readers, rfds);
		eloop_sock_table_set_fds(&eloop.writers, wfds);
		eloop_sock_table_set_fds(&eloop.exceptions, efds);
		res = select(eloop.max_sock + 1, rfds, wfds, efds,
			     timeout ? &_tv : NULL);
		if (res < 0 && errno != EINTR && errno != 0) {
			goto out;
		}

		eloop.readers.changed = 0;
		eloop.writers.changed = 0;
		eloop.exceptions.changed = 0;

		eloop_process_pending_signals();


		/* check if some registered timeouts have occurred */
		timeout = dl_list_first(&eloop.timeout, struct eloop_timeout,
					list);
		if (timeout) {
			get_reltime(&now);
			if (!reltime_before(&now, &timeout->time)) {
				void *eloop_data = timeout->eloop_data;
				void *user_data = timeout->user_data;
				eloop_timeout_handler handler =
					timeout->handler;
				eloop_remove_timeout(timeout);
				handler(eloop_data, user_data);
			}

		}

		if (res <= 0)
			continue;

		if (eloop.readers.changed ||
		    eloop.writers.changed ||
		    eloop.exceptions.changed) {
			 /*
			  * Sockets may have been closed and reopened with the
			  * same FD in the signal or timeout handlers, so we
			  * must skip the previous results and check again
			  * whether any of the currently registered sockets have
			  * events.
			  */
			continue;
		}

		eloop_sock_table_dispatch(&eloop.readers, rfds);
		eloop_sock_table_dispatch(&eloop.writers, wfds);
		eloop_sock_table_dispatch(&eloop.exceptions, efds);
	}

	eloop.terminate = 0;
out:
	free(rfds);
	free(wfds);
	free(efds);
	return;
}


void eloop_terminate(void)
{
	eloop.terminate = 1;
}


void eloop_destroy(void)
{
	struct eloop_timeout *timeout, *prev;
	struct reltime now;

	get_reltime(&now);
	dl_list_for_each_safe(timeout, prev, &eloop.timeout,
			      struct eloop_timeout, list) {
		int sec, usec;
		sec = timeout->time.sec - now.sec;
		usec = timeout->time.usec - now.usec;
		if (timeout->time.usec < now.usec) {
			sec--;
			usec += 1000000;
		}
		eloop_remove_timeout(timeout);
	}
	eloop_sock_table_destroy(&eloop.readers);
	eloop_sock_table_destroy(&eloop.writers);
	eloop_sock_table_destroy(&eloop.exceptions);
	free(eloop.signals);

}


int eloop_terminated(void)
{
	return eloop.terminate || eloop.pending_terminate;
}


void eloop_wait_for_read_sock(int sock)
{
	/*
	 * We can use epoll() here. But epoll() requres 4 system calls.
	 * epoll_create1(), epoll_ctl() for ADD, epoll_wait, and close() for
	 * epoll fd. So select() is better for performance here.
	 */
	fd_set rfds;

	if (sock < 0)
		return;

	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);
	select(sock + 1, &rfds, NULL, NULL, NULL);
}
