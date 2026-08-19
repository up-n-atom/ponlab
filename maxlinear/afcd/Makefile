CC = gcc
CFLAGS = -Wall -Wextra -I./ -I./eloop/ -I./config/ -I./drivers/ -I./https/ -I./utils/ -I./ctrl/ -I./json/
LDFLAGS = -lcurl -lcjson

ifeq ($(NO_PKG_CONFIG),)
NL3xFOUND := $(shell $(PKG_CONFIG) --atleast-version=3.2 libnl-3.0 && echo Y)
ifneq ($(NL3xFOUND),Y)
NL31FOUND := $(shell $(PKG_CONFIG) --exact-version=3.1 libnl-3.1 && echo Y)
ifneq ($(NL31FOUND),Y)
NL3FOUND := $(shell $(PKG_CONFIG) --atleast-version=3 libnl-3.0 && echo Y)
ifneq ($(NL3FOUND),Y)
NL2FOUND := $(shell $(PKG_CONFIG) --atleast-version=2 libnl-2.0 && echo Y)
ifneq ($(NL2FOUND),Y)
NL1FOUND := $(shell $(PKG_CONFIG) --atleast-version=1 libnl-1 && echo Y)
endif
endif
endif
endif

ifeq ($(NL1FOUND),Y)
NLLIBNAME = libnl-1
endif

ifeq ($(NL2FOUND),Y)
CFLAGS += -DCONFIG_LIBNL20
LIBS += -lnl-genl -lnl-2
NLLIBNAME = libnl-2.0
endif

ifeq ($(NL3xFOUND),Y)
# libnl 3.2 might be found as 3.2 and 3.0
NL3FOUND = N
CFLAGS += -DCONFIG_LIBNL30
LIBS += -lnl-genl-3 -lnl-3
NLLIBNAME = libnl-3.0
endif

ifeq ($(NL3FOUND),Y)
CFLAGS += -DCONFIG_LIBNL30
LIBS += -lnl-genl -lnl-3
NLLIBNAME = libnl-3.0
endif

ifeq ($(NL31FOUND),Y)
CFLAGS += -DCONFIG_LIBNL30
LIBS += -lnl-genl -lnl-3
NLLIBNAME = libnl-3.1
endif

ifeq ($(NLLIBNAME),)
$(error Cannot find development files for any supported version of libnl)
endif

CFLAGS += $(shell $(PKG_CONFIG) --cflags $(NLLIBNAME)) $(IFX_CFLAGS)
LDFLAGS = $(IFX_LDFLAGS)
endif # NO_PKG_CONFIG

UTILS_DIR = utils
HTTPS_DIR = https
CONFIG_DIR = config
ELOOP_DIR = eloop
DRV_DIR = drivers
CTRL_DIR = ctrl
JSON_DIR = json

SRC_FILES = main.c afc.c $(UTILS_DIR)/utils.c $(JSON_DIR)/json.c $(HTTPS_DIR)/lib_curl.c $(CONFIG_DIR)/config_file.c $(ELOOP_DIR)/eloop.c $(DRV_DIR)/afc_nl80211.c $(DRV_DIR)/afc_reg_rule.c $(UTILS_DIR)/afc_debug.c $(CTRL_DIR)/ctrl.c
HEADER_FILES = afc.h $(UTILS_DIR)/utils.h $(HTTPS_DIR)/lib_curl.h $(CONFIG_DIR)/config_file.h $(ELOOP_DIR)/eloop.h $(ELOOP_DIR)/list.h $(DRV_DIR)/nl80211.h $(DRV_DIR)/vendor_cmds_copy.h $(DRV_DIR)/afc_nl80211.h $(DRV_DIR)/afc_reg_rule.h $(UTILS_DIR)/afc_debug.h $(CTRL_DIR)/ctrl.h $(JSON_DIR)/json.h

CLI_SRC_FILES = afc_cli.c $(UTILS_DIR)/afc_debug.c $(CTRL_DIR)/ctrl.c $(CTRL_DIR)/process.c $(ELOOP_DIR)/eloop.c $(UTILS_DIR)/utils.c
CLI_HEADER_FILES = afc.h $(UTILS_DIR)/afc_debug.h $(CTRL_DIR)/ctrl.h $(ELOOP_DIR)/eloop.h

OBJS = $(SRC_FILES:.c=.o)
OBJS_C = $(CLI_SRC_FILES:.c=.o)

TARGET = afcd
CLI_TARGET = afcd_cli

all: $(TARGET) $(CLI_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o afcd $(LDFLAGS) $(LIBS)

$(CLI_TARGET): $(OBJS_C)
	$(CC) $(CFLAGS) $(OBJS_C) -o afcd_cli $(LDFLAGS) $(LIBS)

%.o: %.c $(HEADER_FILES) $(CLI_HEADER_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
	rm -f $(CLI_TARGET) $(OBJS_C)

.PHONY: all clean