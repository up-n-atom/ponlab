REM
REM <legal_notice>
REM  MaxLinear, Inc. retains all right, title and interest (including all intellectual
REM  property rights) in and to this computer program, which is protected by applicable
REM  intellectual property laws.  Unless you have obtained a separate written license from
REM  MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
REM  to utilize all or a part of this computer program for any purpose (including
REM  reproduction, distribution, modification, and compilation into object code), and you
REM  must immediately destroy or return all copies of this computer program.  If you are
REM  licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
REM  to utilize this computer program are limited by the terms of that license.
REM 
REM  This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
REM  authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
REM  of this computer program and related information and to not disclose this computer
REM  program and related information to any other person or entity.
REM 
REM  Misuse of this computer program or any information contained in it may results in
REM  violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
REM  trade secret, patent, contractual, and other legal rights.
REM 
REM  THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND MAXLINEAR, INC.
REM  EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE WARRANTIES OF
REM  MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.
REM 
REM ***************************************************************************************
REM                                         Copyright (c) 2021/2022, MaxLinear, Inc.
REM ***************************************************************************************
REM </legal_notice>
REM

@ECHO OFF
@CLS

ECHO DUT Automated Build Process

REM Setup
SET DUT_VERSION_FILENAME=version.inc
SET DUT_CHANGES_FILENAME=changes.inc
SET DUT_BUILD_TAG_FILENAME=build_tag.inc
SET DUT_RELEASE_NOTES_FILENAME=DUT_release_notes.txt
SET DUT_RELEASE_NOTES_TEMP_FILENAME=DUT_release_notes.tmp

SET OUTPUT_DIR=output
IF NOT EXIST %OUTPUT_DIR%\NUL MKDIR %OUTPUT_DIR%

SET REPORTS_DIR=reports
IF NOT EXIST %REPORTS_DIR%\NUL MKDIR %REPORTS_DIR%

IF NOT DEFINED DUT_BUILD_LOG (
  SET DUT_BUILD_LOG=%OUTPUT_DIR%\build.log
)
IF EXIST %DUT_BUILD_LOG% DEL %DUT_BUILD_LOG%
ECHO Build log: %DUT_BUILD_LOG%

IF NOT DEFINED DUT_DEVICE_IP_ADDRESS (
  SET DUT_DEVICE_IP_ADDRESS=192.168.1.1
)
IF NOT DEFINED DUT_DEVICE_WLAN_INDEX (
  SET DUT_DEVICE_WLAN_INDEX=0
)

SET BUILDER="%MICROSOFT_VISUAL_STUDIO_HOME%\2019\Professional\MSBuild\Current\Bin\MSBuild.exe" 
SET DEVENV="%MICROSOFT_VISUAL_STUDIO_HOME%\2019\Professional\Common7\IDE\devenv.exe" 
SET CPPCHECK="%CPPCHECK_HOME%\cppcheck.exe"
SET OPENCPPCOVERAGE="%OPENCPPCOVERAGE_HOME%\OpenCppCoverage.exe"

git -C ../shared_header rev-parse HEAD > shared_header_hash.txt
SET /P SHARED_HEADER_HASH=<shared_header_hash.txt
DEL shared_header_hash.txt


REM Get GIT branch name to customize the binaries if no specific identifier has been passed as input parameter (GCI)
if "%~1"=="" (
git rev-parse --abbrev-ref HEAD > build_tag.txt
) else (
    ECHO %1> build_tag.txt
)
SET /P BUILD_TAG=<build_tag.txt
DEL build_tag.txt
echo %BUILD_TAG%

PRINTF """%BUILD_TAG%""\r\n" > %DUT_BUILD_TAG_FILENAME%

REM Increase revision number
IF DEFINED DUT_SKIP_INCREASE_REVISION (GOTO DONE_INCREASE_REVISION) 
ECHO.
ECHO.
ECHO Increasing revision number ...
REM Read DUT version string and remove quotes
SET /p DUT_VERSION=<%DUT_VERSION_FILENAME%
IF NOT DEFINED DUT_VERSION (
  ECHO Error: Unknown version >> %DUT_BUILD_LOG%
  GOTO ERROR
) 
SET DUT_VERSION=%DUT_VERSION:"=%

REM Split version string into major, minor and revision numbers
for /f "tokens=1,2,3 delims=." %%a in ("%DUT_VERSION%") do (
  SET DUT_VERSION_MAJOR=%%a
  SET DUT_VERSION_MINOR=%%b
  SET DUT_VERSION_REVISION=%%c
)
  
REM Increase revision number
SET /A DUT_VERSION_REVISION=DUT_VERSION_REVISION+1

REM Write new DUT version string
SET DUT_VERSION=%DUT_VERSION_MAJOR%.%DUT_VERSION_MINOR%.%DUT_VERSION_REVISION%
ECHO "%DUT_VERSION%">%DUT_VERSION_FILENAME%

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to write version to %DUT_VERSION_FILENAME% >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_INCREASE_REVISION

REM Build projects (Configuration=Debug)
IF DEFINED DUT_SKIP_DEBUG_BUILD (GOTO DONE_DEBUG_BUILD) 
ECHO.
ECHO.
ECHO Building project (Configuration=Debug) ...

%BUILDER% dut_debug.slnf -target:Clean,Build -p:Configuration=Debug -p:Platform=x64 -fileLogger -fileLoggerParameters:LogFile=%DUT_BUILD_LOG%;Verbosity=normal;Append

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to build debug configuration >> %DUT_BUILD_LOG%
  GOTO ERROR
)  

REM Finally, build debug configuration for x86 platform (for backwards compatibility)
%BUILDER% dut_debug.slnf -target:Build -p:Configuration=Debug -p:Platform=x86 -fileLogger -fileLoggerParameters:LogFile=%DUT_BUILD_LOG%;Verbosity=normal;Append

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to build debug configuration for x86 platform >> %DUT_BUILD_LOG%
  GOTO ERROR
)  

:DONE_DEBUG_BUILD

REM Perform static code analysis with Cppcheck 
IF DEFINED DUT_SKIP_CPPCHECK (GOTO DONE_CPPCHECK) 
ECHO.
ECHO.
ECHO Running CppCheck ...

IF NOT EXIST %REPORTS_DIR%\cppcheck\NUL MKDIR %REPORTS_DIR%\cppcheck

CALL :RUN_CPPCHECK dut_lib/dut_lib.vcxproj
IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: CppCheck failed for project dut_lib >> %DUT_BUILD_LOG%
  GOTO ERROR
)

CALL :RUN_CPPCHECK dut_lib_tests/dut_lib_tests.vcxproj
IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: CppCheck failed for project dut_lib_tests >> %DUT_BUILD_LOG%
  GOTO ERROR
)

CALL :RUN_CPPCHECK dut_version_tool/dut_version_tool.vcxproj
IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: CppCheck failed for project dut_version_tool >> %DUT_BUILD_LOG%
  GOTO ERROR
)

CALL :RUN_CPPCHECK dut_cli/dut_cli.vcxproj
IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: CppCheck failed for project dut_cli >> %DUT_BUILD_LOG%
  GOTO ERROR
)

CALL :RUN_CPPCHECK dut_cli_tests/dut_cli_tests.vcxproj
IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: CppCheck failed for project dut_cli_tests >> %DUT_BUILD_LOG%
  GOTO ERROR
)

CALL :RUN_CPPCHECK dut_c_wrapper/dut_c_wrapper.vcxproj
IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: CppCheck failed for project dut_c_wrapper >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_CPPCHECK

REM Run unit-tests and create coverage report
REM Note: OpenCppCoverage should be used with a debug build. In release build, the compiler can aggressively optimize the source code and some inaccuracies might happen.
IF DEFINED DUT_SKIP_UNIT_TESTS (GOTO DONE_UNIT_TESTS) 
ECHO.
ECHO.
ECHO Running unit-tests and creating coverage report ...

SET UNIT_TESTS="output\x64\Debug\dut_lib_tests.exe"
%OPENCPPCOVERAGE% --sources dut_lib\* --excluded_sources "predefined C++ types" --modules dut_lib_tests.exe --export_type html:%REPORTS_DIR%/coverage/dut_lib -- %UNIT_TESTS% --gtest_filter=-_IntegrationTest.* --log_level=0 >> %DUT_BUILD_LOG%
SET EXIT_CODE=%errorlevel%
IF EXIST LastCoverageResults.log (
  MOVE LastCoverageResults.log %OUTPUT_DIR%\LastCoverageResults_dut_lib.log
) 

IF NOT ["%EXIT_CODE%"]==["0"] (
  ECHO Error: Unable to create coverage report >> %DUT_BUILD_LOG%
  GOTO ERROR
)

SET UNIT_TESTS="output\x64\Debug\dut_cli_tests.exe"
%OPENCPPCOVERAGE% --sources dut_cli\* --excluded_sources "predefined C++ types" --modules dut_cli_tests.exe --export_type html:%REPORTS_DIR%/coverage/dut_cli -- %UNIT_TESTS% >> %DUT_BUILD_LOG%
SET EXIT_CODE=%errorlevel%
IF EXIST LastCoverageResults.log (
  MOVE LastCoverageResults.log %OUTPUT_DIR%\LastCoverageResults_dut_cli.log
) 

IF NOT ["%EXIT_CODE%"]==["0"] (
  ECHO Error: Unable to create coverage report >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_UNIT_TESTS

REM Update version
IF DEFINED DUT_SKIP_UPDATE_VERSION (GOTO DONE_UPDATE_VERSION) 
ECHO.
ECHO.
ECHO Updating version ...

SET VERSION_TOOL="output\x64\Debug\dut_version_tool.exe"
%VERSION_TOOL% %CD%

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to update version information >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_UPDATE_VERSION

REM Build projects (Configuration=Release)
IF DEFINED DUT_SKIP_RELEASE_BUILD (GOTO DONE_RELEASE_BUILD) 
ECHO.
ECHO.
ECHO Building project (Configuration=Release) ...

%BUILDER% dut_release.slnf -target:Clean,Build -p:Configuration=Release -p:Platform=x64 -fileLogger -fileLoggerParameters:LogFile=%DUT_BUILD_LOG%;Verbosity=normal;Append

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to build release configuration >> %DUT_BUILD_LOG%
  GOTO ERROR
)

%BUILDER% dut_release.slnf -target:Build -p:Configuration=Release -p:Platform=x86 -fileLogger -fileLoggerParameters:LogFile=%DUT_BUILD_LOG%;Verbosity=normal;Append

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to build release configuration for x86 platform >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_RELEASE_BUILD

REM Run automation tests
IF DEFINED DUT_SKIP_AUTOMATION_TESTS (GOTO DONE_AUTOMATION_TESTS) 
ECHO.
ECHO.
ECHO Running automation tests

SET AUTOMATION_TESTS="output\Release\dut_automation.exe"
SET DUT_GUI="output\Release\dut_gui.exe"
%AUTOMATION_TESTS% -path %DUT_GUI% -case SHORT -ip %DUT_DEVICE_IP_ADDRESS% -wlan %DUT_DEVICE_WLAN_INDEX%

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Automation tests failed >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_AUTOMATION_TESTS

REM Create .pdf file with DUT API documentation using Doxygen 
IF DEFINED DUT_SKIP_CREATE_DOCUMENTATION (GOTO DONE_CRETE_DOCUMENTATION) 
ECHO.
ECHO.
ECHO Creating PDF file with DUT API documentation ...

REM Create output directory for Doxygen auto-generated files
SET DUT_DOXYGEN_DIR=%OUTPUT_DIR%\doxygen\dut_lib
IF EXIST %DUT_DOXYGEN_DIR%\NUL RD /s /q %DUT_DOXYGEN_DIR%
MKDIR %DUT_DOXYGEN_DIR%

REM Run Doxygen to generate documentation for the DUT library 
START /wait /b /d "dut_lib\doxygen" cmd /c doxygen

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to create DUT API documentation >> %DUT_BUILD_LOG%
  GOTO ERROR
)

REM Convert generated latex file to PDF
START /wait /b /d "%DUT_DOXYGEN_DIR%\latex" cmd /c make.bat

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to convert DUT API documentation to PDF file >> %DUT_BUILD_LOG%
  GOTO ERROR
)

REM Move generated PDF file to the reports directory
IF NOT EXIST %REPORTS_DIR%\doxygen\NUL MKDIR %REPORTS_DIR%\doxygen
MOVE %DUT_DOXYGEN_DIR%\latex\refman.pdf %REPORTS_DIR%\doxygen\dut_lib.pdf

:DONE_CREATE_DOCUMENTATION

REM Workaround for error "ERROR: An error occurred while validating.  HRESULT = '8000000A'"
REM This workaround seems to be not necessary when targeting .NET5 
REM https://stackoverflow.com/a/45580775
IF DEFINED DUT_SKIP_CREATE_INSTALLER (GOTO DONE_CREATE_INSTALLER) 
ECHO.
ECHO.
ECHO DisableOutOfProcBuild ...

PUSHD "%MICROSOFT_VISUAL_STUDIO_HOME%\2019\Professional\Common7\IDE\CommonExtensions\Microsoft\VSI\DisableOutOfProcBuild"
DisableOutOfProcBuild.exe
POPD 

REM Create installer (MSI)
ECHO.
ECHO.
ECHO Creating installer (MSI) ...

%DEVENV% dut.sln /Build "Release|x64" /Project dut_gui_setup /Out %DUT_BUILD_LOG%

IF NOT ["%errorlevel%"]==["0"] (
  ECHO Error: Unable to create installer >> %DUT_BUILD_LOG%
  GOTO ERROR
)

:DONE_CREATE_INSTALLER

REM Update release notes
IF DEFINED DUT_SKIP_UPDATE_RELEASE_NOTES (GOTO DONE_UPDATE_RELEASE_NOTES) 
ECHO.
ECHO.
ECHO Updating release notes ...

SET /P DUT_VERSION=<%DUT_VERSION_FILENAME%
IF NOT DEFINED DUT_VERSION (
  ECHO Error: Unknown version >> %DUT_BUILD_LOG%
  GOTO ERROR
) 
SET DUT_VERSION=%DUT_VERSION:"=%
ECHO Current version: %DUT_VERSION% (%BUILD_TAG%) >> %DUT_BUILD_LOG%

PRINTF "Version %DUT_VERSION% (%BUILD_TAG%), %DATE%\r\n" > %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "\r\nContent/Fixes:\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
TYPE %DUT_CHANGES_FILENAME% >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "\r\nShared headers hash:\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "%SHARED_HEADER_HASH%\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "\r\nVerified CV by DUT Automation:\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "\r\n----------------------------------------------------------------\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
PRINTF "\r\n" >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 
TYPE %DUT_RELEASE_NOTES_FILENAME% >> %DUT_RELEASE_NOTES_TEMP_FILENAME% 

MOVE %DUT_RELEASE_NOTES_TEMP_FILENAME% %DUT_RELEASE_NOTES_FILENAME%
IF EXIST %DUT_CHANGES_FILENAME% DEL %DUT_CHANGES_FILENAME%

:DONE_UPDATE_RELEASE_NOTES

:SUCCESS
ECHO "Done!"
EXIT 0

:ERROR
ECHO "Failed to create release!"
EXIT 1

:RUN_CPPCHECK
ECHO Processing file %1...
%CPPCHECK% --error-exitcode=1 --enable=style --inline-suppr --suppress=useStlAlgorithm --quiet --template="{file} ({line}): [{severity}] {message} ({id})" --output-file=%REPORTS_DIR%/cppcheck/%~nx1.cppcheck.txt --project=%1 --suppress=preprocessorErrorDirective:packages\gmock.1.11.0\lib\native\include\gtest\internal\gtest-port.h --library=googletest
EXIT /b
