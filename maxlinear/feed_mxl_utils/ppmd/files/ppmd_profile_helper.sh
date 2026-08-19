#!/bin/sh

#select single device or all devices
sel_pm=$1
#enable/disable power saving feature(s) or force to low power mode
level_pm=$2
#enable/disable power saving feature(s) or force to low power mode
opt_pm=$3
#SoC ID
soc_id="UNKNOWN"
############################################################

############################################################
# function definition
############################################################
help_pm () {
	echo ""
	echo "Help Power Control:"
	echo "Detected SoC: "$soc_id
	echo "First parameter defines the device, which is treated."
	echo "registered parameters are:"
	echo "PowerStatus                                       all"
	echo "                                                     "
	echo "Power Profile                                     profile"
	echo "            2nd parameter:                        help"
	echo "                                                     "
	echo "WLAN:                                             wlan"
	echo "                                                  wlan0"
	echo "                                                  wlan2"
	echo "                                                  wlan4"
	echo "DSL:                                              dsl0"
	echo "                                                  dsl1"
	echo "ETHernet/GPHY:                                    eth0_2"
	echo "                                                  eth0_3"
	echo "                                                  eth0_4"
	echo "                                                  eth0_5"
	echo "                                               4: LP mode disable"
	echo "                                               5: LP mode enable"
	echo ""
	echo "PCIE:                                             pcie"
	echo ""
	echo "ADP:                                              adp"
	echo "            2nd parameter:                        0=SW_DFVS"
	echo "                               3rd parameter:         0=HIGH, 1=MED, 2=LOW, 3=ppmd start"
	echo "                                                  1=HW_DFS"
	echo "                               3rd parameter:         0=off; 1=on"
	echo ""
	echo "CPU:                                            cpu"
	echo "            2nd parameter:                        0=DVFS off"
	echo "                                                  1=DVFS on"
	echo "                                                  2=C6 off"
	echo "                                                  3=C6 on"
	echo "                                                  4=pstate passive"
	echo "                               3rd parameter:         cpu frequency"
	echo "                                                  5=pstate active"
	echo "                                                  6=offline cpu's"
	echo "                                                  7=online cpu's"
	echo ""
	echo "DDR Self Refresh                                  ddr_sr"
	echo ""
	echo "TOP NOC Frequency                                 top_noc"
	echo "                                    parameters        0x3 (600), 0xE (50)"
	echo "                                                      on/off -> dynamic"
	echo ""
	echo "GSWIP Frequency                                   gswip"
	echo "                                    parameters        0x2 (800), 0x3 (600), 0xE (50)"
	echo ""
	echo ""
	echo "Second parameter defines required action."
	echo "registered paramters are:"
	echo "disable power saving feature(s):                  0"
	echo "enable power saving feature(s):                   1"
	echo "disable device -> low power mode:                 2"
	echo "enable device -> operational mode:                3"
	echo ""
	echo "examples:"
	echo "ppmd_profile_helper wlan2 1    enables powersaving mode (AutoCoCMode) for wlan2"
	echo "ppmd_profile_helper eth0_4 2   puts eth0_4 into low power mode"
	echo "ppmd_profile_helper pcie 1     enable ASPM mode"
	echo "ppmd_profile_helper adp_dvfs 2 set ADP_DVFS level to LOW"
	echo ""
}
############################################################


############################################################
# wlan power saving feature(s)
all_wlan_pm () {
	i=0
	found=0
	for i in 0 2 4; do
		wlan_pm $i
	done
}
############################################################

############################################################
# check the antenna configuration for tx and rx
# $1=wlan id
radio_antenna_check () {
	wlan_rx=`uci show wireless.radio$1.num_antennas | awk -F"'" '{print $2}'`
	wlan_tx=$wlan_rx
	if [ "$1" = "2" ]; then # 5GHz DFS check
		radio6=`uci show wireless.radio6.num_antennas | awk -F"'" '{print $2}'`
		wlan_rx=`expr $wlan_rx + $radio6`
	fi
}
############################################################

############################################################
# check for WAV6 or WAV7
# checking PCI Device ID
which_wav_check () {
	wav_id=0
	if [ ! -f /tmp/pcie_rc_device.txt ]; then
		ls -1 /sys/bus/pci/devices/ | grep ....:00:00.0 1> /tmp/pcie_rc_device.txt 2> /dev/null
	fi
	if [ ! -f /tmp/ppmd/wave_ver.txt ]; then
		i=1
		num=`grep -c ....:00:00.0 /tmp/pcie_rc_device.txt`
		while [ "$i" -le "$num" ]; do
			domain=`awk -F":" 'NR == '$i' {print $1}' /tmp/pcie_rc_device.txt`
			ls /sys/bus/pci/devices/$domain:01:00.0/vendor 1> /dev/null 2> /dev/null
			if [ "$?" -ne "0" ]; then
				let i++
				continue
			fi
			vendor=`cat /sys/bus/pci/devices/$domain:01:00.0/vendor`
			device=`cat /sys/bus/pci/devices/$domain:01:00.0/device`
			if [[ $vendor = 0x1e43 ]] && [[ $device = 0x3171 ]]; then
				wav_id=7 # WAV7 found
				echo 7 > /tmp/ppmd/wave_ver.txt
				break
			elif [[ $vendor = 0x8086 ]] && [[ $device = 0x0d5a ]]; then
				wav_id=6 # WAV6 found
				echo 6 > /tmp/ppmd/wave_ver.txt
				break
			elif [[ $vendor = 0x8086 ]] && [[ $device = 0x09d0 ]]; then
				wav_id=6 # WAV6 found
				echo 6 > /tmp/ppmd/wave_ver.txt
				break
			elif [[ $vendor = 0x17e6 ]] && [[ $device = 0x0d5a ]]; then
				wav_id=6 # WAV6 found
				echo 6 > /tmp/ppmd/wave_ver.txt
				break
			else
				#no MXL WiFi module found
				wav_id=99
				echo 99 > /tmp/ppmd/wave_ver.txt
			fi
			let i++
		done
	else
		wav_id=`cat /tmp/ppmd/wave_ver.txt`
	fi
}
############################################################

############################################################
# wlan power saving feature(s)
wlan_pm () {
	which_wav_check
	nWlan=$1
	pcie_path=`find /sys/bus/platform/drivers/ -name \*-pcie`
	# save the pcie memory address related to the wifi radio
	# save if only one time to get the default config
	if [ ! -f /tmp/radio_pcie_map.txt ]; then	
		ls -al /sys/class/net/ | grep "wlan0 " 1> /tmp/radio_pcie_map.txt 2> /dev/null
		ls -al /sys/class/net/ | grep "wlan2 " 1>> /tmp/radio_pcie_map.txt 2> /dev/null
		ls -al /sys/class/net/ | grep "wlan4 " 1>> /tmp/radio_pcie_map.txt 2> /dev/null
		ls -al /sys/class/net/ | grep "wlan6 " 1>> /tmp/radio_pcie_map.txt 2> /dev/null
	fi
	if ! grep wlan$1 /tmp/radio_pcie_map.txt 1> /dev/null; then
		return
	fi
	if [ "$level_pm" = "0" ]; then
		if [[ "$wav_id" = "6" ]] || [[ "$wav_id" = "7" ]]; then
			#radio_antenna_check $nWlan
			if iw wlan"$nWlan" iwlwav sCoCPower 0 1> /dev/null 2> /dev/null; then
				if iw wlan"$nWlan" iwlwav sCoCPower 0; then
					echo "wlan$nWlan power saving feature (Auto-CoC-MiMo-Mode) will be disabled"
				fi
			fi
		fi
	elif [ "$level_pm" = "1" ]; then
		if [ "$wav_id" = "6" ]; then
			if iw wlan"$nWlan" iwlwav sCoCPower 0 1> /dev/null 2> /dev/null; then
				if iw wlan"$nWlan" iwlwav sCoCPower 0; then
					echo "wlan$nWlan power saving feature (Auto-CoC-MiMo-Mode) will be disabled"
				fi
			fi
		elif [ "$wav_id" = "7" ]; then
			if iw wlan"$nWlan" iwlwav sCoCPower 0 1> /dev/null 2> /dev/null; then
				if iw wlan"$nWlan" iwlwav sCoCPower 0; then
					echo "wlan$nWlan power saving feature (Auto-CoC-MiMo-Mode) will be disabled"
				fi
			fi
		fi
	elif [ "$level_pm" = "2" ]; then
		if [[ "$wav_id" = "6" ]] || [[ "$wav_id" = "7" ]]; then
			if ! ls -l /sys/class/net/ | grep wlan$1 1> /dev/null; then
				echo "no wlan$1 found; already disabled"
				return
			fi
			radio_pcie_id=`cat /tmp/radio_pcie_map.txt | grep "wlan$nWlan " | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
			if [ -n "$radio_pcie_id" ]; then
				echo "Disable wlan$nWlan. Force device into low power mode"
				radio_pcie_id=`cat /tmp/radio_pcie_map.txt | grep "wlan$nWlan " | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
				wifi down radio$nWlan
				sleep 3
				echo -n $radio_pcie_id > $pcie_path/unbind
			fi
			echo "############# Disable WiFi_$nWlan done ##########"
		fi
	elif [ "$level_pm" = "3" ]; then
		if [[ "$wav_id" = "6" ]] || [[ "$wav_id" = "7" ]]; then
	#		enable is only possible for all wifi interfaces
	#		first we need to disable all and then enable all in the right sequence
	#		for MXL-LGP Tri-Band the sequence must be 6G->5G->2.4G.
	#		This is important to get the right wifi numbering wlan4,wlan2,wlan0
	#		first check if all interface are already up
			max_num=`sed -n '$=' /tmp/radio_pcie_map.txt`
			cur_num=0
			for i in 0 2 4 6; do
				wifi_int=`ls -al /sys/class/net/ | grep -c "wlan$i "`
				if [ $wifi_int -eq 1 ]; then
					cur_num=`expr $cur_num + 1`
				fi
			done
			if [ "$max_num" = "$cur_num" ]; then
				echo "All wifi interfaces are already up"
				exit 0
			fi
			echo "Enable wlan$nWlan. Bring the device back into operational mode"
			echo "We need to disable first all wifi interfaces"
			wifi down
			sleep 5
			for i in 0 2 4; do
				if ! ls -l /sys/class/net/ | grep wlan$i 1> /dev/null; then
					echo "Disable wlan$i. Already disabled"
					continue
				fi
				radio_pcie_id=`cat /tmp/radio_pcie_map.txt | grep "wlan$i " | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
				if [ -n "$radio_pcie_id" ]; then
					echo "Disable wlan$i. Force device into low power mode"
					radio_pcie_id=`cat /tmp/radio_pcie_map.txt | grep "wlan$i " | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
					echo -n $radio_pcie_id > $pcie_path/unbind
				fi
			done

			for i in 4 2 0; do
				radio_pcie_id=`cat /tmp/radio_pcie_map.txt | grep "wlan$i " | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
				if [ -n "$radio_pcie_id" ]; then
					echo "Enable wlan$i. Bring the device back into operational mode"
					echo -n $radio_pcie_id > $pcie_path/bind
				fi
			done
			# bring-up all wifi interfaces
			sleep 5
			wifi up
			sleep 7
			echo "############# Enable WiFi done. All interfaces should be up ##########"
			exit 0
		fi
	else
		if iw wlan"$nWlan" iwlwav gCoCPower 1> /dev/null 2> /dev/null; then
			echo ""
			echo "WLAN$nWlan:"
			iw wlan"$nWlan" iwlwav gCoCPower
			if ifconfig | grep -q "wlan$nWlan"; then
				echo "wlan$nWlan in operative mode"
			else
				echo "wlan$nWlan in low power mode"
			fi
		else
			echo "No wlan$nWlan on this board!"
		fi
	fi
}
############################################################
 
############################################################
# dsl helper function to detect VRX518 or VRX618
dsl_vrx_detect () {
	if [ ! -f /tmp/pcie_rc_device.txt ]; then
		ls -1 /sys/bus/pci/devices/ | grep ....:00:00.0 1> /tmp/pcie_rc_device.txt 2> /dev/null
	fi
	if [ ! -f /tmp/dsl_family.txt ]; then	
		i=1
		num=`grep -c ....:00:00.0 /tmp/pcie_rc_device.txt`
		echo "not defined" 1> /tmp/dsl_family.txt 2> /dev/null
		echo "not defined" 1>> /tmp/dsl_family.txt 2> /dev/null
		while [ "$i" -le "$num" ]; do
			domain=`awk -F":" 'NR == '$i' {print $1}' /tmp/pcie_rc_device.txt`
			ls /sys/bus/pci/devices/$domain:01:00.0/vendor 1> /dev/null 2> /dev/null
			if [ "$?" -ne "0" ]; then
				let i++
				continue
			fi
			vendor=`cat /sys/bus/pci/devices/$domain:01:00.0/vendor`
			device=`cat /sys/bus/pci/devices/$domain:01:00.0/device`
			if [[ $vendor = 0x8086 ]] && [[ $device = 0x09a9 ]]; then
				dsl_family="vrx518"
			elif [[ $vendor = 0x8086 ]] && [[ $device = 0x09aa ]]; then
				dsl_family="vrx618"
				dsl_pcie_id="$domain:01"
				echo $dsl_family 1> /tmp/dsl_family.txt 2> /dev/null
				echo $dsl_pcie_id 1>> /tmp/dsl_family.txt 2> /dev/null
			fi
			let i++
		done
	else
		dsl_pcie_id=$(awk 'NR==2' /tmp/dsl_family.txt)
		dsl_family=$(awk 'NR==1' /tmp/dsl_family.txt)
	fi
}
###########################################################
# dsl helper function to detect VRX518 or VRX618
dsl_bonding_detect () {
	if [ ! -f /tmp/dsl_bonding.txt ]; then	
		if < /opt/intel/etc/dsl_daemon.conf grep '"max_device_number": 2' 1> /dev/null 2> /dev/null; then
			dsl_bonding="bonding"
		else
			dsl_bonding="single"
		fi
		echo $dsl_bonding 1> /tmp/dsl_bonding.txt 2> /dev/null
	else
		dsl_bonding=$(cat /tmp/dsl_bonding.txt)
	fi
}
############################################################

############################################################
# dsl power saving feature(s)
dsl_pm () {
	pcie_path=`find /sys/bus/platform/drivers/ -name \*-pcie`
	dsl_vrx_detect
	dsl_bonding_detect
	if [ $dsl_family != "vrx618" ]; then
		exit
	fi
	# save the pcie memory address related to the dsl
	# save if only one time to get the default config
	if [ ! -f /tmp/pci_bus_map.txt ]; then	
		ls -l /sys/class/pci_bus/ 1> /tmp/pci_bus_map.txt 2> /dev/null
	fi

	if [ "$level_pm" = "0" ]; then
		echo "No power saving feature(s) for $dsl_family!"
		
	elif [ "$level_pm" = "1" ]; then
		echo "No power saving feature(s) for $dsl_family!"
	elif [ "$level_pm" = "2" ]; then
		echo "disable vrx618"
		dsl_pcie_addr=`cat /tmp/pci_bus_map.txt | grep $dsl_pcie_id | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
		if [ -n "$dsl_pcie_addr" ]; then
			echo "Disable DSL. Force device into low power mode"
			/etc/init.d/ltq_vrx618_startup.sh stop
			sleep 6
			echo -n $dsl_pcie_addr > $pcie_path/unbind
		fi
		echo "############# Disable vrx618 done ##########"
	elif [ "$level_pm" = "3" ]; then
		echo "enable vrx618"
		dsl_pcie_addr=`cat /tmp/pci_bus_map.txt | grep $dsl_pcie_id | awk -F '/' '{for(i=1;i<=NF;i++){ if($i ~ ".pcie$") print $i } }'`
		if [ -n "$dsl_pcie_addr" ]; then
			echo "Enable DSL. Force device into operational mode"
			echo -n $dsl_pcie_addr > $pcie_path/bind
			/etc/init.d/ltq_vrx618_startup.sh start
			sleep 2
		fi
		echo "############# Enable vrx618 done ##########"
	fi
}
############################################################
#LGS(LGM+P34X###############################################
# eth0_2 = GPHY_port0;	eth0_4 = GPHY_port2
# eth0_3 = GPHY_port1;	eth0_5 = GPHY_port3
############################################################
# all GPHYs/ethernet ports power saving feature(s)
# i: ethernet numbering
all_gphy_pm () {
	i=0
	for i in 2 3 4 5; do
		gphy_pm $i
	done
}
############################################################

############################################################
# GPHY/Ethernet power saving feature(s)
set_ghpy_id() {
	case "$1" in
		"1") echo "no valid p34x gphy id"
			nGphy=99
			;;
		"2") nGphy=0
			;;
		"3") nGphy=1
			;;
		"4") nGphy=2
			;;
		"5") nGphy=3
			;;
		*) echo "Unknown eth_id"
		   nGphy=99
			;;
	esac
}
############################################################

############################################################
gphy_pm () {
	phyctrl=`find /sys/devices/ -name \*p34x@0\*`
	nEth=$1
	set_ghpy_id $1
	if [ "$nGphy" = "99" ]; then
		return
	fi
	if [ "$level_pm" = "0" ]; then
		ethtool --set-eee eth0_"$nEth" eee off 2> /dev/null
	elif [ "$level_pm" = "1" ]; then
		ethtool --set-eee eth0_"$nEth" eee on 2> /dev/null
	elif [ "$level_pm" = "2" -a "$phyctrl" != "" ]; then
		echo phy_power $nGphy 0 > $phyctrl/phy_control
		echo "Disable eth0_$nEth. Force eth0_$nEth into power down mode"
	elif [ "$level_pm" = "3" -a "$phyctrl" != "" ]; then
		echo phy_power $nGphy 1 > $phyctrl/phy_control
		echo "Enable eth0_$nEth. Bring eth0_$nEth back into operational mode"
	elif [ "$level_pm" = "4" -a "$phyctrl" != "" ]; then
		echo phy_lpmode $nGphy 0 > $phyctrl/phy_control
		echo "Disable Low Power Mode for eth0_$nEth"
	elif [ "$level_pm" = "5" -a "$phyctrl" != "" ]; then
		echo phy_lpmode $nGphy 1 > $phyctrl/phy_control
		echo "Enable Low Power Mode for eth0_$nEth"
	else
		echo ""
		# low power mode
		# Hint: phy0 does not support LP mode in P34x.
		# When the phy is in LP mode we cannot read any further
		# info via the mdio bus. Therefore we skip the following
		# parameters.
		if [ "$phyctrl" != "" ]; then
			lp=`cat $phyctrl/phy_control | grep "LowPower:  gphy$nGphy" | awk '{ printf $3}'`
			if [ "$lp" = "active" ]; then
				echo "eth0_$nEth  LP Mode active"
				return
			else
				echo "eth0_$nEth  LP mode $lp"
			fi
			# power down
			pd=`cat $phyctrl/phy_control | grep "PowerDown: gphy$nGphy" | awk '{ printf $3}'`
			echo "        in $pd mode"
		else
			echo "eth0_$nEth"
		fi
		# eee mode
		ethtool --show-eee eth0_$nEth | grep "EEE status"
		# link detection
		ethtool eth0_$nEth | grep "Link detected:"
	fi
}
############################################################

############################################################
# PCIe power saving helper
#this function search for GEN4 interface (16GT/s) and limit L-State to L0s
pcie_gen4_pm () {
	if [ ! -f /tmp/pcie_rc_device.txt ]; then
		ls -1 /sys/bus/pci/devices/ | grep ....:00:00.0 1> /tmp/pcie_rc_device.txt 2> /dev/null
	fi
	num=`grep -c ....:00:00.0 /tmp/pcie_rc_device.txt`
	#check for new link_state control
	pcie_id=`awk 'NR == max {print $1}' max=1 /tmp/pcie_rc_device.txt`
	ls -al /sys/bus/pci/devices/$pcie_id/power/ | grep link_state 1> /dev/null 2> /dev/null
	if [ "$?" -eq "0" ]; then
		#old link_state control
		i=1
		while [ "$i" -le "$num" ]; do
			pcie_id=`awk 'NR == max {print $1}' max="${i}" /tmp/pcie_rc_device.txt`
			cat /sys/bus/pci/devices/$pcie_id/max_link_speed | grep "16 GT/s" 1> /dev/null 2> /dev/null
			if [ "$?" -eq "0" ]; then
				echo 3 > /sys/bus/pci/devices/$pcie_id/power/link_state
			else
				echo 7 > /sys/bus/pci/devices/$pcie_id/power/link_state
			fi
			let i++
		done
	else
		#new link_state control
		vendor=""
		device=""
		ls -R /sys/devices/platform/soc/*pcie* | grep l1_aspm 1> /dev/null 2> /dev/null
		if [ "$?" -eq "0" ]; then
			i=1
			while [ "$i" -le "$num" ]; do
				domain=`awk -F":" 'NR == '$i' {print $1}' /tmp/pcie_rc_device.txt`
				ls /sys/bus/pci/devices/$domain:01:00.0/vendor 1> /dev/null 2> /dev/null
				if [ "$?" -eq "0" ]; then
					vendor=`cat /sys/bus/pci/devices/$domain:01:00.0/vendor`
					device=`cat /sys/bus/pci/devices/$domain:01:00.0/device`
				fi
				pcie_id=`awk 'NR == max {print $1}' max="${i}" /tmp/pcie_rc_device.txt`
				cat /sys/bus/pci/devices/$pcie_id/max_link_speed | grep "16.0 GT/s" 1> /dev/null 2> /dev/null
				if [ "$?" -eq "0" ]; then
					l1_aspm_path=`find /sys/bus/pci/devices/$pcie_id/ -name l1_aspm`
					if [ "$l1_aspm_path" != "" ]; then
						echo 0 > $l1_aspm_path
						#cat $l1_aspm_path
					fi
				elif [[ $vendor = 0x8086 ]] && [[ $device = 0x09aa ]]; then
					l1_aspm_path=`find /sys/bus/pci/devices/$pcie_id/ -name l1_aspm`
					if [ "$l1_aspm_path" != "" ]; then
						echo 0 > $l1_aspm_path
						#cat $l1_aspm_path
					fi
				else
					l1_aspm_path=`find /sys/bus/pci/devices/$pcie_id/ -name l1_aspm`
					if [ "$l1_aspm_path" != "" ]; then
						echo 1 > $l1_aspm_path
						#cat $l1_aspm_path
					fi
				fi
				let i++
			done
		fi
	fi
}
############################################################

############################################################
# PCIe power saving feature(s)
int2str () {
	if [ "$1" -eq "0" ]; then
		echo "Disabled"
	elif [ "$1" -eq "1" ]; then
		echo "Enabled "
	fi
}
############################################################

############################################################
# PCIe power saving feature(s)
pcie_pm () {
	which_wav_check
	if [ ! -f /sys/module/pcie_aspm/parameters/policy ]; then
		echo "ASPM cannot be changed because policy files does not exist"
		return
	fi
	if [ "$level_pm" = "0" ]; then
 		echo performance > /sys/module/pcie_aspm/parameters/policy
#		echo "Disable PCIe power saving feature (ASPM)"
	elif [ "$level_pm" = "1" ]; then
		if [ "$wav_id" = "6" ]; then
			echo performance > /sys/module/pcie_aspm/parameters/policy
		else
 			echo powersave > /sys/module/pcie_aspm/parameters/policy
			pcie_gen4_pm
#			echo "Enable PCIe power saving feature (ASPM)"
		fi
	elif [ "$level_pm" = "2" ]; then
		echo "No power down mode for PCIe available"
	elif [ "$level_pm" = "3" ]; then
		echo "No power down mode for PCIe available"
	else
		if [ ! -f /tmp/pcie_rc_device.txt ]; then
			ls -1 /sys/bus/pci/devices/ | grep ....:00:00.0 1> /tmp/pcie_rc_device.txt 2> /dev/null
		fi
		printf "Status PCIe ASPM:\n\n"
		i=1
		num=`grep -c ....:00:00.0 /tmp/pcie_rc_device.txt`
		while [ "$i" -le "$num" ]; do
			domain=`awk -F":" 'NR == '$i' {print $1}' /tmp/pcie_rc_device.txt`
			ls /sys/bus/pci/devices/$domain:00:00.0/$domain:01:00.0/link/l0s_aspm 1> /dev/null 2> /dev/null
			if [ "$?" -ne "0" ]; then
				let i++
				continue
			fi
			vendor=`cat /sys/bus/pci/devices/$domain:01:00.0/vendor`
			if [[ "$vendor" = "0x8086" ]] || [[ "$vendor" = "0x1e43" ]] || [[ "$vendor" = "0x17e6" ]]; then
				vendor="Maxlinear"
			fi
			device=`cat /sys/bus/pci/devices/$domain:01:00.0/device`
			if [ "$device" = "0x09aa" ]; then
				device="VRX618"
			elif [ "$device" = "0x09a9" ]; then
				device="VRX518"
			elif [ "$device" = "0x0d5a" ]; then
				device="WAV6"
			elif [ "$device" = "0x09d0" ]; then
				device="WAV6"
			elif [ "$device" = "0x3171" ]; then
				device="WAV7"
			fi
			l0s=`cat /sys/bus/pci/devices/$domain:00:00.0/$domain:01:00.0/link/l0s_aspm`
			l1=`cat /sys/bus/pci/devices/$domain:00:00.0/$domain:01:00.0/link/l1_aspm`
			cls=`cat /sys/bus/pci/devices/$domain:00:00.0/current_link_speed`
			clw=`cat /sys/bus/pci/devices/$domain:00:00.0/current_link_width`
			mls=`cat /sys/bus/pci/devices/$domain:00:00.0/max_link_speed`
			mlw=`cat /sys/bus/pci/devices/$domain:00:00.0/max_link_width`
			echo "port:$domain  L0s_aspm=$(int2str $l0s)  L1_aspm=$(int2str $l1)  curspeed=$cls  curwidth=$clw  maxspeed=$mls  maxwidth=$mlw  vendor=$vendor  device=$device"
			let i++
		done
	fi
}
############################################################

############################################################
# CPU power saving feature(s)
cpu_pm () {
	max_cpu_num=`ls /sys/devices/system/cpu | grep -c cpu[0-9]`
	if [ "$level_pm" = "0" ]; then
		i=0
		while [ "$i" -lt "$max_cpu_num" ]; do
			echo performance > /sys/devices/system/cpu/cpufreq/policy$i/scaling_governor
			let i++
		done
		echo "Disable CPU power saving feature DVFS"
	elif [ "$level_pm" = "1" ]; then
		i=0
		while [ "$i" -lt "$max_cpu_num" ]; do
			echo powersave > /sys/devices/system/cpu/cpufreq/policy$i/scaling_governor
			let i++
		done
		echo "Enable CPU power saving feature DVFS"
	elif [ "$level_pm" = "2" ]; then
		i=0
		while [ "$i" -lt "$max_cpu_num" ]; do
			echo 1 > /sys/devices/system/cpu/cpu$i/cpuidle/state2/disable
			let i++
		done
		echo "Disable CPU power saving feature CPUIDLE C6"
	elif [ "$level_pm" = "3" ]; then
		i=0
		while [ "$i" -lt "$max_cpu_num" ]; do
			echo 0 > /sys/devices/system/cpu/cpu$i/cpuidle/state2/disable
			let i++
		done
		echo "Enable CPU power saving feature CPUIDLE C6"
	elif [ "$level_pm" = "4" ]; then
		echo "intel_pstate passive"
		echo passive >/sys/devices/system/cpu/intel_pstate/status
		if [ $opt_pm ]; then
			echo "set new cpu frequency $opt_pm kHz"
			i=0
			while [ "$i" -lt "$max_cpu_num" ]; do
				echo $opt_pm > /sys/devices/system/cpu/cpufreq/policy$i/scaling_setspeed
			let i++
			done
		fi
 	elif [ "$level_pm" = "5" ]; then
		echo "intel_pstate active"
		echo active >/sys/devices/system/cpu/intel_pstate/status
	elif [ "$level_pm" = "6" ]; then
		echo "offline all cpu's except cpu0"
		i=1
		while [ "$i" -lt "$max_cpu_num" ]; do
			echo 0 > /sys/devices/system/cpu/cpu$i/online
			let i++
		done
	elif [ "$level_pm" = "7" ]; then
		echo "online all available cpu's"
		i=1
		while [ "$i" -lt "$max_cpu_num" ]; do
			echo 1 > /sys/devices/system/cpu/cpu$i/online
			let i++
		done
	else
		printf "Status CPU:\n\n"
		printf "cpu freq drv : "
		drv=`cat /sys/devices/system/cpu/intel_pstate/status`
		if [ "$drv" = "active" ]; then
			printf "intel_pstate\n"
		else
			printf "intel_cpufreq\n"
		fi
		i=0
		while [ "$i" -lt "$max_cpu_num" ]; do
			printf "cpu$i:"
			gov=`cat /sys/devices/system/cpu/cpufreq/policy$i/scaling_governor`
			freq=`cat /sys/devices/system/cpu/cpufreq/policy$i/cpuinfo_cur_freq`
			printf " $gov  $freq"
			c1=`cat /sys/devices/system/cpu/cpu$i/cpuidle/state1/disable`
			c6=`cat /sys/devices/system/cpu/cpu$i/cpuidle/state2/disable`
			printf "  C1: "
			if [ $c1 -eq 0 ]; then
				printf "enabled  "
			else
				printf "disabled  "
			fi
			printf " C6: "
			if [ $c6 -eq 0 ]; then
				printf "enabled\n"
			else
				printf "disabled\n"
			fi
			let i++
		done
		echo ""
		printf "Online CPU's : "
		cat /sys/devices/system/cpu/online
		printf "Offline CPU's: "
		cat /sys/devices/system/cpu/offline
		echo ""
	fi
}
############################################################

############################################################
adp_check_ppmd_run () {
	# $1: 0=stop, 1=start
	if [ "$1" -eq "0" ]; then
		ps > /tmp/ps_status.txt
		grep -wq "ppmd -l" /tmp/ps_status.txt
		ret=$?
		if [ "$ret" -eq "0" ]; then
			#echo "stop ppmd"
			/etc/init.d/ppmd stop
		fi
	elif [ $1 = 1 ]; then
		ps > /tmp/ps_status.txt
		grep -wq "ppmd -l" /tmp/ps_status.txt
		ret=$?
		if [ "$ret" -ne "0" ]; then
			#echo "start ppmd"
			/etc/init.d/ppmd restart
		fi
	fi
}
############################################################
adp_pm () {
	epu_path=`find /sys/devices/ -name e0180000.epu`
	if [ "$level_pm" = "0" ]; then #SW_DVFS
		if [ "$opt_pm" = "0" ]; then
			adp_check_ppmd_run 0
			echo $opt_pm > $epu_path/adp_dvfs_level
			adp_lev=$(cat $epu_path/adp_dvfs_level)
			if [ "$adp_lev" = "0" ]; then
	#			echo "ADP DVFS Level set to HIGH successfully"
				echo "ADP DVFS Level: $adp_lev" 1> /dev/null
			else
				echo "ADP DVFS Level could not set successfully to HIGH Level"
				echo "ADP DVFS Level: $adp_lev"
			fi
		elif [ "$opt_pm" = "1" ]; then
			adp_check_ppmd_run 0
			echo $opt_pm > $epu_path/adp_dvfs_level
			adp_lev=$(cat $epu_path/adp_dvfs_level)
			if [ "$adp_lev" = "1" ]; then
	#			echo "ADP DVFS Level set to MED successfully"
				echo "ADP DVFS Level: $adp_lev" 1> /dev/null
			else
				echo "ADP DVFS Level could not set successfully to MED Level"
				echo "ADP DVFS Level: $adp_lev"
			fi
		elif [ "$opt_pm" = "2" ]; then
			adp_check_ppmd_run 0
			echo $opt_pm > $epu_path/adp_dvfs_level
			adp_lev=$(cat $epu_path/adp_dvfs_level)
			if [ "$adp_lev" = "2" ]; then
	#			echo "ADP DVFS Level set to LOW successfully"
				echo "ADP DVFS Level: $adp_lev" 1> /dev/null
			else
				echo "ADP DVFS Level could not set successfully to LOW Level"
				echo "ADP DVFS Level: $adp_lev"
			fi
		elif [ "$opt_pm" = "3" ]; then
			adp_check_ppmd_run 1
		fi
	elif [ "$level_pm" = "1" ]; then # HW_DFS
		if [ "$opt_pm" = "0" ]; then
			echo off > $epu_path/adp_scaling
		elif [ "$opt_pm" = "1" ]; then
			echo on > $epu_path/adp_scaling
		fi
	else
		printf "Status ADP:\n\n"
		echo "Status Register"
		val=`cat $epu_path/adp_dvfs_level`
		case "$val" in
			"0") echo "ADP DVFS Level: high"
				;;
			"1") echo "ADP DVFS Level: med"
				;;
			"2") echo "ADP DVFS Level: low"
				;;
			*) echo "ADP DVFS Level: unknown"
				;;
		esac
		val=`cat $epu_path/adp_scaling`
		echo "ADP HW DFS    : $val"
		echo ""
		echo "Control Register"
		cat $epu_path/adp_level_config
		echo ""
		ps > /tmp/ps_status.txt
		grep -wq ppmd /tmp/ps_status.txt
		ret=$?
		if [ "$ret" -eq "0" ]; then
			echo "ppmd is running in listening mode"
		else
			echo "ppmd is not running"
		fi
		echo ""
	fi
}
############################################################

############################################################
top_noc_pll () {
	if [ $level_pm ]; then
		if [ "$level_pm" = "on" ]; then
			echo 1000 > /proc/sys/lgm_sysctl/sysidle_period
		elif [ "$level_pm" = "off" ]; then
			echo 0 > /proc/sys/lgm_sysctl/sysidle_period
		else
			echo $level_pm > /proc/sys/lgm_sysctl/pll_clk1_div
		fi
	else
		printf "TOP_NOC frequency:\n\n"
		pll_clk1=$(cat /proc/sys/lgm_sysctl/pll_clk1_div)
		case "$pll_clk1" in
			"0") printf "top_noc freq = 2400MHz\n\n";;
			"1") printf "top_noc freq = 1200MHz\n\n";;
			"2") printf "top_noc freq = 800MHz\n\n";;
			"3") printf "top_noc freq = 600MHz\n\n";;
			"4") printf "top_noc freq = 480MHz\n\n";;
			"5") printf "top_noc freq = 400MHz\n\n";;
			"6") printf "top_noc freq = 300MHz\n\n";;
			"7") printf "top_noc freq = 240MHz\n\n";;
			"8") printf "top_noc freq = 200MHz\n\n";;
			"9") printf "top_noc freq = 150MHz\n\n";;
			"10") printf "top_noc freq = 120MHz\n\n";;
			"11") printf "top_noc freq = 100MHz\n\n";;
			"12") printf "top_noc freq = 100MHz\n\n";;
			"13") printf "top_noc freq = 75MHz\n\n";;
			"14") printf "top_noc freq = 50MHz\n\n";;
			"15") printf "top_noc freq = 37MHz\n\n";;
			*) printf "setting out of range\n\n";;
		esac
	fi
}
############################################################

############################################################
gswip_pll () {
	if [ $level_pm ]; then
	echo $level_pm > /proc/sys/lgm_sysctl/pll_clk3_div
	else
		printf "GSWIP frequency:\n\n"
		pll_clk3=$(cat /proc/sys/lgm_sysctl/pll_clk3_div)
		case "$pll_clk3" in
			"0") printf "gswip freq = 2400MHz\n\n";;
			"1") printf "gswip freq = 1200MHz\n\n";;
			"2") printf "gswip freq = 800MHz\n\n";;
			"3") printf "gswip freq = 600MHz\n\n";;
			"4") printf "gswip freq = 480MHz\n\n";;
			"5") printf "gswip freq = 400MHz\n\n";;
			"6") printf "gswip freq = 300MHz\n\n";;
			"7") printf "gswip freq = 240MHz\n\n";;
			"8") printf "gswip freq = 200MHz\n\n";;
			"9") echo "gswip freq = 150MHz\n\n";;
			"10") printf "gswip freq = 120MHz\n\n";;
			"11") printf "gswip freq = 100MHz\n\n";;
			"12") printf "gswip freq = 100MHz\n\n";;
			"13") printf "gswip freq = 75MHz\n\n";;
			"14") printf "gswip freq = 50MHz\n\n";;
			"15") printf "gswip freq = 37MHz\n\n";;
			*) printf "setting out of range\n\n";;
		esac
	fi
}
############################################################

############################################################
ddr_selfrefresh () {
	if [ "$level_pm" = "0" ]; then
		echo 0 > /proc/sys/lgm_sysctl/ddr_selfrefresh
	elif [ "$level_pm" = "1" ]; then
		echo 1 > /proc/sys/lgm_sysctl/ddr_selfrefresh
	else
		sr=`cat /proc/sys/lgm_sysctl/ddr_selfrefresh`
		if [ "$sr" = "0" ]; then
			echo "DDR Self Refresh is disabled"
		elif [ "$sr" = "1" ]; then
			echo "DDR Self Refresh is enabled"
		else
			echo "DDR Self Refresh undefined"
		fi
	fi
}
############################################################

############################################################
all_pm () {
	echo ""
	echo "*********************************************************"
	#wifi:
	echo "Status WiFi:"
	all_wlan_pm
	echo "*********************************************************"
	#GPHY:
	all_gphy_pm
	echo "*********************************************************"
	#CPU:
	cpu_pm
	echo "*********************************************************"
	#PCIe:
	pcie_pm
	echo "*********************************************************"
	#ADP DVFS:
	adp_pm
	echo "*********************************************************"
	#TOP NOC FREQ:
	top_noc_pll
	echo "*********************************************************"
	#GSWIP FREQ:
	gswip_pll
	echo "*********************************************************"
	#DDR Self Refresh:
	ddr_selfrefresh
	echo "*********************************************************"
}
############################################################

############################################################
help_pp () {
	echo ""
	echo "############################################################"
	echo "###########       Set Power Profile       ##################"
	echo "############################################################"
	echo ""
	echo "Following Power Profiles are supported:"
	echo "- balance          released feature set"
	echo "- balance2         best case"
	echo "- performance"
	echo ""
	echo ""
	echo "Usage:"
	echo "ppmd_profile_helper.sh profile <profile name>"
	echo ""
	echo "ppmd_profile_helper.sh profile balance"
	echo "ppmd_profile_helper.sh profile balance2"
	echo "ppmd_profile_helper.sh profile performance"
	echo ""
	echo ""
}
############################################################

############################################################
set_profile () {
	echo "selected power profile: $profile"
	if [ "$profile" = "balance" ]; then
		ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "balance"}}'
		ubus call uci commit '{"config" : "ppmdcfg" }'
	elif [ "$profile" = "balance2" ]; then
		ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "balance2"}}'
		ubus call uci commit '{"config" : "ppmdcfg" }'
	elif [ "$profile" = "performance" ]; then
		ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "performance"}}'
		ubus call uci commit '{"config" : "ppmdcfg" }'
	elif [ "$profile" = "usr_sleep" ]; then
		ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "usr_sleep"}}'
		ubus call uci commit '{"config" : "ppmdcfg" }'
	elif [ "$profile" = "usr_wakeup" ]; then
		ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "usr_wakeup"}}'
		ubus call uci commit '{"config" : "ppmdcfg" }'
	elif [ "$profile" = "bbu_low_power_on" ]; then 
                ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "bbu_low_power_on"}}'
                ubus call uci commit '{"config" : "ppmdcfg" }'
	elif [ "$profile" = "bbu_low_power_off" ]; then 
                ubus call uci set '{ "config" : "ppmdcfg" ,"section" : "global", "values": {"policy_curr" : "bbu_low_power_off"}}'
                ubus call uci commit '{"config" : "ppmdcfg" }'
	else
		echo ""
		echo "????  unknown profile  ????"
		echo ""
	fi
}
############################################################

############################################################
pwr_profile () {
	profile=$level_pm
	case "$profile" in
		"help") help_pp
			;;
		"balance") set_profile
			;;
		"balance2") set_profile
			;;
		"performance") set_profile
			;;
		"usr_sleep") set_profile
			;;
		"usr_wakeup") set_profile
			;;
		"bbu_low_power_on") set_profile
			;;
		"bbu_low_power_off") set_profile
			;;
		*) echo "????  unknown profile  ????"
			;;
	esac
}
############################################################

############################################################
# Main switch case
############################################################
#check SoC and set global Soc_id
if cat /proc/device-tree/model | grep -q "lightning mountain\|lgp\|octopus-641\|octopus-851\|osp-tb341\|osp-tb341-v2"; then
	soc_id="LGM"
fi

case "$sel_pm" in
	"help") help_pm
		;;
	"all") all_pm
		;;
	"wlan") all_wlan_pm
		;;
	"wlan0") wlan_pm 0
		;;
	"wlan2") wlan_pm 2
		;;
	"wlan4") wlan_pm 4
		;;
	"dsl0") dsl_pm 0
		;;
	"dsl1") dsl_pm 1
		;;
	"eth") all_gphy_pm
		;;
	"eth0_2") gphy_pm 2
		;;
	"eth0_3") gphy_pm 3
		;;
	"eth0_4") gphy_pm 4
		;;
	"eth0_5") gphy_pm 5
		;;
	"pcie") pcie_pm
		;;
	"cpu") cpu_pm
		;;
	"adp") adp_pm
		;;
	"top_noc") top_noc_pll
		;;
	"gswip") gswip_pll
		;;
	"ddr_sr") ddr_selfrefresh
		;;
	"profile") pwr_profile
		;;
	*) echo "Unknown Parameter"
		;;
esac
exit 0
############################################################

