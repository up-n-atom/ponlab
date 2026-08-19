#ifndef _DRV_MPS_VERSION_H
#define _DRV_MPS_VERSION_H

/******************************************************************************

               Copyright (c) 2017 - 2019 Intel Corporation
               Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG


  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#define MAJORSTEP    2
#define MINORSTEP    0
#define VERSIONSTEP  0
#define VERS_TYPE    0

#if defined(SYSTEM_GRX500) 
   #define MPS_PLATFORM_NAME "MIPSInterAptiv"
#elif defined(SYSTEM_4KEC)
   #define MPS_PLATFORM_NAME "MIPS4KEC"
#elif defined(SYSTEM_VR9)
   #define MPS_PLATFORM_NAME "MIPS34KC"
#elif defined(SYSTEM_ATOM)
   #define MPS_PLATFORM_NAME "LGM"
#else
   #error unknown system
#endif /*SYSTEM_... */

#define MPS_INFO_STR \
"Intel " MPS_PLATFORM_NAME " MPS driver, version "

#endif /* _DRV_MPS_VERSION_H */
