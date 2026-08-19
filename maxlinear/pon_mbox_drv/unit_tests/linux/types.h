#ifdef LINUX
#include_next <linux/types.h>
#else
typedef unsigned char __u8;
typedef signed char __s8;
typedef unsigned short __u16;
typedef signed short __s16;
typedef unsigned int __u32;
typedef signed int __s32;
#ifdef _WIN64
typedef unsigned long __u64;
typedef signed long __s64;
#else
typedef unsigned long long __u64;
typedef signed long long __s64;
#endif
#include "../fake_linux.hpp"
#endif
