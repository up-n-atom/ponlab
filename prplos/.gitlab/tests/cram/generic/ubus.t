Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that we've correct bridge aliases:

  $ R "ubus call Bridging _get \"{'rel_path':'Bridge.*.Alias'}\" | jsonfilter -e @[*].Alias | sort"
  guest
  lan
  lcm

Check that we've correct DHCP pool settings:

  $ R "ubus call DHCPv4Server.Pool _get \"{'rel_path':''}\" | grep -E '(Alias|MinAddres|MaxAddress|Enable|Servers|Status)' | sort"
  \t\t"Alias": "guest", (esc)
  \t\t"Alias": "lan", (esc)
  \t\t"Alias": "lan_extender", (esc)
  \t\t"Alias": "lcm", (esc)
  \t\t"DNSServers": "192.168.1.1", (esc)
  \t\t"DNSServers": "192.168.1.1", (esc)
  \t\t"DNSServers": "192.168.2.1", (esc)
  \t\t"DNSServers": "192.168.5.1", (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"MaxAddress": "192.168.1.150", (esc)
  \t\t"MaxAddress": "192.168.1.254", (esc)
  \t\t"MaxAddress": "192.168.2.249", (esc)
  \t\t"MaxAddress": "192.168.5.249", (esc)
  \t\t"MinAddress": "192.168.1.100", (esc)
  \t\t"MinAddress": "192.168.1.2", (esc)
  \t\t"MinAddress": "192.168.2.100", (esc)
  \t\t"MinAddress": "192.168.5.100", (esc)
  \t\t"Status": "Enabled", (esc)
  \t\t"Status": "Enabled", (esc)
  \t\t"Status": "Enabled", (esc)
  \t\t"Status": "Enabled", (esc)

  $ R "ubus call DHCPv6Server.Pool _get \"{'rel_path':''}\" | grep -E '(Alias|Enable|Status)' | sort"
  \t\t"Alias": "guest", (esc)
  \t\t"Alias": "lan", (esc)
  \t\t"Alias": "lcm", (esc)
  \t\t"Enable": false, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"IANAEnable": false, (esc)
  \t\t"IANAEnable": true, (esc)
  \t\t"IANAEnable": true, (esc)
  \t\t"IAPDEnable": false, (esc)
  \t\t"IAPDEnable": false, (esc)
  \t\t"IAPDEnable": false, (esc)
  \t\t"Status": "Disabled", (esc)
  \t\t"Status": "Error_Misconfigured", (esc)
  \t\t"Status": "Error_Misconfigured", (esc)

Check that aclmanager has expected setup:

  $ R "ubus call ACLManager.Role _get '{\"rel_path\":\"\"}' | jsonfilter -e @[*].Name -e @[*].Alias | sort"
  admin
  cpe-Role-1
  cpe-Role-2
  cpe-Role-3
  cpe-Role-4
  cwmp
  operator
  untrusted

Check that Users.Role component has expected setup:

  $ R "ubus call Users.Role _get '{\"rel_path\":\"\"}' | jsonfilter -e @[*].Alias -e @[*].RoleName | sort"
  admin
  admin-role
  guest
  guest-role

Check that we've correct hostname and release info:

  $ R "ubus -S call system board | jsonfilter -e '@.hostname' -e '@.release.distribution'"
  prplOS.lan
  prplOS

Check that netifd service is running:

  $ R "ubus -S call service list | jsonfilter -e '@.network.instances.instance1.running'"
  true
