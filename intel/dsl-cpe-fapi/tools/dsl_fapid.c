/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include "dsl_fapid.h"
#include <unistd.h>	/* sleep */

struct fapi_dsl_init_parameter dsl_param_cfg[] = {
	{"BitswapUs_V", "1"},
	{"BitswapUs_A", "1"},
	{"BitswapDs_V", "0"},
	{"BitswapDs_A", "0"},
	{"ReTxUs", "1"},
	{"ReTxDs_V", "0"},
	{"ReTxDs_A", "0"},
	{"SraUs_V", "1"},
	{"SraUs_A", "0"},
	{"SraDs_V", "1"},
	{"SraDs_A", "0"},
	{"VirtualNoiseUs", "1"},
	{"VirtualNoiseDs", "0"},
	{"Vectoring", "Auto"},
	{"XTSE", "0x5 0x0 0x4 0x0 0xC 0x1 0x0 0x7"},
	{"LinkEncapsulationConfig", "G.992.3_Annex_K_ATM,G.992.3_Annex_K_PTM,G.993.2_Annex_K_PTM"},
	{"DSLNextMode", "VDSL"},
	{"DSLActSeq", "StandardGHS"},
	{"DSLActMode", "GHS"},
	{"DSLRemember", "1"},
	{"EntitiesEnabled", "2"},
	{"", ""}
};

int main(int argc, char *argv[])
{
	struct fapi_dsl_ctx *fapi_ctx;
	struct fapi_dsl_ctx *fapi_ctx1;
	struct fapi_dsl_ctx *fapi_ctx2;
	struct fapi_dsl_ctx *fapi_ctx3;
	struct fapi_dsl_init_cfg fapi_cfg;
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	struct dsl_fapi_line_obj line_obj;
	struct dsl_fapi_line_stats_obj line_stats_obj;
	struct dsl_fapi_line_stats_interval_obj line_stats_interval_obj;
	struct dsl_fapi_line_test_params_obj line_test_params_obj;
	struct dsl_fapi_channel_obj channel_obj;
	struct dsl_fapi_channel_stats_obj channel_stats_obj;
	struct dsl_fapi_channel_stats_interval_obj channel_stats_interval_obj;
	struct dsl_fapi_bonding_obj bonding_obj;
	struct dsl_fapi_bonding_channel_obj bonding_channel_obj;
	struct dsl_fapi_bonding_channel_ethernet_obj bonding_channel_ethernet_obj;
	struct dsl_fapi_bonding_stats_obj bonding_stats_obj;
	struct dsl_fapi_bonding_stats_total_obj bonding_stats_total_obj;
	struct dsl_fapi_bonding_stats_day_obj bonding_stats_day_obj;
	struct dsl_fapi_bonding_stats_hour_obj bonding_stats_hour_obj;
	struct dsl_fapi_bonding_ethernet_obj bonding_ethernet_obj;
	struct dsl_fapi_bonding_ethernet_stats_obj bonding_ethernet_stats_obj;

	memset(&line_obj, 0x0, sizeof(struct dsl_fapi_line_obj));
	memset(&line_stats_obj, 0x0, sizeof(struct dsl_fapi_line_stats_obj));
	memset(&line_stats_interval_obj, 0x0, sizeof(struct dsl_fapi_line_stats_interval_obj));
	memset(&line_test_params_obj, 0x0, sizeof(struct dsl_fapi_line_test_params_obj));
	memset(&channel_obj, 0x0, sizeof(struct dsl_fapi_channel_obj));
	memset(&channel_stats_obj, 0x0, sizeof(struct dsl_fapi_channel_stats_obj));
	memset(&channel_stats_interval_obj, 0x0, sizeof(struct dsl_fapi_channel_stats_interval_obj));
	memset(&bonding_obj, 0x0, sizeof(struct dsl_fapi_bonding_obj));
	memset(&bonding_channel_obj, 0x0, sizeof(struct dsl_fapi_bonding_channel_obj));
	memset(&bonding_stats_obj, 0x0, sizeof(struct dsl_fapi_bonding_stats_obj));

	printf("DSL CPE FAPI app: version %s\n", &dsl_fapi_version[4]);

	printf("Number of devices: %d\n", fapi_dsl_get_device_count());

	fapi_ctx = fapi_dsl_open(0);
	printf("DSL CPE FAPI app: ctx %p\n", fapi_ctx);
	printf("Number of devices: %d\n", fapi_dsl_get_device_count());

	/* line 1 is not exist -> will return NULL */
	fapi_ctx1 = fapi_dsl_open(1);
	printf("DSL CPE FAPI app: ctx %p\n", fapi_ctx1);

	fapi_ctx2 = fapi_dsl_open(0);
	printf("DSL CPE FAPI app: ctx %p\n", fapi_ctx2);

	fapi_ctx3 = fapi_dsl_open(0);
	printf("DSL CPE FAPI app: ctx %p\n", fapi_ctx3);

	fapi_cfg.params.array_size = sizeof(dsl_param_cfg) / sizeof(struct fapi_dsl_init_parameter);
	fapi_cfg.params.array = dsl_param_cfg;

	fapi_status = fapi_dsl_init(fapi_ctx, &fapi_cfg);

	sleep(60);

	fapi_status = fapi_dsl_line_get(fapi_ctx, &line_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line: enable %d\n", line_obj.enable);
	printf("DSL CPE FAPI app: Device.DSL.Line: status %s\n", line_obj.status);
	printf("DSL CPE FAPI app: Device.DSL.Line: upsteram %d\n", line_obj.upstream);
	printf("DSL CPE FAPI app: Device.DSL.Line: firmware_version %s\n", line_obj.firmware_version);
	printf("DSL CPE FAPI app: Device.DSL.Line: link status %s\n", line_obj.link_status);
	printf("DSL CPE FAPI app: Device.DSL.Line: standards supported %s\n", line_obj.standards_supported);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtse 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",
	       line_obj.xtse[0], line_obj.xtse[1], line_obj.xtse[2], line_obj.xtse[3],
	       line_obj.xtse[4], line_obj.xtse[5], line_obj.xtse[6], line_obj.xtse[7]);
	printf("DSL CPE FAPI app: Device.DSL.Line: standard used %s\n", line_obj.standard_used);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtse used 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",
	       line_obj.xtse_used[0], line_obj.xtse_used[1], line_obj.xtse_used[2], line_obj.xtse_used[3],
	       line_obj.xtse_used[4], line_obj.xtse_used[5], line_obj.xtse_used[6], line_obj.xtse_used[7]);
	printf("DSL CPE FAPI app: Device.DSL.Line: line encoding %s\n", line_obj.line_encoding);
	printf("DSL CPE FAPI app: Device.DSL.Line: allowed profiles %s\n", line_obj.allowed_profiles);
	printf("DSL CPE FAPI app: Device.DSL.Line: current profile %s\n", line_obj.current_profile);
	printf("DSL CPE FAPI app: Device.DSL.Line: power management state %s\n",
	       line_obj.power_management_state);
	printf("DSL CPE FAPI app: Device.DSL.Line: success failure cause %d\n",
	       line_obj.success_failure_cause);
	printf("DSL CPE FAPI app: Device.DSL.Line: last state transmitted downstream %d\n",
	       line_obj.last_state_transmitted_downstream);
	printf("DSL CPE FAPI app: Device.DSL.Line: last state transmitted upstream %d\n",
	       line_obj.last_state_transmitted_upstream);
	printf("DSL CPE FAPI app: Device.DSL.Line: us0mask 0x%X\n", line_obj.us0_mask);
	printf("DSL CPE FAPI app: Device.DSL.Line: trellis ds %d\n", line_obj.trellis_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line: trellis us %d\n", line_obj.trellis_us);
	printf("DSL CPE FAPI app: Device.DSL.Line: act snr mode ds %d\n", line_obj.act_snr_mode_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line: act snr mode us %d\n", line_obj.act_snr_mode_us);
	printf("DSL CPE FAPI app: Device.DSL.Line: line number %d\n", line_obj.line_number);
	printf("DSL CPE FAPI app: Device.DSL.Line: upstream max bit rate %d\n",
	       line_obj.upstream_max_bit_rate);
	printf("DSL CPE FAPI app: Device.DSL.Line: downstream max bit rate %d\n",
	       line_obj.downstream_max_bit_rate);
	printf("DSL CPE FAPI app: Device.DSL.Line: upstream noise margin %d\n",
	       line_obj.upstream_noise_margin);
	printf("DSL CPE FAPI app: Device.DSL.Line: downstream noise margin %d\n",
	       line_obj.downstream_noise_margin);
	printf("DSL CPE FAPI app: Device.DSL.Line: snr mpb us %s\n", line_obj.snr_mpb_us);
	printf("DSL CPE FAPI app: Device.DSL.Line: snr mpb ds %s\n", line_obj.snr_mpb_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line: upstream power %d\n", line_obj.upstream_power);
	printf("DSL CPE FAPI app: Device.DSL.Line: downstream power %d\n", line_obj.downstream_power);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtur vendor %02X%02X%02X%02X\n",
	       line_obj.xtur_vendor[0], line_obj.xtur_vendor[1], line_obj.xtur_vendor[2],
	       line_obj.xtur_vendor[3]);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtur country %02X%02X\n", line_obj.xtur_country[0],
	       line_obj.xtur_country[1]);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtur ansi std %d\n", line_obj.xtur_ansi_std);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtur ansi rev %d\n", line_obj.xtur_ansi_rev);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtuc vendor %02X%02X%02X%02X\n",
	       line_obj.xtuc_vendor[0], line_obj.xtuc_vendor[1], line_obj.xtuc_vendor[2],
	       line_obj.xtuc_vendor[3]);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtuc country %02X%02X\n", line_obj.xtuc_country[0],
	       line_obj.xtuc_country[1]);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtuc ansi std %d\n", line_obj.xtuc_ansi_std);
	printf("DSL CPE FAPI app: Device.DSL.Line: xtuc ansi rev %d\n", line_obj.xtuc_ansi_rev);
	printf("DSL CPE FAPI app: Device.DSL.Line: upbokler pb %s\n", line_obj.upbokler_pb);
	printf("DSL CPE FAPI app: Device.DSL.Line: rxthrsh ds %s\n", line_obj.rxthrsh_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line: act ra mode ds %d\n", line_obj.act_ra_mode_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line: act ra mode us %d\n", line_obj.act_ra_mode_us);
	printf("DSL CPE FAPI app: Device.DSL.Line: snr mroc us %d\n", line_obj.snr_mroc_us);
	printf("DSL CPE FAPI app: Device.DSL.Line: upstream attenuation %d\n", line_obj.upstream_attenuation);
	printf("DSL CPE FAPI app: Device.DSL.Line: downstream attenuation %d\n",
	       line_obj.downstream_attenuation);

	printf("\n");

	fapi_status = fapi_dsl_line_stats_get(fapi_ctx, &line_stats_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats: total start %d\n", line_stats_obj.total_start);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats: showtime start %d\n", line_stats_obj.showtime_start);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats: last showtime start %d\n",
	       line_stats_obj.last_showtime_start);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats: current day start %d\n",
	       line_stats_obj.current_day_start);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats: quarter hour start %d\n",
	       line_stats_obj.quarter_hour_start);

	printf("\n");

	fapi_status = fapi_dsl_line_stats_total_get(fapi_ctx, &line_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.Total: errored secs %d\n",
	       line_stats_interval_obj.errored_secs);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.Total: severely errored secs %d\n",
	       line_stats_interval_obj.severely_errored_secs);

	printf("\n");

	fapi_status = fapi_dsl_line_stats_showtime_get(fapi_ctx, &line_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.Showtime: errored secs %d\n",
	       line_stats_interval_obj.errored_secs);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.Showtime: severely errored secs %d\n",
	       line_stats_interval_obj.severely_errored_secs);

	printf("\n");

	fapi_status = fapi_dsl_line_stats_last_showtime_get(fapi_ctx, &line_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.LastShowtime: errored secs %d\n",
	       line_stats_interval_obj.errored_secs);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.LastShowtime: severely errored secs %d\n",
	       line_stats_interval_obj.severely_errored_secs);

	printf("\n");

	fapi_status = fapi_dsl_line_stats_current_day_get(fapi_ctx, &line_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.CurrentDay: errored secs %d\n",
	       line_stats_interval_obj.errored_secs);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.CurrentDay: severely errored secs %d\n",
	       line_stats_interval_obj.severely_errored_secs);

	printf("\n");

	fapi_status = fapi_dsl_line_stats_quarter_hour_get(fapi_ctx, &line_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.QuarterHour: errored secs %d\n",
	       line_stats_interval_obj.errored_secs);
	printf("DSL CPE FAPI app: Device.DSL.Line.Stats.QuarterHour: severely errored secs %d\n",
	       line_stats_interval_obj.severely_errored_secs);

	printf("\n");

	fapi_status = fapi_dsl_line_test_params_get(fapi_ctx, &line_test_params_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: hlogg ds %d\n", line_test_params_obj.hlogg_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: hlogg us %d\n", line_test_params_obj.hlogg_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: hlogps ds %s\n",
	       line_test_params_obj.hlogps_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: hlogps us %s\n",
	       line_test_params_obj.hlogps_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: hlogmt ds %d\n",
	       line_test_params_obj.hlogmt_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: hlogmt us %d\n",
	       line_test_params_obj.hlogmt_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: qlng ds %d\n", line_test_params_obj.qlng_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: qlng us %d\n", line_test_params_obj.qlng_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: qlnps ds %s\n", line_test_params_obj.qlnps_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: qlnps us %s\n", line_test_params_obj.qlnps_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: qlnmt ds %d\n", line_test_params_obj.qlnmt_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: qlnmt us %d\n", line_test_params_obj.qlnmt_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: snrg ds %d\n", line_test_params_obj.snrg_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: snrg us %d\n", line_test_params_obj.snrg_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: snrps ds %s\n", line_test_params_obj.snrps_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: snrps us %s\n", line_test_params_obj.snrps_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: snrmt ds %d\n", line_test_params_obj.snrmt_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: snrmt us %d\n", line_test_params_obj.snrmt_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: latn ds %s\n", line_test_params_obj.latn_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: latn us %s\n", line_test_params_obj.latn_us);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: satn ds %s\n", line_test_params_obj.satn_ds);
	printf("DSL CPE FAPI app: Device.DSL.Line.TestParams: satn us %s\n", line_test_params_obj.satn_us);

	printf("\n");

	fapi_status = fapi_dsl_channel_get(fapi_ctx, &channel_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel: enable %d\n", channel_obj.enable);
	printf("DSL CPE FAPI app: Device.DSL.Channel: status %s\n", channel_obj.status);
	printf("DSL CPE FAPI app: Device.DSL.Channel: link encapsulation supported %s\n",
	       channel_obj.link_encapsulation_supported);
	printf("DSL CPE FAPI app: Device.DSL.Channel: link encapsulation used %s\n",
	       channel_obj.link_encapsulation_used);
	printf("DSL CPE FAPI app: Device.DSL.Channel: lpath %d\n", channel_obj.lpath);
	printf("DSL CPE FAPI app: Device.DSL.Channel: intlvdepth %d\n", channel_obj.intlvdepth);
	printf("DSL CPE FAPI app: Device.DSL.Channel: intlvblock %d\n", channel_obj.intlvblock);
	printf("DSL CPE FAPI app: Device.DSL.Channel: actual interleaving delay %d\n",
	       channel_obj.actual_interleaving_delay);
	printf("DSL CPE FAPI app: Device.DSL.Channel: actinp %d\n", channel_obj.actinp);
	printf("DSL CPE FAPI app: Device.DSL.Channel: inpreport %d\n", channel_obj.inpreport);
	printf("DSL CPE FAPI app: Device.DSL.Channel: nfec %d\n", channel_obj.nfec);
	printf("DSL CPE FAPI app: Device.DSL.Channel: rfec %d\n", channel_obj.rfec);
	printf("DSL CPE FAPI app: Device.DSL.Channel: lsymb %d\n", channel_obj.lsymb);
	printf("DSL CPE FAPI app: Device.DSL.Channel: upstream curr rate %d\n",
	       channel_obj.upstream_curr_rate);
	printf("DSL CPE FAPI app: Device.DSL.Channel: downstream curr rate %d\n",
	       channel_obj.downstream_curr_rate);
	printf("DSL CPE FAPI app: Device.DSL.Channel: actndr ds %d\n", channel_obj.actndr_ds);
	printf("DSL CPE FAPI app: Device.DSL.Channel: actinprein ds %d\n", channel_obj.actinprein_ds);
	printf("DSL CPE FAPI app: Device.DSL.Channel: actndr us %d\n", channel_obj.actndr_us);
	printf("DSL CPE FAPI app: Device.DSL.Channel: actinprein us %d\n", channel_obj.actinprein_us);

	printf("\n");

	fapi_status = fapi_dsl_channel_stats_get(fapi_ctx, &channel_stats_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats: total start %d\n", channel_stats_obj.total_start);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats: showtime start %d\n",
	       channel_stats_obj.showtime_start);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats: last showtime start %d\n",
	       channel_stats_obj.last_showtime_start);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats: current day start %d\n",
	       channel_stats_obj.current_day_start);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats: quarter hour start %d\n",
	       channel_stats_obj.quarter_hour_start);

	printf("\n");

	fapi_status = fapi_dsl_channel_stats_total_get(fapi_ctx, &channel_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Total: xtu rfec errors %d\n",
	       channel_stats_interval_obj.xtu_rfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Total: xtu cfec errors %d\n",
	       channel_stats_interval_obj.xtu_cfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Total: xtu rhec errors %d\n",
	       channel_stats_interval_obj.xtu_rhec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Total: xtu chec errors %d\n",
	       channel_stats_interval_obj.xtu_chec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Total: xtu rcrc errors %d\n",
	       channel_stats_interval_obj.xtu_rcrc_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Total: xtu ccrc errors %d\n",
	       channel_stats_interval_obj.xtu_ccrc_errors);

	printf("\n");

	fapi_status = fapi_dsl_channel_stats_showtime_get(fapi_ctx, &channel_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Showtime: xtu rfec errors %d\n",
	       channel_stats_interval_obj.xtu_rfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Showtime: xtu cfec errors %d\n",
	       channel_stats_interval_obj.xtu_cfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Showtime: xtu rhec errors %d\n",
	       channel_stats_interval_obj.xtu_rhec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Showtime: xtu chec errors %d\n",
	       channel_stats_interval_obj.xtu_chec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Showtime: xtu rcrc errors %d\n",
	       channel_stats_interval_obj.xtu_rcrc_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.Showtime: xtu ccrc errors %d\n",
	       channel_stats_interval_obj.xtu_ccrc_errors);

	printf("\n");

	fapi_status = fapi_dsl_channel_stats_last_showtime_get(fapi_ctx, &channel_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.LastShowtime: xtu rfec errors %d\n",
	       channel_stats_interval_obj.xtu_rfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.LastShowtime: xtu cfec errors %d\n",
	       channel_stats_interval_obj.xtu_cfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.LastShowtime: xtu rhec errors %d\n",
	       channel_stats_interval_obj.xtu_rhec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.LastShowtime: xtu chec errors %d\n",
	       channel_stats_interval_obj.xtu_chec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.LastShowtime: xtu rcrc errors %d\n",
	       channel_stats_interval_obj.xtu_rcrc_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.LastShowtime: xtu ccrc errors %d\n",
	       channel_stats_interval_obj.xtu_ccrc_errors);

	printf("\n");

	fapi_status = fapi_dsl_channel_stats_current_day_get(fapi_ctx, &channel_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.CurrentDay: xtu rfec errors %d\n",
	       channel_stats_interval_obj.xtu_rfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.CurrentDay: xtu cfec errors %d\n",
	       channel_stats_interval_obj.xtu_cfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.CurrentDay: xtu rhec errors %d\n",
	       channel_stats_interval_obj.xtu_rhec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.CurrentDay: xtu chec errors %d\n",
	       channel_stats_interval_obj.xtu_chec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.CurrentDay: xtu rcrc errors %d\n",
	       channel_stats_interval_obj.xtu_rcrc_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.CurrentDay: xtu ccrc errors %d\n",
	       channel_stats_interval_obj.xtu_ccrc_errors);

	printf("\n");

	fapi_status = fapi_dsl_channel_stats_quarter_hour_get(fapi_ctx, &channel_stats_interval_obj);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.QuarterHour: xtu rfec errors %d\n",
	       channel_stats_interval_obj.xtu_rfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.QuarterHour: xtu cfec errors %d\n",
	       channel_stats_interval_obj.xtu_cfec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.QuarterHour: xtu rhec errors %d\n",
	       channel_stats_interval_obj.xtu_rhec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.QuarterHour: xtu chec errors %d\n",
	       channel_stats_interval_obj.xtu_chec_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.QuarterHour: xtu rcrc errors %d\n",
	       channel_stats_interval_obj.xtu_rcrc_errors);
	printf("DSL CPE FAPI app: Device.DSL.Channel.Stats.QuarterHour: xtu ccrc errors %d\n",
	       channel_stats_interval_obj.xtu_ccrc_errors);

	fapi_status = fapi_dsl_bonding_get(fapi_ctx, &bonding_obj);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: enable %d\n", bonding_obj.enable);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: status %s\n", bonding_obj.status);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: last_change %d\n", bonding_obj.last_change);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: lower_layers %s\n", bonding_obj.lower_layers);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: bond_chemes_supported %s\n",
	       bonding_obj.bond_chemes_supported);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: bond_scheme %s\n", bonding_obj.bond_scheme);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: group_capacity %d\n", bonding_obj.group_capacity);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup: running_time %d\n", bonding_obj.running_time);

	fapi_status = fapi_dsl_bonding_channel_get(fapi_ctx, &bonding_channel_obj);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.BondedChannel: channel %s\n",
	       bonding_channel_obj.channel);

	fapi_status = fapi_dsl_bonding_ethernet_stats_get(fapi_ctx, &bonding_ethernet_stats_obj);

	fapi_status = fapi_dsl_bonding_stats_get(fapi_ctx, &bonding_stats_obj);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats: total_start %d\n",
	       bonding_stats_obj.total_start);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats: current_day_start %d\n",
	       bonding_stats_obj.current_day_start);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats: quarter_hour_start %d\n",
	       bonding_stats_obj.quarter_hour_start);

	fapi_status = fapi_dsl_bonding_stats_total_get(fapi_ctx, &bonding_stats_total_obj);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: upstream_rate %d\n",
	       bonding_stats_total_obj.upstream_rate);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: downstram_rate %d\n",
	       bonding_stats_total_obj.downstram_rate);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: upstream_differential_delay %d\n",
	       bonding_stats_total_obj.upstream_differential_delay);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: downstram_differential_delay %d\n",
	       bonding_stats_total_obj.downstram_differential_delay);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: upstream_differential_delay %d\n",
	       bonding_stats_total_obj.upstream_differential_delay);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: downstram_differential_delay %d\n",
	       bonding_stats_total_obj.downstram_differential_delay);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: failure_count %d\n",
	       bonding_stats_total_obj.failure_count);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: errored_seconds %d\n",
	       bonding_stats_total_obj.errored_seconds);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: severely_errored_seconds %d\n",
	       bonding_stats_total_obj.severely_errored_seconds);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.Total: unavailable_seconds %d\n",
	       bonding_stats_total_obj.unavailable_seconds);

	fapi_status = fapi_dsl_bonding_stats_day_get(fapi_ctx, &bonding_stats_day_obj);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: failure_reasons %s\n",
	       bonding_stats_day_obj.failure_reasons);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: upstream_rate %d\n",
	       bonding_stats_day_obj.upstream_rate);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: downstream_rate %d\n",
	       bonding_stats_day_obj.downstream_rate);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: upstream_differential_delay %d\n",
	       bonding_stats_day_obj.upstream_differential_delay);
	printf
	    ("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: downstream_differential_delay %d\n",
	     bonding_stats_day_obj.downstream_differential_delay);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: failure_count %d\n",
	       bonding_stats_day_obj.failure_count);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: errored_seconds %d\n",
	       bonding_stats_day_obj.errored_seconds);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: severely_errored_seconds %d\n",
	       bonding_stats_day_obj.severely_errored_seconds);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.CurrentDay: unavailable_seconds %d\n",
	       bonding_stats_day_obj.unavailable_seconds);

	fapi_status = fapi_dsl_bonding_stats_hour_get(fapi_ctx, &bonding_stats_hour_obj);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: failure_reasons %s\n",
	       bonding_stats_hour_obj.failure_reasons);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: upstream_rate %d\n",
	       bonding_stats_hour_obj.upstream_rate);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: downstream_rate %d\n",
	       bonding_stats_hour_obj.downstream_rate);
	printf
	    ("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: upstream_differential_delay %d\n",
	     bonding_stats_hour_obj.upstream_differential_delay);
	printf
	    ("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: downstream_differential_delay %d\n",
	     bonding_stats_hour_obj.downstream_differential_delay);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: failure_count %d\n",
	       bonding_stats_hour_obj.failure_count);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: errored_seconds %d\n",
	       bonding_stats_hour_obj.errored_seconds);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: severely_errored_seconds %d\n",
	       bonding_stats_hour_obj.severely_errored_seconds);
	printf("DSL CPE FAPI app: Device.DSL.BondingGroup.Stats.QuarterHour: unavailable_seconds %d\n",
	       bonding_stats_hour_obj.unavailable_seconds);

	fapi_status = fapi_dsl_line_get(fapi_ctx2, &line_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line: enable %d\n", line_obj.enable);
	printf("DSL CPE FAPI app: Device.DSL.Line: status %s\n", line_obj.status);
	printf("DSL CPE FAPI app: Device.DSL.Line: firmware_version %s\n", line_obj.firmware_version);

	fapi_status = fapi_dsl_line_get(fapi_ctx1, &line_obj);
	printf("DSL CPE FAPI app: Device.DSL.Line: enable %d\n", line_obj.enable);
	printf("DSL CPE FAPI app: Device.DSL.Line: status %s\n", line_obj.status);
	printf("DSL CPE FAPI app: Device.DSL.Line: firmware_version %s\n", line_obj.firmware_version);

	sleep(5);
	fapi_status = fapi_dsl_uninit(fapi_ctx);
	sleep(5);
	fapi_status = fapi_dsl_close(fapi_ctx);

	fapi_status = fapi_dsl_close(fapi_ctx2);
	fapi_status = fapi_dsl_close(fapi_ctx3);

	return 0;
}
