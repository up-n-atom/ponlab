#!/bin/bash

ssh "root@$TARGET_LAN_IP" "ubus -t 200 wait_for DHCPv4.Client.1 NAT.InterfaceSetting.1 IP.Interface.2 DSLite.InterfaceSetting.1"

ssh "root@$TARGET_LAN_IP" "ubus -S call DHCPv4.Client.1 _set '{\"parameters\":{\"Enable\":0}}'"
ssh "root@$TARGET_LAN_IP" "ubus -S call IP.Interface.2 _set '{\"parameters\":{\"IPv4Enable\":0}}'"
ssh "root@$TARGET_LAN_IP" "ubus -S call Logical.Interface.1 _set '{\"parameters\":{\"LowerLayers\":\"Device.IP.Interface.2.,Device.IP.Interface.7.\"}}'"
ssh "root@$TARGET_LAN_IP" "ubus -S call DSLite.InterfaceSetting.1 _set '{\"parameters\":{\"EndpointAssignmentPrecedence\":\"Static\"}}'"
ssh "root@$TARGET_LAN_IP" "\
	ubus call DSLite.InterfaceSetting.1 _set \
	'{\"parameters\":{ \
		\"EndpointAddressTypePrecedence\":\"FQDN\", \
		\"EndpointName\":\"aftr.prplfoundation.org\", \
		\"Enable\":1} \
	}' \
"
ssh "root@$TARGET_LAN_IP" "ubus -S call DSLite _set '{\"parameters\":{\"Enable\":1}}'"
ssh "root@$TARGET_LAN_IP" "ubus call DSLite _get '{\"depth\":2}'"
