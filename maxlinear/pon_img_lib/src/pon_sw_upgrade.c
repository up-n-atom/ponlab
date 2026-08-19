/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <libubus.h>

#include "pon_img.h"
#include "pon_img_debug.h"
#include <pon_img_register.h>
#include <pon_adapter_config.h>

/** \addtogroup PON_IMG_LIB
 *  @{
 */

static const char *help =
	"\n"
	"Options:\n"
	"-f, --filename	Mandatory! Name of the file containing image.\n"
	"-h, --help	Print help and exit.\n"
	"-v, --verbose	Enable verbose mode for more debug data.\n"
	;

static void print_help(char *app_name)
{
	printf("Usage: %s [options]\n", app_name);
	printf("%s", help);
}

static struct option long_opts[] = {
	{"filename", required_argument, 0, 'f'},
	{"help", no_argument, 0, 'h'},
	{"verbose", no_argument, 0, 'v'},
	{0, 0, 0, 0}
};

/** Options string */
static const char opt_string[] = "f:hv";

/** Structure to control application behavior based on options */
struct test_controller {
	/** Name of file containing image */
	char *filename;
	/** Enable image upgrade */
	bool run_enabled;
	/** Enable verbose mode */
	bool verbose_enabled;
} test_ctrl;

static int ubus_call(void *ctx, const char *path, const char *method,
		     struct blob_attr *msg, ubus_data_handler_t cb, void *priv,
		     int timeout)
{
	struct ubus_context *ubus = ctx;
	uint32_t id;
	int err;

	err = ubus_lookup_id(ubus, path, &id);
	if (err)
		return err;

	err = ubus_invoke(ubus, id, method, msg, cb, priv, timeout);
	if (err)
		return err;

	return 0;
}

/** Parse command-line arguments
 *
 *  \param[in] argc Arguments count
 *  \param[in] argv Array of arguments
 */
static int parse_args(int argc, char *argv[])
{
	int c;
	int index;
	int error = 0;

	do {
		c = getopt_long(argc, argv, opt_string, long_opts, &index);

		if (c == -1)
			return 0;

		switch (c) {
		case 'h':
			print_help(argv[0]);
			error = 1;
			break;
		case 'v':
			test_ctrl.verbose_enabled = true;
			libponimg_dbg_lvl_ops.set(DBG_PRN);
			break;
		case 'f':
			if (!optarg) {
				printf("Missing value for argument '-f'\n");
				error = 1;
				break;
			}
			test_ctrl.filename = optarg;
			test_ctrl.run_enabled = true;
			break;
		default:
			break;
		}
	} while (!error);

	return 1;
}

int main(int argc, char *argv[])
{
	struct pon_img_context ctx = {0, };
	static struct ubus_context *ubus_ctx;
	enum pon_adapter_errno ret;
	char partition = 'A';
	bool active;
	const struct pa_config pa_config = {
		.ubus_call = ubus_call,
	};

	/* parse commands arguments */
	if (parse_args(argc, argv)) {
		/* return here if we print help or if we have problem */
		return 0;
	}

	if (!test_ctrl.run_enabled) {
		print_help(argv[0]);
		return 0;
	}

	ubus_ctx = ubus_connect(NULL);
	if (!ubus_ctx) {
		printf("%s: ubus_connect failed\n", argv[0]);
		goto exit;
	}

	ctx.hl_handle = ubus_ctx;
	ctx.pa_config = &pa_config;

	ret = pon_img_active_get(&ctx, 'A', &active);
	if (ret != PON_ADAPTER_SUCCESS) {
		printf("%s: Could not read active state of imageA\n", argv[0]);
		goto exit;
	}
	if (active) {
		if (test_ctrl.verbose_enabled)
			printf("%s: imageA is active\n", argv[0]);
		partition = 'B';
	} else {
		if (test_ctrl.verbose_enabled)
			printf("%s: imageA is inactive\n", argv[0]);
	}

	ret = pon_img_active_get(&ctx, 'B', &active);
	if (ret != PON_ADAPTER_SUCCESS) {
		printf("%s: Could not read active state of imageB\n", argv[0]);
		goto exit;
	}
	if (active) {
		if (test_ctrl.verbose_enabled)
			printf("%s: imageB is active\n", argv[0]);
		partition = 'A';
	} else {
		if (test_ctrl.verbose_enabled)
			printf("%s: imageB is inactive\n", argv[0]);
	}

	ret = pon_img_upgrade(&ctx, partition, test_ctrl.filename);
	if (ret != PON_ADAPTER_SUCCESS) {
		printf("%s: Could not upgrade image \"%s\": %d\n",
		       argv[0], test_ctrl.filename, ret);
		goto exit;
	}

	ret = pon_img_active_set(&ctx, partition);
	if (ret != PON_ADAPTER_SUCCESS) {
		printf("%s: Could not activate image%c\n", argv[0],
		       partition);
		goto exit;
	}

	printf("%s: Please reboot system to boot new image!\n", argv[0]);

exit:
	ubus_free(ubus_ctx);
	return 0;
}

/** @} */
