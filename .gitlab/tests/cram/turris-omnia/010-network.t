Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check correct routing table:

  $ R ip route
  default via 10.0.0.1 dev eth2 
  10.0.0.0/24 dev eth2 proto kernel scope link src 10.0.0.2 
  192.168.1.0/24 dev br-lan proto kernel scope link src 192.168.1.1 
  192.168.2.0/24 dev br-guest proto kernel scope link src 192.168.2.1 linkdown 
  192.168.5.0/24 dev br-lcm proto kernel scope link src 192.168.5.1 linkdown 

Check correct interface setup:

  $ R "ip link | grep ^\\\\d | cut -d: -f2-" | LC_ALL=C sort
   br-guest: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
   br-lan: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default qlen 1000
   br-lcm: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
   eth0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1024
   eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1508 qdisc mq state UP mode DEFAULT group default qlen 1024
   eth2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc htb state UP mode DEFAULT group default qlen 1024
   ip6tnl0@NONE: <NOARP> mtu 1452 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   lan0@eth1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue master br-lan state LOWERLAYERDOWN mode DEFAULT group default qlen 1000
   lan1@eth1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue master br-lan state LOWERLAYERDOWN mode DEFAULT group default qlen 1000
   lan2@eth1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue master br-lan state LOWERLAYERDOWN mode DEFAULT group default qlen 1000
   lan3@eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master br-lan state UP mode DEFAULT group default qlen 1000
   lan4@eth1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue master br-lan state LOWERLAYERDOWN mode DEFAULT group default qlen 1000
   lo: <LOOPBACK,UP,LOWER_UP> mtu 65535 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
   teql0: <NOARP> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 100
   wlan0.1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-lan state DOWN mode DEFAULT group default qlen 1000
   wlan0.2: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-guest state DOWN mode DEFAULT group default qlen 1000
   wlan0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DORMANT group default qlen 1000
   wlan1.1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop master br-guest state DOWN mode DEFAULT group default qlen 1000
   wlan1: <BROADCAST,MULTICAST> mtu 1500 qdisc noqueue master br-lan state DOWN mode DEFAULT group default qlen 1000
