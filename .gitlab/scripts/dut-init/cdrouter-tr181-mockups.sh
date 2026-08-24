#!/bin/bash

scp .gitlab/mockups/tr181-mockups.tar.gz root@${TARGET_LAN_IP}:/tmp/
ssh root@$TARGET_LAN_IP "tar xzf /tmp/tr181-mockups.tar.gz -C / && /etc/init.d/tr181-mockups start"
ssh root@$TARGET_LAN_IP "ubus call ProxyManager register '{\"proxy\":\"Device.LANConfigSecurity.\", \"real\":\"LANConfigSecurity.\" } '"
