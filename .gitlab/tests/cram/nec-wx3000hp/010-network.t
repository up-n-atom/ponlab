Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check correct routing table:

  $ R ip route
  default via 10.0.0.1 dev eth1 
  default via 10.0.0.1 dev eth1 proto static src 10.0.0.2 
  10.0.0.0/24 dev eth1 proto kernel scope link src 10.0.0.2 
  192.168.1.0/24 dev br-lan proto kernel scope link src 192.168.1.1 
  192.168.2.0/24 dev br-guest proto kernel scope link src 192.168.2.1 linkdown 

Check correct interface setup:

  $ R "ip link" | awk '/^[0-9]+:/ { printf $0; next } { print ";"$2 }' | awk '/lo:/{print} !/00:00:00:00:00:00/{print}' | cut -d\; -f1 | cut -d: -f2- | LC_ALL=C sort | grep -v veth_gene_0
   br-guest: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
   br-lan: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default qlen 1000
   eth0_0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   eth0_1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth0_2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth0_3: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast master br-lan state DOWN mode DEFAULT group default qlen 1000
   eth0_4: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master br-lan state UP mode DEFAULT group default qlen 1000
   eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc htb state UNKNOWN mode DEFAULT group default qlen 1000
   ifb0: <BROADCAST,NOARP> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 32
   ifb1: <BROADCAST,NOARP> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 32
   ip6tnl0@NONE: <NOARP> mtu 1452 qdisc noop state DOWN mode DEFAULT group default qlen 1
   lite0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UNKNOWN mode DEFAULT group default qlen 1000
   lo: <LOOPBACK,UP,LOWER_UP> mtu 65535 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1
   loopdev0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
   teql0: <NOARP> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 100
