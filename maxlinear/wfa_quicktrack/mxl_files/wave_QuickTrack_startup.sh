#!/bin/sh
# QuickTrack startup script

. /lib/wifi/platform_dependent.sh
. "$SCRIPTS_PATH/common_utils.sh"

ROLE=$1
BOARD=`cat /proc/device-tree/model | cut -d"," -f2`

if [ "$ROLE" != "afc" ]; then
        echo "bring down hostapd and app's"
        if [ "$OS_NAME" = "URDKB" ] || [ "$OS_NAME" = "RDKB" ]; then
                wifi down
        fi
        killall wfa_quicktrack > /dev/null
        killall hostapd > /dev/null
        sleep 2

        if [ "$OS_NAME" = "URDKB" ] || [ "$OS_NAME" = "RDKB" ]; then
                echo "stop rdkb services"
                systemctl stop ccspwifiagent
                systemctl stop systemd-prplmesh-sub-agent
                systemctl stop systemd-prplmesh-sub-controller
                systemctl stop systemd-dwpal_daemon.service

                echo "reload wave-driver"
                wave-drv stop
                wave-drv init
        else
                echo "stop services"
                /etc/init.d/prplmesh stop
                /etc/init.d/amx-processmonitor stop
                sleep 5
                pid=`ps -eF | grep amx-processmonitor | grep -v grep | awk '{ print $2}'`
                while [ -n "$pid" ]; do
                        ap_tmp=`eval kill -9 $pid`
                        pid=`ps -eF | grep amx-processmonitor | grep -v grep | awk '{ print $2}'`
                done
                if [ -f "/usr/bin/amxrt" ]; then
                # UPDK
                /etc/init.d/prplmesh_whm stop
                /etc/init.d/tr181-bridging stop
                fi
                echo "reload wave-driver"
                rmmod mtlk
                modprobe mtlk
        fi

        # Create radio wave interfaces and assign macaddress
        # QuickTrack expect wlan0 - 2.4G, wlan2 - 5G, wlan4 - 6G
        echo "Create radio wave interfaces"
        phys=`iw phy | grep Wiphy | awk '{ print $2 }'`
        for phy in $phys; do
                #echo $phy
                if [ `get_is_zwdfs_phy $phy` = "1" ]; then
                        #echo "ZWDFS"
                        continue
                fi
                iw_phy2_info=`iw $phy info | grep -c "* 6..... MHz"`
                iw_phy1_info=`iw $phy info | grep -c "* 5..... MHz"`
                iw_phy0_info=`iw $phy info | grep -c "* 24.... MHz"`
                if [ "$BOARD" == "osp-tb341-v2" ]; then
                        iw_phy2_info=`iw $phy info | grep -c "* 6... MHz"`
                        iw_phy1_info=`iw $phy info | grep -c "* 5... MHz"`
                        iw_phy0_info=`iw $phy info | grep -c "* 24.. MHz"`
                fi
                if [ $iw_phy2_info -gt 0 ]
                then
                        #echo 6
                        iw phy $phy interface add wlan4 type managed
                        radio_mac_addr=`update_mac_address wlan4`
                        ifconfig wlan4 hw ether $radio_mac_addr
                elif [ $iw_phy1_info -gt 0 ]
                then
                        #echo 5
                        iw phy $phy interface add wlan2 type managed
                        radio_mac_addr=`update_mac_address wlan2`
                        ifconfig wlan2 hw ether $radio_mac_addr
                elif [ $iw_phy0_info -gt 0 ]
                then
                        # echo 2
                        iw phy $phy interface add wlan0 type managed
                        radio_mac_addr=`update_mac_address wlan0`
                        ifconfig wlan0 hw ether $radio_mac_addr
                fi
        done

        echo "Copy certification config files and scripts"
        cp $SCRIPTS_PATH/QuickTrack/pin_checksum.sh /tmp/
        cp $SCRIPTS_PATH/QuickTrack/wsc_settings_APUT /tmp
        cp $SCRIPTS_PATH/QuickTrack/get_mac_address.sh /tmp

        echo "Stop firewall"
        if [ "$OS_NAME" = "URDKB" ] || [ "$OS_NAME" = "RDKB" ]; then
                sysevent set firewall-stop
                syscfg set firewall_enabled 0
                sysevent set firewall-restart
        else
                if [ -f "/usr/bin/amxrt" ]; then
                        # UPDK
                        /etc/init.d/tr181-firewall stop
                        # WA firewall need to stop twice to take affect
                        sleep 1
                        /etc/init.d/tr181-firewall stop
                else
                        /etc/init.d/firewall stop
                        /etc/init.d/firewall disable
                        /etc/init.d/servd stop
                        /etc/init.d/servd disable
                fi
        fi
else
        # UPDK
        if [ -f "/usr/bin/amxrt" ]; then
                echo "Stop firewall"
                /etc/init.d/tr181-firewall stop
                # WA firewall need to stop twice to take affect
                sleep 1
                /etc/init.d/tr181-firewall stop
        fi
fi

if [ "$ROLE" != "afc" ]; then
	echo "Run QuickTrack agent"
	wfa_quicktrack -p 9004 -d &
else
	echo "Run QuickTrack agent for AFC"
	wfa_quicktrack_afc -p 9004 -d &
fi
