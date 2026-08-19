Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check PCP root datamodel:

  $ R "ubus -S call PCP _get"
  {"PCP.":{"Enable":false,"OptionList":"1,3","PreferredVersion":2,"SupportedVersions":"0,1,2","Debug":false,"ClientNumberOfEntries":1}}
  {}
  {"amxd-error-code":0}

Add Client:

  $ R "ubus-cli PCP.Client+{WANInterface = \"Device.Logical.Interface.1.\"}" > /dev/null; sleep 2

Check Client parameters:

  $ R "ubus call PCP.Client.2 _get | jsonfilter -e @[*].WANInterface -e @[*].Status | sort"
  Device.Logical.Interface.1.
  StackDisabled

Add Server:

  $ R "ubus-cli PCP.Client.2.Server+{Origin = \"DHCPv6\"}" > /dev/null; sleep 2

Check Server parameters:

  $ R "ubus call PCP.Client.2.Server.1 _get | jsonfilter -e @[*].Status -e @[*].Origin | sort"
  DHCPv6
  Disabled
