
/* Header for CPU internal functionality */ 

#ifndef mips_private_h
#define mips_private_h


#include "mips.h"	// required for var types
#include "mips_cpu_def.h"

uint32_t change_endian(uint32_t word);

mips_error advance_pc (mips_cpu_h state, uint32_t offset);

#endif