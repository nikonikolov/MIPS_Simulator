#ifndef mips_cpu_ins_set_h
#define mips_cpu_ins_set_h

#include "mips.h"

void jal(mips_cpu_h state, uint32_t InsWord);
void j(mips_cpu_h state, uint32_t InsWord);

#endif