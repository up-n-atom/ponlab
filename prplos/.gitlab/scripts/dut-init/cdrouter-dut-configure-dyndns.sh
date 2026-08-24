#!/bin/bash

ssh "root@$TARGET_LAN_IP" "sed -i 's/CHECK_SECONDS=300/CHECK_SECONDS=30/g' /usr/lib/ddns/dynamic_dns_updater.sh"
ssh "root@$TARGET_LAN_IP" "ubus -t 200 wait_for DynamicDNS.Client"
ssh "root@$TARGET_LAN_IP" "\
	ubus call DynamicDNS.Server.9 _set \
	'{\"parameters\":{ \
		\"CheckInterval\":60, \
  }}' \
"
ssh "root@$TARGET_LAN_IP" "\
	ubus call DynamicDNS.Client _add \
	'{\"parameters\":{ \
		\"Alias\":\"cdrouter\", \
		\"Server\":\"DynamicDNS.Server.9.\", \
		\"Interface\":\"Device.IP.Interface.2.\", \
		\"Username\":\"qacafe\", \
		\"Password\":\"qacafe123\", \
		\"Enable\":1} \
	}' \
"
ssh "root@$TARGET_LAN_IP" "\
	ubus call DynamicDNS.Client.1.Hostname _add \
	'{\"parameters\":{ \
		\"Name\":\"cpe01.prplOS.prplfoundation.org\", \
		\"Enable\":1} \
	}' \
"
ssh "root@$TARGET_LAN_IP" "ubus call DynamicDNS.Client.1 _get '{\"depth\":2}'"
