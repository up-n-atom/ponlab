Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Copy over testing certificates:

  $ R "mkdir -p /etc/config/autocert"
  $ scp ${CI_PROJECT_DIR}/.gitlab/certs/tr181-security/autocert/* "root@${TARGET_LAN_IP}:/etc/config/autocert/"

Restart tr181-security service:

  $ R "/etc/init.d/tr181-security restart" > /dev/null 2>&1

Check that certs are in place as expected:

  $ R "ubus -S call Security.Certificate _get | jsonfilter -e @[*].Enable -e @[*].Subject -e @[*].SignatureAlgorithm -e @[*].NotBefore | LC_ALL=C sort"
  /C=US/O=PrplFoundation/OU=prplOS/CN=prplOS.lan
  /C=US/O=PrplFoundation/OU=prplOS/CN=prplOS.lan
  2023-12-04T17:41:08.* (re)
  2023-12-04T17:41:08.* (re)
  ecdsa-with-SHA512
  sha512WithRSAEncryption
  true
  true

Remove first certificate from the system:

  $ R "rm /etc/config/autocert/testing*1.pem"

Restart tr181-security service:

  $ R "/etc/init.d/tr181-security restart" > /dev/null 2>&1 ; sleep .5

Check that the first certificate is not present anymore:

  $ R "ubus -S call Security.Certificate _get | jsonfilter -e @[*].Enable -e @[*].Subject -e @[*].SignatureAlgorithm -e @[*].NotBefore | LC_ALL=C sort"
  /C=US/O=PrplFoundation/OU=prplOS/CN=prplOS.lan
  2023-12-04T17:41:08.* (re)
  ecdsa-with-SHA512
  true
