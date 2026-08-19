Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Get initial state of bridges:

  $ R "brctl show | grep -E '(br-lan|br-guest)' | sort | cut -d$'\t' -f1,6" | tr '\t' ' '
  br-guest 
  br-lan lan(1|2) (re)

Add lan2 to the Guest bridge:

  $ printf ' \
  > ubus-cli Bridging.Bridge.guest.Port.+{Name="lan2", Alias="LAN2"}\n
  > ubus-cli Bridging.Bridge.guest.Port.LAN2.Enable=1\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 5

Check that lan2 is added to br-guest bridge:

  $ R "brctl show | grep -E '(br-lan|br-guest)' | sort | cut -d$'\t' -f1,6" | tr '\t' ' '
  br-guest lan2
  br-lan lan1

Remove lan2 from the Guest bridge:

  $ printf '\
  > ubus-cli Bridging.Bridge.guest.Port.LAN2-\n
  > ' > /tmp/run
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run)'" > /dev/null
  $ sleep 5

Check for initial state of bridges again:

  $ R "brctl show | grep -E '(br-lan|br-guest)' | sort | cut -d$'\t' -f1,6" | tr '\t' ' '
  br-guest 
  br-lan lan(1|2) (re)
