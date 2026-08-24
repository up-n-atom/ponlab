Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that we've correct system info:

  $ R "ubus call system board | jsonfilter -e @.system -e @.model -e @.board_name"
  Intel(R) Atom(TM) Quad IA CPU  @ 2.49GHz
  mxl,lightning mountainmxl,lgp
  mxl,lgm
