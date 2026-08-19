#!/bin/sh

#Global variables

# Print DUT log on console
DUT_DEBUG_LOG=true

# Define valid center channels for different bands and bandwidths
valid_center_channels_2_4GHz_20="1 2 3 4 5 6 7 8 9 10 11 12 13 14"
valid_center_channels_2_4GHz_40="3 4 5 6 7 8 9 10 11"
valid_center_channels_5GHz_20="36 40 44 48 52 56 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 124 128 132 136 140 144 149 153 157 161 165 169 173 177"
valid_center_channels_5GHz_40="38 46 54 62 70 78 86 94 102 110 118 126 134 142 151 159 167 175"
valid_center_channels_5GHz_80="42 58 74 90 106 122 138 155"
valid_center_channels_5GHz_160="50 82 114 163"
valid_center_channels_6GHz_20="1 5 9 13 17 21 25 29 33 37 41 45 49 53 57 61 65 69 73 77 81 85 89 93 97 101 105 109 113 117 121 125 129 133 137 141 145 149 153 157 161 165 169 173 177 181 185 189 193 197 201 205 209 213 217 221 225 229 233"
valid_center_channels_6GHz_40="3 11 19 27 35 43 51 59 67 75 83 91 99 107 115 123 131 139 147 155 163 171 179 187 195 203 211 219 227"
valid_center_channels_6GHz_80="7 23 39 55 71 87 103 119 135 151 167 183 199 215 231"
valid_center_channels_6GHz_160="15 47 79 111 143 175 207"
valid_center_channels_6GHz_320="31 63 95 127 159 191"

# Function to search for an element in a space-separated string
# args: <channel> <valid_channel_array>
search_string() {
	local element=$1
	shift
	for val in $@; do
		if [ "$val" -eq "$element" ]; then
		return 0 # Element found
	 fi
	done
	return 1 # Element not found
}

util_check_mem_info()
{
    iw dev wlan4 iwlwav gEEPROM | grep "EEPROM type   : GPIO" > /dev/null
    if [ $? -eq 0 ]; then
        mem_type=1
        mem_size=2048
    else
        iw dev wlan4 iwlwav gEEPROM | grep "EEPROM type   : FILE" > /dev/null
        if [ $? -eq 0 ]; then
            mem_type=2
            mem_size=2048
        else
            echo "EEPROM or FLASH doesn't contain calibration data"
            exit 1
        fi
    fi
}

util_check_dut_mode_status()
{
	local pid_file="/tmp/dut_cli.pid"
	if [ ! -f "$pid_file" ]; then
		echo "false"
		return
	fi

	local pid=$(cat $pid_file)
	if [ -z "$pid" ]; then
		echo "false"
		return
	fi

	if ps -p $pid > /dev/null; then
		echo "true"
	else
		echo "false"
	fi
}

util_manage_dut_mode()
{
	local action=$1
	local pid_file="/tmp/dut_cli.pid"

	if [ "$action" == "true" ]; then
		echo "Stopping dutserver"
		/opt/intel/wave/scripts/load_dut.sh stop
		echo "Starting dutserver"
		/opt/intel/wave/scripts/load_dut.sh start

		# wait until dutserver started
		while true
		do
			if  pgrep -x "dutserver" > /dev/null
			then
				break;
			else
				sleep 0.01
			fi
		done

		echo "Run DUT mode"
		echo "" > /tmp/dut_log
		echo "" > /tmp/dut_commands.txt

		# Get Current br-lan IP address and run dut_cli
		local ip_addr=$(ifconfig br-lan | grep 'inet addr' | awk -F: '{print $2}' | awk '{print $1}')
		tail -f -s0 /tmp/dut_commands.txt | dut_cli -l 4 -a $ip_addr >> /tmp/dut_log &
		echo $(pgrep -f "tail -f -s0 /tmp/dut_commands.txt") > $pid_file

		# Wait for prompt
		util_wait_for_prompt "/tmp/dut_log"
	else
		local pid=$(cat $pid_file)
		kill -9 $pid
		rm $pid_file

		echo "Stopping dutserver"
		/opt/intel/wave/scripts/load_dut.sh stop
	fi
}

util_manage_pwhm()
{
	local action=$1

	# "amx-processmonitor" full name seems not to work for pgrep -x
	if ! pgrep "amx-process" > /dev/null
	then
		if [ "$action" == "true" ]; then
			if [ -f /etc/init.d/amx-processmonitor ]; then
				# Need to start amx-processmonitor to monitor pwhm/wld if exists
				/etc/init.d/amx-processmonitor start
			fi
		fi
	else
		if [ "$action" == "false" ]; then
			# Need to stop amx-processmonitor to prevent it from pulling pwhm/wld back
			# "/etc/init.d/amx-processmonitor stop" cannot work properly
			killall -9 amx-processmonitor
		fi
	fi

	if [ "$action" == "true" ]; then
		/etc/init.d/prplmesh_whm restart
		#TODO: WLANRTSYS-88389 Polling mechanism to see the status
		printInfo "Waiting for 20 seconds to get interfaces up and running"
		sleep 20
	else
		if ! pgrep "wld" > /dev/null
		then
			printInfo "pwhm is not running"
			return
		fi

		# Stop pwhm before entering DUT mode
		/etc/init.d/prplmesh_whm stop
		printInfo "Waiting for 10 sec to stop pwhm"
		sleep 10
	fi

}

util_check_wifi_driver_status()
{
	# List of modules to check
	local modules="cfg80211 compat mac80211 mtlk mtlkroot"
	local status="false"

	# Check each module
	for module in $modules; do
		if lsmod | grep mtlk | awk '{print $1}' | grep -qw "$module"; then
			status="true"
		else
			status="false"
			break
		fi
	done

	echo "$status"
}

util_manage_wifi_radio()
{
	local action=$1
	local radio=$2
	local dut_mode=$(util_check_dut_mode_status)

	if [ $# -gt 2 ]; then
		printInfo "Error: Multiple options provided. Please provide only one radio or none."
		exit 1
	fi

	if [ "$dut_mode" == "true" ]; then
		local band
		util_check_mem_info
		if [ -z "$radio" ]; then
			for band in 0 2 4; do
				if [ "$action" == "true" ]; then
					echo "Initialize DUT band $band"
					dut_send_command "exec $band driverInit --memory-type $mem_type --memory-size $mem_size"
				else
					echo "Release DUT band $band"
					dut_send_command "exec $band driverRelease"
				fi
				util_check_dut_band_status $band
			done
		else
			band=$(util_extract_band $radio) || { echo "$band"; exit 1; }
			if [ "$action" == "true" ]; then
				echo "Initialize DUT band $band"
				dut_send_command "exec $band driverInit --memory-type $mem_type --memory-size $mem_size"
			else
				echo "Release DUT band $band"
				dut_send_command "exec $band driverRelease"
			fi
			util_check_dut_band_status $band
		fi
	else
		if [ -z "$radio" ]; then
			for radioid in 1 2 3; do
				ubus-cli "WiFi.Radio.$radioid.Enable=$action"
			done
			#TODO: WLANRTSYS-88389 Polling mechanism to see the radio status
			printInfo "Waiting for 10 sec to see status"
			sleep 10
			util_check_radio_status $action $radio
		else
			local radioid
			radioid=$(util_extract_radioid $radio) || { echo "$radioid"; exit 1; }
			ubus-cli "WiFi.Radio.$radioid.Enable=$action"
			#TODO: WLANRTSYS-88389 Polling mechanism to see the radio status
			printInfo "Waiting for 10 sec to see status"
			sleep 10
			util_check_radio_status $action $radio
		fi
	fi
}

util_init_wifi_driver()
{
	local dut_mode=$(util_check_dut_mode_status)
	local module_params=$(cat /etc/modules.d/iwlwav-driver-uci-debug | grep "mtlk fastpath")
	local command="modprobe $module_params"
	echo "Command: $command"
	eval $command

	# Only start pwhm if not in DUT mode
	if [ "$dut_mode" == "false" ]; then
		printInfo "Starting AP Mode..."
		util_manage_pwhm "true"
	fi
}

util_check_dut_band_status()
{
	local band=$1
	local status

	dut_send_command "exec $band getBand"
	status=$(grep "getBand" /tmp/dut_out$band)

	echo $status | grep "OK" > /dev/null
	if [ $? -eq 0 ]; then
		printInfo "DUT Interface $(util_convert_band_to_radio $band)G is enable"
	else
		printInfo "WARNING: DUT Interface $(util_convert_band_to_radio $band)G is not enabled"
	fi
}

util_check_radio_status() {
	local expectation=$1
	local radio=$2
	local status="true"
	local interfaces
	local cac_status
	local output
	local i

	if [ "$expectation" == "true" ]; then
		output="enabled"
	else
		output="disabled"
	fi

	if [ "$expectation" == "true" ]; then
		if [ -z "$radio" ] || [ "$radio" == "5" ]; then
			cac_status=$(util_check_cac_status)
			if [ "$cac_status" == "true" ]; then
				printInfo "WARNING: CAC is running on 5Ghz band. Wait until CAC is finished for any transmission in the 5Ghz band"
				if [ "$radio" == "5" ]; then
					return 0
				fi
			fi
		fi
	fi

	if [ -z "$radio" ]; then
		printInfo "No specific radio provided, All radios will be $output"
		if [ "$cac_status" == "true" ]; then
			interfaces="wlan0.1 wlan4.1"
		else
			interfaces="wlan0.1 wlan2.1 wlan4.1"
		fi
	else
		printInfo "Radio $radio will be $output"
		interfaces=$(util_extract_iface $radio)
	fi

	for iface in $interfaces; do
		i=0
		while [ "$i" -lt "10" ]; do
			state=$(cat /sys/class/net/$iface/operstate)
			if [ "$state" == "up" ]; then
				printInfo "Band $radio wifi interface $iface is up and running"
				break
			fi
			sleep 1
			i=$((i+1))
		done
		if [ "$i" -eq "10" ]; then
			status="false"
			printInfo "Band $radio wifi interface $iface is not up and running"
		fi
	done

	if [ "$expectation" == "$status" ]; then
		printInfo "SUCCESS: Wifi interfaces are $output successfully"
	else
		printInfo "ERROR: Wifi interfaces are not $output successfully"
	fi
}

util_check_cac_status()
{
# Get the last occurrence of "CAC started" or "CAC finished" from dmesg
	sleep 5
	local last_status=$(dmesg | grep -E "CAC started|CAC finished" | tail -n 1)

	if echo "$last_status" | grep -q "CAC started"; then
		echo "true"
	else
		echo "false"
	fi
}

# Function to convert frequency to center channel for different Wi-Fi bands
# args: <band_id> <center freq>
util_convert_centerfreq_to_centerchannel() {
	local wifi_band=$1
	local center_freq=$2
	local center_channel
	case $wifi_band in
		"0")
			center_channel=$(( (center_freq - 2407) / 5 ))
			;;
		"2")
			center_channel=$(( (center_freq - 5000) / 5 ))
			;;
		"4")
			center_channel=$(( (center_freq - 5950) / 5 ))
			;;
		*)
			printInfo "(convert_to_center_channel)Unsupported Wi-Fi band. Only band 2.4Ghz,5Ghz and 6Ghz are supported."
			exit 1 #Exit If conversion fails
			;;
	esac
	echo $center_channel
}

# Function to determine the appropriate bandwidth if not specified
# args: <center_channel> <band_id>
determine_bandwidth() {
	local center_channel=$1
	local wifi_band=$2
	local bandwidth

	case $wifi_band in
		"0")
			if search_string "$center_channel" "$valid_center_channels_2_4GHz_20"; then
				bandwidth=0
			elif search_string "$center_channel" "$valid_center_channels_2_4GHz_40"; then
				bandwidth=1
			else
				printInfo "(determine_bandwidth)Error: Unsupported center channel for 2.4GHz band."
				exit 1
			fi
			;;
	 "2")
		if search_string "$center_channel" "$valid_center_channels_5GHz_20"; then
			bandwidth=0
		elif search_string "$center_channel" "$valid_center_channels_5GHz_40"; then
			bandwidth=1
		elif search_string "$center_channel" "$valid_center_channels_5GHz_80"; then
			bandwidth=2
		elif search_string "$center_channel" "$valid_center_channels_5GHz_160"; then
			bandwidth=3
		else
			printInfo "(determine_bandwidth)Error: Unsupported center channel for 5GHz band."
			exit 1
		fi
		;;
	 "4")
		if search_string "$center_channel" "$valid_center_channels_6GHz_20"; then
			bandwidth=0
		elif search_string "$center_channel" "$valid_center_channels_6GHz_40"; then
			bandwidth=1
		elif search_string "$center_channel" "$valid_center_channels_6GHz_80"; then
			bandwidth=2
		elif search_string "$center_channel" "$valid_center_channels_6GHz_160"; then
			bandwidth=3
		elif search_string "$center_channel" "$valid_center_channels_6GHz_320"; then
			bandwidth=4
		else
			printInfo "(determine_bandwidth)Error: Unsupported center channel for 6GHz band."
			exit 1
		fi
		;;
	 *)
		printInfo "(determine_bandwidth)Unsupported Wi-Fi band. Only 2.4GHz, 5GHz, and 6GHz are supported."
		exit 1
		;;
	esac

	echo $bandwidth
}

# Function to determine the maximum supported bandwidth for given protocol
# args: <band_id> <phymode>
# *phymode 802.11a = 0, 802.11b = 1, 802.11g = 2, 802.11n 5GHz = 4, 802.11n 2.4GHz = 5, 802.11ac = 6, 802.11ax = 7 and 802.11be = 8
util_check_max_bandwidth() {
	local band=$1
	local phymode=$2

	case $band in
		"0")
			if [[ $phymode -ge 1 && $phymode -le 2 ]]; then
				echo 20
			elif [ $phymode -ge 5 ]; then
				echo 40
			else
				printInfo "(util_check_max_bandwidth) Invalid protocol for 2.4GHz"
			fi
			;;
		"2")
			if [ $phymode -eq 0 ]; then
				echo 20
			elif [ $phymode -eq 4 ]; then
				echo 40
			elif [ $phymode -ge 6 ]; then
				echo 160
			else
				printInfo "(util_check_max_bandwidth) Invalid protocol for 5GHz"
			fi
			;;
		"4")
			if [ $phymode -eq 7 ]; then
				echo 160
			elif [ $phymode -eq 8 ]; then
				echo 320
			else
				printInfo "(util_check_max_bandwidth) Invalid protocol for 6GHz"
			fi
			;;
		*)
			printInfo "(util_check_max_bandwidth) Unsupported Wi-Fi band. Only 2.4GHz, 5GHz, and 6GHz are supported."
			exit 1
			;;
	esac
}

# Function to determine the maximum supported spatial stream for given protocol
# args: <phymode>
# *phymode 802.11a = 0, 802.11b = 1, 802.11g = 2, 802.11n 5GHz = 4, 802.11n 2.4GHz = 5, 802.11ac = 6, 802.11ax = 7 and 802.11be = 8
util_check_max_nss() {
	local phymode=$1

	if [ $phymode -le 2 ]; then
		echo 1
	else
		echo 4
	fi
}

# Function to validate center channel and bandwidth
# args: <center_channel> <bandwidth> <band_id> <phymode>
# *phymode 802.11a = 0, 802.11b = 1, 802.11g = 2, 802.11n 5GHz = 4, 802.11n 2.4GHz = 5, 802.11ac = 6, 802.11ax = 7 and 802.11be = 8
util_validate_center_channel_and_bandwidth() {
	local center_channel=$1
	local bandwidth=$2
	local band=$3
	local phymode=$4

	case $band in
		"0")
			# 20MHz
			if [ $bandwidth -eq 0 ]; then
					if ! search_string "$center_channel" "$valid_center_channels_2_4GHz_20"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 2.4GHz 20MHz bandwidth."
						exit 1
					fi
			# 40MHz
			elif [[ $bandwidth -eq 1 && $phymode -ge 5 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_2_4GHz_40"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 2.4GHz 40MHz bandwidth."
						exit 1
					fi
			else
					printInfo "(validate_center_channel_and_bandwidth) Unsupported bandwidth $bandwidth for 2.4GHzband phymode $phymode"
					exit 1
			fi
			;;
		"2")
			# 20MHz
			if [ $bandwidth -eq 0 ]; then
					if ! search_string "$center_channel" "$valid_center_channels_5GHz_20"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 5GHz 20MHz bandwidth."
						exit 1
					fi
			# 40MHz
			elif [[ $bandwidth -eq 1 && $phymode -ge 4 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_5GHz_40"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 5GHz 40MHz bandwidth."
						exit 1
					fi
			# 80MHz
			elif [[ $bandwidth -eq 2 && $phymode -ge 6 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_5GHz_80"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 5GHz 80MHz bandwidth."
						exit 1
					fi
			# 160MHz
			elif [[ $bandwidth -eq 3 && $phymode -ge 6 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_5GHz_160"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 5GHz 160MHz bandwidth."
						exit 1
					fi
			else
					printInfo "(validate_center_channel_and_bandwidth) Unsupported bandwidth for 5GHz. Only 20, 40, 80, and 160 MHz are supported."
					exit 1
			fi
			;;
		"4")
			# 20MHz
			if [[ $bandwidth -eq 0 && $phymode -ge 7 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_6GHz_20"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 6GHz 20MHz bandwidth."
						exit 1
					fi
			# 40MHz
			elif [[ $bandwidth -eq 1 && $phymode -ge 7 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_6GHz_40"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 6GHz 40MHz bandwidth."
						exit 1
					fi
			# 80MHz
			elif [[ $bandwidth -eq 2 && $phymode -ge 7 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_6GHz_80"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 6GHz 80MHz bandwidth."
						exit 1
					fi
			# 160MHz
			elif [[ $bandwidth -eq 3 && $phymode -ge 7 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_6GHz_160"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 6GHz 160MHz bandwidth."
						exit 1
					fi
			# 320MHz
			elif [[ $bandwidth -eq 4 && $phymode -ge 8 ]]; then
					if ! search_string "$center_channel" "$valid_center_channels_6GHz_320"; then
						printInfo "(validate_center_channel_and_bandwidth) Invalid center channel for 6GHz 320MHz bandwidth."
						exit 1
					fi
			else
					printInfo "(validate_center_channel_and_bandwidth) Unsupported bandwidth for 6GHz. Only 20, 40, 80, 160, and 320 MHz are supported."
					exit 1
			fi
			;;
		*)
			printInfo "(validate_center_channel_and_bandwidth) Unsupported Wi-Fi band. Only 2.4GHz, 5GHz, and 6GHz are supported."
			exit 1
			;;
		esac
}

# Function to calculate the lowest channel based on center channel and bandwidth
# args: <band_id> <bandwidth> <center_channel>
util_calculate_lowest_channel() {
	local center_channel=$3
	local bandwidth=$2
	local wifi_band=$1
	local lowest_channel

	case $wifi_band in
		"0")
			case $bandwidth in
				0)
					lowest_channel=$center_channel
					;;
				1)
					lowest_channel=$((center_channel - 2))
					;;
				*)
					printInfo "(calculate_lowest_channel) Unsupported bandwidth for 2.4GHz. Only 20 and 40 MHz are supported."
					exit 1
				;;
			esac
			;;
		"2")
			case $bandwidth in
				0)
					lowest_channel=$center_channel
					;;
				1)
					lowest_channel=$((center_channel - 2))
					;;
				2)
					lowest_channel=$((center_channel - 6))
					;;
				3)
					lowest_channel=$((center_channel - 14))
					;;
				*)
					printInfo "(calculate_lowest_channel) Unsupported bandwidth for 5GHz. Only 20, 40, 80, and 160 MHz are supported."
					exit 1
					;;
			esac
			;;
		"4")
			case $bandwidth in
				0)
					lowest_channel=$center_channel
					;;
				1)
					lowest_channel=$((center_channel - 2))
					;;
				2)
					lowest_channel=$((center_channel - 6))
					;;
				3)
					lowest_channel=$((center_channel - 14))
					;;
				4)
					lowest_channel=$((center_channel - 30))
					;;
				*)
					printInfo "(calculate_lowest_channel) Unsupported bandwidth for 6GHz. Only 20, 40, 80, 160, and 320 MHz are supported."
					exit 1
				;;
			esac
			;;
		*)
			printInfo "(calculate_lowest_channel) Unsupported Wi-Fi band. Only 2.4GHz, 5GHz, and 6GHz are supported."
			exit 1
			;;
		esac

		# Return the lowest channel
	echo "$lowest_channel"
}

# Extract BW from input arguments
# args: <Input Bandwidth> <center_channel> <band_id>
util_extract_bw()
{
	local bw_str=$1

# extract bw
	if [ $bw_str == "-" ]; then
			echo "$(determine_bandwidth $2 $3)"
		elif [ $bw_str == "20" ]; then
			echo "0"
		elif [ $bw_str == "40" ]; then
			echo "1"
		elif [ $bw_str == "80" ]; then
			echo "2"
		elif [ $bw_str == "160" ]; then
			echo "3"
		elif [ $bw_str == "320" ]; then
			echo "4"
	else
		printInfo "Invalid Bandwidth $bw_str"
		exit 1
	fi
}


# Extract wlan Band id from input arguments
# args: <Input radio band>
util_extract_band()
{
	local band_str=$1
# extract band id
	if [ $band_str == "2.4" ]; then
		echo "0"
	elif [ $band_str == "5" ]; then
		echo "2"
	elif [ $band_str == "6" ]; then
		echo "4"
	else
		printInfo "Invalid Band $band_str"
		exit 1
	fi
}

# Convert wlan Band id to Radio name
# args: (Input band id)
util_convert_band_to_radio()
{
	local band_id=$1
# extract radio id
	if [ $band_id == "0" ]; then
		echo "2.4"
	elif [ $band_id == "2" ]; then
		echo "5"
	elif [ $band_id == "4" ]; then
		echo "6"
	else
		printInfo "Invalid Band index $band_id"
		exit 1
	fi
}

# Extract wlan Radio id from input arguments
# args: <Input radio band>
util_extract_radioid()
{
        local band_str=$1
# extract radio id
        if [ $band_str == "2.4" ]; then
                echo "1"
        elif [ $band_str == "5" ]; then
                echo "2"
        elif [ $band_str == "6" ]; then
                echo "3"
        else
                printInfo "Invalid Band $band_str"
                exit 1
        fi
}

util_extract_iface()
{
        local band_str=$1
# extract iface
        if [ $band_str == "2.4" ]; then
                echo "wlan0.1"
        elif [ $band_str == "5" ]; then
                echo "wlan2.1"
        elif [ $band_str == "6" ]; then
                echo "wlan4.1"
        else
                printInfo "Invalid Band $band_str"
                exit 1
        fi
}

# Extract phymode from input arguments
# args: <Input phymode> <band_id>
util_extract_phymode()
{
	local phymode_str=$1
	local band_id=$2
# extract phymode
# 802.11a = 0, 802.11b = 1, 802.11g = 2, 802.11n 5GHz = 4, 802.11n 2.4GHz = 5, 802.11ac = 6, 802.11ax = 7 and 802.11be = 8
	if [ $phymode_str == "a" ]; then
		if [ $band_id -ne 2 ]; then	# 11A mode supported only in 5G band
			printInfo "11A mode is not supported for band $band"
			exit 1
		fi
		echo "0"
	elif [ $phymode_str == "b" ]; then
		if [ $band_id -ne 0 ]; then	# 11B mode supported only in 2.4G band
			printInfo "11B mode is not supported for band $band"
			exit 1
		fi
		echo "1"
	elif [ $phymode_str == "g" ]; then
		if [ $band_id -ne 0 ]; then	# 11G mode supported only in 2.4G band
			printInfo "11G mode is not supported for band $band"
			exit 1
		fi
		echo "2"
	elif [ $phymode_str == "n" ]; then
		if [ $band_id -eq 2 ]; then	# 11N mode supported only for 2.4G and 5G bands
			echo "4"
		elif [ $band_id -eq 0 ]; then
			echo "5"
		else
			printInfo "11N mode is not supported for band $band"
			exit 1
		fi
	elif [ $phymode_str == "ac" ]; then
		if [ $band_id -ne 2 ]; then	# 11AC mode supported only in 5G band
			printInfo "11AC mode is not supported for band $band"
			exit 1
		fi
		echo "6"
	elif [ $phymode_str == "ax" ]; then
		echo "7"
	elif [ $phymode_str == "be" ]; then
		echo "8"
	else
			printInfo "Invlid Phy mode $phymode_str"
		exit 1
	fi
}

# Check whether the input value is frequency or channel number
# args: <center freq/channel value>
util_check_if_frequency() {
	local value=$1
	# Check if the value is a 4-digit number (frequency)
	if [[ ${#value} -eq 4 ]]; then
		echo "1" # It's a frequency
	else
		echo "0"  # It's not a frequency
	fi
}

# Check whether the frequency input is valid
# args: <band_id> <center frequency>
util_validate_frequency() {
	local band=$1
	local center_freq=$2

	# Check if the frequency is a valid multiple of 5 for 5GHz or 6GHz bands
	if [[ "$band" == "2" || "$band" == "4" ]]; then
		printInfo "Checking if center frequency is a multiple of 5 for band $band..."
		if [ $(( center_freq % 5 )) -ne 0 ]; then
			printInfo "Error: The center frequency is not a multiple of 5 for band $band."
			exit 1
		fi
		printInfo "Center frequency is a multiple of 5, proceeding..."
	else
		printInfo "Frequency validation skipped for band $band."
	fi
}

# Function to get mcs value with respect to protocol and given mcs/phyrate
#11ag:
#BPSK1/2=6Mbps -> 0
#BPSK3/4=9Mbps -> 14
#QPSK1/2=12Mbps -> 1
#QPSK3/4=18Mbps -> 2
#16-QAM1/2=24Mbps -> 3
#16-QAM3/4=36Mbps -> 4
#64-QAM2/3=48Mbps -> 5
#64-QAM3/4=54Mbps -> 6
#
#11b:
#5.5Mbps -> 17
util_get_mcs_value()
{
	local rate=$1
	local protocol=$2
	local mcsval

	# 11AG
	if [[ $protocol -eq "0"  ||  $protocol -eq "2" ]]; then
		case $rate in
			6)
				mcsval=0
				;;
			9)
				mcsval=20
				;;
			12)
				mcsval=1
				;;
			18)
				mcsval=2
				;;
			24)
				mcsval=3
				;;
			36)
				mcsval=4
				;;
			48)
				mcsval=5
				;;
			54)
				mcsval=6
				;;
			*)
				printInfo "Invalid Phy rate"
				exit 1
			;;
		esac
	elif [ $protocol -eq "1" ]; then	# 11B
		case $rate in
			1)
				mcsval=34
				;;
			2)
				mcsval=35
				;;
			5.5)
				mcsval=36
				;;
			11)
				mcsval=37
				;;
			*)
				printInfo "Invalid Phy rate"
				exit 1
		esac
	else
		mcsval=$rate
	fi

	echo $mcsval
}

# Function to count the number of bits set in a given value
util_count_bits_set() {
    local value=$1
    local count=0

    while [ $value -ne 0 ]; do
        # Increment count if the least significant bit is 1
        : $((count += value & 1))
        # Right shift the value by 1 bit
        : $((value >>= 1))
    done

    echo $count
}

# Function to round-off power value to 0.5dB resolution
# args: <powervalue>
util_power_round_to_half() {
  input=$1
  int_part=${input%.*}
  frac_part=${input#*.}

  if [ "$input" == "$int_part" ]; then
    # If the input is a whole number
    echo $int_part
    return
  fi

  frac_part=${frac_part:0:1}

  if [ -z "$frac_part" ]; then
    frac_part=0
  fi

  if [ "$frac_part" -lt 3 ]; then
    rounded=$int_part
  elif [ "$frac_part" -lt 8 ]; then
    rounded="${int_part}.5"
  else
    rounded=$((int_part + 1))
  fi

  echo $rounded
}

# Extract power value from input
# args: <Input power value>
util_extract_powerval()
{
	local powerval
	# check powerval beyond limit of 30dBm
	if [ $(awk -v var="$1" 'BEGIN { print (var > 30)}') -eq 1 ]; then
		printInfo "Illegal power value $1"
		exit 1
	fi

	powerval=$(util_power_round_to_half $1)
	# calculate power to be supplied to DUT. (double the input value)
	powerval=$(awk -v var="$powerval" 'BEGIN{print var * 2}')
	echo $powerval
}

# Check whether valid antenna mask is entered
# args: <antenna mask>
util_check_ant_mask()
{
	# check antenna mask
	if [ $(awk -v var=$1 'BEGIN { print (var > 15)}') -eq 1 ]; then
		printInfo "Illegal antenna mask $1"
		exit 1
	fi

	echo $1
}

# Print configured antennas
# args: <antenna mask>
util_print_antennas() {
    antsel=$1
    antennas=""

    if [ $((antsel & 1)) -ne 0 ]; then
        antennas="ANT1"
    fi
    if [ $((antsel & 2)) -ne 0 ]; then
        [ -n "$antennas" ] && antennas="$antennas, ANT2" || antennas="ANT2"
    fi
    if [ $((antsel & 4)) -ne 0 ]; then
        [ -n "$antennas" ] && antennas="$antennas, ANT3" || antennas="ANT3"
    fi
    if [ $((antsel & 8)) -ne 0 ]; then
        [ -n "$antennas" ] && antennas="$antennas, ANT4" || antennas="ANT4"
    fi

    if [ -n "$antennas" ]; then
		# Count how many antennas are in the string
		count=$(echo "$antennas" | awk -F, '{ print NF }')
		if [ "$count" -eq 1 ]; then
			echo "Antennas set to $antennas ONLY"
		else
			# Replace last comma with "and" for nicer formatting
			formatted=$(echo "$antennas" | sed 's/, \([^,]*\)$/ and \1/')
			echo "Antennas set to $formatted"
		fi
    else
        echo "No antennas selected"
    fi
}


#Calculate User 1 and User 2 RU params from phymode, BW, ruSize and ruLocation
util_calculate_ru_params()
{
	local phymode=$1
	local bw=$2
	local ruSize=$3
	local ruLocation=$4
	local numRUs
	local ruaStartIndex
	local userOnePs160Rua0
	local userOneRua17
	local userTwoPs160Rua0
	local userTwoRua17
	local userOne
	local userTwo

	# If phyMode is not ax(7) and not be(8) or if any one of ruSize and ruLocation is empty return config to disable RUs
	if [[ $phymode -ne 7 && $phymode -ne 8 ]] || [ "$ruSize" == "" ] || [ "$ruLocation" == "" ]; then
		echo "254 254" # This config will disable RUs
		exit 0
	fi

	case $ruSize in
		"26")
			numRUs=36
			ruaStartIndex=0
			;;
		"52")
			numRUs=16
			ruaStartIndex=37
			;;
		"106")
			numRUs=8
			ruaStartIndex=53
			;;
		"242")
			numRUs=4
			ruaStartIndex=61
			;;
		"484")
			numRUs=2
			ruaStartIndex=65
			;;
		"996")
			numRUs=1
			ruaStartIndex=67
			;;
		"0")
			echo "254 254" # This config will disable RUs
			exit 0
			;;
		*)
			printInfo "Unsupported/Invalid ruSize. Valid ruSize: 26, 52, 106, 242, 484, 996"
			exit 1
			;;
	esac

	case $bw in
		"0")
			numRUs=$(($numRUs / 4))
			;;
		"1")
			numRUs=$(($numRUs / 2))
			;;
		"2"|"3"|"4")
			if [ $ruSize -eq 26 ]; then
				numRUs=$((numRUs + 1)) # This increment is required as ruaIndex 18 is reserved
			fi
	esac

	if [ $numRUs -eq 0 ]; then
		printInfo "INVALID INPUT: RU larger than the configured BW"
		exit 1
	fi

	case $ruLocation in
		"Low")
			userOnePs160Rua0=0
			userOneRua17=$ruaStartIndex
			;;
		"Mid")
			userOnePs160Rua0=$((bw >> 2))

			if [ $bw -lt 3 ]; then
				userOneRua17=$(( ($ruaStartIndex + $ruaStartIndex + $numRUs - 1) / 2 ))
			else
				userOneRua17=$(( $ruaStartIndex + $numRUs - 1 ))
			fi

			# 18 is reserved. Hence use 17 as mid
			if [ $userOneRua17 -eq 18 ]; then
				userOneRua17=$(($userOneRua17 - 1))
			fi
			;;
		"High")
			if [ $bw -lt 2 ]; then
				userOnePs160Rua0=0
			else
				userOnePs160Rua0=$(( ((1 << ($bw - 2)) - 1) ))
			fi

			userOneRua17=$(( $ruaStartIndex + $numRUs - 1 ))
			;;
		"Edges")
			userOnePs160Rua0=0
			userOneRua17=$ruaStartIndex

			if [ $bw -lt 2 ]; then
				userTwoPs160Rua0=0
			else
				userTwoPs160Rua0=$(( ((1 << ($bw - 2)) - 1) ))
			fi

			userTwoRua17=$(( $ruaStartIndex + $numRUs - 1 ))
			;;
		*)
			printInfo "Invalid ruLocation. Valid ruLocation: Low, Mid, High, Edges"
			exit 1
			;;
	esac


	if [ "$ruLocation" != "Edges" ]; then
		userTwoRua17=$userOneRua17
		userTwoPs160Rua0=$userOnePs160Rua0
	fi

	userOne=$(( (($userOnePs160Rua0 >> 1 ) << 8) + ($userOneRua17 << 1) + ($userOnePs160Rua0 & 1) ))
	userTwo=$(( (($userTwoPs160Rua0 >> 1 ) << 8) + ($userTwoRua17 << 1) + ($userTwoPs160Rua0 & 1) ))

	echo "$userOne $userTwo"
}

# Handle exit signal. Clear up the process we started and unlock the critical section
util_handle_exit()
{
	util_cleanup_process
	util_exit_critical_section

	if [ $rx_per_endless_loop -eq 1 ]; then
		rx_per_endless_loop=0
		dut_read_rx_counters
	fi

	dut_stop_rx_measure
	exit
}

# Cleanup any pending processess running
util_cleanup_process()
{
	if [ -n $tailpid ]; then
		kill -9 $tailpid > /dev/null 2>&1
		tailpid=
	fi
}

# wait until the dut_cli process the command from another instance
util_enter_critical_section()
{
	exec 200>/var/lock/dut.lock
	flock -x 200 > /dev/null 2>&1
}

# Unlock critical section
util_exit_critical_section()
{
	flock -u 200 > /dev/null 2>&1
}

# Initialize trap signals
trap util_handle_exit EXIT
trap util_handle_exit INT
trap util_handle_exit SIGINT
trap util_handle_exit SIGTERM

util_wait_for_prompt()
{
	while true
	do
		tail -n1 $1 | grep  ">" > /dev/null
		if [ $? -eq 0 ]; then
			break;
		fi
	done
}

# Execute dut_cli command
# args: <Command string>
dut_send_command()
{
	local line_count

	echo "" > /tmp/dut_out$band
	# only save the curret DUT command logs
	line_count=$(wc -l < /tmp/dut_log)
	tail -n +$((line_count + 1)) -f -s0 /tmp/dut_log > /tmp/dut_out$band &
	tailpid=$!
	[ "$DUT_DEBUG_LOG" = true ] && echo "DUT_COMMAND: $@"

	util_enter_critical_section
	# send command
	echo $@ >> /tmp/dut_commands.txt

	while true
	do
		ret_msg=$(grep "command: $@" /tmp/dut_out$band)
		if [ $? -ne 0 ]; then
			continue
		fi

		echo $ret_msg | grep "OK" > /dev/null
		if [ $? -eq 0 ]; then
			[ "$DUT_DEBUG_LOG" = true ] && echo "Command succeeded"
			util_wait_for_prompt "/tmp/dut_out$band"
			break
		else
			[ "$DUT_DEBUG_LOG" = true ] && echo "Commmand failed"
			util_wait_for_prompt "/tmp/dut_out$band"
			# those commands might need to be continued for DUT band status checking
			# cannot exit even failed
			if echo "$@" | grep -q -e "driverInit" -e "driverRelease" -e "getBand"; then
				break
			fi
			util_exit_critical_section
			exit 1
		fi
	done

	# Cleanup running processes we started
	util_cleanup_process

	# print output
	if [ "$DUT_DEBUG_LOG" = true ]; then
		cat /tmp/dut_out$band
		echo
		echo
	fi

	# reset command file
	echo "" > /tmp/dut_commands.txt
	util_exit_critical_section
}

# Parse and print scan results
# args: util_parse_scanres <filename> <band_id>
util_parse_scanres()
{
	local band=$2
	local bssid
	local freq
	local channel
	local bw
	local vhtbw
	local rssi
	local ssid
	local opclass
	while IFS= read -r line; do
		if [[ "$line" = *"on wlan"* ]]; then
			bssid=$(echo $line | awk '{ print $2 }' | cut -d '(' -f 1)
			freq=
			channel=
			bw=
			vhtbw=
			rssi=
			ssid=
			opclass=
			echo
			printInfo "BSSID: $bssid"
			printInfo "MAC Address: $bssid"
			continue
		fi

		if [[ "$line" = *"freq:"* ]]; then
			freq=$(echo $line | awk '{ print $2 }')
			printInfo "Frequency: $freq"
			continue
		fi

		if [[ "$line" = *"primary channel:"* ]]; then
			channel=$(echo "$line" | awk '{ print $4 }')
			printInfo "Channel: $channel"
			continue
		fi

		if [[ "$line" = *"Primary Channel"* ]]; then
			channel=$(echo "$line" | awk '{ print $3 }')
			printInfo "Channel: $channel"
			continue
		fi

		if [[ "$line" = *"signal:"* ]]; then
			rssi=$(echo "$line" | awk '{ print $2 }')
			printInfo "RSSI: $rssi"
			continue
		fi

		if [[ "$line" = *"SSID:"* ]]; then
			ssid=$(echo "$line" | awk '{ print $2 }')
			printInfo "SSID: $ssid"
			continue
		fi

		if [[ "$line" = *"RSN:"* ]]; then
			printInfo "Encryption: "
			printInfo "$(echo "$line" | cut -d '*' -f 2)"
			for i in $(seq 1 3); do
				IFS= read -r next_line
				printInfo "$(echo "$next_line" | cut -d '*' -f 2)"
			done
			continue
		fi

		if [[ "$line" = *"current operating class:"* ]]; then
			opclass=$(echo "$line" | awk '{ print $5 }')
			bw=$(util_opclass_to_chwidth $opclass)
			printInfo "Bandwidth: $bw"
			continue
		fi


		if [ -n "$opclass" ]; then
			continue
		fi

		if [[ "$line" = *"secondary channel offset"* ]]; then
			if [[ "$line" = *"above"* || "$line" = *"below"* ]]; then
				bw="40"
			else
				bw="20"
			fi

			if [ $band -eq 0 ]; then
				printInfo "Bandwidth: $bw"
			fi
			continue
		fi

		if [ $band -eq 2 ]; then
			if [[ "$line" = *"Channel Width:"* ]]; then
				if [[ "$line" = *"80+80 MHz"* ]]; then
					vhtbw="80+80"
				elif [[ "$line" = *"160 MHz"* ]]; then
					vhtbw="160"
				fi
				continue
			fi

			if echo "$line" | grep "\*\ channel width:" >> /dev/null ; then
				if [[ "$vhtbw" != *"80+80"* && "$vhtbw" != *"160"* ]]; then
					if [[ "$line" = *"80 MHz"* ]]; then
						bw="80"
					fi
				else
					if [ -n "$vhtbw" ]; then
						bw="$vhtbw"
					fi
				fi
				printInfo "Bandwidth: $bw"
				continue
			fi
		fi

	done < "$1"
}

# Get BW string from opclass
util_opclass_to_chwidth()
{
	case $1 in
		81|82)
		echo "20"
		;;
	83|84)
		echo "40"
		;;
	115)
		echo "20"
		;;
	116|117)
		echo "40"
		;;
	118)
		echo "20"
		;;
	119|120)
		echo "40"
		;;
	121)
		echo "20"
		;;
	122|123)
		echo "40"
		;;
	124|125)
		echo "20"
		;;
	126|127)
		echo "40"
		;;
	128)
		echo "80"
		;;
	129)
		echo "160"
		;;
	130)
		echo "80+80"
		;;
	131)
		echo "20"
		;;
	132)
		echo "40"
		;;
	133)
		echo "80"
		;;
	134)
		echo "160"
		;;
	135)
		echo "80+80"
		;;
	136)
		echo "20"
		;;
	137)
		echo "320"
		;;
	180)
		echo "2160"
		;;
	181)
		echo "4320"
		;;
	182)
		echo "6480"
		;;
	183)
		echo "8640"
		;;
	*)
		echo "20"
		;;
	esac
}
