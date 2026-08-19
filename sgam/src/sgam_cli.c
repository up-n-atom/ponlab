// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
** FILE NAME    : sgam_cli.c
** PROJECT      : SGAM Configuration Utility
** MODULES      : Session Group Accounting and Metering (SGAM)
** AUTHOR       : Shashidhar Kattemane Venkatesh
** DESCRIPTION  : SGAM (Bandwidth Monitoring) User Configuration Utility
** COPYRIGHT    : Copyright (c) 2022-2024 MaxLinear, Inc.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <net/sgam_api.h>

#define SGAM_MAX_CMD_OPTS  7
#define SGAM_CMD_NAME      "sgam_cli"
#define SGAM_DEVICE        "/dev/"SGAM_DEV_NAME
#define MAX_RATE_M         (unsigned int)10000 /* Max rate (10Gbps) in Mbps */
#define MAX_RATE_K         MAX_RATE_M * 1000   /* Max rate in Kbps */
#define MAX_BURST_K        (UINT_MAX / 1024)   /* Max burst size in KBytes */
#define MAX_BURST          (UINT_MAX)          /* Max burst size in Bytes*/
#define MAX_DSCP           64                  /* Max DSCP value */

#define SGAM_ERR(msg, ...) fprintf(stderr, "[%s:err] "msg, SGAM_CMD_NAME,\
				   ##__VA_ARGS__)

#define SGAM_INFO(msg, ...) fprintf(stdout, msg, ##__VA_ARGS__)

#define SGAM_DBG(msg, ...) fprintf(stdout, "[%s:%d] "msg, __func__, __LINE__,\
				   ##__VA_ARGS__)

enum sgam_cli_err_types {
	SGAM_CMD_OK = 0,
	SGAM_CMD_HELP,
	SGAM_CMD_DONE,
	SGAM_CMD_NOT_AVAIL,
	SGAM_CMD_ERR,
};

typedef struct sgam_cmd_opts_t {
	int  opt;
	char *optarg;
} SGAM_CMD_OPTS;

typedef struct sgam_cmd_t {
	/* Command string */
	const char *name;

	/* IOCTL command value */
	int ioctl_cmd;

	/* Function to print help for the command.
	 * If 'summary' is:
	 *     0 : print brief help,
	 *     1 : print detailed help.
	 */
	void (*print_help)(int summary);

	/* Function for parsing the command line options and their args.
	 * Also allocates memory to store them.
	 */
	int (*parse_options)(SGAM_CMD_OPTS *popts, void **pdata);

	/* Error checks the options and args from parse_option,
	 * invoke appropriate IOCTL and pass the necessary opts and args to
	 * SGAM kmod.
	 */
	int (*do_command)(struct sgam_cmd_t *pcmd, void *pdata);

	/* Print data returned by SGAM kmod after successful IOCTL call.
	 */
	void (*print_data)(void *pdata, int ret_val);

	/* Long options for the command, ex: --name*/
	const struct option *long_opts;

	/* Short options for the command, ex: -n */
	const char  *short_opts;
} SGAM_COMMAND;

/* NOTE: Order of strings here is directly linked to sgam_meter_types from
 * sgam_api.h. So please do not change the order without updating
 * sgam_meter_types.
 */
static char g_meter_modes[SGAM_TBM_MODES_NUM + 1][15] = {
							 "dtrtcm_cb",
							 "dtrtcm_ca",
							 "trtcm_cb",
							 "trtcm_ca",
							};

/* Common operations for both acct and meter */
static const char sgam_show_cmd_short_opts[] = "-:n:h";
static const struct option sgam_show_cmd_long_opts[] = {
	{"help", no_argument,  NULL, 'h'},
	{"name", required_argument,  NULL, 'n'},
	{0, 0, 0, 0}
};

static const char sgam_list_short_opts[] = "-:h";
static const struct option sgam_list_long_opts[] = {
	{"help", no_argument,  NULL, 'h'},
	{0, 0, 0, 0}
};

static const char sgam_del_short_opts[] = "-:n:h";
static const struct option sgam_del_long_opts[] = {
	{"help", no_argument,  NULL, 'h'},
	{"name", required_argument,  NULL, 'n'},
	{0, 0, 0, 0}
};

static int sgam_do_ioctl_cmd(int ioctl_cmd, void *data)
{
	int ret = SGAM_CMD_OK;
	int fd  = 0;

	fd = open(SGAM_DEVICE, O_RDWR);
	if (fd < 0) {
		SGAM_ERR("open SGAM device (%s) failed. (errno=%d)\n",
			 SGAM_DEVICE, errno);
		ret = SGAM_CMD_ERR;
	} else {
		ret = ioctl(fd, ioctl_cmd, data);
		if (ret < 0)
			ret = errno;
		close(fd);
	}

	return ret;
}

static int sgam_copy_label(char *dst, char *label)
{
	int ret = SGAM_CMD_OK;

	if (!label || !dst) { /* Unlikely */
		SGAM_ERR("[BUG]NULL label or dst!!");
		return SGAM_CMD_ERR;
	}

	if (strlen(label) > MAX_LABEL_LEN) {
		SGAM_ERR("Invalid name: %s!! length is > %d\n", label,
			 MAX_LABEL_LEN);
		ret = SGAM_CMD_ERR;
	} else {
		strncpy(dst, label, MAX_LABEL_LEN);
	}

	return ret;
}

static int sgam_generic_do_cmd(SGAM_COMMAND *pcmd, void *pdata)
{
	int ret = SGAM_CMD_OK;

	ret = sgam_do_ioctl_cmd(pcmd->ioctl_cmd, pdata);

	return ret;
}

static int parse_list_cmd(bool is_acct, void **pdata)
{
	int ret = SGAM_CMD_OK;
	struct sgam_count_info *count_info =
		(struct sgam_count_info *)calloc(1, sizeof(struct sgam_count_info));

	if (!count_info) {
		SGAM_ERR("Failed to alloc memory!\n");
		return SGAM_CMD_ERR;
	}
	*pdata = count_info;

	if (is_acct)
		count_info->db_flag |= SGAM_ACCT_DB_FLAG;
	else
		count_info->db_flag |= SGAM_METER_DB_FLAG;

	return ret;
}

static int sgam_list_do_cmd(SGAM_COMMAND *pcmd, void *pdata)
{
	int ret = SGAM_CMD_OK, i = 0, size = 0;
	struct sgam_list_info *list_info = NULL;
	struct sgam_count_info *count_info = (struct sgam_count_info *)pdata;

	ret = sgam_do_ioctl_cmd(SGAM_GET_COUNT_CMD, count_info);
	if (ret != SGAM_CMD_OK) {
		SGAM_ERR("ICOTL returned err!\n");
		return SGAM_CMD_ERR;
	}

	if (!count_info->count) {
		SGAM_INFO("There are 0 %s configured\n",
			  IS_ACCT_DB(count_info->db_flag) ? "accounts" :
							    "meters");
		return SGAM_CMD_OK;
	}

	size = sizeof(struct sgam_list_info) +
		((count_info->count + 1) * sizeof(struct sgam_name_list));
	list_info = (struct sgam_list_info *)malloc(size);

	if (!list_info) {
		SGAM_ERR("list_info malloc failed!!\n");
		return SGAM_CMD_ERR;
	}

	list_info->count_info.count = count_info->count;
	list_info->count_info.db_flag = count_info->db_flag;
	ret = sgam_do_ioctl_cmd(SGAM_LIST_CMD, list_info);

	if (ret != SGAM_CMD_OK) {
		SGAM_ERR("ICOTL returned err!\n");
		ret = SGAM_CMD_ERR;
	} else {
		SGAM_INFO("  Below is the list of %s configured\n",
			  IS_ACCT_DB(count_info->db_flag) ? "accounts" :
							    "meters");
		for (i = 0; i < list_info->count_info.count; i++) {
			SGAM_INFO("    %d:    %s\n", i + 1,
				  list_info->namelist[i].name);
		}
	}

	if (list_info)
		free(list_info);

	return ret;
}

static int parse_del_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK;
	int arg_count = 0;
	struct sgam_del_info *del_info =
		(struct sgam_del_info *)calloc(1, sizeof(struct sgam_del_info));

	if (!del_info) {
		SGAM_ERR("Failed to alloc memory!\n");
		return SGAM_CMD_ERR;
	}
	*pdata = del_info;

	while (popts->opt) {
		switch (popts->opt) {
		case 'n':
			ret = sgam_copy_label(del_info->name, popts->optarg);
			arg_count++;
			break;

		default:
			ret = SGAM_CMD_ERR;
		}
		if (ret != SGAM_CMD_OK)
			return ret;
		popts++;
	}

	if (arg_count < 1) {
		SGAM_ERR("Name not passed!!\n");
		ret = SGAM_CMD_HELP;
	}

	return ret;
}

static void sgam_del_print_data(void *pdata, int ret_val)
{
	struct sgam_del_info *del_info = (struct sgam_del_info *)pdata;

	if (ret_val == EBUSY) {
		SGAM_INFO("There are %u iptable entries associated with %s!!\n"
			  "Please delete them before deleting the %s: %s\n",
			  del_info->ref_count, del_info->name,
			  IS_ACCT_DB(del_info->db_flag) ? "account" : "meter",
			  del_info->name);

	} else if (ret_val != SGAM_CMD_OK) {
		SGAM_INFO("No %s exists with the name: %s\n",
			  IS_ACCT_DB(del_info->db_flag) ? "account" : "meter",
			  del_info->name);
	} else if (del_info) {
		SGAM_INFO("Successfully deleted: %s\n", del_info->name);
	}
}

/* Accounts related operations */
static const char sgam_add_acct_short_opts[] = "-:g:n:h";
static const struct option sgam_add_acct_long_opts[] = {
	{"help", no_argument,  NULL, 'h'},
	{"name", required_argument,  NULL, 'n'},
	{"group", required_argument,  NULL, 'g'},
	{0, 0, 0, 0}
};

static void sgam_add_acct_help(int summary)
{
	if (summary) {
		SGAM_INFO("addacct -n <new account name> -g <group number>\n"
			  "    -n / --name: specify the name of the new"
			  " account you want to create\n"
			  "    -g / --group: specify the group number (1 or 2)"
			  " to which you want to add this account\n");
	} else {
		SGAM_INFO("    addacct -n <new account name>"
			  " -g <group number>\n");
	}
}

static int sgam_parse_add_acct_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK;
	int arg_count = 0;
	struct sgam_acct_info *acct_info =
		(struct sgam_acct_info *)calloc(1, sizeof(struct sgam_acct_info));

	if (!acct_info) {
		SGAM_ERR("Failed to alloc memory!\n");
		return SGAM_CMD_ERR;
	}
	*pdata = acct_info;


	while (popts->opt) {
		switch (popts->opt) {
		case 'n':
			ret = sgam_copy_label(acct_info->name, popts->optarg);
			break;

		case 'g':
			acct_info->sgc_grp = atoi(popts->optarg);
			break;

		default:
			ret = SGAM_CMD_ERR;
		}
		if (ret != SGAM_CMD_OK)
			return ret;
		popts++;
		arg_count++;
	}

	if (arg_count < 2) {
		SGAM_ERR("Missing opt/arg!!\n");
		ret = SGAM_CMD_HELP;
	}

	return ret;
}

static void sgam_add_acct_print_data(void *pdata, int ret_val)
{
	struct sgam_acct_info *acct_info = (struct sgam_acct_info *)pdata;

	if (ret_val == EEXIST) {
		SGAM_INFO("Account already exists for: %s\n", acct_info->name);
	} else if (ret_val != SGAM_CMD_OK) {
		SGAM_ERR("Failed to create the account: %s!!\n",
			  acct_info->name);
	} else if (acct_info) {
		SGAM_INFO("Successfully created the account: %s\n",
			  acct_info->name);
	}
}

static void sgam_show_acct_help(int summary)
{
	if (summary) {
		SGAM_INFO("showacct -n <account name>\n"
			  "    -n / --name: Name of the account whose stats"
			  " you want to see\n");
	} else {
		SGAM_INFO("    showacct -n <account name>\n");
	}
}

static int sgam_parse_show_acct_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK, arg_count = 0;
	struct sgam_cmd_acct_stats *stats_info =
		(struct sgam_cmd_acct_stats *)calloc(1, sizeof(struct sgam_cmd_acct_stats));

	if (!stats_info) {
		SGAM_ERR("Failed to alloc memory!\n");
		return SGAM_CMD_ERR;
	}
	*pdata = stats_info;

	while (popts->opt) {
		switch (popts->opt) {
		case 'n':
			ret = sgam_copy_label(stats_info->name, popts->optarg);
			arg_count++;
			break;

		default:
			ret = SGAM_CMD_ERR;
		}
		popts++;
	}

	if (arg_count < 1) {
		SGAM_ERR("Name not passed!!\n");
		ret = SGAM_CMD_HELP;
	}

	return ret;
}

static void sgam_show_acct_print_data(void *pdata, int ret_val)
{
	struct sgam_cmd_acct_stats *stats_info = (struct sgam_cmd_acct_stats *)pdata;

	if (ret_val != SGAM_CMD_OK) {
		SGAM_INFO("No account exists with the name: %s!!\n",
			  stats_info->name);
	} else if (stats_info) {
		SGAM_INFO("Below is the statistic of the group account: %s\n"
			  "    Account pool ID       : %u\n"
			  "    Total no. of packets  : %lld\n"
			  "    Total no. of bytes    : %lld Bytes\n",
			  stats_info->name,
			  stats_info->sgc_grp,
			  stats_info->acct_stat.packets,
			  stats_info->acct_stat.bytes);
	}
}

static void sgam_list_acct_help(int summary)
{
	if (summary) {
		SGAM_INFO("listacct\n"
			  "    For listing all configured group accounts\n");
	} else {
		SGAM_INFO("    listacct\n");
	}
}

static int sgam_parse_list_acct_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	return parse_list_cmd(true, pdata);
}

static void sgam_del_acct_help(int summary)
{
	if (summary) {
		SGAM_INFO("delacct -n <account name>\n"
			  "    -n / --name: specify the name of the account"
			  " you want to delete from database\n");
	} else {
		SGAM_INFO("    delacct -n <account name>\n");
	}
}

static int sgam_parse_del_acct_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK;
	struct sgam_del_info *del_info = NULL;

	ret = parse_del_cmd(popts, pdata);

	if (ret != SGAM_CMD_OK)
		return ret;

	if (*pdata) {
		del_info = (struct sgam_del_info *)*pdata;
		del_info->db_flag |= SGAM_ACCT_DB_FLAG;
	}

	return ret;
}

/* Meter related operations */
static const char sgam_add_meter_short_opts[] = "-:n:m:c:p:C:P:R:h";
static const struct option sgam_add_meter_long_opts[] = {
	{"help", no_argument,  NULL, 'h'},
	{"name", required_argument,  NULL, 'n'},
	{"mode", required_argument,  NULL, 'm'},
	{"cir", required_argument,  NULL, 'c'},
	{"pir", required_argument,  NULL, 'p'},
	{"cbs", required_argument,  NULL, 'C'},
	{"pbs", required_argument,  NULL, 'P'}, /* Capital P */
	{"remark-dscp", required_argument,  NULL, 'R'},
	{0, 0, 0, 0}
};

static void sgam_add_meter_help(int summary)
{
	if (summary) {
		SGAM_INFO("addmeter -n <name> -m <meter mode>"
			  " -c <CIR in K or M bits/sec > -C <CBS in bytes >"
			  " -p <PIR in K or M bits/sec> -P <PBS in bytes>\n"
			  "    -n / --name: Name of the new meter you want"
			  " to create\n"
			  "    -m / --mode: Meter mode. Below are the"
			  " supported modes\n"
			  "                 trtcm_ca  -> RFC 2698 Color"
			  " aware mode\n"
			  "                 trtcm_cb  -> RFC 2698 Color"
			  " blind mode\n"
			  "                 dtrtcm_ca -> RFC 4115 Color"
			  " aware mode\n"
			  "                 dtrtcm_cb -> RFC 4115 Color"
			  " blind mode\n"
			  "    -c / --cir : Committed Information Rate in"
			  " Kilo or Mega bits/sec\n"
			  "    -C / --cbs : Committed Burst Size in bytes/kilobytes\n"
			  "    -p / --pir : Peak Information Rate in"
			  " Kilo or Mega bits/sec\n"
			  "    -P / --pbs : Peak Burst Size in bytes/kilobytes\n"
			  "    -R / --remark-dscp : DSCP remarking for yellow pkts, "
			  "accepted values <0 to 63> eg: 10 for AF11\n");
		SGAM_INFO("Example:\n"
			  "    sgam_cli addmeter -n meter_tca_100m -m trtcm_ca"
			  " -c 100m -C 90k -p 200m -P 200k -R 10\n");

	} else {
		SGAM_INFO("    addmeter -n <name> -m <meter mode>"
			  " -c <CIR in K or M bps > -C <CBS in bytes >"
			  " -p <PIR in K or M bps> -P <PBS in bytes>\n");
	}
}

static int set_meter_mode(enum sgam_meter_types *mode, char *arg)
{
	int ret = SGAM_CMD_OK;

	if (!strncmp("trtcm_ca", arg, sizeof("trtcm_ca"))) {
		*mode = SGAM_TBM_MODE_2698_CA;
	} else if (!strncmp("trtcm_cb", arg, sizeof("trtcm_cb"))) {
		*mode = SGAM_TBM_MODE_2698_CB;
	} else if (!strncmp("dtrtcm_ca", arg, sizeof("dtrtcm_ca"))) {
		*mode = SGAM_TBM_MODE_4115_CA;
	} else if (!strncmp("dtrtcm_cb", arg, sizeof("dtrtcm_cb"))) {
		*mode = SGAM_TBM_MODE_4115_CB;
	} else {
		SGAM_ERR("Invalid meter mode: %s!!\n", arg);
		ret = SGAM_CMD_HELP;
	}

	return ret;
}

static int set_dscp_remarking(unsigned char *remark_dscp, char *arg)
{
	unsigned int tmp_dscp = 0;
	char *tmp_arg = arg;

	/* Check if the string is empty */
	if (*tmp_arg == '\0')
		goto ERROR;

	/* Check if string has non-digit characters */
	while (*tmp_arg != '\0') {
		if (!isdigit(*tmp_arg))
			goto ERROR;
		tmp_arg++;
	}

	/* Convert the string to an integer and check if it exceeds the max_dscp */
	tmp_dscp = strtoul(arg, NULL, 10);
	if (tmp_dscp >= MAX_DSCP)
		goto ERROR;

	*remark_dscp = tmp_dscp;

	return SGAM_CMD_OK;

ERROR:
	SGAM_ERR("ERROR: Invalid remark_dscp value\n");
	return SGAM_CMD_ERR;
}

/* Convert rate to just Bytes/sec from Kilo or Mega bits/sec */
static int set_meter_cir_pir(long long unsigned int *rate, char *arg)
{
	long long unsigned int tmp_rate = 0;
	int idx = strlen(arg) - 1;

	if (idx < 0)
		return SGAM_CMD_ERR;

	tmp_rate = atol(arg);
	switch (arg[idx]) {
	/* Convert Kbps to bytes/sec */
	case 'k':
		__attribute__ ((fallthrough));
	case 'K':
		if (tmp_rate <= 0 || tmp_rate >=  MAX_RATE_K) {
			SGAM_ERR("Invalid rate: %s!!\n", arg);
			SGAM_ERR("Valid rate is: 0 < rate < %u Kbps\n",
				 MAX_RATE_K);
			return SGAM_CMD_ERR;
		}
		*rate = tmp_rate * 125; /* (1000/8) */
		break;

	/* Convert Mbps to bytes/sec */
	case 'm':
		__attribute__ ((fallthrough));
	case 'M':
		if (tmp_rate <= 0 || tmp_rate >=  MAX_RATE_M) {
			SGAM_ERR("Invalid rate: %s!!\n", arg);
			SGAM_ERR("Valid rate is: 0 < rate < %u Mbps\n",
				 MAX_RATE_M);
			return SGAM_CMD_ERR;
		}
		*rate = tmp_rate * 125000; /* ((1000*1000)/8) */
		break;

	default:
		SGAM_ERR("Please use only one of these for specifying rate:\n"
			 "           k/K for Kilo bits per second,"
			 " m/M for Mega bits per second\n");
		return SGAM_CMD_ERR;
	}

	return SGAM_CMD_OK;
}


static int set_meter_cbs_pbs(unsigned int *b_size, char *arg)
{
	int mul = -1, ret = SGAM_CMD_OK;
	unsigned int tmp_size = 0;
	int idx = strlen(arg) - 1; /* Index of last char */

	if (idx < 0)
		return SGAM_CMD_ERR;

	tmp_size = atoi(arg);
	/* If last char is 'K' or 'k', then the size paased is in KB.
	 * Convert it to just Bytes.
	 */
	if (isalpha(arg[idx])) {
		if (arg[idx] == 'k' || arg[idx] == 'K')
			mul = 1024;
		else
			mul = -1;
	} else if (isdigit(arg[idx])) {
		mul = 1;
	} else {
		mul = -1;
	}

	if (mul == -1) {
		SGAM_ERR("Please use either k/K for specifying the burst size "
			 "in Kilobytes or pass the exact numbers in bytes\n");
		ret = SGAM_CMD_ERR;
	} else if (mul == 1024 && (tmp_size >= MAX_BURST_K || tmp_size <= 0)) {
		SGAM_ERR("Invalid burst size: %s!!\n", arg);
		SGAM_ERR("Valid burst size is: 0 < burst size < %d Kilo Bytes\n",
			 MAX_BURST_K);
		ret = SGAM_CMD_ERR;

	} else if (mul == 1 && (tmp_size >= MAX_BURST || tmp_size <= 0)) {
		SGAM_ERR("Invalid burst size: %s!!\n", arg);
		SGAM_ERR("Valid burst size is: 0 < burst size < %d Bytes\n",
			 MAX_BURST);
		ret = SGAM_CMD_ERR;
	} else {
		/* Convert burst size to just Bytes from Kilo Bytes */
		*b_size = (tmp_size * mul);
		ret = SGAM_CMD_OK;
	}

	return ret;
}

static int sgam_parse_add_meter_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK;
	int arg_count = 0;
	struct sgam_meter_info *meter_info =
		(struct sgam_meter_info *)calloc(1, sizeof(struct sgam_meter_info));

	if (!meter_info) {
		SGAM_ERR("Failed to alloc memory!\n");
		return SGAM_CMD_ERR;
	}
	*pdata = meter_info;
	meter_info->tbm_id = -1; /* Set an invalid TBM ID */

	while (popts->opt) {
		switch (popts->opt) {
		case 'n':
			ret = sgam_copy_label(meter_info->name, popts->optarg);
			arg_count++;
			break;

		case 'm':
			ret = set_meter_mode(&meter_info->mtr_type,
					     popts->optarg);
			arg_count++;
			break;

		case 'c':
			ret = set_meter_cir_pir(&meter_info->cir,
						popts->optarg);
			arg_count++;
			break;

		case 'p':
			ret = set_meter_cir_pir(&meter_info->pir,
						popts->optarg);
			arg_count++;
			break;

		case 'C':
			ret = set_meter_cbs_pbs(&meter_info->cbs,
						popts->optarg);
			arg_count++;
			break;

		case 'P':
			ret = set_meter_cbs_pbs(&meter_info->pbs,
						popts->optarg);
			arg_count++;
			break;

		case 'R':
			ret = set_dscp_remarking(&meter_info->remark_dscp, popts->optarg);
			arg_count++;
			break;

		default:
			ret = SGAM_CMD_ERR;
		}
		if (ret != SGAM_CMD_OK)
			return ret;
		popts++;
	}

	if (arg_count < 6) {
		SGAM_ERR("Missing opt/arg!!\n");
		ret = SGAM_CMD_HELP;
	} else if (arg_count == 6) {
		/* Set default remark_dscp to invalid if it is not set by user */
		meter_info->remark_dscp = MAX_DSCP;
	} else if ((meter_info->pir < meter_info->cir) ||
		   (meter_info->pbs < meter_info->cbs)) {
		SGAM_ERR("PIR and PBS cannot be less than CIR and CBS!!\n");
		ret = SGAM_CMD_ERR;
	}

	return ret;
}

static void sgam_add_meter_print_data(void *pdata, int ret_val)
{
	struct sgam_meter_info *meter_info = (struct sgam_meter_info *)pdata;

	if (ret_val == EEXIST) {
		SGAM_INFO("Meter already exists for: %s\n", meter_info->name);
	} else if (ret_val != SGAM_CMD_OK) {
		SGAM_INFO("Failed to create the meter: %s!!\n",
			  meter_info->name);
	} else if (meter_info) {
		SGAM_INFO("Successfully created the meter: %s\n",
			  meter_info->name);
	}
}

static void sgam_show_meter_help(int summary)
{
	if (summary) {
		SGAM_INFO("showmeter -n <meter name>\n"\
			  "    -n / --name: Name of the meter whose"
			  " config you want to see\n");
	} else {
		SGAM_INFO("    showmeter -n <meter name>\n");
	}
}

static int sgam_parse_show_meter_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK, arg_count = 0;
	struct sgam_meter_info *meter_info =
		(struct sgam_meter_info *)calloc(1, sizeof(struct sgam_meter_info));

	if (!meter_info) {
		SGAM_ERR("Failed to alloc memory!\n");
		return SGAM_CMD_ERR;
	}
	*pdata = meter_info;
	meter_info->tbm_id = -1; /* Set an invalid TBM ID */

	while (popts->opt) {
		switch (popts->opt) {
		case 'n':
			ret = sgam_copy_label(meter_info->name, popts->optarg);
			arg_count++;
			break;

		default:
			ret = SGAM_CMD_ERR;
		}
		popts++;
	}

	if (arg_count < 1) {
		SGAM_ERR("Name not passed!!\n");
		ret = SGAM_CMD_HELP;
	}

	return ret;
}

static void sgam_show_meter_print_data(void *pdata, int ret_val)
{
	struct sgam_meter_info *meter_info = (struct sgam_meter_info *)pdata;

	if (ret_val != SGAM_CMD_OK) {
		SGAM_INFO("No meter exists with the name: %s!!\n",
			  meter_info->name);
	} else if (meter_info && meter_info->tbm_id != -1) {
		SGAM_INFO("Below is the configurations of the meter: %s\n"
			  "    cir        : %lld Kbps\n"
			  "    pir        : %lld Kbps\n"
			  "    cbs        : %d Bytes\n"
			  "    pbs        : %d Bytes\n"
			  "    tbm_id     : %hu\n",
			  meter_info->name, (meter_info->cir * 8 / 1000),
			  (meter_info->pir * 8 / 1000), meter_info->cbs,
			  meter_info->pbs, meter_info->tbm_id);
		if (meter_info->remark_dscp < MAX_DSCP)
			SGAM_INFO("    remark-dscp: %hhu\n", meter_info->remark_dscp);
		if (meter_info->mtr_type >= SGAM_TBM_MODE_FIRST &&
		    meter_info->mtr_type <= SGAM_TBM_MODE_LAST) {
			SGAM_INFO("    meter type : %s\n",
				  g_meter_modes[meter_info->mtr_type]);
		} else { /* Unliikely */
			SGAM_ERR("Invalid meter type!!\n");
		}
	}
}

static void sgam_list_meter_help(int summary)
{
	if (summary) {
		SGAM_INFO("listmeter\n"
			  "    For listing all configured meters\n");
	} else {
		SGAM_INFO("    listmeter\n");
	}
}

static int sgam_parse_list_meter_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	return parse_list_cmd(false, pdata);
}

static void sgam_del_meter_help(int summary)
{
	if (summary) {
		SGAM_INFO("delmeter -n <meter name>\n"
			  "    -n / --name: specify the name of the meter"
			  " you want to delete from database\n");
	} else {
		SGAM_INFO("    delmeter -n <meter name>\n");
	}
}

static int sgam_parse_del_meter_cmd(SGAM_CMD_OPTS *popts, void **pdata)
{
	int ret = SGAM_CMD_OK;
	struct sgam_del_info *del_info = NULL;

	ret = parse_del_cmd(popts, pdata);

	if (ret != SGAM_CMD_OK)
		return ret;

	if (*pdata) {
		del_info = (struct sgam_del_info *)*pdata;
		del_info->db_flag |= SGAM_METER_DB_FLAG;
	}

	return ret;
}

static SGAM_COMMAND g_sgam_cmd[] = {
	{
		.name = "addacct",
		.ioctl_cmd = SGAM_ADD_ACCOUNT,
		.print_help = sgam_add_acct_help,
		.parse_options = sgam_parse_add_acct_cmd,
		.do_command = sgam_generic_do_cmd,
		.print_data = sgam_add_acct_print_data,
		.long_opts = sgam_add_acct_long_opts,
		.short_opts = sgam_add_acct_short_opts
	},
	{
		.name = "showacct",
		.ioctl_cmd = SGAM_SHOW_ACCOUNT,
		.print_help = sgam_show_acct_help,
		.parse_options = sgam_parse_show_acct_cmd,
		.do_command = sgam_generic_do_cmd,
		.print_data = sgam_show_acct_print_data,
		.long_opts = sgam_show_cmd_long_opts,
		.short_opts = sgam_show_cmd_short_opts
	},
	{
		.name = "listacct",
		.ioctl_cmd = SGAM_LIST_CMD, /* Place holder, not used */
		.print_help = sgam_list_acct_help,
		.parse_options = sgam_parse_list_acct_cmd,
		.do_command = sgam_list_do_cmd,
		.print_data = NULL,
		.long_opts = sgam_list_long_opts,
		.short_opts = sgam_list_short_opts
	},
	{
		.name = "delacct",
		.ioctl_cmd = SGAM_DEL_CMD,
		.print_help = sgam_del_acct_help,
		.parse_options = sgam_parse_del_acct_cmd,
		.do_command = sgam_generic_do_cmd,
		.print_data = sgam_del_print_data,
		.long_opts = sgam_del_long_opts,
		.short_opts = sgam_del_short_opts
	},
	{
		.name = "addmeter",
		.ioctl_cmd = SGAM_ADD_METER,
		.print_help = sgam_add_meter_help,
		.parse_options = sgam_parse_add_meter_cmd,
		.do_command = sgam_generic_do_cmd,
		.print_data = sgam_add_meter_print_data,
		.long_opts = sgam_add_meter_long_opts,
		.short_opts = sgam_add_meter_short_opts
	},
	{
		.name = "showmeter",
		.ioctl_cmd = SGAM_SHOW_METER,
		.print_help = sgam_show_meter_help,
		.parse_options = sgam_parse_show_meter_cmd,
		.do_command = sgam_generic_do_cmd,
		.print_data = sgam_show_meter_print_data,
		.long_opts = sgam_show_cmd_long_opts,
		.short_opts = sgam_show_cmd_short_opts
	},
	{
		.name = "listmeter",
		.ioctl_cmd = SGAM_LIST_CMD, /* Place holder, not used */
		.print_help = sgam_list_meter_help,
		.parse_options = sgam_parse_list_meter_cmd,
		.do_command = sgam_list_do_cmd,
		.print_data = NULL,
		.long_opts = sgam_list_long_opts,
		.short_opts = sgam_list_short_opts
	},
	{
		.name = "delmeter",
		.ioctl_cmd = SGAM_DEL_CMD,
		.print_help = sgam_del_meter_help,
		.parse_options = sgam_parse_del_meter_cmd,
		.do_command = sgam_generic_do_cmd,
		.print_data = sgam_del_print_data,
		.long_opts = sgam_del_long_opts,
		.short_opts = sgam_del_short_opts
	},
	/* The below entry must be kept as last entry */
	{
		.name = NULL,
		.ioctl_cmd = 0,
		.print_help = NULL,
		.parse_options = NULL,
		.do_command = NULL,
		.print_data = NULL,
		.long_opts = NULL,
		.short_opts = NULL
	}
};

static void sgam_print_help(void)
{
	SGAM_COMMAND *pcmd;


	SGAM_INFO("Usage: %s <command> {options}\n Commands:\n",
		  SGAM_CMD_NAME);

	for (pcmd = g_sgam_cmd; pcmd->name != NULL; pcmd++) {
		if (pcmd->print_help)
			(*pcmd->print_help)(0);
	}
}

static void sgam_print_cmd_help(SGAM_COMMAND *pcmd)
{
	if (pcmd->print_help) {
		SGAM_INFO("Usage: %s ", SGAM_CMD_NAME);
		(*pcmd->print_help)(1);
	}
}

static int get_sgam_cmd(char *cmd_str, SGAM_COMMAND **pcmd)
{
	int i;
	/* Locate the command where the name matches the cmd_str and return
	 * the matching command ops in the command array.
	 */
	for (i = 0; g_sgam_cmd[i].name; i++) {
		if (strcmp(cmd_str, g_sgam_cmd[i].name) == 0) {
			*pcmd = &g_sgam_cmd[i];
			return SGAM_CMD_OK;
		}
	}

	return SGAM_CMD_ERR;
}

static int sgam_parse_cmd(int ac, char **av, SGAM_COMMAND *pcmd,
			  SGAM_CMD_OPTS *popts)
{
	int opt, opt_idx, ret = SGAM_CMD_OK;
	int num_opts;

	/* Fill out the SGAM_CMD_OPTS array with the option value and argument
	 * for each option that is found. If option is help, display command
	 * help and do not process command.
	 */
	for (num_opts = 0; num_opts < SGAM_MAX_CMD_OPTS; num_opts++) {
		opt = getopt_long(ac - 1, av + 1, pcmd->short_opts,
				  pcmd->long_opts, &opt_idx);
		if (opt == -1) {
			break;
		} else if (opt == 1) {
			SGAM_ERR("Invalid arg: %s!!\n", optarg);
			ret = SGAM_CMD_ERR;
			break;
		} else if (opt == 'h')	{
			ret = SGAM_CMD_HELP;
			break;
		} else if (opt == '?') {
			ret = SGAM_CMD_ERR;
			break;
		}
		popts->opt  = opt;
		popts->optarg = optarg;
		popts++;
	}

	return ret;
}

static int sgam_parse_cmd_line(int ac, char **av, SGAM_COMMAND **pcmd,
			       void **data)
{
	int ret = 0;
	SGAM_CMD_OPTS *popts = NULL;

	if ((ac <= 1) || (av == NULL))
		return SGAM_CMD_HELP;

	popts = (SGAM_CMD_OPTS *)calloc(SGAM_MAX_CMD_OPTS, sizeof(SGAM_CMD_OPTS));
	if (!popts) {
		SGAM_ERR("Unable to allocate memory for opts!!\n");
		return SGAM_CMD_NOT_AVAIL;
	}

	/* Get the matching command from command table */
	ret = get_sgam_cmd(av[1], pcmd);
	if (ret == SGAM_CMD_OK) {
		ret = sgam_parse_cmd(ac, av, *pcmd, popts);
		if (ret == SGAM_CMD_OK)
			ret = (*pcmd)->parse_options(popts, data);
		else if (ret == SGAM_CMD_ERR)
			ret = SGAM_CMD_HELP;
	} else {
		SGAM_ERR("Unknown command:  %s\n", av[1]);
	}

	free(popts);
	if (ret != SGAM_CMD_OK && *data) {
		free(*data);
		*data = NULL;
	}
	return ret;
}

int main(int argc, char **argv)
{
	int ret = 0;
	void *pdata = NULL;
	SGAM_COMMAND *pcmd = NULL;

	if (argc == 1) {
		sgam_print_help();
		return 0;
	}

	ret = sgam_parse_cmd_line(argc, argv,  &pcmd, &pdata);
	if (ret == SGAM_CMD_OK) {
		if (pcmd->do_command) {
			ret = pcmd->do_command(pcmd, pdata);
			if (pcmd->print_data)
				pcmd->print_data(pdata, ret);
		}
	} else if (ret == SGAM_CMD_HELP) {
		sgam_print_cmd_help(pcmd);
	}

	if (pdata) {
		free(pdata);
		pdata = NULL;
	}

	return ret;
}
