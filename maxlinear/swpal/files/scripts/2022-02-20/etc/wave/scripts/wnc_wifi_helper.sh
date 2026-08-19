#!/bin/sh
source /opt/intel/wave/scripts/wnc_wifi_util.sh

# MxL helper script to run dut_cli commands

# Globals
rx_measure_running=0
rx_per_endless_loop=0

# Run dut_cli and put the radios in DUT_MODE
# args: none
init_dut_mode()
{
	local dut_mode=$(util_check_dut_mode_status)

	# Check if dut_cli already running, if not run it and initialize all the bands to DUT mode
	if [ "$dut_mode" == "false" ]; then
		printInfo "Stopping AP Mode..."
		util_manage_pwhm "false"

		printInfo "Starting Test Mode..."
		util_manage_dut_mode "true"

		util_check_mem_info
		for band in 0 2 4; do
			echo "Initialize DUT band $band"
			dut_send_command "exec $band driverInit --memory-type $mem_type --memory-size $mem_size"
		done
	fi
}

release_dut_mode()
{
	local dut_mode=$(util_check_dut_mode_status)

	if [ "$dut_mode" == "true" ]; then
		printInfo "Stopping Test Mode..."
		util_manage_dut_mode "false"
	fi
}

dut_print_version()
{
	local wlan_version=$(grep 'wave_release_minor' /etc/wave_components.ver | cut -d '"' -f 2 | xargs)
	local progmodel_version=$(grep 'wave700B_progmodel_ver' /etc/wave_components.ver | cut -d '"' -f 2 | xargs)
	local psd_version=$(grep 'wave700B_progmodel_ver' /etc/wave_components.ver | cut -d '"' -f 2 | xargs)
	local regulatory_version=$(grep 'wave_regulatory_ver' /etc/wave_components.ver | cut -d '"' -f 2 | xargs)
	local eeprom_version=$(grep 'EEPROM version' /proc/net/mtlk/wlan0/eeprom_parsed | cut -d ':' -f 2 | xargs)
	local hw_id=$(grep 'HW ID' /proc/net/mtlk/wlan0/eeprom_parsed | cut -d ':' -f 2 | xargs)
	local hw_type=$(grep 'HW type' /proc/net/mtlk/wlan0/eeprom_parsed | cut -d ':' -f 2 | xargs)
	local hw_revision=$(grep 'HW revision' /proc/net/mtlk/wlan0/eeprom_parsed | cut -d ':' -f 2 | xargs)

	printInfo "================================="
	printInfo "MXL Helper script version $version"
	printInfo "================================="
	printInfo "WLAN version: $wlan_version"
	printInfo "================================="
	printInfo "PROGMODEL version: $progmodel_version"
	printInfo "================================="
	printInfo "PSD version: $psd_version"
	printInfo "================================="
	printInfo "REGULATORY version: $regulatory_version"
	printInfo "================================="
	printInfo "EEPROM version: $eeprom_version"
	printInfo "================================="
	printInfo "HW ID: $hw_id"
	printInfo "================================="
	printInfo "HW type: $hw_type"
	printInfo "================================="
	printInfo "HW revision: $hw_revision"
	printInfo "================================="
}

#Helper function to Wi-Fi radio up
dut_wifi_radio_up()
{
	util_manage_wifi_radio "true" $@
}

#Helper function to Wi-Fi radio down
dut_wifi_radio_down()
{
	util_manage_wifi_radio "false" $@
}

#Helper function to terminate Wi-Fi driver
dut_wifi_terminate()
{
	local dut_mode=$(util_check_dut_mode_status)

	release_dut_mode

	local wifi_drv_status=$(util_check_wifi_driver_status)
	if [ "$wifi_drv_status" == "false" ]; then
		printInfo "Wifi Driver is already uninstalled"
		return
	else
		rmmod mtlk
		wifi_drv_status=$(util_check_wifi_driver_status)
	fi

	if [ "$wifi_drv_status" == "false" ]; then
		printInfo "Wifi Driver is uninstalled"
	fi
}

#Helper function to init Wi-Fi
dut_wifi_init()
{
	local dut_mode=$(util_check_dut_mode_status)
	local wifi_drv_status=$(util_check_wifi_driver_status)

	if [ "$wifi_drv_status" == "false" ]; then
		util_init_wifi_driver
		wifi_drv_status=$(util_check_wifi_driver_status)
	fi

	if [ "$wifi_drv_status" == "true" ]; then
		printInfo "Wifi Driver is up and Ready"
		if [ "$dut_mode" == "false" ]; then
			util_check_radio_status "true"
		else
			local band
			for band in 0 2 4; do
				util_check_dut_band_status $band
			done
		fi
	else
		printInfo "ERROR: Wifi Driver is not running"
	fi

	dut_print_version

	printInfo "================================="

	if [ "$dut_mode" == "true" ]; then
		printInfo "Operation Mode : Test Mode"
	else
		printInfo "Operation Mode : AP Mode"
	fi

	printInfo "================================="
}

# Helper func to start Tx
# args: <radio band> <channel> <bandwidth> <power> <antenna selection> <number of spatial streams (NSS)>
#					<Interframe Space ( IFS )> <PSDU Size>  <Protocol> <MCS Index> <Number of Frames to Send> [<RU size> <RU location>]
dut_start_tx()
{
	# Check number of parameters are enough
	if [ "$#" -lt 11 ]; then
		echo "Illegal number of parameters"
	fi

	# Initialize dut mode
	init_dut_mode

	local band
	local lowestchannel
	local bw
	local power
	local ant_mask
	local nss=$6
	local ifs=$7
	local psdulen=$8
	local phymode=$9
	local mcs=${10}
	local repetition=${11}
	local rusize=${12}
	local rulocation=${13}
	local isfreq # Flag to indicate if channel is a frequency
	local centerchannel
	local num_antenna
	local gi=0
	local ltf=1
	local ruParams
	local userOneRu
	local userTwoRu

	band=$(util_extract_band $1) || { echo "$band"; exit 1; }
	phymode=$(util_extract_phymode $9 $band) || { echo "$phymode"; exit 1; }
	isfreq=$(util_check_if_frequency $2)

	if [ "$isfreq" -eq 1 ]; then
		util_validate_frequency $band $2
		# Convert center frequency to center channel
		centerchannel=$(util_convert_centerfreq_to_centerchannel $band $2) || { echo "$centerchannel"; exit 1; }
	else
		# If it is channel number, no conversion needed
		centerchannel=$2
	fi

	#extract bandwidth
	bw=$(util_extract_bw $3 $centerchannel $band) || { echo "$bw"; exit 1; }
	if [ "$3" != "-" ]; then
		#validate center channel and bandwidth if bw option is not hypen(-)
		util_validate_center_channel_and_bandwidth $centerchannel $bw $band $phymode
	fi

	if [[ $9 == "n" || $9 == "ac" ]]; then
		gi=0 # 0.8us
		ltf=0 # short
	elif [[ $9 == "ax" || $9 == "be" ]]; then
		gi=3 # 3.2us
		ltf=2 # long
	fi

	if [ $ifs -lt 10 ]; then
		printInfo "WARNING: IFS: $ifs < 10 (minimum allowed value is 10 us). Use 10 us instead"
		ifs=10
	fi

	lowestchannel=$(util_calculate_lowest_channel $band $bw $centerchannel) || { echo "$lowestchannel"; exit 1; }
	power=$(util_extract_powerval $4) || { echo "$power"; exit 1; }
	ant_mask=$(util_check_ant_mask $5) || { echo "$ant_mask"; exit 1; }
	mcs=$(util_get_mcs_value $mcs $phymode) || { echo "$mcs"; exit 1; }
	ruParams=$(util_calculate_ru_params $phymode $bw $rusize $rulocation) || { echo "$ruParams"; exit 1; }

	userOneRu=$(echo $ruParams | awk '{print $1}')
	userTwoRu=$(echo $ruParams | awk '{print $2}')

	# If the numer of antenna is more than the number of SS then set nss to 1.
	num_antenna=$(util_count_bits_set $ant_mask)
	if [ $(awk -v antval=$num_antenna -v nssval=$nss 'BEGIN { print (antval > nssval)}') -eq 1 ]; then
		printInfo "Num Antenna is more then num nss"
		nss=1
	fi

	if [ $(awk -v antval=$num_antenna -v nssval=$nss 'BEGIN { print (antval < nssval)}') -eq 1 ]; then
		printInfo "WARNING: Number of antenna is less than number of spatial streams configured"
	fi

	if [ $repetition -eq 0 ]; then
		repetition=65535	# Continuous transmission
	fi

	printInfo "channel is $1GHz[$2/$3]"
	printInfo "Confirmed band: $1G"
	printInfo "Confirmed protocol:$9"
	printInfo "The protocol[$9] supports up to BW[$(util_check_max_bandwidth $band $phymode)]"
	printInfo "Confirmed bandwidth:$3"
	printInfo "$(util_print_antennas $ant_mask)"
	printInfo "Confirmed antsel:$ant_mask"
	printInfo "The protocol[$9] supports up to $(util_check_max_nss $phymode)[NSS]"
	printInfo "The ant_sel[$ant_mask] supports up to $num_antenna[NSS]"
	printInfo "Confirmed nss:$nss"
	printInfo "Confirmed power:$(awk "BEGIN { printf \"%.1f\", $power / 2 }") dBm"
	printInfo "Confirmed mcs:$mcs index/rate"
	printInfo "Confirmed ifs:$ifs us"
	printInfo "Confirmed psdu size:$psdulen byte"
	printInfo "Confirmed RU size:${rusize:-no}"
	printInfo "Confirmed RU location:${rulocation:-no}"

	dut_send_command "exec $band setChannel --bandwidth $bw --channel $lowestchannel --phy-mode $phymode"
	dut_send_command "exec $band setRate --bandwidth $bw --mcs $mcs --nss $nss --gi $gi --ltf $ltf"
	dut_send_command "exec $band setTransmitPowerLevel --power-level $power"
	dut_send_command "exec $band setEnabledTxAntennaMask --antenna-mask $ant_mask"
	dut_send_command "exec $band setIfs --ifs $ifs"
	dut_send_command "exec $band setRuParams --user-one $userOneRu --user-two $userTwoRu"
	dut_send_command "exec $band startTx --repetitions $repetition --packet-length $psdulen"
}

# Stop Tx Mode
# args: <radio band>
dut_stop_tx()
{
	local band

	# Initialize dut mode
	init_dut_mode

	if [ "$#" -lt 1 ]; then
		for band in 0 2 4; do
			dut_send_command "exec $band stopTx"
			printInfo "TX Mode stopped for $band"
		done
		exit 0
	fi

	band=$(util_extract_band $1) || { echo "$band"; exit 1; }

	dut_send_command "exec $band stopTx"
}


# DUT start CW transmission
# args: <radio band> <channel> <antennas> <power> <offset>
dut_start_cw()
{
	if [ "$#" -lt 4 ]; then
		echo "Illegal number of parameters"
	fi

	# Initialize dut mode
	init_dut_mode

	local band
	local lowestchannel
	local ant_mask=$3
	local power=$4
	local offset=$5		#Note: not used for now
	local isfreq # Flag to indicate if channel is a frequency
	local centerchannel
	local bw="-" #Bandwidth (BW) is not specified. The appropriate bandwidth will be determined based on the center channel by passing a hyphen for the bandwidth.

	band=$(util_extract_band $1) || { echo "$band"; exit 1; }
	isfreq=$(util_check_if_frequency $2)

	if [ "$isfreq" -eq 1 ]; then
		util_validate_frequency $band $2
		# Convert center frequency to center channel
		centerchannel=$(util_convert_centerfreq_to_centerchannel $band $2) || { echo "$centerchannel"; exit 1; }
	else
		# If it is channel number, no conversion needed
		centerchannel=$2
	fi

	bw=$(util_extract_bw $bw $centerchannel $band) || { echo "$bw"; exit 1; }
	lowestchannel=$(util_calculate_lowest_channel $band $bw $centerchannel) || { echo "$lowestchannel"; exit 1; }
	power=$(util_extract_powerval $4) || { echo "$power"; exit 1; }
	ant_mask=$(util_check_ant_mask $3) || { echo "$ant_mask"; exit 1; }

	printInfo "channel is $1GHz[$2/$3]"
	printInfo "$(util_print_antennas $ant_mask)"
	printInfo "Confirmed antsel:$ant_mask"
	printInfo "Confirmed offset:$(awk 'BEGIN {print 312.5 * '$offset'}') kHz"
	dut_send_command "exec $band setChannel --bandwidth $bw --channel $lowestchannel"
	dut_send_command "exec $band setEnabledTxAntennaMask --antenna-mask $ant_mask"
	dut_send_command "exec $band setTransmitPowerLevel --power-level $power"
	printInfo "The minimum offset supported value is 312.5 kHz. The offset is determined by multiplying 312.5 kHz
	by the given offset value $offset.The final offset frequency is $(awk 'BEGIN {print 312.5 * '$offset'}') kHz."
	dut_send_command "exec $band startCw --tone $offset"
	printInfo "[$1G][AP mode] CW test started"
}

# Stop CW wave transmission
# args: <radio band>
dut_stop_cw()
{
	local band

	# Initialize dut mode
	init_dut_mode

	if [ "$#" -lt 1 ]; then
		for band in 0 2 4; do
			dut_send_command "exec $band stopCw"
			printInfo "CW stopped for band $band"
		done
		exit 0
	fi

	band=$(util_extract_band $1) || { echo "$band"; exit 1; }

	dut_send_command "exec $band stopCw"
}

dut_read_rx_counters()
{
	local per
	local good_frame
	local bad_frame
	local drop_frame

	rx_per_endless_loop=0
	# stopping reception
	dut_send_command "exec $band stopRxPer --calc-per"
	calc_per=$(grep "packetErrorRate" /tmp/dut_out$band)
	dut_send_command "exec $band getRxRateInfo"

	if [ -n "$calc_per" ]; then
		# Extract values using awk and save them to variables
		per=$(echo "$calc_per" | awk -F'[;]' '{print $1}' | awk -F'[:]' '{print $2}')
		good_frame=$(echo "$calc_per" | awk -F'[;]' '{print $2}' | awk -F'[:]' '{print $2}')
		bad_frame=$(echo "$calc_per" | awk -F'[;]' '{print $3}' | awk -F'[:]' '{print $2}')
		drop_frame=$(echo "$calc_per" | awk -F'[;]' '{print $4}' | awk -F'[:]' '{print $2}')
		total_frame=$((good_frame + bad_frame))

		printInfo "[$(util_convert_band_to_radio $band)G][AP mode] RX PER test stopped."
		printInfo "PER:	$(printf "%.1f" "$per")%"
		printInfo "Total frames:	$total_frame"
		printInfo "Good frames:	$good_frame"
		printInfo "Bad frames:	$bad_frame"
		printInfo "Dropped frames:	$drop_frame"
	fi
}

# Start Rx mode and calculate Rx PER.
# args: <radio band> <channel> <bandwidth> <Antenna Selection> <Protocol>
# <Number of Frames to Receive> <Idle Duration> <Destination MAC Address>
dut_calculate_per()
{
	if [ "$#" -lt 7 ]; then
		echo "Illegal number of parameters"
	fi

	# Initialize dut mode
	init_dut_mode

	local band
	local lowestchannel
	local bw
	local ant_mask
	local phymode
	local packet_count=$6	# Number of packets to receive
	local duration=$7			# Idle duration (millisec) to stay in reception
	local isfreq # Flag to indicate if channel is a frequency
	local centerchannel
	local num_antenna
	local input

	band=$(util_extract_band $1) || { echo "$band"; exit 1; }
	phymode=$(util_extract_phymode $5 $band) || { echo "$phymode"; exit 1; }
	isfreq=$(util_check_if_frequency $2)

	if [ "$isfreq" -eq 1 ]; then
		util_validate_frequency $band $2
		# Convert center frequency to center channel
		centerchannel=$(util_convert_centerfreq_to_centerchannel $band $2) || { echo "$centerchannel"; exit 1; }
	else
		# If it is channel number, no conversion needed
		centerchannel=$2
	fi

	#extract bandwidth
	bw=$(util_extract_bw $3 $centerchannel $band) || { echo "$bw"; exit 1; }
	if [ "$3" != "-" ]; then
		#validate center channel and bandwidth if bw option is not hypen(-)
		util_validate_center_channel_and_bandwidth $centerchannel $bw $band $phymode
	fi

	lowestchannel=$(util_calculate_lowest_channel $band $bw $centerchannel) || { echo "$lowestchannel"; exit 1; }
	ant_mask=$(util_check_ant_mask $4) || { echo "$ant_mask"; exit 1; }

	if [ -z $7 ] || [ "$7" == "-" ]; then
		duration=0
	fi

	# If the numer of antenna is more than the number of SS then set nss to 1.
	num_antenna=$(util_count_bits_set $ant_mask)
	printInfo "channel is $1GHz[$2/$3]"
	printInfo "Confirmed band: $1G"
	printInfo "Confirmed protocol:$5"
	printInfo "The protocol[$5] supports up to BW[$(util_check_max_bandwidth $band $phymode)]"
	printInfo "Confirmed bandwidth:$3"
	printInfo "$(util_print_antennas $ant_mask)"
	printInfo "Confirmed antsel:$ant_mask"
	printInfo "Confirmed number of receive frame:$packet_count"
	printInfo "Confirmed idle duration :$duration"
	printInfo "rxPER Started on channel[AP mode]:[$2] bandwidth:[$3] ant sel:[$ant_mask] protocol:[$5] Number of Frame to Receive:[$packet_count] idle Duration:[$duration]"
	printInfo "When the transmission is complete, press enter key to calculate the PER..."

	dut_send_command "exec $band setChannel --bandwidth $bw --channel $lowestchannel --phy-mode $phymode"
	dut_send_command "exec $band setEnabledTxAntennaMask --antenna-mask $ant_mask"
	dut_send_command "exec $band setEnabledRxAntennaMask --antenna-mask $ant_mask"

	# Set Rx packet limits
	dut_send_command "exec $band startRxPer --packet-limit $packet_count --duration-limit $duration"

	# If the duration is 0ms then we need to receive endlessly
	if [ $duration -eq 0 ]; then
		rx_per_endless_loop=1
		echo "==After packet transmit, enter key to calculate the PER=="
		while true; do
			read -r input
			if [ -z "$input" ]; then
				break
			fi
		done
		util_handle_exit
	else
		# Sleep for given duration in ms
		sleep $(awk -v var="$duration" 'BEGIN{print var * 0.001}')
		dut_read_rx_counters
	fi
}

# Run Rx Measure
# args: <radio band> <channel> <bandwidth> <Antenna Selection> <Protocol>
# <Destination MAC Address> <Number of captures> <capture interval>
dut_rx_measure()
{
	if [ "$#" -lt 8 ]; then
		echo "Illegal number of parameters"
	fi

	# Initialize dut mode
	init_dut_mode

	local band
	local centerchannel
	local lowestchannel
	local bw
	local ant_mask
	local phymode
	local isfreq # Flag to indicate if channel is a frequency
	local num_captures
	local interval
	local num_antenna
	local input

	band=$(util_extract_band $1) || { echo "$band"; exit 1; }
	phymode=$(util_extract_phymode $5 $band) || { echo "$phymode"; exit 1; }
	isfreq=$(util_check_if_frequency $2)

	if [ "$isfreq" -eq 1 ]; then
		util_validate_frequency $band $2
		# Convert center frequency to center channel
		centerchannel=$(util_convert_centerfreq_to_centerchannel $band $2) || { echo "$centerchannel"; exit 1; }
	else
		# If it is channel number, no conversion needed
		centerchannel=$2
	fi

	#extract bandwidth
	bw=$(util_extract_bw $3 $centerchannel $band) || { echo "$bw"; exit 1; }
	if [ "$3" != "-" ]; then
		#validate center channel and bandwidth if bw option is not hypen(-)
		util_validate_center_channel_and_bandwidth $centerchannel $bw $band $phymode
	fi

	lowestchannel=$(util_calculate_lowest_channel $band $bw $centerchannel) || { echo "$lowestchannel"; exit 1; }
	ant_mask=$(util_check_ant_mask $4) || { echo "$ant_mask"; exit 1; }
	num_captures=$7
	interval=$8

	# If the numer of antenna is more than the number of SS then set nss to 1.
	num_antenna=$(util_count_bits_set $ant_mask)
	printInfo "channel is $1GHz[$2/$3]"
	printInfo "Confirmed band: $1G"
	printInfo "Confirmed protocol:$5"
	printInfo "The protocol[$5] supports up to BW[$(util_check_max_bandwidth $band $phymode)]"
	printInfo "Confirmed bandwidth:$3"
	printInfo "$(util_print_antennas $ant_mask)"
	printInfo "Confirmed antsel:$ant_mask"
	printInfo "Confirmed number of receive frame:$num_captures"
	printInfo "Confirmed idle duration:$interval"

	# Start Rx measure handler process to listen for events
	rx_measure_filename="/tmp/rx_measure$band.csv"
	rm $rx_measure_filename > /dev/null 2>&1
	rx_measure_handler $band &
	handler_pid=$!
	rx_measure_running=1
	printInfo "Started listening for Rx Measure events. Output file name: $rx_measure_filename"
	printInfo "Testing in Progress...."

	dut_send_command "exec $band setChannel --bandwidth $bw --channel $lowestchannel --phy-mode $phymode"
	dut_send_command "exec $band setEnabledTxAntennaMask --antenna-mask $ant_mask"
	dut_send_command "exec $band setEnabledRxAntennaMask --antenna-mask $ant_mask"

	# Send Rx Measure command
	dut_send_command "exec $band rxMeasure --num-captures $num_captures --interval $interval"

	# wait or press "enter" for completion of event handler
	printInfo "press enter key to get the result"
	echo
	echo "==After Tx power transmit, enter key to get the results=="
	while true; do
		if read -t 1 -r input; then
			if [ -z "$input" ]; then
				break
			fi
		fi
		if ! kill -0 $handler_pid 2>/dev/null; then
			break
		fi
	done
	echo

	dut_stop_rx_measure
}

dut_stop_rx_measure()
{
	if [ $rx_measure_running -eq 1 ]; then
		rx_measure_running=0
		ps -p $handler_pid > /dev/null
		if [ $? -eq 0 ]; then
			kill -SIGINT $handler_pid > /dev/null
			handler_pid=
		fi

		# Disable Rx Measure
		dut_send_command "exec $band getRxRateInfo"
		dut_send_command "exec $band rxMeasure --disable"

		if [ ! -f $rx_measure_filename ]; then
			printInfo "No $rx_measure_filename found"
			printInfo "[$(util_convert_band_to_radio $band)G] rxMeasure test stopped."
			exit 1
		fi

		# Extract RX measurement result from csv file
		# Get the last line
		last_line=$(tail -n 1 "$rx_measure_filename")

		# Check if the last line is the header
		if echo "$last_line" | grep -q "ANT0_RSSI"; then
			printInfo "No packet data available in the CSV file."
			exit 0
		fi

		# Convert CSV to space-separated values
		set -- $(echo "$last_line" | tr ',' ' ')

		i=1
		count=0
		sum_rssi=0
		sum_rcpi=0
		sum_noise=0
		sum_evm=0

		while [ $i -le 16 ]; do
			eval rssi=\${$i}
			eval rcpi=\${$(($i + 1))}
			eval noise=\${$(($i + 2))}
			eval evm=\${$(($i + 3))}

			rssi_int=$(printf "%.0f" "$rssi")
			rcpi_int=$(printf "%.0f" "$rcpi")
			noise_int=$(printf "%.0f" "$noise")
			evm_int=$(printf "%.0f" "$evm")

			if [ "$rssi_int" != "0" ] && [ "$rssi_int" != "-128" ]; then
				# convert from dBm to mw
				rssi_mw=$(awk -v dbm="$rssi_int" 'BEGIN { printf "%.15e\n", 10^(dbm / 10) }')
				rcpi_mw=$(awk -v dbm="$rcpi_int" 'BEGIN { printf "%.15e\n", 10^(dbm / 10) }')
				noise_mw=$(awk -v dbm="$noise_int" 'BEGIN { printf "%.15e\n", 10^(dbm / 10) }')
				evm_mw=$(awk -v dbm="$evm_int" 'BEGIN { printf "%.15e\n", 10^(dbm / 10) }')

				sum_rssi=$(awk -v a="$sum_rssi" -v b="$rssi_mw" 'BEGIN {printf "%.15f\n", a + b}')
				sum_rcpi=$(awk -v a="$sum_rcpi" -v b="$rcpi_mw" 'BEGIN {printf "%.15f\n", a + b}')
				sum_noise=$(awk -v a="$sum_noise" -v b="$noise_mw" 'BEGIN {printf "%.15f\n", a + b}')
				sum_evm=$(awk -v a="$sum_evm" -v b="$evm_mw" 'BEGIN {printf "%.15f\n", a + b}')

				count=$((count + 1))
			fi

			i=$((i + 4))
		done

		if [ "$count" -gt 0 ]; then
			# calculate average in mw
			avg_rssi_mw=$(awk -v a="$sum_rssi" -v b="$count" 'BEGIN {printf "%.15f\n", a / b}')
			avg_rcpi_mw=$(awk -v a="$sum_rcpi" -v b="$count" 'BEGIN {printf "%.15f\n", a / b}')
			avg_noise_mw=$(awk -v a="$sum_noise" -v b="$count" 'BEGIN {printf "%.15f\n", a / b}')
			avg_evm_mw=$(awk -v a="$sum_evm" -v b="$count" 'BEGIN {printf "%.15f\n", a / b}')

			# convert from mw to dBm
			log10() { awk -v x=$1 'BEGIN { print log(x)/log(10) }'; }
			avg_rssi=$(log10 $avg_rssi_mw | awk '{printf "%d", 10 * $1}')
			avg_rcpi=$(log10 $avg_rcpi_mw | awk '{printf "%d", 10 * $1}')
			avg_noise=$(log10 $avg_noise_mw | awk '{printf "%d", 10 * $1}')
			avg_evm=$(log10 $avg_evm_mw | awk '{printf "%d", 10 * $1}')

			printInfo "RSSI        : $avg_rssi"
			printInfo "RCPI        : $avg_rcpi"
			printInfo "Noise level : $avg_noise"
			printInfo "EVM         : $avg_evm"
		else
			printInfo "No valid antennas found in the last row."
		fi

		printInfo "[$(util_convert_band_to_radio $band)G] rxMeasure test stopped."
	fi
}

dut_print_history()
{
	# Changelog
	printInfo "12-Jun-2025 - v1.2"
	printInfo "- Fix 6G test mode init issue"
	printInfo "- Fix test mode crash issue (by skipping pwhm/wld/hostapd processes in test mode)"
	printInfo "- Added default IFS value of 10us for txMode"
	printInfo "- Added support to stop rxMeasure/rxPER by pressing the enter key"
	printInfo "- Modified output format of txMode, rxPER, rxMeasure, and cwwave commands"
	printInfo

	printInfo "11-Mar-2025 - v1.1"
	printInfo "- Fix Rx Measure whole duration (num capture x interval) minimum value limitation"
	printInfo "- Added support for CW offset parameter"
	printInfo "- Fix Rx PER MCS values displayed for 11n to show as MCS 0 to 7 for respective NSS"
	printInfo "- Added support for RU parameters in Tx mode command"
	printInfo "- Added changes to throw error for invalid channel, phymode, BW combinations"
	printInfo "- Added support for APscan and AP Status commands"
	printInfo "- Added WARNING print when number of Antenna is less than NSS"
	printInfo

	printInfo "01-Mar-2025 - v1.0"
	printInfo "- Added support for init"
	printInfo "- Added suppport for version"
	printInfo "- Added support for txMode and stoptx"
	printInfo "- Added support for startcw and stopcw commands"
	printInfo "- Added support for Rx PER"
	printInfo "- Added support for Rx Measure"
	printInfo
}

# Run scan for all channels in band
# args: [radio band]
dut_start_ap_scan()
{
	local band
	local scanfile
	local start_time
	local end_time
	local duration
	local band_ids

	if [ "$#" -lt 1 ]; then
		band_ids="0 2 4"
	else
		band=$(util_extract_band $1) || { echo "$band"; exit 1; }
		band_ids="$band"
	fi

	for band in $band_ids; do
		# File to store scan results
		scanfile="/tmp/scanres$band.txt"
		printInfo "Scanning band $band"

		# Record start time
		start_time=$(date +%s)

		# Start scan
		iw dev wlan$band scan ap-force | grep -E 'on wlan|freq:|signal:|primary channel:|Primary Channel|SSID:|channel offset:|current operating class:|channel width:|Channel Width:|RSN:|Group cipher:|Pairwise ciphers:|Authentication suites:' > "$scanfile"

		# Record scan complete time and calculate duration
		end_time=$(date +%s)
		duration=$((end_time - start_time))

		# Parse scan results and print
		util_parse_scanres "$scanfile" $band
		printInfo "Scan Duration: $duration seconds"
	done
}

# Dump radio status with peer stats connected to first vap in the radio
# args: [band_id]
dut_print_radio_status()
{
	local band
	local stalist
	local sta_mac
	local state
	local bssinfo
	local channel

	if [ "$#" -lt 1 ]; then
		band_ids="0 2 4"
	else
		band=$(util_extract_band $1) || { echo "$band"; exit 1; }
		band_ids="$band"
	fi

	for band in $band_ids; do
		printInfo "Radio$band Status:"
		state=$(cat /sys/class/net/wlan$band.1/operstate)
		if [ "$state" != "up" ]; then
			printInfo "Interface wlan$band.1 is not UP"
			continue
		fi

		bssinfo=$(iw dev wlan$band.1 info)
		ap_ssid=$(echo $bssinfo | awk -F "ssid" '{print $2}' | awk '{print $1}')
		channel=$(echo $bssinfo | awk -F "channel" '{print $2}' | awk '{print $1}')

		stalist=$(dwpal_cli wlan$band.1 peerlist)
		stalist=$(echo $stalist | awk -F'connected: ' '{print $2}')

		for sta_mac in $stalist; do
			local bw
			local ratesinfo
			local flowstatus
			local phyrxstatus
			local sta_bandwidth
			local sta_rssi
			local sta_rssi_ant1
			local sta_rssi_ant2
			local sta_rssi_ant3
			local sta_rssi_ant4
			local sta_rcpi
			local sta_rcpi_ant1
			local sta_rcpi_ant2
			local sta_rcpi_ant3
			local sta_rcpi_ant4

			ratesinfo=$(dwpal_cli wlan$band.1 peerratesinfo $sta_mac)
			flowstatus=$(dwpal_cli wlan$band.1 peerflowstatus $sta_mac)
			phyrxstatus=$(dwpal_cli wlan$band.1 peerphyrxstatus $sta_mac)
			sta_bandwidth=$(echo $ratesinfo | awk -F "Data uplink rate info" '{print $2}' | awk -F "BW index" '{print $2}' | awk '{print $1}')
			sta_rssi=$(echo $flowstatus | awk -F "Average RSSI of all antennas" '{print $2}' | awk '{print $2}')

			sta_rssi_ant1=$(echo $flowstatus | awk -F "Short-term RSSI average per antenna" '{print $2}' | awk '{print $2}')
			sta_rssi_ant2=$(echo $flowstatus | awk -F "Short-term RSSI average per antenna" '{print $2}' | awk '{print $5}')
			sta_rssi_ant3=$(echo $flowstatus | awk -F "Short-term RSSI average per antenna" '{print $2}' | awk '{print $8}')
			sta_rssi_ant4=$(echo $flowstatus | awk -F "Short-term RSSI average per antenna" '{print $2}' | awk '{print $11}')

			sta_rcpi=$(echo $phyrxstatus | awk -F " : rcpi_avg" '{print $1}' | awk '{print $NF}')
			sta_rcpi_ant1=$(echo $phyrxstatus | awk -F "rcpi " '{print $2}' | awk '{print $1}')
			sta_rcpi_ant2=$(echo $phyrxstatus | awk -F "rcpi " '{print $2}' | awk '{print $4}')
			sta_rcpi_ant3=$(echo $phyrxstatus | awk -F "rcpi " '{print $2}' | awk '{print $7}')
			sta_rcpi_ant4=$(echo $phyrxstatus | awk -F "rcpi " '{print $2}' | awk '{print $10}')

			printInfo "STA MAC:     $sta_mac"
			printInfo "AP SSID:     $ap_ssid"
			printInfo "Channel:     $channel"
			printInfo "Bandwidth:   $sta_bandwidth MHz"
			printInfo "RSSI global: $sta_rssi dBm"
			printInfo "RCPI global: $sta_rcpi dBm"
			printInfo "RSSI[ant 1]: $sta_rssi_ant1 dBm"
			printInfo "RCPI[ant 1]: $sta_rcpi_ant1 dBm"
			printInfo "RSSI[ant 2]: $sta_rssi_ant2 dBm"
			printInfo "RCPI[ant 2]: $sta_rcpi_ant2 dBm"
			printInfo "RSSI[ant 3]: $sta_rssi_ant3 dBm"
			printInfo "RCPI[ant 3]: $sta_rcpi_ant3 dBm"
			printInfo "RSSI[ant 4]: $sta_rssi_ant4 dBm"
			printInfo "RCPI[ant 4]: $sta_rcpi_ant4 dBm"
			echo
		done
	done
}
