Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Create testing user:

  $ printf "\
  > ubus-cli Users.Group.+{GroupID=999,Groupname='testgroup',Alias='testgroup'}
  > ubus-cli Users.User.+{UserID=666,Username='testuser',Alias='testuser',GroupParticipation='Users.Group.testgroup.',Password='password',Shell='Users.SupportedShell.ash-shell.'}
  > sleep 5; passwd -d root
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null
  $ sleep 1

Check that user and group exists:

  $ R "grep testgroup /etc/group"
  testgroup:x:999:testuser

  $ R "grep testuser /etc/passwd"
  testuser:x:666:999:testuser:/var:/bin/ash

  $ R "grep -c testuser /etc/shadow"
  1

Check that we are able to login:

  $ sshpass -ppassword ssh testuser@$TARGET_LAN_IP id
  uid=666(testuser) gid=999(testgroup) groups=999(testgroup),999(testgroup)

Change password to unsecure word:

  $ printf "\
  > ubus-cli Users.User.testuser.Password="unsecure"
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null
  $ sleep 1

Check that we are able to login with unsecure password:

  $ sshpass -punsecure ssh testuser@$TARGET_LAN_IP id
  uid=666(testuser) gid=999(testgroup) groups=999(testgroup),999(testgroup)

Delete group and user:

  $ printf "\
  > ubus-cli Users.User.testuser.-
  > ubus-cli Users.Group.testgroup.-
  > " > /tmp/cram
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/cram)'" > /dev/null
  $ sleep 1

Check that user and group does not exists:

  $ R "grep testgroup /etc/group"
  [1]

  $ R "grep testuser /etc/passwd"
  [1]

  $ R "grep testuser /etc/shadow"
  [1]

Check that we are not able to login:

  $ sshpass -punsecure ssh testuser@$TARGET_LAN_IP id
  Permission denied, please try again.\r (esc)
  [5]
