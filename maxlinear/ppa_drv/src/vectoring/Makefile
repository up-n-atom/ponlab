##################################################################################
#                                                                                #
#       Copyright © 2020 MaxLinear, Inc.                                         #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################

obj-$(CONFIG_PTM_VECTORING)	+= ifxmips_vectoring.o
obj-y += ifxmips_vectoring_stub.o
ifeq ($(CONFIG_DSL_MEI_CPE_DRV),)
obj-$(CONFIG_PTM_VECTORING)	+= ifxmips_vectoring_test.o
endif
