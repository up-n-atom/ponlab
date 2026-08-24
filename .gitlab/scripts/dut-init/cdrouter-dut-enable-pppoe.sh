#!/bin/bash

ssh root@$TARGET_LAN_IP "ubus -t 200 wait_for WANManager"
ssh root@$TARGET_LAN_IP "ubus-cli WANManager.WAN.Ethernet_PPP.Intf.1.Type=\"untagged\""
ssh root@$TARGET_LAN_IP "ubus call WANManager setWANMode '{ \"WANMode\": \"Ethernet_PPP\" }'"
