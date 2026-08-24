Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that datamodel has expected content:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.1 _get | jsonfilter -e @[*].Connector -e @[*].PONMode -e @[*].Identifier | sort"
  3
  SC
  XGS-PON

  $ R "ubus -S call XPON.ONU.1.ANI.1.TC.ONUActivation _get"
  {"XPON.ONU.1.ANI.1.TC.ONUActivation.":{"ONUID":0,"VendorID":"ALCL","SerialNumber":"ALCL96F0E9A0","ONUState":"O5"}}
  {}
  {"amxd-error-code":0}

  $ R "ubus -S call XPON.ONU.1.ANI.1 _get"
  {"XPON.ONU.1.ANI.1.":{"TransceiverNumberOfEntries":1,"PONMode":"XGS-PON","LastChange":[0-9]+,"Enable":true,"Name":"","Alias":"cpe-ANI-1","Status":"Up"}} (re)
  {}
  {"amxd-error-code":0}

Check that there is no Transceiver.2 datamodel available:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.2 _get"
  Command failed: Not found
  [4]

Disable PON ONU:

  $ R "ubus -S call XPON.ONU.1 _set '{\"parameters\":{\"Enable\":0}}'" ; sleep 10
  {"XPON.ONU.1.":{"Enable":false}}
  {}
  {"amxd-error-code":0}

Check for expected ONU state:

  $ R "ubus -S call XPON.ONU.1.ANI.1.TC.ONUActivation _get"
  {"XPON.ONU.1.ANI.1.TC.ONUActivation.":{"ONUID":0,"VendorID":"ALCL","SerialNumber":"ALCL96F0E9A0","ONUState":"O1"}}
  {}
  {"amxd-error-code":0}

  $ R "ubus -S call XPON.ONU.1.ANI.1 _get"
  {"XPON.ONU.1.ANI.1.":{"TransceiverNumberOfEntries":1,"PONMode":"XGS-PON","LastChange":[0-9]+,"Enable":true,"Name":"","Alias":"cpe-ANI-1","Status":"Dormant"}} (re)
  {}
  {"amxd-error-code":0}

Enable PON ONU again:

  $ R "ubus -S call XPON.ONU.1 _set '{\"parameters\":{\"Enable\":1}}'" ; sleep 10
  {"XPON.ONU.1.":{"Enable":true}}
  {}
  {"amxd-error-code":0}

Check for expected ONU state:

  $ R "ubus -S call XPON.ONU.1.ANI.1.TC.ONUActivation _get"
  {"XPON.ONU.1.ANI.1.TC.ONUActivation.":{"ONUID":0,"VendorID":"ALCL","SerialNumber":"ALCL96F0E9A0","ONUState":"O5"}}
  {}
  {"amxd-error-code":0}

  $ R "ubus -S call XPON.ONU.1.ANI.1 _get"
  {"XPON.ONU.1.ANI.1.":{"TransceiverNumberOfEntries":1,"PONMode":"XGS-PON","LastChange":[0-9]+,"Enable":true,"Name":"","Alias":"cpe-ANI-1","Status":"Up"}} (re)
  {}
  {"amxd-error-code":0}
