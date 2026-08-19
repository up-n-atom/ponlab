/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_cfgActions.c                                  *
*         Description  : MxL Schedule and Call Actions to PWHM State Machine   *
*                                                                              *
*  *****************************************************************************/

#include "swl/swl_common.h"
#include <swla/swla_mac.h>
#include "swla/swla_chanspec.h"

#include "wld/wld_radio.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_util.h"
#include "wld/wld_hostapd_ap_api.h"
#include "wld/wld_hostapd_cfgFile.h"
#include "wld/wld_wpaCtrlInterface.h"
#include "wld/Utils/wld_autoCommitMgr.h"
#include "wld/wld_wpaCtrl_api.h"

#include "whm_mxl_cfgActions.h"
#include "whm_mxl_hostapd_cfg.h"
#include "whm_mxl_utils.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_zwdfs.h"
#include "whm_mxl_reconfMngr.h"

#define ME "mxlAct"

static swl_rc_ne s_doHapdRestart(T_Radio* pRad, T_AccessPoint* pAP _UNUSED) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    pRad->pFA->mfn_wrad_secDmn_restart(pRad, SET);
    return SWL_RC_OK;
}

static swl_rc_ne s_doHapdToggle(T_Radio* pRad, T_AccessPoint* pAP _UNUSED) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    pRad->pFA->mfn_wrad_toggle(pRad, SET);
    return SWL_RC_OK;
}

static swl_rc_ne s_doHapdSighup(T_Radio* pRad, T_AccessPoint* pAP _UNUSED) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    pRad->pFA->mfn_wrad_secDmn_refresh(pRad, SET);
    return SWL_RC_OK;
}

static swl_rc_ne s_doReconf(T_Radio* pRad, T_AccessPoint* pAP _UNUSED) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERTI_NOT_NULL(pRadVendor, SWL_RC_INVALID_PARAM, ME, "pRadVendor is NULL");
    setBitLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW, RECONF_FSM_DO_RECONF);
    whm_mxl_reconfMngr_doCommit(pRad);
    return SWL_RC_OK;
}

static swl_rc_ne s_doUpdateBeacon(T_Radio* pRad _UNUSED, T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    swl_rc_ne rc = SWL_RC_OK;
    ASSERTI_TRUE(wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface), SWL_RC_INVALID_STATE, ME, "%s: wpaCtrl disconnected", pAP->alias);
    if(!(wld_ap_hostapd_updateBeacon(pAP, "updateBeacon"))) {
        SAH_TRACEZ_ERROR(ME, "%s: Failed to update becaon", pAP->alias);
        rc = SWL_RC_ERROR;
    }
    wld_ap_doSync(pAP);
    return rc;
}

static swl_rc_ne s_doHapdConfUpdate(T_Radio* pRad, T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    T_AccessPoint* tgtAP = pAP;
    /* Replace with master VAP AP when no particular AP context passed */
    if (pAP == NULL) {
        T_AccessPoint* masterVap = wld_rad_getFirstVap(pRad);
        ASSERT_NOT_NULL(masterVap, SWL_RC_INVALID_PARAM, ME, "masterVap is NULL");
        tgtAP = masterVap;
    }
    wld_ap_doSync(tgtAP);
    return SWL_RC_OK;
}

static swl_rc_ne s_doNoAction(T_Radio* pRad _UNUSED, T_AccessPoint* pAP _UNUSED) {
    SAH_TRACEZ_INFO(ME, "No Action");
    return SWL_RC_OK;
}

SWL_TABLE(sActionHandlers,
          ARR(whm_mxl_hapd_action_e action; void* actionCb; ),
          ARR(swl_type_uint32, swl_type_voidPtr, ),
          ARR(//hapd actions mapping to execute functions
              {HAPD_ACTION_ERROR,               NULL},
              {HAPD_ACTION_NONE,                s_doNoAction},
              {HAPD_ACTION_NEED_UPDATE_CONF,    s_doHapdConfUpdate},
              {HAPD_ACTION_NEED_UPDATE_BEACON,  s_doUpdateBeacon},
              {HAPD_ACTION_NEED_SIGHUP,         s_doHapdSighup},
              {HAPD_ACTION_NEED_TOGGLE,         s_doHapdToggle},
              {HAPD_ACTION_NEED_RECONF,         s_doReconf},
              {HAPD_ACTION_NEED_RESTART,        s_doHapdRestart},
              ));

SWL_TABLE(sVendorParamsOdlToConf,
          ARR(char* paramOdlName; char* paramHapdName; ),
          ARR(swl_type_charPtr, swl_type_charPtr, ),
          ARR(//Convert ODL name to HAPD conf name
              //Radio Parameters
              {"ApMaxNumSta",                   "ap_max_num_sta"},
              {"OverrideMBSSID",                "override_6g_mbssid_default_mode"},
              {"SetProbeReqCltMode",            "sProbeReqCltMode"},
              {"SetBfMode",                     "sBfMode"},
              {"SetPowerSelection",             "sPowerSelection"},
              {"Ignore40MhzIntolerant",         "ignore_40_mhz_intolerant"},
              {"ProbeReqListTimer",             "ProbeReqListTimer"},
              {"DfsChStateFile",                "dfs_channels_state_file_location"},
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
              {"DfsDebugChan",                  "dfs_debug_chan"},
              {"ZwdfsDebugChan",                "zwdfs_debug_chan"},
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
              {"SubBandDFS",                    "sub_band_dfs"},
              {"TwtResponderSupport",           "twt_responder_support"},
              {"HeMacTwtResponderSupport",      "he_mac_twt_responder_support"},
              {"DynamicEdca",                   "dynamic_edca"},
              {"HeDebugMode",                   "enable_he_debug_mode"},
              {"HeBeacon",                      "he_beacon"},
              {"DuplicateBeaconEnabled",        "duplicate_beacon_enabled"},
              {"DuplicateBeaconBw",             "duplicate_beacon_bw"},
              {"SetQAMplus",                    "sQAMplus"},
              {"SetRadarRssiTh",                "sRadarRssiTh"},
              {"ObssBeaconRssiThreshold",       "obss_beacon_rssi_threshold"},
              {"ObssInterval",                  "obss_interval"},
              {"ScanPassiveDwell",              "scan_passive_dwell"},
              {"ScanActiveDwell",               "scan_active_dwell"},
              {"ScanPassiveTotalPerChannel",    "scan_passive_total_per_channel"},
              {"ScanActiveTotalPerChannel",     "scan_active_total_per_channel"},
              {"ChannelTransitionDelayFactor",  "channel_transition_delay_factor"},
              {"ScanActivityThreshold",         "scan_activity_threshold"},
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
              {"AcsFallbackChan",               "acs_fallback_chan"},
              {"AcsScanMode",                   "acs_scan_mode"},
              {"AcsUpdateDoSwitch",             "acs_update_do_switch"},
              {"AcsFils",                       "acs_fils"},
              {"Acs6gOptChList",                "acs_6g_opt_ch_list"},
              {"AcsStrictChList",               "acs_strict_chanlist"},
              {"Acs6gPunctMode",                "acs_6g_punct_mode"},
              {"AcsBgScanInterval",             "acs_bgscan_interval"},
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
              {"BackgroundCac",                 "background_cac"},
              {"StartAfter",                    "start_after"},
              {"StartAfterDelay",               "start_after_delay"},
              {"StartAfterWatchdogTime",        "start_after_watchdog_time"},
              {"PunctureBitMap",                "punct_bitmap"},
              {"AfcdSock",                      "afcd_sock"},
              {"AfcOpClass",                    "afc_op_class"},
              {"AfcFrequencyRange",             "afc_freq_range"},
              {"AfcCertIds",                    "afc_cert_ids"},
              {"AfcSerialNumber",               "afc_serial_number"},
              {"AfcLinearPolygon",              "afc_linear_polygon"},
              {"AfcLocationType",               "afc_location_type"},
              {"AfcRequestId",                  "afc_request_id"},
              {"AfcRequestVersion",             "afc_request_version"},
              {"HePhyLdpcCodingInPayload",      "he_phy_ldpc_coding_in_payload"},
              {"HeMacMsduAckEnabledMpduSupport","he_mac_a_msdu_in_ack_enabled_a_mpdu_support"},
              {"HeMacMaxAMpduLengthExponent",   "he_mac_maximum_a_mpdu_length_exponent"},
              {"HeMacOmControlSupport",         "he_mac_om_control_support"},
              {"HtMinMpduStartSpacing",         "ht_minimum_mpdu_start_spacing"},
              {"MultibssEnable",                "multibss_enable"},
              {"HeMcsNssRxMapLessOrEqual80Mhz", "he_mcs_nss_rx_he_mcs_map_less_than_or_equal_80_mhz"},
              {"HeMcsNssTxMapLessOrEqual80Mhz", "he_mcs_nss_tx_he_mcs_map_less_than_or_equal_80_mhz"},
              {"HeMcsNssRxHeMcsMap160Mhz",      "he_mcs_nss_rx_he_mcs_map_160_mhz"},
              {"HeMcsNssTxHeMcsMap160Mhz",      "he_mcs_nss_tx_he_mcs_map_160_mhz"},
              {"VhtMcsSetPart0",                "vht_mcs_set_part0"},
              {"VhtMcsSetPart1",                "vht_mcs_set_part1"},
              {"HePhyMaxNc",                    "he_phy_max_nc"},
              {"SrCtrlHesigaSpatialReuseVal15", "sr_control_field_hesiga_spatial_reuse_value15_allowed"},
              {"HeOperationCohostedBss",        "he_operation_cohosted_bss"},
              {"HeMuEdcaIePresent",             "he_mu_edca_ie_present"},
              {"HePhyDcmMaxConstellationTx",    "he_phy_dcm_max_constellation_tx"},
              {"HePhyDcmMaxConstellationRx",    "he_phy_dcm_max_constellation_rx"},
              {"HePhyDcmMaxNssTx",              "he_phy_dcm_max_nss_tx"},
              {"HePhyDcmMaxNssRx",              "he_phy_dcm_max_nss_rx"},
              {"Ieee80211nAcAxCompat",          "ieee80211n_acax_compat"},
              {"EnableHeDebugMode",             "enable_he_debug_mode"},
              {"HeMuEdcaAcBeAifsn",             "he_mu_edca_ac_be_aifsn"},
              {"HeMuEdcaAcBeEcwmin",            "he_mu_edca_ac_be_ecwmin"},
              {"HeMuEdcaAcBeEcwmax",            "he_mu_edca_ac_be_ecwmax"},
              {"HeMuEdcaAcBeTimer",             "he_mu_edca_ac_be_timer"},
              {"HeMuEdcaAcBkAifsn",             "he_mu_edca_ac_bk_aifsn"},
              {"HeMuEdcaAcBkAci",               "he_mu_edca_ac_bk_aci"},
              {"HeMuEdcaAcBeEcwmin",            "he_mu_edca_ac_bk_ecwmin"},
              {"HeMuEdcaAcBeEcwmax",            "he_mu_edca_ac_bk_ecwmax"},
              {"HeMuEdcaAcBkTimer",             "he_mu_edca_ac_bk_timer"},
              {"HeMuEdcaAcViEcwmin",            "he_mu_edca_ac_vi_ecwmin"},
              {"HeMuEdcaAcViEcwmax",            "he_mu_edca_ac_vi_ecwmax"},
              {"HeMuEdcaAcViAifsn",             "he_mu_edca_ac_vi_aifsn"},
              {"HeMuEdcaAcViAci",               "he_mu_edca_ac_vi_aci"},
              {"HeMuEdcaAcViTimer",             "he_mu_edca_ac_vi_timer"},
              {"HeMuEdcaAcVoAifsn",             "he_mu_edca_ac_vo_aifsn"},
              {"HeMuEdcaAcVoAci",               "he_mu_edca_ac_vo_aci"},
              {"HeMuEdcaAcVoEcwmin",            "he_mu_edca_ac_vo_ecwmin"},
              {"HeMuEdcaAcVoEcwmax",            "he_mu_edca_ac_vo_ecwmax"},
              {"HeMuEdcaAcVoTimer",             "he_mu_edca_ac_vo_timer"},
              {"EnableEhtDebugMode",            "enable_eht_debug_mode"},
              {"EhtMacEhtOmControl",            "eht_mac_eht_om_control"},
              {"EhtMacRestrictedTwt",           "eht_mac_restricted_twt"},
              {"EhtMacTrigTxopSharingMode1",    "eht_mac_trig_txop_sharing_mode1"},
              {"EhtMacTrigTxopSharingMode2",    "eht_mac_trig_txop_sharing_mode2"},
              {"EhtPhyTrigMuBfPartialBwFb",     "eht_phy_trig_mu_bf_partial_bw_fb"},
              {"EhtPhyMaxNc",                   "eht_phy_max_nc"},
              {"EhtMcsMapLessOrEq80MHzRx09",    "eht_mcs_map_less_than_or_equal_80_mhz_rx_max_nss_eht_mcs_0_9"},
              {"EhtMcsMapLessOrEq80MHzTx09",    "eht_mcs_map_less_than_or_equal_80_mhz_tx_max_nss_eht_mcs_0_9"},
              {"EhtMcsMapLessOrEq80MHzRx1011",  "eht_mcs_map_less_than_or_equal_80_mhz_rx_max_nss_eht_mcs_10_11"},
              {"EhtMcsMapLessOrEq80MHzTx1011",  "eht_mcs_map_less_than_or_equal_80_mhz_tx_max_nss_eht_mcs_10_11"},
              {"EhtMcsMapLessOrEq80MHzRx1213",  "eht_mcs_map_less_than_or_equal_80_mhz_rx_max_nss_eht_mcs_12_13"},
              {"EhtMcsMapLessOrEq80MHzTx1213",  "eht_mcs_map_less_than_or_equal_80_mhz_tx_max_nss_eht_mcs_12_13"},
              {"EhtPhyMaxAmpduLenExpExt",       "eht_mac_max_ampdu_len_exp_ext"},
              {"EhtPhySuBeamformer",            "eht_phy_su_beamformer"},
              {"EhtPhySuBeamformee",            "eht_phy_su_beamformee"},
              {"EhtMacMaxMpduLen",              "eht_mac_max_mpdu_len"},
              {"EhtPhyPpeThresholdsPresent",    "eht_phy_ppe_thresholds_present"},
              {"EhtPhyCommonNominalPktPad",     "eht_phy_common_nominal_pkt_pad"},
              {"SetDynamicMuTypeDownLink",      "sDynamicMuTypeDownLink"},
              {"SetDynamicMuTypeUpLink",        "sDynamicMuTypeUpLink"},
              {"EhtMacScsTrafficDesc",          "eht_mac_scs_traffic_desc"},
              {"EhtMldTsfDiff",                 "eht_mld_tsf_diff"},
              {"EhtMcsMap160MHzRxMcs09",        "eht_mcs_map_160_mhz_rx_max_nss_eht_mcs_0_9"},
              {"EhtMcsMap160MHzTxMcs09",        "eht_mcs_map_160_mhz_tx_max_nss_eht_mcs_0_9"},
              {"EhtMcsMap160MHzTxMcs1011",      "eht_mcs_map_160_mhz_tx_max_nss_eht_mcs_10_11"},
              {"EhtMcsMap160MHzRxMcs1011",      "eht_mcs_map_160_mhz_rx_max_nss_eht_mcs_10_11"},
              {"EhtMcsMap160MHzTxMcs1213",      "eht_mcs_map_160_mhz_tx_max_nss_eht_mcs_12_13"},
              {"EhtMcsMap160MHzRxMcs1213",      "eht_mcs_map_160_mhz_rx_max_nss_eht_mcs_12_13"},
              {"EhtMcsMap320MHzRxMcs09",        "eht_mcs_map_320_mhz_rx_max_nss_eht_mcs_0_9"},
              {"EhtMcsMap320MHzTxMcs09",        "eht_mcs_map_320_mhz_tx_max_nss_eht_mcs_0_9"},
              {"EhtMcsMap320MHzRxMcs1011",      "eht_mcs_map_320_mhz_rx_max_nss_eht_mcs_10_11"},
              {"EhtMcsMap320MHzTxMcs1011",      "eht_mcs_map_320_mhz_tx_max_nss_eht_mcs_10_11"},
              {"EhtMcsMap320MHzRxMcs1213",      "eht_mcs_map_320_mhz_rx_max_nss_eht_mcs_12_13"},
              {"EhtMcsMap320MHzTxMcs1213",      "eht_mcs_map_320_mhz_tx_max_nss_eht_mcs_12_13"},
              {"EhtPhy320MHzIn6GHz",            "eht_phy_320_mhz_in_6_ghz"},
              {"AdvertiseEcsaIe",               "advertise_ecsa_ie"},
              {"SetMaxMpduLen",                 "sMaxMpduLen"},
              {"HePhySuBeamformeeCapable",      "he_phy_su_beamformee_capable"},
              {"HePhySuBeamformerCapable",      "he_phy_su_beamformer_capable"},
              {"HePhyBeamformeeStsLesOrEq80Mhz","he_phy_beamformee_sts_for_less_than_or_equal_80mhz"},
              {"HePhyBeamformeeStsGreater80Mhz","he_phy_beamformee_sts_for_greater_than_80mhz"},
              {"HePhyDeviceClass",              "he_phy_device_class"},
              {"HePhySuPpdu1xHeLtfAnd08UsGi",   "he_phy_su_ppdu_with_1x_he_ltf_and_08_us_gi"},
              {"HePhySuPpduHeMu4xHeLtf08UsGi",  "he_phy_su_ppdu_and_he_mu_with_4x_he_ltf_and_08us_gi"},
              {"HePhyMuBeamformerCapable",      "he_phy_mu_beamformer_capable"},
              {"HePhyNdpWith4xHeLtfAnd32UsGi",  "he_phy_ndp_with_4x_he_ltf_and_32_us_gi"},
              {"HePhyNg16SuFeedback",           "he_phy_ng_16_su_feedback"},
              {"HePhyNg16MuFeedback",           "he_phy_ng_16_mu_feedback"},
              {"HePhyNumSoundDimenLeOrEq80Mhz", "he_phy_number_of_sounding_dimensions_for_less_than_or_equal_80mhz"},
              {"HePhyNumSoundDimenGreater80Mhz","he_phy_number_of_sounding_dimensions_for_greater_than_80mhz"},
              {"HePhyTriggerSuBeamformFeedback","he_phy_triggered_su_beamforming_feedback"},
              {"HePhyDopplerRx",                "he_phy_doppler_rx"},
              {"HePhyDopplerTx",                "he_phy_doppler_tx"},
              {"HePhyFullBandwidthUlMuMimo",    "he_phy_full_bandwidth_ul_mu_mimo"},
              {"HePhyPartialBandwidthUlMuMimo", "he_phy_partial_bandwidth_ul_mu_mimo"},
              {"HePhyPartialBWExtendedRange",   "he_phy_partial_bandwidth_extended_range"},
              {"HePhyTriggeredCqiFeedback",     "he_phy_triggered_cqi_feedback"},
              {"HePhyPpeThresholdsPresent",     "he_phy_ppe_thresholds_present"},
              {"HePhyCodebookSize42SuSupport",  "he_phy_codebook_size42_for_su_support"},
              {"HePhyCodebookSize75MuSupport",  "he_phy_codebook_size75_for_mu_support"},
              {"HePhyPowBoostFactAlphaSupport", "he_phy_power_boost_factor_alpha_support"},
              {"HeMacOmCtrlMuDisableRxSupport", "he_mac_om_control_ul_mu_data_disable_rx_support"},
              {"HeOpTxopDurationRtsThreshold",  "he_operation_txop_duration_rts_threshold"},
              {"HeMacUl2x996ToneRuSupport",     "he_mac_ul_2x996tone_ru_support"},
              {"HeMacAckEnabledAggrSupport",    "he_mac_ack_enabled_aggregation_support"},
              {"HeMacBroadcastTwtSupport",      "he_mac_broadcast_twt_support"},
              {"HePhyDcmMaxBw",                 "he_phy_dcm_max_bw"},
              {"HePhyLong16HeSigOfdmSymSupport","he_phy_longer_than_16_he_sigb_ofdm_sym_support"},
              {"HeMacNdpFeedbackReportSupport", "he_mac_ndp_feedback_report_support"},
              {"HePhyRx1024QLt242ToneRuSupport","he_phy_rx_1024_qam_lt_242_tone_ru_support"},
              {"HePhyRxFullBwSuUsingMuCompSigb","he_phy_rx_full_bw_su_using_mu_comp_sigb"},
              {"HePhyRxFulBwUsingMuNonComSigb", "he_phy_rx_full_bw_su_using_mu_non_comp_sigb"},
              {"HePhyStbcTxLessThanOrEq80Mhz",  "he_phy_stbc_tx_less_than_or_equal_80mhz"},
              {"HePhyStbcTxGreaterThan80Mhz",   "he_phy_stbc_tx_greater_than_80mhz"},
              {"HeOperationErSuDisable",        "he_operation_er_su_disable"},
              {"HePhyErSuPpdu4xLtf8UsGi",       "he_phy_er_su_ppdu_4x_ltf_8us_gi"},
              {"HePhyPreamblePuncturingRx",     "he_phy_preamble_puncturing_rx"},
              {"HeMacMultiTidAggrTxSupport",    "he_mac_multi_tid_aggregation_tx_support"},
              {"HeMacMultiTidAggrRxSupport",    "he_mac_multi_tid_aggregation_rx_support"},
              {"EhtPhyNumSoundDim80MhzOrBelow", "eht_phy_num_sounding_dim_80_mhz_or_below"},
              {"EhtPhyNumSoundingDim160Mhz",    "eht_phy_num_sounding_dim_160_mhz"},
              {"EhtPhyNumSoundingDim320Mhz",    "eht_phy_num_sounding_dim_320_mhz"},
              {"EhtPhyMuBeamformerBw80MhzBelow","eht_phy_mu_beamformer_bw_80_mhz_or_below"},
              {"EhtPhyMuBeamformerBw160Mhz",    "eht_phy_mu_beamformer_bw_160_mhz"},
              {"EhtPhyMuBeamformerBw320Mhz",    "eht_phy_mu_beamformer_bw_320_mhz"},
              {"EhtPhyNdp4xEhtLtfAnd32UsGi",    "eht_phy_ndp_4x_eht_ltf_and_3_2_us_gi"},
              {"EhtPhyPartialBwUlMuMimo",       "eht_phy_partial_bw_ul_mu_mimo"},
              {"EhtPhyBeamformeeSs80MhzOrBelow","eht_phy_beamformee_ss_80_mhz_or_below"},
              {"EhtPhyBeamformeeSs160Mhz",      "eht_phy_beamformee_ss_160_mhz"},
              {"EhtPhyBeamformeeSs320Mhz",      "eht_phy_beamformee_ss_320_mhz"},
              {"EhtPhyEhtDupIn6Ghz",            "eht_phy_eht_dup_in_6_ghz"},
              {"EhtPhy20MhzOpStaRxNdpWiderBw",  "eht_phy_20_mhz_operating_sta_rx_ndp_with_wider_bw"},
              {"EhtPhyNg16SuFeedback",          "eht_phy_ng_16_su_feedback"},
              {"EhtPhyNg16MuFeedback",          "eht_phy_ng_16_mu_feedback"},
              {"EhtPhyCodebookSize42SuFb",      "eht_phy_codebook_size_4_2_su_fb"},
              {"EhtPhyCodebookSize755MuFb",     "eht_phy_codebook_size_75_5_mu_fb"},
              {"EhtPhyTrigSuBfFb",              "eht_phy_trig_su_bf_fb"},
              {"EhtPhyTrigCqiFb",               "eht_phy_trig_cqi_fb"},
              {"EhtPhyPartialBwDlMuMimo",       "eht_phy_partial_bw_dl_mu_mimo"},
              {"EhtPhyPsrBasedSr",              "eht_phy_psr_based_sr"},
              {"EhtPhyEhtMuPpdu4xEhtLtf08UsGi", "eht_phy_eht_mu_ppdu_with_4x_eht_ltf_and_0_8_us_gi"},
              {"EhtPhyRx1024Qam4096QamBel242Ru","eht_phy_rx_1024_qam_and_4096_qam_below_242_ru"},
              {"EhtPhyMaxNumOfSupportedEhtLtfs","eht_phy_max_num_of_supported_eht_ltfs"},
              {"EhtPhyMcs15",                   "eht_phy_mcs_15"},
              {"EhtPhyNonOfdmaMuMimo80MhzBelow","eht_phy_non_ofdma_ul_mu_mimo_bw_80_mhz_or_below"},
              {"EhtPhyNonOfdmaUlMuMimoBw160Mhz","eht_phy_non_ofdma_ul_mu_mimo_bw_160_mhz"},
              {"EhtPhyNonOfdmaUlMuMimoBw320Mhz","eht_phy_non_ofdma_ul_mu_mimo_bw_320_mhz"},
              {"SetDynamicMuMinStationsInGroup", "sDynamicMuMinStationsInGroup=4"},
              {"SetDynamicMuMaxStationsInGroup", "sDynamicMuMaxStationsInGroup"},
              {"SetDynamicMuCdbConfig",          "sDynamicMuCdbConfig"},
              {"Rnr6gOpClass137Allowed",         "rnr_6g_op_class_137_allowed"},
              {"RnrTbttMldNonZeroPad",           "rnr_tbtt_mld_non_zero_pad"},
              {"Country3",                       "country3"},
              //VAP Parameters
              {"EnableHairpin",                 "enable_hairpin"},
              {"ApMaxInactivity",               "ap_max_inactivity"},
              {"UnsolBcastProbeRespInterval",   "unsol_bcast_probe_resp_interval"},
              {"FilsDiscoveryMaxInterval",      "fils_discovery_max_interval"},
              {"BssTransition",                 "bss_transition"},
              {"ManagementFramesRate",          "management_frames_rate"},
              {"MgmtFramePowerControl",         "mgmt_frame_power_control"},
              {"ClientDisallow",                "multi_ap_client_disallow"},
              {"NumResSta",                     "num_res_sta"},
              {"VendorVht",                     "vendor_vht"},
              {"MLOEnable",                     "mlo_enable"},
              {"ApMldMac",                      "ap_mld_mac"},
              {"WdsSingleMlAssoc",              "wds_single_ml_assoc"},
              {"WdsPrimaryLink",                "wds_primary_link"},
              {"SoftBlockAclEnable",            "soft_block_acl_enable"},
              {"SoftBlockAclWaitTime",          "soft_block_acl_wait_time"},
              {"SoftBlockAclAllowTime",         "soft_block_acl_allow_time"},
              {"SoftBlockAclOnAuthReq",         "soft_block_acl_on_auth_req"},
              {"SoftBlockAclOnProbeReq",        "soft_block_acl_on_probe_req"},
              {"OWETransitionBSSID",            "owe_transition_bssid"},
              {"OWETransitionSSID",             "owe_transition_ssid"},
              {"RadiusSecretKey",               "auth_server_shared_secret"},
              {"DynamicMulticastMode",          "dynamic_multicast_mode"},
              {"DynamicMulticastRate",          "dynamic_multicast_rate"},
              {"DisableBeaconProtection",       "disable_beacon_prot"},
              {"DisablePbac",                   "disable_pbac"},
              {"SetBridgeMode",                 "sBridgeMode"},
              {"EnableBssLoad",                 "enable_bss_load_ie"},
              {"MboCellAware",                  "mbo_cell_aware"},
              {"SetAggrConfig",                 "sAggrConfig"},
              {"Set11nProtection",              "s11nProtection"},
              {"EmlCapabTransitionTimeout",     "eml_capab_transition_timeout"},
              {"ApProtectedKeepAliveRequired",  "ap_protected_keep_alive_required"},
              {"MldMediumsyncPresent",          "mld_mediumsync_present"},
              {"MldMediumsyncDuration",         "mld_mediumsync_duration"},
              {"MldMediumsyncOfdmedthresh",     "mld_mediumsync_ofdmedthresh"},
              {"MldMediumsyncMaxtxop",          "mld_mediumsync_maxtxop"},
              {"EhtMacEpcsPrioAccess",          "eht_mac_epcs_prio_access"},
              {"MloT2lmSupport",                "mlo_t2lm_support"},
              {"GroupMgmtCipher",               "group_mgmt_cipher"},
              {"RrmNeighRpt",                   "rrm_neighbor_report"},
              {"WnmBssTransQueryAutoresp",      "wnm_bss_trans_query_auto_resp"},
              {"WmmAcVICWMin",                  "wmm_ac_vi_cwmin"},
              {"WmmAcVOCWMin",                  "wmm_ac_vo_cwmin"},
              {"WmmAcBECWMin",                  "wmm_ac_be_cwmin"},
              {"WmmAcBKCWMin",                  "wmm_ac_bk_cwmin"},
              {"WmmAcVICWMax",                  "wmm_ac_vi_cwmax"},
              {"WmmAcVOCWMax",                  "wmm_ac_vo_cwmax"},
              {"WmmAcBECWMax",                  "wmm_ac_be_cwmax"},
              {"WmmAcBKCWMax",                  "wmm_ac_bk_cwmax"},
              {"WmmAcVIAifs",                   "wmm_ac_vi_aifs"},
              {"WmmAcVOAifs",                   "wmm_ac_vo_aifs"},
              {"WmmAcBEAifs",                   "wmm_ac_be_aifs"},
              {"WmmAcBKAifs",                   "wmm_ac_bk_aifs"},
              {"WmmAcVITXOP",                   "wmm_ac_vi_txop_limit"},
              {"WmmAcVOTXOP",                   "wmm_ac_vo_txop_limit"},
              {"WmmAcBETXOP",                   "wmm_ac_be_txop_limit"},
              {"WmmAcBKTXOP",                   "wmm_ac_bk_txop_limit"},
              {"WmmAcVIAcm",                    "wmm_ac_vi_acm"},
              {"WmmAcVOAcm",                    "wmm_ac_vo_acm"},
              {"WmmAcBEAcm",                    "wmm_ac_be_acm"},
              {"WmmAcBKAcm",                    "wmm_ac_bk_acm"},
              {"TxQueueVICWMin",                "tx_queue_data1_cwmin"},
              {"TxQueueVOCWMin",                "tx_queue_data0_cwmin"},
              {"TxQueueBECWMin",                "tx_queue_data2_cwmin"},
              {"TxQueueBKCWMin",                "tx_queue_data3_cwmin"},
              {"TxQueueVICWMax",                "tx_queue_data1_cwmax"},
              {"TxQueueVOCWMax",                "tx_queue_data0_cwmax"},
              {"TxQueueBECWMax",                "tx_queue_data2_cwmax"},
              {"TxQueueBKCWMax",                "tx_queue_data3_cwmax"},
              {"TxQueueVIAifs",                 "tx_queue_data1_aifs"},
              {"TxQueueVOAifs",                 "tx_queue_data0_aifs"},
              {"TxQueueBEAifs",                 "tx_queue_data2_aifs"},
              {"TxQueueBKAifs",                 "tx_queue_data3_aifs"},
              {"GroupCipher",                   "group_cipher"},
              {"GasCBDelay",                    "gas_comeback_delay"},
              {"SCSEnable",                     "scs_enable"},
              {"MSCSEnable",                    "mscs_enable"},
              {"Ignore11vDiassoc",              "ignore_da_timer"},
              {"QoSMap",                        "qos_map_set"},
              ));

SWL_TABLE(sRadCfgParamsActionMap,
          ARR(char* param; whm_mxl_hapd_action_e action; ),
          ARR(swl_type_charPtr, swl_type_uint32, ),
          ARR(//params/object set and applied with hostapd actions
              //Actions applied with hostapd restart
              {"Ignore40MhzIntolerant",         HAPD_ACTION_NEED_RESTART},
              {"HtCapabilities",                HAPD_ACTION_NEED_RESTART},
              {"VhtCapabilities",               HAPD_ACTION_NEED_RESTART},
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
              {"AcsFallbackChan",               HAPD_ACTION_NEED_RESTART},
              {"AcsScanMode",                   HAPD_ACTION_NEED_RESTART},
              {"AcsUpdateDoSwitch",             HAPD_ACTION_NEED_RESTART},
              {"AcsFils",                       HAPD_ACTION_NEED_RESTART},
              {"Acs6gOptChList",                HAPD_ACTION_NEED_RESTART},
              {"AcsStrictChList",               HAPD_ACTION_NEED_RESTART},
              {"Acs6gPunctMode",                HAPD_ACTION_NEED_RESTART},
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
              {"AfcdSock",                      HAPD_ACTION_NEED_RESTART},
              {"AfcOpClass",                    HAPD_ACTION_NEED_RESTART},
              {"AfcFrequencyRange",             HAPD_ACTION_NEED_RESTART},
              {"AfcCertIds",                    HAPD_ACTION_NEED_RESTART},
              {"AfcSerialNumber",               HAPD_ACTION_NEED_RESTART},
              {"AfcLinearPolygon",              HAPD_ACTION_NEED_RESTART},
              {"AfcLocationType",               HAPD_ACTION_NEED_RESTART},
              {"AfcRequestId",                  HAPD_ACTION_NEED_RESTART},
              {"AfcRequestVersion",             HAPD_ACTION_NEED_RESTART},
              //Actions applied with hostapd toggle
              {"OverrideMBSSID",                HAPD_ACTION_NEED_TOGGLE},
              {"ApMaxNumSta",                   HAPD_ACTION_NEED_TOGGLE},
              {"SetProbeReqCltMode",            HAPD_ACTION_NEED_TOGGLE},
              {"SetBfMode",                     HAPD_ACTION_NEED_TOGGLE},
              {"SetPowerSelection",             HAPD_ACTION_NEED_TOGGLE},
              {"ObssInterval",                  HAPD_ACTION_NEED_TOGGLE},
              {"ObssBeaconRssiThreshold",       HAPD_ACTION_NEED_TOGGLE},
              {"ProbeReqListTimer",             HAPD_ACTION_NEED_TOGGLE},
              {"DfsChStateFile",                HAPD_ACTION_NEED_TOGGLE},
#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
              {"DfsDebugChan",                  HAPD_ACTION_NEED_TOGGLE},
              {"ZwdfsDebugChan",                HAPD_ACTION_NEED_TOGGLE},
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */
              {"SubBandDFS",                    HAPD_ACTION_NEED_TOGGLE},
              {"TwtResponderSupport",           HAPD_ACTION_NEED_TOGGLE},
              {"HeMacTwtResponderSupport",      HAPD_ACTION_NEED_TOGGLE},
              {"DynamicEdca",                   HAPD_ACTION_NEED_TOGGLE},
              {"HeDebugMode",                   HAPD_ACTION_NEED_TOGGLE},
              {"HeBeacon",                      HAPD_ACTION_NEED_TOGGLE},
              {"DuplicateBeaconEnabled",        HAPD_ACTION_NEED_TOGGLE},
              {"DuplicateBeaconBw",             HAPD_ACTION_NEED_TOGGLE},
              {"SetQAMplus",                    HAPD_ACTION_NEED_TOGGLE},
              {"SetRadarRssiTh",                HAPD_ACTION_NEED_TOGGLE},
              {"ScanPassiveDwell",              HAPD_ACTION_NEED_TOGGLE},
              {"ScanActiveDwell",               HAPD_ACTION_NEED_TOGGLE},
              {"ScanPassiveTotalPerChannel",    HAPD_ACTION_NEED_TOGGLE},
              {"ScanActiveTotalPerChannel",     HAPD_ACTION_NEED_TOGGLE},
              {"ChannelTransitionDelayFactor",  HAPD_ACTION_NEED_TOGGLE},
              {"ScanActivityThreshold",         HAPD_ACTION_NEED_TOGGLE},
              {"FirstNonDfs",                   HAPD_ACTION_NEED_TOGGLE},
              {"BackgroundCac",                 HAPD_ACTION_NEED_TOGGLE},
              {"StartAfter",                    HAPD_ACTION_NEED_TOGGLE},
              {"StartAfterDelay",               HAPD_ACTION_NEED_TOGGLE},
              {"StartAfterWatchdogTime",        HAPD_ACTION_NEED_TOGGLE},
              {"PunctureBitMap",                HAPD_ACTION_NEED_TOGGLE},
              {"TestBedMode",                   HAPD_ACTION_NEED_TOGGLE},
              {"HePhyLdpcCodingInPayload",      HAPD_ACTION_NEED_TOGGLE},
              {"HeMacMsduAckEnabledMpduSupport",HAPD_ACTION_NEED_TOGGLE},
              {"HeMacMaxAMpduLengthExponent",   HAPD_ACTION_NEED_TOGGLE},
              {"HeMacOmControlSupport",         HAPD_ACTION_NEED_TOGGLE},
              {"HtMinMpduStartSpacing",         HAPD_ACTION_NEED_TOGGLE},
              {"MultibssEnable",                HAPD_ACTION_NEED_TOGGLE},
              {"HeMcsNssRxMapLessOrEqual80Mhz", HAPD_ACTION_NEED_TOGGLE},
              {"HeMcsNssTxMapLessOrEqual80Mhz", HAPD_ACTION_NEED_TOGGLE},
              {"HeMcsNssRxHeMcsMap160Mhz",      HAPD_ACTION_NEED_TOGGLE},
              {"HeMcsNssTxHeMcsMap160Mhz",      HAPD_ACTION_NEED_TOGGLE},
              {"VhtMcsSetPart0",                HAPD_ACTION_NEED_TOGGLE},
              {"VhtMcsSetPart1",                HAPD_ACTION_NEED_TOGGLE},
              {"HePhyMaxNc",                    HAPD_ACTION_NEED_TOGGLE},
              {"SrCtrlHesigaSpatialReuseVal15", HAPD_ACTION_NEED_TOGGLE},
              {"HeOperationCohostedBss",        HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaIePresent",             HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDcmMaxConstellationTx",    HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDcmMaxConstellationRx",    HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDcmMaxNssTx",              HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDcmMaxNssRx",              HAPD_ACTION_NEED_TOGGLE},
              {"Ieee80211nAcAxCompat",          HAPD_ACTION_NEED_TOGGLE},
              {"EnableHeDebugMode",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBeAifsn",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBeEcwmin",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBeEcwmax",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBeTimer",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBkAifsn",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBkAci",               HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBkEcwmin",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBkEcwmax",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcBkTimer",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcViEcwmin",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcViEcwmax",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcViAifsn",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcViAci",               HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcViTimer",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcVoAifsn",             HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcVoAci",               HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcVoEcwmin",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcVoEcwmax",            HAPD_ACTION_NEED_TOGGLE},
              {"HeMuEdcaAcVoTimer",             HAPD_ACTION_NEED_TOGGLE},
              {"EnableEhtDebugMode",            HAPD_ACTION_NEED_TOGGLE},
              {"EhtMacEhtOmControl",            HAPD_ACTION_NEED_TOGGLE},
              {"EhtMacRestrictedTwt",           HAPD_ACTION_NEED_TOGGLE},
              {"EhtMacTrigTxopSharingMode1",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtMacTrigTxopSharingMode2",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyTrigMuBfPartialBwFb",     HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMaxNc",                   HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMapLessOrEq80MHzRx09",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMapLessOrEq80MHzTx09",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMapLessOrEq80MHzRx1011",  HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMapLessOrEq80MHzTx1011",  HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMapLessOrEq80MHzRx1213",  HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMapLessOrEq80MHzTx1213",  HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMaxAmpduLenExpExt",       HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhySuBeamformer",            HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhySuBeamformee",            HAPD_ACTION_NEED_TOGGLE},
              {"EhtMacMaxMpduLen",              HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyPpeThresholdsPresent",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyCommonNominalPktPad",     HAPD_ACTION_NEED_TOGGLE},
              {"SetDynamicMuTypeDownLink",      HAPD_ACTION_NEED_TOGGLE},
              {"SetDynamicMuTypeUpLink",        HAPD_ACTION_NEED_TOGGLE},
              {"EhtMacScsTrafficDesc",          HAPD_ACTION_NEED_TOGGLE},
              {"EhtMldTsfDiff",                 HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap160MHzRxMcs09",        HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap160MHzTxMcs09",        HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap160MHzTxMcs1011",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap160MHzRxMcs1011",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap160MHzTxMcs1213",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap160MHzTxMcs1213",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap320MHzRxMcs09",        HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap320MHzTxMcs09",        HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap320MHzRxMcs1011",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap320MHzTxMcs1011",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap320MHzRxMcs1213",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtMcsMap320MHzTxMcs1213",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhy320MHzIn6GHz",            HAPD_ACTION_NEED_TOGGLE},
              {"AdvertiseEcsaIe",               HAPD_ACTION_NEED_TOGGLE},
              {"SetMaxMpduLen",                 HAPD_ACTION_NEED_TOGGLE},
              {"HePhySuBeamformeeCapable",      HAPD_ACTION_NEED_TOGGLE},
              {"HePhySuBeamformerCapable",      HAPD_ACTION_NEED_TOGGLE},
              {"HePhyBeamformeeStsLesOrEq80Mhz",HAPD_ACTION_NEED_TOGGLE},
              {"HePhyBeamformeeStsGreater80Mhz",HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDeviceClass",              HAPD_ACTION_NEED_TOGGLE},
              {"HePhySuPpdu1xHeLtfAnd08UsGi",   HAPD_ACTION_NEED_TOGGLE},
              {"HePhySuPpduHeMu4xHeLtf08UsGi",  HAPD_ACTION_NEED_TOGGLE},
              {"HePhyMuBeamformerCapable",      HAPD_ACTION_NEED_TOGGLE},
              {"HePhyNdpWith4xHeLtfAnd32UsGi",  HAPD_ACTION_NEED_TOGGLE},
              {"HePhyNg16SuFeedback",           HAPD_ACTION_NEED_TOGGLE},
              {"HePhyNg16MuFeedback",           HAPD_ACTION_NEED_TOGGLE},
              {"HePhyNumSoundDimenLeOrEq80Mhz", HAPD_ACTION_NEED_TOGGLE},
              {"HePhyNumSoundDimenGreater80Mhz",HAPD_ACTION_NEED_TOGGLE},
              {"HePhyTriggerSuBeamformFeedback",HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDopplerRx",                HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDopplerTx",                HAPD_ACTION_NEED_TOGGLE},
              {"HePhyFullBandwidthUlMuMimo",    HAPD_ACTION_NEED_TOGGLE},
              {"HePhyPartialBandwidthUlMuMimo", HAPD_ACTION_NEED_TOGGLE},
              {"HePhyPartialBWExtendedRange",   HAPD_ACTION_NEED_TOGGLE},
              {"HePhyTriggeredCqiFeedback",     HAPD_ACTION_NEED_TOGGLE},
              {"HePhyPpeThresholdsPresent",     HAPD_ACTION_NEED_TOGGLE},
              {"HePhyCodebookSize42SuSupport",  HAPD_ACTION_NEED_TOGGLE},
              {"HePhyCodebookSize75MuSupport",  HAPD_ACTION_NEED_TOGGLE},
              {"HePhyPowBoostFactAlphaSupport", HAPD_ACTION_NEED_TOGGLE},
              {"HeMacOmCtrlMuDisableRxSupport", HAPD_ACTION_NEED_TOGGLE},
              {"HeOpTxopDurationRtsThreshold",  HAPD_ACTION_NEED_TOGGLE},
              {"HeMacUl2x996ToneRuSupport",     HAPD_ACTION_NEED_TOGGLE},
              {"HeMacAckEnabledAggrSupport",    HAPD_ACTION_NEED_TOGGLE},
              {"HeMacBroadcastTwtSupport",      HAPD_ACTION_NEED_TOGGLE},
              {"HePhyDcmMaxBw",                 HAPD_ACTION_NEED_TOGGLE},
              {"HePhyLong16HeSigOfdmSymSupport",HAPD_ACTION_NEED_TOGGLE},
              {"HeMacNdpFeedbackReportSupport", HAPD_ACTION_NEED_TOGGLE},
              {"HePhyRx1024QLt242ToneRuSupport",HAPD_ACTION_NEED_TOGGLE},
              {"HePhyRxFullBwSuUsingMuCompSigb",HAPD_ACTION_NEED_TOGGLE},
              {"HePhyRxFulBwUsingMuNonComSigb", HAPD_ACTION_NEED_TOGGLE},
              {"HePhyStbcTxLessThanOrEq80Mhz",  HAPD_ACTION_NEED_TOGGLE},
              {"HePhyStbcTxGreaterThan80Mhz",   HAPD_ACTION_NEED_TOGGLE},
              {"HeOperationErSuDisable",        HAPD_ACTION_NEED_TOGGLE},
              {"HePhyErSuPpdu4xLtf8UsGi",       HAPD_ACTION_NEED_TOGGLE},
              {"HePhyPreamblePuncturingRx",     HAPD_ACTION_NEED_TOGGLE},
              {"HeMacMultiTidAggrTxSupport",    HAPD_ACTION_NEED_TOGGLE},
              {"HeMacMultiTidAggrRxSupport",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNumSoundDim80MhzOrBelow", HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNumSoundingDim160Mhz",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNumSoundingDim320Mhz",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMuBeamformerBw80MhzBelow",HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMuBeamformerBw160Mhz",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMuBeamformerBw320Mhz",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNdp4xEhtLtfAnd32UsGi",    HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyPartialBwUlMuMimo",       HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyBeamformeeSs80MhzOrBelow",HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyBeamformeeSs160Mhz",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyBeamformeeSs320Mhz",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyEhtDupIn6Ghz",            HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhy20MhzOpStaRxNdpWiderBw",  HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNg16SuFeedback",          HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNg16MuFeedback",          HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyCodebookSize42SuFb",      HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyCodebookSize755MuFb",     HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyTrigSuBfFb",              HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyTrigCqiFb",               HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyPartialBwDlMuMimo",       HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyPsrBasedSr",              HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyEhtMuPpdu4xEhtLtf08UsGi", HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyRx1024Qam4096QamBel242Ru",HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMaxNumOfSupportedEhtLtfs",HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyMcs15",                   HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNonOfdmaMuMimo80MhzBelow",HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNonOfdmaUlMuMimoBw160Mhz",HAPD_ACTION_NEED_TOGGLE},
              {"EhtPhyNonOfdmaUlMuMimoBw320Mhz",HAPD_ACTION_NEED_TOGGLE},
              {"SetDynamicMuMinStationsInGroup", HAPD_ACTION_NEED_TOGGLE},
              {"SetDynamicMuMaxStationsInGroup", HAPD_ACTION_NEED_TOGGLE},
              {"SetDynamicMuCdbConfig",          HAPD_ACTION_NEED_TOGGLE},
              {"Rnr6gOpClass137Allowed",         HAPD_ACTION_NEED_TOGGLE},
              {"RnrTbttMldNonZeroPad",           HAPD_ACTION_NEED_TOGGLE},
              {"Country3",                       HAPD_ACTION_NEED_TOGGLE},
              ));

static swl_rc_ne whm_mxl_hostapd_getRadParamAction(whm_mxl_hapd_action_e* pOutMappedAction, const char* paramName) {
    whm_mxl_hapd_action_e* pMappedAction = (whm_mxl_hapd_action_e*) swl_table_getMatchingValue(&sRadCfgParamsActionMap, 1, 0, paramName);
    ASSERTS_NOT_NULL(pMappedAction, SWL_RC_ERROR, ME, "NULL");
    W_SWL_SETPTR(pOutMappedAction, *pMappedAction);
    return SWL_RC_OK;
}

static whm_mxl_actionHandler_f whm_mxl_getActionHdlr(uint32_t action) {
    whm_mxl_actionHandler_f* pfActionHdlr = (whm_mxl_actionHandler_f*) swl_table_getMatchingValue(&sActionHandlers, 1, 0, &action);
    ASSERTS_NOT_NULL(pfActionHdlr, NULL, ME, "no internal hdlr defined for action(%d)", action);
    return *pfActionHdlr;
}

/**
 * @brief Determine which actions to take when specific RADIO parameter is changed
 *
 * @param pRad radio
 * @param paramName parameter name in data model
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_determineRadParamAction(T_Radio* pRad, const char* paramName, const char* paramValue) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    ASSERT_NOT_NULL(paramName, SWL_RC_INVALID_PARAM, ME, "paramName is NULL");
    whm_mxl_hapd_action_e action = HAPD_ACTION_NONE;
    swl_rc_ne rc;
    whm_mxl_actionHandler_f pfRadActionHdlr;
    bool ret = 0;

    /* First try setting parameter dynamically via ctrl interface before applying any action */
    T_AccessPoint* masterVap = wld_rad_getFirstVap(pRad);
    if (masterVap != NULL) {
        if(wld_wpaCtrlInterface_isReady(masterVap->wpaCtrlInterface) && (paramValue != NULL)) {
            const char* pConfName= (char*) swl_table_getMatchingValue(&sVendorParamsOdlToConf, 1, 0, paramName);
            ret = wld_ap_hostapd_setParamValue(masterVap, pConfName, paramValue, paramName);
            /* TO DO: maybe fallback to specific action if SET failed? */
        }
    }

    whm_mxl_hostapd_getRadParamAction(&action, paramName);
    ASSERT_NOT_EQUALS(action, HAPD_ACTION_ERROR, SWL_RC_INVALID_PARAM, ME, "Action HAPD_ACTION_ERROR");
    SAH_TRACEZ_INFO(ME, "%s: paramName=%s action=%d", pRad->Name, paramName, action);
    if ((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN)) {
        action = HAPD_ACTION_NONE;
        SAH_TRACEZ_INFO(ME, "%s: Invalid radio state(%d), forcing action HAPD_ACTION_NONE", pRad->Name, pRad->status);
    }
    pfRadActionHdlr = whm_mxl_getActionHdlr(action);
    ASSERTS_NOT_NULL(pfRadActionHdlr, SWL_RC_NOT_IMPLEMENTED, ME, "No handler for action(%d)", action);
    rc = pfRadActionHdlr(pRad, NULL);
    return rc;
}

SWL_TABLE(sVapCfgParamsActionMap,
          ARR(char* param; whm_mxl_hapd_action_e action; ),
          ARR(swl_type_charPtr, swl_type_uint32, ),
          ARR(//params/object set and applied with hostapd actions
              //Actions applied with hostapd restart
              {"UnsolBcastProbeRespInterval",   HAPD_ACTION_NEED_RESTART},
              {"FilsDiscoveryMaxInterval",      HAPD_ACTION_NEED_RESTART},
              //Actions applied with hostapd toggle
              {"EnableHairpin",                 HAPD_ACTION_NEED_TOGGLE},
              {"ManagementFramesRate",          HAPD_ACTION_NEED_TOGGLE},
              {"NumResSta",                     HAPD_ACTION_NEED_TOGGLE},
              {"VendorVht",                     HAPD_ACTION_NEED_TOGGLE},
              {"MLOEnable",                     HAPD_ACTION_NEED_TOGGLE},
              {"ApMldMac",                      HAPD_ACTION_NEED_TOGGLE},
              {"WdsSingleMlAssoc",              HAPD_ACTION_NEED_TOGGLE},
              {"WdsPrimaryLink",                HAPD_ACTION_NEED_TOGGLE},
              {"SoftBlockAclEnable",            HAPD_ACTION_NEED_TOGGLE},
              {"SoftBlockAclWaitTime",          HAPD_ACTION_NEED_TOGGLE},
              {"SoftBlockAclAllowTime",         HAPD_ACTION_NEED_TOGGLE},
              {"SoftBlockAclOnAuthReq",         HAPD_ACTION_NEED_TOGGLE},
              {"SoftBlockAclOnProbeReq",        HAPD_ACTION_NEED_TOGGLE},
              {"DynamicMulticastMode",          HAPD_ACTION_NEED_TOGGLE},
              {"DynamicMulticastRate",          HAPD_ACTION_NEED_TOGGLE},
              {"DisableBeaconProtection",       HAPD_ACTION_NEED_TOGGLE},
              {"SetBridgeMode",                 HAPD_ACTION_NEED_TOGGLE},
              {"MboCellAware",                  HAPD_ACTION_NEED_TOGGLE},
              {"Set11nProtection",              HAPD_ACTION_NEED_TOGGLE},
              {"EmlCapabTransitionTimeout",     HAPD_ACTION_NEED_TOGGLE},
              {"ApProtectedKeepAliveRequired",  HAPD_ACTION_NEED_TOGGLE},
              {"MldMediumsyncPresent",          HAPD_ACTION_NEED_TOGGLE},
              {"MldMediumsyncDuration",         HAPD_ACTION_NEED_TOGGLE},
              {"MldMediumsyncOfdmedthresh",     HAPD_ACTION_NEED_TOGGLE},
              {"MldMediumsyncMaxtxop",          HAPD_ACTION_NEED_TOGGLE},
              {"MloT2lmSupport",                HAPD_ACTION_NEED_TOGGLE},
              {"WnmBssTransQueryAutoresp",      HAPD_ACTION_NEED_TOGGLE},
              {"RrmNeighRpt",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVICWMin",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVOCWMin",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBECWMin",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBKCWMin",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVICWMax",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVOCWMax",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBECWMax",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBKCWMax",                  HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVIAifs",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVOAifs",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBEAifs",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBKAifs",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVITXOP",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVOTXOP",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBETXOP",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBKTXOP",                   HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVIAcm",                    HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcVOAcm",                    HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBEAcm",                    HAPD_ACTION_NEED_TOGGLE},
              {"WmmAcBKAcm",                    HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueVICWMin",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueVOCWMin",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueBECWMin",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueBKCWMin",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueVICWMax",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueVOCWMax",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueBECWMax",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueBKCWMax",                HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueVIAifs",                 HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueVOAifs",                 HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueBEAifs",                 HAPD_ACTION_NEED_TOGGLE},
              {"TxQueueBKAifs",                 HAPD_ACTION_NEED_TOGGLE},
              {"GasCBDelay",                    HAPD_ACTION_NEED_TOGGLE},
              {"SCSEnable",                     HAPD_ACTION_NEED_TOGGLE},
              {"MSCSEnable",                    HAPD_ACTION_NEED_TOGGLE},
              {"QoSMap",                        HAPD_ACTION_NEED_TOGGLE},
              //Actions applied with sighup to hostpad
              {"RadiusSecretKey",               HAPD_ACTION_NEED_SIGHUP},
              {"OWETransitionBSSID",            HAPD_ACTION_NEED_SIGHUP},
              {"OWETransitionSSID",             HAPD_ACTION_NEED_SIGHUP},
              {"ClientDisallow",                HAPD_ACTION_NEED_SIGHUP},
              {"DisablePbac",                   HAPD_ACTION_NEED_SIGHUP},
              {"SetAggrConfig",                 HAPD_ACTION_NEED_SIGHUP},
              {"GroupMgmtCipher",               HAPD_ACTION_NEED_SIGHUP},
              {"GroupCipher",                   HAPD_ACTION_NEED_SIGHUP},
              //Actions applied with update beacon
              {"BssTransition",                 HAPD_ACTION_NEED_UPDATE_BEACON},
              {"ApMaxInactivity",               HAPD_ACTION_NEED_UPDATE_BEACON},
              {"EnableBssLoad",                 HAPD_ACTION_NEED_UPDATE_BEACON},
              //Action applied with update hostapd conf
              {"MgmtFramePowerControl",         HAPD_ACTION_NEED_UPDATE_CONF},
              {"Ignore11vDiassoc",              HAPD_ACTION_NEED_UPDATE_CONF},
              //Action applied with the Reconf of the AccessPoint
              {"EhtMacEpcsPrioAccess",          HAPD_ACTION_NEED_RECONF},
              ));

static swl_rc_ne whm_mxl_hostapd_getVapParamAction(whm_mxl_hapd_action_e* pOutMappedAction, const char* paramName) {
    whm_mxl_hapd_action_e* pMappedAction = (whm_mxl_hapd_action_e*) swl_table_getMatchingValue(&sVapCfgParamsActionMap, 1, 0, paramName);
    ASSERTS_NOT_NULL(pMappedAction, SWL_RC_ERROR, ME, "NULL");
    W_SWL_SETPTR(pOutMappedAction, *pMappedAction);
    return SWL_RC_OK;
}

/**
 * @brief Determine which actions to take when specific VAP parameter is changed
 *
 * @param pAP accesspoint
 * @param paramName parameter name in data model
 * @param paramValue param value represented as a string
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_determineVapParamAction(T_AccessPoint* pAP, const char* paramName, const char* paramValue) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");
    ASSERT_NOT_NULL(paramName, SWL_RC_INVALID_PARAM, ME, "paramName is NULL");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    whm_mxl_hapd_action_e action = HAPD_ACTION_NONE;
    swl_rc_ne rc;
    whm_mxl_actionHandler_f pfVapActionHdlr;
    bool ret = 0;

    /* First try setting parameter dynamically via ctrl interface before applying any action */
    if(wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface) && (paramValue != NULL)) {
        const char* pConfName= (char*) swl_table_getMatchingValue(&sVendorParamsOdlToConf, 1, 0, paramName);
        ret = wld_ap_hostapd_setParamValue(pAP, pConfName, paramValue, paramName);
    }

    whm_mxl_hostapd_getVapParamAction(&action, paramName);
    ASSERT_NOT_EQUALS(action, HAPD_ACTION_ERROR, SWL_RC_INVALID_PARAM, ME, "Action HAPD_ACTION_ERROR");
    SAH_TRACEZ_INFO(ME, "%s: paramName=%s action=%d", pAP->alias, paramName, action);
    if ((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN)) {
        action = HAPD_ACTION_NONE;
        SAH_TRACEZ_INFO(ME, "%s: Invalid radio state(%d), forcing action HAPD_ACTION_NONE", pRad->Name, pRad->status);
    } else if (pAP->status == APSTI_DISABLED) {
        action = HAPD_ACTION_NEED_UPDATE_CONF;
        SAH_TRACEZ_INFO(ME, "%s: AP state(%d), forcing action HAPD_ACTION_NEED_UPDATE_CONF", pRad->Name, pAP->status);
    }
    pfVapActionHdlr = whm_mxl_getActionHdlr(action);
    ASSERTS_NOT_NULL(pfVapActionHdlr, SWL_RC_NOT_IMPLEMENTED, ME, "No handler for action(%d)", action);
    rc = pfVapActionHdlr(pRad, pAP);
    return rc;
}

static swl_rc_ne s_doEpUpdate(T_Radio* pRad _UNUSED, T_EndPoint* pEP) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "No pEP Mapped");
    pEP->pFA->mfn_wendpoint_update(pEP, SET);
    return SWL_RC_OK;
}

static swl_rc_ne s_doEpNoAction(T_Radio* pRad _UNUSED, T_EndPoint* pEP _UNUSED) {
    SAH_TRACEZ_INFO(ME, "No EP Action");
    return SWL_RC_OK;
}

SWL_TABLE(sEpActionHandlers,
          ARR(whm_mxl_hapd_action_e action; void* actionCb; ),
          ARR(swl_type_uint32, swl_type_voidPtr, ),
          ARR(//hapd actions mapping to execute functions
              {WPA_SUPP_ACTION_ERROR,               NULL},
              {WPA_SUPP_ACTION_NONE,                s_doEpNoAction},
              {WPA_SUPP_ACTION_NEED_UPDATE,         s_doEpUpdate},
              ));

SWL_TABLE(sEpCfgParamsActionMap,
          ARR(char* param; whm_mxl_supplicant_action_e action; ),
          ARR(swl_type_charPtr, swl_type_uint32, ),
          ARR(//params/object set and applied with hostapd actions
              {"Wds",              WPA_SUPP_ACTION_NEED_UPDATE},
              {"VendorElements",   WPA_SUPP_ACTION_NEED_UPDATE},
              {"MultiApProfile",   WPA_SUPP_ACTION_NEED_UPDATE},
              {"WpsCredAddSae",    WPA_SUPP_ACTION_NEED_UPDATE},
              ));

static swl_rc_ne whm_mxl_hostapd_getEpParamAction(whm_mxl_supplicant_action_e* pOutMappedAction, const char* paramName) {
    whm_mxl_supplicant_action_e* pMappedAction = (whm_mxl_supplicant_action_e*) swl_table_getMatchingValue(&sEpCfgParamsActionMap, 1, 0, paramName);
    ASSERTS_NOT_NULL(pMappedAction, SWL_RC_ERROR, ME, "NULL");
    W_SWL_SETPTR(pOutMappedAction, *pMappedAction);
    return SWL_RC_OK;
}

static whm_mxl_actionEpHandler_f whm_mxl_getEpActionHdlr(uint32_t action) {
    whm_mxl_actionEpHandler_f* pfActionHdlr = (whm_mxl_actionEpHandler_f*) swl_table_getMatchingValue(&sEpActionHandlers, 1, 0, &action);
    ASSERTS_NOT_NULL(pfActionHdlr, NULL, ME, "no internal hdlr defined for EP action(%d)", action);
    return *pfActionHdlr;
}

/**
 * @brief Determine which actions to take when specific EP parameter is changed
 *
 * @param pEP endpoint
 * @param paramName parameter name in data model
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_determineEpParamAction(T_EndPoint* pEP, const char* paramName) {
    ASSERT_NOT_NULL(pEP, SWL_RC_INVALID_PARAM, ME, "No pEP Mapped");
    ASSERT_NOT_NULL(paramName, SWL_RC_INVALID_PARAM, ME, "paramName is NULL");
    T_Radio* pRad = pEP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    whm_mxl_supplicant_action_e action = WPA_SUPP_ACTION_NONE;
    swl_rc_ne rc;
    whm_mxl_actionEpHandler_f pfEpActionHdlr;

    whm_mxl_hostapd_getEpParamAction(&action, paramName);
    ASSERT_NOT_EQUALS(action, WPA_SUPP_ACTION_ERROR, SWL_RC_INVALID_PARAM, ME, "Action WPA_SUPP_ACTION_ERROR");
    SAH_TRACEZ_INFO(ME, "%s: paramName=%s action=%d", pEP->Name, paramName, action);
    if ((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN)) {
        action = WPA_SUPP_ACTION_NONE;
        SAH_TRACEZ_INFO(ME, "%s: Invalid radio state(%d), forcing action WPA_SUPP_ACTION_NONE", pRad->Name, pRad->status);
    }
    pfEpActionHdlr = whm_mxl_getEpActionHdlr(action);
    ASSERTS_NOT_NULL(pfEpActionHdlr, SWL_RC_NOT_IMPLEMENTED, ME, "No handler for action(%d)", action);
    rc = pfEpActionHdlr(pRad, pEP);
    return rc;
}

/**
 * @brief Request hostapd restart from pwhm state machine
 *
 * @param pRad radio
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_restartHapd(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    return s_doHapdRestart(pRad, NULL);
}

/**
 * @brief Request hostapd restart (Kill and Restart) from pwhm state machine for MLO config
 *
 * @param pRad radio
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_mlo_restartHapd(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    wld_secDmn_setRestartNeeded(pRad->hostapd, true);
    return s_doHapdRestart(pRad, NULL);
}

/**
 * @brief Request hostapd toggle (interface disable --> enable) from pwhm state machine
 *
 * @param pRad radio
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_toggleHapd(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    return s_doHapdToggle(pRad, NULL);
}

/**
 * @brief Request hostapd refresh (SIGHUP) from pwhm state machine
 *
 * @param pRad radio
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_sighupHapd(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    return s_doHapdSighup(pRad, NULL);
}

/**
 * @brief Do UPDATE_BEACON on requeseted AP and sync configuration file
 *
 * @param pRad radio
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_updateBeaconHapd(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    return s_doUpdateBeacon(NULL, pAP);
}

/**
 * @brief Request to update hostapd configuration file
 *
 * @param pRad radio
 * @param pAP accesspoint - can also pass NULL to sync via master VAP AP
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_confModHapd(T_Radio* pRad, T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    /* pAP NULL is a valid input - hence not checked */
    return s_doHapdConfUpdate(pRad, pAP);
}

/**
 * @brief Request to restart security daemon for all the radios
 *
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_restartAllRadios() {
    T_Radio* pRad;
    wld_for_eachRad(pRad) {
        if (pRad && pRad->pBus) {
            SAH_TRACEZ_INFO(ME, "%s: Restarting radio", pRad->Name);
            whm_mxl_restartHapd(pRad);
        }
    }
    T_Radio* zwdfsRadio = mxl_rad_getZwDfsRadio();
    if (zwdfsRadio) {
        SAH_TRACEZ_INFO(ME, "Restarting ZWDFS radio");
        whm_mxl_restartHapd(zwdfsRadio);
    } else {
        SAH_TRACEZ_WARNING(ME, "Unable to restart ZWDFS radio, radio ctx does not exist");
    }
    return SWL_RC_OK;
}

static bool s_whm_mxl_setParam(T_AccessPoint* pAP, const char* param, const char* newValue) {
    ASSERTS_NOT_NULL(param, false, ME, "NULL");
    ASSERTS_FALSE(swl_str_isEmpty(newValue), false, ME, "newValue does not exist");
    return wld_ap_hostapd_setParamValue(pAP, param, newValue, param);
}

/**
 * @brief Do hostapd SET for array Vendor VAP parameters
 *
 * @param pAP accesspoint
 * @param params set of hostapd config parameters
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_set_vendorMultipleParams(T_AccessPoint* pAP, const char* params[], uint32_t nParams) {
    ASSERTS_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    swl_mapChar_t newVapParams;
    swl_mapChar_t* pNewVapParams = &newVapParams;
    swl_mapChar_init(pNewVapParams);
    whm_mxl_vap_updateConfigMap(pAP, pNewVapParams);
    for(uint32_t i = 0; i < nParams; i++) {
        s_whm_mxl_setParam(pAP, params[i], swl_mapChar_get(pNewVapParams, (char*) params[i]));
    }
    swl_mapChar_cleanup(pNewVapParams);
    return SWL_RC_OK;
}

static swl_rc_ne s_bApSetParams(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    char bApSsid[256] = {0};
    char bApPass[256] = {0};
    swl_mapChar_t newVapParams;
    swl_mapChar_t* pNewVapParams = &newVapParams;
    const char* bApParams[] = {
        "multi_ap_client_disallow"
    };
    whm_mxl_set_vendorMultipleParams(pAP, bApParams, SWL_ARRAY_SIZE(bApParams));

    /* Fetch and prepare bAP credentials */
    swl_mapChar_init(pNewVapParams);
    wld_hostapd_cfgFile_setVapConfig(pAP, pNewVapParams, (swl_mapChar_t*) NULL);
    swl_str_catFormat(bApSsid, sizeof(bApSsid), "SET %s \"%s\"", "multi_ap_backhaul_ssid", swl_mapChar_get(pNewVapParams, "ssid"));
    if (!swl_str_isEmpty(swl_mapChar_get(pNewVapParams, "wpa_passphrase"))) {
        swl_str_catFormat(bApPass, sizeof(bApPass), "SET %s %s", "multi_ap_backhaul_wpa_passphrase",
                            swl_mapChar_get(pNewVapParams, "wpa_passphrase"));
    } else {
        swl_str_catFormat(bApPass, sizeof(bApPass), "SET %s %s", "multi_ap_backhaul_wpa_psk",
                            swl_mapChar_get(pNewVapParams, "wpa_psk"));
    }

    /* Update colocated fAPs with the newly created bAP credentials */
    amxc_llist_for_each(ap_it, &pRad->llAP) {
        T_AccessPoint* pOtherAp = amxc_llist_it_get_data(ap_it, T_AccessPoint, it);
        mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pOtherAp);
        if ((mxlVapVendorData == NULL) || (whm_mxl_utils_isDummyVap(pOtherAp))) {
            continue;
        }
        if (mxlVapVendorData->mxlMultiApType == MXL_FRONTHAUL_MAP) {
            /* Update bAP credentials */
            SAH_TRACEZ_INFO(ME, "%s: sending cmd:%s to AP(%s)", pAP->alias, bApSsid, pOtherAp->alias);
            wld_ap_hostapd_sendCommand(pOtherAp, bApSsid, "multi ap backhaul ssid");
            SAH_TRACEZ_INFO(ME, "%s: sending cmd:%s to AP(%s)", pAP->alias, bApPass, pOtherAp->alias);
            wld_ap_hostapd_sendCommand(pOtherAp, bApPass, "multi ap backhaul password");
        }
    }
    swl_mapChar_cleanup(pNewVapParams);

    return SWL_RC_OK;
}

static swl_rc_ne s_hybridApSetParams(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");
    char hybridApSsid[256] = {0};
    char hybridApPass[256] = {0};
    swl_mapChar_t newVapParams;
    swl_mapChar_t* pNewVapParams = &newVapParams;

    const char* hybridApParams[] = {
        "multi_ap_client_disallow"
    };
    whm_mxl_set_vendorMultipleParams(pAP, hybridApParams, SWL_ARRAY_SIZE(hybridApParams));

    /* Fetch and update hybrid AP credentials */
    swl_mapChar_init(pNewVapParams);
    wld_hostapd_cfgFile_setVapConfig(pAP, pNewVapParams, (swl_mapChar_t*) NULL);
    swl_str_catFormat(hybridApSsid, sizeof(hybridApSsid), "SET %s \"%s\"", "multi_ap_backhaul_ssid", swl_mapChar_get(pNewVapParams, "ssid"));
    if (!swl_str_isEmpty(swl_mapChar_get(pNewVapParams, "wpa_passphrase"))) {
        swl_str_catFormat(hybridApPass, sizeof(hybridApPass), "SET %s %s", "multi_ap_backhaul_wpa_passphrase",
                            swl_mapChar_get(pNewVapParams, "wpa_passphrase"));
    } else {
        swl_str_catFormat(hybridApPass, sizeof(hybridApPass), "SET %s %s", "multi_ap_backhaul_wpa_psk",
                            swl_mapChar_get(pNewVapParams, "wpa_psk"));
    }
    SAH_TRACEZ_INFO(ME, "%s: sending cmd:%s", pAP->alias, hybridApSsid);
    wld_ap_hostapd_sendCommand(pAP, hybridApSsid, "multi ap backhaul ssid");
    SAH_TRACEZ_INFO(ME, "%s: sending cmd:%s", pAP->alias, hybridApPass);
    wld_ap_hostapd_sendCommand(pAP, hybridApPass, "multi ap backhaul password");

    swl_mapChar_cleanup(pNewVapParams);

    return SWL_RC_OK;
}

/**
 * @brief Request multi AP type update from pwhm state machine 
 *
 * @param pAP accesspoint
 * @return return code of executed action.
 */
swl_rc_ne whm_mxl_updateMultiAp(T_AccessPoint* pAP) {
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    mxl_VapVendorData_t* mxlVapVendorData = mxl_vap_getVapVendorData(pAP);
    ASSERTS_NOT_NULL(mxlVapVendorData, SWL_RC_INVALID_PARAM, ME, "mxlVapVendorData is NULL");
    whm_mxl_multi_ap_type_e multiApType = mxlVapVendorData->mxlMultiApType;

    ASSERT_TRUE((multiApType < MXL_MAP_TYPE_MAX), SWL_RC_ERROR, ME, "%s: Invalid Multi AP type(%d)", pAP->alias, multiApType);
    ASSERTI_FALSE((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN), SWL_RC_INVALID_STATE, ME, "%s: Invalid radio state(%d)", pRad->Name, pRad->status);
    ASSERTI_TRUE(wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface), SWL_RC_INVALID_STATE, ME, "%s: wpaCtrl disconnected", pAP->alias);

    /* Update generic multi ap params */
    wld_secDmn_action_rc_ne rc = wld_ap_hostapd_setNoSecParams(pAP);
    ASSERT_FALSE(rc < SECDMN_ACTION_OK_DONE, SWL_RC_ERROR, ME, "%s: fail to set common params", pAP->alias);

    switch (multiApType) {
        case MXL_BACKHAUL_MAP: {
            s_bApSetParams(pAP);
            break;
        }
        case MXL_HYBRID_MAP: {
            s_hybridApSetParams(pAP);
            break;
        }
        case MXL_FRONTHAUL_MAP:
            /* Fallthrough */
        case MXL_MAP_OFF:
            break;
        default:
            break;
    }
    whm_mxl_toggleHapd(pRad);
    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_handleMbssidOverride(T_Radio* pRad, bool overideMbssid) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "pRad is NULL");
    ASSERT_FALSE((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN), SWL_RC_OK, ME, "%s: Invalid radio state(%d)", pRad->Name, pRad->status);
    T_AccessPoint* masterVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(masterVap, SWL_RC_INVALID_PARAM, ME, "masterVap is NULL");
    if ((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN)) {
        return SWL_RC_OK;
    }
    if (wld_wpaCtrlInterface_isReady(masterVap->wpaCtrlInterface)) {
        wld_ap_hostapd_setParamValue(masterVap, "override_6g_mbssid_default_mode", (overideMbssid ? "1" : "0"), "override 6G mbssid");
        /* Reset multibss_enable in hostapd after setting/resetting 6G mbssid override and let hostapd manage */
        wld_ap_hostapd_setParamValue(masterVap, "multibss_enable", "0", "disable multibss");
    }
    whm_mxl_toggleHapd(pRad);
    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_configureSaeExt(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");

    const char* secApParams[] = {
        "wpa_key_mgmt"
    };

    whm_mxl_set_vendorMultipleParams(pAP, secApParams, SWL_ARRAY_SIZE(secApParams));
    whm_mxl_toggleHapd(pRad);
    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_hostapd_setMldParams(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");

    const char* mldParams[] = {
        "mlo_enable"
    };

    whm_mxl_set_vendorMultipleParams(pAP, mldParams, SWL_ARRAY_SIZE(mldParams));
    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_toggleWPA3PersonalCompatibility(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "No Radio Mapped");
    ASSERT_FALSE((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN), SWL_RC_INVALID_STATE, ME, "%s: Invalid radio state(%d)", pRad->Name, pRad->status);
    ASSERTI_TRUE(wld_secDmn_isAlive(pRad->hostapd), SWL_RC_ERROR, ME, "hostapd not active");

    const char* secApParams[] = {
        "wpa", "wpa_key_mgmt", "rsn_pairwise", "group_cipher",
        "ieee80211w", "sae_pwe", "sae_require_mfp",
        "rsn_override_key_mgmt", "rsn_override_pairwise",
        "rsn_override_mfp", "rsn_override_key_mgmt_2",
        "rsn_override_pairwise_2", "rsn_override_mfp_2"
    };

    whm_mxl_set_vendorMultipleParams(pAP, secApParams, SWL_ARRAY_SIZE(secApParams));
    whm_mxl_sighupHapd(pRad);
    return SWL_RC_OK;
}

static bool s_isParamChanged(swl_mapChar_t* pCurrVapParams,
                              const char* param, const char* newValue) {
    ASSERTS_NOT_NULL(param, false, ME, "NULL");
    ASSERTS_NOT_NULL(pCurrVapParams, false, ME, "NULL");
    const char* oldValue = swl_mapChar_get(pCurrVapParams, (char*) param);
    ASSERTS_FALSE(swl_str_matches(oldValue, newValue), false, ME, "same value");
    return true;
}

static bool s_isAnyMultipleParamsChanged(swl_mapChar_t* pCurrVapParams, swl_mapChar_t* pNewVapParams,
                                    const char* params[], uint32_t nParams) {
    ASSERTS_NOT_NULL(pCurrVapParams, false, ME, "NULL");
    ASSERTS_NOT_NULL(pNewVapParams, false, ME, "NULL");
    bool changed = false;
    for (uint32_t i = 0; i < nParams; i++) {
        changed |= s_isParamChanged(pCurrVapParams, params[i], swl_mapChar_get(pNewVapParams, (char*) params[i]));
    }
    return changed;
}

static whm_mxl_config_flow_e s_chooseGeneralConfigFlow(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "No pAP Mapped");
    // Map generic parameters here to be configured by reconf
    return WHM_MXL_CONFIG_FLOW_GENERIC;
}

static whm_mxl_config_flow_e s_chooseSsidConfigFlow(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "No pAP Mapped");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "No pRad Mapped");
    /* Schedule reconf only when hostapd is running */
    ASSERTI_TRUE(wld_secDmn_isEnabled(pRad->hostapd), WHM_MXL_CONFIG_FLOW_GENERIC, ME, "hostapd is not running");
    wld_hostapd_config_t* pCurrentConfig = NULL;
    bool ret = wld_hostapd_loadConfig(&pCurrentConfig, pRad->hostapd->cfgFile);
    ASSERTI_TRUE(ret, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "no saved current config");
    swl_mapChar_t* pCurrVapParams = wld_hostapd_getConfigMapByBssid(pCurrentConfig, (swl_macBin_t*) pAP->pSSID->BSSID);
    swl_mapChar_t newVapParams;
    swl_mapChar_t* pNewVapParams = &newVapParams;
    swl_mapChar_init(pNewVapParams);
    wld_hostapd_cfgFile_setVapConfig(pAP, pNewVapParams, (swl_mapChar_t*) NULL);

    /* SSID requires extra checks - handle it first */
    const char* ssidVapParams[] = {
        "ssid"
    };
    bool ssidChanged = s_isAnyMultipleParamsChanged(pCurrVapParams, pNewVapParams, ssidVapParams, SWL_ARRAY_SIZE(ssidVapParams));
    if (ssidChanged && !pAP->SSIDAdvertisementEnabled) {
        SAH_TRACEZ_INFO(ME, "%s: SSID changed with hidden ssid enabled - need reconf", pAP->alias);
        swl_mapChar_cleanup(pNewVapParams);
        wld_hostapd_deleteConfig(pCurrentConfig);
        return WHM_MXL_CONFIG_FLOW_RECONF;
    }

    const char* ssidRelatedVapParams[] = {
        "ignore_broadcast_ssid"
    };
    bool anyChanged = s_isAnyMultipleParamsChanged(pCurrVapParams, pNewVapParams, ssidRelatedVapParams, SWL_ARRAY_SIZE(ssidRelatedVapParams));

    swl_mapChar_cleanup(pNewVapParams);
    wld_hostapd_deleteConfig(pCurrentConfig);

    return (anyChanged ? WHM_MXL_CONFIG_FLOW_RECONF : WHM_MXL_CONFIG_FLOW_GENERIC);
}

static whm_mxl_config_flow_e s_chooseSecurityConfigFlow(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "No pAP Mapped");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "No pRad Mapped");
    /* Schedule reconf only when hostapd is running */
    ASSERTI_TRUE(wld_secDmn_isEnabled(pRad->hostapd), WHM_MXL_CONFIG_FLOW_GENERIC, ME, "hostapd is not running");
    wld_hostapd_config_t* pCurrentConfig = NULL;
    bool ret = wld_hostapd_loadConfig(&pCurrentConfig, pRad->hostapd->cfgFile);
    ASSERTI_TRUE(ret, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "no saved current config");
    swl_mapChar_t* pCurrVapParams = wld_hostapd_getConfigMapByBssid(pCurrentConfig, (swl_macBin_t*) pAP->pSSID->BSSID);
    swl_mapChar_t newVapParams;
    swl_mapChar_t* pNewVapParams = &newVapParams;
    swl_mapChar_init(pNewVapParams);
    wld_hostapd_cfgFile_setVapConfig(pAP, pNewVapParams, (swl_mapChar_t*) NULL);

    const char* secVapParams[] = {
        "wpa", "wpa_group_rekey", "wps_state"
    };
    bool anyChanged = s_isAnyMultipleParamsChanged(pCurrVapParams, pNewVapParams, secVapParams, SWL_ARRAY_SIZE(secVapParams));

    swl_mapChar_cleanup(pNewVapParams);
    wld_hostapd_deleteConfig(pCurrentConfig);

    return (anyChanged ? WHM_MXL_CONFIG_FLOW_RECONF : WHM_MXL_CONFIG_FLOW_GENERIC);
}

whm_mxl_config_flow_e whm_mxl_chooseVapConfigFlow(T_AccessPoint* pAP, whm_mxl_config_type_e type) {
    ASSERT_NOT_NULL(pAP, WHM_MXL_CONFIG_FLOW_GENERIC, ME, "No pAP Mapped");
    whm_mxl_config_flow_e flow = WHM_MXL_CONFIG_FLOW_GENERIC;

    switch (type) {
        case WHM_MXL_CONFIG_TYPE_GENRAL: {
            flow = s_chooseGeneralConfigFlow(pAP);
            break;
        }
        case WHM_MXL_CONFIG_TYPE_SSID: {
            flow = s_chooseSsidConfigFlow(pAP);
            break;
        }
        case WHM_MXL_CONFIG_TYPE_SECURITY: {
            flow = s_chooseSecurityConfigFlow(pAP);
            break;
        }
        default: {
            SAH_TRACEZ_NOTICE(ME, "%s: Invalid config type(%d)", pAP->alias, type);
            break;
        }

    }

    return flow;
}

static void s_updateMaxAssociatedDevices(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, , ME, "pAP is NULL");
    T_Radio* pRad = pAP->pRadio;
    ASSERT_NOT_NULL(pRad, , ME, "pRad is NULL");
    ASSERTI_FALSE((pRad->status == RST_ERROR) || (pRad->status == RST_UNKNOWN), , ME, "%s: Invalid radio state(%d)", pRad->Name, pRad->status);
    ASSERTI_TRUE(wld_secDmn_isAlive(pRad->hostapd), , ME, "hostapd not active");
    SAH_TRACEZ_NOTICE(ME, "%s: Updating Number of Max associated Devices", pAP->alias);
    if(pAP->ActiveAssociatedDeviceNumberOfEntries > pAP->MaxStations) {
        swl_mapChar_t newVapParams;
        swl_mapChar_t* pNewVapParams = &newVapParams;
        swl_mapChar_init(pNewVapParams);
        wld_hostapd_cfgFile_setVapConfig(pAP, pNewVapParams, (swl_mapChar_t*) NULL);
        wld_ap_hostapd_setParamValue(pAP, "max_num_sta", swl_mapChar_get(pNewVapParams, "max_num_sta"), "update max_num_sta");
        swl_mapChar_cleanup(pNewVapParams);
        whm_mxl_toggleHapd(pRad);
    }
}

swl_rc_ne whm_mxl_updateOnEventMaxAssociatedDevices(T_AccessPoint* pAP) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "pAP is NULL");
    swla_delayExec_addTimeout((swla_delayExecFun_cbf) s_updateMaxAssociatedDevices, pAP, DM_EVENT_HOOK_TIMEOUT_MS);
    return SWL_RC_OK;
}

#ifdef CONFIG_VENDOR_MXL_PROPRIETARY
/**
 * @brief configure BG ACS Scan Interval
 *
 * @param pRad radio
 * @param bgAcsInterval BG ACS interval in seconds
 * @return SWL_RC_OK when executed successfully. Otherwise matching SWL error.
 */
swl_rc_ne whm_mxl_configureBgAcs(T_Radio* pRad, uint16_t bgAcsInterval) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");
    T_AccessPoint* masterVap = wld_rad_getFirstVap(pRad);
    ASSERT_NOT_NULL(masterVap, SWL_RC_INVALID_PARAM, ME, "masterVap is NULL");
    bool ctrlIfaceReady = wld_wpaCtrlInterface_isReady(masterVap->wpaCtrlInterface);

    if (ctrlIfaceReady) {
        char interval[256] = {0};
        swl_str_catFormat(interval, sizeof(interval), "%u", bgAcsInterval);
        /* Set bg acs interval in minutes */
        wld_ap_hostapd_setParamValue(masterVap, "acs_bgscan_interval", interval, "bg_acs_interval");
    }

    /* Check if we can configure BG ACS Scan on the fly */
    if (pRad->autoChannelEnable || !bgAcsInterval) {
        if (wld_rad_isActive(pRad) && ctrlIfaceReady) {
            char cmd[256] = {0};
            /* Command format: -i <ifname> ACS_BG_SCAN_INTERVAL <timeout_in_mins> */
            swl_str_catFormat(cmd, sizeof(cmd), "ACS_BG_SCAN_INTERVAL %u", bgAcsInterval);
            whm_mxl_hostapd_sendCommand(masterVap, cmd, "bg_acs_interval");
        }
    } else {
        SAH_TRACEZ_NOTICE(ME, "%s: Unable to conifgure BG ACS on the fly (acs %d interval %u)", pRad->Name, pRad->autoChannelEnable, bgAcsInterval);
    }

    s_doHapdConfUpdate(pRad, NULL);

    return SWL_RC_OK;
}
#endif /* CONFIG_VENDOR_MXL_PROPRIETARY */

/**
 * @brief wraper for the generic pwhm send hostapd command
 *
 * @param pAP accesspoint
 * @param cmd the command to send
 * @param reason the command caller
 * @return true when the SET cmd is executed successfully. Otherwise false.
 */
bool whm_mxl_hostapd_sendCommand(T_AccessPoint* pAP, char* cmd, const char* reason) {
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "No pAP Mapped");
    return wld_ap_hostapd_sendCommand(pAP, cmd, reason);
}

/**
 * @brief Send hostapd command and check if the response includes specific string
 *
 * @param pAP accesspoint
 * @param cmd the command to send
 * @param reason the command caller
 * @param expectedResponse the specific string to check if included in the response from hostapd
 * @return true when the SET cmd is executed successfully. Otherwise false.
 */
bool whm_mxl_wpaCtrl_sendCmdCheckSpecificResponse(T_AccessPoint* pAP, char* cmd, const char* reason, char* expectedResponse) {
    ASSERTS_NOT_NULL(pAP, false, ME, "NULL");
    ASSERTS_TRUE(wld_wpaCtrlInterface_isReady(pAP->wpaCtrlInterface), false, ME, "%s: wpactrl link not ready", pAP->alias);
    SAH_TRACEZ_INFO(ME, "%s: send hostapd cmd %s for %s",
                    wld_wpaCtrlInterface_getName(pAP->wpaCtrlInterface), cmd, reason);
    char reply[MSG_LENGTH] = {'\0'};
    // send the command
    ASSERTS_TRUE(wld_wpaCtrl_sendCmdSynced(pAP->wpaCtrlInterface, cmd, reply, sizeof(reply) - 1), false, ME, "sending cmd %s failed", cmd);
    SAH_TRACEZ_INFO(ME, "Hostapd cmd reply is %s", reply);
    // check the response
    ASSERT_TRUE(swl_str_nmatches(reply, expectedResponse, strlen(expectedResponse)), false, ME, "cmd(%s) reply(%s): unmatch expect(%s)", cmd, reply, expectedResponse);

    return true;
}
