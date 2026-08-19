This directory contains the Doxygen configuration file 'Doxyfile', which is 
required by the Doxygen tool to automatically create documentation of the 
the DUT API from the dut_lib\include\dut\Dut.h header file.

The 'Doxyfile' file has been created with the following command:

> doxygen -g

A backup of the default configuration file has been saved to 
'Doxyfile.original' for comparison purposes. Then, the 'Doxyfile' 
configuration file has then been modified to accomodate the project's needs. 

To generate the documentation for the project just enter:

> doxygen

For more information about Doxygen, see https://www.doxygen.nl/

Doxygen can create documentation in different formats but the 'Doxyfile' is set
to produce a LaTeX output only. Later, the build script converts the generated 
file to a PDF (Portable Document Format) with the "TeX Live on Windows" tool 
(see https://www.tug.org/texlive/windows.html#w64).

In summary, documentation for the DUT library is created as a .pdf document 
from within the release.bat script located in the root folder of this 
repository. Such document is created at reports\doxygen\dut_lib.pdf whenever a 
new CV is released.


