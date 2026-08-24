Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check correct routing table:

  $ R ip route
  default via 10.0.0.1 dev eth1 
  10.0.0.0/24 dev eth1 proto kernel scope link src 10.0.0.2 
  192.168.1.0/24 dev br-lan proto kernel scope link src 192.168.1.1 
  192.168.2.0/24 dev br-guest proto kernel scope link src 192.168.2.1 linkdown 
  192.168.5.0/24 dev br-lcm proto kernel scope link src 192.168.5.1 linkdown 

Check correct interface setup:

  $ R "ip link" | awk '/^[0-9]+:/ { printf $0; next } { print ";"$2 }' | awk '/lo:/{print} !/00:00:00:00:00:00/{print}' | cut -d\; -f1 | cut -d: -f2- | LC_ALL=C sort
   br-guest: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
   br-lan: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default qlen 1000
   br-lcm: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
   eth0_1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth0_2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq master br-lan state UP mode DEFAULT group default qlen 1000
   eth0_3: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth0_4: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth0_5: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc htb state UP mode DEFAULT group default qlen 1000
   gre0@NONE: <NOARP> mtu 1476 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   ip6gre0@NONE: <NOARP> mtu 1448 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   ip6tnl0@NONE: <NOARP> mtu 1452 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   lo: <LOOPBACK,UP,LOWER_UP> mtu 65535 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
   lpdev0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UNKNOWN mode DEFAULT group default qlen 1000
   mxl_vpn: <> mtu 10034 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   sit0@NONE: <NOARP> mtu 1480 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   teql0: <NOARP> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 100
   tunl0@NONE: <NOARP> mtu 1480 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   wlan0.1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-lan state DOWN mode DEFAULT group default qlen 1000
   wlan0.2: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-guest state DOWN mode DEFAULT group default qlen 1000
   wlan0: <BROADCAST,MULTICAST> mtu 1500 qdisc mq state DOWN mode DEFAULT group default qlen 1000
   wlan1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN mode DORMANT group default qlen 1000
   wlan2.1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-lan state DOWN mode DEFAULT group default qlen 1000
   wlan2.2: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-guest state DOWN mode DEFAULT group default qlen 1000
   wlan2: <BROADCAST,MULTICAST> mtu 1500 qdisc mq state DOWN mode DEFAULT group default qlen 1000
   wlan3: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN mode DORMANT group default qlen 1000
   wlan4.1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-lan state DOWN mode DEFAULT group default qlen 1000
   wlan4.2: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-guest state DOWN mode DEFAULT group default qlen 1000
   wlan4: <BROADCAST,MULTICAST> mtu 1500 qdisc mq state DOWN mode DEFAULT group default qlen 1000
