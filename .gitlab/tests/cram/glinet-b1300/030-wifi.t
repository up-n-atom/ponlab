Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check for correct SSID setup:

  $ R "iw dev | grep -e Interface -e ssid | sort"
  \tInterface wlan0 (esc)
  \tInterface wlan1 (esc)