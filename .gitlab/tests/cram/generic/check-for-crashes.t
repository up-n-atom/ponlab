Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that there are no signs of crashes:

  $ R "logread" | \
  > grep -C10 -E \
  >      -e '(traps:.*general protection|segfault at [[:digit:]]+ ip.*error.*in)' \
  >      -e 'do_page_fault\(\): sending' \
  >      -e 'Unable to handle kernel.*address' \
  >      -e '(PC is at |pc : )([^+\[ ]+).*' \
  >      -e 'epc\s+:\s+\S+\s+([^+ ]+).*' \
  >      -e 'EIP: \[<.*>\] ([^+ ]+).*' \
  >      -e 'RIP: [[:xdigit:]]{4}:(\[<[[:xdigit:]]+>\] \[<[[:xdigit:]]+>\] )?([^+ ]+)\+0x.*'
  [1]
