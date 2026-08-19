Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Configure MAC filter rule:

  $ printf "\
  > ubus-cli Firewall.Chain.Low.Rule+{Alias='test'}
  > ubus-cli Firewall.Chain.Low.Rule.test.SourceMAC='AA:BB:CC:DD:EE:FF'
  > ubus-cli Firewall.Chain.Low.Rule.test.Target=Accept
  > ubus-cli Firewall.Chain.Low.Rule.test.IPVersion=4
  > ubus-cli Firewall.Chain.Low.Rule.test.Enable=1
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null; sleep 1

Check that there is correct rule present:

  $ R "iptables -L FORWARD_L_Low -n | grep aa:bb:cc"
  ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0            MACaa:bb:cc:dd:ee:ff

Disable MAC filter rule:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli Firewall.Chain.Low.Rule.test.Enable=0" > /dev/null; sleep 1

Check that the rule is missing:

  $ R "iptables -L FORWARD_L_Low -n | grep AA:BB:CC"
  [1]

Enable MAC filter rule:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli Firewall.Chain.Low.Rule.test.Enable=1" > /dev/null; sleep 1

Check that the rule is back again:

  $ R "iptables -L FORWARD_L_Low -n | grep aa:bb:cc"
  ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0            MACaa:bb:cc:dd:ee:ff

Remove the rule and check that it is gone:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli Firewall.Chain.Low.Rule.test-" > /dev/null; sleep 1
  $ R "iptables -L FORWARD_L_Low -n | grep AA:BB:CC"
  [1]
