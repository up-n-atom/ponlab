Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Get initial state of bridges:

  $ R "bridge -json link" | jq -r 'sort_by(.master,.ifname) | reverse | .[] | "\(.master)@\(.ifname)"'
  br-lan@wlan2
  br-lan@wlan1
  br-lan@wlan0.1
  br-lan@lan3
  br-lan@lan2
  br-lan@lan1
  br-guest@wlan2.1
  br-guest@wlan1.1
  br-guest@wlan0.2

Remove lan1 from LAN bridge and add it to the Guest bridge:

  $ printf ' \
  > ubus-cli Bridging.Bridge.lan.Port.LAN1-\n
  > ubus-cli Bridging.Bridge.guest.Port.+{Name="LAN1", Alias="LAN1", LowerLayers="Device.Ethernet.Interface.2."}\n
  > sleep 1\n
  > ubus-cli Bridging.Bridge.guest.Port.LAN1.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 2

Check that lan1 is added to Guest bridge:

  $ R "bridge -json link" | jq -r 'sort_by(.master,.ifname) | reverse | .[] | "\(.master)@\(.ifname)"'
  br-lan@wlan2
  br-lan@wlan1
  br-lan@wlan0.1
  br-lan@lan3
  br-lan@lan2
  br-guest@wlan2.1
  br-guest@wlan1.1
  br-guest@wlan0.2
  br-guest@lan1

Remove lan1 from the Guest bridge and add it back to the LAN bridge:

  $ printf '\
  > ubus-cli Bridging.Bridge.guest.Port.LAN1-\n
  > ubus-cli Bridging.Bridge.lan.Port.+{Name="LAN1", Alias="LAN1", LowerLayers="Device.Ethernet.Interface.2."}\n
  > sleep 1\n
  > ubus-cli Bridging.Bridge.lan.Port.LAN1.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 2

Check for initial state of bridges again:

  $ R "bridge -json link" | jq -r 'sort_by(.master,.ifname) | reverse | .[] | "\(.master)@\(.ifname)"'
  br-lan@wlan2
  br-lan@wlan1
  br-lan@wlan0.1
  br-lan@lan3
  br-lan@lan2
  br-lan@lan1
  br-guest@wlan2.1
  br-guest@wlan1.1
  br-guest@wlan0.2
