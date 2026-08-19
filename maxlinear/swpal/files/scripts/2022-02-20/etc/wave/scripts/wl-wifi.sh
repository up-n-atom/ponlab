#!/bin/sh

source /opt/intel/wave/scripts/wnc_wifi_helper.sh

# Maxlinear version of the script.
# Original script provided by WNC.
# Current Version
version="v1.2" #Important: Whenever change to new version, update the changelog in dut_print_history


wifiHistory()
{
	printInfo "Revision History:"
	printInfo "wifi.sh current version $version"
	dut_print_history

}

########################################################################
#
# This script is as an interface to the wireless configuration utilities.
#
# NOTE: As currently written can only be used for Broadcom interfaces
#       that use the WL commands
########################################################################
# Paths and binaries


########################################################################
#                              Utilities                               #
########################################################################
#=======================================================================
# Syntax: printInfo [message]
#
# Prints the information message given
#=======================================================================
printInfo()
{
	echo -e "[wlWifi INFO]  $@"
}

#=======================================================================
# Syntax: printVersion []
#
# Prints the firmware ver, wireless driver version, firmware id, nvram version, hw version.....
#=======================================================================
printVersion()
{
	# WIFI driver version
	dut_print_version
}

########################################################################
#                               Functions                              #
########################################################################

#=======================================================================
# Syntax: wifiInit
#
# Makes sure that the drivers are installed and the module is in station
# mode.
#
# This function must succeed if the WiFi driver is already up and ready
#=======================================================================
wifiInit()
#New Init function for the BGW210 from Arris
{
	printInfo "wifiInit"
	dut_wifi_init
}

#=======================================================================
# Syntax: wifiTerm
#
# mark WiFi down (disabled)
#=======================================================================
wifiTerm()
{
	printInfo "WiFi down."
	dut_wifi_terminate
}

#=======================================================================
# Syntax: wifiRadioUp [radio band]
#
# Brings up Wi-Fi interface per radio band.
#
# Inputs:
# - interface: 2.4, 5, 6
#=======================================================================
wifiRadioUp()
{
	printInfo "wifiRadioUp"
	dut_wifi_radio_up $@
}

#=======================================================================
# Syntax: wifiRadioDown [radio band]
#
# Brings down Wi-Fi radio band
#
# Inputs:
# - radio band: 2.4, 5, 6
#=======================================================================
wifiRadioDown()
{
	printInfo "wifiRadioDown"
	dut_wifi_radio_down $@
}

#=======================================================================
# Syntax: wifiVersion
#
# Prints the version of the current wifi.sh
#=======================================================================
wifiVersion()
{
	printInfo "wifi.sh version $version"

	# display Various information
	printVersion
}

#=======================================================================
# Syntax: wifiStatus
#
# Inputs:
# - radio band: 2.4, 5, 6
#=======================================================================
wifiStatus()
{
	printInfo "wifiStatus"
	dut_print_radio_status $@
}

# ./wl-wifi.sh cwwave 5 36 15 40 1
#=======================================================================
# Syntax: wifiCWWave <radio band> <channel> <antennas> <power> <offset>
#
# Transmits a CW waveform on the selected channel
#
# Inputs:
# - radio band: 2.4, 5, 6
# - channel: channel to use to transmit the CW wave
# - antennas : 15 = 1111 means 4 antennas supported, corresponding antenna is enabled if bit is 1
# - power : power index
# - offset : the actual value is twice of the input value (1 --> 2kHz 2 --> 4kHz 3 --> 6kHz)
#=======================================================================
wifiCWWave()
{
	dut_start_cw $@
}

#./wl-wifi.sh rxMeasure 5 36 20 1 ax 01:00:00:C0:FF:EE 1000 10000
#=======================================================================
# Syntax: wifiRxMeasure <radio band> <channel> <bandwidth> <Antenna Selection> <Protocol>
# [Destination MAC Address] [Number of captures] [capture interval]
# Sets chip to receive a modulated signal on the selected channel
# This function should only count packets with a destination MAC address
#       of 01:00:00:C0:FF:EE in the packet header.
#       Note: MAC address of wifi module for testing purposes is set to 01:00:00:C0:FF:EE
#       Reports PER based on 10000 Packet input
#
# Inputs:
# - radio band: 2.4, 5, 6
# - channel: channel to use to transmit
# - bandwidth: bandwidth to use, 20 or 40  or 80 or 160 MHz
# - antenna #: sets antennas to use, takes a number such that in binary form, bit0 controls ANT1, bit1 controls ANT2, bit2 controls ANT3 and bit3 controls ANT4. If it is a 0, the selected antenna is to be disabled.  If it is a 1, the selected antenna is to be enabled.
# - protocol: 802.11 flavor:  a b g or n or ac or ax
# - Destination MAC Address:
# - Number of captures:
# - capture interval: ms
#=======================================================================
wifiRxMeasure()
{
	dut_rx_measure $@
}

#=======================================================================
# Syntax: normalTxMode [] for b+g+n+a+ac+ax(2.4GHz)
# in_band : 2 : 2.4GHz , 5:5GHz
# fix_channel : 1, 2, 3,4,5, 36, 40, 44, 48 etc.....
# fix_bandwidth : 20, 40, 80 160
# fix_power : power of txmode
# fix_antsel : 1, 2, 4, 8, etc.....
# fix_nss : spatial stream 
# fix_ifs : interframe space
# fix_psdu_size : frame size
# fix_protocol : a, b, g, n, ac 
# fix_mcsi : rate or mcs index 
# fix_sendtoframe : frame count 
# ru_size : 26 / 52 / 106 / 242 / 484 / 996 / 1992
# ru_location : Low / Mid / High / Edges
#=======================================================================
normalTxMode()
{
	printInfo "normalTxMode"
}


#./wl-wifi.sh txMode 5 36 20 10.25 1 1 30 1500 ax 0 0
#=======================================================================
# Syntax: wifiTxMode <radio band> <channel> <bandwidth> <power> <antenna selection> <number of spatial streams (NSS)>
# <Interframe Space ( IFS )> <PSDU Size>  <Protocol> <MCS Index | Data Rate> <Number of Frames to Send> [<RU size> <RU location>]
#
# Transmits a modulated signal on the selected channel
#
# Inputs:
# - radio band: 2.4, 5, 6 
# - channel: channel to use to transmit
# - bandwidth: bandwidth to use, 20 or 40  or 80 or 160 MHz
# - power: set Power of TxMode
# - antenna #: sets antennas to use, takes a number such that in binary form, bit0 controls ANT1, bit1 controls ANT2, bit2 controls ANT3 and bit3 controls ANT4. If it is a 0, the selected antenna is to be disabled.  If it is a 1, the selected antenna is to be enabled.
# - NSS : number of spatial streams , 1,2,3,4
# - IFS: inter-packet gap, in microseconds
# - PSDU Size : 
# - protocol: 802.11 flavor:  a b g or n or ac or ax
# - modulation index: If 802.11n, which MCS index (0-31), If 802.11ac, which MCS index (0-9), If 802.11ax, which MCS index (0-11), if not, enter data rate in Mbps
# - number of frames to send : 
# - RU size : 26 / 52 / 106 / 242 / 484 / 996 / 1992
# - RU location : Low / Mid / High / Edges
#=======================================================================
wifiTxMode()
{
	dut_start_tx $@
}

#./wl-wifi.sh rxPER 5 36 20 8 ax 1000 10000 01:00:00:C0:FF:EE
#=======================================================================
# Syntax: wifiRxPER <radio band> <channel> <bandwidth> <Antenna Selection> <Protocol>
# <Number of Frames to Receive> <Idle Duration> <Destination MAC Address>
# Sets chip to receive a modulated signal on the selected channel
# This function should only count packets with a destination MAC address
#       of 01:00:00:C0:FF:EE in the packet header.
#       Note: MAC address of wifi module for testing purposes is set to 01:00:00:C0:FF:EE
#       Reports PER based on 10000 Packet input
#
# Inputs:
# - radio band: 2.4, 5, 6
# - channel: channel to use to transmit
# - bandwidth: bandwidth to use, 20 or 40  or 80 or 160 MHz
# - antenna #: sets antennas to use, takes a number such that in binary form, bit0 controls ANT1, bit1 controls ANT2, bit2 controls ANT3 and bit3 controls ANT4. If it is a 0, the selected antenna is to be disabled.  If it is a 1, the selected antenna is to be enabled.
# - protocol: 802.11 flavor:  a b g or n or ac or ax
# - number of frames to Receive : 
# - Destination MAC Address : 
#=======================================================================
wifiRxPER()
{
	dut_calculate_per $@
}

# ./wl-wifi.sh txModeStop
#=======================================================================
# Syntax: wifiTxModeStop [radio band]
#
# Stops transmission of modulated signal
#
# Inputs:
# - radio band: 2.4, 5, 6
#
#=======================================================================
wifiTxModeStop()
{
	printInfo "wifiTxModeStop"
	dut_stop_tx $@
}

# ./wl-wifi.sh cwwaveStop
#=======================================================================
# Syntax: wifiCWWaveStop [radio band]
#
# Stops the CW wave test
#
# Inputs:
# - radio band: 2.4, 5, 6
#
#=======================================================================
wifiCWWaveStop()
{
	printInfo "wifiCWWaveStop"
	dut_stop_cw $@
}

#=======================================================================
# Syntax: wifiReset
#
# Resets the wifi Module to default state
#
# Inputs:
# none
#=======================================================================
wifiReset()
{
	wifiTerm

	sleep 2

	wifiInit
}

# ./wl-wifi.sh APscan [radio band] [scan interval]
#=======================================================================
# Syntax: wifiAPscan [radio band] [scan interval]
#
# Scan for BSS
#
# Inputs:
# - radio band: 2.4, 5, 6
# - scan interval: Set the period to listen.
#=======================================================================
wifiAPscan()
{
	printInfo "wifiAPscan"
	dut_start_ap_scan $@
}


#=======================================================================
# Syntax: wifiHelp
#
# Prints the command usage of wifi Script
#
# Inputs:
# - none
#=======================================================================
wifiHelp ()
{
	printInfo ""
	printInfo "  version          : Prints the version of the Script, firmware, driver, etc."
	printInfo ""
	printInfo "  history          : Prints the Revision history of wifi Script"
	printInfo ""
	printInfo "  help             : Prints the command usage of wifi Script"
	printInfo ""
	printInfo "  init             : Init and make wifi module in AP mode"
	printInfo ""
	printInfo "  term             : Terminate and make wifi module down"
	printInfo ""
	printInfo "  reset            : Resets the wifi module to default state"
	printInfo ""
	printInfo "  status           : Prints details for current connection."
	printInfo "                     # Syntax : status <radio band>"
	printInfo "                       e.g. status 2.4"
	printInfo ""
	printInfo "  txMode           : Transmits a modulated signal on the selected channel"
	printInfo "                     # Syntax : txMode <radio band> <channel> <bandwidth> <power> <antenna selection> <number of spatial streams (NSS)> <Interframe Space ( IFS )> <PSDU Size>  <Protocol> <MCS Index | Data Rate> <Number of Frames to Send> [<RU size> <RU location>]"
	printInfo "                       e.g. txMode 5 36 20 10.25 1 1 30 1500 ax 0 0"
	printInfo "                       e.g. [AX only] txMode 5 36 160 19 15 1 100 1000 ax 11 0 52 Mid"
	printInfo ""
	printInfo "  txModeStop       : Stops single band transmission of modulated signal"
	printInfo "                     # Syntax : txModeStop [radio band]"
	printInfo ""
	printInfo "  rxPER            : Reports PER based on Rx Packet number input default base on 10000"
	printInfo "                     # Syntax : rxPER <radio band> <channel> <bandwidth> <Antenna Selection> <Protocol> <Number of Frames to Receive> <Idle Duration> <Destination MAC Address>"
	printInfo "                       e.g. rxPER 5 36 20 1 ax 20000 0 01:00:00:C0:FF:EE"
	printInfo ""
	printInfo "  cwwave           : Transmits a CW waveform on the selected channel with offset"
	printInfo "                     # Syntax : cwwave <radio band> <channel> <antennas> <power> <offset>"
	printInfo "                       e.g. cwwave 5 36 15 19 1"
	printInfo ""
	printInfo "  cwwaveStop       : Stops the CW wave test"
	printInfo "                     # Syntax : cwwaveStop [radio band]"
	printInfo ""
	printInfo "  rxMeasure        : Sets chip to measure RSSI on current channel."
	printInfo "                     # Syntax : rxMeasure <radio band> <channel> <bandwidth> <Antenna Selection> <Protocol> [Destination MAC Address] [Number of captures] [capture interval]"
	printInfo "                       e.g. rxMeasure 5 36 20 1 ax"
	printInfo ""
	printInfo "  wifiRadioUp      : Brings up Wi-Fi per radio band."
	printInfo "                     # Syntax : wifiRadioUp [radio band]"
	printInfo "                       e.g. wifiRadioUp 5"
	printInfo ""
	printInfo "  wifiRadioDown    : Brings down Wi-Fi per radio band."
	printInfo "                     # Syntax : wifiRadioDown [radio band]"
	printInfo "                       e.g. wifiRadioDown 5"
	printInfo ""
	printInfo "=========================================== Parameter Note ================================================="
	printInfo "  [radio band]     - 2.4, 5, 6"
	printInfo "  [antennas]       - 15 = 0x1111 means 4 antennas supported, corresponding antenna is enabled if bit is 1"
	printInfo "  [protocol]       - n : 11n / ac : 11ac / ax : 11ax / a : 11a / g : 11g / b : 11b"
	printInfo "  [bandwidth]      - 20 : 20 MHz / 40 : 40 MHz / 80 : 80 MHz / 160 : 160 MHz"
	printInfo "                     Please matching the channel and bandwidth"
	printInfo "  [mcs index]      - ax  : mcsi in 0~11 nss in 1~4"
	printInfo "                     ac  : mcsi in 0~9  nss in 1~4"
	printInfo "                     n   : mcsi in 0~7 nss set to 1, 8~15 nss set to 2, 16~23 nss set to 3, 24~31 nss set to 4"
	printInfo "                     a/g : Legacy OFDM rate: 6/9/12/18/24/36/48/54  nss set to 1"
	printInfo "                     b   : Legacy OFDM rate: 1/2/5.5/11  nss set to 1"
	printInfo "  [MAC Address]    - Default MAC Address is 01:00:00:C0:FF:EE when this is set to '-' or '0'."
	printInfo "  [offset]         - the actual value is twice of the input value. Note: offset is 0 means stop!"
	printInfo "                       e.g. 1 --> 2kHz 2 --> 4kHz 3 --> 6kHz"
	printInfo "  [RU Size]        - This ONLY applies when [protocol] is set to 'ax'."
	printInfo "                     If the [protocol] is not set to 'ax' this parameter as well as <RU Location> will be omitted."
	printInfo "                     If this parameter is set to '0', it will not use partial RU loading."
	printInfo "                       Accepted values : 0, 26, 52, 106, 242, 484, 996"
	printInfo "  [RU Location]    - This ONLY applies when [protocol] is set to 'ax'."
	printInfo "                     If [RU Size] is set to '0' or the [protocol] is not set to 'ax', this parameter shall be omitted."
	printInfo "                       Low   : Lowest RU (location based) within the allocated bandwidth for the Wi-Fi channel selected."
	printInfo "                       Mid   : The RU is located in the middle (frequency allocation) within the allocationed bandwidth for the Wi-Fi channel selected."
	printInfo "                       High  : Highest RU (location based) within the allocated bandwidth for the Wi-Fi channel selected."
	printInfo "                       Edges : The lowest AND highest RU within the allocated bandwidth for the Wi-Fi channel selected."
	printInfo "============================================ *** End *** ==================================================="
	printInfo ""
}

########################################################################
#                             Script Main                              #
########################################################################

# Param 1 is the function, the rest the params for that function
func=$1
shift
params="$@"

# Call the function given
case $func in
	"init")                         wifiInit $params ;;
	"term")                         wifiTerm $params ;;
	"wifiRadioUp")                  wifiRadioUp $params ;;
	"wifiRadioDown")                wifiRadioDown $params ;;
	"status"|"Status")              wifiStatus $params ;;
	"txMode")                       wifiTxMode $params ;;
	"txModeStop")                   wifiTxModeStop $params ;;
	"rxPER")                        wifiRxPER $params ;;
	"rxMeasure")                    wifiRxMeasure $params ;;
	"cwwave"|"CWWave")              wifiCWWave $params ;;
	"cwwaveStop"|"CWWaveStop")      wifiCWWaveStop $params ;;
	"reset")                        wifiReset $params ;;
	"version")                      wifiVersion $params ;;
	"history")                      wifiHistory $params ;;
	"APscan")                       wifiAPscan $params ;;
	"help")                         wifiHelp $params ;;
	*) ;;
esac

# Exit if script not sourced
[ "$0" != "-sh" ] && exit $WIFI_OK

