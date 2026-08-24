Check that we've correct DHCPv4 pools:

  $ script --command "ssh -t root@$TARGET_LAN_IP 'ubus-cli DHCPv4Server.Pool.*.Alias?'" | grep 'Pool\.[[:digit:]]\.Alias' | sort
  DHCPv4Server.Pool.1.Alias="lan"\r (esc)
  DHCPv4Server.Pool.2.Alias="guest"\r (esc)
  DHCPv4Server.Pool.3.Alias="lcm"\r (esc)
  DHCPv4Server.Pool.4.Alias="lan_extender"\r (esc)

Check that we've correct DHCPv6 pools:

  $ script --command "ssh -t root@$TARGET_LAN_IP 'ubus-cli DHCPv6Server.Pool.*.Alias?'" | grep Alias= | sort
  DHCPv6Server.Pool.1.Alias="lan"\r (esc)
  DHCPv6Server.Pool.2.Alias="guest"\r (esc)
  DHCPv6Server.Pool.3.Alias="lcm"\r (esc)
