#!/bin/bash

vap_index=1

echo "$LABGRID_TARGET" | grep -q "urx851" && vap_index=2
echo "$LABGRID_TARGET" | grep -q "prpl-haze" && vap_index=3

# shellcheck disable=SC2029
ssh "root@$TARGET_LAN_IP" "ubus -t 200 wait_for WiFi.AccessPoint.${vap_index}"

# shellcheck disable=SC2029
ssh "root@$TARGET_LAN_IP" "ubus -S call WiFi.AccessPoint.${vap_index} _set '{\"parameters\":{\"Enable\":1}}'"
