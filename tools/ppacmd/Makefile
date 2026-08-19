#
# Makefile for ppacmd
#

PPA_CFG_FLAG:=$(CFG_FLAG)
PPA_CFG_FLAG +=-DCONFIG_PPA -DCONFIG_PPA_QOS 
PPA_CFG_FLAG +=-DCONFIG_IPV6
PPA_CFG_FLAG += $(if $(PLATFORM_XRX500),-DCONFIG_SOC_GRX500)
PPA_CFG_FLAG += $(if $(PLATFORM_PUMA7),-DCONFIG_PPA_PUMA7 -DCONFIG_PPA -DCONFIG_PPA_PUMA_HAL)
PPA_CFG_FLAG +=-DCONFIG_PPA_API_SW_FASTPATH

CFLAGS += -O2 $(PPA_CFG_FLAG) -I./ -I$(LINUX_DIR)/user_headers/include -I$(STAGING_DIR)/usr/include

LDFLAGS += -L$(STAGING_DIR)/usr/lib/ -lsafec-1.0

all: ppacmd

ppacmd.o: ppacmd.c
#ppacmd_autotest.o: ppacmd_autotest.c

#ppacmd: ppacmd.o

ppacmd: ppacmd_autotest.o ppacmd.o

clean:
	rm -f *.o ppacmd
