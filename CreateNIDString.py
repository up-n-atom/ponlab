###################################################################################
#
#        Copyright © 2023 MaxLinear, Inc.
#        Copyright (C) 2017-2018 Intel Corporation
#        Lantiq Beteiligungs-GmbH & Co. KG
#        Lilienthalstrasse 15, 85579 Neubiberg, Germany
#        For licensing information, see the file 'LICENSE' in the root folder of
#        this software module.
###################################################################################

#! /opt/TWWfsw/bin/python
#! /usr/bin/python

import sys
from collections import deque
import argparse

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description='Update Error String in ugw_error.c file from ugw_error.h file comment section of enums')
parser.add_argument('InFile', nargs=1, type=argparse.FileType('r'), help='Header file which contains the enums with comments')
parser.add_argument('OutFile', nargs=1, type=argparse.FileType('r+'), help='C file which contains the default switch statement')
cargs = parser.parse_args()

for hfilename in cargs.InFile:
    pass
for cfilename in cargs.OutFile:
    pass

bufile = []
errcase = []
hfcount = 0
cfcount = 0

# Creating the case statement and mapping error sting from ugw_error.h file
start = 0
with open(hfilename.name, "r", encoding="utf-8", errors="ignore") as hfile:
    line = hfile.readline()
    while (line != ''):
        word = line.split()
        if ("typedef" in word and "enum" in word):
            start = 1
        elif ('}' in word):
            start = 0
        elif start == 1 and ('/*!<' in word):
            comment = deque(line.split('/*!<'))
            enum = deque(comment.popleft().split(','))
            errstr = enum.popleft()
            errstr=''.join(errstr.split())
            errcase.append("case \t"+ errstr + ": "+"return \""+errstr+ "\";\n")
            hfcount = hfcount + 1
        else:
            pass
        line = hfile.readline()
# Closing the file
hfile.close()

# Buffering the existing mapping string case to append it later from existing C file
start = 0
with open(cfilename.name, "r", encoding="utf-8", errors="ignore") as cfile:
    line = cfile.readline()
    while (line != ''):
        if("switch" in line):
            start = 1
        line = cfile.readline()
        word = line.split()
        if(start == 1):
            bufile.append(line)
            if('}' not in word):
                cfcount = cfcount + 1
# Closing the file
cfile.close()

# Check if already C file is updated
# If not, Write the case and mapping error string from ugw_error.h file to the ugw_error.c file
if (cfcount > hfcount):
    print ("\nDo clean before compile to get the updated \"" + cfilename.name + "\" File !!\n")
else:
    text = []
    with open(cfilename.name, 'r+') as cfile:
        lines = cfile.readlines()
        for line in lines:
            text.append(line)
            if("switch" in line):
                for error in errcase:
                    text.append("\t\t"+error)
                for buf in bufile:
                    text.append(buf)
                break
        cfile.seek(0)
        cfile.writelines(text)
        print ("\nFile \"" + cfilename.name + "\" Updated Successfully\n")
    # Closing the file
    cfile.close()
