Check that REST API is working on non-protected parameters:

  $ curl --silent --max-time 3 "http://192.168.1.1/serviceElements/Device.DeviceInfo.FriendlyName"
  [{"parameters":{"FriendlyName":"prplHGW"},"path":"Device.DeviceInfo."}] (no-eol)

Check we can get protected parameters:

  $ session_id=$(curl --silent -X POST "http://192.168.1.1/session" --data '{"username":"admin","password":"admin"}' --max-time 3 | jq -r .sessionID)
  $ curl -X GET "http://192.168.1.1/serviceElements/Device.Time.Client.1.Version" -H "Authorization: bearer $session_id" --silent --max-time 3
  [{"parameters":{"Version":4},"path":"Device.Time.Client.1."}] (no-eol)

Check we have working commands endpoint:

  $ curl --silent --max-time 3 \
  > -H "Authorization: bearer $session_id" \
  > -H "Content-type: application/json" "http://192.168.1.1/commands" \
  > --data '{ "sendresp": true, "command": "Device.ScheduleTimer()", "inputArgs": {"DelaySeconds":1}}'
  [{"outputArgs":{"ScheduleTimer":""},"executed":"Device.ScheduleTimer()"}] (no-eol)

Check that commands endpoint ACL is working:

  $ curl -i --silent --max-time 3 \
  > -H "Content-type: application/json" "http://192.168.1.1/commands" \
  > --data '{ "sendresp": true, "command": "Device.ScheduleTimer()", "inputArgs": {"DelaySeconds":1}}' | grep Forbidden
  HTTP/1.1 403 Forbidden\r (esc)

Check we cannot access protected datamodel sections:

  $ curl -X GET -i "http://192.168.1.1/serviceElements/Security." -H "Authorization: bearer $session_id" --silent --max-time 3 | grep Forbidden
  HTTP/1.1 403 Forbidden\r (esc)

Check that REST API is not usable with invalid credentials:

  $ curl -X POST -i "http://192.168.1.1/session" --data '{"username":"admin","password":"failure"}' --silent --max-time 3 | grep "Bad Request"
  HTTP/1.1 400 Bad Request\r (esc)
