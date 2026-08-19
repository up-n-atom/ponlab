@ECHO off

TYPE c:\work\mmaz\dut\dut_cli\examples\commands.txt | dut_cli --fail-on-exception 2>1>NUL

IF ["%errorlevel%"]==["0"] (
  ECHO Ok
) ELSE (
  ECHO Error
)
