#!/bin/sh
#set -x

usage () {
        echo -e "\n     -t      Sets the time in seconds between prints - the default is 3s if this is not set"
        echo -e "               ex. ./wifitp -t 1       Will print out stats for all clients every 1 second"
        echo -e "               Note: With more than a few clients are connected, it may take more than a second to print"
        echo -e "               Note: Throughput and PPS stats may be off if the interval is set too high"
        echo -e "\n     -r      specify a radio [0 - wlan0.0, 2 - wlan2.0, 4 - wlan4.0]"
        echo -e "               ex. ./wifitp -r 0       Will print out stats for all clients connected to wlan0.0"
        echo -e "\n     -m      specify a mac address to show stats for only a unique client"
        echo -e "               ex. ./wifitp -m AA:BB:CC:DD:EE:FF       "
        echo -e "               ex. ./wifitp -m AA:BB:CC:DD:EE:FF -t 1"
        echo -e "\n     -u      1 = UGW platform (AXP) , 0 = RDKB platform (CGR)"
        echo -e "\n     -c      number of interations"
        echo -e "\n     -h      this help menu\n"
}

#support both UGW and RDKB
if [[ -f "/nvram/etc/config/wireless" ]]
then
	ugwplatform=0
else
	ugwplatform=1
fi

countinterations=0
countinterationstillexit=0

USDS_0="       Downstream:     "
USDS_1="       Upstream:       "
USDS_2="       TxPkt:          "
USDS_3="       RxPkt:          "
USDS_4="       RetranPkt:      "
ratestext_0="       Network(Phy):   "
ratestext_1="       BW(MHz):        "
ratestext_2="       MCSindex:       "
ratestext_3="       nSS:            "
ratestext_4="       PHYRate:        "
ratestext_5="       BeamForm:       "
ratestext_6="       TXpower:        "

flowtext_0="       TxRate:         "
flowtext_1="       RxRate:         "
flowtext_2="       RSSI:           "
flowtext_3="       Per-chain:      "
flowtext_4="       RSSI[0]:        "
flowtext_5="       RSSI[1]:        "
flowtext_6="       RSSI[2]:        "
flowtext_7="       RSSI[3]:        "
flowtext_8="       Util(%):        "
tmpval_0=0
tmpval_1=0
tmpval_2=0
tmpval_3=0
tmpval_4=0

#the mylast should be saved per MAC that is being tested for statistics
mylast_0=0
mylast_1=0
mylast_2=0
mylast_3=0
mylast_4=0

#these will store mylast_0 to mylast_4 for MAC index 0 to MAC index 9 - up to 20 MAC addresses supported
#note that it is for all radios, also note that mac list must not change while this script is running!
mac_str_0="0 0 0 0 0"
mac_str_1="0 0 0 0 0"
mac_str_2="0 0 0 0 0"
mac_str_3="0 0 0 0 0"
mac_str_4="0 0 0 0 0"
mac_str_5="0 0 0 0 0"
mac_str_6="0 0 0 0 0"
mac_str_7="0 0 0 0 0"
mac_str_8="0 0 0 0 0"
mac_str_9="0 0 0 0 0"
mac_str_10="0 0 0 0 0"
mac_str_11="0 0 0 0 0"
mac_str_12="0 0 0 0 0"
mac_str_13="0 0 0 0 0"
mac_str_14="0 0 0 0 0"
mac_str_15="0 0 0 0 0"
mac_str_16="0 0 0 0 0"
mac_str_17="0 0 0 0 0"
mac_str_18="0 0 0 0 0"
mac_str_19="0 0 0 0 0"


# a function that receives the string with all mylast values of the current mac and dispatches to the mylast variables
fillMyLast () {
		val_index=0
		for val in $1
		do
				case "$val_index" in
						0)      mylast_0=$val
						;;
						1)      mylast_1=$val
						;;
						2)      mylast_2=$val
						;;
						3)      mylast_3=$val
						;;
						4)      mylast_4=$val
						;;
				esac
				val_index=$(( $val_index + 1 ))
		done
}

while getopts ":t:m:r:u:c:h" opt; do
        case $opt in
                t) CustomSleep="$OPTARG"
                ;;
                m) macaddr="$OPTARG"
                ;;
                r) setrad="$OPTARG"
                ;;
                u) ugwplatform="$OPTARG"
                ;;
                c) countinterationstillexit="$OPTARG"
                ;;
                h)
                        usage
                        exit 1
                ;;
                :)      echo -e "Option -$OPTARG required an argument"
                        usage
                        exit 1
                ;;
                \?)     echo "Invalid option -$OPTARG. Use -h for available options" >&2
                        usage
                        exit 1
                ;;
        esac
done

if [ "$ugwplatform" == 0 ]
then
		echo "Running for RDKB (CGR)"
		while true; do rm /tmp/dwpal_command_get_ended_socket_* 2>/dev/null; sleep 5; done &
		cleanup_pid=$!
		trap "kill $cleanup_pid" EXIT
else
		echo "Running for UGW (AXP)"
fi

if [ -n "$CustomSleep" ]
then
        sleeptime=$CustomSleep
        [ -n "$sleeptime" ] && [ "$sleeptime" -eq "$sleeptime" ] 2>/dev/null
        if [ $? -ne 0 ]; then
                echo "Invalid sleep time '$sleeptime', must be an integer"
                echo "Setting sleep time to 3s"
                sleeptime=3
                sleep 2
        fi
else
        echo "Setting sleep time to 3s"
        sleeptime=3
fi

looptime=$sleeptime

radiolist="0 2 4"
singlemac=0
foundmac=0

if [ -n "$setrad" ]
then
        if [ -n "$macaddr" ] && [ -n "$setrad" ]
        then
                echo "There is no need to specify a mac and radio at the same time"
                echo "-r $setrad is not being used"
                sleep 2
        else
                case "$setrad" in
                        0)      if [ "$ugwplatform" == 1 ]
								then
										echo "Only using wlan0.1"
								else
										echo "Only using wlan0.0"
								fi
                                radiolist="0"
                                sleep 2
                        ;;
                        2)      if [ "$ugwplatform" == 1 ]
								then
										echo "Only using wlan2.2"
								else
										echo "Only using wlan2.0"
								fi
                                radiolist="2"
                                sleep 2
                        ;;
                        4)      if [ "$ugwplatform" == 1 ]
								then
										echo "Only using wlan4.3"
								else
										echo "Only using wlan4.0"
								fi
                                radiolist="4"
                                sleep 2
                        ;;
                        *)      echo "invalid radio used, please enter 0, 2 or 4"
                                exit
                        ;;
                esac
        fi
fi

if [ -n "$macaddr" ]
then
		if [ "$ugwplatform" == 1 ]
		then
				vapindex=1
		else
				vapintex=0
		fi
        for m in $(dwpal_cli wlan0.$vapindex PeerList | grep -A 100 'connected' | sed '1,2d' | sed '$d')
        do
				echo "m = $m\n"
				if [ "$macaddr" == "$m" ]
				then
						echo "Mac $m belongs to wlan0.$vapindex"
						radiolist="0"
						foundmac=1
				fi
        done
		if [ "$foundmac" == 0 ]
		then
				if [ "$ugwplatform" == 1 ]
				then
						vapindex=2
				else
						vapintex=0
				fi
				for m in $(dwpal_cli wlan2.$vapindex PeerList | grep -A 100 'connected' | sed '1,2d' | sed '$d')
				do
						echo "m = $m\n"
						if [ "$macaddr" == "$m" ]
						then
								echo "Mac $m belongs to wlan2.$vapindex"
								radiolist="2"
								foundmac=1
						fi
				done
		fi
		if [ "$foundmac" == 0 ]
		then
				if [ "$ugwplatform" == 1 ]
				then
						vapindex=3
				else
						vapintex=0
				fi
				for m in $(dwpal_cli wlan4.$vapindex PeerList | grep -A 100 'connected' | sed '1,2d' | sed '$d')
				do
						echo "m = $m\n"
						if [ "$macaddr" == "$m" ]
						then
								echo "Mac $m belongs to wlan4.$vapindex"
								radiolist="4"
								foundmac=1
						fi
				done
		fi
        if [ "$foundmac" == 1 ]
        then
                singlemac=1
        else
                echo "MacAddress was not found - please check entry and make sure the client is connected"
                exit 1
        fi
else
        singlemac=0
fi

while true
do
		mac_global_index=0
		counti=0
		countj=0
		countk=0
		echo "-----------------------------------------"
		echo "Time: $(date +%s)"
		for i in $radiolist
		do
				vapindex=0;
				if [ "$ugwplatform" == 1 ]
				then
					case "$i" in
							0)      vapindex=1;
							;;
							2)      vapindex=2;
							;;
							4)      vapindex=3;
							;;
					esac
				fi
				countj=0
				echo -e "\n\n==wlan$i Statistics=="
				dwpal_cli wlan$i HWFlowStatus | grep -E 'Airtime' | awk '{print "DutyCycle:     " $1}' | head -n 1
				iw dev wlan$i iwlwav gTemperature | cut -c 14-30 | awk '{print  "RF-Temp:       " $1}'
				#iw dev wlan$i iwlwav gPVT | cut -c 6-20 | awk '{print "BB-Temp:       "$2"\nBB-mV:         "$1}'
				##Builds loop array for for process
				maclist=""
				if [ "$singlemac" == 1 ]
				then
						maclist="$maclist $macaddr"
				else
						mindex=0
						for m in $(dwpal_cli wlan$i.$vapindex PeerList | grep -A 100 'connected' | sed '1,2d' | sed '$d')
						do
								maclist="$maclist $m"
						done
				fi

				# echo "dgabay maclist = $maclist\n"
				for j in $maclist
				do
						echo -e "\nMac:    $j"
						mycount=0
						countk=0
						#read the mylast values from the relevant mac_str array
						# echo "dgabay reading mylast variables from mac_str with index $mac_global_index"
						val_index=0
						case "$mac_global_index" in
								0)      fillMyLast "$mac_str_0"
								;;
								1)      fillMyLast "$mac_str_1"
								;;
								2)      fillMyLast "$mac_str_2"
								;;
								3)      fillMyLast "$mac_str_3"
								;;
								4)      fillMyLast "$mac_str_4"
								;;
								5)      fillMyLast "$mac_str_5"
								;;
								6)      fillMyLast "$mac_str_6"
								;;
								7)      fillMyLast "$mac_str_7"
								;;
								8)      fillMyLast "$mac_str_8"
								;;
								9)      fillMyLast "$mac_str_9"
								;;
								10)     fillMyLast "$mac_str_10"
								;;
								11)     fillMyLast "$mac_str_11"
								;;
								12)     fillMyLast "$mac_str_12"
								;;
								13)     fillMyLast "$mac_str_13"
								;;
								14)     fillMyLast "$mac_str_14"
								;;
								15)     fillMyLast "$mac_str_15"
								;;
								16)     fillMyLast "$mac_str_16"
								;;
								17)     fillMyLast "$mac_str_17"
								;;
								18)     fillMyLast "$mac_str_18"
								;;
								19)     fillMyLast "$mac_str_19"
								;;
						esac
						# echo "dgabay mylast_0 = $mylast_0, mylast_1 = $mylast_1, mylast_2 = $mylast_2, mylast_3 = $mylast_3, mylast_4 = $mylast_4"
						##Loop for some PeerFlowStats
						newflowtext=$(dwpal_cli wlan$i.$vapindex PeerFlowStatus $j)
						for k in $(echo "$newflowtext" | grep -E 'Bytes|Packets|: RetryCount' | awk '{print $1}')
						do
								#0 and 1 are for bytecounters
								if [ "$countk" -lt  2 ]
								then
										#Check for bytecount reset

										if [ "$countk" == 0 ]
										then
												checkval=$(echo "$k ${mylast_0}" | awk '{print ($1 - $2)}' 2>/dev/null)
										else
												checkval=$(echo "$k ${mylast_1}" | awk '{print ($1 - $2)}' 2>/dev/null)
										fi
										if [ "$checkval" -ge 0 ]
										then
												if [ "$countk" == 0 ]
												then
														tmpval_0=$(echo "$k ${mylast_0} $looptime" | awk '{print ($1 - $2)*8 / 1000000 / $3}' 2>/dev/null)
														echo "${USDS_0} ${tmpval_0} Mbps"
												else
														tmpval_1=$(echo "$k ${mylast_1} $looptime" | awk '{print ($1 - $2)*8 / 1000000 / $3}' 2>/dev/null)
														echo "${USDS_1} ${tmpval_1} Mbps"
												fi
										else
												if [ "$countk" == 0 ]
												then
														tmpval_0=$(echo "$k ${mylast_0} $looptime" | awk '{print ((4294967295 - $2) + $1)*8 / 1000000 / $3}' 2>/dev/null)
														echo "${USDS_0} ${tmpval_0} Mbps"
												else
														tmpval_1=$(echo "$k ${mylast_1} $looptime" | awk '{print ((4294967295 - $2) + $1)*8 / 1000000 / $3}' 2>/dev/null)
														echo "${USDS_1} ${tmpval_1} Mbps"
												fi
										fi
								else
										if [ "$countk" == 2 ]
										then
												tmpval_2=$(echo "$k ${mylast_2} $looptime" | awk '{print ($1 - $2) / $3 }' 2>/dev/null)
												echo "${USDS_2} ${tmpval_2} pps"
										else
												if [ "$countk" == 3 ]
												then
														tmpval_3=$(echo "$k ${mylast_3} $looptime" | awk '{print ($1 - $2) / $3 }' 2>/dev/null)
														echo "${USDS_3} ${tmpval_3} pps"
												else
														tmpval_4=$(echo "$k ${mylast_4} $looptime" | awk '{print ($1 - $2) / $3 }' 2>/dev/null)
														echo "${USDS_4} ${tmpval_4} pps"
												fi
										fi
								fi
								case "$countk" in
										0)      mylast_0=$k
										;;
										1)      mylast_1=$k
										;;
										2)      mylast_2=$k
										;;
										3)      mylast_3=$k
										;;
										4)      mylast_4=$k
										;;
								esac
								countk=$(( $countk + 1 ))
						done
						percent_rt=$(echo "${tmpval_4} ${tmpval_2}" | awk '{print ($1 / $2)*100}' 2>/dev/null || echo 0)
						echo "  %RT:             ${percent_rt}%"
						echo "  Tx_PacketSize:   $(echo "${tmpval_0} ${tmpval_2}" | awk '{print ($1/$2)*125000}' 2>/dev/null)"
						echo "  Rx_PacketSize:   $(echo "${tmpval_1} ${tmpval_3}" | awk '{print ($1/$2)*125000}' 2>/dev/null)"
						##PeerRatesInfoStats
						mycount=0
						for k in $(dwpal_cli wlan$i.$vapindex PeerRatesInfo $j | grep -A 20 'Data downlink' | grep -E 'Network|MCS|downlink|NSS|TX po|BW [[]|Beam' | tail -7 | awk '{print $1}')
						do
								case "$mycount" in
										0)      echo "${ratestext_0} $k"
										;;
										1)      echo "${ratestext_1} $k"
										;;
										2)      echo "${ratestext_2} $k"
										;;
										3)      echo "${ratestext_3} $k"
										;;
										4)      echo "${ratestext_4} $k"
										;;
										5)      echo "${ratestext_5} $k"
										;;
										6)      echo "${ratestext_6} $k"
										;;
								esac
								mycount=$(( $mycount + 1 ))
						done

						##PeerFlowStatus (For RSSI)
						mycount=0
						for k in $(echo "$newflowtext" | grep -A 7 'LastDataDownlinkRate' | tail -8 | awk '{print $1}')
						do
								if [ "$mycount" -gt 1 ]
								then
										if [ "$mycount" -ne 3 ]
										then
												case "$mycount" in
														2)      echo "${flowtext_2} $k"
														;;
														4)      echo "${flowtext_4} $k"
														;;
														5)      echo "${flowtext_5} $k"
														;;
														6)      echo "${flowtext_6} $k"
														;;
														7)      echo "${flowtext_7} $k"
														;;
														8)      echo "${flowtext_8} $k"
														;;
												esac
										fi
								else
										echo "${flowtext_0} $(echo "$k" | awk '{print $1 / 1000}')"
								fi
								mycount=$(( $mycount + 1 ))
						done
						echo "$newflowtext" | grep AirtimeUsage | awk '{print "       AT_Util(%):      " $1}'
						countj=$(( $countj + 1 ))
						#store the mylast values to the relevant mac_str array
						case "$mac_global_index" in
								0)      mac_str_0=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								1)      mac_str_1=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								2)      mac_str_2=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								3)      mac_str_3=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								4)      mac_str_4=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								5)      mac_str_5=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								6)      mac_str_6=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								7)      mac_str_7=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								8)      mac_str_8=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								9)      mac_str_9=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								10)     mac_str_10=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								11)     mac_str_11=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								12)     mac_str_12=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								13)     mac_str_13=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								14)     mac_str_14=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								15)     mac_str_15=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								16)     mac_str_16=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								17)     mac_str_17=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								18)     mac_str_18=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
								19)     mac_str_19=""$mylast_0" "$mylast_1" "$mylast_2" "$mylast_3" "$mylast_4""
								;;
						esac
						mac_global_index=$(( $mac_global_index + 1 ))
						if [ "$mac_global_index" -gt 19 ]
						then
								echo "Reached the limit of 20 supported MAC addresses!"
								mac_global_index=19
						fi
				done
				counti=$(( $counti + 1 ))
		done
		if [ "$countinterationstillexit" -gt 0 ]
		then
			countinterations=$(( $countinterations + 1 ))
			if [ "$countinterations" -ge $countinterationstillexit ]
			then
					echo "Reached the num iterations limit, exiting"
					exit 0
			fi
		fi
		sleep $sleeptime
done
