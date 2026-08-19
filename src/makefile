##################################################################################
#                                                                                #
#       Copyright (c) 2023 - 2024, MaxLinear, Inc.                               #
#                                                                                #
#  This software may be distributed under the terms of the BSD license.          #
#  See README for more details.                                                  #
##################################################################################
include ../makefile.inc

SRCDIR = .
INCDIR_PRIV = ../include_priv/
INCDIRS = $(INCDIR_PRIV) $(if $(STAGINGDIR), $(STAGINGDIR)/include) $(if $(STAGINGDIR), $(STAGINGDIR)/usr/include) $(if $(STAGINGDIR), $(STAGINGDIR)/usr/include/libnl3)
STAGING_LIBDIR = $(if $(STAGINGDIR), -L$(STAGINGDIR)/lib) $(if $(STAGINGDIR), -L$(STAGINGDIR)/usr/lib) $(if $(STAGINGDIR), -L$(STAGINGDIR)/usr/lib/amx/wld)

CFLAGS += -std=gnu99 -fPIC -g -Wall -Werror -Wextra -Wno-unused-but-set-variable -I$(STAGINGDIR)/include/ -I$(STAGINGDIR)/usr/include/
CFLAGS += -Wimplicit-function-declaration -Wl,--no-undefined
CFLAGS += -I. \
          $(addprefix -I ,$(INCDIRS)) \
          -DSAHTRACES_ENABLED -DSAHTRACES_LEVEL_DEFAULT=500

# CFLAGS += -I$(DWPAL_6X_DIR)/include/

CFLAGS += -I$(DRV_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/wireless/driver/ \
          -I$(DRV_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/wireless/shared_iwlwav-tools/ \
          -I$(DRV_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/wireless/shared_mbss_mac/ \
          -I$(DRV_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/wireless/driver/shared/

ifneq ($(CONFIG_MOD_WHM_CSI_SOCKET_PATH),)
CFLAGS += -DCONFIG_MOD_WHM_CSI_SOCKET_PATH=\"$(CONFIG_MOD_WHM_CSI_SOCKET_PATH)\"
else
CFLAGS += -DCONFIG_MOD_WHM_CSI_SOCKET_PATH=\"/var/run/whm-csi.sock\"
endif

LDFLAGS += $(STAGING_LIBDIR) -lsahtrace -lswlc -lnl-3 -lnl-genl-3
LDFLAGS += -Wl,-rpath,/lib -Wl,-rpath,/usr/lib -Wl,-rpath,/usr/lib/amx/wld
LDFLAGS += -lsahtrace -lswlc -lwld
LDFLAGS += -lamxc -lamxm -lamxp -lamxd -lamxb -lamxo

ifneq ($(CONFIG_MXL_WLAN_OSS_BUILD),y)
CFLAGS += -DCONFIG_VENDOR_MXL_PROPRIETARY
endif


OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c */*.c */*/*.c))

all: $(COMPONENT).so

$(COMPONENT).so: $(OBJECTS)
	$(CC) -Wl,-soname,$(@) -shared -fPIC -o $(@) $(OBJECTS) $(LDFLAGS) $(LDFLAGS-$@)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $(@) $(<)
	@$(CC) $(CFLAGS) -MM -MP -MT '$(@) $(@:.o=.d)' -MF $(@:.o=.d) $(<) >/dev/null

clean:
	rm -f $(COMPONENT).so *.o *.d *.a *.gcno *.gcda
	find . -name *.o -delete
	find . -name *.d -delete

-include $(OBJECTS:.o=.d)

.PHONY: all clean
