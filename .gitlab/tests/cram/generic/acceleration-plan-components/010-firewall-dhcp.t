Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that client is able to get new lease:

  $ sudo nmap --script broadcast-dhcp-discover -e $TESTBED_LAN_INTERFACE 2>&1 | egrep '(Server|Router|Subnet)' | sort
  |     Router: 192.168.1.1
  |     Server Identifier: 192.168.1.1
  |     Subnet Mask: 255.255.255.0
  |_    Domain Name Server: 192.168.1.1

Remove cpe-dhcpv4s-lan rule:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli Firewall.Service.cpe-dhcpv4s-lan-" > /dev/null; sleep .5

Check that the firewall rule was actually removed:

  $ R "iptables -vnL INPUT_Services | grep :67 | awk '/ACCEPT.*udp/ {print \$3, \$4, \$6, \$11}' | sort"
  ACCEPT udp br-guest dpt:67
  ACCEPT udp br-lcm dpt:67

Check that client is unable to get new lease:

  $ sudo nmap --script broadcast-dhcp-discover -e $TESTBED_LAN_INTERFACE 2>&1 | egrep '(Server|Router|Subnet)'
  [1]

Add back firewall rule for cpe-dhcpv4s-lan access from LAN:

  $ printf "\
  > ubus-cli Firewall.Service+{Alias='cpe-dhcpv4s-lan'}
  > ubus-cli Firewall.Service.cpe-dhcpv4s-lan.Action=Accept
  > ubus-cli Firewall.Service.cpe-dhcpv4s-lan.DestPort=67
  > ubus-cli Firewall.Service.cpe-dhcpv4s-lan.IPVersion=4
  > ubus-cli Firewall.Service.cpe-dhcpv4s-lan.Interface=Device.IP.Interface.3.
  > ubus-cli Firewall.Service.cpe-dhcpv4s-lan.Protocol=17
  > ubus-cli Firewall.Service.cpe-dhcpv4s-lan.Enable=1
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null

Check that the firewall rule was actually created:

  $ R "iptables -vnL INPUT_Services | grep :67 | cut -d ' ' -f 11,16,20,53 | sort"
  ACCEPT udp br-guest dpt:67
  ACCEPT udp br-lan dpt:67
  ACCEPT udp br-lcm dpt:67

Check that client is able to get new lease again:

  $ sudo nmap --script broadcast-dhcp-discover -e $TESTBED_LAN_INTERFACE 2>&1 | egrep '(Server|Router|Subnet)' | sort
  |     Router: 192.168.1.1
  |     Server Identifier: 192.168.1.1
  |     Subnet Mask: 255.255.255.0
  |_    Domain Name Server: 192.168.1.1
