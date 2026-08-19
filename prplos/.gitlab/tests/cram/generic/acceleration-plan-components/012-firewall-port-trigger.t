Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Configure port trigger rule:

  $ printf "\
  > ubus-cli NAT.PortTrigger+{Alias='test'}
  > ubus-cli NAT.PortTrigger.test.Port=6000
  > ubus-cli NAT.PortTrigger.test.Protocol="TCP"
  > ubus-cli NAT.PortTrigger.test.AutoDisableDuration=7
  > ubus-cli NAT.PortTrigger.test.Rule+{Alias='test-rule'}
  > ubus-cli NAT.PortTrigger.test.Rule.test-rule.Port=8000
  > ubus-cli NAT.PortTrigger.test.Rule.test-rule.Protocol="UDP"
  > ubus-cli NAT.PortTrigger.test.Enable=1
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null; sleep 1

Check that there is NFQUEUE rule:

  $ R "iptables -L FORWARD_PortTrigger -n | grep 6000"
  NFQUEUE    tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:6000 NFQUEUE num 1

Disable port trigger rule:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli NAT.PortTrigger.test.Enable=0" > /dev/null; sleep 1

Check that there is no NFQUEUE rule:

  $ R "iptables -L FORWARD_PortTrigger -n | grep 6000"
  [1]

Enable port trigger rule:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli NAT.PortTrigger.test.Enable=1" > /dev/null; sleep 1

Add route to 10.10.10.10 via TARGET_LAN_IP:

  $ sudo ip route add 10.10.10.10/32 via $TARGET_LAN_IP dev $TESTBED_LAN_INTERFACE 2> /dev/null

Trigger port rule:

  $ curl --silent --output /dev/null --max-time 2 http://10.10.10.10:6000 ; sleep 3

Check that additional rules has been created:

  $ R "iptables -L FORWARD_PortTrigger -n | grep 8000 | sort"
  ACCEPT     udp  --  0.0.0.0/0            192.168.1.2          udp dpt:8000
  ACCEPT     udp  --  192.168.1.2          0.0.0.0/0            udp spt:8000

Check that the owner IPAddress was correctly set:

  $ R "ubus call NAT.PortTrigger _get '{\"rel_path\":\"test.Stats.IPAddress\"}' | jsonfilter -e @[*].IPAddress"
  192.168.1.2

Wait for expiration of port trigger and check that everything is disabled:

  $ sleep 6
  $ R "iptables -L FORWARD_PortTrigger -n | grep 8000"
  [1]

Remove port trigger rule and route:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli NAT.PortTrigger.test-" > /dev/null
  $ sudo ip route del 10.10.10.10/32 via $TARGET_LAN_IP dev $TESTBED_LAN_INTERFACE 2> /dev/null
