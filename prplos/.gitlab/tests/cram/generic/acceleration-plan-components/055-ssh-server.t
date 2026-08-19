Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Add testing SSH server instance on LAN interface and port 1922:

  $ printf "\
  > ubus-cli SSH.Server.+{Alias='ci-testing',Port=1922,AllowPasswordLogin='False',AllowRootLogin='False',AllowRootPasswordLogin='False'}
  > ubus-cli SSH.Server.ci-testing.Interface=Device.IP.Interface.3.
  > ubus-cli SSH.Server.ci-testing.IPv4AllowedSourcePrefix="192.168.1.0/24"
  > ubus-cli SSH.Server.ci-testing.Enable=1
  > ubus-cli SSH.AuthorizedKey.+{Alias='ci-testing',User='Users.User.1.'}
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null; sleep 5

Check that the server is running:

  $ R "pgrep -f dropbear.*1922 --count"
  1

Check that root is not able to login with password:

  $ script --quiet --command "ssh -t -o StrictHostKeyChecking=no -p 1922 root@$TARGET_LAN_IP" | grep -c "Permission denied (publickey)"
  1

Enable password login:

  $ printf "\
  > ubus-cli SSH.Server.ci-testing.AllowPasswordLogin='True'
  > ubus-cli SSH.Server.ci-testing.AllowRootPasswordLogin='True'
  > ubus-cli SSH.Server.ci-testing.AllowRootLogin='True'
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null; sleep 5

Check datamodel:

  $ R "ubus call SSH.Server.3 _get | jsonfilter -e @[*].Status -e @[*].AllowPasswordLogin -e @[*].AllowRootLogin" | sort
  Enabled
  true
  true

Check that root is able to login with no password:

  $ ssh -o BatchMode=yes -o "UserKnownHostsFile=/dev/null" -o StrictHostKeyChecking=no -p 1922 root@$TARGET_LAN_IP 'exit 0' 2> /dev/null

Disable password login:

  $ R "ubus -S call SSH.Server.3 _set '{\"parameters\":{\"AllowRootPasswordLogin\":False}}'" ; sleep 2
  {"SSH.Server.3.":{"AllowRootPasswordLogin":false}}
  {}
  {"amxd-error-code":0}

Add public key:

  $ R "ubus -S call SSH.AuthorizedKey.1 _set '{\"parameters\":{\"Key\":\"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIHiusVUeBdR2CY8vcBY5MjKBs8zfIoyZ4kfrJfSM13PS\"}}'" ; sleep 2
  {"SSH.AuthorizedKey.1.":{"Key":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIHiusVUeBdR2CY8vcBY5MjKBs8zfIoyZ4kfrJfSM13PS"}}
  {}
  {"amxd-error-code":0}

Start two client connections using public key authentication:

  $ ssh -i ${CI_PROJECT_DIR}/.gitlab/certs/ssh-server/ed25519-ci -o "UserKnownHostsFile=/dev/null" -o BatchMode=yes -o StrictHostKeyChecking=no -p 1922 root@$TARGET_LAN_IP 'sleep 5' > /dev/null 2>&1 &
  $ sleep 1

  $ ssh -i ${CI_PROJECT_DIR}/.gitlab/certs/ssh-server/ed25519-ci -o "UserKnownHostsFile=/dev/null" -o BatchMode=yes -o StrictHostKeyChecking=no -p 1922 root@$TARGET_LAN_IP 'sleep 5' > /dev/null 2>&1 &
  $ sleep 1

Check the Session datamodel:

  $ R "ubus call SSH.Server.3.Session _get | jsonfilter -e @[*].IPAddress -e @[*].User -e @[*].Alias" | sort
  192.168.1.2
  192.168.1.2
  cpe-Session-2
  cpe-Session-3
  root
  root

Wait a bit and check that the Session datamodel is empty:

  $ sleep 4 ; R "ubus call SSH.Server.3.Session _get | jsonfilter -e @[*].IPAddress -e @[*].User -e @[*].Alias" | sort

Remove the testing SSH server:

  $ script --command "ssh -t root@$TARGET_LAN_IP 'ubus-cli SSH.Server.ci-testing-'" > /dev/null 2>&1 ; sleep 3

Check that the testing SSH server is not running:

  $ R "pgrep -f dropbear.*1922 --count"
  0
  [1]
