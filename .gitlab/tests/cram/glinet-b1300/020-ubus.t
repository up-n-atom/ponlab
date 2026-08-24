Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that ubus has all expected services available:

  $ R "ubus list | grep -v '^[[:upper:]]'"
  dhcp
  dnsmasq
  hostapd
  network
  network.device
  network.interface
  network.interface.guest
  network.interface.lan
  network.interface.loopback
  network.interface.wan
  network.interface.wan6
  network.wireless
  service
  session
  system
  uci
  umdns
  wpa_supplicant

Check that we've correct system info:

  $ R "ubus call system board | jsonfilter -e @.system -e @.model -e @.board_name"
  ARMv7 Processor rev 5 (v7l)
  GL.iNet GL-B1300
  glinet,gl-b1300

  $ R "ubus call DeviceInfo _get | jsonfilter -e '@[\"DeviceInfo.\"].ProductClass'"
  gl-b1300

Check that we've correct bridge port aliases:

  $ R "ubus call Bridging _get \"{'rel_path':'Bridge.*.Port.*.Alias'}\" | jsonfilter -e @[*].Alias | sort"
  eth_port0
  guest_bridge
  guest_wl0
  guest_wl1
  lan_bridge
  lcm_bridge
  wlan_port0
  wlan_port1

Check that we've correct ethernet interface details:

  $ R "ubus call Ethernet _get \"{'rel_path':'Interface.'}\" | grep -E '(Alias|Enable|Name)' | sort"
  \t\t"Alias": "ETH0", (esc)
  \t\t"Alias": "ETH1", (esc)
  \t\t"EEEEnable": false, (esc)
  \t\t"EEEEnable": false, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Name": "eth0", (esc)
  \t\t"Name": "eth1", (esc)

Check that we've correct ethernet link details:

  $ R "ubus call Ethernet _get \"{'rel_path':'Link.'}\" | grep -E '(Alias|Enable|Name)' | sort"
  \t\t"Alias": "bridge_guest", (esc)
  \t\t"Alias": "bridge_lan", (esc)
  \t\t"Alias": "bridge_lcm", (esc)
  \t\t"Alias": "eth_wan", (esc)
  \t\t"Alias": "link_lo", (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Enable": true, (esc)
  \t\t"Name": "br-guest", (esc)
  \t\t"Name": "br-lan", (esc)
  \t\t"Name": "br-lcm", (esc)
  \t\t"Name": "eth1", (esc)
  \t\t"Name": "lo", (esc)

Check that IP.Interface provides expected output:

  $ R "ubus call IP _get '{\"rel_path\":\"Interface.\",\"depth\":100}' | jsonfilter -e @[*].Alias -e @[*].Name -e @[*].IPAddress -e @[*].SubnetMask | sort" | egrep -v '(^f[0-9a-z:]+|^$)'
  10.0.0.2
  127.0.0.1
  192.0.0.2
  192.168.1.1
  192.168.2.1
  192.168.5.1
  255.0.0.0
  255.255.255.0
  255.255.255.0
  255.255.255.0
  255.255.255.0
  255.255.255.0
  ::1
  DHCP
  DHCP
  DSLite-entry
  DSLite-exit
  GUA
  GUA
  GUA
  GUA
  GUA_IAPD
  GUA_IAPD
  GUA_IAPD
  GUA_IAPD
  GUA_RA
  GUA_RA
  GUA_RA
  GUA_RA
  LLA
  LLA
  ULA
  ULA1
  ULA64
  br-guest
  br-lan
  eth1
  guest
  guest
  iptv
  lan
  lan
  lcm
  lcm
  lo
  loopback
  loopback_ipv4
  loopbackipv6
  mgmt
  primary
  primary
  primary
  primary
  primary
  public-lan
  voip
  wan
  wan6

Check that NAT.Interface provides expected output:

  $ R "ubus call NAT _get '{\"rel_path\":\"InterfaceSetting.\",\"depth\":100}' | jsonfilter -e @[*].Alias -e @[*].Interface"
  lcm
  wan
  lan
  guest
  Device.IP.Interface.5.
  Device.Logical.Interface.1.
  Device.IP.Interface.3.
  Device.IP.Interface.4.

Check that NetDev.Link provides expected output:

  $ R "ubus call NetDev _get '{\"rel_path\":\"Link.\",\"depth\":100}' | jsonfilter -e @[*].Name | sort"
  br-guest
  br-lan
  eth0
  eth1
  ifb0
  ifb1
  ip6tnl0
  lo
  teql0
  veth_gene_0
  wlan0
  wlan1

Check that NetModel.Intf provides expected output:

  $ R "ubus call NetModel _get '{\"rel_path\":\"Intf.\",\"depth\":100}' | jsonfilter -e @[*].Alias -e @[*].Flags -e @[*].Name -e @[*].Status | sed '/^$/d' | grep -v -e ^bridgeport -e 'bridge '| sort"
  Disabled
  Disabled
  Disabled
  Disabled
  Disabled
  Disabled
  Disabled
  Disabled
  Enabled
  Enabled
  Enabled
  Enabled
  Enabled
  Enabled
  Enabled
  Enabled
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  Error
  br-guest
  br-guest
  br-guest
  br-lan
  br-lan
  br-lan
  br-lcm
  br-lcm
  bridge-ETH1
  bridge-ETH1
  bridge-eth_port0
  bridge-eth_port0
  bridge-eth_port0
  bridge-guest_bridge
  bridge-guest_bridge
  bridge-guest_bridge
  bridge-guest_bridge
  bridge-guest_wl0
  bridge-guest_wl0
  bridge-guest_wl1
  bridge-guest_wl1
  bridge-lan_bridge
  bridge-lan_bridge
  bridge-lan_bridge
  bridge-lan_bridge
  bridge-lan_bridge
  bridge-lcm_bridge
  bridge-lcm_bridge
  bridge-wlan_port0
  bridge-wlan_port0
  bridge-wlan_port1
  bridge-wlan_port1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-1
  cpe-IPv4Address-2
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Address.* (re)
  cpe-IPv6Prefix-1
  cpe-IPv6Prefix-1
  cpe-IPv6Prefix-1
  cpe-IPv6Prefix-1
  cpe-IPv6Prefix-2
  cpe-IPv6Prefix-2
  cpe-IPv6Prefix-2
  cpe-IPv6Prefix-2
  cpe-IPv6Prefix-3
  cpe-ReqOption-1
  cpe-ReqOption-1
  cpe-ReqOption-1
  cpe-ReqOption-1
  cpe-ReqOption-10
  cpe-ReqOption-10
  cpe-ReqOption-10
  cpe-ReqOption-10
  cpe-ReqOption-2
  cpe-ReqOption-2
  cpe-ReqOption-2
  cpe-ReqOption-2
  cpe-ReqOption-3
  cpe-ReqOption-3
  cpe-ReqOption-3
  cpe-ReqOption-3
  cpe-ReqOption-4
  cpe-ReqOption-4
  cpe-ReqOption-4
  cpe-ReqOption-4
  cpe-ReqOption-5
  cpe-ReqOption-5
  cpe-ReqOption-5
  cpe-ReqOption-5
  cpe-ReqOption-6
  cpe-ReqOption-6
  cpe-ReqOption-6
  cpe-ReqOption-6
  cpe-ReqOption-7
  cpe-ReqOption-7
  cpe-ReqOption-7
  cpe-ReqOption-7
  cpe-ReqOption-8
  cpe-ReqOption-8
  cpe-ReqOption-8
  cpe-ReqOption-8
  cpe-ReqOption-9
  cpe-ReqOption-9
  cpe-ReqOption-9
  cpe-ReqOption-9
  default_radio0
  default_radio1
  dslite netdev
  dslite-dslite0
  dslite0
  eth0
  eth0
  eth1
  eth1
  eth1
  eth1
  ethIntf-ETH0
  ethIntf-ETH0
  ethIntf-ETH1
  ethIntf-ETH1
  ethIntf-ETH1
  ethLink-bridge_guest
  ethLink-bridge_guest
  ethLink-bridge_guest
  ethLink-bridge_lan
  ethLink-bridge_lan
  ethLink-bridge_lan
  ethLink-bridge_lcm
  ethLink-bridge_lcm
  ethLink-bridge_lcm
  ethLink-eth_wan
  ethLink-eth_wan
  ethLink-eth_wan
  ethLink-eth_wan
  ethLink-eth_wan
  ethLink-eth_wan
  ethLink-eth_wan
  ethLink-link_lo
  ethLink-link_lo
  eth_intf netdev.* (re)
  eth_intf netdev.* (re)
  eth_link enabled
  eth_link enabled
  eth_link enabled up
  eth_link enabled up
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  false
  guest
  guest
  guest_radio0
  guest_radio1
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip .* (re)
  ip-DSLite-entry
  ip-DSLite-entry
  ip-DSLite-exit
  ip-DSLite-exit
  ip-guest
  ip-guest
  ip-guest
  ip-iptv
  ip-iptv
  ip-iptv
  ip-lan
  ip-lan
  ip-lan
  ip-lcm
  ip-lcm
  ip-loopback
  ip-loopback
  ip-mgmt
  ip-mgmt
  ip-mgmt
  ip-voip
  ip-voip
  ip-voip
  ip-wan
  ip-wan
  ip-wan
  ip-wan6
  iptv
  iptv
  lan
  lan
  lo
  lo
  logical up enabled
  logical up enabled
  logical up enabled
  logical up enabled
  logical up enabled
  logical up enabled
  mgmt
  mgmt
  netdev eth_link netdev-bound enabled ipv4 ipv6.* (re)
  permanent
  permanent
  permanent
  permanent
  permanent
  permanent
  permanent
  permanent
  ppp netdev
  ppp-wan
  ppp-wan
  pppoe-wan
  radio enabled
  radio enabled
  radio-wifi0
  radio-wifi0
  radio-wifi0
  radio-wifi1
  radio-wifi1
  radio-wifi1
  radio-wifi1
  resolver
  resolver
  ssid netdev netdev-bound
  ssid netdev netdev-bound
  ssid netdev netdev-bound
  ssid netdev netdev-bound
  ssid netdev netdev-bound
  ssid-ep5g0
  ssid-ep5g0
  ssid-vap2g0guest
  ssid-vap2g0guest
  ssid-vap2g0priv
  ssid-vap2g0priv
  ssid-vap5g0guest
  ssid-vap5g0guest
  ssid-vap5g0priv
  ssid-vap5g0priv
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  true
  up
  voip
  voip
  wan
  wan
  wlan0
  wlan0
  wlan0.1
  wlan1
  wlan1
  wlan1
  wlan1.1
