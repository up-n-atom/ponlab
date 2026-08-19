# libwlan source Makefile
PKG_NAME := libspal

bins := libspal_utils.so libspal.so libspal_static.a

# dynamic libraries
libspal_utils.so_sources := src/uci_tools.c
libspal_utils.so_cflags := -I./src/include `pkg-config --cflags libnl-3.0`
libspal_utils.so_ldflags := -L./ -L./src/

libspal.so_sources := src/uci_wrapper.c src/uci_tools.c
libspal.so_cflags := -I./src/include `pkg-config --cflags libnl-3.0`
libspal.so_ldflags := -L./ -L./src/ -luci -lpthread

# static library
libspal_static.a_sources := src/uci_wrapper.c src/uci_tools.c
libspal_static.a_cflags := -I./src/include `pkg-config --cflags libnl-3.0` -DWAVEAPI_USE_EXTERN_LOG
libspal_static.a_ldflags := -L./ -L./src/ -luci -lpthread

include make.inc
