/******************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 * Copyright (c) 2012 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/
#ifndef __dump_h
#define __dump_h

/** Initialize/shutdown table group
 *
 * \param[in] init true - initialize group, otherwise shutdown
 */
void table_group_init(int init);

/** Get version/status dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_status(struct top_context *ctx,
		const char *input_file_name);

/** Get version/status dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_status(struct top_context *ctx,
			      int entry,
			      char *text);

/** Get capability and configuration dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_cap(struct top_context *ctx,
		const char *input_file_name);

/** Get capability and configuration dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_cap(struct top_context *ctx,
			      int entry,
			      char *text);

/** Get LAN interface status & counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_lan(struct top_context *ctx,
		const char *input_file_name);

/** Get LAN interface status & counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_lan(struct top_context *ctx,
					int entry,
					char *text);

/** Get GEM/XGEM port status dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gem_port_status(struct top_context *ctx,
		const char *input_file_name);

/** Get GEM/XGEM port status dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gem_port_status(struct top_context *ctx,
					int entry,
					char *text);

/** Get GEM/XGEM port counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gem_port_counters(struct top_context *ctx,
		const char *input_file_name);

/** Get GEM/XGEM port counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gem_port_counters(struct top_context *ctx,
		int entry,
		char *text);

/** Get GEM/XGEM port DS counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gem_port_ds_counters(struct top_context *ctx,
		const char *input_file_name);

/** Get GEM/XGEM port DS counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gem_port_ds_counters(struct top_context *ctx,
					int entry,
					char *text);

/** Get GEM/XGEM port US counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gem_port_us_counters(struct top_context *ctx,
		const char *input_file_name);

/** Get GEM/XGEM port US counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gem_port_us_counters(struct top_context *ctx,
					int entry,
					char *text);

/** Get GEM/XGEM port ethernet DS counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gem_port_ethernet_ds_counters(struct top_context *ctx,
					    const char *input_file_name);

/** Get GEM/XGEM port ethernet DS counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gem_port_ethernet_ds_counters(struct top_context *ctx,
						    int entry,
						    char *text);

/** Get GEM/XGEM port ethernet US counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gem_port_ethernet_us_counters(struct top_context *ctx,
					    const char *input_file_name);

/** Get GEM/XGEM port ethernet US counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gem_port_ethernet_us_counters(struct top_context *ctx,
						    int entry,
						    char *text);

/** Get FEC status & counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_fec_status(struct top_context *ctx,
		const char *input_file_name);

/** Get FEC status & counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_fec_status(struct top_context *ctx,
					int entry,
					char *text);

/** Get GTC/XGTC status & counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_gtc_status(struct top_context *ctx,
		const char *input_file_name);

/** Get GTC/XGTC status & counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_gtc_status(struct top_context *ctx,
					int entry,
					char *text);
/** Get power save status dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_power_status(struct top_context *ctx,
		const char *input_file_name);

/** Get power save status dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_power_status(struct top_context *ctx,
					int entry,
					char *text);

/** Get allocation status dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_alloc_status(struct top_context *ctx,
		const char *input_file_name);

/** Get allocation status dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_alloc_status(struct top_context *ctx,
					int entry,
					char *text);

/** Get allocation counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_alloc_counters(struct top_context *ctx,
		const char *input_file_name);

/** Get allocation counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_alloc_counters(struct top_context *ctx,
					int entry,
					char *text);

/** Get PLOAM downstream counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_ploam_ds_counters(struct top_context *ctx,
		const char *input_file_name);

/** Get PLOAM downstream counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_ploam_ds_counters(struct top_context *ctx,
					int entry,
					char *text);

/** Get PLOAM upstream counters dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_ploam_us_counters(struct top_context *ctx,
		const char *input_file_name);

/** Get PLOAM upstream counters dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_ploam_us_counters(struct top_context *ctx,
					int entry,
					char *text);

/** Get optical interface status dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_optic_status(struct top_context *ctx,
		const char *input_file_name);

/** Get optical interface status dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
*/
char *table_entry_get_optic_status(struct top_context *ctx,
					int entry,
					char *text);
/** Get optical interface info dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_optic_info(struct top_context *ctx,
		const char *input_file_name);

/** Get optical interface info dump entry
 *
 * \param[in]  entry Table entry number; -1 for header
 * \param[out] text  Table entry text
 */
char *table_entry_get_optic_info(struct top_context *ctx,
				 int entry,
				 char *text);
/** Get active alarms dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_alarms(struct top_context *ctx,
		const char *input_file_name);

/** Get alarms dump entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_alarms(struct top_context *ctx,
					int entry,
					char *text);

/** Get PSM config dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_psm_cfg(struct top_context *ctx,
		      const char *input_file_name);

/** Get PSM config dump entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_psm_cfg(struct top_context *ctx,
			      const int entry,
			      char *text);

/** Get Debug Burst Profile dump
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_debug_burst_profile(struct top_context *ctx,
				  const char *input_file_name);

/** Get Debug Burst Profile entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_debug_burst_profile(struct top_context *ctx,
					  int entry,
					  char *text);

/** Get TWDM Tuning Control Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_tc_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM Tuning Control Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_tc_counters(struct top_context *ctx,
					  int entry,
					  char *text);

/** Get TWDM LODS Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_lods_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM LODS Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_lods_counters(struct top_context *ctx,
					  int entry,
					  char *text);
/** Get TWDM PLOAM DS Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_ploam_ds_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM PLOAM DS Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_ploam_ds_counters(struct top_context *ctx,
					  int entry,
					  char *text);
/** Get TWDM PLOAM US Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_ploam_us_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM PLOAM US Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_ploam_us_counters(struct top_context *ctx,
					  int entry,
					  char *text);
/** Get TWDM Power Level Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_pl_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM Power Level Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_pl_counters(struct top_context *ctx,
					  int entry,
					  char *text);
/** Get TWDM XGTC Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_xgtc_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM XGTC Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_xgtc_counters(struct top_context *ctx,
					  int entry,
					  char *text);
/** Get TWDM xGEM-Port Counters
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_xgem_port_counters(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM xGEM-Port Counters entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_xgem_port_counters(struct top_context *ctx,
					  int entry,
					  char *text);
/** Get TWDM Channel Profile Status
 *
 * \return Number of entries (lines) in the dump
 */
int table_get_twdm_channel_profile_status(struct top_context *ctx,
				  const char *input_file_name);

/** Get TWDM Channel Profile Status entry
 *
 *  \param[in]  entry Table entry number; -1 for header
 *  \param[out] text  Table entry text
 */
char *table_entry_get_twdm_channel_profile_status(struct top_context *ctx,
					  int entry,
					  char *text);
#endif
