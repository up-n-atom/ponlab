Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check the root datamodel settings:

  $ R "ba-cli --json ProcessMonitor.?0 | sed -n '2p'" | jq --sort-keys '.[0]'
  {
    "ProcessMonitor.": {
      "CycleDuration": \d+, (re)
      "LastReboot": "0001-01-01T00:00:00Z",
      "MaxReboots": 3,
      "NumberOfTest": \d+, (re)
      "RebootReason": "Unknown",
      "TestCustomTimeout": 5000,
      "TestInterval": \d+, (re)
      "TestPluginTimeout": 3000
    }
  }

Check that only one instance of tr181-led manager is running:

  $ R "pgrep -cf 'tr181-led -D'"
  1

Get current tr181-led manager PID:

  $ current_pid=$(R "cat /var/run/tr181-led.pid")

Add test for checking tr181-leds manager using PID:

  $ R "ba-cli 'ProcessMonitor.Test+{Type=Process,Name=tr181-led,Subject=/var/run/tr181-led.pid,FailAction=RESTART,TestInterval=2,MaxFailNum=1}' | grep -v '^>' | head -n -2"
  ProcessMonitor.Test.\d+. (re)

Check the LED manager check datamodel settings:

  $ R "ba-cli --json ProcessMonitor.Test.[Name==\\\"tr181-led\\\"]? | sed -n '2p'" | jq --sort-keys '.[0]'
  {
    "ProcessMonitor.Test.\d+.": { (re)
      "CurrentTestInterval": 2,
      "FailAction": "RESTART",
      "FailedSince": "0001-01-01T00:00:00Z",
      "Health": "Awaiting check",
      "LastCheck": "0001-01-01T00:00:00Z",
      "LastFailAction": "0001-01-01T00:00:00Z",
      "LastSuccess": "0001-01-01T00:00:00Z",
      "MaxFailDuration": -1,
      "MaxFailNum": 1,
      "Name": "tr181-led",
      "NumFailActions": 0,
      "NumFailed": 0,
      "Subject": "/var/run/tr181-led.pid",
      "SuccessfulSince": "0001-01-01T00:00:00Z",
      "TestInterval": 2,
      "TestIntervalMultiplier": 1,
      "TestResetInterval": 3600,
      "Type": "Process"
    }
  }

Shorthen the test cycle duration to 1 second:

  $ R "ba-cli 'ProcessMonitor.CycleDuration=1' | grep -v '^>' | head -n -2"
  ProcessMonitor.
  ProcessMonitor.CycleDuration=1

Kill the LED manager service:

  $ R "kill \$(cat /var/run/tr181-led.pid)"

Check that LED manager is not running:

  $ sleep 1

  $ R "pgrep -cf 'tr181-led -D'"
  0
  [1]

Check that ProcessMonitor have restarted the LED manager properly:

  $ R "ubus -t5 wait_for LEDs"

Check that one LED manager instance is running:

  $ R "pgrep -cf 'tr181-led -D'"
  1

Check that PIDs are different:

  $ new_pid=$(R "cat /var/run/tr181-led.pid")
  $ test $current_pid -ne $new_pid

Set the test cycle duration to 10 seconds:

  $ R "ba-cli 'ProcessMonitor.CycleDuration=10' | grep -v '^>' | head -n -2"
  ProcessMonitor.
  ProcessMonitor.CycleDuration=10
