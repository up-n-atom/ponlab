##
## File: Makefile.am
##
## Template for building Makefile with automake+configure.
## The Makefile allows building EIP-123 DDK.
##

##############################################################################
## Copyright (c) 2009-2015 INSIDE Secure B.V. All Rights Reserved.
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
##############################################################################

ACLOCAL_AMFLAGS = -I ../../m4
AM_CFLAGS = -std=gnu99 -Wall -pedantic
top_src = $(top_srcdir)/../..
list_mk_prefix := ../../

## Build instructions for DDK / SafeZone

# Dependencies on this Makefile: libcal.a

#
# Configuration Options:
#
# ENABLE_CUSTOM
#    Indicates that the customer-specific configuration is selected
# ENABLE_VERSATILE
#    Indicates that the Versatile configuration is selected
# Note: ENABLE_CUSTOM/VERSATILE are multiple-exclusive
# WITH_CM_HW1 requests EIP-122 CM support ('legacy')
# WITH_CM_HW2 requests EIP-123 CM support
#
# ENABLE_DEBUG
#    Enable debugging
# ENABLE_STACK
#    Enable stack tracing
# ENABLE_HEAP
#    Enable heap tracing
# ENABLE_COVERAGE
#    Enable coverage tracing
# ENABLE_POLLING
#    Enable polling instead of interrupt
# ENABLE_ASM_X86
#    Enables optimization assembly code for x86
# ENABLE_SST_FLASH
#    Enable storage of SST objects in (emulated) FLASH
#

list_mk_prefix := ../../

if ENABLE_CUSTOM
CPPFLAGS += -DCFG_ENABLE_TARGET_CUSTOM
endif

if ENABLE_VERSATILE
CPPFLAGS += -DCFG_ENABLE_TARGET_VERSATILE
endif

if WITH_CM_HW1
CPPFLAGS += -DCFG_ENABLE_CM_HW1
endif

if WITH_CM_HW2
CPPFLAGS += -DCFG_ENABLE_CM_HW2
endif

# Note: This option is only for debugging with SFZUTF.
if ENABLE_STACK
CFLAGS += -finstrument-functions
CPPFLAGS += -DSTACK_MEASUREMENT -DCFG_ENABLE_STACK
endif

# Note: This option is only for debugging with SFZUTF.
if ENABLE_HEAP
CPPFLAGS += -DHEAP_MEASUREMENT -DCFG_ENABLE_HEAP
endif

if ENABLE_GCC_STRICT_WARNINGS
AM_CFLAGS += -W -Wshadow -Wvla
endif

if ENABLE_DEBUG
# Debugging enabled.
CPPFLAGS += -DCFG_ENABLE_DEBUG
else
# No debugging (performance build)
CPPFLAGS += -DCFG_IMPLDEFS_NO_DEBUG
endif   # ENABLE_DEBUG

if ENABLE_COVERAGE
CPPFLAGS += -DGCOV_PROFILE -DCFG_ENABLE_COVERAGE
LIBS += -lgcov
if ENABLE_DEBUG
# With debugging, test coverage analysis requires enabling additional flag to
# dump coverage reports prior abnormal exit
CPPFLAGS += -DDEBUG_CF_ABORT_WRITE_PROFILE
endif   # ENABLE_DEBUG
endif   # ENABLE_COVERAGE

if ENABLE_POLLING
CPPFLAGS += -DCFG_ENABLE_POLLING
endif

CONFIGURATION_INCLUDES = -I$(top_src)/Config

DEFAULT_INCLUDES_BASE := $(DEFAULT_INCLUDES)

FMWK_HEADER_LIST = \
    $(top_src)/Framework/PUBDEFS/incl/public_defs.h \
    $(top_src)/Framework/EE_ID_API/incl/ee_id.h

include ../../CAL/CAL_API/incl/list.mk
include ../../CAL/CAL_CONTEXT/incl/list.mk
CAL_HEADER_LIST = \
    $(CAL_CAL_API_incl_list_h) \
    $(CAL_CAL_CONTEXT_incl_list_h)

# following files will be installed
include_HEADERS = \
    $(FMWK_HEADER_LIST) \
    $(CAL_HEADER_LIST)

EXTRA_PROGRAMS =

# following programs will be installed
bin_PROGRAMS = \
    @safezone_config_name@

# following composite libraries will be installed
lib_LIBRARIES = \
    libcal.a

noinst_PROGRAMS =

# the following libraries will NOT be installed
noinst_LIBRARIES = \
    libfmwk.a \
    libcal_cm_v1.a \
    libcal_cm_v2.a \
    libcal_hw.a

if ENABLE_VERSATILE
noinst_LIBRARIES += \
    libumdevxs.a \
    libtarget_versatile.a
endif

# No check programs or libraries.
check_PROGRAMS =
check_LIBRARIES =

# Include additional tests (if provided).
libcal_hw_a_SOURCES=
CAL_SIM_LIBS=
include test.am

#----------------------------------------------------------------------------
# libfmwk: Library with Framework implementation (CLIB, DEBUG, EE_ID, SPAL)
#----------------------------------------------------------------------------

libfmwk_a_CPPFLAGS = \
    $(CONFIGURATION_INCLUDES) \
    -I$(top_src)/Framework/PUBDEFS/incl \
    -I$(top_src)/Framework/IMPLDEFS/incl \
    -I$(top_src)/Framework/CLIB/incl \
    -I$(top_src)/Framework/DEBUG/incl \
    -I$(top_src)/Framework/EE_ID_API/incl \
    -I$(top_src)/Framework/SPAL_API/incl

if ENABLE_HEAP
libfmwk_a_CPPFLAGS += \
    -I$(top_src)/SFZUTF/incl
endif   # ENABLE_HEAP

include ../../Framework/CLIB/src/list.mk
include ../../Framework/DEBUG/src/list.mk
include ../../Framework/EE_ID_GENERIC/src/list.mk
include ../../Framework/SPAL_MEMORY_ANSI/src/list.mk
include ../../Framework/SPAL_POSIX/src/list.mk
libfmwk_a_SOURCES = \
    $(Framework_CLIB_src_list_c) \
    $(Framework_DEBUG_src_list_c) \
    $(Framework_EE_ID_GENERIC_src_list_c) \
    $(Framework_SPAL_MEMORY_ANSI_src_list_c) \
    $(Framework_SPAL_POSIX_src_list_c)

#----------------------------------------------------------------------------
# libcal_hw: Library with the CAL_HW implementation
#----------------------------------------------------------------------------

libcal_hw_a_CPPFLAGS = \
    $(CONFIGURATION_INCLUDES) \
    -I$(top_src)/Framework/PUBDEFS/incl \
    -I$(top_src)/Framework/IMPLDEFS/incl \
    -I$(top_src)/Framework/CLIB/incl \
    -I$(top_src)/Framework/EE_ID_API/incl \
    -I$(top_src)/Framework/SPAL_API/incl \
    -I$(top_src)/CAL/CAL_API/incl \
    -I$(top_src)/CAL/CAL_HW/incl \
    -I$(top_src)/Kit/DriverFramework/v4_safezone/Basic_Defs/incl \
    -I$(top_src)/Kit/DriverFramework/v4_safezone/CLib_Abstraction/incl \
    -I$(top_src)/Kit/DriverFramework/v4/Device_API/incl \
    -I$(top_src)/Kit/DriverFramework/v4/DMAResource_API/incl \
    -I$(top_src)/Kit/EIP201_SL/incl \
    -I$(top_src)/Kit/EIP28_SL/incl \
    -I$(top_src)/Kit/Log/incl \
    -I$(top_src)/Kit/Log/src/safezone \
    -I$(top_src)/Integration/DMARes_Record/incl \
    -I$(top_src)/Integration/Identities/incl \
    -I$(top_src)/Integration/InterruptDispatcher/incl \
    -I$(top_src)/Integration/OneTimeInit/incl \
    -I$(top_src)/Integration/UMDevXS/UserPart/incl

if ENABLE_VERSATILE
libcal_hw_a_SOURCES += \
    $(top_src)/CAL/CAL_HW/src/cal_hw_init_cm-fpga.c
endif

if ENABLE_CUSTOM
libcal_hw_a_SOURCES += \
    $(top_src)/CAL/CAL_HW/src/cal_hw_init_cm-custom.c
endif

if WITH_CM_HW1
libcal_hw_a_CPPFLAGS += \
    -I$(top_src)/Kit/EIP122_CM_Tokens/incl \
    -I$(top_src)/Kit/EIP122_SL/incl

libcal_hw_a_SOURCES += \
    $(top_src)/CAL/CAL_HW/src/cal_hw_v1.c \
    $(top_src)/Kit/EIP122_SL/src/eip122_sl.c \
    $(top_src)/Kit/EIP122_SL/src/eip122_interrupt.c
endif

if WITH_CM_HW2
libcal_hw_a_CPPFLAGS += \
    -I$(top_src)/Kit/EIP123_CM_Tokens/incl \
    -I$(top_src)/Kit/EIP123_SL/incl

libcal_hw_a_SOURCES += \
    $(top_src)/CAL/CAL_HW/src/cal_hw_v2.c \
    $(top_src)/Integration/Identities/src/identities_ee_id.c \
    $(top_src)/Kit/EIP123_SL/src/eip123_sl.c
endif

#----------------------------------------------------------------------------
# libcal_cm_v1: Library with the CAL implementation for EIP-122 CM
#----------------------------------------------------------------------------

if WITH_CM_HW1

libcal_cm_v1_a_CPPFLAGS = \
    $(CONFIGURATION_INCLUDES) \
    $(libcal_hw_a_CPPFLAGS) \
    -I$(top_src)/CAL/CAL_CONTEXT/incl \
    -I$(top_src)/CAL/CAL_DISPATCHER/incl

libcal_cm_v1_a_SOURCES = \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_init.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_ae_asset.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_ae_auth_crypt.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_ae_registry.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_aesdes.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_aesf8.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_arc4.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_cmac.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_dma.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_hash.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_hmac.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_nop.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_nvm.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_random.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_random_selftest.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_tokenexchange.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_featurematrix_amend.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_symm_crypto.c \
    $(top_src)/CAL/CAL_CM-v1/src/cal_cm-v1_read_version.c \
    $(top_src)/Kit/EIP122_CM_Tokens/src/cm1_tokens_common.c \
    $(top_src)/Kit/EIP122_CM_Tokens/src/cm1_tokens_errdetails.c \
    $(top_src)/Kit/EIP122_SL/src/eip122_dma.c

endif   # WITH_CM_HW1

#----------------------------------------------------------------------------
# libcal_cm_v2: Library with the CAL implementation for EIP-123 CM
#----------------------------------------------------------------------------

if WITH_CM_HW2

libcal_cm_v2_a_CPPFLAGS = \
    $(CONFIGURATION_INCLUDES) \
    $(libcal_hw_a_CPPFLAGS) \
    -I$(top_src)/CAL/CAL_CONTEXT/incl \
    -I$(top_src)/CAL/CAL_DISPATCHER/incl \
    -I$(top_src)/Simulation/CMSim/incl

libcal_cm_v2_a_SOURCES = \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_init.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_aesdes.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_aesf8.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_arc4.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_camellia.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_c2.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_multi2.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_asset.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_cmac.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_dma.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_hash.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_hmac.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_nop.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_nvm.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_random.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_random_selftest.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_tokenexchange.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_featurematrix_amend.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_symm_crypto.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_read_version.c \
    $(top_src)/CAL/CAL_CM-v2/src/cal_cm-v2_aunlock.c \
    $(top_src)/Kit/EIP123_CM_Tokens/src/cm_tokens_common.c \
    $(top_src)/Kit/EIP123_CM_Tokens/src/cm_tokens_errdetails.c \
    $(top_src)/Kit/EIP123_SL/src/eip123_dma.c

endif   # WITH_CM_HW2

#----------------------------------------------------------------------------
# libtarget_versatile: Library for the Versatile FPGA target
#----------------------------------------------------------------------------

if ENABLE_VERSATILE

libtarget_versatile_a_CPPFLAGS = \
    $(CONFIGURATION_INCLUDES) \
    -I$(top_src)/Framework/PUBDEFS/incl \
    -I$(top_src)/Framework/IMPLDEFS/incl \
    -I$(top_src)/Framework/CLIB/incl \
    -I$(top_src)/Framework/SPAL_API/incl \
    -I$(top_src)/Integration/WorkerThread/incl \
    -I$(top_src)/Integration/OneTimeInit/incl \
    -I$(top_src)/Integration/InterruptDispatcher/incl \
    -I$(top_src)/Integration/DMARes_Record/incl \
    -I$(top_src)/Integration/UMDevXS/UserPart/incl \
    -I$(top_src)/Kit/DriverFramework/v4_safezone/Basic_Defs/incl \
    -I$(top_src)/Kit/DriverFramework/v4_safezone/CLib_Abstraction/incl \
    -I$(top_src)/Kit/DriverFramework/v4/Device_API/incl \
    -I$(top_src)/Kit/DriverFramework/v4/DMAResource_API/incl \
    -I$(top_src)/Kit/Log/incl \
    -I$(top_src)/Kit/Log/src/safezone

libtarget_versatile_a_SOURCES = \
    $(top_src)/Integration/OneTimeInit/src/sharedlibs_onetimeinit_cm.c \
    $(top_src)/Integration/DriverFramework_v4_impl/src/hwpal_dmares_umdevxs.c \
    $(top_src)/Integration/DriverFramework_v4_impl/src/hwpal_dmares_addr_cm_fpga.c \
    $(top_src)/Integration/DriverFramework_v4_impl/src/hwpal_device_umdevxs.c

if !ENABLE_POLLING

libtarget_versatile_a_CPPFLAGS += \
    -I$(top_src)/Kit/EIP201_SL/incl

libtarget_versatile_a_SOURCES += \
    $(top_src)/Integration/InterruptDispatcher/src/intdispatch_umdevxs.c \
    $(top_src)/Integration/WorkerThread/src/workerthread_spalv2.c \
    $(top_src)/Kit/EIP201_SL/src/eip201_sl.c

endif   # !ENABLE_POLLING

endif   # ENABLE_VERSATILE

#----------------------------------------------------------------------------
# libumdevxs: UMDevXS Proxy implementation
#----------------------------------------------------------------------------

libumdevxs_a_CPPFLAGS = \
    $(CONFIGURATION_INCLUDES) \
    -I$(top_src)/Framework/PUBDEFS/incl \
    -I$(top_src)/Framework/IMPLDEFS/incl \
    -I$(top_src)/Kit/DriverFramework/v4_safezone/Basic_Defs/incl \
    -I$(top_src)/Kit/Log/incl \
    -I$(top_src)/Kit/Log/src/safezone \
    -I$(top_src)/Integration/UMDevXS/UserPart/incl \
    -I$(top_src)/Integration/UMDevXS/KernelPart/incl

libumdevxs_a_SOURCES = \
    $(top_src)/Integration/UMDevXS/UserPart/src/umdevxsproxy.c

#----------------------------------------------------------------------------
# Composite Libraries
#----------------------------------------------------------------------------

CAL_LIBS=

if WITH_CM_HW1
CAL_LIBS += libcal_cm_v1.a
endif

if WITH_CM_HW2
CAL_LIBS += libcal_cm_v2.a
endif

CAL_LIBS += $(CAL_SIM_LIBS)
CAL_LIBS += libcal_hw.a

if ENABLE_VERSATILE
CAL_LIBS += \
    libtarget_versatile.a \
    libumdevxs.a
endif

CAL_LIBS += \
    libfmwk.a

libcal.a: $(CAL_LIBS)
	mkdir $@_work;\
	for pkg in $^;do cd $@_work;ar x ../$$pkg;cd ..;done
	ar crs $@ $@_work/""*
	rm -rf $@_work

#----------------------------------------------------------------------------
# configuration script
#----------------------------------------------------------------------------

@safezone_config_name@: safezone-config-template
	cp $< $@
	chmod 755 $@

#----------------------------------------------------------------------------
# Clean up
#----------------------------------------------------------------------------

distclean-local:
	rm -rf *.gcno *.gcda install-sh missing depcomp configure aclocal.m4 \
               Makefile.in config.guess config.sub autom4te.cache

# end of file Makefile.am
