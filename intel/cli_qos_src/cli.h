/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

typedef int32_t (*pfn_func_t)(int32_t, char **);
  
struct name2func { 
  const char *pcname;
  pfn_func_t pfnfunc;
};


