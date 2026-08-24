Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Set firewall level to High:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli Firewall.Config=High" > /dev/null; sleep 1

Check that it is set properly:

  $ R "iptables -n -L FORWARD_L_Low | grep references"
  Chain FORWARD_L_Low (3 references)

  $ R "iptables -n -L FORWARD_L_High | grep references"
  Chain FORWARD_L_High (0 references)

Set firewall level to Low:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli Firewall.Config=Low" > /dev/null; sleep 1

Check that it is set properly:

  $ R "iptables -n -L FORWARD_L_Low | grep references"
  Chain FORWARD_L_Low (3 references)

  $ R "iptables -n -L FORWARD_L_High | grep references"
  Chain FORWARD_L_High (0 references)
