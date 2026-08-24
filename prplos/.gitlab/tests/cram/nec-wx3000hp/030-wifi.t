Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check for correct SSID setup:

  $ R "iwinfo" | awk '/wlan[0-9.]+/{printf("%s %s ",$1,$3)};/Point:/{print $3}' | grep -v 00:00:00:00:00:00
  [1]
