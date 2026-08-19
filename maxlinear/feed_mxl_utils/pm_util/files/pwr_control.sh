#!/bin/sh

#select single device or all devices
sel_pm=$1
#enable/disable power saving feature(s) or force to low power mode
level_pm=$2
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
	echo "WLAN:                                             -ewlan     (for all wlan)"
	echo "                                                  -ewlan0"
	echo "                                                  -ewlan2"
	echo "                                                   ..."
	echo "                                                  -ewlan10"
	if [ "$soc_id" = "LGM" -o  "$soc_id" = "GRX500" ]; then
		echo "DSL:                                              -edsl      (for all dsl)"
	fi
	echo "                                                  -edsl0"
	echo "                                                  -edsl1"
	echo "Ethernet/GPHY:                                    -eeth      (for all GPHYs)"
	echo "                                                  -eeth0_1"
	echo "                                                  -eeth0_2"
	echo "                                                   ..."
	echo "                                                  -eeth0_4"
	if [ "$soc_id" = "LGM" ]
		then
		echo "                                               4: LP mode disable"
		echo "                                               5: LP mode enable"
	fi
	echo "PCIe:                                             -epcie"
	echo "CPU:                                              -ecpu"
	if [ "$soc_id" = "LGM" ]
		then
		echo "ADP: (get status SW-DVFS, HW-DFS)                 -eadp"
		echo "ADP_SW: [-i0=HIGH, -i1=MED, -i2=LOW]              -eadp_sw"
		echo "ADP_HW: [-i0=OFF, -i1=ON]                         -eadp_hw"
		echo "EPU:                                              -eepu"
		echo "CPU:                                              -ecpu -i2 (offline cpu's)"
		echo "CPU:                                              -ecpu -i3 (online cpu's)"
	fi
	echo "all above at once:                                -eall"
	echo ""
	echo "Second parameter defines required action."
	echo "registered paramters are:"
	echo "disable power saving feature(s):                  -i0"
	echo "enable power saving feature(s):                   -i1"
	echo "disable device/low power mode:                    -i2"
	echo "enabel device/operational mode:                   -i3"
	echo "current device status:                             no argument"
	echo ""
	echo "examples:"
	echo "pm_util -ewlan0                  shows current status of wlan0"
	echo "pm_util -ewlan2 -i1              enables powersaving mode (AutoCoCMode) for wlan2"
	echo "pm_util -eeth0_4 -i2             puts eth0_4 into low power mode"
	echo ""
}
############################################################

############################################################
# all available power saving features for the system
all_pm () {
	if [ "$level_pm" = "0" ]; then
		echo "all power saving features will be disabled:"

		#wifi:
		i=0
		for i in 0 2 4 6 8 10; do
			wlan_pm $i
		done

		#GPHY:
		all_gphy_pm

		#CPU:
		cpu_pm

		#PCIe:
		pcie_pm
	elif [ "$level_pm" = "1" ]; then
		echo "all power saving features will be enabled:"

		#wifi:
		i=0
		for i in 0 2 4 6 8 10; do
			wlan_pm $i
		done

		#GPHY:
		all_gphy_pm

		#CPU:
		cpu_pm

		#PCIe:
		pcie_pm
	elif [ "$level_pm" = "2" ]; then
		echo "Disable devices. Force devices into low power mode"
	
		#wifi:
		i=0
		for i in 0 2 4 6 8 10; do
			wlan_pm $i
		done

		#GPHY:
		all_gphy_pm

		#DSL:
		i=0
		for i in 0 1; do
			dsl_pm $i
		done
	elif [ "$level_pm" = "3" ]; then
		echo "Enable devices. Bring the devices back into operational mode"

		#wifi:
		i=0
		for i in 0 2 4 6 8 10; do
			wlan_pm $i
		done

		#GPHY:
		all_gphy_pm

		#DSL:
		i=0
		for i in 0 1; do
			dsl_pm $i
		done
	else #get status
		echo ""

		echo "*********************************************************"
		#wifi:
		echo "status wlan:"
		i=0
		found=0
		for i in 0 2 4 6 8 10; do
			wlan_pm $i
		done
		if [ $found -eq 0 ]; then
			echo ""
			echo "No wlan interface found"
		fi

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
		#DSL:
		i=0
		for i in 0 1; do
			dsl_pm $i
		done

		echo "*********************************************************"
		#Device Tree Status:
		echo ""
		echo "Device Tree Status:"
		esc=$(printf '\033');find /sys/firmware/devicetree/base/ -name status -print0 -exec cat {} \; | xargs -n 2 -0 | sort|  sed "s,disabled,${esc}[31m&${esc}[0m," | sed "s,ok.*,${esc}[32m&${esc}[0m,"

		echo "*********************************************************"
		#pm_util -a:
		echo ""
		echo "pm_util Status:"
		pm_util -a

		if [ "$soc_id" = "LGM" ]; then
			echo "*********************************************************"
			#EPU Table:
			epu_table

			echo "*********************************************************"
			#ADP DVFS:
			echo ""
			adp_pm
		fi

		echo "*********************************************************"
		#/proc/cmdline
		echo ""
		echo "/proc/cmdline"
		cmdline

		echo "*********************************************************"
		#runtime_suspend status
		echo ""
		echo "runtime_suspend status"
		runtime_suspend
	fi
}
############################################################

############################################################
wait_for_opc_idle() {
	nidle=0
	while [ 1 ]; do
		nidle=`io -4 $1 | awk '{ printf $2}'`
		nidle=`echo $(((0x$nidle & 0x1)))`
		if [ $nidle -eq 1 ]; then
			break
		fi
		sleep 1
		#echo "nidle=$nidle"
	done
}
############################################################

############################################################
# EPU Table Support
epu_table () {
	if [ "$level_pm" = "0" ]; then #disable voltage scaling
		pm_util -sd2 1 #just allow C1
		pm_util -sgperformance #set voltage to the highest level
		#CPU0
		io -4 0xE010e000 0x0 #opc_control
		wait_for_opc_idle 0xE010e008 #wait for opc idle
		#Set PMIC_TIMEOUT_CNT to '0'
		io -4 0xE0180478 0x0
		#Disable Voltage Scaling by changing the start address of the micro code
		io -1 0xE0112800 0x46
		io -1 0xE0112808 0x46
		io -1 0xE0112810 0x46
		io -1 0xE0112818 0x46
		io -1 0xE0112820 0x46
		io -1 0xE0112828 0x46
		io -1 0xE0112830 0x46
		io -1 0xE0112838 0x46
		io -1 0xE0112850 0x46
		io -1 0xE0112858 0x46
		io -1 0xE0112860 0x46
		io -1 0xE0112868 0x46
		io -1 0xE0112870 0x46
		io -1 0xE0112878 0x46
		#io -4 0xE010e000 0x5 #opc_control
		#CPU1
		io -4 0xE011e000 0x0 #opc_control
		wait_for_opc_idle 0xE011e008 #wait for opc idle
		#Set PMIC_TIMEOUT_CNT to '0'
		io -4 0xE018047c 0x0
		#Disable Voltage Scaling by changing the start address of the micro code
		io -1 0xE0113800 0x46
		io -1 0xE0113808 0x46
		io -1 0xE0113810 0x46
		io -1 0xE0113818 0x46
		io -1 0xE0113820 0x46
		io -1 0xE0113828 0x46
		io -1 0xE0113830 0x46
		io -1 0xE0113838 0x46
		io -1 0xE0113850 0x46
		io -1 0xE0113858 0x46
		io -1 0xE0113860 0x46
		io -1 0xE0113868 0x46
		io -1 0xE0113870 0x46
		io -1 0xE0113878 0x46
		#io -4 0xE011e000 0x5 #opc_control
		#ADP
		io -4 0xE010c000 0x0 #opc_control
		wait_for_opc_idle 0xE010c008 #wait for opc idle
		#Set PMIC_TIMEOUT_CNT to '0'
		io -4 0xE0180480 0x0
		#Disable Voltage Scaling by changing the start address of the micro code
		io -1 0xE0110800 0x46
		io -1 0xE0110808 0x46
		io -1 0xE0110810 0x46
		io -1 0xE0110818 0x46
		io -1 0xE0110820 0x46
		io -1 0xE0110838 0x46
		io -1 0xE0110840 0x46
		io -1 0xE0110848 0x46
		io -1 0xE0110850 0x46
		io -1 0xE0110858 0x46
		io -1 0xE0110860 0x46
		io -1 0xE0110868 0x46
		io -1 0xE0110870 0x46
		io -1 0xE0110878 0x46
		#io -4 0xE010c000 0x5 #opc_control
		pm_util -sd2 0 #enable C6NS
		pm_util -sgpowersave
	elif [ "$level_pm" = "1" ]; then #enable voltage scaling
		pm_util -sd2 1 #just allow C1
		pm_util -sgperformance #set voltage to the highest level
		#CPU0
		io -4 0xE010e000 0x0 #opc_control
		wait_for_opc_idle 0xE010e008 #wait for opc idle
		#Enable Voltage Scaling by changing the start address of the micro code
		io -1 0xE0112800 0x0
		io -1 0xE0112808 0x0
		io -1 0xE0112810 0x0
		io -1 0xE0112818 0x0
		io -1 0xE0112820 0x0
		io -1 0xE0112828 0x0
		io -1 0xE0112830 0x0
		io -1 0xE0112838 0x0
		io -1 0xE0112850 0x0
		io -1 0xE0112858 0x0
		io -1 0xE0112860 0x0
		io -1 0xE0112868 0x0
		io -1 0xE0112870 0x0
		io -1 0xE0112878 0x0
		#Set PMIC_TIMEOUT_CNT to '0xffffffff'
		io -4 0xE0180478 0xffffffff
		io -4 0xE010e000 0x5 #opc_control
		#CPU1
		io -4 0xE011e000 0x0 #opc_control
		wait_for_opc_idle 0xE011e008 #wait for opc idle
		#Enable Voltage Scaling by changing the start address of the micro code
		io -1 0xE0113800 0x0
		io -1 0xE0113808 0x0
		io -1 0xE0113810 0x0
		io -1 0xE0113818 0x0
		io -1 0xE0113820 0x0
		io -1 0xE0113828 0x0
		io -1 0xE0113830 0x0
		io -1 0xE0113838 0x0
		io -1 0xE0113850 0x0
		io -1 0xE0113858 0x0
		io -1 0xE0113860 0x0
		io -1 0xE0113868 0x0
		io -1 0xE0113870 0x0
		io -1 0xE0113878 0x0
		#Set PMIC_TIMEOUT_CNT to '0xffffffff'
		io -4 0xE018047c 0xffffffff
		io -4 0xE011e000 0x5 #opc_control
		#ADP
		#Enable Voltage Scaling by changing the start address of the micro code
		io -4 0xE010c000 0x0 #opc_control
		wait_for_opc_idle 0xE010c008 #wait for opc idle
		io -1 0xE0110800 0x0
		io -1 0xE0110808 0x0
		io -1 0xE0110810 0x0
		io -1 0xE0110818 0x0
		io -1 0xE0110820 0x0
		io -1 0xE0110838 0x0
		io -1 0xE0110840 0x0
		io -1 0xE0110848 0x0
		io -1 0xE0110850 0x0
		io -1 0xE0110858 0x0
		io -1 0xE0110860 0x0
		io -1 0xE0110868 0x0
		io -1 0xE0110870 0x0
		io -1 0xE0110878 0x0 
		#Set PMIC_TIMEOUT_CNT to '0xffffffff'
		io -4 0xE0180480 0xffffffff
		io -4 0xE010c000 0x5 #opc_control
		pm_util -sgpowersave #enable DVFS
		pm_util -sd2 0 #enable C6NS
	else
		epu_path=`find find /sys/kernel/debug -name \*_epu`
		cat $epu_path/paseq1_cpu0_idx_tbl
		cat $epu_path/paseq2_cpu1_idx_tbl
		cat $epu_path/paseq0_adp_idx_tbl
		pmic_to_cpu0=`io -4 0xE0180478`
		pmic_to_cpu1=`io -4 0xE018047c`
		pmic_to_adp=`io -4 0xE0180480`
		echo ""
		echo "CPU0 PMIC_TIMEOUT_CNT: $pmic_to_cpu0"
		echo "CPU1 PMIC_TIMEOUT_CNT: $pmic_to_cpu1"
		echo "ADP  PMIC_TIMEOUT_CNT: $pmic_to_adp"
	fi
}
############################################################

############################################################
# ADP DVFS feature(s)
adp_pm () {
	echo "Status:"
	epu_path=`find /sys/devices/ -name e0180000.epu`
	val=`cat $epu_path/adp_dvfs_level`
	case "$val" in
		"0") echo "ADP DVFS Level: HIGH"
			;;
		"1") echo "ADP DVFS Level: MED"
			;;
		"2") echo "ADP DVFS Level: LOW"
			;;
		*) echo "ADP DVFS Level: UNKNOWN"
			;;
	esac
	val=`cat $epu_path/adp_scaling`
	echo "ADP HW DFS    : $val"
	echo ""
	echo "Control:"
	cat $epu_path/adp_level_config
}

# ADP DVFS feature(s)
adp_sw_pm () {
	epu_path=`find /sys/devices/ -name e0180000.epu`
	if [ "$level_pm" = "0" ]; then
		echo $level_pm > $epu_path/adp_dvfs_level
		adp_lev=$(cat $epu_path/adp_dvfs_level)
		if [ "$adp_lev" = "0" ]; then
			echo "ADP DVFS Level set to HIGH successfully"
			echo "ADP DVFS Level: $adp_lev"
		else
			echo "ADP DVFS Level could not set successfully to HIGH Level"
			echo "ADP DVFS Level: $adp_lev"
		fi
	elif [ "$level_pm" = "1" ]; then
		echo $level_pm > $epu_path/adp_dvfs_level
		adp_lev=$(cat $epu_path/adp_dvfs_level)
		if [ "$adp_lev" = "1" ]; then
			echo "ADP DVFS Level set to MED successfully"
			echo "ADP DVFS Level: $adp_lev"
		else
			echo "ADP DVFS Level could not set successfully to MED Level"
			echo "ADP DVFS Level: $adp_lev"
		fi
	elif [ "$level_pm" = "2" ]; then
		echo $level_pm > $epu_path/adp_dvfs_level
		adp_lev=$(cat $epu_path/adp_dvfs_level)
		if [ "$adp_lev" = "2" ]; then
			echo "ADP DVFS Level set to LOW successfully"
			echo "ADP DVFS Level: $adp_lev"
		else
			echo "ADP DVFS Level could not set successfully to LOW Level"
			echo "ADP DVFS Level: $adp_lev"
		fi
	elif [ "$level_pm" = "3" ]; then
		echo "Not supported mode"
	else
		val=`cat $epu_path/adp_dvfs_level`
		case "$val" in
			"0") echo "ADP DVFS Level: HIGH"
				;;
			"1") echo "ADP DVFS Level: MED"
				;;
			"2") echo "ADP DVFS Level: LOW"
				;;
			*) echo "ADP DVFS Level: UNKNOWN"
				;;
		esac
	fi
}

############################################################
# ADP DVFS feature(s)
adp_hw_pm () {
	epu_path=`find /sys/devices/ -name e0180000.epu`
	if [ "$level_pm" = "0" ]; then
		echo off > $epu_path/adp_scaling
	elif [ "$level_pm" = "1" ]; then
		echo on > $epu_path/adp_scaling
	else
		val=`cat $epu_path/adp_scaling`
		echo "ADP HW DFS    : $val"
	fi
}
############################################################

############################################################
#/proc/cmdline
cmdline () {
	cat /proc/cmdline
}
############################################################

############################################################
#runtime_suspend
runtime_suspend () {
	cat /sys/kernel/debug/pm_genpd/pm_genpd_summary
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
# wlan power saving feature(s)
wlan_pm () {
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
		radio_antenna_check $nWlan
		if iw wlan"$nWlan" iwlwav sCoCPower 0 $wlan_tx $wlan_rx 1> /dev/null 2> /dev/null; then
			if iw wlan"$nWlan" iwlwav sCoCPower 0 $wlan_tx $wlan_rx; then
				echo "wlan$nWlan power saving feature (Auto-CoC-MiMo-Mode) will be disabled"
			fi
		fi
	elif [ "$level_pm" = "1" ]; then
		if iw wlan"$nWlan" iwlwav sCoCPower 1 1> /dev/null 2> /dev/null; then
			if iw wlan"$nWlan" iwlwav sCoCPower 1; then
				echo "wlan$nWlan power saving feature (Auto-CoC-MiMo-Mode) will be enabled"
			fi
		fi
	elif [ "$level_pm" = "2" ]; then
#		disable is possible for a single wifi interface
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
	elif [ "$level_pm" = "3" ]; then
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
############################################################
#DSL Helper function for vrx518 single line
#Tear Down DSL Link and Power Down LD
wait_for_firmware_ready() {
	nTimeout=$1
	while [ "$nTimeout" -gt 0 ]; do
		FDSG_VALS=$(/opt/intel/bin/dsl_cpe_pipe.sh asg)
		if echo "$FDSG_VALS"; then
			for k in $FDSG_VALS; do eval "$k" 2>/dev/null; done
			if [ "$nStatus" = "5" ]; then
				#echo "nStatus="$nStatus
				break
			fi
		fi
		nTimeout=$(("$nTimeout"-1))
		sleep 1
		echo "$nTimeout"
	done
}
########################################################
#DSL Helper function for vrx518 bonding
#Tear Down DSL Link and Power Down LD
wait_for_firmware_ready2() {
	nTimeout=$1
	nLine=$2
	echo "nLine=""$nLine"
	echo "nTimeout=""$nTimeout"
	while [ "$nTimeout" -gt 0 ]; do
		FDSG_VALS=$(/opt/lantiq/bin/dsl_cpe_pipe.sh asg "$nLine")
		if echo "$FDSG_VALS"; then
			for k in $FDSG_VALS; do eval "$k" 2>/dev/null; done
			if [ "$nStatus" = "5" ]; then
				#echo "nStatus="$nStatus
				break
			elif [ "$nStatus" = "7" ]; then
				#echo "nStatus="$nStatus
				break
			fi
		fi
		nTimeout=$(("$nTimeout"-1))
		sleep 1
		echo "$nTimeout"
	done
}
###########################################################
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
	if [ "$soc_id" = "LGM" ]; then
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
	else
		if < /proc/driver/mei_cpe/devinfo grep "MaxDeviceNumber=2" 1> /dev/null 2> /dev/null; then
			dsl_bonding="bonding"
		else
			dsl_bonding="single"
		fi
	fi
}
############################################################

############################################################
# wlan power saving feature(s)
all_dsl_pm () {
	i=0
	for i in 0 1; do
		dsl_pm $i
	done

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
#GRX500#####################################################
# eth0_0 = 10G Aquantia PHY
# eth0_1 = 2.5G internal PHY
#GRX500#####################################################
# eth0_1 = GPHY_port2;	eth0_3 = GPHY_port4
# eth0_2 = GPHY_port3;	eth0_4 = GPHY_port5
############################################################
#LGS(LGM+P34X###############################################
# eth0_2 = GPHY_port0;	eth0_4 = GPHY_port2
# eth0_3 = GPHY_port1;	eth0_5 = GPHY_port3
############################################################
# all GPHYs/ethernet ports power saving feature(s)
# i: ethernet numbering
all_gphy_pm () {
	if [ "$soc_id" = "LGM" ]; then
		for i in 2 3 4 5; do
			gphy_pm $i
		done
	elif [ "$soc_id" = "PRXyyy" ]; then
		for i in 0 1; do
			gphy_pm $i
		done
	else
		for i in 1 2 3 4; do
			gphy_pm $i
		done
	fi	
}

############################################################
# GPHY/Ethernet power saving feature(s)
set_ghpy_id() {
	if [ "$soc_id" = "LGM" ]; then
		case "$1" in
			"1") echo "no valid 2.5G gphy id"
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
	elif [ "$soc_id" = "PRXyyy" ]; then
		case "$1" in
			"0") nGphy=0
				;;
			"1") nGphy=1
				;;
			*) echo "Unknown eth_id"
			   nGphy=99
				;;
		esac
	else
		case "$1" in
			"1") nGphy=2
				;;
			"2") nGphy=3
				;;
			"3") nGphy=4
				;;
			"4") nGphy=5
				;;
			"5") echo "no valid 1G gphy id"
				 nGphy=99
				;;
			*) echo "Unknown eth_id"
			   nGphy=99
				;;
		esac
	fi
}

gphy_pm () {
	phyctrl=`find /sys/devices/ -name \*p34x@0\*`
	nEth=$1
	set_ghpy_id $1
	if [ "$nGphy" = "99" ]; then
		return
	fi
	if [ "$soc_id" = "LGM" ]; then
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
	elif [ "$soc_id" = "PRXyyy" ]; then
		if [ "$level_pm" = "0" ]; then
			ethtool --set-eee eth0_"$nEth" eee off
			echo "GPHY power saving feature (EEE-Mode) will be disabled for all ports"
		elif [ "$level_pm" = "1" ]; then
			ethtool --set-eee eth0_"$nEth" eee on
			echo "GPHY power saving feature (EEE-Mode) will be enabled for all ports"
		else
			echo "status GPHYs:"
			echo ""
			echo "interface eth0_$nEth:"
			ethtool --show-eee eth0_$nEth
			ethtool eth0_$nEth | grep "Link detected:"
		fi
	else # GRX
		if [ "$level_pm" = "0" ]; then
			#ethtool --set-eee eth0_"$nEth" tx-lpi off
			ethtool --set-eee eth0_"$nEth" eee off
			echo "GPHY power saving feature (EEE-Mode) will be disabled for port$nGphy"
		elif [ "$level_pm" = "1" ]; then
			#ethtool --set-eee eth0_"$nEth" tx-lpi on
			ethtool --set-eee eth0_"$nEth" eee on
			echo "GPHY power saving feature (EEE-Mode) will be enabled for port$nGphy"
		elif [ "$level_pm" = "2" ]; then
			switch_cli GSW_MDIO_DATA_WRITE nAddressDev="$nGphy" nAddressReg=0 nData=0x1840 1> /dev/null 2> /dev/null
			echo "Disable GPHY. Force port$nEth into low power mode"
		elif [ "$level_pm" = "3" ]; then
			switch_cli GSW_MDIO_DATA_WRITE nAddressDev="$nGphy" nAddressReg=0 nData=0x1000 1> /dev/null 2> /dev/null
			echo "Enable GPHY. Bring port$nEth back into operational mode"
		else
			echo "status GPHY$nGphy:"
			ethtool --show-eee eth0_"$nEth" | grep "EEE status"
			ethtool eth0_"$nEth" | grep "Link detected:"

			if switch_cli GSW_MDIO_DATA_READ nAddressDev="$nGphy" nAddressReg=0 | grep -q "0x18"; then
				echo "eth0_$nEth in power down mode"
			else
				echo "eth0_$nEth in operational mode"
			fi
		fi
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
	if [ ! -f /sys/module/pcie_aspm/parameters/policy ]; then
		echo "ASPM cannot be changed because policy files does not exist"
		return
	fi
	if [ "$level_pm" = "0" ]; then
 		echo performance > /sys/module/pcie_aspm/parameters/policy
#		echo "Disable PCIe power saving feature (ASPM)"
	elif [ "$level_pm" = "1" ]; then
 		echo powersave > /sys/module/pcie_aspm/parameters/policy
		pcie_gen4_pm
#		echo "Enable PCIe power saving feature (ASPM)"
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
	if [ "$soc_id" = "LGM" ]; then
		if [ "$level_pm" = "0" ]; then
			echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
			echo performance > /sys/devices/system/cpu/cpufreq/policy1/scaling_governor
			echo performance > /sys/devices/system/cpu/cpufreq/policy2/scaling_governor
			echo performance > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
			echo 1 > /sys/devices/system/cpu/cpu0/cpuidle/state2/disable
			echo 1 > /sys/devices/system/cpu/cpu0/cpuidle/state3/disable
			echo 1 > /sys/devices/system/cpu/cpu0/cpuidle/state4/disable
			echo 1 > /sys/devices/system/cpu/cpu1/cpuidle/state2/disable
			echo 1 > /sys/devices/system/cpu/cpu1/cpuidle/state3/disable
			echo 1 > /sys/devices/system/cpu/cpu1/cpuidle/state4/disable
			echo 1 > /sys/devices/system/cpu/cpu2/cpuidle/state2/disable
			echo 1 > /sys/devices/system/cpu/cpu2/cpuidle/state3/disable
			echo 1 > /sys/devices/system/cpu/cpu2/cpuidle/state4/disable
			echo 1 > /sys/devices/system/cpu/cpu3/cpuidle/state2/disable
			echo 1 > /sys/devices/system/cpu/cpu3/cpuidle/state3/disable
			echo 1 > /sys/devices/system/cpu/cpu3/cpuidle/state4/disable
			echo "Disable CPU power saving feature (DVFS/CPUIDLE)"
		elif [ "$level_pm" = "1" ]; then
			echo powersave > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
			echo powersave > /sys/devices/system/cpu/cpufreq/policy1/scaling_governor
			echo powersave > /sys/devices/system/cpu/cpufreq/policy2/scaling_governor
			echo powersave > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
			echo 0 > /sys/devices/system/cpu/cpu0/cpuidle/state2/disable
			echo 0 > /sys/devices/system/cpu/cpu0/cpuidle/state3/disable
			echo 0 > /sys/devices/system/cpu/cpu0/cpuidle/state4/disable
			echo 0 > /sys/devices/system/cpu/cpu1/cpuidle/state2/disable
			echo 0 > /sys/devices/system/cpu/cpu1/cpuidle/state3/disable
			echo 0 > /sys/devices/system/cpu/cpu1/cpuidle/state4/disable
			echo 0 > /sys/devices/system/cpu/cpu2/cpuidle/state2/disable
			echo 0 > /sys/devices/system/cpu/cpu2/cpuidle/state3/disable
			echo 0 > /sys/devices/system/cpu/cpu2/cpuidle/state4/disable
			echo 0 > /sys/devices/system/cpu/cpu3/cpuidle/state2/disable
			echo 0 > /sys/devices/system/cpu/cpu3/cpuidle/state3/disable
			echo 0 > /sys/devices/system/cpu/cpu3/cpuidle/state4/disable
			echo "Enable CPU power saving feature (DVFS/CPUIDLE)"
		elif [ "$level_pm" = "2" ]; then
			echo "offline cpu1,cpu2,cpu3"
			echo 0 > /sys/devices/system/cpu/cpu1/online
			echo 0 > /sys/devices/system/cpu/cpu2/online
			echo 0 > /sys/devices/system/cpu/cpu3/online
		elif [ "$level_pm" = "3" ]; then
			echo "online cpu1,cpu2,cpu3"
			echo 1 > /sys/devices/system/cpu/cpu1/online
			echo 1 > /sys/devices/system/cpu/cpu2/online
			echo 1 > /sys/devices/system/cpu/cpu3/online
		else
			echo "status CPU:"
			cd /sys/devices/system/cpu/cpufreq
			grep -rvl "\x00" -- * | while read -r file; do printf "%s: " "$file"; cat "$file"; done
			echo ""
			echo "Online CPU's:"
			cat /sys/devices/system/cpu/online
			echo "Offline CPU's:"
			cat /sys/devices/system/cpu/offline
		fi
	else
		if [ "$level_pm" = "0" ]; then
			echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
			echo "Disable CPU power saving feature (DVFS)"
		elif [ "$level_pm" = "1" ]; then
			echo conservative > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
			echo "Enable CPU power saving feature (DVFS)"
		elif [ "$level_pm" = "2" ]; then
			echo "No power down mode for CPU available"
		elif [ "$level_pm" = "3" ]; then
			echo "No power down mode for CPU available"
		else
			echo "status CPU:"
			cd /sys/devices/system/cpu/cpufreq
			grep -rvl "\x00" -- * | while read -r file; do printf "%s: " "$file"; cat "$file"; done
		fi
	fi
}
############################################################


############################################################
# Main switch case
############################################################
#check SoC and set global Soc_id
if cat /proc/device-tree/model | grep -q "lightning mountain\|lgp\|octopus-641\|octopus-851\|osp-tb341\|osp-tb341-v2"; then
	soc_id="LGM"
elif cat /proc/device-tree/model | grep -q "GRX550"; then
	soc_id="GRX500"
elif cat /proc/device-tree/model | grep -q "GRX350"; then
	soc_id="GRX500"
elif cat /proc/device-tree/model | grep -q "PRX"; then
	soc_id="PRXyyy"
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
	"wlan6") wlan_pm 6
		;;
	"wlan8") wlan_pm 8
		;;
	"wlan10") wlan_pm 10
		;;
	"dsl") all_dsl_pm
		;;
	"dsl1") dsl_pm 1
		;;
	"dsl0") dsl_pm 0
		;;
	"dsl1") dsl_pm 1
		;;
	"eth") all_gphy_pm
		;;
	"eth0_0")
		if [ "$soc_id" = "PRXyyy" ]; then
			 gphy_pm 0
		fi
		;;
	"eth0_1") gphy_pm 1
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
	"adp")
		if [ "$soc_id" = "LGM" ]; then
			adp_pm
		fi
		;;
	"adp_sw")
		if [ "$soc_id" = "LGM" ]; then
			adp_sw_pm
		fi
		;;
	"adp_hw")
		if [ "$soc_id" = "LGM" ]; then
			adp_hw_pm
		fi
		;;
	"epu")
		if [ "$soc_id" = "LGM" ]; then
			epu_table
		fi
		;;
	*) echo "Unknown Parameter"
		;;
esac
exit 0
############################################################

