##################################################################################
#                                                                                #
#       Copyright © 2020 MaxLinear, Inc.                                         #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################

ifeq ($(CONFIG_PRX300_CQM),y)
	obj-$(CONFIG_SOC_GRX500)	+= prx300/
else
	obj-$(CONFIG_SOC_GRX500)	+= xrx500/
endif
ifeq ($(CONFIG_SOC_LGM),y)
        obj-$(CONFIG_SOC_LGM) += lgm/
else
        obj-$(CONFIG_X86_INTEL_LGM) += lgm/
endif
