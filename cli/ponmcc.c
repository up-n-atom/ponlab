/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>

#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <netlink/attr.h>
#include <arpa/inet.h>

#include <pon/pon_mcc.h>
#include <pon/pon_mcc_ikm.h>

enum getopt_options {
	IFINDEX,
	PROTO,
	GRP_ADDR,
	SRC_ADDR,
	FLT_MODE,
	IGMP_VERSION,
	MC_GEM_IFINDEX,
	ADD,
	DEL,
	READ,
	MULTICAST_ENABLE,
	MULTICAST_DISABLE,
	HELP
};

struct flags {
	int adflag;
	int eflag;
	int ifindex_flag;
	int proto_flag;
	int grpaddr_flag;
	int srcaddr_flag;
	int fltmode_flag;
	int igmpversion_flag;
	int mcgemifindex_flag;
};

static const struct option long_options[] = {
	{"table_entry_add",	no_argument,		0, ADD},
	{"table_entry_delete",	no_argument,		0, DEL},
	{"table_entry_read",	required_argument,	0, READ},
	{"multicast_enable",	no_argument,		0, MULTICAST_ENABLE},
	{"multicast_disable",	no_argument,		0, MULTICAST_DISABLE},
	{"ifindex",		required_argument,	0, IFINDEX},
	{"proto",		required_argument,	0, PROTO},
	{"grp_addr",		required_argument,	0, GRP_ADDR},
	{"src_addr",		required_argument,	0, SRC_ADDR},
	{"flt_mode",		required_argument,	0, FLT_MODE},
	{"igmp_version",	required_argument,	0, IGMP_VERSION},
	{"mc_gem_ifindex",	required_argument,	0, MC_GEM_IFINDEX},
	{"help",		no_argument,		0, HELP},
	{NULL,			0,			0,  0 },
};

static void print_help(void)
{
	fprintf(stdout, "PON MCC driver command line interface\n\n");
	fprintf(stdout, "Parameters:\n");
	fprintf(stdout,
		"--table_entry_add --ifindex {ifindex} --proto {proto (0 - IPv4, 1 - IPv6)} --grp_addr {grp_addr} --src_addr {src_addr} --flt_mode {flt_mode}\n");
	fprintf(stdout,
		"--table_entry_delete --ifindex {ifindex} --proto {proto (0 - IPv4, 1 - IPv6)} --grp_addr {grp_addr} --src_addr {src_addr} --flt_mode {flt_mode}\n");
	fprintf(stdout, "--table_entry_read {initial}\n");
	fprintf(stdout,
		"--multicast_enable --igmp_version {igmp_version} --mc_gem_ifindex {mc_gem_ifindex}\n");
	fprintf(stdout, "--multicast_disable\n");
	fprintf(stdout,
		"Example: table entry read = \"--table_entry_read 1\"\n\n");
}

static int table_entry_mod(uint32_t ifindex, uint8_t proto, const char *grp,
			   const char *src, uint8_t flt_mode, int add)
{
	uint8_t src_buf[PON_MCC_ADDR6SZ];
	uint8_t grp_buf[PON_MCC_ADDR6SZ];
	int ret = 0;
	int family = 0;

	switch (proto) {
	case MCC_L3_PROTO_IPV4:
		family = AF_INET;
		break;
	case MCC_L3_PROTO_IPV6:
		family = AF_INET6;
		break;
	default:
		return -EINVAL;
	}

	if (src) {
		ret = inet_pton(family, src, src_buf);
		if (!ret)
			return -errno;
	}

	if (grp) {
		ret = inet_pton(family, grp, grp_buf);
		if (!ret)
			return -errno;
	}

	if (add)
		return mcl_table_entry_add(ifindex, proto, grp ? grp_buf : NULL,
					   src ? src_buf : NULL, flt_mode);
	else
		return mcl_table_entry_del(ifindex, proto, grp ? grp_buf : NULL,
					   src ? src_buf : NULL, flt_mode);
}

static int table_entry_add(uint32_t ifindex, uint8_t proto, const char *grp,
			   const char *src, uint8_t flt_mode)
{
	return table_entry_mod(ifindex, proto, grp, src, flt_mode, 1);
}

static int table_entry_del(uint32_t ifindex, uint8_t proto, const char *grp,
			   const char *src, uint8_t flt_mode)
{
	return table_entry_mod(ifindex, proto, grp, src, flt_mode, 0);
}

int main(int argc, char **argv)
{
	struct flags flags;
	const char *grp_addr = NULL;
	const char *src_addr = NULL;
	uint8_t initial = 1;
	uint8_t igmp_version = IGMP_V2;
	uint32_t ifindex = 0;
	uint8_t proto = 0;
	uint8_t flt_mode = 0;
	uint32_t mc_gem_ifindex = 0;
	int option_index;
	int opt;
	int ret = EXIT_SUCCESS;

	if (argc < 2)
		fprintf(stdout,
			"Use \"--help\" to check list of available commands\n");

	memset(&flags, 0, sizeof(struct flags));

	while ((opt = getopt_long(argc, argv, "", long_options,
				  &option_index)) != -1) {
		switch (opt) {
		case ADD:
			flags.adflag = 1;
			break;
		case DEL:
			flags.adflag = 2;
			break;
		case READ:
			initial = strtoul(optarg, NULL, 0);
			ret = mcl_table_entry_read(initial);
			fprintf(stdout, "errorcode=%d\n", ret);
			break;
		case MULTICAST_ENABLE:
			flags.eflag = 1;
			break;
		case MULTICAST_DISABLE:
			ret = mcl_multicast_disable();
			fprintf(stdout, "errorcode=%d\n", ret);
			break;
		case IFINDEX:
			flags.ifindex_flag = 1;
			ifindex = strtoul(optarg, NULL, 0);
			break;
		case PROTO:
			flags.proto_flag = 1;
			proto = strtoul(optarg, NULL, 0);
			break;
		case GRP_ADDR:
			flags.grpaddr_flag = 1;
			grp_addr = optarg;
			break;
		case SRC_ADDR:
			flags.srcaddr_flag = 1;
			src_addr = optarg;
			break;
		case FLT_MODE:
			flags.fltmode_flag = 1;
			flt_mode = strtoul(optarg, NULL, 0);
			break;
		case IGMP_VERSION:
			flags.igmpversion_flag = 1;
			igmp_version = strtoul(optarg, NULL, 0);
			break;
		case MC_GEM_IFINDEX:
			flags.mcgemifindex_flag = 1;
			mc_gem_ifindex = strtoul(optarg, NULL, 0);
			break;
		case HELP:
			print_help();
			break;
		default:
			fprintf(stderr, "Invalid parameter\n");
			print_help();
			break;
		}
	}

	if (flags.adflag == 1) {
		if (flags.ifindex_flag && flags.proto_flag &&
		    flags.grpaddr_flag && flags.srcaddr_flag &&
		    flags.fltmode_flag) {
			ret = table_entry_add(ifindex, proto, grp_addr,
					      src_addr, flt_mode);
			fprintf(stdout, "errorcode=%d\n", ret);
		} else {
			fprintf(stdout,
				"at least one of input parameters is missing\n");
			return -EINVAL;
		}
	}

	if (flags.adflag == 2) {
		if (flags.ifindex_flag && flags.proto_flag &&
		    flags.grpaddr_flag && flags.srcaddr_flag &&
		    flags.fltmode_flag) {
			ret = table_entry_del(ifindex, proto, grp_addr,
					      src_addr, flt_mode);
			fprintf(stdout, "errorcode=%d\n", ret);
		} else {
			fprintf(stdout,
				"at least one of input parameters is missing\n");
			return -EINVAL;
		}
	}

	if (flags.eflag == 1) {
		if (flags.igmpversion_flag && flags.mcgemifindex_flag) {
			ret = mcl_multicast_enable(igmp_version,
						   mc_gem_ifindex);
			fprintf(stdout, "errorcode=%d\n", ret);
		} else {
			fprintf(stdout,
				"at least one of input parameters is missing\n");
			return -EINVAL;
		}
	}

	return ret;
}
