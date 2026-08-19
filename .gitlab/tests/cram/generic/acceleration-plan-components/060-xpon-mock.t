Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Skip the tests if TFTP_IMAGE_DOWNLOAD_URL is used as the mock package would be missing:

  $ [ -n "$TFTP_IMAGE_DOWNLOAD_URL" ] && exit 80
  [1]

Skip test on urx851-b0-dk-pon as it has real PON and thus mocking is not needed:

  $ [ "$DUT_BOARD" = "urx851-b0-dk-pon" ] && exit 80
  [1]

Install mod-xpon-prpl-mock package:

  $ scp ${CI_PROJECT_DIR}/${DUT_ARCH_PACKAGES_PATH}/feed_prpl/mod-xpon-prpl-mock*.ipk "root@${TARGET_LAN_IP}:/tmp/"
  $ R "opkg install --force-reinstall /tmp/mod-xpon-prpl-mock*.ipk"
  No packages removed.
  Installing mod-xpon-prpl-mock .* to root... (re)
  Configuring mod-xpon-prpl-mock.

Stop tr181-xpon service:

  $ R "/etc/init.d/tr181-xpon stop" > /dev/null 2>&1

Check that there is no Transceiver.1 datamodel available:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.1 _get"
  Command failed: Not found
  [4]

Start tr181-xpon service and mock:

  $ R "/etc/init.d/tr181-xpon start" > /dev/null 2>&1
  $ R "ubus -t10 wait_for XPON.ONU"

  $ R "( /opt/prplos/usr/bin/xpon-onu-hal-mock | logger -t xpon-onu-hal-mock ) &"
  $ R "ubus -t30 wait_for XPON.ONU.1.ANI.1.Transceiver.1"

Check that Transceiver.1 datamodel is properly available:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.1 _get | jsonfilter -e @[*].VendorName -e @[*].PONMode -e @[*].VendorRevision | sort"
  MyVendorName
  Version_1
  XGS-PON

Check that there is no Transceiver.2 datamodel available:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.2 _get"
  Command failed: Not found
  [4]

Add Transceiver.2 instance:

  $ R "/opt/prplos/usr/bin/xpon-onu-hal-mock-dbgtool add_instance"
  $ R "ubus -t30 wait_for XPON.ONU.1.ANI.1.Transceiver.2"

Check that Transceiver.1 datamodel is properly available:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.2 _get | jsonfilter -e @[*].VendorName -e @[*].PONMode -e @[*].VendorRevision | sort"
  
  NG-PON2
  SomeOtherVendor

Remove Transceiver.2 instance:

  $ R "/opt/prplos/usr/bin/xpon-onu-hal-mock-dbgtool remove_instance ; sleep 1"

Check that there is no Transceiver.2 datamodel available:

  $ R "ubus call XPON.ONU.1.ANI.1.Transceiver.2 _get"
  Command failed: Not found
  [4]

Cleanup:

  $ R "pkill -9 -f xpon-onu-hal-mock" > /dev/null 2>&1
  $ R "/etc/init.d/tr181-xpon restart" > /dev/null 2>&1

  $ R "opkg remove mod-xpon-prpl-mock"
  Removing package mod-xpon-prpl-mock from root...
