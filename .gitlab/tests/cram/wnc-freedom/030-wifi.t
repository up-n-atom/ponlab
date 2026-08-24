Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check for correct SSID setup:

  $ R "iw dev | grep -e Interface -e ssid | sort"
  \tInterface wlan0 (esc)
  \tInterface wlan0.1 (esc)
  \tInterface wlan1 (esc)
  \tInterface wlan1.1 (esc)
  \tInterface wlan2 (esc)
  \tInterface wlan2.1 (esc)
  \tInterface wlan2.2 (esc)
