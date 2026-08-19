/******************************************************************************
 *
 *  Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include "fapi_pon.h"

static const char *help =
	"\n"
	"Options:\n"
	"-s, --stest	Mailbox stress test. Argument: number of calls per thread.\n"
	"-t, --threads	Number of threads for chosen test.\n"
	"-h, --help	Print help and exit.\n"
	"-v, --verbose	Enable verbose mode for more debug data.\n"
	;

static void print_help(char *app_name)
{
	if (strlen(app_name) != 0)
		printf("Usage: %s [options]\n", app_name);
	else
		printf("Usage: [options]\n");

	printf("%s", help);
}

static struct option long_opts[] = {
	{"stest", required_argument, 0, 's'},
	{"threads", required_argument, 0, 't'},
	{"help", no_argument, 0, 'h'},
	{"verbose", no_argument, 0, 'v'},
	{0, 0, 0, 0}
};

/* Structure to control test framework */
struct test_controller {
	/* Enable mailbox stress test */
	bool stest_enabled;
	/* Number of calls for chosen test */
	uint32_t call_cnt;
	/* Number of running threads */
	uint32_t thread_cnt;
	/* Enable verbose mode */
	bool verbose_enabled;
} test_ctrl;

/* Mailbox stress test errors counters */
struct stest_call_errors {
	uint32_t fapi_err;
	uint32_t mbox_err;
};

/** Parse command-line arguments
 *
 *  \param[in] argc Arguments count
 *  \param[in] argv Array of arguments
 */
static int parse_args(int argc, char *argv[])
{
	int c;
	int ret;
	int index;
	int error = 0;

	do {
		c = getopt_long(argc, argv, "s:t:hv", long_opts, &index);

		if (c == -1)
			return 0;

		switch (c) {
		case 'h':
			print_help(argv[0]);
			error = 1;
			break;
		case 'v':
			test_ctrl.verbose_enabled = true;
			break;
		case 's':
			if (optarg == NULL) {
				printf("Missing value for argument '-s'\n");
				error = 1;
				break;
			}
			ret = sscanf(optarg, "%u", &test_ctrl.call_cnt);
			if (!ret) {
				printf("Invalid value for argument '-s'\n");
				error = 1;
				break;
			}

			test_ctrl.stest_enabled = true;
			break;
		case 't':
			if (optarg == NULL) {
				printf("Missing value for argument '-t'\n");
				error = 1;
				break;
			}
			ret = sscanf(optarg, "%u", &test_ctrl.thread_cnt);
			if (!ret) {
				printf("Invalid value for argument '-t'\n");
				error = 1;
				break;
			}
			break;
		default:
			break;
		}
	} while (!error);

	return 1;
}

static void *fapi_pon_call(void *vargo)
{
	int i;
	int calls = *(int *)vargo;
	struct stest_call_errors *errors;
	struct pon_ctx *ctx;
	struct pon_gpon_cfg param;
	enum fapi_pon_errorcode ret;

	errors = calloc(1, sizeof(struct stest_call_errors));
	if (!errors) {
		printf("Could not allocate memory for error counters.\n");
		pthread_exit(0);
	}

	ret = fapi_pon_open(&ctx);
	if (ret != PON_STATUS_OK) {
		printf("fapi_pon_open failed - thread_id=%ld errorcode=%d",
			(long)pthread_self(),
			(int)ret);
		pthread_exit(0);
	}

	for (i = 0; i < calls; i++) {
		ret = fapi_pon_gpon_cfg_get(ctx, &param);
		if (ret != PON_STATUS_OK) {
			if (ret == PON_STATUS_MBOX_ERR)
				++errors->mbox_err;
			else
				++errors->fapi_err;
		}

		if (ret != PON_STATUS_OK && test_ctrl.verbose_enabled)
			printf("thread_id=%ld errorcode=%d\n",
				(long)pthread_self(),
				(int)ret);
	}

	ret = fapi_pon_close(ctx);
	if (ret != PON_STATUS_OK) {
		printf("fapi_pon_close failed - thread_id=%ld errorcode=%d",
			(long)pthread_self(),
			(int)ret);
	}

	if (test_ctrl.verbose_enabled)
		printf("thread_id=%ld fapi_error_cnt=%u, mbox_error_cnt=%u\n",
			(long)pthread_self(),
			errors->fapi_err,
			errors->mbox_err);

	pthread_exit(errors);
}

static void mailbox_stress_test(uint32_t threads_cnt, uint32_t calls_cnt,
				uint32_t *fapi_err, uint32_t *mbox_err)
{
	int error;
	uint32_t i;
	struct stest_call_errors *stest_thr_err = 0;
	pthread_t tid[threads_cnt];

	for (i = 0; i < threads_cnt; i++) {
		error = pthread_create(&tid[i], NULL, fapi_pon_call,
				       &calls_cnt);
		if (error != 0)
			printf("pthread create failed - id=%d errorcode=%d",
				i, error);
	}

	for (i = 0; i < threads_cnt; i++) {
		error = pthread_join(tid[i], (void **)&stest_thr_err);
		if (error != 0) {
			printf("pthread_join failed - id=%d errorcode=%d",
				i, error);
			continue;
		}
		if (stest_thr_err != 0) {
			*fapi_err += stest_thr_err->fapi_err;
			*mbox_err += stest_thr_err->mbox_err;

			free(stest_thr_err);
		}
	}
}

int main(int argc, char *argv[])
{
	struct timeval stop, start;
	struct stest_call_errors stest_err = {0};
	unsigned long long start_mili, stop_mili;

	/* parse commands arguments */
	if (parse_args(argc, argv)) {
		/* return here if we print help or if we have problem */
		return 0;
	}

	if (test_ctrl.thread_cnt < 1)
		/* set default thread number */
		test_ctrl.thread_cnt = 8;

	if (test_ctrl.stest_enabled) {
		gettimeofday(&start, NULL);

		mailbox_stress_test(test_ctrl.thread_cnt,
				    test_ctrl.call_cnt,
				    &stest_err.fapi_err,
				    &stest_err.mbox_err);

		gettimeofday(&stop, NULL);

		start_mili = (unsigned long long)start.tv_sec * 1000;
		start_mili += start.tv_usec / 1000;
		stop_mili = (unsigned long long)stop.tv_sec * 1000;
		stop_mili += stop.tv_usec / 1000;

		printf("fapi_error_cnt=%u mbox_error_cnt=%u time=%lf thread_cnt=%u calls_cnt=%u total_calls_cnt=%u\n",
			stest_err.fapi_err,
			stest_err.mbox_err,
			(stop_mili - start_mili) / 1000.0,
			test_ctrl.thread_cnt, test_ctrl.call_cnt,
			test_ctrl.thread_cnt * test_ctrl.call_cnt);

		return 0;
	}

	return 0;
}
