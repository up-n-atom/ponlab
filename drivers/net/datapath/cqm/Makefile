#
# Makefile for CBM driver.
#
ccflags-y += -I$(srctree)/$(src)/lgm

obj-$(CONFIG_MXL_CBM) +=  cqm_dev.o cqm_common.o cbm_wrapper.o
ifneq ($(CONFIG_LGM_CQM),)
obj-$(CONFIG_MXL_CBM) += lgm/
endif

obj-$(CONFIG_MXL_CBM_TEST) += test_cqm/
