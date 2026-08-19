#!/bin/sh
# Program:
#       Config LED behavior during boot-up and interface connect/disconnect
# History:
# 2021/11/26    Ray Chang First release

#$1: interface
#$2: brightness: 0~255, 0-disable, 255-enable netdev trigger; value between 0-255:for wifi only, set weak light when wifi not associated.
netdev_trigger_enable()
{
    case $1 in
    eth0_1)
        leddir="led20:green:lan_sfp"
        ;;
    wlan0.1)
        leddir="led21:green:2.4G"
        ;;
    wlan2.2)
        leddir="led22:green:5G low"
        ;;
    wlan4.3)
        leddir="led23:green:5G high"
        ;;
    *)
        #echo "else return"
        return
        ;;
    esac

    if [ "$2" -eq 0 ]; then
        triggerStatus="$(cat /sys/class/leds/"$leddir"/trigger)"
        value="${triggerStatus##*[}"
        value="${value%]*}"
        [ "$value" != "none" ] &&
        {
            echo none > /sys/class/leds/"$leddir"/trigger
            echo 0 > /sys/class/leds/"$leddir"/brightness;
        }
    elif [ "$2" -ne 0 -a "$2" -ne 255  ]; then
        briValue=`cat /sys/class/leds/"$leddir"/brightness`
        [ "$briValue" -eq 0 ] || [ "$briValue" -eq 255 ] &&
        {
            echo netdev > /sys/class/leds/"$leddir"/trigger
            echo $1 > /sys/class/leds/"$leddir"/device_name
            echo 1 > /sys/class/leds/"$leddir"/link
            echo 0 > /sys/class/leds/"$leddir"/tx
            echo 0 > /sys/class/leds/"$leddir"/rx
            echo "$2" > /sys/class/leds/"$leddir"/brightness
        }
    else
        [ ! -f /sys/class/leds/"$leddir"/link ] || [ `cat /sys/class/leds/"$leddir"/brightness` != "255" ] &&
        {
            echo netdev > /sys/class/leds/"$leddir"/trigger
            echo $1 > /sys/class/leds/"$leddir"/device_name
            echo 1 > /sys/class/leds/"$leddir"/link
            echo 1 > /sys/class/leds/"$leddir"/tx
            echo 1 > /sys/class/leds/"$leddir"/rx
            cat /sys/class/leds/"$leddir"/max_brightness > /sys/class/leds/"$leddir"/brightness
        }
    fi
}

netdev_trigger_enable_WAN()
{
    case $1 in
    255)
        [ ! -f "/sys/class/leds/led16:red:wan/device_name" ] || [ $4 != `cat /sys/class/leds/led16:red:wan/device_name` ] &&
        {
            echo netdev > /sys/class/leds/led16:red:wan/trigger
            echo $4 > /sys/class/leds/led16:red:wan/device_name
            echo 1 > /sys/class/leds/led16:red:wan/link
            echo 1 > /sys/class/leds/led16:red:wan/tx
            echo 1 > /sys/class/leds/led16:red:wan/rx
            cat /sys/class/leds/led16:red:wan/max_brightness > /sys/class/leds/led16:red:wan/brightness
        }
        ;;
    0)
        triggerStatus="$(cat /sys/class/leds/led16:red:wan/trigger)"
        value="${triggerStatus##*[}"
        value="${value%]*}"
        [ "$value" != "none" ] &&
        {
            echo none > /sys/class/leds/led16:red:wan/trigger
            echo 0 > /sys/class/leds/led16:red:wan/brightness;
        }
        ;;
    esac

    case $2 in
    255)
        [ ! -f "/sys/class/leds/led17:green:wan/device_name" ] || [ $4 != `cat /sys/class/leds/led17:green:wan/device_name` ] &&
        {
            echo netdev > /sys/class/leds/led17:green:wan/trigger
            echo $4 > /sys/class/leds/led17:green:wan/device_name
            echo 1 > /sys/class/leds/led17:green:wan/link
            echo 1 > /sys/class/leds/led17:green:wan/tx
            echo 1 > /sys/class/leds/led17:green:wan/rx
            cat /sys/class/leds/led17:green:wan/max_brightness > /sys/class/leds/led17:green:wan/brightness
        }
        ;;
    0)
        triggerStatus="$(cat /sys/class/leds/led17:green:wan/trigger)"
        value="${triggerStatus##*[}"
        value="${value%]*}"
        [ "$value" != "none" ] &&
        {
            echo none > /sys/class/leds/led17:green:wan/trigger
            echo 0 > /sys/class/leds/led17:green:wan/brightness;
        }
        ;;
    esac

    case $3 in
    255)
        [ ! -f "/sys/class/leds/led18:blue:wan/device_name" ] || [ $4 != `cat /sys/class/leds/led18:blue:wan/device_name` ] &&
        {
            echo netdev > /sys/class/leds/led18:blue:wan/trigger
            echo $4 > /sys/class/leds/led18:blue:wan/device_name
            echo 1 > /sys/class/leds/led18:blue:wan/link
            echo 1 > /sys/class/leds/led18:blue:wan/tx
            echo 1 > /sys/class/leds/led18:blue:wan/rx
            cat /sys/class/leds/led18:blue:wan/max_brightness > /sys/class/leds/led18:blue:wan/brightness
        }
        ;;
    0)
        triggerStatus="$(cat /sys/class/leds/led18:blue:wan/trigger)"
        value="${triggerStatus##*[}"
        value="${value%]*}"
        [ "$value" != "none" ] &&
        {
            echo none > /sys/class/leds/led18:blue:wan/trigger
            echo 0 > /sys/class/leds/led18:blue:wan/brightness;
        }
        ;;
    esac
}

wan_led_change()
{
    local dflt_route interface value
    local color1 color2 color3

    dflt_route="$(sed -n '2p' /proc/net/route)"
    interface="${dflt_route//[[:space:]]*}"
    value="${dflt_route#*[[:space:]]}"
    value="${value//[[:space:]]*}"

    #WAN
    if [ "$value" = "00000000" ]; then
        #WAN color: PON(255,255,255), DSL(0,0,255), 10G-ETH(255,0,255), Boot-Up(255,255,0)
        if [[ "$interface" =~ "ptm0" ]] || [[ "$interface" =~ "nas0" ]];then
            color1=0
            color2=0
            color3=255
        elif [[ "$interface" =~ "ani0" ]];then
            color1=255
            color2=255
            color3=255
        elif [[ "$interface" =~ "eth1" ]] || [[ "$interface" =~ "pppoe" ]];then
            color1=255
            color2=0
            color3=255
        fi
    else
        color1=255
        color2=255
        color3=0
        interface="NoWANup"
    fi
    netdev_trigger_enable_WAN $color1 $color2 $color3 $interface
}


    inf_Def_Array="wlan0.1 wlan2.2 wlan4.3"

    while sleep 1
    do
        # check WIFI associated status
        echo "$inf_Def_Array" | tr ' ' '\n' | while read item; do
        [ -d /sys/class/net/"$item"/ ] && [ `cat /sys/class/net/$item/carrier` == 1 ] &&
        {
            strAsso=`dwpal_cli $item peerlist | grep connected | awk '{print $NF}'`
            if [ "connected:" = $strAsso ]; then
                netdev_trigger_enable $item 255
            else
                netdev_trigger_enable $item 10
            fi
        }
        done

        # check Lan-SFP link status
        [ -d /sys/class/net/eth0_1/ ] &&
        {
            LanSfp=`ethtool eth0_1`
            if [ "${LanSfp##*: }" == "yes" ]; then
                netdev_trigger_enable eth0_1 255
            else
                netdev_trigger_enable eth0_1 0
            fi
        }

        # check WAN status
        WanSfp=`ethtool eth1`
        [ "${WanSfp##*: }" == "yes" ] || [ `cat /sys/class/net/ptm0/carrier` == 1 ] &&
        {
            wan_led_change
        }
    done

