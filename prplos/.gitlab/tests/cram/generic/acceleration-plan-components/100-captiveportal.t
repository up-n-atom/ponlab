Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Enable Captive Portal (not activated by default):

  $ R "ba-cli 'CaptivePortal.Enable=1'" >/dev/null
  $ R "ba-cli 'CaptivePortal.Status?'" | grep -v '^>' | head -n -2 
  CaptivePortal.Status="Enabled"

Disable WAN using IP datamodel and check that Captive Portal starts intercepting:

  $ R "ba-cli 'Device.IP.Interface.[Alias==\"wan\"].Enable=0'" >/dev/null

Get a DHCP lease from the router (openNDS requires clients to be registered):

  $ sudo nmap -sU -p 67 --script=dhcp-discover --script-args dhcptype=DHCPREQUEST,DHCPACK "${TARGET_LAN_IP}" 2>&1 | grep "DHCP Message Type"
  |   DHCP Message Type: DHCPACK

Wait for status change:

  $ sleep 15
  $ R "ba-cli 'ubus-protected;CaptivePortal.Status?' 2>&1" | grep -v '^>' | head -n -2 
  * access ubus: protected * (glob)
  * (glob)
  CaptivePortal.Status="Enabled"
  CaptivePortal.LANInterface.1.Status="Intercepting"

Check openNDS http interface has been opened on br-lan:

  $ R "ba-cli 'UserInterface.HTTPAccess.[Alias==\"captive\"].Interface?'" | grep -v '^>' | head -n -2 
  UserInterface.HTTPAccess.3.Interface="Device.IP.Interface.3."

Send a curl http request to detectportal.firefox.com and check we have a 307 temporary redirect:

  $ curl -i --resolve "detectportal.firefox.com:80:${TARGET_LAN_IP}" detectportal.firefox.com 2>/dev/null | head -1
  HTTP/1.1 307 Temporary Redirect\r (esc)

Reenable WAN:

  $ R "ba-cli 'Device.IP.Interface.[Alias==\"wan\"].Enable=1'" >/dev/null
  $ R "ba-cli 'CaptivePortal.Status?'" | grep -v '^>' | head -n -2 
  CaptivePortal.Status="Enabled"

Cleanup:

  $ R "ba-cli 'CaptivePortal.Enable=0'" >/dev/null
