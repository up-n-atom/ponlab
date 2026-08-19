# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the DPDP Driver
#
# Copyright (c) 2023 MaxLinear, Inc.

obj-y += directpath_hook.o

obj-$(CONFIG_DP_DIRECTPATH) += directpath_dp.o
directpath_dp-objs := directpath_api.o directpath_debugfs.o

obj-$(CONFIG_DP_DIRECTPATH_TEST) += directpath_loopback.o
