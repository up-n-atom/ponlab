Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Create portmapping:

  $ printf "\
  > ubus-cli NAT.PortMapping+{Alias='testing'}
  > ubus-cli NAT.PortMapping.testing.ExternalPort=5000
  > ubus-cli NAT.PortMapping.testing.Interface=IP.Interface.2.
  > ubus-cli NAT.PortMapping.testing.InternalClient=$TARGET_LAN_TEST_HOST
  > ubus-cli NAT.PortMapping.testing.InternalPort=12345
  > ubus-cli NAT.PortMapping.testing.Protocol=TCP
  > ubus-cli NAT.PortMapping.testing.Enable=1
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null

Check that portmapping works:

  $ sudo tcpdump -c 1 -l -n -i $TESTBED_LAN_INTERFACE tcp port 12345 > /tmp/tcpdump-works 2> /dev/null &
  $ sleep 1

  $ echo foo | nc -w 1 10.0.0.2 5000 > /dev/null 2>&1
  [1]

  $ grep -c 12345 /tmp/tcpdump-works
  1

Remove portmapping:

  $ script --command "ssh -t root@$TARGET_LAN_IP ubus-cli NAT.PortMapping.testing-" > /dev/null; sleep .5

Check that portmapping doesnt work:

  $ sudo tcpdump -c 1 -l -n -i $TESTBED_LAN_INTERFACE tcp port 12345 > /tmp/tcpdump 2> /dev/null &
  $ sleep 1

  $ echo foo | nc -w 1 10.0.0.2 5000 > /dev/null 2>&1
  [1]

  $ wc -l /tmp/tcpdump
  0 /tmp/tcpdump
