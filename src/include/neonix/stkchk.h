#ifndef __NEONIX_STKCHK_H
#define __NEONIX_STKCHK_H

#include <stdint.h>

void __stack_chk_guard_init(uint64_t rand);

#endif
