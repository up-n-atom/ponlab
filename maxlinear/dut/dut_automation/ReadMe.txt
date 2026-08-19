DUT Automation
================================================================================

The 'dut_automation' is a Windows console application to automatically test 
the user interface of the 'dut_gui' application. The automation application 
launches the 'dut_gui' and simulates a human exercising the user interface, 
but faster and in a consistent way. 

The 'dut_automation' has been written in C# and uses a library from Microsoft 
(called the Microsoft UI Automation library) to obtain references to controls 
in a form like buttons, edit boxes, drop-down lists, etc and interact with 
them. 

The application lets you select the test cases to run with a command line 
parameter. There are short tests and long tests, depending on the number of 
features they test. The longest test case, the one that checks all input 
combinations, may take up to 3 hours to complete. 

The automation tests require a board to be connected because they test the 
whole path from 'dut_gui' to the FW. This means that if the test clicks the 
“SetChannel” button for example, then the channel will be actually set on the 
FW.

The DUT automation is not connected to any measurement device, so it has no 
feedback about the RF behavior of the WiFi card and cannot check if 
everything works as expected.
