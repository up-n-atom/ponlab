Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that miniupnpd is enabled and running by default:

  $ R "pgrep --count miniupnpd"
  1

  $ R "ubus -S call UPnP.Device _get '{\"rel_path\":\"UPnPIGD\"}'"
  {"UPnP.Device.":{"UPnPIGD":true}}
  {}
  {"amxd-error-code":0}

Disable miniupnpd:

  $ R "ubus -S call UPnP.Device _set '{\"parameters\":{\"UPnPIGD\":False}}'" ; sleep 2
  {"UPnP.Device.":{"UPnPIGD":false}}
  {}
  {"amxd-error-code":0}

Check that miniupnpd is disabled and not running:

  $ R "pgrep --count miniupnpd"
  0
  [1]

  $ R "ubus -S call UPnP.Device _get '{\"rel_path\":\"UPnPIGD\"}'"
  {"UPnP.Device.":{"UPnPIGD":false}}
  {}
  {"amxd-error-code":0}

Enable miniupnpd:

  $ R "ubus -S call UPnP.Device _set '{\"parameters\":{\"UPnPIGD\":True}}'" ; sleep 2
  {"UPnP.Device.":{"UPnPIGD":true}}
  {}
  {"amxd-error-code":0}

Check that miniupnpd is enabled and running again:

  $ R "pgrep --count miniupnpd"
  1

  $ R "ubus -S call UPnP.Device _get '{\"rel_path\":\"UPnPIGD\"}'"
  {"UPnP.Device.":{"UPnPIGD":true}}
  {}
  {"amxd-error-code":0}
