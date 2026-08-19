Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that we've expected datamodel:

  $ R "ubus list | grep UserInterface. | sort"
  UserInterface.HTTPAccess
  UserInterface.HTTPAccess.1
  UserInterface.HTTPAccess.1.Session
  UserInterface.HTTPAccess.1.Session.1
  UserInterface.HTTPAccess.1.X_PRPL-COM_HTTPConfig
  UserInterface.HTTPAccess.2
  UserInterface.HTTPAccess.2.Session
  UserInterface.HTTPAccess.2.X_PRPL-COM_HTTPConfig

  $ R "ubus call UserInterface.HTTPAccess _get | jsonfilter -e @[*].Port -e @[*].Status -e @[*].AccessType -e @[*].Alias | sort"
  80
  8090
  Down
  LocalAccess
  LocalGUI
  RemoteAccess
  RemoteGUI
  Up

Check that prpl-webui is available from LAN by default:

  $ curl --silent --max-time 3 "http://${TARGET_LAN_IP}" | grep -c prpl-webui/config/environment
  1

Disable prpl-webui access from LAN:

  $ R "ubus -S call UserInterface.HTTPAccess.1 _set '{\"parameters\":{\"Enable\":False}}'" ; sleep 2
  {"UserInterface.HTTPAccess.1.":{"Enable":false}}
  {}
  {"amxd-error-code":0}
Check that prpl-webui is not available from LAN:

  $ curl --silent --max-time 1 "http://${TARGET_LAN_IP}" | grep -c prpl-webui/config/environment
  0
  [1]

Enable prpl-webui access from LAN:

  $ R "ubus -S call UserInterface.HTTPAccess.1 _set '{\"parameters\":{\"Enable\":True}}'"
  {"UserInterface.HTTPAccess.1.":{"Enable":true}}
  {}
  {"amxd-error-code":0}

Check that prpl-webui is available from LAN again:

  $ curl --silent --retry-connrefused --retry 3 --max-time 3 "http://${TARGET_LAN_IP}" | grep -c prpl-webui/config/environment
  1
