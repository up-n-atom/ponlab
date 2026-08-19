/*****************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifdef LINUX
#include <fcntl.h>
#endif

#ifdef HAVE_CONFIG_H
#include "pon_config.h"
#endif
#include "pon_adapter.h"
#include "pon_adapter_config.h"

#include "fapi_pon_pa_common.h"
#include "fapi_pon_pa_register.h"

#include "fapi_pon.h"
#include "fapi_pon_error.h"

#include <pthread.h>
#include <string.h>

#define DMI_STATUS_CONTROL	110
#define DMI_CONTROL_SOFT_TX_DISABLE	(1 << 6)

static bool is_operational_state(int state)
{
	return state >= 50 && state <= 69;
}

static bool is_serial_number_acquisition_phase(int state)
{
	/* Serial Number Acquisition Phase activation/PLOAM states are
	 * O2-3 (standards other than G-PON) and O3 (G-PON).
	 * In our implementation these correspond to states 23 and 30.
	 */
	return state == 23 || state == 30;
}

static enum fapi_pon_errorcode set_soft_tx_disable(struct pon_ctx *ponevt_ctx,
						   bool tx_disabled)
{
	unsigned char data, tmp;
	enum fapi_pon_errorcode ret = PON_STATUS_OK;
	int loops = 2;

	/* Try setting the soft tx disable bit multiple times,
	 * exit early when it is already set to the expected value
	 */
	while (loops--) {
		ret = fapi_pon_eeprom_data_get(ponevt_ctx, PON_DDMI_A2, &data,
					       DMI_STATUS_CONTROL,
					       sizeof(data));

		/* Silently ignore error if eeprom access is not possible */
		if (ret == PON_STATUS_INPUT_ERR)
			return PON_STATUS_OK;

		if (ret != PON_STATUS_OK) {
			dbg_wrn("Could not read from dmi eeprom file!\n");
			return ret;
		}

		tmp = data;
		if (tx_disabled)
			data |= DMI_CONTROL_SOFT_TX_DISABLE;
		else
			data &= ~DMI_CONTROL_SOFT_TX_DISABLE;

		dbg_prn("tx %s -> change 0x%02X to 0x%02X\n",
			tx_disabled ? "disable" : "enable", tmp, data);

		/* Skip writing when bit is already at expected level */
		if (tmp == data)
			return PON_STATUS_OK;

		ret = fapi_pon_eeprom_data_set(ponevt_ctx, PON_DDMI_A2, &data,
					       DMI_STATUS_CONTROL,
					       sizeof(data));
	}

	return ret;
}

static void
handle_ploam_state_change(void *priv,
			  const struct pon_ploam_state_evt *ploam_state)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	enum fapi_pon_errorcode ret;

	/* recheck and send ANI-G alarms when reaching operational state */
	if (is_operational_state(ploam_state->current) &&
	    !is_operational_state(ploam_state->previous))
		pon_ani_g_alarm_recheck(ctx);
	/* Some SFP sticks have by default the laser transmission disabled by
	 * the soft tx disable bit. When such stick gets inserted, upon initial
	 * LOF/LODS clear event it is attempted to clear the bit, but the
	 * access to SFP's EEPROM is NOT yet functioning properly (e.g. readout
	 * of register 110 is 0x00 which is wrong, but there is no indication
	 * that this is a faulty readout) at that time. So to counter this, we
	 * here try again to clear it. Upon entering the serial number
	 * acquisition phase is the last chance to enable transmission and
	 * hopefully accessing the EEPROM works by now. If clearing of the bit
	 * succeeded on initial LOF/LODS clear event anyhow, this attempt here
	 * will be a no-op.
	 */
	if (is_serial_number_acquisition_phase(ploam_state->current)) {
		if ((cfg->sfp_tweaks & SFP_TWEAK_SKIP_SOFT_TX_DISABLE) == 0) {
			dbg_prn("Serial Number acquisition phase, attempting to clear Soft TX Disable bit\n");
			/* Clear the soft tx disable bit in the DMI EEPROM */
			ret = set_soft_tx_disable(ctx->ponevt_ctx, false);
			if (ret != PON_STATUS_OK)
				dbg_err_fn_ret(set_soft_tx_disable, ret);
		}
	}

	if (!ctx->event_handlers.ploam_state)
		return;

	/* Compare the main state only, ignore the sub-state. */
	if (ploam_state->previous / 10 != ploam_state->current / 10)
		ctx->event_handlers.ploam_state(ctx->hl_ctx,
				ploam_state->previous / 10,
				ploam_state->current / 10);
}

static enum fapi_pon_errorcode handle_xgtc_power_level(void *priv,
						       uint32_t oper,
						       uint32_t *attenuation)
{
	/* TODO:
	 * This function just return errorcode when requested to change the
	 * power level, assuming that the optical interface is not able to
	 * change or report the power level. Return a success with a power
	 * level of 0 when asked for the current power level as it is not
	 * allowed to return NACK here.
	 * It shall be enhanced once optical modules are available which can
	 * modify the transmit power and the related programming is known.
	 */

	UNUSED(priv);

	*attenuation = 0;

	if (oper == 3)
		return PON_STATUS_OK;

	return PON_STATUS_SUPPORT;
}

static void
handle_active_alarms(void *priv, const struct pon_alarm_status *alarms)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct pon_ctx *ponevt_ctx = ctx->ponevt_ctx;
	struct pon_gpon_status gpon_status;
	struct pon_omci_ik pon_omci_ik;
	struct pa_omci_ik pa_omci_ik;
	enum fapi_pon_errorcode ret;
	enum pon_adapter_errno err;

	switch (alarms->alarm_id) {
	case PON_ALARM_EDGE_OIKC:
		if (ctx->event_handlers.omci_ik_update) {
			ret = fapi_pon_omci_ik_get(ponevt_ctx, &pon_omci_ik);
			if (ret == PON_STATUS_OK) {
				if (memcpy_s(&pa_omci_ik, sizeof(pa_omci_ik),
					     &pon_omci_ik,
					     sizeof(pon_omci_ik))) {
					dbg_err_fn(memcpy_s);
					return;
				}
				ctx->event_handlers.omci_ik_update(ctx->hl_ctx,
								   &pa_omci_ik);
			}
		}
		break;

	case PON_ALARM_EDGE_ASC:
		if (ctx->event_handlers.auth_status_chg) {
			ret = fapi_pon_gpon_status_get(ponevt_ctx, &gpon_status);
			if (ret == PON_STATUS_OK)
				ctx->event_handlers.auth_status_chg(
					ctx->hl_ctx, gpon_status.auth_status);
		}
		break;

	case PON_ALARM_EDGE_CPI_TO:
		ctx->cfg.twdm.ch_partition_index = 0;

		err = pon_pa_config_write(ctx, "optic", "twdm",
					  "ch_partition_index", "0", true);
		if (err != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(pon_pa_config_write, err);

		if (ctx->event_handlers.ch_partition_index_reset)
			ctx->event_handlers.ch_partition_index_reset(
				ctx->hl_ctx);
		break;

	case PON_ALARM_STATIC_SF:
		ctx->ani_g_data.signal_fail = PON_ALARM_EN;
		break;

	case PON_ALARM_STATIC_SD:
		ctx->ani_g_data.signal_degrade = PON_ALARM_EN;
		break;

	default:
		break;
	}

	if (ctx->event_handlers.pon_alarm)
		ctx->event_handlers.pon_alarm(ctx->hl_ctx, alarms->alarm_id,
					      alarms->alarm_status);
}

static void
handle_clear_alarms(void *priv, const struct pon_alarm_status *alarms)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	enum fapi_pon_errorcode ret;

	switch (alarms->alarm_id) {
	case PON_ALARM_STATIC_SF:
		ctx->ani_g_data.signal_fail = PON_ALARM_DIS;
		break;

	case PON_ALARM_STATIC_SD:
		ctx->ani_g_data.signal_degrade = PON_ALARM_DIS;
		break;

	case PON_ALARM_STATIC_LODS:
	case PON_ALARM_STATIC_LOF:
		/* Clearing the Soft TX Disable when the LODS (XGS-PON) or
		 * LOF (GPON) alarm is cleared.
		 * This is done to allow the transceiver to re-enable the TX
		 * path if it was previously disabled. We disable TX manually
		 * before loading the firmware, but this can also happen when a
		 * transceiver is re-inserted after the firmware is loaded and
		 * it has the TX disabled by default.
		 */
		if ((cfg->sfp_tweaks & SFP_TWEAK_SKIP_SOFT_TX_DISABLE) == 0) {
			/* Clear the soft tx disable bit in the DMI EEPROM */
			ret = set_soft_tx_disable(ctx->ponevt_ctx, false);
			if (ret != PON_STATUS_OK)
				dbg_err_fn_ret(set_soft_tx_disable, ret);
		}
		break;

	default:
		break;
	}

	if (ctx->event_handlers.pon_alarm)
		ctx->event_handlers.pon_alarm(ctx->hl_ctx, alarms->alarm_id,
					      alarms->alarm_status);
}

static enum fapi_pon_errorcode
	handle_twdm_wl_check(void *priv,
			     const enum pon_twdm_oper_type oper_type,
			     uint8_t ch_id, bool execute)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;

	/* dwlch_id and uwlch_id values range = {0..7}
	 *   0 - channel id = 1
	 *       ...
	 *   7 - channel id = 8
	 */

	/* check if requested channel id is available */
	if (!(cfg->twdm_channel_mask & (1 << ch_id))) {
		dbg_wrn("The channel id %d do not match the channel mask 0x%02x - oper type %d\n",
			ch_id, cfg->twdm_channel_mask, oper_type);
		return PON_STATUS_SUPPORT;
	}

	/* Check only if the wavelength shall be physically changed */
	if (!execute)
		return PON_STATUS_OK;

	/* For config method 3 check if given upstream channel id
	 * is the same as previously set downstream channel id.
	 */
	if (cfg->twdm_config_method == PON_TWDM_CONF_METHOD3) {
		if ((oper_type == PON_TWDM_US_WL_CONF) &&
		    (ch_id != ctx->used_dwlch_id)) {
			dbg_wrn("US channel id different than used DS channel id\n");
			return PON_STATUS_VALUE_RANGE_ERR;
		}
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
	handle_twdm_wl_config(void *priv,
			      const enum pon_twdm_oper_type oper_type,
			      uint8_t ch_id)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct pon_ctx *ponevt_ctx = ctx->ponevt_ctx;

	switch (oper_type) {
	case PON_TWDM_DS_WL_CONF:
		return pon_twdm_write_ds(ctx, ponevt_ctx, ch_id);
	case PON_TWDM_US_WL_CONF:
		return pon_twdm_write_us(ctx, ponevt_ctx, ch_id);
	case PON_TWDM_US_WL_TUNING:
		return pon_twdm_tuning(ctx, ponevt_ctx, ch_id);
	default:
		/* Development error - Should never happen. */
		dbg_err("Wrong operation type!\n");
		return PON_STATUS_ERR;
	}
}

static enum fapi_pon_errorcode
	handle_cal_record_state(void *priv,
				struct pon_twdm_cal_record *cal_record)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	uint64_t cal_stat = cfg->cal_status_record;
	int i;

	/* Mapping of channels to the correct byte index
	 * cal_rec[0] is the MSB from the cal_stat
	 */
	for (i = 0; i < ARRAY_SIZE(cal_record->cal_rec); i++)
		cal_record->cal_rec[i] = ((cal_stat >> 8 * (7 - i)) & 0xFF);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
	handle_twdm_us_wl_tuning(void *priv)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	struct pon_ctx *ponevt_ctx = ctx->ponevt_ctx;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;

	/* Return error when tuning is not supported by transceiver */
	if (!cfg->twdm_tuning) {
		dbg_wrn("Tuning is not supported by transceiver!\n");
		return PON_STATUS_ERR;
	}

	return pon_twdm_tuning(ctx, ponevt_ctx, 0);
}

static enum fapi_pon_errorcode
	handle_twdm_ch_profile(void *priv,
			       struct pon_twdm_channel_profile *twdm_ch_prfl)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;
	bool is_active = false;
	uint8_t ch_index;

	if (!ctx->event_handlers.twdm_ch_profile_update)
		return PON_STATUS_ERR;

	if (twdm_ch_prfl->ds_valid && twdm_ch_prfl->us_valid)
		is_active = true;

	if (twdm_ch_prfl->dswlch_id != twdm_ch_prfl->uswlch_id) {
		dbg_wrn("dswlch_id (%d) does not equal uswlch_id (%d)\n",
			twdm_ch_prfl->dswlch_id, twdm_ch_prfl->uswlch_id);
		return PON_STATUS_ERR;
	}
	ch_index = twdm_ch_prfl->dswlch_id;

	ctx->event_handlers.twdm_ch_profile_update(ctx->hl_ctx, is_active,
						   ch_index);
	return PON_STATUS_OK;
}

static void
handle_onu_rnd_chl_tbl(void *priv,
		       const struct pon_generic_auth_table *onu_rnd_chl_tbl)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;

	if (!ctx->event_handlers.onu_rnd_chl_tbl)
		return;

	ctx->event_handlers.onu_rnd_chl_tbl(ctx->hl_ctx,
					    onu_rnd_chl_tbl->table,
					    onu_rnd_chl_tbl->size);
}

static void
handle_onu_auth_res_tbl(void *priv,
			const struct pon_generic_auth_table *onu_auth_res_tbl)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;

	if (!ctx->event_handlers.auth_result_rdy)
		return;

	ctx->event_handlers.auth_result_rdy(ctx->hl_ctx,
					    onu_auth_res_tbl->table,
					    onu_auth_res_tbl->size);
}

static enum fapi_pon_errorcode handle_unlink_all(void *priv)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;

	if (!ctx->event_handlers.mib_reset)
		return PON_STATUS_ERR;

	ctx->event_handlers.mib_reset(ctx->hl_ctx);

	return PON_STATUS_OK;
}

/*
 * This function is called when the firmware was successfully loaded,
 * also after a reset done by some other program.
 *
 * Set pon_ctx based on the thread where it is called from. If this function
 * gets called from the event handing thread use the ponevt_ctx, if it is
 * called from the main omci action thread use pon_ctx.
 */
static void init_ponip_fw(struct fapi_pon_wrapper_ctx *ctx,
			  struct pon_ctx *pon_ctx)
{
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct pon_ctx *ponevt_ctx = ctx->ponevt_ctx;
	struct pon_gpon_rerange_cfg rerange_cfg = {0};
	struct pon_omci_cfg omci_cfg = {0};
	struct pon_optic_cfg optic_cfg = {0};
	struct pon_gpon_cfg gpon_onu_cfg = {0};
	struct pon_iop_cfg iop_cfg = {0};
	struct pon_enc_cfg enc_cfg = {0};
	enum fapi_pon_errorcode ret = PON_STATUS_OK;

	fapi_pon_register_alarm_report(ponevt_ctx, handle_active_alarms);
	fapi_pon_register_alarm_clear(ponevt_ctx, handle_clear_alarms);
	fapi_pon_register_ploam_state(ponevt_ctx, handle_ploam_state_change);
	fapi_pon_register_xgtc_power_level(ponevt_ctx, handle_xgtc_power_level);
	fapi_pon_register_twdm_wl_check(ponevt_ctx, handle_twdm_wl_check);
	fapi_pon_register_twdm_wl_config(ponevt_ctx, handle_twdm_wl_config);
	fapi_pon_register_twdm_us_wl_tuning(ponevt_ctx,
					    handle_twdm_us_wl_tuning);
	fapi_pon_register_cal_record_state(ponevt_ctx, handle_cal_record_state);
	fapi_pon_register_twdm_ch_profile(ponevt_ctx, handle_twdm_ch_profile);
	fapi_pon_register_onu_rnd_chl_tbl(ponevt_ctx,
					  handle_onu_rnd_chl_tbl);
	fapi_pon_register_onu_auth_res_tbl(ponevt_ctx,
					   handle_onu_auth_res_tbl);
	fapi_pon_register_unlink_all(ponevt_ctx, handle_unlink_all);

	/* Trigger OMCI MIB reset for FW restart */
	if (ctx->event_handlers.mib_reset)
		ctx->event_handlers.mib_reset(ctx->hl_ctx);

	if (memcpy_s(omci_cfg.mac_sa, sizeof(omci_cfg.mac_sa),
		     cfg->mac_sa, sizeof(cfg->mac_sa)))
		ret = PON_STATUS_ERR;

	if (memcpy_s(omci_cfg.mac_da, sizeof(omci_cfg.mac_da),
		     cfg->mac_da, sizeof(cfg->mac_da)))
		ret = PON_STATUS_ERR;

	omci_cfg.ethertype = cfg->ethertype;

	if (memcpy_s(omci_cfg.protocol, sizeof(omci_cfg.protocol),
		     cfg->protocol, sizeof(cfg->protocol)))
		ret = PON_STATUS_ERR;

	if (ret != PON_STATUS_OK) {
		dbg_err_fn(memcpy_s);
		goto err;
	}

	if (cfg->mode == PON_MODE_984_GPON ||
	    cfg->mode == PON_MODE_987_XGPON ||
	    cfg->mode == PON_MODE_9807_XGSPON ||
	    cfg->mode == PON_MODE_989_NGPON2_2G5 ||
	    cfg->mode == PON_MODE_989_NGPON2_10G) {
		iop_cfg.iop_mask = cfg->iop_mask;
		ret = fapi_pon_iop_cfg_set(pon_ctx, &iop_cfg);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(fapi_pon_iop_cfg_set, ret);
			goto err;
		}

		/* By default PON is enabled, only disable it if requested */
		if (!cfg->enabled) {
			rerange_cfg.psmdis = 1;
			rerange_cfg.txdis = 1;
			ret = fapi_pon_gpon_rerange_cfg_set(pon_ctx,
							    &rerange_cfg);
			if (ret != PON_STATUS_OK) {
				dbg_err_fn_ret(fapi_pon_gpon_rerange_cfg_set,
					       ret);
				goto err;
			}
		}
	}

	if (cfg->mode == PON_MODE_989_NGPON2_10G ||
	    cfg->mode == PON_MODE_989_NGPON2_2G5) {
		/* Set Wavelengtch selection LOCK
		 * based on configured config_method
		 */
		cfg->twdm.wl_lock =
			(cfg->twdm_config_method == PON_TWDM_CONF_METHOD3);
		/* This function must be called before fapi_pon_optic_cfg_set()
		 * and fapi_pon_gpon_cfg_set(), otherwise it will fail with
		 * PON_STATUS_FW_STATE
		 */
		ret = fapi_pon_twdm_cfg_set(pon_ctx, &cfg->twdm);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(fapi_pon_twdm_cfg_set, ret);
			goto err;
		}

		ret = fapi_pon_twdm_wlse_config_set(pon_ctx,
						    &cfg->twdm_wlse_config);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(fapi_pon_twdm_wlse_config_set, ret);
			goto err;
		}
	}

	ret = fapi_pon_serdes_cfg_set(pon_ctx, &cfg->serdes);
	if (ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_serdes_cfg_set, ret);
		goto err;
	}

	/* Function must be called before fapi_pon_optic_cfg_set() */
	ret = fapi_pon_gpio_cfg_set(pon_ctx, &cfg->gpio);
	if (ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_gpio_cfg_set, ret);
		goto err;
	}

	if (memcpy_s(&optic_cfg, sizeof(optic_cfg), &cfg->optic,
		     sizeof(cfg->optic)))
		ret = PON_STATUS_ERR;

	/* Add time-offset values */
	optic_cfg.laser_setup_time += cfg->optic_offsets.laser_setup_time;
	optic_cfg.laser_hold_time += cfg->optic_offsets.laser_hold_time;
	optic_cfg.serdes_setup_time += cfg->optic_offsets.serdes_setup_time;
	optic_cfg.serdes_hold_time += cfg->optic_offsets.serdes_hold_time;
	optic_cfg.bias_setup_time += cfg->optic_offsets.bias_setup_time;
	optic_cfg.bias_hold_time += cfg->optic_offsets.bias_hold_time;

	ret = fapi_pon_optic_cfg_set(pon_ctx, &optic_cfg);
	if (ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_optic_cfg_set, ret);
		goto err;
	}

	if (cfg->mode == PON_MODE_984_GPON ||
	    cfg->mode == PON_MODE_987_XGPON ||
	    cfg->mode == PON_MODE_9807_XGSPON ||
	    cfg->mode == PON_MODE_989_NGPON2_2G5 ||
	    cfg->mode == PON_MODE_989_NGPON2_10G) {
		ret = fapi_pon_omci_cfg_set(pon_ctx, &omci_cfg);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(fapi_pon_omci_cfg_set, ret);
			goto err;
		}

		if (memcpy_s(gpon_onu_cfg.serial_no,
			     sizeof(gpon_onu_cfg.serial_no),
			     cfg->serial_no,
			     sizeof(cfg->serial_no)))
			ret = PON_STATUS_ERR;

		if (memcpy_s(gpon_onu_cfg.reg_id, sizeof(gpon_onu_cfg.reg_id),
			     cfg->reg_id, sizeof(cfg->reg_id)))
			ret = PON_STATUS_ERR;

		/* PLOAM password uses the LSB part of the Registration ID
		 * as the value of the password.
		 */
		if (memcpy_s(gpon_onu_cfg.password, PON_PASSWD_SIZE,
			     cfg->reg_id + (PON_REG_ID_SIZE - PON_PASSWD_SIZE),
			     PON_PASSWD_SIZE))
			ret = PON_STATUS_ERR;

		if (ret != PON_STATUS_OK) {
			dbg_err_fn(memcpy_s);
			goto err;
		}

		gpon_onu_cfg.ident = cfg->ident;
		gpon_onu_cfg.stop = cfg->ploam_emerg_stop_state;
		gpon_onu_cfg.plev_cap = cfg->plev_cap;
		gpon_onu_cfg.ploam_timeout_0 = cfg->ploam_timeout_0;
		gpon_onu_cfg.ploam_timeout_1 = cfg->ploam_timeout_1;
		gpon_onu_cfg.ploam_timeout_2 = cfg->ploam_timeout_2;
		gpon_onu_cfg.ploam_timeout_3 = cfg->ploam_timeout_3;
		gpon_onu_cfg.ploam_timeout_4 = cfg->ploam_timeout_4;
		gpon_onu_cfg.ploam_timeout_5 = cfg->ploam_timeout_5;
		gpon_onu_cfg.ploam_timeout_6 = cfg->ploam_timeout_6;
		/* 0 is an invalid value, set it to 80 instead.
		 * 0 was our default value for a long time.
		 */
		if (cfg->ploam_timeout_cpl == 0)
			gpon_onu_cfg.ploam_timeout_cpl = 80;
		else
			gpon_onu_cfg.ploam_timeout_cpl = cfg->ploam_timeout_cpl;

		if (cfg->mode == PON_MODE_989_NGPON2_2G5 ||
		    cfg->mode == PON_MODE_989_NGPON2_10G) {
			gpon_onu_cfg.ploam_timeout_tpd = cfg->ploam_timeout_tpd;
			gpon_onu_cfg.ploam_timeout_cpi = cfg->ploam_timeout_cpi;
		} else {
			gpon_onu_cfg.ploam_timeout_tpd = 0;
			gpon_onu_cfg.ploam_timeout_cpi = 0;
		}

		gpon_onu_cfg.tdm_coexistence = cfg->tdm_coexistence;
		gpon_onu_cfg.mode = cfg->mode;
		gpon_onu_cfg.dg_dis = cfg->dg_dis;
		gpon_onu_cfg.ds_fcs_en = cfg->ds_fcs_en;
		gpon_onu_cfg.ds_ts_dis = cfg->ds_ts_dis;

		ret = fapi_pon_gpon_cfg_set(pon_ctx, &gpon_onu_cfg);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(fapi_pon_gpon_cfg_set, ret);
			goto err;
		}

		enc_cfg.enc_mode = cfg->auth_mode;
		enc_cfg.key_size = cfg->psk_size;
		if (memcpy_s(enc_cfg.psk, sizeof(enc_cfg.psk),
			     cfg->psk, (cfg->psk_size / 8)))
			ret = PON_STATUS_ERR;

		if (ret != PON_STATUS_OK) {
			dbg_err_fn(memcpy_s);
			goto err;
		}
	}

	if (cfg->mode == PON_MODE_987_XGPON ||
	    cfg->mode == PON_MODE_9807_XGSPON) {
		ret = fapi_pon_auth_enc_cfg_set(pon_ctx, &enc_cfg);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(fapi_pon_auth_enc_cfg_set, ret);
			goto err;
		}
	}

err:
	ctx->init_result = ret;
}

static enum fapi_pon_errorcode fw_init_complete(void *priv)
{
	struct fapi_pon_wrapper_ctx *ctx = priv;

	/* The FW gets only loaded in case the MEs are already initialized and
	 * events can be handled, otherwise we could lose some events.
	 */
	pthread_mutex_lock(&ctx->init_lock);
	if (ctx->init_done_omci)
		init_ponip_fw(ctx, ctx->ponevt_ctx);
	ctx->init_done_fw = true;
	pthread_mutex_unlock(&ctx->init_lock);

	sem_post(&ctx->init_done);

	return PON_STATUS_OK;
}

static void *
event_handling_thread(void *arg)
{
	struct fapi_pon_wrapper_ctx *ctx = arg;
	enum fapi_pon_errorcode ret;
	int err;

	err = pthread_setname_np(pthread_self(), "ponevt");
	if (err)
		dbg_err("%s: Can't set name <%s> for thread\n",
			__func__, "ponevt");

	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

#ifdef PON_LIB_SIMULATOR
	/* In pon_simulation "reset" will be completed by this time */
	sem_post(&ctx->init_done);
#endif

	for (;;) {
		pthread_testcancel();
		ret = fapi_pon_listener_run(ctx->ponevt_ctx);
		if (ret != PON_STATUS_OK)
			break;
	}

	return EXIT_SUCCESS;
}

/* This writes the initial configuration to the FW and registers the callback
 * handlers. After this configuration the FW will start its state machine and
 * try to get to O5. It is checked if the FW was already loaded successfully.
 */
enum pon_adapter_errno pon_pa_event_start(struct fapi_pon_wrapper_ctx *ctx)
{
	pthread_mutex_lock(&ctx->init_lock);
	if (ctx->init_done_fw)
		init_ponip_fw(ctx, ctx->pon_ctx);
	ctx->init_done_omci = true;
	pthread_mutex_unlock(&ctx->init_lock);

	if (ctx->init_result != PON_STATUS_OK)
		return PON_ADAPTER_ERROR;

	ctx->used_dwlch_id = 0;

	return PON_ADAPTER_SUCCESS;
}

/* This starts the loading of the FW and waits till it is loaded. */
enum pon_adapter_errno
pon_pa_event_handling_init(struct fapi_pon_wrapper_ctx *ctx)
{
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct pon_ctx *ponevt_ctx = NULL;
	enum fapi_pon_errorcode ret;
	struct timespec ts;
	int err;

	sem_init(&ctx->init_done, 0, 0);
	pthread_mutex_init(&ctx->init_lock, NULL);

	ret = fapi_pon_open(&ponevt_ctx);
	if (ret != PON_STATUS_OK)
		return EXIT_FAILURE;

	if (strnlen_s(cfg->eeprom_serial_id,
		      sizeof(cfg->eeprom_serial_id)) > 0) {
		ret = fapi_pon_eeprom_open(ponevt_ctx, PON_DDMI_A0,
					   cfg->eeprom_serial_id);
		if (ret != PON_STATUS_OK) {
			fapi_pon_close(ponevt_ctx);
			return EXIT_FAILURE;
		}
	}

	if (strnlen_s(cfg->eeprom_dmi,
		      sizeof(cfg->eeprom_dmi)) > 0) {
		ret = fapi_pon_eeprom_open(ponevt_ctx, PON_DDMI_A2,
					   cfg->eeprom_dmi);
		if (ret != PON_STATUS_OK) {
			fapi_pon_close(ponevt_ctx);
			return EXIT_FAILURE;
		}
	}

	ctx->ponevt_ctx = ponevt_ctx;

	ret = fapi_pon_listener_connect(ponevt_ctx, ctx);
	if (ret != PON_STATUS_OK) {
		dbg_err("pa_events: connecting event listener failed\n");
		fapi_pon_close(ponevt_ctx);
		return EXIT_FAILURE;
	}

	/* Set the soft tx disable bit in the DMI EEPROM.
	 * By this we ensure that the TX path is disabled before loading
	 * the firmware and we don't have the laser active before the firmware
	 * is active and correctly configured.
	 */
	if ((cfg->sfp_tweaks & SFP_TWEAK_SKIP_SOFT_TX_DISABLE) == 0) {
		ret = set_soft_tx_disable(ponevt_ctx, true);
		if (ret != PON_STATUS_OK) {
			dbg_err_fn_ret(set_soft_tx_disable, ret);
			fapi_pon_close(ponevt_ctx);
			return EXIT_FAILURE;
		}
	}

	fapi_pon_register_fw_init_complete(ponevt_ctx, fw_init_complete);
	fapi_pon_reset(ponevt_ctx, cfg->mode);

	err = pthread_create(&ctx->tid, NULL, event_handling_thread, ctx);
	if (err) {
		dbg_err("%s: Can't start <%s> thread\n", __func__, "ponevt");
		fapi_pon_close(ponevt_ctx);
		return PON_ADAPTER_ERROR;
	}

	/* wait for the execution of the reset and initial configuration */
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 1000;
	err = sem_timedwait(&ctx->init_done, &ts);

	/* timeout */
	if (err)
		return PON_ADAPTER_ERROR;

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_pa_event_handling_stop(struct fapi_pon_wrapper_ctx *ctx)
{
	int err;
	enum pon_adapter_errno ret;

	ret = pon_fapi_thread_stop(&ctx->tid, "ponevt", 5);
	if (ret != PON_ADAPTER_SUCCESS)
		return ret;

	if (ctx->ponevt_ctx) {
		err = fapi_pon_close(ctx->ponevt_ctx);
		if (err != PON_STATUS_OK)
			return PON_ADAPTER_ERROR;
		ctx->ponevt_ctx = NULL;
	}

	sem_destroy(&ctx->init_done);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_pa_config_write(struct fapi_pon_wrapper_ctx *ctx,
		    const char *path,
		    const char *sec,
		    const char *opt,
		    const char *val,
		    bool commit)
{
	enum pon_adapter_errno error;

	if (!ctx->cfg_ops->set) {
		dbg_err("%s config set callback not registered\n",
			__func__);
		return PON_ADAPTER_ERR_NOT_SUPPORTED;
	}

	if (!path || !sec || !opt || !val)
		return PON_ADAPTER_ERR_INVALID_VAL;

	error = ctx->cfg_ops->set(ctx->hl_ctx, path, sec,
				  opt, val, false);
	if (error != PON_ADAPTER_SUCCESS) {
		dbg_err("can't set %s.%s.%s:%s, err %d\n",
			path, sec, opt, val, error);
		return error;
	}

	/* commit is optional and some implementations may not need it */
	if (commit && ctx->cfg_ops->commit) {
		error = ctx->cfg_ops->commit(ctx->hl_ctx, path);
		if (error != PON_ADAPTER_SUCCESS &&
		    error != PON_ADAPTER_ERR_NOT_SUPPORTED) {
			dbg_err("can't store the config, err %d\n",
				error);
			return error;
		}
	}

	return error;
}
