#!/bin/sh
##
## File: bootstrap.sh
##
## Generates configure for configuring this package with automake+autoconf
##
## Copyright (c) 2008-2013 INSIDE Secure B.V. All Rights Reserved.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program. If not, see <http://www.gnu.org/licenses/>.
##

# Omit warnings
aclocal 2>/dev/null \
  && automake --add-missing --copy 2>/dev/null \
  && autoconf 2>/dev/null

if test "$?" != "0";
then
  # Error occured in processing, then output the error
  aclocal \
    && automake --add-missing --copy \
    && autoconf
  exit $?
fi

# Bootstrap subdirectories
for dir in `find . -name build_linux -type d`
do
    (cd $dir; if [ -e ./bootstrap.sh ]; then /bin/sh ./bootstrap.sh; fi)
done

# end of file
