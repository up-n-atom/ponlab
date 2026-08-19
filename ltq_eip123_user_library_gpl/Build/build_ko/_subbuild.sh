#!/bin/bash
#

# $1=build configuration (pci/mem/sim)
subbuild()
{
    echo "Building configuration: $1"
    cd $1
    make clean
    make
    cp umdevxs_k.ko ../umdevxs_k-$1.ko
    cd ..
}

echo "Running subbuild.sh"

subbuild pci
subbuild mem
subbuild sim

# end of file
