Device Under Test (DUT)

Description
================================================================================

This repository contains the source code of the DUT library and DUT_GUI 
application as well as the required custom and 3rd. party libraries and tools.

The DUT library 'dut_lib' is a Windows static library that defines a set of 
API functions to test and calibrate WiFi cards. This library has been written 
in the C++ language, as a port of an initial version that was written in C#. 

The project 'dut_lib_tests' contains a set of unit-tests for the DUT library. 
These tests have been written using the GoogleTest and GoogleMock libraries.

The DUT library 'dut_dll' is a Windows dynamic library offering the same API as
the static library 'dut_lib'.

The project 'dut_version_tool' is a Windows console application to set version
and copyright information to the rest of projects during the creation of a new
release. 

The project 'dut_cli_wrapper' defines a Windows dynamic library with a set of 
C++/CLI wrapper classes for the DUT library. This wrapper provides 
interoperability with Microsoft .NET languages such as C#.

The project 'dut_c_wrapper' defines a Windows dynamic library with a set of 
C wrapper functions for the DUT library. This wrapper provides interoperability 
with almost any other language and is best suited for explicit linking (a.k.a. 
run-time dynamic linking).

The DUT GUI application 'dut_gui' is a Windows application that provides a 
Graphical User Interface to interact with a WiFi card using the DUT library.

The 'dut_gui' application has been written in C# and therefore requires the 
'dut_cli_wrapper' C++/CLI wrapper to use the 'dut_lib' C++ library.

The 'dut_automation' is a Windows console application to automatically test 
the user interface of the 'dut_gui' application. The automation application 
launches the 'dut_gui' and simulates a human exercising the user interface, 
but faster and in a consistent way. 

The 'dut_cli' is a Windows console application that provides a Command Line 
Interface to call the DUT API functions. The project 'dut_cli_tests' contains 
the unit-tests for this application.

The 'dut_dev' project is meant to create dut_dev.zip file, which contains all
the necessary header files, library files and examples of use for a developer 
to write a program using the DUT library in any of its flavors. 
This .zip file is copied by the installer in the installation directory. 

Finally, the 'dut_gui_setup' is a Microsoft Visual Studio Installer Project to 
create a setup file (MSI) for the 'dut_gui' and 'dut_cli' applications.

Shared headers
================================================================================

The DUT library uses C header files contained in the 'shared_header' 
repository. These header files contain the structures and the message 
definitions exchanged between the DUT library and the FW running in the device.

Because of this dependency, the DUT library and GUI are tightly coupled to the 
combined version running in the device. Put in other words, the shared headers 
used must be compatible with those used by the FW running in the device or 
otherwise errors might happen (and that kind of errors are very difficult to 
debug). 

The different projects in the 'dut' repository that use header files in the 
'shared_header' repository expect it to have been cloned to a directory called
'shared_header' at the same level as the directory where the 'dut' repository 
was cloned to.

Build instructions
================================================================================

In order to build DUT, it is required a Windows server with the following 
tools installed:

- Visual Studio Professional 2019 with the following optional components:
	- .NET desktop development
	- Desktop development with C++
	- Windows 10 SDK
	- .NET SDK (out of support). This installs version 5.0.416.
- CppCheck (tested with version 2.6)
https://cppcheck.sourceforge.io/
- OpenCppCoverage (tested with version Version: 0.9.9.0)
https://github.com/OpenCppCoverage/OpenCppCoverage/releases
- 7-zip (tested with version 21.07 (x64))
https://www.7-zip.org/
- GIT for Windows
https://git-scm.com/downloads
- Doxygen (tested with version 1.12.0)
https://www.doxygen.nl/download.html
https://www.doxygen.nl/files/doxygen-1.12.0-setup.exe
- TeX Live (note: it may take a few hours depending on the internet)
https://www.tug.org/texlive/windows.html#install
https://mirror.ctan.org/systems/texlive/tlnet/install-tl-windows.exe

Additionally, such Windows server must define the following environment
variables:
MICROSOFT_VISUAL_STUDIO_HOME=C:\Program Files (x86)\Microsoft Visual Studio
CPPCHECK_HOME=C:\Program Files\Cppcheck
OPENCPPCOVERAGE_HOME=C:\Program Files\OpenCppCoverage
ZIP_HOME=C:\Program Files\7-Zip

To have access to the 'printf' command, add the directory containing the "Git 
for Windows" binaries to the PATH environment variable:
SET PATH=%PATH%;C:\Program Files\Git\usr\bin

If required, before running the build script, set the version and copyright 
information by editing files "version.inc" and "copyright.inc" respectively.

The applications and libraries can be built by simply executing this build 
script:

  > release.bat

Optionally, you can add a parameter to customize the applications and 
libraries with this identifier as part of the version information. When 
creating a pre-release for testing some changes before merging them,
a good choice for the tag would be the JIRA ID, but it can be any string:

  > release.bat custom_r182

This Windows batch file goes through the following steps:
- Increase revision number in file "version.inc"
- Build 'dut_lib' and 'dut_cli' projects in Debug mode.
- Perform static code analysis using Cppcheck.
- Run unit-tests for 'dut_lib' and 'dut_cli' projects and create coverage 
reports.
- Copy version and copyright information from dut\dut_lib\VersionInfo.h to the 
rest of projects using the version setter tool.
- Build 'dut_lib', 'dut_dll', 'dut_cli', 'dut_cli_wrapper' and 'dut_gui' 
projects in Release mode.
- Run automation tests (requires a device operating in DUT mode). Device IP 
address and WLAN index are configurable through the environment variables 
DUT_DEVICE_IP_ADDRESS (if not set default is 192.168.1.1) and 
DUT_DEVICE_WLAN_INDEX (if not set default is 0) respectively.
- Create a PDF file with the DUT API documentation extracted from Dut.h file.
- Create installer (MSI) for dut_gui and dut_cli applications and for the 
files required to develop applications using DUT library (include and library 
files all packed in dut_dev.zip).
- Update the release notes file.

Each one of the previous steps can be disabled by setting an environment 
variable before executing the script. For example, to build without running 
the automation tests, issue the following command:

  > set DUT_SKIP_AUTOMATION_TESTS=1 & release.bat
  
If any of the previous steps fails, the batch file stops with an error exit code.
On success, the installer file can be found in folder:

  dut\dut_gui_setup\Release\

All the binary artifacts created during the process can be found in:

  dut\output
  
The DUT API auto-generated documentation, Cppcheck results and test coverage 
report can be found in the reports folder:

  dut\reports



