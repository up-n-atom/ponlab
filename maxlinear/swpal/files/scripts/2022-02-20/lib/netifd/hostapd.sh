. /lib/functions/network.sh
. /lib/functions.sh
if [ -f /lib/netifd/debug_infrastructure.sh ]; then
	. /lib/netifd/debug_infrastructure.sh
fi
. /lib/functions.sh

wpa_supplicant_add_rate() {
	local var="$1"
	local val="$(($2 / 1000))"
	local sub="$((($2 / 100) % 10))"
	append $var "$val" ","
	[ $sub -gt 0 ] && append $var "."
}

hostapd_add_rate() {
	local var="$1"
	local val="$(($2 / 100))"
	append $var "$val" " "
}

hostapd_append_wep_key() {
	local var="$1"

	wep_keyidx=0
	set_default key 1
	case "$key" in
		[1234])
			for idx in 1 2 3 4; do
				local zidx
				zidx=$(($idx - 1))
				json_get_var ckey "key${idx}"
				[ -n "$ckey" ] && \
					append $var "wep_key${zidx}=$(prepare_key_wep "$ckey")" "$N$T"
			done
			wep_keyidx=$((key - 1))
		;;
		*)
			append $var "wep_key0=$(prepare_key_wep "$key")" "$N$T"
		;;
	esac
}

hostapd_append_wpa_key_mgmt() {
	local auth_type_l="$(echo $auth_type | tr 'a-z' 'A-Z')"

	case "$auth_type" in
		psk|eap)
			if [ "$mode" = "sta" ]; then
				append wpa_key_mgmt "WPA-$auth_type_l"
				[ "${ieee80211w:-0}" -gt 0 ] && append wpa_key_mgmt "WPA-${auth_type_l}-SHA256"
			else
				[ "${ieee80211w:-0}" -ne 2 ] && append wpa_key_mgmt "WPA-$auth_type_l"
				[ "${ieee80211w:-0}" -gt 1 ] && append wpa_key_mgmt "WPA-${auth_type_l}-SHA256"
			fi
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-${auth_type_l}"
		;;
		eap192)
			append wpa_key_mgmt "WPA-EAP-SUITE-B-192"
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-EAP"
		;;
		eap-eap192)
			append wpa_key_mgmt "WPA-EAP-SUITE-B-192"
			append wpa_key_mgmt "WPA-EAP"
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-EAP"
			if [ "$mode" = "sta" ]; then
				[ "${ieee80211w:-0}" -gt 0 ] && append wpa_key_mgmt "WPA-EAP-SHA256"
			else
				[ "${ieee80211w:-0}" -gt 1 ] && append wpa_key_mgmt "WPA-EAP-SHA256"
			fi
		;;
		sae)
			append wpa_key_mgmt "SAE"
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-SAE"
		;;
		sae-ext-key)
			#Configure support for AKM24 and AKM8 only for WiFi-7 WPA3 tests to address the bug WLANRTSYS-63134
			#The file "/tmp/force_akm_suite_type_to_sae_and_sae_ext_key" will create only for WiFi-7 WPA3 tests
			FORCE_AKM_SUITE_TYPE_TO_SAE_AND_SAEEXTKEY_FILE_PATH="/tmp/force_akm_suite_type_to_sae_and_sae_ext_key"
			if [ -e $FORCE_AKM_SUITE_TYPE_TO_SAE_AND_SAEEXTKEY_FILE_PATH ]; then
				append wpa_key_mgmt "SAE"
				[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-SAE"
			fi
			append wpa_key_mgmt "SAE-EXT-KEY"
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-SAE-EXT-KEY"
		;;
		psk-sae)
			append wpa_key_mgmt "WPA-PSK"
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-PSK"
			if [ "$mode" = "sta" ]; then
				[ "${ieee80211w:-0}" -gt 0 ] && append wpa_key_mgmt "WPA-PSK-SHA256"
			else
				[ "${ieee80211w:-0}" -gt 1 ] && append wpa_key_mgmt "WPA-PSK-SHA256"
			fi
			append wpa_key_mgmt "SAE"
			[ "${ieee80211r:-0}" -gt 0 ] && append wpa_key_mgmt "FT-SAE"
		;;
		owe)
			append wpa_key_mgmt "OWE"
		;;
	esac

	[ "$auth_osen" = "1" ] && append wpa_key_mgmt "OSEN"
	[ "$dpp_auth_enabled" -eq 1 ] && append wpa_key_mgmt "DPP"
	[ "$auth_osen" = "1" ] && append wpa_key_mgmt "OSEN"
}

hostapd_append_wmm_params() {
	local var="$1"

	[ -n "$wmm_ac_bk_cwmin" ] && append $var "wmm_ac_bk_cwmin=$wmm_ac_bk_cwmin" "$N"
	[ -n "$wmm_ac_bk_cwmax" ] && append $var "wmm_ac_bk_cwmax=$wmm_ac_bk_cwmax" "$N"
	[ -n "$wmm_ac_bk_aifs"  ] && append $var "wmm_ac_bk_aifs=$wmm_ac_bk_aifs" "$N"
	[ -n "$wmm_ac_bk_txop_limit" ] && append $var "wmm_ac_bk_txop_limit=$wmm_ac_bk_txop_limit" "$N"
	[ -n "$wmm_ac_bk_acm"  ] && append $var "wmm_ac_bk_acm=$wmm_ac_bk_acm" "$N"

	[ -n "$wmm_ac_be_cwmin" ] && append $var "wmm_ac_be_cwmin=$wmm_ac_be_cwmin" "$N"
	[ -n "$wmm_ac_be_cwmax" ] && append $var "wmm_ac_be_cwmax=$wmm_ac_be_cwmax" "$N"
	[ -n "$wmm_ac_be_aifs"  ] && append $var "wmm_ac_be_aifs=$wmm_ac_be_aifs" "$N"
	[ -n "$wmm_ac_be_txop_limit" ] && append $var "wmm_ac_be_txop_limit=$wmm_ac_be_txop_limit" "$N"
	[ -n "$wmm_ac_be_acm"  ] && append $var "wmm_ac_be_acm=$wmm_ac_be_acm" "$N"

	[ -n "$wmm_ac_vi_cwmin" ] && append $var "wmm_ac_vi_cwmin=$wmm_ac_vi_cwmin" "$N"
	[ -n "$wmm_ac_vi_cwmax" ] && append $var "wmm_ac_vi_cwmax=$wmm_ac_vi_cwmax" "$N"
	[ -n "$wmm_ac_vi_aifs"  ] && append $var "wmm_ac_vi_aifs=$wmm_ac_vi_aifs" "$N"
	[ -n "$wmm_ac_vi_txop_limit" ] && append $var "wmm_ac_vi_txop_limit=$wmm_ac_vi_txop_limit" "$N"
	[ -n "$wmm_ac_vi_acm"  ] && append $var "wmm_ac_vi_acm=$wmm_ac_vi_acm" "$N"

	[ -n "$wmm_ac_vo_cwmin" ] && append $var "wmm_ac_vo_cwmin=$wmm_ac_vo_cwmin" "$N"
	[ -n "$wmm_ac_vo_cwmax" ] && append $var "wmm_ac_vo_cwmax=$wmm_ac_vo_cwmax" "$N"
	[ -n "$wmm_ac_vo_aifs"  ] && append $var "wmm_ac_vo_aifs=$wmm_ac_vo_aifs" "$N"
	[ -n "$wmm_ac_vo_txop_limit" ] && append $var "wmm_ac_vo_txop_limit=$wmm_ac_vo_txop_limit" "$N"
	[ -n "$wmm_ac_vo_acm"  ] && append $var "wmm_ac_vo_acm=$wmm_ac_vo_acm" "$N"

	[ -n "$tx_queue_data0_cwmin" ] && append $var "tx_queue_data0_cwmin=$tx_queue_data0_cwmin" "$N"
	[ -n "$tx_queue_data0_cwmax" ] && append $var "tx_queue_data0_cwmax=$tx_queue_data0_cwmax" "$N"
	[ -n "$tx_queue_data0_aifs"  ] && append $var "tx_queue_data0_aifs=$tx_queue_data0_aifs" "$N"
	[ -n "$tx_queue_data0_burst" ] && append $var "tx_queue_data0_burst=$tx_queue_data0_burst" "$N"

	[ -n "$tx_queue_data1_cwmin" ] && append $var "tx_queue_data1_cwmin=$tx_queue_data1_cwmin" "$N"
	[ -n "$tx_queue_data1_cwmax" ] && append $var "tx_queue_data1_cwmax=$tx_queue_data1_cwmax" "$N"
	[ -n "$tx_queue_data1_aifs"  ] && append $var "tx_queue_data1_aifs=$tx_queue_data1_aifs" "$N"
	[ -n "$tx_queue_data1_burst" ] && append $var "tx_queue_data1_burst=$tx_queue_data1_burst" "$N"

	[ -n "$tx_queue_data2_cwmin" ] && append $var "tx_queue_data2_cwmin=$tx_queue_data2_cwmin" "$N"
	[ -n "$tx_queue_data2_cwmax" ] && append $var "tx_queue_data2_cwmax=$tx_queue_data2_cwmax" "$N"
	[ -n "$tx_queue_data2_aifs"  ] && append $var "tx_queue_data2_aifs=$tx_queue_data2_aifs" "$N"
	[ -n "$tx_queue_data2_burst" ] && append $var "tx_queue_data2_burst=$tx_queue_data2_burst" "$N"

	[ -n "$tx_queue_data3_cwmin" ] && append $var "tx_queue_data3_cwmin=$tx_queue_data3_cwmin" "$N"
	[ -n "$tx_queue_data3_cwmax" ] && append $var "tx_queue_data3_cwmax=$tx_queue_data3_cwmax" "$N"
	[ -n "$tx_queue_data3_aifs"  ] && append $var "tx_queue_data3_aifs=$tx_queue_data3_aifs" "$N"
	[ -n "$tx_queue_data3_burst" ] && append $var "tx_queue_data3_burst=$tx_queue_data3_burst" "$N"
}

hostapd_add_log_config() {
	config_add_boolean \
		log_80211 \
		log_8021x \
		log_radius \
		log_wpa \
		log_driver \
		log_iapp \
		log_mlme

	config_add_int log_level
}

hostapd_common_add_device_config() {
	config_add_array basic_rate
	config_add_array supported_rates

	config_add_string country country3
	config_add_string shared_hapd
	config_add_boolean country_ie doth
	config_add_boolean spectrum_mgmt_required
	config_add_int local_pwr_constraint
	config_add_string require_mode
	config_add_string dfs_ch_state_file
	config_add_boolean legacy_rates
	config_add_int cell_density

	config_add_string acs_chan_bias
	config_add_array hostapd_options

	config_add_int airtime_mode
	config_add_boolean acs_use24overlapped
	config_add_boolean tx_mcs_set_defined
	config_add_boolean ignore_supported_channels_errors
	config_add_int sub_band_dfs
	config_add_int dfs_unii4
	config_add_int sRadarRssiTh
	config_add_int ScanRssiTh
	config_add_int acs_scan_no_flush
	config_add_int sta_statistics
	config_add_int sConfigMRCoexActiveTime
	config_add_int sConfigMRCoex
	config_add_int sConfigMRCoexInactiveTime
	config_add_int sConfigMRCoexCts2SelfActive
	config_add_string sCoCPower
	config_add_string sCoCAutoCfg
	config_add_string sErpSet
	config_add_string sFWRecovery
	config_add_string sFixedRateCfg
	config_add_string sInterferDetThresh
	config_add_int acs_bg_scan_do_switch
	config_add_int acs_update_do_switch
	config_add_int num_antennas
	config_add_int owl
	config_add_int notify_action_frame
	config_add_int rts_threshold
	config_add_int fragm_threshold
	config_add_int sBfMode
	config_add_int sMaxMpduLen
	config_add_int sProbeReqCltMode
	config_add_boolean sQAMplus
	config_add_boolean allow_scan_during_cac
	config_add_boolean background_cac
	config_add_boolean dynamic_country
	config_add_boolean dynamic_failsafe
	config_add_int adv_proto_query_resp_len
	config_add_int rssi_reject_assoc_rssi
	config_add_int mem_only_cred
	config_add_string secure_store_hash
	config_add_int process_radar_slave_events
	config_add_boolean dynamic_wmm
	config_add_boolean dynamic_edca
	config_add_string start_after
	config_add_int start_after_delay
	config_add_int start_after_watchdog_time


	if [ -f /lib/netifd/debug_infrastructure.sh ]; then
		debug_infrastructure_config_add_string debug_hostap_conf_
	fi

	hostapd_add_log_config
}

hostapd_prepare_device_config() {
	local config="$1"
	local driver="$2"

	local base_cfg=

	json_get_vars country country3 country_ie beacon_int:100 dtim_period:2 doth require_mode legacy_rates \
		acs_chan_bias local_pwr_constraint spectrum_mgmt_required airtime_mode cell_density dfs_debug_chan \
		sInterferDetThresh acs_bg_scan_do_switch acs_update_do_switch \
		ScanRssiTh acs_scan_no_flush \
		externally_managed testbed_mode sub_band_dfs sCoCPower sCoCAutoCfg sErpSet sFWRecovery sFixedRateCfg \
		sta_statistics notify_action_frame rts_threshold fragm_threshold \
		allow_scan_during_cac background_cac dfs_ch_state_file ignore_supported_channels_errors dynamic_country dynamic_failsafe \
		sConfigMRCoexActiveTime sQAMplus sBfMode sConfigMRCoex sConfigMRCoexCts2SelfActive tx_mcs_set_defined \
		sConfigMRCoexInactiveTime sProbeReqCltMode acs_use24overlapped sMaxMpduLen \
		adv_proto_query_resp_len rssi_reject_assoc_rssi \
		mem_only_cred process_radar_slave_events dfs_unii4 dynamic_wmm dynamic_edca \
		start_after start_after_delay start_after_watchdog_time

        json_get_var sPowerSelection txpower

	hostapd_set_log_options base_cfg

	set_default country_ie 1
	set_default spectrum_mgmt_required 0
	set_default doth 1
	set_default airtime_mode 0
	set_default cell_density 0
	set_default legacy_rates 1
	set_default testbed_mode 0
	set_default sPowerSelection "$txpower"
	set_default dynamic_edca 1
	set_default dynamic_wmm 1

	case "$sPowerSelection" in
		"6") sPowerSelection=12 ;;
		"12") sPowerSelection=9 ;;
		"25") sPowerSelection=6 ;;
		"50") sPowerSelection=3 ;;
		"75") sPowerSelection=1 ;;
		"100") sPowerSelection=0 ;;
		*) sPowerSelection= ;;
	esac

	[ -n "$sCoCPower" ] && append base_cfg "sCoCPower=$sCoCPower" "$N"
	[ -n "$sCoCAutoCfg" ] && append base_cfg "sCoCAutoCfg=$sCoCAutoCfg" "$N"
	[ -n "$sErpSet" ] && append base_cfg "sErpSet=$sErpSet" "$N"
	[ -n "$sPowerSelection" ] && append base_cfg "sPowerSelection=$sPowerSelection" "$N"
	[ -n "$sFWRecovery" ] && append base_cfg "sFWRecovery=$sFWRecovery" "$N"
	[ -n "$sFixedRateCfg" ] && append base_cfg "sFixedRateCfg=$sFixedRateCfg" "$N"
	[ -n "$sInterferDetThresh" ] && append base_cfg "sInterferDetThresh=$sInterferDetThresh" "$N"
	[ -n "$acs_bg_scan_do_switch" ] && append base_cfg "acs_bg_scan_do_switch=$acs_bg_scan_do_switch" "$N"
	[ -n "$acs_update_do_switch" ] && append base_cfg "acs_update_do_switch=$acs_update_do_switch" "$N"
	[ -n "$sta_statistics" ] && append base_cfg "sStationsStat=$sta_statistics" "$N"
	[ -n "$owl" ] && append base_cfg "owl=$owl" "$N"
	[ -n "$notify_action_frame" ] && append base_cfg "notify_action_frame=$notify_action_frame" "$N"
	[ -n "$rts_threshold" ] && append base_cfg "rts_threshold=$rts_threshold" "$N"
	[ -n "$fragm_threshold" ] && append base_cfg "fragm_threshold=$fragm_threshold" "$N"
	[ -n "$sBfMode" ] && append base_cfg "sBfMode=$sBfMode" "$N"
	[ -n "$sProbeReqCltMode" ] && append base_cfg "sProbeReqCltMode=$sProbeReqCltMode" "$N"
	[ -n "$acs_use24overlapped" ] && append base_cfg "acs_use24overlapped=$acs_use24overlapped" "$N"
	[ -n "$sMaxMpduLen" ] && append base_cfg "sMaxMpduLen=$sMaxMpduLen" "$N"
	[ -n "$dynamic_country" ] && append base_cfg "dynamic_country=$dynamic_country" "$N"
	[ -n "$dynamic_failsafe" ] && append base_cfg "dynamic_failsafe=$dynamic_failsafe" "$N"
	[ -n "$ScanRssiTh" ] && append base_cfg "ScanRssiTh=$ScanRssiTh" "$N"
	[ -n "$acs_scan_no_flush" ] && append base_cfg "acs_scan_no_flush=$acs_scan_no_flush" "$N"
	[ -n "$mem_only_cred" ] && append base_cfg "mem_only_cred=$mem_only_cred" "$N"
	[ -n "$dynamic_wmm" ] && append base_cfg "dynamic_wmm=$dynamic_wmm" "$N"
	[ -n "$dynamic_edca" ] && append base_cfg "dynamic_edca=$dynamic_edca" "$N"

	if [ -n "$sConfigMRCoex" ]; then
		append base_cfg "sConfigMRCoex=$sConfigMRCoex" "$N"
		[ -n "$sConfigMRCoexActiveTime" ] && append base_cfg "sConfigMRCoexActiveTime=$sConfigMRCoexActiveTime" "$N"
		[ -n "$sConfigMRCoexInactiveTime" ] && append base_cfg "sConfigMRCoexInactiveTime=$sConfigMRCoexInactiveTime" "$N"
		[ -n "$sConfigMRCoexCts2SelfActive" ] && append base_cfg "sConfigMRCoexCts2SelfActive=$sConfigMRCoexCts2SelfActive" "$N"
	fi

	[ -n "$dfs_ch_state_file" ] && append base_cfg "dfs_channels_state_file_location=$dfs_ch_state_file" "$N"
	[ -n "$allow_scan_during_cac" ] && append base_cfg "allow_scan_during_cac=$allow_scan_during_cac" "$N"
	[ -n "$background_cac" ] && append base_cfg "background_cac=$background_cac" "$N"
	[ -n "$tx_mcs_set_defined" ] && append base_cfg "tx_mcs_set_defined=$tx_mcs_set_defined" "$N"

	[ -n "$ignore_supported_channels_errors" ] && append base_cfg "ignore_supported_channels_errors=$ignore_supported_channels_errors" "$N"

	[ "$testbed_mode" -gt 0 ] && append base_cfg "testbed_mode=1" "$N"

	if [ "$hwmode" = "g" ] && [ -n "$ieee80211n" ]; then
		[ -n "$sQAMplus" ] && append base_cfg "sQAMplus=$sQAMplus" "$N"
	fi

	[ -n "$country" ] && append base_cfg "country_code=$country" "$N"
	[ -n "$country3" ] && append base_cfg "country3=$country3" "$N"

	[ "$country_ie" -gt 0 ] && {
		append base_cfg "ieee80211d=1" "$N"
		[ -n "$local_pwr_constraint" ] && append base_cfg "local_pwr_constraint=$local_pwr_constraint" "$N"
		[ "$spectrum_mgmt_required" -gt 0 ] && append base_cfg "spectrum_mgmt_required=$spectrum_mgmt_required" "$N"
	}
	[ "$band" = "5GHz" -a "$doth" -gt 0 ] && {
		append base_cfg "ieee80211h=1" "$N"
		[ -n "$sub_band_dfs" ] && append base_cfg "sub_band_dfs=$sub_band_dfs" "$N"
		[ -n "$sRadarRssiTh" ] && append base_cfg "sRadarRssiTh=$sRadarRssiTh" "$N"
		[ -n "$dfs_unii4" ] && append base_cfg "dfs_unii4=$dfs_unii4" "$N"
	}

	[ -n "$acs_chan_bias" ] && append base_cfg "acs_chan_bias=$acs_chan_bias" "$N"

	local brlist= br
	json_get_values basic_rate_list basic_rate
	local rlist= r
	json_get_values rate_list supported_rates

	[ -n "$hwmode" ] && append base_cfg "hw_mode=$hwmode" "$N"
	if [ "$hwmode" = "g" ] || [ "$hwmode" = "a" ]; then
		case "$require_mode" in
			n) append base_cfg "require_ht=1" "$N";;
			ac) append base_cfg "require_vht=1" "$N";;
			ax) append base_cfg "require_he=1" "$N";;
		esac
	fi
	case "$hwmode" in
		b)
			if [ "$cell_density" -eq 1 ]; then
				set_default rate_list "5500 11000"
				set_default basic_rate_list "5500 11000"
			elif [ "$cell_density" -ge 2 ]; then
				set_default rate_list "11000"
				set_default basic_rate_list "11000"
			fi
		;;
		g)
			# disable legacy rates if "11b" is not configured
			# overwrite rate lists
			json_get_var cfg_hwmode hwmode
			if [ "$legacy_rates" -ne 0 ]; then
				case "$cfg_hwmode" in
					11b*)			;; # keep enabled
					*)	legacy_rates=0	;; # disable
				esac
			fi
			if [ "$cell_density" -eq 0 ] || [ "$cell_density" -eq 1 ]; then
				if [ "$legacy_rates" -eq 0 ]; then
					rate_list="6000 9000 12000 18000 24000 36000 48000 54000"
					basic_rate_list="6000 12000 24000"
				elif [ "$cell_density" -eq 1 ]; then
					rate_list="5500 6000 9000 11000 12000 18000 24000 36000 48000 54000"
					basic_rate_list="5500 11000"
				fi
			elif [ "$cell_density" -ge 3 ] && [ "$legacy_rates" -ne 0 ] || [ "$cell_density" -eq 2 ]; then
				if [ "$legacy_rates" -eq 0 ]; then
					rate_list="12000 18000 24000 36000 48000 54000"
					basic_rate_list="12000 24000"
				else
					rate_list="11000 12000 18000 24000 36000 48000 54000"
					basic_rate_list="11000"
				fi
			elif [ "$cell_density" -ge 3 ]; then
				rate_list="24000 36000 48000 54000"
				basic_rate_list="24000"
			fi
		;;
		a)
			if [ "$cell_density" -eq 1 ]; then
				set_default rate_list "6000 9000 12000 18000 24000 36000 48000 54000"
				set_default basic_rate_list "6000 12000 24000"
			elif [ "$cell_density" -eq 2 ]; then
				set_default rate_list "12000 18000 24000 36000 48000 54000"
				set_default basic_rate_list "12000 24000"
			elif [ "$cell_density" -ge 3 ]; then
				set_default rate_list "24000 36000 48000 54000"
				set_default basic_rate_list "24000"
			fi
		;;
	esac

	for r in $rate_list; do
		hostapd_add_rate rlist "$r"
	done

	for br in $basic_rate_list; do
		hostapd_add_rate brlist "$br"
	done

	[ -n "$rlist" ] && append base_cfg "supported_rates=$rlist" "$N"
	[ -n "$brlist" ] && append base_cfg "basic_rates=$brlist" "$N"
	append base_cfg "beacon_int=$beacon_int" "$N"
	#Commented to sync with 8.x scripts
	#append base_cfg "dtim_period=$dtim_period" "$N"
	[ "$airtime_mode" -gt 0 ] && append base_cfg "airtime_mode=$airtime_mode" "$N"

	json_get_values opts hostapd_options
	for val in $opts; do
		append base_cfg "$val" "$N"
	done

	[ -n "$dfs_debug_chan" ] && append base_cfg "dfs_debug_chan=$dfs_debug_chan" "$N"

	[ "$externally_managed" = "1" ] && append base_cfg "acs_scan_mode=1" "$N"
	[ -n "$he_beacon" ] && append base_cfg "he_beacon=$he_beacon" "$N"

	[ -n "$adv_proto_query_resp_len" ] && append base_cfg "adv_proto_query_resp_len=$adv_proto_query_resp_len" "$N"
	[ -n "$rssi_reject_assoc_rssi" ] && append base_cfg "rssi_reject_assoc_rssi=$rssi_reject_assoc_rssi" "$N"
	[ -n "$process_radar_slave_events" ] && append base_cfg "process_radar_slave_events=$process_radar_slave_events" "$N"
	[ -n "$start_after" ] && append base_cfg "start_after=$start_after" "$N"
	[ -n "$start_after_delay" ] && append base_cfg "start_after_delay=$start_after_delay" "$N"
	[ -n "$start_after_watchdog_time" ] && append base_cfg "start_after_watchdog_time=$start_after_watchdog_time" "$N"
	cat > "$config" <<EOF
driver=$driver
$base_cfg
EOF
}

max_num_psk_file_entries_idx=29 # 30 entries
config_add_wpa_psk_config() {
	for i in $(seq 0 $max_num_psk_file_entries_idx); do
		config_add_array wpa_psk_key_$i
	done
}

hostapd_common_add_bss_config() {
	config_add_string colocated_6g_radio_info
	config_add_array 'colocated_6g_vap_info:list(int,macaddr,string,boolean,boolean,boolean,int,int)'
	config_add_string 'bssid:macaddr' 'ssid:string'
	config_add_boolean wmm uapsd hidden utf8_ssid

	config_add_int maxassoc max_inactivity sae_pwe
	config_add_boolean disassoc_low_ack isolate short_preamble enable_hairpin
	config_add_string sRTSmode

	config_add_int sBridgeMode

	config_add_string sAddPeerAP

	config_add_int \
		wep_rekey eap_reauth_period \
		wpa_group_rekey wpa_pair_rekey wpa_master_rekey
	config_add_boolean wpa_strict_rekey
	config_add_boolean wpa_disable_eapol_key_retries

	config_add_boolean tdls_prohibit

	config_add_boolean rsn_preauth auth_cache
	config_add_int ieee80211w
	config_add_boolean disable_pbac
	config_add_boolean beacon_protection_enabled
	config_add_string 'ap_mld_mac:macaddr'
	config_add_boolean disable_bigtk_rekey
	config_add_string group_mgmt_cipher
	config_add_int eapol_version

	config_add_string 'auth_server:host' 'server:host'
	config_add_string auth_secret key
	config_add_int 'auth_port:port' 'port:port'

	config_add_string sec_auth_server
	config_add_string sec_auth_secret
	config_add_int sec_auth_port

	config_add_string acct_server
	config_add_string acct_secret
	config_add_int acct_port
	config_add_int acct_interval

	config_add_int bss_load_update_period chan_util_avg_period
	config_add_int acct_interim_interval

	config_add_string sec_acct_server
	config_add_string sec_acct_secret
	config_add_int sec_acct_port

	config_add_int eap_aaa_req_retries eap_aaa_req_timeout pmksa_life_time pmksa_interval \
		max_eap_failure auth_fail_blacklist_duration eap_req_id_retry_interval \
		failed_authentication_quiet_period

	config_add_string dae_client
	config_add_string dae_secret
	config_add_int dae_port

	config_add_string nasid
	config_add_string ownip
	config_add_string radius_client_addr
	config_add_string iapp_interface
	config_add_string eap_type ca_cert client_cert identity anonymous_identity auth priv_key priv_key_pwd
	config_add_boolean ca_cert_usesystem ca_cert2_usesystem
	config_add_string subject_match subject_match2
	config_add_array altsubject_match altsubject_match2
	config_add_array domain_match domain_match2 domain_suffix_match domain_suffix_match2
	config_add_string ieee80211w_mgmt_cipher

	config_add_int dynamic_vlan vlan_naming
	config_add_string vlan_tagged_interface vlan_bridge
	config_add_string vlan_file

	config_add_string 'key1:wepkey' 'key2:wepkey' 'key3:wepkey' 'key4:wepkey' 'password:wpakey'

	config_add_string wpa_psk_file
	config_add_wpa_psk_config

	config_add_string sae_key

	config_add_boolean wps_pushbutton wps_keypad wps_label ext_registrar wps_pbc_in_m1
	config_add_boolean wps_virtual_pushbutton wps_physical_pushbutton wps_virtual_display wps_physical_display
	config_add_int wps_ap_setup_locked wps_independent wps_state
	config_add_int wps_cred_processing wps_cred_add_sae
	config_add_string wps_device_type wps_device_name wps_manufacturer wps_pin
	config_add_string wps_uuid wps_pin_requests wps_os_version wps_rf_bands
	config_add_string wps_manufacturer_url wps_model_description upnp_bridge
	config_add_string wps_model_number wps_serial_number wps_skip_cred_build
	config_add_string wps_extra_cred wps_ap_settings wps_friendly_name
	config_add_string wps_model_url wps_upc wps_model_name

	config_add_boolean wnm_sleep_mode wnm_sleep_mode_no_keys bss_transition
	config_add_int time_advertisement
	config_add_string time_zone

	config_add_boolean ieee80211k rrm_neighbor_report rrm_beacon_report

	config_add_boolean ftm_responder stationary_ap
	config_add_string lci civic

	config_add_boolean ieee80211r pmk_r1_push ft_psk_generate_local ft_over_ds
	config_add_int r0_key_lifetime reassociation_deadline
	config_add_string mobility_domain r1_key_holder
	config_add_array r0kh r1kh

	config_add_int ieee80211w_max_timeout ieee80211w_retry_timeout

	config_add_string macfilter 'macfile:file'
	config_add_array 'maclist:list(macaddr)'

	config_add_array bssid_blacklist
	config_add_array bssid_whitelist

	config_add_int mcast_rate
	config_add_array basic_rate
	config_add_array supported_rates

	config_add_boolean sae_require_mfp

	config_add_string 'owe_transition_bssid:macaddr' 'owe_transition_ssid:string'

	config_add_boolean hs20 disable_dgaf osen offload_gas
	config_add_int anqp_domain_id
	config_add_int hs20_deauth_req_timeout
	config_add_array hs20_oper_friendly_name
	config_add_array osu_provider
	config_add_array operator_icon
	config_add_array hs20_conn_capab
	config_add_string osu_ssid hs20_wan_metrics hs20_operating_class hs20_t_c_filename hs20_t_c_timestamp
	config_add_int hs20_release
	config_add_boolean enable_bss_load_ie manage_p2p allow_cross_connection tdls_prohibit

	config_add_string dpp_controller dpp_connector dpp_csign dpp_netaccesskey \
		dpp_netaccesskey_expired dpp_resp_wait_time dpp_gas_query_timeout_period

	config_add_int num_res_sta
	config_add_int proxy_arp
	config_add_int mbo
	config_add_int mbo_cell_aware
	config_add_int rrm_neighbor_report
	config_add_int bss_transition
	config_add_int interworking
	config_add_int access_network_type
	config_add_int gas_comeback_delay
	config_add_string authresp_elements
	config_add_string vendor_elements
	config_add_string assocresp_elements
	config_add_int multi_ap
	config_add_int multi_ap_profile1_disallow
	config_add_int multi_ap_profile2_disallow
	config_add_string multi_ap_backhaul_ssid multi_ap_backhaul_key
	config_add_int multi_ap_profile
	config_add_int multi_ap_primary_vlanid
	config_add_string ul_csa
	config_add_string ctrl_interface_group
	config_add_string qos_map_set
	config_add_int s11nProtection
	config_add_string sAggrConfig
	config_add_string wav_bridge
	config_add_boolean vendor_vht
	config_add_boolean internet_available
	config_add_boolean asra
	config_add_boolean esr
	config_add_boolean uesa
	config_add_boolean rnr_auto_update
	config_add_boolean ap_protected_keep_alive_required
	config_add_int venue_type
	config_add_int venue_group
	config_add_string hessid
	config_add_int management_frames_rate
	config_add_int bss_beacon_int
	config_add_int sFwrdUnkwnMcast
	config_add_int dynamic_multicast_mode
	config_add_int dynamic_multicast_rate
	config_add_int roaming_anqp_ois_count
	config_add_string roaming_consortium_0
	config_add_string roaming_consortium_1
	config_add_string roaming_consortium_2
	config_add_int mgmt_frame_power_control
	config_add_int transition_disable
	config_add_int unsol_bcast_probe_resp_interval
	config_add_int fils_discovery_max_interval


	config_add_boolean soft_block_acl_enable
	config_add_int soft_block_acl_wait_time \
		soft_block_acl_allow_time soft_block_acl_on_auth_req soft_block_acl_on_probe_req \
		event_cache_interval


	config_add_int \
		wmm_ac_bk_cwmin wmm_ac_bk_cwmax wmm_ac_bk_aifs wmm_ac_bk_txop_limit wmm_ac_bk_acm \
		wmm_ac_be_cwmin wmm_ac_be_cwmax wmm_ac_be_aifs wmm_ac_be_txop_limit wmm_ac_be_acm \
		wmm_ac_vi_cwmin wmm_ac_vi_cwmax wmm_ac_vi_aifs wmm_ac_vi_txop_limit wmm_ac_vi_acm \
		wmm_ac_vo_cwmin wmm_ac_vo_cwmax wmm_ac_vo_aifs wmm_ac_vo_txop_limit wmm_ac_vo_acm \
		tx_queue_data0_cwmin tx_queue_data0_cwmax tx_queue_data0_aifs \
		tx_queue_data1_cwmin tx_queue_data1_cwmax tx_queue_data1_aifs \
		tx_queue_data2_cwmin tx_queue_data2_cwmax tx_queue_data2_aifs \
		tx_queue_data3_cwmin tx_queue_data3_cwmax tx_queue_data3_aifs

	config_add_boolean intra_vap_mcast
	config_add_int pmf
	config_add_string secure_store_hash greylist_logpath greylist_file 'cmmac:macaddr'
	config_add_boolean greylist

	if [ -f /lib/netifd/debug_infrastructure.sh ]; then
		debug_infrastructure_config_add_string debug_hostap_conf_
	fi

	config_add_string \
		tx_queue_data0_burst tx_queue_data1_burst tx_queue_data2_burst tx_queue_data3_burst

	config_add_boolean sae_require_mfp

	config_add_string 'owe_transition_bssid:macaddr' 'owe_transition_ssid:string'

	config_add_boolean iw_enabled iw_internet iw_asra iw_esr iw_uesa
	config_add_int iw_access_network_type iw_venue_group iw_venue_type
	config_add_int iw_ipaddr_type_availability iw_gas_address3
	config_add_string iw_hessid iw_network_auth_type iw_qos_map_set
	config_add_array iw_roaming_consortium iw_domain_name iw_anqp_3gpp_cell_net iw_nai_realm
	config_add_array iw_anqp_elem

	config_add_boolean hs20 disable_dgaf osen
	config_add_int anqp_domain_id
	config_add_int hs20_deauth_req_timeout
	config_add_array hs20_oper_friendly_name
	config_add_array osu_provider
	config_add_array operator_icon
	config_add_array hs20_conn_capab
	config_add_string osu_ssid hs20_wan_metrics hs20_operating_class hs20_t_c_filename hs20_t_c_timestamp

	config_add_array airtime_sta_weight
	config_add_int airtime_bss_weight airtime_bss_limit

	config_add_boolean multicast_to_unicast per_sta_vif

	config_add_array hostapd_bss_options
}

hostapd_set_vlan_file() {
	local ifname="$1"
	local vlan="$2"
	json_get_vars name vid
	echo "${vid} ${ifname}-${name}" >> /var/run/hostapd-${ifname}.vlan
	wireless_add_vlan "${vlan}" "${ifname}-${name}"
}

hostapd_set_vlan() {
	local ifname="$1"

	rm -f /var/run/hostapd-${ifname}.vlan
	for_each_vlan hostapd_set_vlan_file ${ifname}
}

hostapd_set_psk_file() {
	local ifname="$1"
	local vlan="$2"
	local vlan_id=""

	json_get_vars mac vid key
	set_default mac "00:00:00:00:00:00"
	[ -n "$vid" ] && vlan_id="vlanid=$vid "
	echo "${vlan_id} ${mac} ${key}" >> /var/run/hostapd-${ifname}.psk
}

hostapd_set_psk() {
	local ifname="$1"

	rm -f /var/run/hostapd-${ifname}.psk
	for_each_station hostapd_set_psk_file ${ifname}
}

append_iw_roaming_consortium() {
	[ -n "$1" ] && append bss_conf "roaming_consortium=$1" "$N"
}

append_iw_domain_name() {
	if [ -z "$iw_domain_name_conf" ]; then
		iw_domain_name_conf="$1"
	else
		iw_domain_name_conf="$iw_domain_name_conf,$1"
	fi
}

append_iw_anqp_3gpp_cell_net() {
	if [ -z "$iw_anqp_3gpp_cell_net_conf" ]; then
		iw_anqp_3gpp_cell_net_conf="$1"
	else
		iw_anqp_3gpp_cell_net_conf="$iw_anqp_3gpp_cell_net_conf:$1"
	fi
}

append_iw_anqp_elem() {
	[ -n "$1" ] && append bss_conf "anqp_elem=$1" "$N"
}

append_iw_nai_realm() {
	[ -n "$1" ] && append bss_conf "nai_realm=$1" "$N"
}

append_hs20_oper_friendly_name() {
	append bss_conf "hs20_oper_friendly_name=$1" "$N"
}

append_osu_provider_service_desc() {
	append bss_conf "osu_service_desc=$1" "$N"
}

append_hs20_icon() {
	local width height lang type path
	config_get width "$1" width
	config_get height "$1" height
	config_get lang "$1" lang
	config_get type "$1" type
	config_get path "$1" path

	append bss_conf "hs20_icon=$width:$height:$lang:$type:$1:$path" "$N"
}

append_hs20_icons() {
	config_load wireless
	config_foreach append_hs20_icon hs20-icon
}

append_operator_icon() {
	append bss_conf "operator_icon=$1" "$N"
}

append_osu_icon() {
	append bss_conf "osu_icon=$1" "$N"
}

append_osu_provider() {
	local cfgtype osu_server_uri osu_friendly_name osu_nai osu_nai2 osu_method_list

	config_load wireless
	config_get cfgtype "$1" TYPE
	[ "$cfgtype" != "osu-provider" ] && return

	append bss_conf "# provider $1" "$N"
	config_get osu_server_uri "$1" osu_server_uri
	config_get osu_nai "$1" osu_nai
	config_get osu_nai2 "$1" osu_nai2
	config_get osu_method_list "$1" osu_method

	append bss_conf "osu_server_uri=$osu_server_uri" "$N"
	append bss_conf "osu_nai=$osu_nai" "$N"
	append bss_conf "osu_nai2=$osu_nai2" "$N"
	append bss_conf "osu_method_list=$osu_method_list" "$N"

	config_list_foreach "$1" osu_service_desc append_osu_provider_service_desc
	config_list_foreach "$1" osu_icon append_osu_icon

	append bss_conf "$N"
}

append_hs20_conn_capab() {
	[ -n "$1" ] && append bss_conf "hs20_conn_capab=$1" "$N"
}

append_airtime_sta_weight() {
	[ -n "$1" ] && append bss_conf "airtime_sta_weight=$1" "$N"
}
# List of arrays, each array contains 2 string entries: key_id and key
write_wpa_psk_file() {
	local ifname="$1"
	local psk_file="$2"
	local psk_conf=

	for i in $(seq 0 $max_num_psk_file_entries_idx); do
		j=0
		json_get_values wpa_psk_key_entry "wpa_psk_key_$i"

		if [ -z "$wpa_psk_key_entry" ]; then
			break
		fi

		for str in $wpa_psk_key_entry; do
			if [ $j -eq 0 ]; then
				append psk_conf "keyid=$str 00:00:00:00:00:00" "$N"
			elif [ $j -eq 1 ]; then
				append psk_conf "$str"
			else
				logger -s "WPA PSK: Unsupported string $str in array at index $j"
			fi
			j=$((j+1))
		done
	done

	if [ -n "$psk_conf" ]; then
		if [ -z "$psk_file" ]; then
			psk_file="/var/run/hostapd-$ifname.wpa_psk"
			append bss_conf "wpa_psk_file=$psk_file" "$N"
		fi

		cat > "$psk_file" <<EOF
$psk_conf
EOF
	fi
}

hostapd_setup_ap_mld_mac(){
	local mlo_id="$1"
	local bssid="$2"
	ap_mld_mac=$3
	local tmp_mld_mac tmp_mld first_link
	local mld_name mld_mac MACAPLIST
	local mld_mac_list="/tmp/.mld_mac_list"

	is_first_link="1"
	MLDAPLIST="`cat $mld_mac_list`"

	# Check for MLD name is in the MLD AP LIST
	for tmp_mld in $MLDAPLIST; do
		mld_name=$(echo $tmp_mld | cut -f1 -d-)
		mld_mac=$(echo $tmp_mld | cut -f2 -d-)
		if [ $mlo_id = $mld_name ]; then
			is_first_link="0"
			tmp_mld_mac=$mld_mac
		fi
	done
	#Add the Entry
	if [ $is_first_link = "1" ]; then
		echo "$MLDAPLIST $mlo_id-$bssid">$mld_mac_list
		tmp_mld_mac=$bssid
	fi
	ap_mld_mac=$tmp_mld_mac
}

hostapd_set_bss_options() {
	local var="$1"
	local phy="$2"
	local vif="$3"
	local macaddr="$4"
	local type="$5"

	wireless_vif_parse_encryption

	local bss_conf bss_md5sum mlo_id ap_mld_mac mlo_enable
	local wep_rekey wpa_group_rekey wpa_pair_rekey wpa_master_rekey wpa_key_mgmt
	local legacy_vendor_elements="dd050009860100"

	json_get_vars \
		wep_rekey wpa_group_rekey wpa_pair_rekey wpa_master_rekey wpa_strict_rekey \
		wpa_disable_eapol_key_retries tdls_prohibit \
		maxassoc max_inactivity disassoc_low_ack isolate auth_cache greylist \
		multi_ap multi_ap_profile1_disallow multi_ap_profile2_disallow \
		multi_ap_profile multi_ap_primary_vlanid \
		multi_ap_backhaul_ssid multi_ap_backhaul_key \
		wps_pushbutton wps_keypad wps_label ext_registrar wps_pbc_in_m1 wps_ap_setup_locked \
		wps_virtual_pushbutton wps_physical_pushbutton wps_virtual_display wps_physical_display \
		wps_independent wps_device_type wps_device_name wps_manufacturer wps_pin \
		macfilter ssid utf8_ssid wmm uapsd hidden short_preamble rsn_preauth \
		iapp_interface eapol_version dynamic_vlan ieee80211w sec_acct_server \
		sec_acct_secret sec_acct_port acct_interim_interval wps_state \
		wps_rf_bands wps_uuid qos_map_set nasid \
		acct_server acct_secret acct_port acct_interval \
		bss_load_update_period chan_util_avg_period sae_require_mfp \
		airtime_bss_weight airtime_bss_limit airtime_sta_weight \
		multicast_to_unicast per_sta_vif \
		mbo mbo_cell_aware rrm_neighbor_report num_res_sta ctrl_interface_group proxy_arp \
		wmm_ac_bk_cwmin wmm_ac_bk_cwmax wmm_ac_bk_aifs wmm_ac_bk_txop_limit wmm_ac_bk_acm \
		wmm_ac_be_cwmin wmm_ac_be_cwmax wmm_ac_be_aifs wmm_ac_be_txop_limit wmm_ac_be_acm \
		wmm_ac_vi_cwmin wmm_ac_vi_cwmax wmm_ac_vi_aifs wmm_ac_vi_txop_limit wmm_ac_vi_acm \
		wmm_ac_vo_cwmin wmm_ac_vo_cwmax wmm_ac_vo_aifs wmm_ac_vo_txop_limit wmm_ac_vo_acm \
		tx_queue_data0_cwmin tx_queue_data0_cwmax tx_queue_data0_aifs tx_queue_data0_burst \
		tx_queue_data1_cwmin tx_queue_data1_cwmax tx_queue_data1_aifs tx_queue_data1_burst \
		tx_queue_data2_cwmin tx_queue_data2_cwmax tx_queue_data2_aifs tx_queue_data2_burst \
		tx_queue_data3_cwmin tx_queue_data3_cwmax tx_queue_data3_aifs tx_queue_data3_burst \
		authresp_elements vendor_elements assocresp_elements gas_comeback_delay \
		wps_pin_requests wps_os_version wps_cred_processing wps_cred_add_sae wps_manufacturer_url \
		wps_model_description interworking access_network_type bss_transition \
		s11nProtection sAggrConfig wav_bridge upnp_bridge \
		wps_model_number wps_serial_number wps_skip_cred_build wps_extra_cred \
		wps_ap_settings wps_friendly_name wps_model_url wps_upc wps_model_name \
		vendor_vht internet_available asra esr uesa \
		venue_type venue_group hessid sae_require_mfp management_frames_rate \
		sBridgeMode sAddPeerAP bss_beacon_int sFwrdUnkwnMcast \
		dynamic_multicast_mode dynamic_multicast_rate sae_pwe \
		beacon_protection_enabled disable_pbac disable_bigtk_rekey group_mgmt_cipher colocated_6g_radio_info \
		soft_block_acl_enable soft_block_acl_wait_time soft_block_acl_allow_time \
		soft_block_acl_on_auth_req soft_block_acl_on_probe_req rnr_auto_update ul_csa \
		roaming_consortium_0 roaming_consortium_1 roaming_consortium_2 \
		roaming_anqp_ois_count ap_protected_keep_alive_required \
		intra_vap_mcast enable_hairpin mgmt_frame_power_control event_cache_interval \
		greylist_logpath greylist_file cmmac transition_disable sRTSmode \
		unsol_bcast_probe_resp_interval fils_discovery_max_interval dtim_period max_listen_int

	if [ -f /lib/netifd/debug_infrastructure.sh ]; then
		debug_infrastructure_json_get_vars debug_hostap_conf_
	fi

	set_default isolate 0
	set_default max_inactivity 0
	set_default proxy_arp 0
	set_default num_res_sta 0
	set_default gas_comeback_delay 0
	set_default short_preamble 1
	set_default disassoc_low_ack 0
	set_default hidden 0
	set_default wmm 1
	set_default uapsd 1
	set_default wpa_disable_eapol_key_retries 0
	set_default tdls_prohibit 0
	set_default eapol_version $((wpa & 1))
	set_default acct_port 1813
	set_default bss_load_update_period 60
	set_default chan_util_avg_period 600
	set_default utf8_ssid 1
	set_default airtime_bss_weight 0
	set_default airtime_bss_limit 0
	set_default sec_acct_port 1813
	set_default mbo 1
	set_default rrm_neighbor_report 0
	set_default bss_transition 1
	set_default interworking 1
	set_default access_network_type 0
	set_default vendor_vht 1
	set_default enable_hairpin 0
	set_default greylist 0
	set_default multi_ap 0
	set_default disable_pbac 1

	append bss_conf "ctrl_interface=/var/run/hostapd" "$N"
	[ -n "$ctrl_interface_group" ] && {
		append bss_conf "ctrl_interface_group=$ctrl_interface_group" "$N"
	}

	# Multi-AP settings
	[ "$multi_ap" -gt 0 ] && {
		local  multi_ap_client_disallow=0
		append bss_conf "multi_ap=$multi_ap" "$N"
		[ -n "$multi_ap_profile" ] && append bss_conf "multi_ap_profile=$multi_ap_profile" "$N"
		[ -n "$multi_ap_primary_vlanid" ] && append bss_conf "multi_ap_vlanid=$multi_ap_primary_vlanid" "$N"
		# backhaul or hybrid mode
		[ "$multi_ap" = 1 ] || [ "$multi_ap" = 3 ] && {
			[ -n "$multi_ap_profile1_disallow" ] && ((multi_ap_client_disallow |= 1))
			[ -n "$multi_ap_profile2_disallow" ] && ((multi_ap_client_disallow |= 2))
			append bss_conf "multi_ap_client_disallow=$multi_ap_client_disallow" "$N"
		}
	}
	[ -n "$ul_csa" ] && append bss_conf "ul_csa=$ul_csa" "$N"

	[ -n "$enable_hairpin" ] && append bss_conf "enable_hairpin=$enable_hairpin" "$N"
	if [ "$isolate" -gt 0 ]; then
		append bss_conf "ap_isolate=$isolate" "$N"
	fi
	if [ "$greylist" -eq 1 ]; then
		append bss_conf "greylist=1" "$N"
		append bss_conf "ap_vlan=0" "$N"
	else
		append bss_conf "greylist=0" "$N"
	fi
	[ -n "$cmmac" ] && append bss_conf "cmmac=$cmmac" "$N"
	[ -n "$greylist_logpath" ] && append bss_conf "greylist_logpath=$greylist_logpath" "$N"
	[ -n "$greylist_file" ] && append bss_conf "greylist_file=$greylist_file" "$N"

	[ -n "$maxassoc" ] && append bss_conf "max_num_sta=$maxassoc" "$N"

	[ -n "$sBridgeMode" ] && append bss_conf "sBridgeMode=$sBridgeMode" "$N"

	[ -n "$sAddPeerAP" ] && append bss_conf "sAddPeerAP=$sAddPeerAP" "$N"

	if [ "$max_inactivity" -gt 0 ]; then
		append bss_conf "ap_max_inactivity=$max_inactivity" "$N"
	fi
	if [ "$proxy_arp" -gt 0 ]; then
		append bss_conf "proxy_arp=$proxy_arp" "$N"
	fi
	if [ "$num_res_sta" -gt 0 ]; then
		append bss_conf "num_res_sta=$num_res_sta" "$N"
	fi
	if [ "$gas_comeback_delay" -gt "0" ]; then
		append bss_conf "gas_comeback_delay=$gas_comeback_delay" "$N"
	fi
	if [ "$rrm_neighbor_report" -gt "0" ]; then
		append bss_conf "rrm_neighbor_report=$rrm_neighbor_report" "$N"
	fi
	if [ "$bss_transition" -gt "0" ]; then
		append bss_conf "bss_transition=$bss_transition" "$N"
	fi

	[ -n "$bss_beacon_int" ] && append bss_conf "bss_beacon_int=$bss_beacon_int" "$N"
	[ -n "$sFwrdUnkwnMcast" ] && append bss_conf "sFwrdUnkwnMcast=$sFwrdUnkwnMcast" "$N"
	[ -n "$dynamic_multicast_mode" ] && append bss_conf "dynamic_multicast_mode=$dynamic_multicast_mode" "$N"
	[ -n "$dynamic_multicast_rate" ] && append bss_conf "dynamic_multicast_rate=$dynamic_multicast_rate" "$N"
	[ -n "$rnr_auto_update" ] && append bss_conf "rnr_auto_update=$rnr_auto_update" "$N"
	[ -n "$ap_protected_keep_alive_required" ] && append bss_conf "ap_protected_keep_alive_required=$ap_protected_keep_alive_required" "$N"
	[ -n "$intra_vap_mcast" ] && append bss_conf "intra_vap_mcast=$intra_vap_mcast" "$N"
	[ -n "$unsol_bcast_probe_resp_interval" ] && append bss_conf "unsol_bcast_probe_resp_interval=$unsol_bcast_probe_resp_interval" "$N"
	[ -n "$fils_discovery_max_interval" ] && append bss_conf "fils_discovery_max_interval=$fils_discovery_max_interval" "$N"

	if [ "$interworking" -gt 0 ]; then
		append bss_conf "interworking=$interworking" "$N"
		
		if [ "$access_network_type" -gt 0 ]; then
			append bss_conf "access_network_type=$access_network_type" "$N"
		fi

		if [ -n "$internet_available" ]; then
			append bss_conf "internet=$internet_available" "$N"
		fi
		if [ -n "$asra" ]; then
			append bss_conf "asra=$asra" "$N"
		fi
		if [ -n "$esr" ]; then
			append bss_conf "esr=$esr" "$N"
		fi
		if [ -n "$uesa" ]; then
			append bss_conf "uesa=$uesa" "$N"
		fi
		if [ -n "$venue_type" ]; then
			append bss_conf "venue_type=$venue_type" "$N"
		fi
		if [ -n "$venue_group" ]; then
			append bss_conf "venue_group=$venue_group" "$N"
		fi
		if [ -n "$hessid" ]; then
			append bss_conf "hessid=$hessid" "$N"
		fi
		if [ -n "$roaming_anqp_ois_count" ]; then
			append bss_conf "roaming_anqp_ois_count=$roaming_anqp_ois_count" "$N"
		fi
		if [ -n "$roaming_consortium_0" ]; then
			append bss_conf "roaming_consortium=$roaming_consortium_0" "$N"
		fi
		if [ -n "$roaming_consortium_1" ]; then
			append bss_conf "roaming_consortium=$roaming_consortium_1" "$N"
		fi
		if [ -n "$roaming_consortium_2" ]; then
			append bss_conf "roaming_consortium=$roaming_consortium_2" "$N"
		fi
	fi

	[ "$airtime_bss_weight" -gt 0 ] && append bss_conf "airtime_bss_weight=$airtime_bss_weight" "$N"
	[ "$airtime_bss_limit" -gt 0 ] && append bss_conf "airtime_bss_limit=$airtime_bss_limit" "$N"
	json_for_each_item append_airtime_sta_weight airtime_sta_weight

	#Commented to sync with 8.x scripts
	#append bss_conf "bss_load_update_period=$bss_load_update_period" "$N"
	#append bss_conf "chan_util_avg_period=$chan_util_avg_period" "$N"
	[ -n "$management_frames_rate"  ] && append bss_conf "management_frames_rate=$management_frames_rate" "$N"

	append bss_conf "disassoc_low_ack=$disassoc_low_ack" "$N"
	if [[ "$hwmode" = "g" || "$hwmode" = "b" ]]; then
		append bss_conf "preamble=$short_preamble" "$N"
	fi
	append bss_conf "wmm_enabled=$wmm" "$N"
	append bss_conf "ignore_broadcast_ssid=$hidden" "$N"
	[ -n "$sRTSmode" ] && append bss_conf "sRTSmode=$sRTSmode" "$N"

	[ "$hidden" -gt 0 ] && {
		/usr/bin/logger -s "********* WPS warning! $ifname ignore_broadcast_ssid is set, WPS will be disabled  ***********"
	}
	append bss_conf "uapsd_advertisement_enabled=$uapsd" "$N"
	#Commented to sync with 8.x scripts
	#append bss_conf "utf8_ssid=$utf8_ssid" "$N"

	[ "$tdls_prohibit" -gt 0 ] && append bss_conf "tdls_prohibit=$tdls_prohibit" "$N"

	[ "$mbo" -gt 0 ] && {
		append bss_conf "mbo=$mbo" "$N"
		[ -n "$mbo_cell_aware" ] && append bss_conf "mbo_cell_aware=$mbo_cell_aware" "$N"
	}

	[ "$wmm" -gt 0 ] && {
		hostapd_append_wmm_params bss_conf
	}

	[ "$wpa" -gt 0 ] && {
		[ -n "$wpa_group_rekey"  ] && append bss_conf "wpa_group_rekey=$wpa_group_rekey" "$N"
		[ -n "$wpa_pair_rekey"   ] && append bss_conf "wpa_ptk_rekey=$wpa_pair_rekey"    "$N"
		[ -n "$wpa_master_rekey" ] && append bss_conf "wpa_gmk_rekey=$wpa_master_rekey"  "$N"
		[ -n "$wpa_strict_rekey" ] && append bss_conf "wpa_strict_rekey=$wpa_strict_rekey" "$N"
	}

	[ -n "$nasid" ] && append bss_conf "nas_identifier=$nasid" "$N"

	append bss_conf "vendor_elements=${legacy_vendor_elements}${vendor_elements}" "$N"
	[ -n "$authresp_elements" ] && append bss_conf "authresp_elements=$authresp_elements" "$N"
	[ -n "$assocresp_elements" ] && append bss_conf "assocresp_elements=$assocresp_elements" "$N"

	[ -n "$qos_map_set" ] && append bss_conf "qos_map_set=$qos_map_set" "$N"
	[ -n "$s11nProtection" ] && append bss_conf "s11nProtection=$s11nProtection" "$N"
	[ -n "$sAggrConfig" ] && append bss_conf "sAggrConfig=$sAggrConfig" "$N"

	[ -n "$mgmt_frame_power_control" ] && append bss_conf "mgmt_frame_power_control=$mgmt_frame_power_control" "$N"
	[ -n "$transition_disable" ] && append bss_conf "transition_disable=$transition_disable" "$N"

	# if soft_block_acl_enable isn't defined or set to false/0,
	# softblock parameters will not be written into .conf file
	[ -n "$soft_block_acl_enable" ] && {
		set_default soft_block_acl_enable 0
		append bss_conf "soft_block_acl_enable=$soft_block_acl_enable" "$N"

		# if soft_block_acl_enable defined and > 0, write parameters to conf
		[ "$soft_block_acl_enable" -gt 0 ] && {
			set_default soft_block_acl_wait_time 10000
			set_default soft_block_acl_allow_time 15000
			set_default soft_block_acl_on_auth_req 1
			set_default soft_block_acl_on_probe_req 1
			append bss_conf "soft_block_acl_wait_time=$soft_block_acl_wait_time" "$N"
			append bss_conf "soft_block_acl_allow_time=$soft_block_acl_allow_time" "$N"
			append bss_conf "soft_block_acl_on_auth_req=$soft_block_acl_on_auth_req" "$N"
			append bss_conf "soft_block_acl_on_probe_req=$soft_block_acl_on_probe_req" "$N"
		}
	}

	set_default event_cache_interval 1
	append bss_conf "event_cache_interval=$event_cache_interval" "$N"

	local hs20 disable_dgaf osen anqp_domain_id hs20_deauth_req_timeout \
		osu_ssid hs20_wan_metrics hs20_operating_class hs20_t_c_filename hs20_t_c_timestamp \
		hs20_release enable_bss_load_ie manage_p2p allow_cross_connection tdls_prohibit \
		offload_gas
	json_get_vars hs20 disable_dgaf osen anqp_domain_id hs20_deauth_req_timeout \
		osu_ssid hs20_wan_metrics hs20_operating_class hs20_t_c_filename hs20_t_c_timestamp \
		hs20_release enable_bss_load_ie manage_p2p allow_cross_connection tdls_prohibit \
		offload_gas

	set_default hs20 0
	set_default osen 0
	set_default anqp_domain_id 0
	set_default hs20_deauth_req_timeout 60
	set_default hs20_release 1
	if [ "$hs20" = "1" ]; then
		set_default offload_gas 1
		append bss_conf "hs20=1" "$N"
		append_hs20_icon
		append bss_conf "osen=$osen" "$N"
		append bss_conf "anqp_domain_id=$anqp_domain_id" "$N"
		append bss_conf "hs20_deauth_req_timeout=$hs20_deauth_req_timeout" "$N"
		append bss_conf "hs20_release=$hs20_release" "$N"
		[ -n "$disable_dgaf" ] && append bss_conf "disable_dgaf=$disable_dgaf" "$N"
		[ -n "$osu_ssid" ] && append bss_conf "osu_ssid=$osu_ssid" "$N"
		[ -n "$hs20_wan_metrics" ] && append bss_conf "hs20_wan_metrics=$hs20_wan_metrics" "$N"
		[ -n "$hs20_operating_class" ] && append bss_conf "hs20_operating_class=$hs20_operating_class" "$N"
		[ -n "$hs20_t_c_filename" ] && append bss_conf "hs20_t_c_filename=$hs20_t_c_filename" "$N"
		[ -n "$hs20_t_c_timestamp" ] && append bss_conf "hs20_t_c_timestamp=$hs20_t_c_timestamp" "$N"
		json_for_each_item append_hs20_conn_capab hs20_conn_capab
		json_for_each_item append_hs20_oper_friendly_name hs20_oper_friendly_name
		json_for_each_item append_osu_provider osu_provider
		json_for_each_item append_operator_icon operator_icon
	fi

	[ -n "$offload_gas" ] && append bss_conf "offload_gas=$offload_gas" "$N"
	[ -n "$enable_bss_load_ie" ] && append bss_conf "enable_bss_load_ie=$enable_bss_load_ie" "$N"
	[ -n "$tdls_prohibit" ] && append bss_conf "tdls_prohibit=$tdls_prohibit" "$N"
	[ -n "$manage_p2p" ] && {
		append bss_conf "manage_p2p=$manage_p2p" "$N"
		[ -n "$allow_cross_connection" ] && append bss_conf "allow_cross_connection=$allow_cross_connection" "$N"
	}

#	bss_md5sum=$(echo $bss_conf | md5sum | cut -d" " -f1)
#	append bss_conf "config_id=$bss_md5sum" "$N"

	if [ "$hwmode" = "g" ]; then
		[ -n "$ieee80211n" ] && append bss_conf "vendor_vht=$vendor_vht" "$N"
	fi

	[ -n "$acct_server" ] && {
		append bss_conf "acct_server_addr=$acct_server" "$N"
		append bss_conf "acct_server_port=$acct_port" "$N"
		[ -n "$acct_secret" ] && \
			append bss_conf "acct_server_shared_secret=$acct_secret" "$N"
		[ -n "$acct_interval" ] && \
			append bss_conf "radius_acct_interim_interval=$acct_interval" "$N"
	}

	case "$auth_type" in
		sae|owe|eap192|eap-eap192)
			set_default ieee80211w 2
			set_default sae_require_mfp 1
		;;
		psk-sae)
			set_default ieee80211w 1
			set_default sae_require_mfp 1
		;;
	esac
	[ -n "$sae_require_mfp" ] && append bss_conf "sae_require_mfp=$sae_require_mfp" "$N"
	[ -n "$sec_acct_server" ] && {
		append bss_conf "acct_server_addr=$sec_acct_server" "$N"
		append bss_conf "acct_server_port=$sec_acct_port" "$N"
		[ -n "$sec_acct_secret" ] && \
			append bss_conf "acct_server_shared_secret=$sec_acct_secret" "$N"
	}

	[ -n "$acct_server" -o -n "$sec_acct_server" ] && {
		[ -n "$acct_interim_interval" ] && \
		append bss_conf "radius_acct_interim_interval=$acct_interim_interval" "$N"
	}

	case "$auth_type" in
		sae|owe|eap192|eap-eap192)
			set_default ieee80211w 2
			set_default sae_require_mfp 1
		;;
		psk-sae)
			set_default ieee80211w 1
			set_default sae_require_mfp 1
		;;
	esac
	[ -n "$sae_require_mfp" ] && append bss_conf "sae_require_mfp=$sae_require_mfp" "$N"
	[ -n "$sae_pwe" ]  && append bss_conf "sae_pwe=$sae_pwe" "$N"

	# DPP configuration
	if [ "$dpp_auth_enabled" -eq 1 ]; then
		# For AP ieee80211w can be optional
		set_default ieee80211w 1
		set_default sae_require_mfp 1

		json_get_vars \
			dpp_controller dpp_connector dpp_csign dpp_netaccesskey

		set_default interworking 1
		[ -n "$dpp_controller" ] && append bss_conf "dpp_controller=$dpp_controller" "$N"
		[ -n "$dpp_connector" ] && append bss_conf "dpp_connector=$dpp_connector" "$N"
		[ -n "$dpp_csign" ] && append bss_conf "dpp_csign=$dpp_csign" "$N"
		[ -n "$dpp_netaccesskey" ] && append bss_conf "dpp_netaccesskey=$dpp_netaccesskey" "$N"
		if [ -n "$dpp_netaccesskey" ]; then
			json_get_vars dpp_netaccesskey_expiry
			[ -n "$dpp_netaccesskey_expiry" ] && append bss_conf "dpp_netaccesskey_expiry=$dpp_netaccesskey_expiry" "$N"
		fi

		# DPP timeouts
		json_get_vars dpp_resp_wait_time dpp_gas_query_timeout_period
		set_default dpp_resp_wait_time 5500
		set_default dpp_gas_query_timeout_period 10
		[ -n "$dpp_resp_wait_time" ] && append bss_conf "dpp_resp_wait_time=$dpp_resp_wait_time" "$N"
		[ -n "$dpp_gas_query_timeout_period" ] && append bss_conf "dpp_gas_query_timeout_period=$dpp_gas_query_timeout_period" "$N"
	fi

	local vlan_possible=""

	case "$auth_type" in
		none|owe)
			json_get_vars \
				owe_transition_bssid owe_transition_ssid \
				auth_server auth_secret auth_port \
				sec_auth_server sec_auth_secret sec_auth_port \
				dae_client dae_secret dae_port \
				ownip
			[ -n "$owe_transition_ssid" ] && append bss_conf "owe_transition_ssid=\"$owe_transition_ssid\"" "$N"
			[ -n "$owe_transition_bssid" ] && append bss_conf "owe_transition_bssid=$owe_transition_bssid" "$N"

			wps_possible=1
			# Here we make the assumption that if we're in open mode
			# with WPS enabled, we got to be in unconfigured state.
			wps_not_configured=1
			[ "$auth_type" = "none" ] &&
				/usr/bin/logger -s "********* WPS warning! $ifname security is not set, require explicit operation to create open network  ***********"
			if [ "$isolate" -gt 0 ]; then
				[ -n "$auth_server" -a -n "$auth_port" -a -n "$auth_secret" ] && {
					set_default auth_port 1812
					append bss_conf "auth_server_addr=$auth_server" "$N"
					append bss_conf "auth_server_port=$auth_port" "$N"
					append bss_conf "auth_server_shared_secret=$auth_secret" "$N"
				}

				[ -n "$sec_auth_server" -a -n "$sec_auth_port" -a -n "$sec_auth_secret" ] && {
					set_default sec_auth_port 1812
					append bss_conf "auth_server_addr=$sec_auth_server" "$N"
					append bss_conf "auth_server_port=$sec_auth_port" "$N"
					append bss_conf "auth_server_shared_secret=$sec_auth_secret" "$N"
				}

				[ -n "$dae_client" -a -n "$dae_port" -a -n "$dae_secret" ] && {
					set_default dae_port 3799
					append bss_conf "radius_das_port=$dae_port" "$N"
					append bss_conf "radius_das_client=$dae_client $dae_secret" "$N"
				}

				[ -n "$ownip" ] && append bss_conf "own_ip_addr=$ownip" "$N"
			fi
		;;
		psk)
			json_get_vars key wpa_psk_file
			if [ ${#key} -eq 64 ]; then
				append bss_conf "wpa_psk=$key" "$N"
			elif [ ${#key} -ge 8 ] && [ ${#key} -le 63 ]; then
				append bss_conf "wpa_passphrase=$key" "$N"
			elif [ -n "$key" ] || [ -z "$wpa_psk_file" ]; then
				if [ "$mem_only_cred" != "1" ]; then
					wireless_setup_vif_failed INVALID_WPA_PSK
					return 1
				fi
			fi
			#Commented to sync with 8.x scripts
			#[ -z "$wpa_psk_file" ] && set_default wpa_psk_file /var/run/hostapd-$ifname.psk
			[ -n "$wpa_psk_file" ] && {
				[ -e "$wpa_psk_file" ] || touch "$wpa_psk_file"
				append bss_conf "wpa_psk_file=$wpa_psk_file" "$N"
			}

			write_wpa_psk_file $ifname $wpa_psk_file

			[ "$eapol_version" -ge "1" -a "$eapol_version" -le "2" ] && append bss_conf "eapol_version=$eapol_version" "$N"

			#set_default dynamic_vlan 0
			#vlan_possible=1
			wps_possible=1
		;;
		psk-sae)
			json_get_vars key sae_key
			if [ "$mem_only_cred" != "1" ]; then
				if [[ ${#key} -lt 8 || ${#key} -gt 63 ]]; then
					wireless_setup_vif_failed INVALID_WPA_PSK
					return 1
				fi
			fi

			if [[ "$key" != "" && "$sae_key" != "" ]]; then
				append bss_conf "wpa_passphrase=$key" "$N"
				append bss_conf "sae_password=$sae_key" "$N"
			elif [ "$key" != "" ]; then
				append bss_conf "wpa_passphrase=$key" "$N"
				append bss_conf "sae_password=$key" "$N"
			else
				if [ "$mem_only_cred" != "1" ]; then
					wireless_setup_vif_failed INVALID_WPA_PSK
					return 1
				fi
			fi

			wps_possible=1
		;;
		sae|sae-ext-key)
			json_get_vars key sae_key
			if [ "$sae_key" != "" ]; then
				append bss_conf "sae_password=$sae_key" "$N"
			elif [ "$key" != "" ]; then
				append bss_conf "sae_password=$key" "$N"
			else
				if [ "$mem_only_cred" != "1" ]; then
					wireless_setup_vif_failed INVALID_WPA_PSK
					return 1
				fi
			fi
		;;
		eap|eap192|eap-eap192)
			json_get_vars \
				auth_server auth_secret auth_port \
				sec_auth_server sec_auth_secret sec_auth_port \
				dae_client dae_secret dae_port \
				ownip radius_client_addr \
				eap_reauth_period \
				venue_type venue_group hessid sae_require_mfp \
				eap_aaa_req_retries eap_aaa_req_timeout pmksa_life_time pmksa_interval \
				max_eap_failure auth_fail_blacklist_duration eap_req_id_retry_interval \
				failed_authentication_quiet_period

			# radius can provide VLAN ID for clients
			vlan_possible=1

			# legacy compatibility
			[ -n "$auth_server" ] || json_get_var auth_server server
			[ -n "$auth_port" ] || json_get_var auth_port port
			[ -n "$auth_secret" ] || json_get_var auth_secret key

			if [ -z "$auth_server" -o -z "$auth_secret" ]; then
				wireless_setup_vif_failed INVALID_RADIUS_CONFIG
				return 1
			fi

			set_default auth_port 1812
			set_default sec_auth_port 1812
			set_default dae_port 3799

			append bss_conf "auth_server_addr=$auth_server" "$N"
			append bss_conf "auth_server_port=$auth_port" "$N"
			append bss_conf "auth_server_shared_secret=$auth_secret" "$N"

			[ -n "$eap_aaa_req_retries" ] && append bss_conf "eap_aaa_req_retries=$eap_aaa_req_retries" "$N"
			[ -n "$eap_aaa_req_timeout" ] && append bss_conf "eap_aaa_req_timeout=$eap_aaa_req_timeout" "$N"
			[ -n "$pmksa_life_time" ] && append bss_conf "pmksa_life_time=$pmksa_life_time" "$N"
			[ -n "$pmksa_interval" ] && append bss_conf "pmksa_interval=$pmksa_interval" "$N"
			[ -n "$max_eap_failure" ] && append bss_conf "max_eap_failure=$max_eap_failure" "$N"
			[ -n "$auth_fail_blacklist_duration" ] && append bss_conf "auth_fail_blacklist_duration=$auth_fail_blacklist_duration" "$N"
			[ -n "$eap_req_id_retry_interval" ] && append bss_conf "eap_req_id_retry_interval=$eap_req_id_retry_interval" "$N"
			[ -n "$failed_authentication_quiet_period" ] && append bss_conf "failed_authentication_quiet_period=$failed_authentication_quiet_period" "$N"

			[ -n "$sec_auth_server" ] && {
				append bss_conf "auth_server_addr=$sec_auth_server" "$N"
				append bss_conf "auth_server_port=$sec_auth_port" "$N"
				append bss_conf "auth_server_shared_secret=$sec_auth_secret" "$N"
			}

			[ -n "$eap_reauth_period" ] && append bss_conf "eap_reauth_period=$eap_reauth_period" "$N"

			[ -n "$dae_client" -a -n "$dae_secret" ] && {
				append bss_conf "radius_das_port=$dae_port" "$N"
				append bss_conf "radius_das_client=$dae_client $dae_secret" "$N"
			}

			[ -n "$ownip" ] && append bss_conf "own_ip_addr=$ownip" "$N"
			[ -n "$radius_client_addr" ] && append bss_conf "radius_client_addr=$radius_client_addr" "$N"
			append bss_conf "eapol_key_index_workaround=1" "$N"
			append bss_conf "ieee8021x=1" "$N"

			[ "$eapol_version" -ge "1" -a "$eapol_version" -le "2" ] && append bss_conf "eapol_version=$eapol_version" "$N"
		;;
		wep)
			local wep_keyidx=0
			json_get_vars key
			hostapd_append_wep_key bss_conf
			append bss_conf "wep_default_key=$wep_keyidx" "$N"
			[ -n "$wep_rekey" ] && append bss_conf "wep_rekey_period=$wep_rekey" "$N"
			/usr/bin/logger -s "********* WPS warning! $ifname wep security is set, WPS will be disabled ***********"
		;;
	esac

	local auth_algs=$((($auth_mode_shared << 1) | $auth_mode_open))
	append bss_conf "auth_algs=${auth_algs:-1}" "$N"
	append bss_conf "wpa=$wpa" "$N"
	[ -n "$wpa_pairwise" ] && append bss_conf "wpa_pairwise=$wpa_pairwise" "$N"
	[ -n "$rsn_pairwise" ] && append bss_conf "rsn_pairwise=$rsn_pairwise" "$N"

	set_default wps_pushbutton 0
	set_default wps_keypad 0
	set_default wps_label 0
	set_default wps_pbc_in_m1 0
	set_default wps_virtual_pushbutton 0
	set_default wps_physical_pushbutton 0
	set_default wps_virtual_display 0
	set_default wps_physical_display 0

	config_methods=
	[ "$wps_pushbutton" -gt 0 ] && append config_methods push_button
	[ "$wps_label" -gt 0 ] && append config_methods label
	[ "$wps_keypad" -gt 0 ] && append config_methods keypad
	[ "$wps_virtual_pushbutton" -gt 0 ] && append config_methods virtual_push_button
	[ "$wps_physical_pushbutton" -gt 0 ] && append config_methods physical_push_button
	[ "$wps_virtual_display" -gt 0 ] && append config_methods virtual_display
	[ "$wps_physical_display" -gt 0 ] && append config_methods physical_display

	[ "$macfilter" = "allow" ] && wps_possible=

	# WPS not possible on Multi-AP backhaul-only SSID
	[ "$multi_ap" = 1 ] && wps_possible=

	[ -n "$wps_possible" -a -n "$config_methods" ] && {
		set_default ext_registrar 0
		set_default wps_device_type "6-0050F204-1"
		set_default wps_device_name "WLAN-ROUTER"
		set_default wps_manufacturer "Intel Corporation"
		set_default wps_manufacturer_url "http://www.intel.com"
		set_default wps_model_description "TR069 Gateway"
		set_default wps_os_version "01020300"
		set_default wps_cred_processing 1
		set_default wps_independent 1
		set_default wps_state 2

		if [ "$ext_registrar" -gt "0" ] && [ -n "$network_bridge" ]; then
			append bss_conf "upnp_iface=$network_bridge" "$N"
		elif [ -n "$upnp_bridge" ]; then
			append bss_conf "upnp_iface=$upnp_bridge" "$N"
		fi

		append bss_conf "eap_server=1" "$N"
		[ -n "$wps_pin" ] && append bss_conf "ap_pin=$wps_pin" "$N"
		[ -n "$wps_uuid" ] && append bss_conf "uuid=$wps_uuid" "$N"
		[ -n "$wps_pin_requests" ] && append bss_conf "wps_pin_requests=$wps_pin_requests" "$N"
		append bss_conf "wps_state=$wps_state" "$N"
		append bss_conf "device_type=$wps_device_type" "$N"
		append bss_conf "device_name=$wps_device_name" "$N"
		append bss_conf "manufacturer=$wps_manufacturer" "$N"
		append bss_conf "config_methods=$config_methods" "$N"
		append bss_conf "wps_independent=$wps_independent" "$N"
		append bss_conf "wps_cred_processing=$wps_cred_processing" "$N"
		[ -n "$wps_ap_setup_locked" ] && append bss_conf "ap_setup_locked=$wps_ap_setup_locked" "$N"
		[ "$wps_pbc_in_m1" -gt 0 ] && append bss_conf "pbc_in_m1=$wps_pbc_in_m1" "$N"
		[ -n "$wps_cred_add_sae" ] && append bss_conf "wps_cred_add_sae=$wps_cred_add_sae" "$N"
		[ "$multi_ap" -gt 0 ] && [ -n "$multi_ap_backhaul_ssid" ] && {
			append bss_conf "multi_ap_backhaul_ssid=\"$multi_ap_backhaul_ssid\"" "$N"
			if [ -z "$multi_ap_backhaul_key" ]; then
				:
			elif [ ${#multi_ap_backhaul_key} -lt 8 ]; then
				wireless_setup_vif_failed INVALID_WPA_PSK
				return 1
			elif [ ${#multi_ap_backhaul_key} -eq 64 ]; then
				append bss_conf "multi_ap_backhaul_wpa_psk=$multi_ap_backhaul_key" "$N"
			else
				append bss_conf "multi_ap_backhaul_wpa_passphrase=$multi_ap_backhaul_key" "$N"
			fi
		}
		append bss_conf "os_version=$wps_os_version" "$N"
		append bss_conf "manufacturer_url=$wps_manufacturer_url" "$N"
		append bss_conf "model_description=$wps_model_description" "$N"

		[ -n "$wps_rf_bands" ] && append bss_conf "wps_rf_bands=$wps_rf_bands" "$N"
		[ -n "$wps_model_name" ] && append bss_conf "model_name=$wps_model_name" "$N"
		[ -n "$wps_model_number" ] && append bss_conf "model_number=$wps_model_number" "$N"
		[ -n "$wps_serial_number" ] && append bss_conf "serial_number=$wps_serial_number" "$N"
		[ -n "$wps_skip_cred_build" ] && append bss_conf "skip_cred_build=$wps_skip_cred_build" "$N"
		[ -n "$wps_extra_cred" ] && append bss_conf "extra_cred=$wps_extra_cred" "$N"
		[ -n "$wps_ap_settings" ] && append bss_conf "ap_settings=$wps_ap_settings" "$N"
		[ -n "$wps_friendly_name" ] && append bss_conf "friendly_name=$wps_friendly_name" "$N"
		[ -n "$wps_model_url" ] && append bss_conf "model_url=$wps_model_url" "$N"
		[ -n "$wps_upc" ] && append bss_conf "upc=$wps_upc" "$N"
	}

	append bss_conf "ssid=$ssid" "$N"

	[ -n "$colocated_6g_radio_info" ] && append bss_conf "colocated_6g_radio_info=$colocated_6g_radio_info" "$N"
	json_get_values colocated_6g_vap_info_list colocated_6g_vap_info
	i=1
	num_coloc_vap_param=8 # <Radio_index> <BSSID> <SSID> <multibss_enable> 
				#<is_transmitted_bssid> <unsolicited_frame_support> <max_tx_power> <is_hidden>

	if [ -n "$colocated_6g_vap_info_list" ]; then
		for j in $colocated_6g_vap_info_list #special handling as list data type
		do
			if [ -n "$val" ]; then
				val=$val" "$j
			else
				val=$j
			fi
			if [ $i -eq $num_coloc_vap_param ]; then
				append bss_conf "colocated_6g_vap_info=$val" "$N"
				i=1
				val=""
			else
				i=$((i+1))
			fi

		done
	fi

	if [ -n "$network_bridge" ]; then
		 append bss_conf "bridge=$network_bridge" "$N"
	else
		if [ -n "$wav_bridge" ]; then
			append bss_conf "bridge=$wav_bridge" "$N"
		fi
	fi
	[ -n "$iapp_interface" ] && {
		local ifname
		network_get_device ifname "$iapp_interface" || ifname="$iapp_interface"
		append bss_conf "iapp_interface=$ifname" "$N"
	}

	json_get_vars time_advertisement time_zone wnm_sleep_mode wnm_sleep_mode_no_keys bss_transition
	set_default bss_transition 0
	set_default wnm_sleep_mode 0
	set_default wnm_sleep_mode_no_keys 0

	[ -n "$time_advertisement" ] && append bss_conf "time_advertisement=$time_advertisement" "$N"
	[ -n "$time_zone" ] && append bss_conf "time_zone=$time_zone" "$N"
	if [ "$wnm_sleep_mode" -eq "1" ]; then
		append bss_conf "wnm_sleep_mode=1" "$N"
		[ "$wnm_sleep_mode_no_keys" -eq "1" ] && append bss_conf "wnm_sleep_mode_no_keys=1" "$N"
	fi
	[ "$bss_transition" -eq "1" ] && append bss_conf "bss_transition=1" "$N"

	json_get_vars ieee80211k rrm_neighbor_report rrm_beacon_report
	set_default ieee80211k 0
	if [ "$ieee80211k" -eq "1" ]; then
		set_default rrm_neighbor_report 1
		set_default rrm_beacon_report 1
	else
		set_default rrm_neighbor_report 0
		set_default rrm_beacon_report 0
	fi

	[ "$rrm_neighbor_report" -eq "1" ] && append bss_conf "rrm_neighbor_report=1" "$N"
	[ "$rrm_beacon_report" -eq "1" ] && append bss_conf "rrm_beacon_report=1" "$N"

	json_get_vars ftm_responder stationary_ap lci civic
	set_default ftm_responder 0
	if [ "$ftm_responder" -eq "1" ]; then
		set_default stationary_ap 0
		iw phy "$phy" info | grep -q "ENABLE_FTM_RESPONDER" && {
			append bss_conf "ftm_responder=1" "$N"
			[ "$stationary_ap" -eq "1" ] && append bss_conf "stationary_ap=1" "$N"
			[ -n "$lci" ] && append bss_conf "lci=$lci" "$N"
			[ -n "$civic" ] && append bss_conf "lci=$civic" "$N"
		}
	fi

	if [ "$wpa" -ge "1" ]; then
		json_get_vars ieee80211r
		set_default ieee80211r 0

		if [ "$ieee80211r" -gt "0" ]; then
			json_get_vars mobility_domain ft_psk_generate_local ft_over_ds reassociation_deadline

			set_default mobility_domain "$(echo "$ssid" | md5sum | head -c 4)"
			set_default ft_over_ds 1
			set_default reassociation_deadline 1000

			case "$auth_type" in
				psk|sae|psk-sae)
					set_default ft_psk_generate_local 1
				;;
				*)
					set_default ft_psk_generate_local 0
				;;
			esac

			append bss_conf "mobility_domain=$mobility_domain" "$N"
			append bss_conf "ft_psk_generate_local=$ft_psk_generate_local" "$N"
			append bss_conf "ft_over_ds=$ft_over_ds" "$N"
			append bss_conf "reassociation_deadline=$reassociation_deadline" "$N"
			[ -n "$nasid" ] || append bss_conf "nas_identifier=${macaddr//\:}" "$N"

			if [ "$ft_psk_generate_local" -eq "0" ]; then
				json_get_vars r0_key_lifetime r1_key_holder pmk_r1_push
				json_get_values r0kh r0kh
				json_get_values r1kh r1kh

				set_default r0_key_lifetime 10000
				set_default pmk_r1_push 0

				[ -n "$r1_key_holder" ] && append bss_conf "r1_key_holder=$r1_key_holder" "$N"
				append bss_conf "r0_key_lifetime=$r0_key_lifetime" "$N"
				append bss_conf "pmk_r1_push=$pmk_r1_push" "$N"

				for kh in $r0kh; do
					append bss_conf "r0kh=${kh//,/ }" "$N"
				done
				for kh in $r1kh; do
					append bss_conf "r1kh=${kh//,/ }" "$N"
				done
			fi
		fi

		append bss_conf "wpa_disable_eapol_key_retries=$wpa_disable_eapol_key_retries" "$N"

		hostapd_append_wpa_key_mgmt
		[ -n "$wpa_key_mgmt" ] && append bss_conf "wpa_key_mgmt=$wpa_key_mgmt" "$N"
	fi

	if [ "$wpa" -ge "2" ]; then
		if [ -n "$network_bridge" -a "$rsn_preauth" = 1 ]; then
			set_default auth_cache 1
			append bss_conf "rsn_preauth=1" "$N"
			append bss_conf "rsn_preauth_interfaces=$network_bridge" "$N"
		else
			case "$auth_type" in
			sae|psk-sae|owe)
				set_default auth_cache 1
			;;
			*)
				set_default auth_cache 0
			;;
			esac
		fi

		append bss_conf "okc=$auth_cache" "$N"
		# Don't write parameter "disable_pmksa_caching" if dpp_auth_enabled==1
		[ "$auth_cache" = 0 -a "$dpp_auth_enabled" -ne 1 ] && append bss_conf "disable_pmksa_caching=1" "$N"

		# RSN -> allow management frame protection
		case "$ieee80211w" in
			[012])
				json_get_vars ieee80211w_mgmt_cipher ieee80211w_max_timeout ieee80211w_retry_timeout beacon_protection_enabled disable_bigtk_rekey group_mgmt_cipher
				append bss_conf "ieee80211w=$ieee80211w" "$N"
				[ "$ieee80211w" -gt "0" ] && {
					set_default group_mgmt_cipher ${ieee80211w_mgmt_cipher:-AES-128-CMAC}
					[ -n "$ieee80211w_max_timeout" ] && \
						append bss_conf "assoc_sa_query_max_timeout=$ieee80211w_max_timeout" "$N"
					[ -n "$ieee80211w_retry_timeout" ] && \
						append bss_conf "assoc_sa_query_retry_timeout=$ieee80211w_retry_timeout" "$N"
					[ -n "$beacon_protection_enabled" ] && \
						append bss_conf "beacon_protection_enabled=$beacon_protection_enabled" "$N"
					[ -n "$disable_pbac" ] && \
						append bss_conf "disable_pbac=$disable_pbac" "$N"
					[ -n "$disable_bigtk_rekey" ] && \
						append bss_conf "disable_bigtk_rekey=$disable_bigtk_rekey" "$N"
					[ -n "$group_mgmt_cipher" ] && \
						append bss_conf "group_mgmt_cipher=$group_mgmt_cipher" "$N"
				}
			;;
		esac
	fi

	base_macfile="/var/run/hostapd-$ifname.maclist"
	_macfile="$(mktemp -u -p /var/run/ -t hostapd-${ifname}.maclist.XXXXXX)"
	case "$macfilter" in
		allow)
			append bss_conf "macaddr_acl=1" "$N"
			append bss_conf "accept_mac_file=$_macfile" "$N"
			# accept_mac_file can be used to set MAC to VLAN ID mapping
			vlan_possible=1
		;;
		deny)
			append bss_conf "macaddr_acl=0" "$N"
			append bss_conf "deny_mac_file=$_macfile" "$N"
		;;
		*)
			_macfile=""
		;;
	esac

	[ -n "$_macfile" ] && {
		json_get_vars macfile
		json_get_values maclist maclist

		rm -f "$base_macfile"*
		(
			for mac in $maclist; do
				echo "$mac"
			done
			[ -n "$macfile" -a -f "$macfile" ] && cat "$macfile"
		) > "$_macfile"
	}

	[ -n "$vlan_possible" -a -n "$dynamic_vlan" ] && {
		json_get_vars vlan_naming vlan_tagged_interface vlan_bridge vlan_file
		set_default vlan_naming 1
		[ -z "$vlan_file" ] && set_default vlan_file /var/run/hostapd-$ifname.vlan
		append bss_conf "dynamic_vlan=$dynamic_vlan" "$N"
		append bss_conf "vlan_naming=$vlan_naming" "$N"
		[ -n "$vlan_bridge" ] && \
			append bss_conf "vlan_bridge=$vlan_bridge" "$N"
		[ -n "$vlan_tagged_interface" ] && \
			append bss_conf "vlan_tagged_interface=$vlan_tagged_interface" "$N"
		[ -n "$vlan_file" ] && {
			[ -e "$vlan_file" ] || touch "$vlan_file"
			append bss_conf "vlan_file=$vlan_file" "$N"
		}
	}

	json_get_vars iw_enabled iw_internet iw_asra iw_esr iw_uesa iw_access_network_type
	json_get_vars iw_hessid iw_venue_group iw_venue_type iw_network_auth_type
	json_get_vars iw_roaming_consortium iw_domain_name iw_anqp_3gpp_cell_net iw_nai_realm
	json_get_vars iw_anqp_elem iw_qos_map_set iw_ipaddr_type_availability iw_gas_address3

	set_default iw_enabled 0
	if [ "$iw_enabled" = "1" ]; then
		append bss_conf "interworking=1" "$N"
		set_default iw_internet 1
		set_default iw_asra 0
		set_default iw_esr 0
		set_default iw_uesa 0

		append bss_conf "internet=$iw_internet" "$N"
		append bss_conf "asra=$iw_asra" "$N"
		append bss_conf "esr=$iw_esr" "$N"
		append bss_conf "uesa=$iw_uesa" "$N"

		[ -n "$iw_access_network_type" ] && \
			append bss_conf "access_network_type=$iw_access_network_type" "$N"
		[ -n "$iw_hessid" ] && append bss_conf "hessid=$iw_hessid" "$N"
		[ -n "$iw_venue_group" ] && \
			append bss_conf "venue_group=$iw_venue_group" "$N"
		[ -n "$iw_venue_type" ] && append bss_conf "venue_type=$iw_venue_type" "$N"
		[ -n "$iw_network_auth_type" ] && \
			append bss_conf "network_auth_type=$iw_network_auth_type" "$N"
		[ -n "$iw_gas_address3" ] && append bss_conf "gas_address3=$iw_gas_address3" "$N"
		[ -n "$iw_qos_map_set" ] && append bss_conf "qos_map_set=$iw_qos_map_set" "$N"

		json_for_each_item append_iw_roaming_consortium iw_roaming_consortium
		json_for_each_item append_iw_anqp_elem iw_anqp_elem
		json_for_each_item append_iw_nai_realm iw_nai_realm

		iw_domain_name_conf=
		json_for_each_item append_iw_domain_name iw_domain_name
		[ -n "$iw_domain_name_conf" ] && \
			append bss_conf "domain_name=$iw_domain_name_conf" "$N"

		iw_anqp_3gpp_cell_net_conf=
		json_for_each_item append_iw_anqp_3gpp_cell_net iw_anqp_3gpp_cell_net
		[ -n "$iw_anqp_3gpp_cell_net_conf" ] && \
			append bss_conf "anqp_3gpp_cell_net=$iw_anqp_3gpp_cell_net_conf" "$N"
	fi


	local hs20 disable_dgaf osen anqp_domain_id hs20_deauth_req_timeout \
		osu_ssid hs20_wan_metrics hs20_operating_class hs20_t_c_filename hs20_t_c_timestamp
	json_get_vars hs20 disable_dgaf osen anqp_domain_id hs20_deauth_req_timeout \
		osu_ssid hs20_wan_metrics hs20_operating_class hs20_t_c_filename hs20_t_c_timestamp

	set_default hs20 0
	set_default disable_dgaf $hs20
	set_default osen 0
	set_default anqp_domain_id 0
	set_default hs20_deauth_req_timeout 60
	if [ "$hs20" = "1" ]; then
		append bss_conf "hs20=1" "$N"
		append_hs20_icons
		append bss_conf "disable_dgaf=$disable_dgaf" "$N"
		append bss_conf "osen=$osen" "$N"
		append bss_conf "anqp_domain_id=$anqp_domain_id" "$N"
		append bss_conf "hs20_deauth_req_timeout=$hs20_deauth_req_timeout" "$N"
		[ -n "$osu_ssid" ] && append bss_conf "osu_ssid=$osu_ssid" "$N"
		[ -n "$hs20_wan_metrics" ] && append bss_conf "hs20_wan_metrics=$hs20_wan_metrics" "$N"
		[ -n "$hs20_operating_class" ] && append bss_conf "hs20_operating_class=$hs20_operating_class" "$N"
		[ -n "$hs20_t_c_filename" ] && append bss_conf "hs20_t_c_filename=$hs20_t_c_filename" "$N"
		[ -n "$hs20_t_c_timestamp" ] && append bss_conf "hs20_t_c_timestamp=$hs20_t_c_timestamp" "$N"
		json_for_each_item append_hs20_conn_capab hs20_conn_capab
		json_for_each_item append_hs20_oper_friendly_name hs20_oper_friendly_name
		json_for_each_item append_osu_provider osu_provider
		json_for_each_item append_operator_icon operator_icon
	fi

	set_default multicast_to_unicast 0
	if [ "$multicast_to_unicast" -gt 0 ]; then
		append bss_conf "multicast_to_unicast=$multicast_to_unicast" "$N"
	fi

	set_default per_sta_vif 0
	if [ "$per_sta_vif" -gt 0 ]; then
		append bss_conf "per_sta_vif=$per_sta_vif" "$N"
	fi

	json_get_values opts hostapd_bss_options
	for val in $opts; do
		append bss_conf "$val" "$N"
	done

	local wds wds_bridge dtim_period max_listen_int start_disabled
	json_get_vars wds wds_bridge dtim_period max_listen_int start_disabled

	set_default wds 0
	set_default start_disabled 0

	[ "$wds" -gt 0 ] && {
		append bss_conf "wds_sta=1" "$N"
		[ -n "$wds_bridge" ] && append bss_conf "wds_bridge=$wds_bridge" "$N"
	}

	[ "$type" = "bss" ] && [ "$staidx" -gt 0 ] && start_disabled=1 # Repeater mode & not Master VAP

	[ "$start_disabled" -eq 1 ] && append bss_conf "start_disabled=1" "$N"

	append bss_conf "bssid=$macaddr" "$N"
	[ -n "$dtim_period" ]    && append bss_conf "dtim_period=$dtim_period" "$N"
	[ -n "$max_listen_int" ] && append bss_conf "max_listen_interval=$max_listen_int" "$N"

	if [ -f /lib/netifd/debug_infrastructure.sh ]; then
		debug_infrastructure_append debug_hostap_conf_ bss_conf $mlo_enable $mlo_id

		if [ -n $mlo_enable ] && [ $mlo_id ]; then
			ap_mld_mac=`uci get wireless.$mlo_id.ap_mld_mac`
			if ! [ $ap_mld_mac ]; then
				hostapd_setup_ap_mld_mac $mlo_id $macaddr $ap_mld_mac
			fi
			append bss_conf "mlo_enable=1" "$N"
			append bss_conf "ap_mld_mac=$ap_mld_mac" "$N"
		fi
	fi

	bss_md5sum=$(echo $bss_conf | md5sum | cut -d" " -f1)
	append bss_conf "config_id=$bss_md5sum" "$N"

	append "$var" "$bss_conf" "$N"
	return 0
}

hostapd_set_log_options() {
	local var="$1"

	local log_level log_80211 log_8021x log_radius log_wpa log_driver log_iapp log_mlme
	json_get_vars log_level log_80211 log_8021x log_radius log_wpa log_driver log_iapp log_mlme

	set_default log_level 2
	set_default log_80211  1
	set_default log_8021x  1
	set_default log_radius 1
	set_default log_wpa    1
	set_default log_driver 1
	set_default log_iapp   1
	set_default log_mlme   1

	local log_mask=$(( \
		($log_80211  << 0) | \
		($log_8021x  << 1) | \
		($log_radius << 2) | \
		($log_wpa    << 3) | \
		($log_driver << 4) | \
		($log_iapp   << 5) | \
		($log_mlme   << 6)   \
	))

	append "$var" "logger_syslog=$log_mask" "$N"
	append "$var" "logger_syslog_level=$log_level" "$N"
	append "$var" "logger_stdout=$log_mask" "$N"
	append "$var" "logger_stdout_level=$log_level" "$N"

	return 0
}

_wpa_supplicant_common() {
	local ifname="$1"

	_rpath="/var/run/wpa_supplicant"
	_config="${_rpath}-$ifname.conf"
	_pid_file="${_rpath}-$ifname.pid"
}

wpa_supplicant_teardown_interface() {
	_wpa_supplicant_common "$1"
	rm -rf "$_rpath/$1" "$_config" "$_pid_file"
}

wpa_supplicant_prepare_interface() {
	local ifname="$1"
	_w_driver="$2"

	_wpa_supplicant_common "$1"

	json_get_vars mode wds multi_ap vendor_elems wps_manufacturer wps_device_name wps_device_type \
		wps_cred_processing wps_os_version wps_model_name wps_model_number wps_serial_number \
		wps_pushbutton wps_keypad wps_label wps_virtual_pushbutton wps_physical_pushbutton \
		wps_virtual_display wps_physical_display multi_ap_profile pmf process_radar_slave_events \
		wps_cred_add_sae disable_ht disable_vht disable_he disable_eht

	set_default wps_pushbutton 0
	set_default wps_keypad 0
	set_default wps_label 0
	set_default wps_virtual_pushbutton 0
	set_default wps_physical_pushbutton 0
	set_default wps_virtual_display 0
	set_default wps_physical_display 0
	set_default multi_ap 0

	[ -n "$network_bridge" ] && {
		fail=
		case "$mode" in
			adhoc)
				fail=1
			;;
			sta)
				[ "$wds" = 1 -o "$multi_ap" = 1 ] || fail=1
			;;
		esac

		[ -n "$fail" ] && {
			wireless_setup_vif_failed BRIDGE_NOT_ALLOWED
			return 1
		}
	}

	local ap_scan=

	_w_mode="$mode"

	[ "$mode" = adhoc ] && {
		ap_scan="ap_scan=2"
	}

	local country_str=
	[ -n "$country" ] && {
		country_str="country=$country"
	}

	local multiap_flag_file="${_config}.is_multiap"
	if [ "$multi_ap" = "1" ]; then
		touch "$multiap_flag_file"
	else
		rm -f "$multiap_flag_file"
	fi

	local vendor_elems_str=
	[ -n "$vendor_elems" ] && {
		vendor_elems_str="vendor_elems=$vendor_elems"
	}

	local wds_str=
	[ -n "$wds" ] && {
		wds_str="wds=$wds"
	}

	local multi_ap_profile_str=
	[ -n "$multi_ap_profile" ] && {
		multi_ap_profile_str="multi_ap_profile=$multi_ap_profile"
	}

	local pmf_str=
	[ -n "$pmf" ] && {
		pmf_str="pmf=$pmf"
	}

	local process_radar_slave_events_str=
	[ -n "$process_radar_slave_events" ] && {
		process_radar_slave_events_str="process_radar_slave_events=$process_radar_slave_events"
	}

	wpa_supplicant_teardown_interface "$ifname"
	cat > "$_config" <<EOF
${scan_list:+freq_list=$scan_list}
$ap_scan
$country_str
$vendor_elems_str
$wds_str
$multi_ap_profile_str
$pmf_str
$process_radar_slave_events_str
EOF

	config_methods=
	[ "$wps_pushbutton" -gt 0 ] && append config_methods push_button
	[ "$wps_label" -gt 0 ] && append config_methods label
	[ "$wps_keypad" -gt 0 ] && append config_methods keypad
	[ "$wps_virtual_pushbutton" -gt 0 ] && append config_methods virtual_push_button
	[ "$wps_physical_pushbutton" -gt 0 ] && append config_methods physical_push_button
	[ "$wps_virtual_display" -gt 0 ] && append config_methods virtual_display
	[ "$wps_physical_display" -gt 0 ] && append config_methods physical_display

	[ -n "$config_methods" ] && {
		set_default wps_manufacturer "Intel Corporation"
		set_default wps_device_name "WLAN-REPEATER"
		set_default wps_device_type "6-0050F204-1"
		set_default wps_cred_processing 1
		set_default wps_os_version "01020300"

		[ -n "$wps_manufacturer" ] && {
			echo "manufacturer=$wps_manufacturer" >> "$_config"
		}

		[ -n "$wps_device_name" ] && {
			echo "device_name=$wps_device_name" >> "$_config"
		}

		[ -n "$wps_device_type" ] && {
			echo "device_type=$wps_device_type" >> "$_config"
		}

		[ -n "$wps_cred_processing" ] && {
			echo "wps_cred_processing=$wps_cred_processing" >> "$_config"
		}

		[ -n "$wps_os_version" ] && {
			echo "os_version=$wps_os_version" >> "$_config"
		}

		[ -n "$wps_model_name" ] && {
			echo "model_name=$wps_model_name" >> "$_config"
		}

		[ -n "$wps_model_number" ] && {
			echo "model_number=$wps_model_number" >> "$_config"
		}

		[ -n "$wps_serial_number" ] && {
			echo "serial_number=$wps_serial_number" >> "$_config"
		}

		echo "config_methods=$config_methods" >> "$_config"
	}

	[ -n "$wps_cred_add_sae" ] && {
		echo "wps_cred_add_sae=$wps_cred_add_sae" >> "$_config"
	}

	[ -n "$disable_ht" ] && echo "disable_ht=$disable_ht" >> "$_config"
	[ -n "$disable_vht" ] && echo "disable_vht=$disable_vht" >> "$_config"
	[ -n "$disable_he" ] && echo "disable_he=$disable_he" >> "$_config"
	[ -n "$disable_eht" ] && echo "disable_eht=$disable_eht" >> "$_config"

	return 0
}

wpa_supplicant_set_fixed_freq() {
	local freq="$1"
	local htmode="$2"

	append network_data "fixed_freq=1" "$N$T"
	append network_data "frequency=$freq" "$N$T"
	case "$htmode" in
		NOHT) append network_data "disable_ht=1" "$N$T";;
		HT20|VHT20) append network_data "disable_ht40=1" "$N$T";;
		HT40*|VHT40*|VHT80*|VHT160*) append network_data "ht40=1" "$N$T";;
	esac
	case "$htmode" in
		VHT*) append network_data "vht=1" "$N$T";;
	esac
	case "$htmode" in
		VHT80) append network_data "max_oper_chwidth=1" "$N$T";;
		VHT160) append network_data "max_oper_chwidth=2" "$N$T";;
		VHT20|VHT40) append network_data "max_oper_chwidth=0" "$N$T";;
		*) append network_data "disable_vht=1" "$N$T";;
	esac
}

wpa_supplicant_add_network() {
	local ifname="$1"
	local freq="$2"
	local htmode="$3"
	local noscan="$4"

	_wpa_supplicant_common "$1"
	wireless_vif_parse_encryption

	json_get_vars \
		ssid bssid key \
		basic_rate mcast_rate \
		ieee80211w ieee80211r \
		multi_ap \
		multi_ap_profile

	case "$auth_type" in
		sae|owe|eap192|eap-eap192)
			set_default ieee80211w 2
		;;
		psk-sae)
			set_default ieee80211w 1
		;;
	esac

	set_default ieee80211r 0
	set_default multi_ap 0

	local key_mgmt='NONE'
	local network_data=
	local T="	"

	local scan_ssid="scan_ssid=1"
	local freq wpa_key_mgmt

	[ "$_w_mode" = "adhoc" ] && {
		append network_data "mode=1" "$N$T"
		[ -n "$freq" ] && wpa_supplicant_set_fixed_freq "$freq" "$htmode"
		[ "$noscan" = "1" ] && append network_data "noscan=1" "$N$T"

		scan_ssid="scan_ssid=0"

		[ "$_w_driver" = "nl80211" ] ||	append wpa_key_mgmt "WPA-NONE"
	}

	[ "$_w_mode" = "mesh" ] && {
		json_get_vars mesh_id mesh_fwding mesh_rssi_threshold
		[ -n "$mesh_id" ] && ssid="${mesh_id}"

		append network_data "mode=5" "$N$T"
		[ -n "$mesh_fwding" ] && append network_data "mesh_fwding=${mesh_fwding}" "$N$T"
		[ -n "$mesh_rssi_threshold" ] && append network_data "mesh_rssi_threshold=${mesh_rssi_threshold}" "$N$T"
		[ -n "$freq" ] && wpa_supplicant_set_fixed_freq "$freq" "$htmode"
		[ "$noscan" = "1" ] && append network_data "noscan=1" "$N$T"
		append wpa_key_mgmt "SAE"
		scan_ssid=""
	}

	[ "$multi_ap" = 1 -a "$_w_mode" = "sta" ] && {
		append network_data "multi_ap_backhaul_sta=1" "$N$T"
		[ -n "$multi_ap_profile" ] && append network_data "multi_ap_profile=$multi_ap_profile" "$N$T"
	}

	[ "$dpp_auth_enabled" -eq 1 ] && hostapd_append_wpa_key_mgmt

	case "$auth_type" in
		none) ;;
		owe)
			hostapd_append_wpa_key_mgmt
			key_mgmt="$wpa_key_mgmt"
		;;
		wep)
			local wep_keyidx=0
			hostapd_append_wep_key network_data
			append network_data "wep_tx_keyidx=$wep_keyidx" "$N$T"
		;;
		wps)
			key_mgmt='WPS'
		;;
		psk|sae|psk-sae)
			local passphrase

			if [ "$_w_mode" != "mesh" ]; then
				hostapd_append_wpa_key_mgmt
			fi

			key_mgmt="$wpa_key_mgmt"

			if [ ${#key} -eq 64 ]; then
				passphrase="psk=${key}"
			else
				if [ "$_w_mode" = "mesh" ]; then
					passphrase="sae_password=\"${key}\""
				else
					passphrase="psk=\"${key}\""
				fi
			fi
			append network_data "$passphrase" "$N$T"
		;;
		eap|eap192|eap-eap192)
			hostapd_append_wpa_key_mgmt
			key_mgmt="$wpa_key_mgmt"

			json_get_vars eap_type identity anonymous_identity ca_cert ca_cert_usesystem

			if [ "$ca_cert_usesystem" -eq "1" -a -f "/etc/ssl/certs/ca-certificates.crt" ]; then
				append network_data "ca_cert=\"/etc/ssl/certs/ca-certificates.crt\"" "$N$T"
			else
				[ -n "$ca_cert" ] && append network_data "ca_cert=\"$ca_cert\"" "$N$T"
			fi
			[ -n "$identity" ] && append network_data "identity=\"$identity\"" "$N$T"
			[ -n "$anonymous_identity" ] && append network_data "anonymous_identity=\"$anonymous_identity\"" "$N$T"
			case "$eap_type" in
				tls)
					json_get_vars client_cert priv_key priv_key_pwd
					append network_data "client_cert=\"$client_cert\"" "$N$T"
					append network_data "private_key=\"$priv_key\"" "$N$T"
					append network_data "private_key_passwd=\"$priv_key_pwd\"" "$N$T"

					json_get_vars subject_match
					[ -n "$subject_match" ] && append network_data "subject_match=\"$subject_match\"" "$N$T"

					json_get_values altsubject_match altsubject_match
					if [ -n "$altsubject_match" ]; then
						local list=
						for x in $altsubject_match; do
							append list "$x" ";"
						done
						append network_data "altsubject_match=\"$list\"" "$N$T"
					fi

					json_get_values domain_match domain_match
					if [ -n "$domain_match" ]; then
						local list=
						for x in $domain_match; do
							append list "$x" ";"
						done
						append network_data "domain_match=\"$list\"" "$N$T"
					fi

					json_get_values domain_suffix_match domain_suffix_match
					if [ -n "$domain_suffix_match" ]; then
						local list=
						for x in $domain_suffix_match; do
							append list "$x" ";"
						done
						append network_data "domain_suffix_match=\"$list\"" "$N$T"
					fi
				;;
				fast|peap|ttls)
					json_get_vars auth password ca_cert2 ca_cert2_usesystem client_cert2 priv_key2 priv_key2_pwd
					set_default auth MSCHAPV2

					if [ "$auth" = "EAP-TLS" ]; then
						if [ "$ca_cert2_usesystem" -eq "1" -a -f "/etc/ssl/certs/ca-certificates.crt" ]; then
							append network_data "ca_cert2=\"/etc/ssl/certs/ca-certificates.crt\"" "$N$T"
						else
							[ -n "$ca_cert2" ] && append network_data "ca_cert2=\"$ca_cert2\"" "$N$T"
						fi
						append network_data "client_cert2=\"$client_cert2\"" "$N$T"
						append network_data "private_key2=\"$priv_key2\"" "$N$T"
						append network_data "private_key2_passwd=\"$priv_key2_pwd\"" "$N$T"
					else
						append network_data "password=\"$password\"" "$N$T"
					fi

					json_get_vars subject_match
					[ -n "$subject_match" ] && append network_data "subject_match=\"$subject_match\"" "$N$T"

					json_get_values altsubject_match altsubject_match
					if [ -n "$altsubject_match" ]; then
						local list=
						for x in $altsubject_match; do
							append list "$x" ";"
						done
						append network_data "altsubject_match=\"$list\"" "$N$T"
					fi

					json_get_values domain_match domain_match
					if [ -n "$domain_match" ]; then
						local list=
						for x in $domain_match; do
							append list "$x" ";"
						done
						append network_data "domain_match=\"$list\"" "$N$T"
					fi

					json_get_values domain_suffix_match domain_suffix_match
					if [ -n "$domain_suffix_match" ]; then
						local list=
						for x in $domain_suffix_match; do
							append list "$x" ";"
						done
						append network_data "domain_suffix_match=\"$list\"" "$N$T"
					fi

					phase2proto="auth="
					case "$auth" in
						"auth"*)
							phase2proto=""
						;;
						"EAP-"*)
							auth="$(echo $auth | cut -b 5- )"
							[ "$eap_type" = "ttls" ] &&
								phase2proto="autheap="
							json_get_vars subject_match2
							[ -n "$subject_match2" ] && append network_data "subject_match2=\"$subject_match2\"" "$N$T"

							json_get_values altsubject_match2 altsubject_match2
							if [ -n "$altsubject_match2" ]; then
								local list=
								for x in $altsubject_match2; do
									append list "$x" ";"
								done
								append network_data "altsubject_match2=\"$list\"" "$N$T"
							fi

							json_get_values domain_match2 domain_match2
							if [ -n "$domain_match2" ]; then
								local list=
								for x in $domain_match2; do
									append list "$x" ";"
								done
								append network_data "domain_match2=\"$list\"" "$N$T"
							fi

							json_get_values domain_suffix_match2 domain_suffix_match2
							if [ -n "$domain_suffix_match2" ]; then
								local list=
								for x in $domain_suffix_match2; do
									append list "$x" ";"
								done
								append network_data "domain_suffix_match2=\"$list\"" "$N$T"
							fi
						;;
					esac
					append network_data "phase2=\"$phase2proto$auth\"" "$N$T"
				;;
			esac
			append network_data "eap=$(echo $eap_type | tr 'a-z' 'A-Z')" "$N$T"
		;;
	esac

	[ "$wpa_cipher" = GCMP ] && {
		append network_data "pairwise=GCMP" "$N$T"
		append network_data "group=GCMP" "$N$T"
	}

	[ "$mode" = mesh ] || {
		case "$wpa" in
			1)
				append network_data "proto=WPA" "$N$T"
			;;
			2)
				append network_data "proto=RSN" "$N$T"
			;;
		esac

		case "$ieee80211w" in
			[012])
				[ "$wpa" -ge 2 ] && append network_data "ieee80211w=$ieee80211w" "$N$T"
			;;
		esac
	}
	[ -n "$bssid" ] && append network_data "bssid=$bssid" "$N$T"

	local bssid_blacklist bssid_whitelist
	json_get_values bssid_blacklist bssid_blacklist
	json_get_values bssid_whitelist bssid_whitelist

	[ -n "$bssid_blacklist" ] && append network_data "bssid_blacklist=$bssid_blacklist" "$N$T"
	[ -n "$bssid_whitelist" ] && append network_data "bssid_whitelist=$bssid_whitelist" "$N$T"

	[ -n "$basic_rate" ] && {
		local br rate_list=
		for br in $basic_rate; do
			wpa_supplicant_add_rate rate_list "$br"
		done
		[ -n "$rate_list" ] && append network_data "rates=$rate_list" "$N$T"
	}

	[ -n "$mcast_rate" ] && {
		local mc_rate=
		wpa_supplicant_add_rate mc_rate "$mcast_rate"
		append network_data "mcast_rate=$mc_rate" "$N$T"
	}

	if [ "$key_mgmt" = "WPS" ]; then
		echo "wps_cred_processing=1" >> "$_config"
	else

	[ -n "$ssid" ] && {
		cat >> "$_config" <<EOF
network={
	$scan_ssid
	ssid="$ssid"
	key_mgmt=$key_mgmt
	$network_data
}
EOF
	}
	fi
	return 0
}

wpa_supplicant_run() {
	local ifname="$1"; shift

	_wpa_supplicant_common "$ifname"

	[ -f "$_pid_file" ] && [ kill `cat "$_pid_file"` >/dev/null 2>&1 ]
	kill `ps -w | grep wpa_supplicant | grep ${ifname} | awk '{print $1;}' ` >/dev/null 2>&1

	/usr/sbin/wpa_supplicant -s -B \
		${network_bridge:+-b $network_bridge} \
		-P "$_pid_file" \
		-D ${_w_driver:-wext} \
		-i "$ifname" \
		-c "$_config" \
		-C "$_rpath" \
		"$@"

	ret="$?"

	if [ "$ret" != 0 ]; then
		wireless_setup_vif_failed WPA_SUPPLICANT_FAILED
		return $ret
	fi

	retry_count=0
	wpa_supplicant_pid=
	until [ $retry_count -ge 5 ]
	do
		wpa_supplicant_pid=`cat $_pid_file`
		if [ -n "$wpa_supplicant_pid" ]; then
			break;
		fi
		retry_count=$((retry_count+1))
		sleep 1
	done

	if [ -n "$wpa_supplicant_pid" ]; then
		wireless_add_process "$wpa_supplicant_pid" /usr/sbin/wpa_supplicant 1
	else
		wireless_setup_vif_failed WPA_SUPPLICANT_FAILED
		ret=1
	fi

	return $ret
}

hostapd_common_cleanup() {
	killall meshd-nl80211
}
