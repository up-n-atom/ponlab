Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Create global date pattern to be used in various mathcing tests:

  $ datepattern="[0-9]{4} [A-Z][a-z]{2} [ 0-9][0-9] [0-9]{2}:[0-9]{2}:[0-9]{2}"

Check logs appears in /var/log/messages with expected format:

  $ logmarker="CRAM syslog test default logs"
  $ R logger -t cram $logmarker
  $ sleep 1
  $ logmatch=$(R "tail -100 /var/log/messages | grep -E '^$datepattern \w+ cram: $logmarker'")
  $ test -n "$logmatch" && echo "Log marker found"
  Log marker found

Check that filtering and command are working as expected (testing dhcp):

  $ dhcplogfile=$(R "ba-cli 'Syslog.Action.[Alias==\"dhcp\"].LogFile.FilePath?'" | grep -v '^>' | head -n -2  | sed -E 's|.*"file://(.*)"|\1|')
  $ R "test -s $dhcplogfile && echo 'log file is non empty'"
  log file is non empty

  $ dhcppattern=$(R "ba-cli 'Syslog.Filter.[Alias==\"dhcp\"].PatternMatch?'" | grep -v '^>' | head -n -2 | sed -E 's|.*="\^?(.+?)"|\1|')
  $ R "grep -v -E '^$datepattern [^ ]+ $dhcppattern' $dhcplogfile" || echo "Only expect filtered log found"
  Only expect filtered log found

Following section will test network sources and remote logging:
- source listen on br-lan 12345 (and open corresponding firewall port)
- action to write in specific log file and redirect as well to localhost listener
- localhost source configured by default as a ref for main
- send a netcat message on first source and check it is written both in specific logfile and /var/log/messages

Setup all through TR181:

  $ logmarker="CRAM remote log test"
  $ srcref=$(R "ba-cli 'Syslog.Source.+{Alias=\"cramRemoteSrc\",Network.Interface=\"Device.IP.Interface.3.\", Network.Port=\"12345\", Network.Enable=1}'| grep -E '^Syslog\.Source\.[0-9]+\.$'  ")
  $ srcref=${srcref%.}
  $ R "ba-cli 'Syslog.Action.+{Alias=\"cramRemoteDst\",SourceRef=\"$srcref\",TemplateRef=\"Syslog.Template.1\",LogFile.Enable=1,LogFile.FilePath=\"file:///var/log/messages_cram\",LogRemote.Address=\"127.0.0.1\",LogRemote.Enable=1,LogRemote.Port=514,LogRemote.Protocol=\"UDP\"}'" >/dev/null
  $ R "ba-cli 'Firewall.Service.+{Alias=\"cram-syslog\",DestPort=\"12345\",IPVersion=4,Protocol=\"17\", Action=\"Accept\",Enable=1}'" >/dev/null

syslog-ng needs a slight delay to reload and apply its configuration:

  $ sleep 2

Send log marker to remote source via netcat:

  $ echo $logmarker | nc -u -w 1 $TARGET_LAN_IP 12345
  $ sleep 1

Check log marker is in local log file (remote source --> local action):

  $ logmatch=$(R "grep -E '^$datepattern ([0-9]{1,3}\.){3}[0-9]{1,3} $logmarker' /var/log/messages_cram")
  $ test -n "$logmatch" && echo "Log marker found"
  Log marker found

Check log marker is in global message file (local action remote log --> default localhost remote source):

  $ logmatch=$(R "tail -100 /var/log/messages | grep -E '^$datepattern \w+ $logmatch'")
  $ test -n "$logmatch" && echo "Log marker found"
  Log marker found

Cleanup:

  $ R rm "/var/log/messages_cram"
  $ R "ba-cli 'Syslog.Action.[Alias==\"cramRemoteDst\"]-'" >/dev/null
  $ R "ba-cli 'Syslog.Source.[Alias==\"cramRemoteSrc\"]-'" >/dev/null
  $ R "ba-cli 'Firewall.Service.[Alias==\"cram-syslog\"]-'" >/dev/null
