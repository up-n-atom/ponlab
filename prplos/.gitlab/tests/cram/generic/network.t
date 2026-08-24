Check DUT is reachable over LAN:

  $ ping -c3 -W1 192.168.1.2 | grep '3 packets' | cut -d, -f1-3
  3 packets transmitted, 3 received, 0% packet loss

Check DUT is reachable over WAN:

  $ ping -c3 -W1 10.0.0.2 | grep '3 packets' | cut -d, -f1-3
  3 packets transmitted, 3 received, 0% packet loss
