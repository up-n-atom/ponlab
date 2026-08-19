Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Get initial state of bridges:

  $ R "bridge -json link" | jq -r 'sort_by(.master,.ifname) | reverse | .[] | "\(.master)@\(.ifname)"'
  br-lan@wlan4.1
  br-lan@wlan2.1
  br-lan@wlan0.1
  br-lan@eth0_5
  br-lan@eth0_4
  br-lan@eth0_3
  br-lan@eth0_2
  br-lan@eth0_1
  br-guest@wlan4.2
  br-guest@wlan2.2
  br-guest@wlan0.2

Remove eth0_1 from LAN bridge and add it to the Guest bridge:

  $ printf ' \
  > ubus-cli Bridging.Bridge.lan.Port.ETH0_1-\n
  > ubus-cli Bridging.Bridge.guest.Port.+{Name="eth0_1", Alias="ETH0_1", LowerLayers="Device.Ethernet.Interface.2."}\n
  > sleep 1\n
  > ubus-cli Bridging.Bridge.guest.Port.ETH0_1.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 2

Check that eth0_1 is added to Guest bridge:

  $ R "bridge -json link" | jq -r 'sort_by(.master,.ifname) | reverse | .[] | "\(.master)@\(.ifname)"'
  br-lan@wlan4.1
  br-lan@wlan2.1
  br-lan@wlan0.1
  br-lan@eth0_5
  br-lan@eth0_4
  br-lan@eth0_3
  br-lan@eth0_2
  br-guest@wlan4.2
  br-guest@wlan2.2
  br-guest@wlan0.2
  br-guest@eth0_1

Remove eth0_1 from the Guest bridge and add it back to the LAN bridge:

  $ printf '\
  > ubus-cli Bridging.Bridge.guest.Port.ETH0_1-\n
  > ubus-cli Bridging.Bridge.lan.Port.+{Name="eth0_1", Alias="ETH0_1", LowerLayers="Device.Ethernet.Interface.2."}\n
  > sleep 1\n
  > ubus-cli Bridging.Bridge.lan.Port.ETH0_1.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 2

Check for initial state of bridges again:

  $ R "bridge -json link" | jq -r 'sort_by(.master,.ifname) | reverse | .[] | "\(.master)@\(.ifname)"'
  br-lan@wlan4.1
  br-lan@wlan2.1
  br-lan@wlan0.1
  br-lan@eth0_5
  br-lan@eth0_4
  br-lan@eth0_3
  br-lan@eth0_2
  br-lan@eth0_1
  br-guest@wlan4.2
  br-guest@wlan2.2
  br-guest@wlan0.2
