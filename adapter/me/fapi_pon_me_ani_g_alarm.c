/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifdef LINUX
	#include <unistd.h>
#endif /* LINUX */

#include "pon_adapter.h"
#include "omci/me/pon_adapter_ani_g.h"

#include "../fapi_pon_pa_common.h"

#include "fapi_pon.h"
#include "fapi_pon_error.h"

#include <pthread.h>
#include <string.h>

/**
 * Check values against threshold and report alarms
 *
 * \param ctx		Wrapper context
 * \param check_upper	Check for upper (true) or lower (false) limits
 * \param value		Current value
 * \param limit		Limit to check against
 * \param last_state	Pointer to last alarm state, to detect and update on
 *			changes
 * \param alarm_nr	Number of alarm to report
 * \param only_change	Report only on change (true) or always the current state
 *			(false)
 */
static void alarm_check_and_set(struct fapi_pon_wrapper_ctx *ctx,
				bool check_upper, int32_t value, int32_t limit,
				bool *last_state, int alarm_nr,
				bool only_change)
{
	bool alarm_state;

	if (check_upper)
		alarm_state = (value >= limit);
	else
		alarm_state = (value <= limit);

	if (!only_change || *last_state != alarm_state)
		ctx->event_handlers.optic_alarm(ctx->hl_ctx, alarm_nr,
						alarm_state);

	*last_state = alarm_state;
}

/* We are checking optical values every 10 seconds */
#define OPTIC_CHECK_INTERVAL 10
/* First check is delayed 1 ms after thread start */
#define OPTIC_CHECK_FIRST 1
/* Maximum number of EEPROM read attempts */
#define MAX_EEPROM_READ_ATTEMPTS 10

static void *ani_g_alarm_thread(void *arg)
{
	struct fapi_pon_wrapper_ctx *ctx = arg;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct fapi_pon_ani_g_data *ani_g_data = &ctx->ani_g_data;
	enum fapi_pon_errorcode ret;
	struct pon_optic_status optic_status;
	uint32_t bias_thr;
	int err;
	int read_err_count;

	err = pthread_setname_np(pthread_self(), "pon_ani_g_alarm");
	if (err)
		dbg_err("Can't set name <pon_ani_g_alarm> for thread\n");

	/*
	 * Both bias_thr and bias_threshold are given in units of 2uA.
	 * No units conversion is needed.
	 */
	bias_thr = cfg->bias_threshold;

	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	/* do first check OPTIC_CHECK_FIRST after thread start */
	usleep(OPTIC_CHECK_FIRST * 1000);
	read_err_count = 0;
	for (;;) {
		pthread_testcancel();

		ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
						ctx->cfg.optic.tx_power_scale);
		if (ret == PON_STATUS_INPUT_ERR) {
			/* "INPUT ERR" means the eeprom file is not open */
			dbg_err("Exit thread <pon_ani_g_alarm>, no eeprom assigned\n");
			break;
		}
		if (ret == PON_STATUS_EEPROM_READ_ERR) {
			if (read_err_count < MAX_EEPROM_READ_ATTEMPTS) {
				read_err_count++;
				continue;
			} else {
				dbg_err("Exit thread <pon_ani_g_alarm>, couldn't read from EEPROM file\n");
				break;
			}
		}
		read_err_count = 0;

		if (ret != PON_STATUS_OK)
			continue;

		/* Do not allow thread cancelling during the locked section. */
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		pthread_mutex_lock(&ani_g_data->lock);

		alarm_check_and_set(ctx, false, optic_status.rx_power,
				    ani_g_data->lower_optic_thr,
				    &ani_g_data->lower_optic_alarm,
				    PA_ALARM_ID_ANIG_LOW_RX_OPT_POWER,
				    !ani_g_data->update_status);

		alarm_check_and_set(ctx, true, optic_status.rx_power,
				    ani_g_data->upper_optic_thr,
				    &ani_g_data->upper_optic_alarm,
				    PA_ALARM_ID_ANIG_HIGH_RX_OPT_POWER,
				    !ani_g_data->update_status);

		alarm_check_and_set(ctx, false, optic_status.tx_power,
				    ani_g_data->lower_tx_power_thr,
				    &ani_g_data->lower_tx_power_alarm,
				    PA_ALARM_ID_ANIG_LOW_TX_OPT_POWER,
				    !ani_g_data->update_status);

		alarm_check_and_set(ctx, true, optic_status.tx_power,
				    ani_g_data->upper_tx_power_thr,
				    &ani_g_data->upper_tx_power_alarm,
				    PA_ALARM_ID_ANIG_HIGH_TX_OPT_POWER,
				    !ani_g_data->update_status);

		alarm_check_and_set(ctx, true, optic_status.bias,
				    bias_thr,
				    &ani_g_data->bias_current_alarm,
				    PA_ALARM_ID_ANIG_LASER_BIAS_CURRENT,
				    !ani_g_data->update_status);

		ctx->event_handlers.optic_alarm(ctx->hl_ctx,
						PA_ALARM_ID_ANIG_SF,
						ani_g_data->signal_fail);

		ctx->event_handlers.optic_alarm(ctx->hl_ctx,
						PA_ALARM_ID_ANIG_SD,
						ani_g_data->signal_degrade);

		ani_g_data->update_status = false;

		pthread_mutex_unlock(&ani_g_data->lock);

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		/*
		 * We want to check the fapi_pon_optic_status_get() values
		 * every OPTIC_CHECK_INTERVAL seconds, so we sleep the time.
		 * Functions: sleep and usleep are cancellation points.
		 */
		sleep(OPTIC_CHECK_INTERVAL);
	}
	return EXIT_SUCCESS;
}

enum pon_adapter_errno
pon_pa_ani_g_alarm_check_start(struct fapi_pon_wrapper_ctx *ctx)
{
	int err;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct fapi_pon_ani_g_data *ani_g_data = &ctx->ani_g_data;

	/* Set ANI-G data to default values in case update will not trigger */
	if (!ani_g_data->update_status) {
		ani_g_data->lower_optic_thr =
			cfg->lower_receive_optical_threshold * 500;
		ani_g_data->upper_optic_thr =
			cfg->upper_receive_optical_threshold * 500;
		ani_g_data->lower_tx_power_thr =
			cfg->lower_transmit_power_threshold * 500;
		ani_g_data->upper_tx_power_thr =
			cfg->upper_transmit_power_threshold * 500;
	}

	err = pthread_create(&ani_g_data->tid, NULL, ani_g_alarm_thread, ctx);
	if (err) {
		dbg_err("%s: Can't start <pon_ani_g_alarm> event handling thread\n",
			__func__);
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_pa_ani_g_alarm_check_stop(struct fapi_pon_wrapper_ctx *ctx)
{
	return pon_fapi_thread_stop(&ctx->ani_g_data.tid, "pon_ani_g_alarm", 5);
}
