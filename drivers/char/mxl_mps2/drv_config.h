/* drv_config.h.in.  Generated from configure.in by autoheader.  */

/******************************************************************************

		Copyright (c) 2017 - 2019 Intel Corporation
		Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#ifndef _DRV_CONFIG_H_
#define _DRV_CONFIG_H_
/*enable vpe0 configuration*/
#ifdef CONFIG_SOC_TYPE_GRX500_TEP
	#define SYSTEM_4KEC
	#define VPE0
#elif defined(CONFIG_SOC_GRX500)
	#define SYSTEM_GRX500
	#define FEAT_LINUX_PLATFORM_DRIVER
#else /* LGM Config and cleanup VR9 as it is not supported */
	#define SYSTEM_ATOM
	#define VPE1
	#define FEAT_LINUX_PLATFORM_DRIVER
#endif

#endif /* _DRV_CONFIG_H_ */

