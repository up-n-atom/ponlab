/******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

#include <asm/types.h>

typedef unsigned int u32;
typedef unsigned long ulong;
typedef unsigned char u8;
typedef unsigned long long u64;

typedef long long      __kernel_loff_t;
typedef __kernel_loff_t                loff_t;
typedef unsigned char           u_char;
typedef unsigned short          u_short;
typedef unsigned int            u_int;
typedef unsigned long           u_long;


typedef unsigned short __u16; 
typedef unsigned int __u32;
typedef unsigned char __u8;
 
typedef  __u16           u_int16_t;
typedef  __u32           u_int32_t;
typedef  __u8            uint8_t;
typedef  __u32           uint32_t;
#ifdef ARCH_MIPS
typedef  __u64           uint64_t;
#endif

typedef u64 phys_addr_t;
typedef u64 phys_size_t;
