Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Assure that sysntpd is not running:

  $ R "pgrep -ax ntpd"
  [1]

Assure that sleep command supports float duration:

  $ R "sleep .05"
