#--------------------------------------------------------------------------
#
#  Copyright © 2021 MaxLinear, Inc.
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#--------------------------------------------------------------------------


CC=mips-linux-gcc
STRIP=mips-linux-strip

CFLAGS='-O2 -Wall -I ../../../../wireline_sw_linux26/ifx_wl_linux/include/'

echo $CC $CFLAGS -o qoscmd qoscmd.c

$CC $CFLAGS -o qoscmd qoscmd.c
$STRIP  qoscmd

