Check that we've correct system info:

  $ script --command "ssh -t root@$TARGET_LAN_IP 'ubus-cli DeviceInfo.?'" | grep ProductClass
  DeviceInfo.ProductClass="gl-b1300"\r (esc)
