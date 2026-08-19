/******************************************************************************
 *
 *  Copyright (c) 2020 - 2023 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include "fapi_pon.h"
#include "fapi_pon_os.h"
#include "pon_cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <inttypes.h>

#define MAX_FILENAME_LEN 128
#define MAX_BIT_STRING 32
/* Maximum size of binary output (bits + spaces) */
#define MAX_BIN_STRING 40

static char *dec_to_bin(uint32_t data, uint32_t size)
{
	int i;
	int pos = 0;
	/* space_bit shows places in binary output where space
	 * character should be added.
	 */
	int space_bit = 5;
	char *output_num = malloc(MAX_BIN_STRING);

	if (!output_num) {
		fprintf(stderr, "Cannot alloc memory\n");
		return NULL;
	}

	/* check if size is not divisible by 4 to allocate enough memory to
	 * binary output
	 */
	if (size % 4 != 0)
		space_bit = size % 4 + 1;

	for (i = size - 1; i >= 0; i--) {
		if ((data >> i) & 1)
			output_num[pos++] = '1';
		else
			output_num[pos++] = '0';
		if ((pos + 1) % space_bit == 0 &&
		    (pos + 1) != MAX_BIN_STRING) {
			output_num[pos++] = ' ';
			space_bit += 5;
		}
	}
	output_num[pos] = '\0';

	return output_num;
}

static uint32_t mask(uint32_t start_bit, uint32_t stop_bit)
{
	int i = 0;
	uint32_t mask = 0;

	if (start_bit > 31 || stop_bit > 31) {
		fprintf(stderr,
			"Start bit or stop bit is out of range - cannot return binary data\n");
		return 0;
	}

	if (start_bit < stop_bit) {
		fprintf(stderr,
			"Start bit must be higher than stop bit - cannot return binary data\n");
		return 0;
	}

	for (i = start_bit; i >= (int) stop_bit; i--)
		mask |= 1U << i;

	return mask;
}

static uint32_t parse_input(char *data, uint32_t start_bit, uint32_t stop_bit)
{
	int i;
	uint32_t num;
	int pos = 0;
	int num_of_bits;
	char output_num[MAX_BIT_STRING + 1] = { 0 };
	uint64_t power_max;
	char *pEnd = NULL;
	int ret = 0;

	if (start_bit > 31 || stop_bit > 31) {
		fprintf(stderr,
			"Start bit or stop bit is out of range - cannot parse input data\n");
		return 0;
	}

	if (start_bit < stop_bit) {
		fprintf(stderr,
			"Start bit must be higher than stop bit - cannot parse input data\n");
		return 0;
	}

	num_of_bits = start_bit - stop_bit;
	power_max = (uint64_t)1 << (num_of_bits + 1);

	if (data[1] == 'b')
		num = strtol(data + 2, &pEnd, 2);
	else if (data[1] == 'x')
		num = strtol(data + 2, &pEnd, 16);
	else
		num = strtol(data, &pEnd, 10);

	if (num > power_max - 1) {
		fprintf(stderr, "Data value is wrong and cannot be set\n");
		return 0;
	};

	for (i = num_of_bits; i >= 0; i--) {
		if ((num >> i) & 1)
			output_num[pos++] = '1';
		else
			output_num[pos++] = '0';
	}

	for (i = stop_bit - 1; i >= 0; i--)
		output_num[pos++] = '0';

	output_num[pos] = '\0';

	for (i = 0; i < (int)strnlen_s(output_num, MAX_BIT_STRING + 1); ++i) {
		ret *= 2;
		if (output_num[i] == '1')
			ret += 1;
	}

	return ret;
}

#ifdef INCLUDE_CLI_SUPPORT

#include "lib_cli_config.h"
#include "lib_cli_core.h"

#define FAPI_PON_CRLF "\n"

/** \addtogroup FAPI_PON_CLI_COMMANDS
   @{
*/

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_bit_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	uint32_t dst_addr = 0;
	uint32_t start_bit = 0;
	uint32_t stop_bit = 0;
	uint32_t bit_mask = 0;
	char *bin = NULL;
	struct pon_register param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: bit_get" FAPI_PON_CRLF
		"Short Form: bg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- uint32_t dst_addr" FAPI_PON_CRLF
		"- uint32_t start_bit" FAPI_PON_CRLF
		"- uint32_t stop_bit" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint32_t addr" FAPI_PON_CRLF
		"- uint32_t data" FAPI_PON_CRLF
		"- char* bin" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = cli_sscanf(p_cmd, "%u %u %u", &dst_addr, &start_bit, &stop_bit);
	if (ret != 3)
		return cli_check_help__file("-h", usage, p_out);
	fct_ret = fapi_pon_register_get(p_ctx, dst_addr, &param);

	bit_mask = mask(start_bit, stop_bit);
	if (bit_mask != 0) {
		param.data = param.data & bit_mask;
		param.data = param.data >> stop_bit;
		bin = dec_to_bin(param.data, start_bit - stop_bit + 1);
	} else {
		return 0;
	}

	if (bin != NULL) {
		fprintf(p_out, "errorcode=%d addr=0x%x data=0x%x bin=%s %s",
		(int)fct_ret, param.addr, param.data, bin, FAPI_PON_CRLF);
		free(bin);
	} else {
		fprintf(p_out, "errorcode=%d addr=0x%x data=0x%x bin=0 %s",
		(int)fct_ret, param.addr, param.data, FAPI_PON_CRLF);
	}

	return 0;
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_bit_set(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	uint32_t start_bit = 0;
	uint32_t stop_bit = 0;
	char data[35];
	struct pon_register param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: bit_set" FAPI_PON_CRLF
		"Short Form: bs" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- uint32_t addr" FAPI_PON_CRLF
		"- uint32_t start_bit" FAPI_PON_CRLF
		"- uint32_t stop_bit" FAPI_PON_CRLF
		"- char data[35]" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = sscanf_s(p_cmd, "%x %u %u %34s", &param.addr, &start_bit,
			 &stop_bit, SSCANF_STR(data, sizeof(data)));
	if (ret != 4)
		return cli_check_help__file("-h", usage, p_out);

	param.data = parse_input(data, start_bit, stop_bit);

	fct_ret = fapi_pon_register_set(p_ctx, &param);
	return fprintf(p_out, "errorcode=%d %s", (int)fct_ret, FAPI_PON_CRLF);
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_register_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	uint32_t dst_addr = 0;
	char *bin = NULL;
	struct pon_register param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: register_get" FAPI_PON_CRLF
		"Short Form: rg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- uint32_t dst_addr" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint32_t addr" FAPI_PON_CRLF
		"- uint32_t data" FAPI_PON_CRLF
		"- char* bin" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = cli_sscanf(p_cmd, "%u", &dst_addr);
	if (ret != 1)
		return cli_check_help__file("-h", usage, p_out);
	fct_ret = fapi_pon_register_get(p_ctx, dst_addr, &param);

	bin = dec_to_bin(param.data, 32);

	if (bin != NULL) {
		fprintf(p_out, "errorcode=%d addr=0x%x data=0x%x bin=%s %s",
		(int)fct_ret, param.addr, param.data, bin, FAPI_PON_CRLF);
		free(bin);
	} else {
		fprintf(p_out, "errorcode=%d addr=0x%x data=0x%x bin=0 %s",
		(int)fct_ret, param.addr, param.data, FAPI_PON_CRLF);
	}

	return 0;
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_omci_cfg_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_omci_cfg param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: omci_cfg_get" FAPI_PON_CRLF
		"Short Form: " CLI_EMPTY_CMD_HELP FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint8_t mac_sa[6]" FAPI_PON_CRLF
		"- uint8_t mac_da[6]" FAPI_PON_CRLF
		"- uint16_t ethertype" FAPI_PON_CRLF
		"- uint8_t protocol[5]" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_omci_cfg_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d mac_sa=\"%X:%X:%X:%X:%X:%X\" mac_da=\"%X:%X:%X:%X:%X:%X\" ethertype=0x%hX protocol=\"0x%X 0x%X 0x%X 0x%X 0x%X\" %s",
		(int)fct_ret, param.mac_sa[0], param.mac_sa[1],
		param.mac_sa[2], param.mac_sa[3], param.mac_sa[4],
		param.mac_sa[5], param.mac_da[0], param.mac_da[1],
		param.mac_da[2], param.mac_da[3], param.mac_da[4],
		param.mac_da[5], param.ethertype, param.protocol[0],
		param.protocol[1], param.protocol[2], param.protocol[3],
		param.protocol[4], FAPI_PON_CRLF);
}

static int cli_fapi_pon_omci_cfg_set(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_omci_cfg param = {0};
	uint32_t mac_sa[6];
	uint32_t mac_da[6];
	uint32_t protocol[5];
	int i;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: omci_cfg_set" FAPI_PON_CRLF
		"Short Form: " FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- uint8_t mac_sa[6]" FAPI_PON_CRLF
		"- uint8_t mac_da[6]" FAPI_PON_CRLF
		"- uint16_t ethertype" FAPI_PON_CRLF
		"- uint8_t protocol[5]" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = sscanf_s(p_cmd, "%x:%x:%x:%x:%x:%x %x:%x:%x:%x:%x:%x %hx %x %x %x %x %x",
			 &mac_sa[0], &mac_sa[1], &mac_sa[2], &mac_sa[3],
			 &mac_sa[4], &mac_sa[5], &mac_da[0], &mac_da[1],
			 &mac_da[2], &mac_da[3], &mac_da[4], &mac_da[5],
			 &param.ethertype, &protocol[0], &protocol[1],
			 &protocol[2], &protocol[3], &protocol[4]);
	if (ret != 18)
		return cli_check_help__file("-h", usage, p_out);

	for (i = 0; i < 6; i++) {
		param.mac_sa[i] = (uint8_t)mac_sa[i];
		param.mac_da[i] = (uint8_t)mac_da[i];
		if (i < 5)
			param.protocol[i] = (uint8_t)protocol[i];
	}

	fct_ret = fapi_pon_omci_cfg_set(p_ctx, &param);
	return fprintf(p_out, "errorcode=%d %s",
		(int)fct_ret, FAPI_PON_CRLF);
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_gpon_cfg_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_gpon_cfg param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: gpon_cfg_get" FAPI_PON_CRLF
		"Short Form: gcg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- enum pon_mode mode" FAPI_PON_CRLF
		"   PON_MODE_UNKNOWN = 0" FAPI_PON_CRLF
		"   PON_MODE_984_GPON = 1" FAPI_PON_CRLF
		"   PON_MODE_987_XGPON = 2" FAPI_PON_CRLF
		"   PON_MODE_9807_XGSPON = 3" FAPI_PON_CRLF
		"   PON_MODE_989_NGPON2_2G5 = 4" FAPI_PON_CRLF
		"   PON_MODE_989_NGPON2_10G = 5" FAPI_PON_CRLF
		"- uint8_t serial_no[8]" FAPI_PON_CRLF
		"- char password[10]" FAPI_PON_CRLF
		"- uint8_t reg_id[36]" FAPI_PON_CRLF
		"- uint32_t ident" FAPI_PON_CRLF
		"- uint32_t stop" FAPI_PON_CRLF
		"- uint32_t plev_cap" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_0" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_1" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_2" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_3" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_4" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_5" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_6" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_cpl" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_cpi" FAPI_PON_CRLF
		"- uint32_t ploam_timeout_tpd" FAPI_PON_CRLF
		"- uint32_t tdm_coexistence" FAPI_PON_CRLF
		"- uint32_t dg_dis" FAPI_PON_CRLF
		"- uint32_t ds_fcs_en" FAPI_PON_CRLF
		"- uint32_t ds_ts_dis" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_gpon_cfg_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d mode=%u serial_no=\"%c%c%c%c%02X%02X%02X%02X\" password=\"%c%c%c%c%c%c%c%c%c%c\" reg_id=\"%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\" ident=%u stop=%u plev_cap=%u ploam_timeout_0=%u ploam_timeout_1=%u ploam_timeout_2=%u ploam_timeout_3=%u ploam_timeout_4=%u ploam_timeout_5=%u ploam_timeout_6=%u ploam_timeout_cpl=%u ploam_timeout_cpi=%u ploam_timeout_tpd=%u tdm_coexistence=%u dg_dis=%u ds_fcs_en=%u ds_ts_dis=%u %s",
		(int)fct_ret, param.mode, param.serial_no[0],
		param.serial_no[1], param.serial_no[2], param.serial_no[3],
		param.serial_no[4], param.serial_no[5], param.serial_no[6],
		param.serial_no[7], param.password[0], param.password[1],
		param.password[2], param.password[3], param.password[4],
		param.password[5], param.password[6], param.password[7],
		param.password[8], param.password[9], param.reg_id[0],
		param.reg_id[1], param.reg_id[2], param.reg_id[3],
		param.reg_id[4], param.reg_id[5], param.reg_id[6],
		param.reg_id[7], param.reg_id[8], param.reg_id[9],
		param.reg_id[10], param.reg_id[11], param.reg_id[12],
		param.reg_id[13], param.reg_id[14], param.reg_id[15],
		param.reg_id[16], param.reg_id[17], param.reg_id[18],
		param.reg_id[19], param.reg_id[20], param.reg_id[21],
		param.reg_id[22], param.reg_id[23], param.reg_id[24],
		param.reg_id[25], param.reg_id[26], param.reg_id[27],
		param.reg_id[28], param.reg_id[29], param.reg_id[30],
		param.reg_id[31], param.reg_id[32], param.reg_id[33],
		param.reg_id[34], param.reg_id[35], param.ident, param.stop,
		param.plev_cap, param.ploam_timeout_0, param.ploam_timeout_1,
		param.ploam_timeout_2, param.ploam_timeout_3,
		param.ploam_timeout_4, param.ploam_timeout_5,
		param.ploam_timeout_6, param.ploam_timeout_cpl,
		param.ploam_timeout_cpi, param.ploam_timeout_tpd,
		param.tdm_coexistence, param.dg_dis, param.ds_fcs_en,
		param.ds_ts_dis, FAPI_PON_CRLF);
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_cred_set(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_cred_cfg param = {0};
	char serial[14];
	char password[11];
	unsigned int sn4, sn5, sn6, sn7;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: cred_set" FAPI_PON_CRLF
		"Short Form: crs" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- char *serial_no" FAPI_PON_CRLF
		"- char *password" FAPI_PON_CRLF
		"- uint8_t reg_id[36]" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;

	ret = cli_sscanf(p_cmd, "%13s %10s %bu[36]",
			 serial, password, &param.reg_id[0]);
	if (ret != 3)
		return cli_check_help__file("-h", usage, p_out);

	ret = sscanf_s(serial, "%c%c%c%c%02X%02X%02X%02X",
			 SSCANF_CHAR(&param.serial_no[0]),
			 SSCANF_CHAR(&param.serial_no[1]),
			 SSCANF_CHAR(&param.serial_no[2]),
			 SSCANF_CHAR(&param.serial_no[3]),
			 &sn4, &sn5, &sn6, &sn7);
	if (ret != 8)
		return cli_check_help__file("-h", usage, p_out);

	ret = sscanf_s(password, "%c%c%c%c%c%c%c%c%c%c",
			 SSCANF_CHAR(&param.password[0]),
			 SSCANF_CHAR(&param.password[1]),
			 SSCANF_CHAR(&param.password[2]),
			 SSCANF_CHAR(&param.password[3]),
			 SSCANF_CHAR(&param.password[4]),
			 SSCANF_CHAR(&param.password[5]),
			 SSCANF_CHAR(&param.password[6]),
			 SSCANF_CHAR(&param.password[7]),
			 SSCANF_CHAR(&param.password[8]),
			 SSCANF_CHAR(&param.password[9]));
	if (ret != 10)
		return cli_check_help__file("-h", usage, p_out);

	param.serial_no[4] = (uint8_t)sn4;
	param.serial_no[5] = (uint8_t)sn5;
	param.serial_no[6] = (uint8_t)sn6;
	param.serial_no[7] = (uint8_t)sn7;

	fct_ret = fapi_pon_cred_set(p_ctx, &param);
	return fprintf(p_out, "errorcode=%d %s",
		(int)fct_ret, FAPI_PON_CRLF);
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_cred_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_cred_cfg param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: cred_get" FAPI_PON_CRLF
		"Short Form: crg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint8_t serial_no[8]" FAPI_PON_CRLF
		"- uint8_t password[10]" FAPI_PON_CRLF
		"- uint8_t reg_id[36]" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_cred_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d serial_no=\"%c%c%c%c%02X%02X%02X%02X\" password=\"%c%c%c%c%c%c%c%c%c%c\" reg_id=\"%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\" %s",
		(int)fct_ret, param.serial_no[0], param.serial_no[1],
		param.serial_no[2], param.serial_no[3], param.serial_no[4],
		param.serial_no[5], param.serial_no[6], param.serial_no[7],
		param.password[0], param.password[1], param.password[2],
		param.password[3], param.password[4], param.password[5],
		param.password[6], param.password[7], param.password[8],
		param.password[9], param.reg_id[0], param.reg_id[1],
		param.reg_id[2], param.reg_id[3], param.reg_id[4],
		param.reg_id[5], param.reg_id[6], param.reg_id[7],
		param.reg_id[8], param.reg_id[9], param.reg_id[10],
		param.reg_id[11], param.reg_id[12], param.reg_id[13],
		param.reg_id[14], param.reg_id[15], param.reg_id[16],
		param.reg_id[17], param.reg_id[18], param.reg_id[19],
		param.reg_id[20], param.reg_id[21], param.reg_id[22],
		param.reg_id[23], param.reg_id[24], param.reg_id[25],
		param.reg_id[26], param.reg_id[27], param.reg_id[28],
		param.reg_id[29], param.reg_id[30], param.reg_id[31],
		param.reg_id[32], param.reg_id[33], param.reg_id[34],
		param.reg_id[35], FAPI_PON_CRLF);
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_serial_number_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_serial_number param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: serial_number_get" FAPI_PON_CRLF
		"Short Form: sng" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- char serial_no[8]" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_serial_number_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d serial_no=\"%c%c%c%c%02X%02X%02X%02X\" %s",
		(int)fct_ret, param.serial_no[0], param.serial_no[1],
		param.serial_no[2], param.serial_no[3], param.serial_no[4],
		param.serial_no[5], param.serial_no[6], param.serial_no[7],
		FAPI_PON_CRLF);
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_eeprom_data_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	char eeprom_file_path[128];
	unsigned char buffer[256];
	unsigned int offset = 0;
	unsigned int data_size = 0;
	unsigned int i;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: eeprom_data_get" FAPI_PON_CRLF
		"Short Form: edg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- unsigned int offset (max 255)" FAPI_PON_CRLF
		"- unsigned int data_size (max 256)" FAPI_PON_CRLF
		"- char filename[128] (name of EEPROM)" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- unsigned char data[]" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;

	ret = cli_sscanf(p_cmd, "%u %u %127s",
		&offset, &data_size, &eeprom_file_path[0]);
	if (ret != 3)
		return cli_check_help__file("-h", usage, p_out);

	if (data_size > 256)
		data_size = 256;

	/* We always use PON_DDMI_A0 here, as the cli context is only temporary
	 * and for the raw eeprom access it does not matter.
	 */
	fct_ret = fapi_pon_eeprom_open(p_ctx, PON_DDMI_A0, eeprom_file_path);

	if (fct_ret == PON_STATUS_OK)
		fct_ret = fapi_pon_eeprom_data_get(p_ctx, PON_DDMI_A0,
						   &buffer[0], offset,
						   data_size);

	fprintf(p_out, "errorcode=%d ", (int)fct_ret);

	if (fct_ret == PON_STATUS_OK) {
		fprintf(p_out, "data=\"");
		for (i = 0; i < data_size; i++) {
			fprintf(p_out, "0x%02X", buffer[i] & 0xFF);
			if (i < data_size - 1)
				fprintf(p_out, " ");
		}
		fprintf(p_out, "\" ");
	}

	return fprintf(p_out, "%s", FAPI_PON_CRLF);
}

/**
 * Parse a space-separated string of hex bytes into a data buffer.
 *
 * \param data      Pointer to the output buffer to store parsed bytes.
 * \param data_size Size of the output buffer.
 * \param cmd       Input string containing space-separated hex bytes.
 * \param skip      Number of initial tokens to skip in the input string.
 * \return          Number of bytes parsed and stored in the buffer, or -1 on error.
 */
static int bytes_parse(uint8_t *data, int data_size, char *cmd, int skip)
{
	int curr_byte;
	const char *sep = " ";
	char *byte;
	char *cmd_tok = NULL;
	uint8_t *p = data;

	byte = strtok_r(cmd, sep, &cmd_tok);
	curr_byte = 0;
	while (byte != NULL) {
		if (curr_byte >= data_size)
			return -1;

		if (skip) {
			skip--;
		} else {
			char *endptr = NULL;
			long val = strtol(byte, &endptr, 16);
			if (endptr == byte || *endptr != '\0' || val < 0 || val > 0xFF) {
				return -1;
			}
			p[curr_byte++] = (uint8_t)(val & 0xFF);
		}

		byte = strtok_r(0, sep, &cmd_tok);
	}

	return curr_byte;
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_eeprom_data_set(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	char buffer[512];
	char eeprom_file_path[128];
	unsigned char data[4];
	unsigned int offset = 0;
	int data_size;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: eeprom_data_set" FAPI_PON_CRLF
		"Short Form: eds" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- unsigned int offset (max 255)" FAPI_PON_CRLF
		"- char filename[128] (name of EEPROM)" FAPI_PON_CRLF
		"- unsigned char data[1..4] (hex)" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;

	ret = cli_sscanf(p_cmd, "%u %127s", &offset, &eeprom_file_path[0]);
	if (ret != 2)
		return cli_check_help__file("-h", usage, p_out);

	ret = sprintf_s(buffer, sizeof(buffer), "%s", p_cmd);
	if (ret < 0)
		return ret;

	data_size = bytes_parse(data, sizeof(data), buffer, 2);
	if (data_size <= 0 || data_size > (int)sizeof(data))
		return cli_check_help__file("-h", usage, p_out);

	/* We always use PON_DDMI_A0 here, as the cli context is only temporary
	 * and for the raw eeprom access it does not matter.
	 */
	fct_ret = fapi_pon_eeprom_open(p_ctx, PON_DDMI_A0, eeprom_file_path);

	if (fct_ret == PON_STATUS_OK)
		fct_ret = fapi_pon_eeprom_data_set(p_ctx, PON_DDMI_A0, &data[0],
						   offset, data_size);

	fprintf(p_out, "errorcode=%d ", (int)fct_ret);
	return fprintf(p_out, "%s", FAPI_PON_CRLF);
}

/** Handle command
 *  \param[in] p_ctx     FAPI_PON context pointer
 *  \param[in] p_cmd     Input commands
 *  \param[in] p_out     Output FD
 */
static int cli_fapi_pon_optic_status_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_optic_status param = {0};
	char *eeprom_file_path[PON_DDMI_MAX];
	char *tx_power_scale_string;
	char buffer[150];
	char *saveptr;
	enum pon_tx_power_scale tx_power_scale = TX_POWER_SCALE_0_1;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: optic_status_get" FAPI_PON_CRLF
		"Short Form: osg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- char* filename of EEPROM A0h" FAPI_PON_CRLF
		"- char* filename of EEPROM A2h" FAPI_PON_CRLF
		"- enum pon_tx_power_scale tx_power_scale"
		" (optional)" FAPI_PON_CRLF
		"   TX_POWER_SCALE_0_1 (0.1 µW/LSB) = 0x0000"
		" (default)" FAPI_PON_CRLF
		"   TX_POWER_SCALE_0_2 (0.2 µW/LSB) = 0x0001" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- int32_t temperature" FAPI_PON_CRLF
		"- uint32_t voltage" FAPI_PON_CRLF
		"- uint32_t bias" FAPI_PON_CRLF
		"- int32_t tx_power" FAPI_PON_CRLF
		"- int32_t rx_power" FAPI_PON_CRLF
		"- uint32_t rx_los" FAPI_PON_CRLF
		"- uint32_t tx_disable" FAPI_PON_CRLF
		"- uint32_t tx_fault" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;

	ret = sprintf_s(buffer, sizeof(buffer), "%s", p_cmd);
	if (ret < 0)
		return ret;

	eeprom_file_path[PON_DDMI_A0] = strtok_r(buffer, " ", &saveptr);
	eeprom_file_path[PON_DDMI_A2] = strtok_r(NULL, " ", &saveptr);
	tx_power_scale_string = strtok_r(NULL, " ", &saveptr);

	if ((strnlen_s(eeprom_file_path[PON_DDMI_A0], MAX_FILENAME_LEN) ==
	     MAX_FILENAME_LEN) ||
	     !strnlen_s(eeprom_file_path[PON_DDMI_A0], MAX_FILENAME_LEN) ||
	     (strnlen_s(eeprom_file_path[PON_DDMI_A2], MAX_FILENAME_LEN) ==
	     MAX_FILENAME_LEN) ||
	     !strnlen_s(eeprom_file_path[PON_DDMI_A2], MAX_FILENAME_LEN))
		return cli_check_help__file("-h", usage, p_out);

	if (tx_power_scale_string) {
		if (strtol(tx_power_scale_string, NULL, 0) == 1)
			tx_power_scale = TX_POWER_SCALE_0_2;
	}

	fct_ret = fapi_pon_eeprom_open(p_ctx, PON_DDMI_A0,
				   eeprom_file_path[PON_DDMI_A0]);
	if (fct_ret == PON_STATUS_OK)
		fct_ret = fapi_pon_eeprom_open(p_ctx, PON_DDMI_A2,
					       eeprom_file_path[PON_DDMI_A2]);
	if (fct_ret == PON_STATUS_OK)
		fct_ret = fapi_pon_optic_status_get(p_ctx, &param,
						    tx_power_scale);

	fprintf(p_out, "errorcode=%d temperature=%d voltage=%u bias=%u tx_power=%d rx_power=%d rx_los=%u tx_disable=%u tx_fault=%u %s",
		(int)fct_ret, param.temperature, param.voltage, param.bias,
		param.tx_power, param.rx_power, param.rx_los,
		param.tx_disable, param.tx_fault, FAPI_PON_CRLF);

	return 0;
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_optic_properties_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_optic_properties param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: optic_properties_get" FAPI_PON_CRLF
		"Short Form: opg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- char* filename of EEPROM" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- char vendor_name[17]" FAPI_PON_CRLF
		"- uint8_t vendor_oui[4]" FAPI_PON_CRLF
		"- char part_number[17]" FAPI_PON_CRLF
		"- char revision[5]" FAPI_PON_CRLF
		"- char serial_number[17]" FAPI_PON_CRLF
		"- char date_code[9]" FAPI_PON_CRLF
		"- uint8_t identifier" FAPI_PON_CRLF
		"- uint8_t connector" FAPI_PON_CRLF
		"- uint32_t signaling_rate" FAPI_PON_CRLF
		"- uint32_t tx_wavelength" FAPI_PON_CRLF
		"- uint32_t high_power_lvl_decl" FAPI_PON_CRLF
		"- uint32_t paging_implemented_ind" FAPI_PON_CRLF
		"- uint32_t retimer_ind" FAPI_PON_CRLF
		"- uint32_t cooled_transceiver_decl" FAPI_PON_CRLF
		"- uint32_t power_lvl_decl" FAPI_PON_CRLF
		"- uint32_t linear_rx_output_impl" FAPI_PON_CRLF
		"- uint32_t rx_decision_thr_impl" FAPI_PON_CRLF
		"- uint32_t tunable_transmitter" FAPI_PON_CRLF
		"- uint32_t rate_select" FAPI_PON_CRLF
		"- uint32_t tx_disable" FAPI_PON_CRLF
		"- uint32_t tx_fault" FAPI_PON_CRLF
		"- uint32_t signal_detect" FAPI_PON_CRLF
		"- uint32_t rx_los" FAPI_PON_CRLF
		"- uint32_t digital_monitoring" FAPI_PON_CRLF
		"- uint32_t int_calibrated" FAPI_PON_CRLF
		"- uint32_t ext_calibrated" FAPI_PON_CRLF
		"- uint32_t rx_power_measurement_type" FAPI_PON_CRLF
		"- uint32_t address_change_req" FAPI_PON_CRLF
		"- uint32_t optional_flags_impl" FAPI_PON_CRLF
		"- uint32_t soft_tx_disable_monitor" FAPI_PON_CRLF
		"- uint32_t soft_tx_fault_monitor" FAPI_PON_CRLF
		"- uint32_t soft_rx_los_monitor" FAPI_PON_CRLF
		"- uint32_t soft_rate_select_monitor" FAPI_PON_CRLF
		"- uint32_t app_select_impl" FAPI_PON_CRLF
		"- uint32_t soft_rate_select_ctrl_impl" FAPI_PON_CRLF
		"- uint32_t compliance" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;

	if ((strnlen_s(p_cmd, MAX_FILENAME_LEN) == MAX_FILENAME_LEN) ||
		      !strnlen_s(p_cmd, MAX_FILENAME_LEN))
		return cli_check_help__file("-h", usage, p_out);

	fct_ret = fapi_pon_eeprom_open(p_ctx, PON_DDMI_A0, p_cmd);
	if (fct_ret == PON_STATUS_OK)
		fct_ret = fapi_pon_optic_properties_get(p_ctx, &param);

	return fprintf(p_out,
		"errorcode=%d vendor_name=\"%.*s\" vendor_oui=\"%.*s\" part_number=\"%.*s\" revision=\"%.*s\" serial_number=\"%.*s\" date_code=\"%.*s\" identifier=%x connector=%x signaling_rate=%u tx_wavelength=%u high_power_lvl_decl=%u paging_implemented_ind=%u retimer_ind=%u cooled_transceiver_decl=%u power_lvl_decl=%u linear_rx_output_impl=%u rx_decision_thr_impl=%u tunable_transmitter=%u rate_select=%u tx_disable=%u tx_fault=%u signal_detect=%u rx_los=%u digital_monitoring=%u int_calibrated=%u ext_calibrated=%u rx_power_measurement_type=%u address_change_req=%u optional_flags_impl=%u soft_tx_disable_monitor=%u soft_tx_fault_monitor=%u soft_rx_los_monitor=%u soft_rate_select_monitor=%u app_select_impl=%u soft_rate_select_ctrl_impl=%u compliance=%u %s",
		(int)fct_ret, (int)sizeof(param.vendor_name),
		param.vendor_name, (int)sizeof(param.vendor_oui),
		param.vendor_oui, (int)sizeof(param.part_number),
		param.part_number, (int)sizeof(param.revision),
		param.revision, (int)sizeof(param.serial_number),
		param.serial_number, (int)sizeof(param.date_code),
		param.date_code, param.identifier, param.connector,
		param.signaling_rate, param.tx_wavelength,
		param.high_power_lvl_decl, param.paging_implemented_ind,
		param.retimer_ind, param.cooled_transceiver_decl,
		param.power_lvl_decl, param.linear_rx_output_impl,
		param.rx_decision_thr_impl, param.tunable_transmitter,
		param.rate_select, param.tx_disable, param.tx_fault,
		param.signal_detect, param.rx_los, param.digital_monitoring,
		param.int_calibrated, param.ext_calibrated,
		param.rx_power_measurement_type, param.address_change_req,
		param.optional_flags_impl, param.soft_tx_disable_monitor,
		param.soft_tx_fault_monitor, param.soft_rx_los_monitor,
		param.soft_rate_select_monitor, param.app_select_impl,
		param.soft_rate_select_ctrl_impl, param.compliance,
		FAPI_PON_CRLF);
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_password_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_password param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: password_get" FAPI_PON_CRLF
		"Short Form: pg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- char password[10]" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_password_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d password=\"%c%c%c%c%c%c%c%c%c%c\" %s",
		(int)fct_ret, param.password[0], param.password[1],
		param.password[2], param.password[3], param.password[4],
		param.password[5], param.password[6], param.password[7],
		param.password[8], param.password[9], FAPI_PON_CRLF);
}

/** Handle command
   \param[in] p_ctx     FAPI_PON context pointer
   \param[in] p_cmd     Input commands
   \param[in] p_out     Output FD
*/
static int cli_fapi_pon_alloc_gem_port_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	uint32_t alloc_id = 0;
	uint32_t gem_ports[256] = { 0 };
	uint32_t gem_ports_num = sizeof(gem_ports) / 4;
	unsigned int i;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: alloc_gem_port_get" FAPI_PON_CRLF
		"Short Form: agpg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- uint32_t alloc_id" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint32_t alloc_id" FAPI_PON_CRLF
		"- uint32_t gem_port_id[256]" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = cli_sscanf(p_cmd, "%u", &alloc_id);
	if (ret != 1)
		return cli_check_help__file("-h", usage, p_out);
	fct_ret = fapi_pon_alloc_gem_port_get(p_ctx, alloc_id,
					      &gem_ports_num, gem_ports);

	fprintf(p_out, "errorcode=%d ", (int)fct_ret);

	if (gem_ports_num != 0) {
		fprintf(p_out, "alloc_id=%u gem_port_id=\"", alloc_id);
		for (i = 0; i < gem_ports_num; i++) {
			fprintf(p_out, "%u", gem_ports[i]);
			if (i < gem_ports_num - 1)
				fprintf(p_out, " ");
		}
		fprintf(p_out, "\" ");
	}

	return fprintf(p_out, "%s", FAPI_PON_CRLF);
}

/** Handle command
* \param[in] p_ctx     FAPI_PON context pointer
* \param[in] p_cmd     Input commands
* \param[in] p_out     Output FD
*/
static int cli_fapi_pon_gem_all_counters_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_gem_port_counters param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: gem_all_counters_get" FAPI_PON_CRLF
		"Short Form: gacg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint64_t tx_frames" FAPI_PON_CRLF
		"- uint64_t tx_fragments" FAPI_PON_CRLF
		"- uint64_t tx_bytes" FAPI_PON_CRLF
		"- uint64_t rx_frames" FAPI_PON_CRLF
		"- uint64_t rx_fragments" FAPI_PON_CRLF
		"- uint64_t rx_bytes" FAPI_PON_CRLF
		"- uint64_t key_errors" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_gem_all_counters_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d tx_frames=%" PRIu64
		" tx_fragments=%" PRIu64
		" tx_bytes=%" PRIu64
		" rx_frames=%" PRIu64
		" rx_fragments=%" PRIu64
		" rx_bytes=%" PRIu64
		" key_errors=%" PRIu64
		" %s",
		(int)fct_ret, param.tx_frames,
		param.tx_fragments, param.tx_bytes, param.rx_frames,
		param.rx_fragments, param.rx_bytes, param.key_errors,
		FAPI_PON_CRLF);
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_twdm_xgem_all_counters_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	uint8_t dswlch_id = 0;
	struct pon_gem_port_counters param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: twdm_xgem_all_counters_get" FAPI_PON_CRLF
		"Short Form: txacg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- uint8_t dswlch_id" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- uint64_t tx_frames" FAPI_PON_CRLF
		"- uint64_t tx_fragments" FAPI_PON_CRLF
		"- uint64_t tx_bytes" FAPI_PON_CRLF
		"- uint64_t rx_frames" FAPI_PON_CRLF
		"- uint64_t rx_fragments" FAPI_PON_CRLF
		"- uint64_t rx_bytes" FAPI_PON_CRLF
		"- uint64_t key_errors" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = cli_sscanf(p_cmd, "%bu", &dswlch_id);
	if (ret != 1)
		return cli_check_help__file("-h", usage, p_out);
	fct_ret = fapi_pon_twdm_xgem_all_counters_get(p_ctx, dswlch_id, &param);
	return fprintf(p_out,
		"errorcode=%d tx_frames=%" PRIu64
		" tx_fragments=%" PRIu64
		" tx_bytes=%" PRIu64
		" rx_frames=%" PRIu64
		" rx_fragments=%" PRIu64
		" rx_bytes=%" PRIu64
		" key_errors=%" PRIu64
		" %s",
		(int)fct_ret, param.tx_frames,
		param.tx_fragments, param.tx_bytes, param.rx_frames,
		param.rx_fragments, param.rx_bytes, param.key_errors,
		FAPI_PON_CRLF);
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_debug_test_pattern_cfg_get(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_debug_test_pattern param = {0};

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: debug_test_pattern_cfg_get" FAPI_PON_CRLF
		"Short Form: dtpcg" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		"- enum test_pattern_type tx_type" FAPI_PON_CRLF
		"   TP_MODE_PRBS31_28 = 1" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_18 = 2" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_21 = 3" FAPI_PON_CRLF
		"   TP_MODE_PRBS16 = 4" FAPI_PON_CRLF
		"   TP_MODE_PRBS15 = 5" FAPI_PON_CRLF
		"   TP_MODE_PRBS11 = 6" FAPI_PON_CRLF
		"   TP_MODE_PRBS9 = 7" FAPI_PON_CRLF
		"   TP_MODE_PRBS7 = 8" FAPI_PON_CRLF
		"   TP_MODE_PAT = 9" FAPI_PON_CRLF
		"   TP_MODE_PAT_DCBAL = 10" FAPI_PON_CRLF
		"   TP_MODE_PAT_FIX = 11" FAPI_PON_CRLF
		"- enum test_pattern_type rx_type" FAPI_PON_CRLF
		"   TP_MODE_PRBS31_28 = 1" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_18 = 2" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_21 = 3" FAPI_PON_CRLF
		"   TP_MODE_PRBS16 = 4" FAPI_PON_CRLF
		"   TP_MODE_PRBS15 = 5" FAPI_PON_CRLF
		"   TP_MODE_PRBS11 = 6" FAPI_PON_CRLF
		"   TP_MODE_PRBS9 = 7" FAPI_PON_CRLF
		"   TP_MODE_PRBS7 = 8" FAPI_PON_CRLF
		"   TP_MODE_PAT = 9" FAPI_PON_CRLF
		"   TP_MODE_PAT_DCBAL = 10" FAPI_PON_CRLF
		"   TP_MODE_PAT_FIX = 11" FAPI_PON_CRLF
		"- uint16_t pattern" FAPI_PON_CRLF
		"- int8_t rx_inv" FAPI_PON_CRLF
		"- int8_t tx_inv" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	fct_ret = fapi_pon_debug_test_pattern_cfg_get(p_ctx, &param);
	return fprintf(p_out,
		"errorcode=%d tx_type=%u rx_type=%u pattern=0x%x rx_inv=%d tx_inv=%d %s",
		(int)fct_ret, param.tx_type, param.rx_type, param.pattern,
		param.rx_inv, param.tx_inv, FAPI_PON_CRLF);
}

/** Handle command
 * \param[in] p_ctx     FAPI_PON context pointer
 * \param[in] p_cmd     Input commands
 * \param[in] p_out     Output FD
 */
static int cli_fapi_pon_debug_test_pattern_cfg_set(
	void *p_ctx,
	const char *p_cmd,
	clios_file_io_t *p_out)
{
	int ret = 0;
	enum fapi_pon_errorcode fct_ret = (enum fapi_pon_errorcode)0;
	struct pon_debug_test_pattern param = {0};
	uint16_t pattern = 0;

#ifndef FAPI_PON_DEBUG_DISABLE
	static const char usage[] =
		"Long Form: debug_test_pattern_cfg_set" FAPI_PON_CRLF
		"Short Form: dtpcs" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Input Parameter" FAPI_PON_CRLF
		"- enum test_pattern_type tx_type" FAPI_PON_CRLF
		"   TP_MODE_PRBS31_28 = 1" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_18 = 2" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_21 = 3" FAPI_PON_CRLF
		"   TP_MODE_PRBS16 = 4" FAPI_PON_CRLF
		"   TP_MODE_PRBS15 = 5" FAPI_PON_CRLF
		"   TP_MODE_PRBS11 = 6" FAPI_PON_CRLF
		"   TP_MODE_PRBS9 = 7" FAPI_PON_CRLF
		"   TP_MODE_PRBS7 = 8" FAPI_PON_CRLF
		"   TP_MODE_PAT = 9" FAPI_PON_CRLF
		"   TP_MODE_PAT_DCBAL = 10" FAPI_PON_CRLF
		"   TP_MODE_PAT_FIX = 11" FAPI_PON_CRLF
		"- enum test_pattern_type rx_type" FAPI_PON_CRLF
		"   TP_MODE_PRBS31_28 = 1" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_18 = 2" FAPI_PON_CRLF
		"   TP_MODE_PRBS23_21 = 3" FAPI_PON_CRLF
		"   TP_MODE_PRBS16 = 4" FAPI_PON_CRLF
		"   TP_MODE_PRBS15 = 5" FAPI_PON_CRLF
		"   TP_MODE_PRBS11 = 6" FAPI_PON_CRLF
		"   TP_MODE_PRBS9 = 7" FAPI_PON_CRLF
		"   TP_MODE_PRBS7 = 8" FAPI_PON_CRLF
		"   TP_MODE_PAT = 9" FAPI_PON_CRLF
		"   TP_MODE_PAT_DCBAL = 10" FAPI_PON_CRLF
		"   TP_MODE_PAT_FIX = 11" FAPI_PON_CRLF
		"- uint8_t pattern" FAPI_PON_CRLF
		"- int8_t rx_inv" FAPI_PON_CRLF
		"- int8_t tx_inv" FAPI_PON_CRLF
		FAPI_PON_CRLF
		"Output Parameter" FAPI_PON_CRLF
		"- enum fapi_pon_errorcode errorcode" FAPI_PON_CRLF
		FAPI_PON_CRLF;
#else
#undef usage
#define usage ""
#endif

	ret = cli_check_help__file(p_cmd, usage, p_out);
	if (ret != 0)
		return ret;
	ret = cli_sscanf(p_cmd, "%u %u %hu %bd %bd",
		&param.tx_type, &param.rx_type, &pattern, &param.rx_inv,
		&param.tx_inv);
	if (ret != 5)
		return cli_check_help__file("-h", usage, p_out);
	if (pattern > 0xFF)
		return fprintf(p_out, "errorcode=%d %s",
			       (int)PON_STATUS_ERR, FAPI_PON_CRLF);
	param.pattern = (uint8_t) pattern;

	fct_ret = fapi_pon_debug_test_pattern_cfg_set(p_ctx, &param);
	return fprintf(p_out, "errorcode=%d %s",
		       (int)fct_ret, FAPI_PON_CRLF);
}

/** Register cli commands */
int pon_ext_cli_cmd_register(struct cli_core_context_s *p_core_ctx)
{
	unsigned int group_mask = 0;

	cli_core_key_add__file(p_core_ctx, group_mask, "crs",
		"cred_set", cli_fapi_pon_cred_set);
	cli_core_key_add__file(p_core_ctx, group_mask, "crg",
		"cred_get", cli_fapi_pon_cred_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "rg",
		"register_get", cli_fapi_pon_register_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "bg",
		"bit_get", cli_fapi_pon_bit_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "bs",
		"bit_set", cli_fapi_pon_bit_set);
	cli_core_key_add__file(p_core_ctx, group_mask,
		CLI_EMPTY_CMD, "omci_cfg_get", cli_fapi_pon_omci_cfg_get);
	cli_core_key_add__file(p_core_ctx, group_mask,
		CLI_EMPTY_CMD, "omci_cfg_set", cli_fapi_pon_omci_cfg_set);
	cli_core_key_add__file(p_core_ctx, group_mask, "gcg",
		"gpon_cfg_get", cli_fapi_pon_gpon_cfg_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "sng",
		"serial_number_get", cli_fapi_pon_serial_number_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "edg",
		"eeprom_data_get", cli_fapi_pon_eeprom_data_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "eds",
		"eeprom_data_set", cli_fapi_pon_eeprom_data_set);
	cli_core_key_add__file(p_core_ctx, group_mask, "osg",
		"optic_status_get", cli_fapi_pon_optic_status_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "opg",
		"optic_properties_get", cli_fapi_pon_optic_properties_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "pg",
		"password_get", cli_fapi_pon_password_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "agpg",
		"alloc_gem_port_get", cli_fapi_pon_alloc_gem_port_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "gacg",
		"gem_all_counters_get", cli_fapi_pon_gem_all_counters_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "txacg",
		"twdm_xgem_all_counters_get", cli_fapi_pon_twdm_xgem_all_counters_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "dtpcg",
		"debug_test_pattern_cfg_get",
		cli_fapi_pon_debug_test_pattern_cfg_get);
	cli_core_key_add__file(p_core_ctx, group_mask, "dtpcs",
		"debug_test_pattern_cfg_set",
		cli_fapi_pon_debug_test_pattern_cfg_set);

	return 0;
}

/*! @} */

#endif
