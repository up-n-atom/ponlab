#--------------------------------------------------------------------------
#
#  Copyright © 2020 MaxLinear, Inc.
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#--------------------------------------------------------------------------


CC=mips-linux-gcc
STRIP=mips-linux-strip

CFLAGS='-O2 -Wall -I ../../../../wireline_sw_linux26/ifx_wl_linux/include/'

echo $CC $CFLAGS -o ppacmd ppacmd.c

$CC $CFLAGS -o ppacmd ppacmd.c
$STRIP  ppacmd

