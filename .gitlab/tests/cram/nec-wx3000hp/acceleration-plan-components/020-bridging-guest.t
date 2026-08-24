Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Get initial state of bridges:

  $ R "brctl show | sort | cut -d$'\t' -f1,4-"
  \t\t\t\t\teth0_2 (esc)
  \t\t\t\t\teth0_3 (esc)
  \t\t\t\t\teth0_4 (esc)
  br-guest\tno\t\t (esc)
  br-lan\tno\t\teth0_1 (esc)
  bridge name\tSTP enabled\tinterfaces (esc)

Remove eth0_1 from LAN bridge and add it to the Guest bridge:

  $ printf ' \
  > ubus-cli Bridging.Bridge.lan.Port.eth_port0-\n
  > ubus-cli Bridging.Bridge.guest.Port.+{Name="eth0_1", Alias="eth_port0", LowerLayers="Device.Ethernet.Interface.2."}\n
  > ubus-cli Bridging.Bridge.guest.Port.eth_port0.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 10

Check that eth0_1 is added to Guest bridge:

  $ R "brctl show | sort | cut -d$'\t' -f1,4-"
  \t\t\t\t\teth0_3 (esc)
  \t\t\t\t\teth0_4 (esc)
  br-guest\tno\t\teth0_1 (esc)
  br-lan\tno\t\teth0_2 (esc)
  bridge name\tSTP enabled\tinterfaces (esc)

Remove eth0_1 from the Guest bridge and add it back to the LAN bridge:

  $ printf '\
  > ubus-cli Bridging.Bridge.guest.Port.eth_port0-\n
  > ubus-cli Bridging.Bridge.lan.Port.+{Name="eth0_1", Alias="eth_port0", LowerLayers="Device.Ethernet.Interface.2."}\n
  > ubus-cli Bridging.Bridge.lan.Port.eth_port0.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 10

Check for initial state of bridges again:

  $ R "brctl show | sort | cut -d$'\t' -f1,4-"
  \t\t\t\t\teth0_2 (esc)
  \t\t\t\t\teth0_3 (esc)
  \t\t\t\t\teth0_4 (esc)
  br-guest\tno\t\t (esc)
  br-lan\tno\t\teth0_1 (esc)
  bridge name\tSTP enabled\tinterfaces (esc)
