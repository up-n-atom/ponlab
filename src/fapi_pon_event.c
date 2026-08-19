/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
#  include "pon_config.h"
#endif

#include "pon_mbox.h"
#include "fapi_pon.h"
#include "fapi_pon_core.h"
#include "fapi_pon_debug.h"
#include "fapi_pon_os.h"
#include "pon_ip_msg.h"

#define COPY_32_BITS_TO_8_BITS(fapi_array, fw_param, index)		\
	do {								\
		fapi_array[index] = (fw_param & 0xFF000000) >> 24;	\
		fapi_array[index + 1] = (fw_param & 0xFF0000) >> 16;	\
		fapi_array[index + 2] = (fw_param & 0xFF00) >> 8;	\
		fapi_array[index + 3] = fw_param & 0xFF;		\
	} while (0)

#define COPY_16_BITS_TO_8_BITS(fapi_array, fw_param, index)		\
	do {								\
		fapi_array[index] = (fw_param & 0xFF00) >> 8;		\
		fapi_array[index + 1] = fw_param & 0xFF;		\
	} while (0)

static void fapi_pon_listener_xgtc_log(struct pon_ctx *ctx, struct nl_msg *msg,
				       struct nlattr **attrs)
{
	struct pon_xgtc_ploam_message xgtc_log;
	struct ponfw_xgtc_ploam_log *fw_param;
	enum fapi_pon_errorcode err;
	int i;

	if (!ctx->xgtc_log)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	xgtc_log.direction = (uint8_t)fw_param->dir;
	xgtc_log.time_stamp = fw_param->time_stamp;
	xgtc_log.onu_id = fw_param->id;
	xgtc_log.message_type_id = fw_param->type;
	xgtc_log.message_seq_no = fw_param->seqno;
	for (i = 0; i < 9; i++)
		COPY_32_BITS_TO_8_BITS(xgtc_log.message,
				       fw_param->msg[i], i * 4);

	ctx->xgtc_log(ctx->priv, &xgtc_log);

	err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, NULL, 0,
				       PON_MBOX_C_MSG);
	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("sending ack for XGTC_PLOAM_LOG failed %i",
			      err);
}

static void fapi_pon_listener_gtc_log(struct pon_ctx *ctx, struct nl_msg *msg,
				      struct nlattr **attrs)
{
	struct pon_gtc_ploam_message gtc_log;
	struct ponfw_gtc_ploam_log *fw_param;
	enum fapi_pon_errorcode err;

	if (!ctx->gtc_log)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	gtc_log.direction = (uint8_t)fw_param->dir;
	gtc_log.time_stamp = fw_param->time_stamp;
	gtc_log.onu_id = fw_param->id;
	gtc_log.message_type_id = fw_param->mid;
	COPY_16_BITS_TO_8_BITS(gtc_log.message, fw_param->data1, 0);
	COPY_32_BITS_TO_8_BITS(gtc_log.message, fw_param->data2, 2);
	COPY_32_BITS_TO_8_BITS(gtc_log.message, fw_param->data3, 6);

	ctx->gtc_log(ctx->priv, &gtc_log);

	err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, NULL, 0,
				       PON_MBOX_C_MSG);
	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("sending ack for GTC_PLOAM_LOG failed %i", err);
}

static void fapi_pon_listener_ploam_state(struct pon_ctx *ctx,
					  struct nl_msg *msg,
					  struct nlattr **attrs)
{
	struct pon_ploam_state_evt ploam_state;
	struct ponfw_ploam_state *fw_param;
	enum fapi_pon_errorcode err;

	if (!ctx->ploam_state)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	ploam_state.current = fw_param->ploam_act;
	ploam_state.previous = fw_param->ploam_prev;
	ploam_state.time_prev = fw_param->ploam_time;
	ploam_state.change_reason = fw_param->reason;

	ctx->ploam_state(ctx->priv, &ploam_state);

	err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, NULL, 0,
				       PON_MBOX_C_MSG);
	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("sending ack for PLOAM_STATE failed %i", err);
}

static void fapi_pon_listener_alarm_report(struct pon_ctx *ctx,
					   struct nl_msg *msg,
					   struct nlattr **attrs)
{
	struct pon_alarm_status alarms;
	struct ponfw_report_alarm *fw_param;

	UNUSED(msg);

	if (!ctx->alarm_report)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	alarms.alarm_id = fw_param->alarm_id;
	alarms.alarm_status = PON_ALARM_EN;

	ctx->alarm_report(ctx->priv, &alarms);

	/* No Ack needed by FW for alarms */
}

static void fapi_pon_listener_alarm_clear(struct pon_ctx *ctx,
					  struct nl_msg *msg,
					  struct nlattr **attrs)
{
	struct pon_alarm_status alarms;
	struct ponfw_clear_alarm *fw_param;

	UNUSED(msg);

	if (!ctx->alarm_clear)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	alarms.alarm_id = fw_param->alarm_id;
	alarms.alarm_status = PON_ALARM_DIS;

	ctx->alarm_clear(ctx->priv, &alarms);

	/* No Ack needed by FW for alarms */
}

static void fapi_pon_listener_xgtc_power_level(struct pon_ctx *ctx,
					       struct nl_msg *msg,
					       struct nlattr **attrs)
{
	struct ponfw_tx_power_level_req *fw_param;
	enum fapi_pon_errorcode err;
	uint32_t attenuation;

	if (!ctx->xgtc_power_level)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	attenuation = fw_param->attenuation;

	err = ctx->xgtc_power_level(ctx->priv, fw_param->oper, &attenuation);

	fw_param->attenuation = attenuation;

	switch (err) {
	case PON_STATUS_OK_NO_RESPONSE:
		return;
	case PON_STATUS_OK:
		err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK,
					       fw_param, sizeof(*fw_param),
					       PON_MBOX_C_MSG);
		break;
	default:
		err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_NACK,
					       NULL, 0, PON_MBOX_C_MSG);
		break;
	}

	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("sending (N)ACK for XGTC_POWER_LEVEL failed %i",
			      err);
}

static void fapi_pon_listener_onu_tod_sync(struct pon_ctx *ctx,
					   struct nl_msg *msg,
					   struct nlattr **attrs)
{
	struct ponfw_onu_tod_sync *fw_param;
	enum fapi_pon_errorcode err;
	struct pon_gpon_tod_sync onu_tod_sync = {0};
	struct pon_cap caps = {0};
	int clock_cycle;

	if (!ctx->onu_tod_sync)
		return;

	if (!attrs[PON_MBOX_A_DATA]
	   || nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	err = fapi_pon_cap_get(ctx, &caps);
	if (err != PON_STATUS_OK)
		return;

	onu_tod_sync.multiframe_count = fw_param->mf_count;
	if (caps.features & PON_FEATURE_G989)
		onu_tod_sync.multiframe_count &= 0x3FFFFFFF;

	clock_cycle = get_clock_cycle_from_caps(&caps);

	onu_tod_sync.tod_seconds = fw_param->tod_sec;
	onu_tod_sync.tod_extended_seconds = 0;
	onu_tod_sync.tod_nano_seconds = ((uint64_t)fw_param->tod_micro * 100000)
					+ ((uint64_t)fw_param->tod_clocks
					* ((float)clock_cycle / 1000));
	onu_tod_sync.tod_offset_pico_seconds = 0;
	onu_tod_sync.tod_quality = fw_param->tod_quality;

	err = ctx->onu_tod_sync(ctx->priv, &onu_tod_sync);

	switch (err) {
	case PON_STATUS_OK_NO_RESPONSE:
		return;
	case PON_STATUS_OK:
		err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK,
					       fw_param, sizeof(*fw_param),
					       PON_MBOX_C_MSG);
		break;
	default:
		break;
	}

	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("sending (N)ACK for ONU_TOD_SYNC failed %i",
			      err);
}

/* TODO: remove the definition when the proper one will be provided
 * in pon_mbox_drv:pon_ip_msg.h
 */
#ifndef PONFW_TWDM_US_WL_CONFIG_LENW
	#define PONFW_TWDM_US_WL_CONFIG_LENW 4
#endif

static void fapi_pon_twdm_us_wl_config(struct pon_ctx *ctx,
		struct nl_msg *msg,
		struct nlattr **attrs)
{
	struct ponfw_twdm_us_wl_config *fw_param;
	enum fapi_pon_errorcode ret;

	if (!ctx->twdm_wl_check)
		return;

	if (!attrs[PON_MBOX_A_DATA]
		|| nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	/* check if the switching is possible */
	ret = ctx->twdm_wl_check(ctx->priv, PON_TWDM_US_WL_CONF,
				 fw_param->uwlch_id, fw_param->us_execute);
	if (ret == PON_STATUS_OK_NO_RESPONSE)
		return;

	fw_param->us_valid = (ret == PON_STATUS_OK);

	if (fw_param->us_valid && fw_param->us_execute && ctx->twdm_wl_conf) {
		ret = ctx->twdm_wl_conf(ctx->priv, PON_TWDM_US_WL_CONF,
				fw_param->uwlch_id);
		if (ret != PON_STATUS_OK) {
			/* clear us_valid if the configuration failed */
			fw_param->us_valid = false;
			PON_DEBUG_ERR("Applying the TWDM_US_WL_CONFIG failed %i",
				      ret);
		}
	}

	ret = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, fw_param,
				       PONFW_TWDM_US_WL_CONFIG_LENW,
				       PON_MBOX_C_MSG);

	if (ret != PON_STATUS_OK)
		PON_DEBUG_ERR("Sending ACK for TWDM_US_WL_CONFIG failed %i",
			      ret);
}

/* TODO: remove the definition when the proper one will be provided
 * in pon_mbox_drv:pon_ip_msg.h
 */
#ifndef PONFW_TWDM_DS_WL_CONFIG_LENW
	#define PONFW_TWDM_DS_WL_CONFIG_LENW 4
#endif

static void fapi_pon_twdm_ds_wl_config(struct pon_ctx *ctx,
		struct nl_msg *msg,
		struct nlattr **attrs)
{
	struct ponfw_twdm_ds_wl_config *fw_param;
	enum fapi_pon_errorcode ret;
	uint8_t dswlch_id;

	if (!ctx->twdm_wl_check)
		return;

	if (!attrs[PON_MBOX_A_DATA]
		|| nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	/* check if the switching is possible */
	ret = ctx->twdm_wl_check(ctx->priv, PON_TWDM_DS_WL_CONF,
				 fw_param->dwlch_id, fw_param->ds_execute);
	if (ret == PON_STATUS_OK_NO_RESPONSE)
		return;

	fw_param->ds_valid = (ret == PON_STATUS_OK);

	if (fw_param->ds_valid && fw_param->ds_execute && ctx->twdm_wl_conf) {
		ret = ctx->twdm_wl_conf(ctx->priv, PON_TWDM_DS_WL_CONF,
				fw_param->dwlch_id);
		if (ret != PON_STATUS_OK) {
			/* clear ds_valid if the configuration failed */
			fw_param->ds_valid = false;
			PON_DEBUG_ERR("Applying the TWDM_DS_WL_CONFIG failed %i",
				      ret);
		} else {
			/* Apply the new wavelength channel for counters too */
			dswlch_id = fw_param->dwlch_id;
			ret = fapi_pon_twdm_counter_wlchid_set(ctx, dswlch_id);
			if (ret != PON_STATUS_OK)
				PON_DEBUG_ERR("Switch DS Channel ID"
						" for PM counters failed %i",
						ret);
		}
	}

	ret = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, fw_param,
				       PONFW_TWDM_DS_WL_CONFIG_LENW,
				       PON_MBOX_C_MSG);
	if (ret != PON_STATUS_OK)
		PON_DEBUG_ERR("Sending ACK for TWDM_DS_WL_CONFIG failed %i",
			      ret);
}

static void fapi_pon_lc_twdm_us_wl_tuning(struct pon_ctx *ctx,
		struct nl_msg *msg,
		struct nlattr **attrs)
{
	struct ponfw_twdm_us_wl_tuning *fw_param;
	enum fapi_pon_errorcode ret;

	if (!ctx->twdm_wl_tun)
		return;

	if (!attrs[PON_MBOX_A_DATA]
		|| nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	/* TODO: Extend this dummy in the future if necessary */
	ret = ctx->twdm_wl_tun(ctx->priv);
	if (ret != PON_STATUS_OK) {
		ret = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_NACK,
				NULL, 0, PON_MBOX_C_MSG);
		if (ret != PON_STATUS_OK)
			PON_DEBUG_ERR("Sending NACK for TWDM_US_WL_TUNING failed %i",
				      ret);
		return;
	}

	ret = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK,
			fw_param, sizeof(*fw_param),
			PON_MBOX_C_MSG);
	if (ret != PON_STATUS_OK)
		PON_DEBUG_ERR("Sending ACK for TWDM_US_WL_TUNING failed %i",
			      ret);
}

static void fapi_pon_listener_synce_status(struct pon_ctx *ctx,
		struct nl_msg *msg,
		struct nlattr **attrs)
{
	struct ponfw_synce_status *fw_param;
	struct pon_synce_status param;

	UNUSED(msg);

	if (!ctx->synce_status)
		return;

	if (!attrs[PON_MBOX_A_DATA]
		|| nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	param.stat = fw_param->synce_stat;

	ctx->synce_status(ctx->priv, &param);
}

static void fapi_pon_twdm_cal_record_status(struct pon_ctx *ctx,
					    struct nl_msg *msg,
					    struct nlattr **attrs)
{
	struct ponfw_twdm_onu_cal_record fw_param;
	struct pon_twdm_cal_record cal_record;
	enum fapi_pon_errorcode err;

	if (!ctx->pon_cal_record_state)
		return;

	err = ctx->pon_cal_record_state(ctx->priv, &cal_record);
	if (err != PON_STATUS_OK) {
		err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_NACK,
					       NULL, 0, PON_MBOX_C_MSG);
		if (err != PON_STATUS_OK)
			PON_DEBUG_ERR(
				"Sending NACK for ONU_CAL_RECORD failed %i",
				err);
		return;
	}

	pon_byte_copy(fw_param.cal_rec, cal_record.cal_rec,
		      sizeof(fw_param.cal_rec));

	err = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, &fw_param,
				       sizeof(fw_param),
				       PON_MBOX_C_MSG);
	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("Sending calibration status record failed %i",
			      err);
}

static void fapi_pon_listener_twdm_ch_prfl_status(struct pon_ctx *ctx,
						  struct nl_msg *msg,
						  struct nlattr **attrs)
{
	struct ponfw_twdm_channel_profile *fw_param;
	struct pon_twdm_channel_profile twdm_channel_profile = {0};
	enum fapi_pon_errorcode ret;

	if (!ctx->twdm_ch_profile)
		return;

	if (!attrs[PON_MBOX_A_DATA] ||
	    nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	twdm_channel_profile.ds_valid  = fw_param->ds_valid;
	twdm_channel_profile.us_valid  = fw_param->us_valid;
	twdm_channel_profile.dswlch_id = fw_param->dwlch_id;
	twdm_channel_profile.uswlch_id = fw_param->uwlch_id;

	ret = ctx->twdm_ch_profile(ctx->priv, &twdm_channel_profile);
	if (ret != PON_STATUS_OK) {
		ret = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_NACK,
					       NULL, 0, PON_MBOX_C_MSG);
		if (ret != PON_STATUS_OK)
			PON_DEBUG_ERR("Sending NACK for TWDM_CHANNEL_PROFILE failed %i",
				      ret);
		return;
	}
	ret = fapi_pon_send_msg_answer(ctx, msg, attrs, PONFW_ACK, NULL,
				       0, PON_MBOX_C_MSG);
	if (ret != PON_STATUS_OK)
		PON_DEBUG_ERR("Sending ACK for TWDM_CHANNEL_PROFILE failed %i",
			      ret);
}

static void fapi_pon_listener_onu_rnd_chl_tbl(struct pon_ctx *ctx,
					      struct nl_msg *msg,
					      struct nlattr **attrs)
{
	struct ponfw_xgtc_onu_rnd_chal_table *fw_param;
	struct pon_generic_auth_table param;

	UNUSED(msg);

	if (!ctx->onu_rnd_chl_tbl)
		return;

	if (!attrs[PON_MBOX_A_DATA]
		|| nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);
	param.size = MAX_AUTH_TABLE_SIZE;
	param.table = calloc(param.size, sizeof(uint8_t));
	if (!param.table) {
		PON_DEBUG_ERR("Cannot reserve memory");
		return;
	}
	pon_byte_copy(param.table, fw_param->onurct, param.size);

	ctx->onu_rnd_chl_tbl(ctx->priv, &param);
	free(param.table);
}

static void fapi_pon_listener_onu_auth_res_tbl(struct pon_ctx *ctx,
					       struct nl_msg *msg,
					       struct nlattr **attrs)
{
	struct ponfw_xgtc_onu_auth_result_table *fw_param;
	struct pon_generic_auth_table param;

	UNUSED(msg);

	if (!ctx->onu_auth_res_tbl)
		return;

	if (!attrs[PON_MBOX_A_DATA]
		|| nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);
	param.size = MAX_AUTH_TABLE_SIZE;
	param.table = calloc(param.size, sizeof(uint8_t));
	if (!param.table) {
		PON_DEBUG_ERR("Cannot reserve memory");
		return;
	}
	pon_byte_copy(param.table, fw_param->onuart, param.size);

	ctx->onu_auth_res_tbl(ctx->priv, &param);
	free(param.table);
}

static void fapi_pon_listener_twdm_config(struct pon_ctx *ctx,
					  struct nl_msg *msg,
					  struct nlattr **attrs)
{
	struct ponfw_twdm_config *fw_param;

	UNUSED(msg);

	if (!ctx->twdm_config)
		return;

	if (!attrs[PON_MBOX_A_DATA] ||
	    nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);

	ctx->twdm_config(ctx->priv, fw_param->cpi, fw_param->dwlch_id);
}

static void fapi_pon_unlink_all_answer(struct pon_ctx *ctx,
				       bool sw_ready)
{
	struct ponfw_alloc_id_unlink fw_param = {0};
	enum fapi_pon_errorcode err;

	fw_param.alloc_id = 0;
	fw_param.alloc_link_ref = 0;
	fw_param.all = sw_ready ? 1 : 0;

	err = fapi_pon_generic_set(ctx,
				   PONFW_ALLOC_ID_UNLINK_CMD_ID,
				   &fw_param,
				   sizeof(fw_param));
	if (err != PON_STATUS_OK)
		PON_DEBUG_ERR("Sending unlink all response(%u) failed %i",
			      fw_param.all, err);
}

static void fapi_pon_listener_unlink_all(struct pon_ctx *ctx,
					 struct nl_msg *msg,
					 struct nlattr **attrs)
{
	struct ponfw_alloc_id_unlink *fw_param;
	enum fapi_pon_errorcode ret;

	UNUSED(msg);

	if (!ctx->unlink_all)
		return;

	if (!attrs[PON_MBOX_A_DATA] ||
	    nla_len(attrs[PON_MBOX_A_DATA]) != sizeof(*fw_param)) {
		PON_DEBUG_ERR("Cannot read FW data");
		return;
	}

	fw_param = nla_data(attrs[PON_MBOX_A_DATA]);
	if (!fw_param->all)
		return;

	ret = ctx->unlink_all(ctx->priv);
	if (ret == PON_STATUS_OK_NO_RESPONSE)
		return;
	if (ret != PON_STATUS_OK)
		PON_DEBUG_ERR("Alloc ID unlink all failed %i", ret);

	/* Inform PON FW that the dequeue ports of the OMCC channel was
	 * cleaned up and it can leave O11 again if possible.
	 */
	fapi_pon_unlink_all_answer(ctx, false);
	/* Inform PON FW that all dequeue ports were cleaned up and it can
	 * create new T-Conts again.
	 */
	fapi_pon_unlink_all_answer(ctx, true);
}

fapi_pon_get_xgtc_log fapi_pon_register_xgtc_log(
					struct pon_ctx *ctx,
					fapi_pon_get_xgtc_log func)
{
	fapi_pon_get_xgtc_log func_old = ctx->xgtc_log;

	ctx->xgtc_log = func;

	return func_old;
}

fapi_pon_get_gtc_log fapi_pon_register_gtc_log(
					struct pon_ctx *ctx,
					fapi_pon_get_gtc_log func)
{
	fapi_pon_get_gtc_log func_old = ctx->gtc_log;

	ctx->gtc_log = func;

	return func_old;
}

fapi_pon_get_ploam_state fapi_pon_register_ploam_state(
					struct pon_ctx *ctx,
					fapi_pon_get_ploam_state func)
{
	fapi_pon_get_ploam_state func_old = ctx->ploam_state;

	ctx->ploam_state = func;

	return func_old;
}

fapi_pon_alarm_report fapi_pon_register_alarm_report(
					struct pon_ctx *ctx,
					fapi_pon_alarm_report func)
{
	fapi_pon_alarm_report func_old = ctx->alarm_report;

	ctx->alarm_report = func;

	return func_old;
}

fapi_pon_alarm_report fapi_pon_register_alarm_clear(
					struct pon_ctx *ctx,
					fapi_pon_alarm_report func)
{
	fapi_pon_alarm_report func_old = ctx->alarm_clear;

	ctx->alarm_clear = func;

	return func_old;
}

fapi_pon_get_xgtc_power_level fapi_pon_register_xgtc_power_level(
					struct pon_ctx *ctx,
					fapi_pon_get_xgtc_power_level func)
{
	fapi_pon_get_xgtc_power_level func_old = ctx->xgtc_power_level;

	ctx->xgtc_power_level = func;

	return func_old;
}

fapi_pon_get_onu_tod_sync fapi_pon_register_onu_tod_sync(
						struct pon_ctx *ctx,
						fapi_pon_get_onu_tod_sync func)
{
	fapi_pon_get_onu_tod_sync func_old = ctx->onu_tod_sync;

	ctx->onu_tod_sync = func;

	return func_old;
}

fapi_pon_twdm_wl_check fapi_pon_register_twdm_wl_check(
						struct pon_ctx *ctx,
						fapi_pon_twdm_wl_check func)
{
	fapi_pon_twdm_wl_check func_old = ctx->twdm_wl_check;

	ctx->twdm_wl_check = func;

	return func_old;
}

fapi_pon_twdm_wl_config fapi_pon_register_twdm_wl_config(
						struct pon_ctx *ctx,
						fapi_pon_twdm_wl_config func)
{
	fapi_pon_twdm_wl_config func_old = ctx->twdm_wl_conf;

	ctx->twdm_wl_conf = func;

	return func_old;
}

fapi_pon_twdm_us_wl_tuning fapi_pon_register_twdm_us_wl_tuning(
						struct pon_ctx *ctx,
						fapi_pon_twdm_us_wl_tuning func)
{
	fapi_pon_twdm_us_wl_tuning func_old = ctx->twdm_wl_tun;

	ctx->twdm_wl_tun = func;

	return func_old;
}

fapi_pon_synce_status fapi_pon_register_synce_status(struct pon_ctx *ctx,
						fapi_pon_synce_status func)
{
	fapi_pon_synce_status func_old = ctx->synce_status;

	ctx->synce_status = func;

	return func_old;
}

fapi_pon_get_cal_record_state
fapi_pon_register_cal_record_state(struct pon_ctx *ctx,
				   fapi_pon_get_cal_record_state func)
{
	fapi_pon_get_cal_record_state func_old = ctx->pon_cal_record_state;

	ctx->pon_cal_record_state = func;

	return func_old;
}

fapi_pon_twdm_ch_profile
fapi_pon_register_twdm_ch_profile(struct pon_ctx *ctx,
				  fapi_pon_twdm_ch_profile func)
{
	fapi_pon_twdm_ch_profile func_old = ctx->twdm_ch_profile;

	ctx->twdm_ch_profile = func;

	return func_old;
}

fapi_pon_twdm_config
fapi_pon_register_twdm_config(struct pon_ctx *ctx,
			      fapi_pon_twdm_config func)
{
	fapi_pon_twdm_config func_old = ctx->twdm_config;

	ctx->twdm_config = func;

	return func_old;
}

fapi_pon_onu_rnd_chl_tbl
fapi_pon_register_onu_rnd_chl_tbl(struct pon_ctx *ctx,
				  fapi_pon_onu_rnd_chl_tbl func)
{
	fapi_pon_onu_rnd_chl_tbl func_old = ctx->onu_rnd_chl_tbl;

	ctx->onu_rnd_chl_tbl = func;

	return func_old;
}

fapi_pon_onu_auth_res_tbl
fapi_pon_register_onu_auth_res_tbl(struct pon_ctx *ctx,
				   fapi_pon_onu_auth_res_tbl func)
{
	fapi_pon_onu_auth_res_tbl func_old = ctx->onu_auth_res_tbl;

	ctx->onu_auth_res_tbl = func;

	return func_old;
}

fapi_pon_unlink_all
fapi_pon_register_unlink_all(struct pon_ctx *ctx,
			     fapi_pon_unlink_all func)
{
	fapi_pon_unlink_all func_old = ctx->unlink_all;

	ctx->unlink_all = func;

	return func_old;
}

void fapi_pon_listener_msg(uint16_t command, struct pon_ctx *ctx,
			   struct nl_msg *msg, struct nlattr **attrs)
{
	switch (command) {
	case PONFW_XGTC_PLOAM_LOG_CMD_ID:
		fapi_pon_listener_xgtc_log(ctx, msg, attrs);
		return;
	case PONFW_GTC_PLOAM_LOG_CMD_ID:
		fapi_pon_listener_gtc_log(ctx, msg, attrs);
		return;
	case PONFW_PLOAM_STATE_CMD_ID:
		fapi_pon_listener_ploam_state(ctx, msg, attrs);
		return;
	case PONFW_REPORT_ALARM_CMD_ID:
		fapi_pon_listener_alarm_report(ctx, msg, attrs);
		return;
	case PONFW_CLEAR_ALARM_CMD_ID:
		fapi_pon_listener_alarm_clear(ctx, msg, attrs);
		return;
	case PONFW_TX_POWER_LEVEL_REQ_CMD_ID:
		fapi_pon_listener_xgtc_power_level(ctx, msg, attrs);
		return;
	case PONFW_ONU_TOD_SYNC_CMD_ID:
		fapi_pon_listener_onu_tod_sync(ctx, msg, attrs);
		return;
	case PONFW_TWDM_US_WL_CONFIG_CMD_ID:
		fapi_pon_twdm_us_wl_config(ctx, msg, attrs);
		return;
	case PONFW_TWDM_DS_WL_CONFIG_CMD_ID:
		fapi_pon_twdm_ds_wl_config(ctx, msg, attrs);
		return;
	case PONFW_TWDM_US_WL_TUNING_CMD_ID:
		fapi_pon_lc_twdm_us_wl_tuning(ctx, msg, attrs);
		return;
	case PONFW_TWDM_ONU_CAL_RECORD_CMD_ID:
		fapi_pon_twdm_cal_record_status(ctx, msg, attrs);
		return;
	case PONFW_SYNCE_STATUS_CMD_ID:
		fapi_pon_listener_synce_status(ctx, msg, attrs);
		return;
	case PONFW_TWDM_CHANNEL_PROFILE_CMD_ID:
		fapi_pon_listener_twdm_ch_prfl_status(ctx, msg, attrs);
		return;
	case PONFW_TWDM_CONFIG_CMD_ID:
		fapi_pon_listener_twdm_config(ctx, msg, attrs);
		return;
	case PONFW_XGTC_ONU_RND_CHAL_TABLE_CMD_ID:
		fapi_pon_listener_onu_rnd_chl_tbl(ctx, msg, attrs);
		return;
	case PONFW_XGTC_ONU_AUTH_RESULT_TABLE_CMD_ID:
		fapi_pon_listener_onu_auth_res_tbl(ctx, msg, attrs);
		return;
	case PONFW_ALLOC_ID_UNLINK_CMD_ID:
		fapi_pon_listener_unlink_all(ctx, msg, attrs);
		return;
	case PONFW_ALLOC_ID_LINK_CMD_ID:
		return;
	default:
		PON_DEBUG_ERR("got unknown event: 0x%x", command);
		return;
	}
}

fapi_pon_fw_init_complete fapi_pon_register_fw_init_complete(
					struct pon_ctx *ctx,
					fapi_pon_fw_init_complete func)
{
	fapi_pon_fw_init_complete func_old = ctx->fw_init_complete;

	ctx->fw_init_complete = func;

	return func_old;
}

enum fapi_pon_errorcode
fapi_pon_fw_init_complete_msg(struct pon_ctx *ctx,
			      struct nl_msg *msg,
			      struct nlattr **attrs)
{
	UNUSED(attrs);
	UNUSED(msg);

	/* invalidate the cache */
	ctx->caps_valid = 0;
	ctx->ver_valid = 0;
	ctx->limits_valid = 0;
	ctx->mode_valid = 0;
	ctx->ext_cal_valid = 0;
	ctx->actual_plat_type = 0;

	if (ctx->fw_init_complete)
		ctx->fw_init_complete(ctx->priv);
	return PON_STATUS_OK;
}
