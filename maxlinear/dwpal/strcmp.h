/******************************************************************************

         Copyright (c) 2020 - 2025, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/
#ifndef __STRCMP_H__
#define __STRCMP_H__

/* Workaround for strcmp()/strncmp()
	These functions are not banned, but they can be implemented in C RTL as system macros
	that use "banned" functions internally.

	This header must be include BEFORE defintion of banned functions
*/

#ifdef strcmp

static inline int __strcmp__(char const *s1, char const *s2) {
	return strcmp(s1, s2);
}

#undef strcmp
#define strcmp(s1, s2)  __strcmp__((s1), (s2))
#endif // strcmp

/* --------------------------- */

#ifdef strncmp

static inline int __strncmp__(char const *s1, char const *s2, size_t num) {
    return strncmp(s1, s2, num);
}

#undef strncmp
#define strncmp(s1, s2, num)  __strncmp__((s1), (s2), (num))
#endif // strncmp

#endif // __STRCMP_H__
