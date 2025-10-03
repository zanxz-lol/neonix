#ifndef __NAHO_COMPILER_H
#define __NAHO_COMPILER_H

#include <stdint.h>

#define __noreturn __attribute((noreturn))
#define __packed __attribute((packed))
#define __unused __attribute((unused))
#define __used   __attribute((used))
#define __section(name) __attribute((section(name)))
#define __align(bytes) __attribute((aligned(bytes)))
#define __hot __attribute((hot))
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)
#define __externally_visible __attribute((externally_visible))
#else 
#define ____externally_visible
#endif
#define __nostackcheck __attribute((__optimize__("-fno-stack-protector")))

#endif
